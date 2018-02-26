#include "Stdafx.h"
#include "GfxTexture.h"
#include <d3dx9tex.h>

using namespace System::Runtime::InteropServices;

//Custom re-define of this struct since it has a pointer in it 
// which makes it wrong size for 64bit builds
// which, though working for runtime, screws up file format 
// which is defined to 32bit pointers

#include <ddraw.h>

typedef struct _DDSURFACEDESC2_32
{
    DWORD               dwSize;                 // size of the DDSURFACEDESC structure
    DWORD               dwFlags;                // determines what fields are valid
    DWORD               dwHeight;               // height of surface to be created
    DWORD               dwWidth;                // width of input surface
    union
    {
        LONG            lPitch;                 // distance to start of next line (return value only)
        DWORD           dwLinearSize;           // Formless late-allocated optimized surface size
    } DUMMYUNIONNAMEN(1);
    union
    {
        DWORD           dwBackBufferCount;      // number of back buffers requested
        DWORD           dwDepth;                // the depth if this is a volume texture 
    } DUMMYUNIONNAMEN(5);
    union
    {
        DWORD           dwMipMapCount;          // number of mip-map levels requestde
                                                // dwZBufferBitDepth removed, use ddpfPixelFormat one instead
        DWORD           dwRefreshRate;          // refresh rate (used when display mode is described)
        DWORD           dwSrcVBHandle;          // The source used in VB::Optimize
    } DUMMYUNIONNAMEN(2);
    DWORD               dwAlphaBitDepth;        // depth of alpha buffer requested
    DWORD               dwReserved;             // reserved
    DWORD              lpSurface;              // pointer to the associated surface memory
    union
    {
        DDCOLORKEY      ddckCKDestOverlay;      // color key for destination overlay use
        DWORD           dwEmptyFaceColor;       // Physical color for empty cubemap faces
    } DUMMYUNIONNAMEN(3);
    DDCOLORKEY          ddckCKDestBlt;          // color key for destination blt use
    DDCOLORKEY          ddckCKSrcOverlay;       // color key for source overlay use
    DDCOLORKEY          ddckCKSrcBlt;           // color key for source blt use
    union
    {
        DDPIXELFORMAT   ddpfPixelFormat;        // pixel format description of the surface
        DWORD           dwFVF;                  // vertex format description of vertex buffers
    } DUMMYUNIONNAMEN(4);
    DDSCAPS2            ddsCaps;                // direct draw surface capabilities
    DWORD               dwTextureStage;         // stage in multitexture cascade
} DDSURFACEDESC2_32;

using namespace System::IO;
using namespace FableMod::CLRCore;
using namespace FableMod::Data;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

