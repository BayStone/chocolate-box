#pragma once

#include <d3dx9.h>
#include <d3d9.h>

using namespace System;
using namespace System::IO;

using namespace FableMod::BIG;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

public enum class TextureFileType : unsigned __int32
{

	BMP = 0,
	JPG = 1,
	TGA = 2,
	PNG = 3,
	DDS = 4,
	PPM = 5,
	DIB = 6,
	HDR = 7,
	PFM = 8
};

public enum class TextureFormat : unsigned __int32
{
	DXT1 = D3DFMT_DXT1,
	DXT3 = D3DFMT_DXT3,
};

#pragma pack(push,1)

struct TextureSubHeader 
{
	WORD	Width;
	WORD	Height;
	WORD	VolumeDepth;
	WORD	FrameWidth;
	WORD	FrameHeight;
	WORD	FrameCount;
	WORD	DXT_Compression;
	WORD	Unk0;
	BYTE	NumberAlpha;
	BYTE	NumberMips;
	WORD	Unk1;
	ULONG	TopMipMapSize;
	ULONG	TopMipMapCompressedSize;
	WORD	Unk2; //DXT again?
	ULONG	Unk3;
};
	
#pragma pack(pop)

public ref class GfxTexture : public IDisposable
{
public:
	
	GfxTexture(AssetEntry^ entry);
	
	GfxTexture(String^ filename, TextureFormat format);

	virtual ~GfxTexture();

	!GfxTexture();

	void Save(String^ filename, int frameIndex);

	void ApplyToEntry(AssetEntry^ entry);

	void AddFrame(String^ filename);
	
	void Draw(System::Windows::Forms::Control^ control, 
			  System::Drawing::Rectangle rect, 
			  int FrameIndex);

	property int Width { int get(); }

	property int Height { int get(); }
	
	property int Depth { int get(); }

	property int FrameCount { int get(); }

	property int FrameWidth 
	{ 
		int get(); 
		void set(int value);
	}

	property int FrameHeight 
	{ 
		int get(); 
		void set(int value);
	}

	property Byte AlphaChannels 
	{ 
		Byte get();
		void set(Byte value);
	}
	
	property TextureFormat Format
	{
		TextureFormat get();
	}

	FableMod::Gfx::Texture* GetBaseTexture();

	LPDIRECT3DTEXTURE9 GetBaseD3DTexture();

	static String^ SAVE_FILE_FILTER =
		"BMP Files (*.bmp)|*.bmp|"+
		"JPG Files (*.jpg)|*.jpg|"+
		"TGA Files (*.tga)|*.tga|"+
		"PNG Files (*.png)|*.png|"+
		"DDS Files (*.dds)|*.dds||";

	static String^ LOAD_FILE_FILTER =
		"All Image Files|*.bmp;*.jpg;*.tga;*.png;*.dds|"+
		SAVE_FILE_FILTER;		

protected:
	
	ID3DXSprite* m_ISprite;
	IDirect3DTexture9**	m_ITextures;
	TextureSubHeader* m_SubHeader;
	::FableMod::Gfx::TexturePtr* m_pBaseTexture;
};

}
}
}