GfxTexture::GfxTexture(AssetEntry^ entry) :
m_pBaseTexture(0),
m_ITextures(0),
m_SubHeader(0),
m_ISprite(0)
{
	int sizeoftexture = sizeof(TextureSubHeader);
	
	if (entry->GetSubHeaderLength() != sizeof(TextureSubHeader))
	{
		throw gcnew System::Exception(
			"FableMod::Gfx::Integration: Invalid subheader length");
	}

	m_SubHeader = new TextureSubHeader;

	memcpy(m_SubHeader, entry->GetSubHeader(), sizeof(TextureSubHeader));
	
	unsigned int size = 0;
	
	for (int i = 0; i < m_SubHeader->NumberMips; ++i)
		size += m_SubHeader->TopMipMapSize/(pow(4.0,i));

	// temp buffer sized to match dds format
	char* data = new char[128+(size*m_SubHeader->FrameCount)]; 
	
	// do fourcc
	*(DWORD*)data = MAKEFOURCC('D', 'D', 'S', ' ');
	DDSURFACEDESC2_32* d = (DDSURFACEDESC2_32*)(data+4);
	ZeroMemory(d, sizeof(*d));
	d->dwSize = sizeof(*d);
	d->dwFlags = 
		DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS | DDSD_PIXELFORMAT | 
		DDSD_LINEARSIZE | DDSD_MIPMAPCOUNT;
	d->dwHeight = m_SubHeader->Height;
	d->dwWidth = m_SubHeader->Width;
	d->dwDepth = m_SubHeader->VolumeDepth;
	d->dwMipMapCount = m_SubHeader->NumberMips;
	d->dwLinearSize = m_SubHeader->TopMipMapSize;
	d->ddpfPixelFormat.dwSize = sizeof(d->ddpfPixelFormat);
	d->ddpfPixelFormat.dwFlags = DDPF_FOURCC;
	
	if (m_SubHeader->DXT_Compression == 0x1F)
	{
		d->ddpfPixelFormat.dwFourCC = D3DFMT_DXT1;
	}
	/*
	else if (m_SubHeader->DXT_Compression == 0x1)
	{
		d->ddpfPixelFormat.dwFourCC = D3DFMT_DXT1;
	}
	*/
	else if (m_SubHeader->DXT_Compression == 0x20 || 
			 m_SubHeader->DXT_Compression == 0x23)
	{
		d->ddpfPixelFormat.dwFourCC = D3DFMT_DXT3;
	}
	else
	{
		throw gcnew System::Exception(String::Format(
			"Invalid texture format: {0:X2}",
			m_SubHeader->DXT_Compression));
	}

	d->ddsCaps.dwCaps = 
		DDSCAPS_TEXTURE | DDSCAPS_MIPMAP | DDSCAPS_COMPLEX;
	d->ddsCaps.dwCaps2 = DDSCAPS2_VOLUME; 
	
	m_ITextures = new IDirect3DTexture9*[m_SubHeader->FrameCount];
	
	memset(m_ITextures, 0, sizeof(m_ITextures[0])*m_SubHeader->FrameCount);

	IDirect3DDevice9* pD3DDevice = 
		::FableMod::Gfx::Manager::GetDevice()->GetD3DDevice();
	
	unsigned int offset = 0;

	for (int i = 0; i < m_SubHeader->FrameCount; ++i)
	{
		if (m_SubHeader->TopMipMapCompressedSize != 0)
		{
			unsigned int outlen = m_SubHeader->TopMipMapSize;
			
			LZO::DecompressChunk(
				(unsigned char*)entry->GetData()+offset, 
				m_SubHeader->TopMipMapCompressedSize, 
				(unsigned char*)data+128, &outlen);
			
			memcpy(
				data+128+m_SubHeader->TopMipMapSize, 
				entry->GetData()+offset+
					m_SubHeader->TopMipMapCompressedSize, 
				size-m_SubHeader->TopMipMapSize);
			
			// move offset to next frame for next iteration
			offset += size-m_SubHeader->TopMipMapSize+
				m_SubHeader->TopMipMapCompressedSize;
		} 
		else
		{
			memcpy(data+128, entry->GetData()+offset, size);
		
			// move offset to next frame for next iteration
			offset += size;
		}

		if (FAILED(D3DXCreateTextureFromFileInMemory(
			pD3DDevice, data, size+128, &m_ITextures[i])))
		{
			throw gcnew System::Exception(
				"FableMod::Gfx::Integration: Failed to create texture");
		}
	}

	// Delete the temp data.
	delete[] data;
	
	ID3DXSprite* pSprite = 0;

	if (FAILED(D3DXCreateSprite(pD3DDevice, &pSprite)))
	{
		throw gcnew System::Exception(
			"FableMod::Gfx::Integration: Failed to create sprite");	
	}

	m_ISprite = pSprite;

	// Set up the texture for the 3d editor.
	m_pBaseTexture = new FableMod::Gfx::TexturePtr(
		new FableMod::Gfx::Texture());

	(*m_pBaseTexture)->SetAlphaRequired(AlphaChannels > 0);
	(*m_pBaseTexture)->SetD3DTexture(m_ITextures[0]);

	System::IntPtr ptr = 
		Marshal::StringToHGlobalUni(entry->DevSymbolName);
	(*m_pBaseTexture)->SetName((LPCTSTR)ptr.ToPointer());
	Marshal::FreeHGlobal(ptr);

	entry->Purge();
}

GfxTexture::~GfxTexture()
{
	this->!GfxTexture();
}

GfxTexture::!GfxTexture()
{
	if (m_ITextures)
	{
		for (int i = 0; i < m_SubHeader->FrameCount; ++i)
		{
			GFX_SAFE_RELEASE(m_ITextures[i]);
		}
		
		delete[] m_ITextures;
	}

	if (m_SubHeader)
		delete[] m_SubHeader;
	
	GFX_SAFE_RELEASE(m_ISprite);

	GFX_SAFE_DELETE(m_pBaseTexture);
}

GfxTexture::GfxTexture(String^ filename, TextureFormat format)
{
	FileStream^ f = File::OpenRead(filename);
	
	if (!f->CanRead)
		throw gcnew System::Exception("failed to read file");

	const unsigned int uiLength = f->Length;

	char* data = new char[uiLength];

	FileControl::Read(f, data, uiLength);
	
	f->Close();

	m_SubHeader = new TextureSubHeader;

	ZeroMemory(m_SubHeader, sizeof(TextureSubHeader));
	
	m_SubHeader->FrameCount = 1;
	
	if (format == TextureFormat::DXT1)
	{
		m_SubHeader->DXT_Compression = 0x1F;
		m_SubHeader->Unk2 = 0x0403;
		m_SubHeader->Unk3 = 0x0;
	    m_SubHeader->NumberAlpha = 0;
	}
	else if (format == TextureFormat::DXT3)
	{
		m_SubHeader->DXT_Compression = 0x20;
		m_SubHeader->Unk2 = 0x0802;
		m_SubHeader->Unk3 = 0x0;
        
        // HERE!?
	    m_SubHeader->NumberAlpha = 1;
	}

	m_ITextures = new IDirect3DTexture9*[m_SubHeader->FrameCount];

	IDirect3DDevice9* pD3DDevice = 
		::FableMod::Gfx::Manager::GetDevice()->GetD3DDevice();

	LPDIRECT3DTEXTURE9 pTexture = 0;
	
	if (FAILED(D3DXCreateTextureFromFileInMemoryEx(
		pD3DDevice, 
		data, 
		uiLength,
		0,0,0,0,
		(D3DFORMAT)format,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		NULL,
		NULL,
		&pTexture)))
	{
		throw gcnew System::Exception("failed to create texture");
	}

	m_ITextures[0] = pTexture;
	
	D3DSURFACE_DESC d;

	m_ITextures[0]->GetLevelDesc(0,&d);
	
	m_SubHeader->Height = m_SubHeader->FrameHeight = d.Height;
	m_SubHeader->Width = m_SubHeader->FrameWidth = d.Width;
	m_SubHeader->VolumeDepth = 0;
    
	LPD3DXSPRITE pSprite = 0;

	if (FAILED(D3DXCreateSprite(pD3DDevice, &pSprite)))
		throw gcnew System::Exception("failed to create sprite");
	
	m_ISprite = pSprite;

	delete[] data;
}

void GfxTexture::Save(String^ fileName, 
					  int frameIndex)
{
	if (frameIndex >= m_SubHeader->FrameCount || frameIndex < 0)
		frameIndex = 0;

	IntPtr pfilename = Marshal::StringToHGlobalUni(fileName);
	
	String^ ext = Path::GetExtension(fileName)->ToUpper();

	D3DXIMAGE_FILEFORMAT format = D3DXIFF_JPG;

	if (ext == ".BMP")
		format = D3DXIFF_BMP;
	else if (ext == ".PNG")
		format = D3DXIFF_PNG;
	else if (ext == ".TGA")
		format = D3DXIFF_TGA;
	else if (ext == ".DDS")
		format = D3DXIFF_DDS;

	D3DXSaveTextureToFile(
		(TCHAR*)pfilename.ToPointer(), 
		format, 
		m_ITextures[frameIndex], NULL);
	
	Marshal::FreeHGlobal(pfilename);
}

void GfxTexture::ApplyToEntry(AssetEntry^ entry)
{
	if (m_SubHeader->DXT_Compression == 0x20)
		m_SubHeader->TopMipMapSize = 
			m_SubHeader->Width*m_SubHeader->Height;
	else if (m_SubHeader->DXT_Compression == 0x1F)
		m_SubHeader->TopMipMapSize = 
			(m_SubHeader->Width*m_SubHeader->Height)/2;
	else
		throw gcnew InvalidOperationException("Invalid DXT");

	int tmp;

	if (m_SubHeader->Width > m_SubHeader->Height)
		tmp = m_SubHeader->Height;
	else
		tmp = m_SubHeader->Width;

	int p;

	for (p = 1; tmp > 4; ++p)
		tmp = tmp >> 1;

	m_SubHeader->NumberMips = p;

	tmp = 0;

	for (int i = 0; i < p; ++i)
		tmp += m_SubHeader->TopMipMapSize/(pow(4.0,i));

	ID3DXBuffer* pBuffer = 0;

	if (m_SubHeader->FrameCount == 1)
	{
		m_ITextures[0]->GenerateMipSubLevels();
		
		HRESULT ret = D3DXSaveTextureToFileInMemory(
			&pBuffer, 
			(D3DXIMAGE_FILEFORMAT)TextureFileType::DDS, 
			m_ITextures[0], 
			NULL);
		
		unsigned char* data = (unsigned char*)pBuffer->GetBufferPointer();
		unsigned int forfun = pBuffer->GetBufferSize();
		unsigned int length = tmp+128;
		unsigned char* out = new unsigned char[length];
		unsigned int outlen = length;

		if (m_SubHeader->DXT_Compression == 0x20)
			LZO::CompressChunk(data+128, m_SubHeader->TopMipMapSize, 
				out, &outlen, 3, true);
		else
			LZO::CompressChunk(data+128, m_SubHeader->TopMipMapSize, 
				out, &outlen, 3);

		m_SubHeader->TopMipMapCompressedSize = outlen;

		memcpy(
			out+outlen, 
			data+128+m_SubHeader->TopMipMapSize, 
			tmp-m_SubHeader->TopMipMapSize);
		
		outlen = tmp+outlen-m_SubHeader->TopMipMapSize;
		
		entry->SetData((char*)out, outlen);
		
		delete[] out;
		
		entry->SetSubHeader((char*)m_SubHeader, sizeof(*m_SubHeader));
		
		pBuffer->Release();
	}
	else
	{
		m_ITextures[0]->GenerateMipSubLevels();
		
		HRESULT ret = D3DXSaveTextureToFileInMemory(
			&pBuffer, 
			(D3DXIMAGE_FILEFORMAT)TextureFileType::DDS, 
			m_ITextures[0], 
			NULL);
		
		unsigned char* data = (unsigned char*)pBuffer->GetBufferPointer();
		unsigned int length = tmp;
		unsigned char* out = 
			new unsigned char[length*m_SubHeader->FrameCount];
		
		m_SubHeader->TopMipMapCompressedSize = 0;
		
		int offset = 0;
		
		memcpy(out+offset, data+128, length);
		
		pBuffer->Release();

		offset += length;

		for(int i = 1 ; i < m_SubHeader->FrameCount; ++i)
		{
			m_ITextures[i]->GenerateMipSubLevels();
			
			HRESULT ret = D3DXSaveTextureToFileInMemory(
				&pBuffer, 
				(D3DXIMAGE_FILEFORMAT)TextureFileType::DDS, 
				m_ITextures[i], 
				NULL);
			
			unsigned char* data = 
				(unsigned char*)pBuffer->GetBufferPointer();

			memcpy(out+offset, data+128, length);
			
			pBuffer->Release();
			
			offset+=length;
		}

		entry->SetData((char*)out, length*m_SubHeader->FrameCount);
		
		delete[] out;
		
		entry->SetSubHeader((char*)m_SubHeader, sizeof(*m_SubHeader));
	}
}

void GfxTexture::AddFrame(String^ filename)
{
	FileStream^ f = File::OpenRead(filename);

	if (!f->CanRead)
	{
		throw gcnew System::Exception(
			"FableMod::Gfx::Integration:: Failed to read file");
	}

	char* data = new char[f->Length];

	unsigned int uiLength = f->Length;

	FileControl::Read(f, data, uiLength);
	
	f->Close();
	
	TextureFormat format;

	if (m_SubHeader->DXT_Compression == 0x1f)
		format = TextureFormat::DXT1;
	else if (m_SubHeader->DXT_Compression = 0x20)
		format = TextureFormat::DXT3;

	IDirect3DDevice9* pD3DDevice = 
		::FableMod::Gfx::Manager::GetDevice()->GetD3DDevice();

	LPDIRECT3DTEXTURE9 pTexture = 0;
	
	if (FAILED(D3DXCreateTextureFromFileInMemoryEx(
		pD3DDevice, 
		data, uiLength, 
		0,0,0,0,
		(D3DFORMAT)format,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,NULL,NULL,
		&pTexture)))
	{
		delete[] data;

		throw gcnew System::Exception("Failed to create texture");
	}

	// Delete the raw data.
	delete[] data;
		
	D3DSURFACE_DESC desc;
	pTexture->GetLevelDesc(0,&desc);
	
	if (m_SubHeader->Height != desc.Height || 
		m_SubHeader->Width != desc.Width)
	{
		pTexture->Release();
		throw gcnew System::ArgumentException("Invalid texture size");
	}

	// So far so good, expand texture array.

	++m_SubHeader->FrameCount;
	
	IDirect3DTexture9** ppNewTextures = 
		new IDirect3DTexture9*[m_SubHeader->FrameCount];
	
	memcpy(
		ppNewTextures, 
		m_ITextures, 
		sizeof(IDirect3DTexture9*)*(m_SubHeader->FrameCount-1));
	
	delete[] m_ITextures;
	
	m_ITextures = ppNewTextures;

	m_ITextures[m_SubHeader->FrameCount-1] = pTexture;
}

void GfxTexture::Draw(System::Windows::Forms::Control^ control, 
					  System::Drawing::Rectangle destination, 
					  int FrameIndex)
{
	::FableMod::Gfx::Device* pDevice = 
		::FableMod::Gfx::Manager::GetDevice();

	pDevice->Lock();

	pDevice->BeginScene();
	
	pDevice->ClearBuffers(D3DCOLOR_XRGB(255,0,255));

	if (FrameIndex >= m_SubHeader->FrameCount || FrameIndex < 0)
		FrameIndex = 0;
	
	D3DSURFACE_DESC desc;
	
	m_ITextures[FrameIndex]->GetLevelDesc(0,&desc);

	FLOAT fWidth = desc.Width;
	FLOAT fHeight = desc.Height;
	
	if (desc.Width > destination.Width || desc.Height > destination.Height)
	{
		FLOAT fW = destination.Width/(FLOAT)desc.Width;
		FLOAT fH = destination.Height/(FLOAT)desc.Height;

		if (fW > 1.0f)
			fW = 1.0f;

		if (fH > 1.0f)
			fH = 1.0f;

		FLOAT fRatio = (fW < fH) ? fW : fH;

		fWidth = desc.Width*fRatio;
		fHeight = desc.Height*fRatio;
	}

	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, 
		fWidth/desc.Width, fHeight/desc.Height, 1.0f);

	m_ISprite->SetTransform(&matScale);

	m_ISprite->Begin(D3DXSPRITE_ALPHABLEND);

	RECT s,d;
	
	s.left = 0;
	s.top = 0;
	s.right = destination.Width;
	s.bottom = destination.Height;

	d.left = destination.Left;
	d.top = destination.Top;
	d.right = destination.Width;
	d.bottom = destination.Height;

	m_ISprite->Draw(
		m_ITextures[FrameIndex], 
		NULL, 
		NULL, 
		NULL,  
		D3DCOLOR_RGBA(0xFF,0xFF,0xFF,0xFF));
	
	m_ISprite->End();
	
	pDevice->EndScene();

	pDevice->Present((HWND)control->Handle.ToPointer(), &s, &d);

	pDevice->Unlock();
}

int GfxTexture::Width::get()
{
	return m_SubHeader->Width;
}

int GfxTexture::Height::get()
{
	return m_SubHeader->Height;
}

int GfxTexture::Depth::get()
{
	return m_SubHeader->VolumeDepth;
}

int GfxTexture::FrameCount::get()
{
	return m_SubHeader->FrameCount;
}

int GfxTexture::FrameWidth::get()
{
	return m_SubHeader->FrameWidth;
}

int GfxTexture::FrameHeight::get()
{
	return m_SubHeader->FrameHeight;
}

void GfxTexture::FrameWidth::set(int width)
{
	if (width > 0)
		m_SubHeader->FrameWidth = width;
}

void GfxTexture::FrameHeight::set(int height)
{
	if (height > 0)
		m_SubHeader->FrameHeight = Height;
}

Byte GfxTexture::AlphaChannels::get()
{
	return m_SubHeader->NumberAlpha;
}

void GfxTexture::AlphaChannels::set(Byte chans)
{
	m_SubHeader->NumberAlpha = chans;
}

TextureFormat GfxTexture::Format::get()
{
	if (m_SubHeader->DXT_Compression == 0x1F)
		return TextureFormat::DXT1;
	//else if (m_SubHeader->DXT_Compression == 0x20)
	//	return TextureFormat::DXT3;
	else
		return TextureFormat::DXT3;
}

FableMod::Gfx::Texture* GfxTexture::GetBaseTexture()
{
	return *m_pBaseTexture;
}

LPDIRECT3DTEXTURE9 GfxTexture::GetBaseD3DTexture()
{
	return m_ITextures[0];
}

}
}
}