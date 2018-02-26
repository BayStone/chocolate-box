#include "Stdafx.h"
#include "GfxTagModel.h"
#include "Helpers.h"
#include "AllocateHierarchy.h"

using namespace FableMod::Data;
using namespace System::IO;
using namespace System::Runtime::InteropServices;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

//---------------------------------------------------------------------------

GfxTagModel::GfxTagModel(AssetEntry^ entry) :
GfxBaseModel(entry)
{
	array<System::Byte>^ data = entry->Data;
                
    if (data[data->Length-3] == 0x11 && 
        data[data->Length-2] == 0x00 && 
        data[data->Length-1] == 0x00)
    {
		array<System::Byte>^ decomp = LZO::DecompressRaw(
			data, 4, data->Length-4);

		pin_ptr<System::Byte> pIn = &(decomp[0]);

		BufferStream^ buffer = gcnew BufferStream(
			pIn, decomp->Length);

		ReadData(buffer);

		buffer->Close();
		buffer = nullptr;
	}
	else
	{
		BufferStream^ buffer = gcnew BufferStream(
			entry->GetData(), entry->Length);

		ReadData(buffer);

		buffer->Close();
		buffer = nullptr;
	}
}

GfxTagModel::~GfxTagModel()
{
}

bool GfxTagModel::ReadChunkInfo(
	BufferStream^ stream, SChunk* pChunk)
{
	if (stream->Tell() >= stream->GetSize()-8)
		return false;

	stream->Read(pChunk, sizeof(SChunk));
	
	return true;
}

void GfxTagModel::ReadData(BufferStream^ stream)
{
	/// Magic!
	stream->Ignore(4);

	// Read info chunk...

	SChunk Chunk;

	ReadChunkInfo(stream, &Chunk);

	if (Chunk == "3DMF")
	{
		System::String^ sign = stream->MReadZString();
		
		if (sign != "Copyright Big Blue Box Studios Ltd.")
			throw gcnew System::Exception(String::Format(
				"Invalid signature {0}", sign));
	}
	else
	{
		throw gcnew System::Exception("Invalid starting tag");
	}

	// Start reading top level chunks...
	while (ReadChunkInfo(stream, &Chunk))
	{
		if (Chunk == "3DRT")
		{
			Read3DRT(Chunk.dwSize, stream);		
		}
		else
			stream->Ignore(Chunk.dwSize);
	}
}

void GfxTagModel::Read3DRT(int iLength, BufferStream^ stream)
{
	CHUNK_BEGIN(stream, iLength);

	SChunk Chunk;

	m_pRoot = new D3DXEXTENDEDFRAME;
	m_pRoot->Name = CopyXFileSafeString("Scene Root");
	D3DXMatrixIdentity(&m_pRoot->TransformationMatrix);

	while (ReadChunkInfo(stream, &Chunk))
	{
		if (Chunk == "MTLS")
		{
			ReadMTLS(Chunk.dwSize, stream);		
		}
		else if (Chunk == "SUBM")
		{
			ReadSUBM(m_pRoot, Chunk.dwSize, stream);		
		}
		/*
		else if (Chunk == "HLPR")
		{
			ReadHLPR(m_pRoot, Chunk.dwSize, stream);		
		}
		*/
		else
			stream->Ignore(Chunk.dwSize);

		CHUNK_BREAK(stream);
	}

	CHUNK_END(stream);
}

void GfxTagModel::ReadMTLS(int iLength, BufferStream^ stream)
{
	CHUNK_BEGIN(stream, iLength);

	SChunk Chunk;

	while (ReadChunkInfo(stream, &Chunk))
	{
		if (Chunk == "MTRL")
		{
			ReadMTRL(Chunk.dwSize, stream);		
		}
		else if (Chunk == "MTLE")
		{
			ReadMTLE(Chunk.dwSize, stream);		
		}
		else
			stream->Ignore(Chunk.dwSize);

		CHUNK_BREAK(stream);	
	}

	CHUNK_END(stream);
}

void GfxTagModel::ReadMTRL(int iLength, BufferStream^ stream)
{
	CHUNK_BEGIN(stream, iLength);
	
	char* pszName = stream->ReadZString();
	Console::WriteLine("Material: \"{0}\"", gcnew String(pszName));
	delete[] pszName;

	CHUNK_END(stream);
}

void GfxTagModel::ReadMTLE(int iLength, BufferStream^ stream)
{
	stream->Ignore(iLength);
}

void GfxTagModel::ReadHLPR(D3DXEXTENDEDFRAME* pParent, 
						   int iLength, 
						   BufferStream^ stream)
{
	Console::WriteLine("Reading HLPR...");

	CHUNK_BEGIN(stream, iLength);

	D3DXEXTENDEDFRAME* pFrame = new D3DXEXTENDEDFRAME;
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);

	D3DXFrameAppendChild(pParent, pFrame);

	SChunk Chunk;

	while (ReadChunkInfo(stream, &Chunk))
	{
		if (Chunk == "HCVL")
		{
			ReadHCVL(pFrame, Chunk.dwSize, stream);
		}
		else if (Chunk == "HPNT")
		{
			ReadHPNT(pFrame, Chunk.dwSize, stream);
		}
		else
			stream->Ignore(Chunk.dwSize);

		CHUNK_BREAK(stream);
	}

	CHUNK_END(stream);
}

void GfxTagModel::ReadHCVL(D3DXEXTENDEDFRAME* pFrame, 
						   int iLength, 
						   BufferStream^ stream)
{
	CHUNK_BEGIN(stream, iLength);

	stream->Ignore(4);

	char szTmp[256];

	char* pszName = stream->ReadZString();
	
	sprintf(szTmp, "HCVL_%s", pszName);
	
	pFrame->Name = CopyXFileSafeString(szTmp);
	
	delete[] pszName;

	CHUNK_END(stream);
}

void GfxTagModel::ReadHPNT(D3DXEXTENDEDFRAME* pFrame, 
						   int iLength, 
						   BufferStream^ stream)
{
	LPDIRECT3DDEVICE9 pD3DDevice =
		FableMod::Gfx::Manager::GetDevice()->GetD3DDevice();

	CHUNK_BEGIN(stream, iLength);

	D3DXVECTOR3 vPos;

	stream->Read(&vPos, sizeof(vPos));
	
	stream->Ignore(4);

	ID3DXMesh* pD3DXMesh = 0;

	if (FAILED(D3DXCreateSphere(
		pD3DDevice,
		10,
		8,
		8,
		&pD3DXMesh,
		0)))
	{
		throw gcnew System::Exception("Failed to create mesh");
	}

	D3DXMATERIAL* pMaterial = new D3DXMATERIAL;

	memset(pMaterial, 0, sizeof(D3DXMATERIAL));

	pMaterial->MatD3D.Diffuse.a = 1.0f;
	pMaterial->MatD3D.Diffuse.r = 0.95f;
	pMaterial->MatD3D.Diffuse.g = 0.95f;
	pMaterial->MatD3D.Diffuse.b = 0.6f;
	pMaterial->MatD3D.Ambient = pMaterial->MatD3D.Diffuse;
	
	D3DXMESHCONTAINER* pContainer = new D3DXMESHCONTAINER; 
	
	pContainer->MeshData.Type = D3DXMESHTYPE_MESH;
	pContainer->MeshData.pMesh = pD3DXMesh;
	pContainer->Name = NULL;
	pContainer->pMaterials = pMaterial;
	pContainer->NumMaterials = 1;
	pContainer->pAdjacency = NULL;
	pContainer->pEffects = NULL;
	pContainer->pNextMeshContainer = NULL;
	pContainer->pSkinInfo = NULL;
	
	pFrame->pMeshContainer = pContainer;

	D3DXMatrixTranslation(
		&pFrame->TransformationMatrix, vPos.x, vPos.y, vPos.z);

	CHUNK_END(stream);
}

void GfxTagModel::ReadSUBM(D3DXEXTENDEDFRAME* pParent, 
						   int iLength, 
						   BufferStream^ stream)
{
	Console::WriteLine("Reading SUBM...");

	CHUNK_BEGIN(stream, iLength);
	
	D3DXEXTENDEDFRAME* pFrame = new D3DXEXTENDEDFRAME;
	char* pszName = stream->ReadZString();
	pFrame->Name = CopyXFileSafeString(pszName);
	delete[] pszName;
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);

	D3DXFrameAppendChild(pParent, pFrame);

	SChunk Chunk;

	stream->Ignore(4*4);

	while (ReadChunkInfo(stream, &Chunk))
	{
		if (Chunk == "TRFM")
		{
			ReadTRFM(pFrame, Chunk.dwSize, stream);
		}
		else if (Chunk == "PRIM")
		{
			ReadPRIM(pFrame, Chunk.dwSize, stream);
		}
		else
			stream->Ignore(Chunk.dwSize);

		CHUNK_BREAK(stream);
	}

	CHUNK_END(stream);
}

void GfxTagModel::ReadTRFM(D3DXEXTENDEDFRAME* pFrame,
						   int iLength, 
						   BufferStream^ stream)
{
	CHUNK_BEGIN(stream, iLength);

	float afMatrix[12];

	stream->Read(afMatrix, sizeof(afMatrix));

	pFrame->TransformationMatrix = D3DXMATRIX(
		afMatrix[0],  afMatrix[1],  afMatrix[2], 0.0f,
		afMatrix[3],  afMatrix[4],  afMatrix[5], 0.0f,
		afMatrix[6],  afMatrix[7],  afMatrix[8], 0.0f,
		afMatrix[9], afMatrix[10], afMatrix[11], 1.0f);

	CHUNK_END(stream);
}

void GfxTagModel::ReadPRIM(D3DXEXTENDEDFRAME* pFrame,
						   int iLength, 
						   BufferStream^ stream)
{
	CHUNK_BEGIN(stream, iLength);

	LPDIRECT3DDEVICE9 pD3DDevice =
		FableMod::Gfx::Manager::GetDevice()->GetD3DDevice();

	stream->Ignore(4);

	SChunk Chunk;

	int iNumFaces = 0;
	WORD* awFaces = 0;
	int iNumVertices = 0;
	SVertex* aVertices = 0;

	while (ReadChunkInfo(stream, &Chunk))
	{
		if (Chunk == "TRIS")
        {
            ReadTRIS(Chunk.dwSize, stream, &iNumFaces, &awFaces);
		}
		else if (Chunk == "VERT")
        {
            ReadVERT(Chunk.dwSize, stream, &iNumVertices, &aVertices);
        }
        else
    		stream->Ignore(Chunk.dwSize);

		CHUNK_BREAK(stream);
	}

	if (iNumFaces <= 0 || iNumVertices <= 0)
	{
		throw gcnew System::Exception("Invalid mesh");
	}

	ID3DXMesh* pD3DXMesh = 0;
    
	if (FAILED(D3DXCreateMeshFVF(
		iNumFaces, 
		iNumVertices, 
		D3DXMESH_MANAGED, 
		D3DFVF_XYZ  | 
		D3DFVF_NORMAL |
		D3DFVF_TEX1 | 
		D3DFVF_TEXCOORDSIZE2(0), 
		pD3DDevice, 
		&pD3DXMesh)))
	{
		throw gcnew System::Exception("Failed to create mesh");
	}

	WORD* pwFaces = 0;
	pD3DXMesh->LockIndexBuffer(0, (void**)&pwFaces);
	
	for (int i = 0; i < iNumFaces*3; i += 3)
	{
		pwFaces[i]   = awFaces[i+2];
		pwFaces[i+1] = awFaces[i+1];
		pwFaces[i+2] = awFaces[i];
	}

	pD3DXMesh->UnlockIndexBuffer();

	SVertex* pVerts = 0;
	pD3DXMesh->LockVertexBuffer(0, (void**)&pVerts);
	memcpy(pVerts, aVertices, sizeof(SVertex)*iNumVertices);
	pD3DXMesh->UnlockVertexBuffer();

	D3DXMATERIAL* pMaterial = new D3DXMATERIAL;

	memset(pMaterial, 0, sizeof(D3DXMATERIAL));

	pMaterial->MatD3D.Diffuse.a = 0.8f;
	pMaterial->MatD3D.Diffuse.r = 0.5f;
	pMaterial->MatD3D.Diffuse.g = 0.5f;
	pMaterial->MatD3D.Diffuse.b = 0.55f;
	pMaterial->MatD3D.Ambient = pMaterial->MatD3D.Diffuse;
	
	D3DXMESHCONTAINER* pContainer = new D3DXMESHCONTAINER; 
	
	pContainer->MeshData.Type = D3DXMESHTYPE_MESH;
	pContainer->MeshData.pMesh = pD3DXMesh;
	pContainer->Name = NULL;
	pContainer->pMaterials = pMaterial;
	pContainer->NumMaterials = 1;
	pContainer->pAdjacency = NULL;
	pContainer->pEffects = NULL;
	pContainer->pNextMeshContainer = NULL;
	pContainer->pSkinInfo = NULL;
	
	pFrame->pMeshContainer = pContainer;
	
	if (aVertices)
		delete[] aVertices;
	
	if (awFaces)
		delete[] awFaces;

	CHUNK_END(stream);
}

void GfxTagModel::ReadTRIS(int iLength, 
						   BufferStream^ stream,
						   int* piNumFaces,
						   WORD** pawFaces)
{
	CHUNK_BEGIN(stream, iLength);

	LPDIRECT3DDEVICE9 pD3DDevice =
		FableMod::Gfx::Manager::GetDevice()->GetD3DDevice();
   
	UInt32 uiNumFaces = stream->ReadUInt32();
    
	WORD* awFaces = new WORD[uiNumFaces*3];

	stream->Read(awFaces, sizeof(WORD)*(uiNumFaces*3));

	*piNumFaces = uiNumFaces;
    *pawFaces = awFaces;

	CHUNK_END(stream);
}

void GfxTagModel::ReadVERT(int iLength, 
						   BufferStream^ stream,
						   int* piNumVertices,
						   SVertex** paVertices)
{
	CHUNK_BEGIN(stream, iLength);

	LPDIRECT3DDEVICE9 pD3DDevice =
		FableMod::Gfx::Manager::GetDevice()->GetD3DDevice();
   
	UInt32 uiNumVerts = stream->ReadUInt32();
    
	SVertex* aVertices = new SVertex[uiNumVerts];

	stream->Read(aVertices, sizeof(SVertex)*uiNumVerts);

	*piNumVertices = uiNumVerts;
    *paVertices = aVertices;

	CHUNK_END(stream);
}

void GfxTagModel::Clear()
{
	GFX_SAFE_DELETE(m_pRoot);
	GFX_SAFE_DELETE(m_pNode);

	m_pRoot = new D3DXEXTENDEDFRAME;
	D3DXMatrixIdentity(&m_pRoot->TransformationMatrix);
}

void GfxTagModel::ImportX(String^ fileName)
{
	AllocateHierarchy* pArc = new AllocateHierarchy();

	LPDIRECT3DDEVICE9 pD3DDevice =
		FableMod::Gfx::Manager::GetDevice()->GetD3DDevice();

	IntPtr ptr = Marshal::StringToHGlobalUni(fileName);
	
	D3DXFRAME* pFrame;

	if (FAILED(D3DXLoadMeshHierarchyFromX(
		(LPCTSTR)ptr.ToPointer(),
		D3DXMESH_MANAGED, 
		pD3DDevice,
		pArc,
		NULL,
		&pFrame,
		NULL)))
	{
		Marshal::FreeHGlobal(ptr);
		throw gcnew System::Exception("Failed to import file");
	}

	Marshal::FreeHGlobal(ptr);
	
	if (m_pRoot)
	{	
		m_pRoot->Destroy();
		delete m_pRoot;
	}

	GFX_SAFE_DELETE(m_pNode);
	
	m_pRoot = (D3DXEXTENDEDFRAME*)pFrame;
}

void GfxTagModel::CompileToEntry(AssetEntry^ entry)
{
	BufferStream^ stream = gcnew BufferStream();

	stream->WriteUInt32(0x3E3E3E3E);

	stream->WriteUInt32(CHUNK_TAG('3','D','M','F'));
	stream->WriteUInt32(100);
	stream->WriteZString("Copyright Big Blue Box Studios Ltd.");

	Compile3DRT(stream);

	/*
	unsigned int uiSize = stream->GetWritten()*2;

	unsigned char* pucOut = new unsigned char[uiSize];

	LZO::CompressRaw(
		stream->GetData(), stream->GetWritten(), pucOut, &uiSize);

	entry->SetData((char*)pucOut, uiSize);

	delete[] pucOut;
	*/

	entry->SetData((char*)stream->GetData(), stream->GetWritten());

	
	/*
	FileStream^ file = File::Create("test.dat");
	FileControl::Write(file, stream->GetData(), stream->GetWritten());
	file->Close();
	*/

	stream->Close();
}

void GfxTagModel::Compile3DRT(BufferStream^ stream)
{
	CHUNK_WRITE_BEGIN(stream, 3DRT);

	CompileMaterials(stream);
	CompileFrame(m_pRoot, stream);

	CHUNK_WRITE_END(stream);
}

void GfxTagModel::CompileMaterials(BufferStream^ stream)
{
	CHUNK_WRITE_BEGIN(stream, MTLS);
	
	stream->WriteUInt32(CHUNK_TAG('M','T','R','L'));
	stream->WriteUInt32(58);
	stream->WriteZString("Material #24");
	unsigned char* pucTmp = new unsigned char[29];
	memset(pucTmp, 0, 29);
	stream->Write(pucTmp, 29);
	delete[] pucTmp;

	stream->WriteUInt32(CHUNK_TAG('M','T','L','E'));
	stream->WriteUInt32(8);
	stream->WriteUInt32(1);
	stream->WriteUInt32(0);
	
	CHUNK_WRITE_END(stream);
}

void GfxTagModel::CompileFrame(D3DXEXTENDEDFRAME* pFrame,
							   BufferStream^ stream)
{
	if (pFrame->pMeshContainer)
	{
		CompileSUBM(pFrame, stream);
	}

	if (pFrame->pFrameFirstChild)
		CompileFrame((D3DXEXTENDEDFRAME*)pFrame->pFrameFirstChild, stream);
	if (pFrame->pFrameSibling)
		CompileFrame((D3DXEXTENDEDFRAME*)pFrame->pFrameSibling, stream);
}

void GfxTagModel::CompileSUBM(D3DXEXTENDEDFRAME* pFrame,
							  BufferStream^ stream)
{
	CHUNK_WRITE_BEGIN(stream, SUBM);
	
	stream->WriteZString(pFrame->Name);
	stream->WriteUInt32(0x00000000);
	stream->WriteUInt32(0xFFFFFFFF);
	stream->WriteUInt32(0xFFFFFFFF);
	stream->WriteUInt32(0xFFFFFFFF);

	CompileTRFM(pFrame, stream);
	CompilePRIM(pFrame->pMeshContainer->MeshData.pMesh, stream);

	CHUNK_WRITE_END(stream);
}

void GfxTagModel::CompileTRFM(D3DXEXTENDEDFRAME* pFrame,
							  BufferStream^ stream)
{
	CHUNK_WRITE_BEGIN(stream, TRFM);
	
	D3DXMATRIX matTmp(pFrame->TransformationMatrix);

	stream->WriteFloat(matTmp._11);
	stream->WriteFloat(matTmp._12);
	stream->WriteFloat(matTmp._13);
	stream->WriteFloat(matTmp._21);
	stream->WriteFloat(matTmp._22);
	stream->WriteFloat(matTmp._23);
	stream->WriteFloat(matTmp._31);
	stream->WriteFloat(matTmp._32);
	stream->WriteFloat(matTmp._33);
	stream->WriteFloat(matTmp._41);
	stream->WriteFloat(matTmp._42);
	stream->WriteFloat(matTmp._43);

	CHUNK_WRITE_END(stream);
}

void GfxTagModel::CompilePRIM(LPD3DXMESH pMesh,
							  BufferStream^ stream)
{
	CHUNK_WRITE_BEGIN(stream, PRIM);
	
	stream->WriteUInt32(0x00000000);
	
	CompileTRIS(pMesh, stream);
	CompileVERT(pMesh, stream);
	//CompileUNIV(pMesh, stream);

	CHUNK_WRITE_END(stream);
}

void GfxTagModel::CompileTRIS(LPD3DXMESH pMesh,
							  BufferStream^ stream)
{
	CHUNK_WRITE_BEGIN(stream, TRIS);
	
	stream->WriteUInt32(pMesh->GetNumFaces());
	
	WORD* pwFaces = 0;

	pMesh->LockIndexBuffer(0, (void**)&pwFaces);
	
	for (int i = 0; i < pMesh->GetNumFaces()*3; i += 3)
	{
		stream->Write(&pwFaces[i+2], sizeof(WORD));
		stream->Write(&pwFaces[i+1], sizeof(WORD));
		stream->Write(&pwFaces[i+0], sizeof(WORD));
	}

	pMesh->UnlockIndexBuffer();

	CHUNK_WRITE_END(stream);
}


void GfxTagModel::CompileVERT(LPD3DXMESH pMesh,
							  BufferStream^ stream)
{
	CHUNK_WRITE_BEGIN(stream, VERT);
	
	stream->WriteUInt32(pMesh->GetNumVertices());
	
	SVertex* pVerts = 0;
	pMesh->LockVertexBuffer(0, (void**)&pVerts);
	
	stream->Write(pVerts, sizeof(SVertex)*pMesh->GetNumVertices());

	pMesh->UnlockVertexBuffer();

	CHUNK_WRITE_END(stream);
}

void GfxTagModel::CompileUNIV(LPD3DXMESH pMesh, BufferStream^ stream)
{
	CHUNK_WRITE_BEGIN(stream, UNIV);
	
	stream->WriteUInt32(pMesh->GetNumVertices());
	
	SVertex* pVerts = 0;
	pMesh->LockVertexBuffer(0, (void**)&pVerts);
	
	DWORD* adwIndices = new DWORD[pMesh->GetNumVertices()];
	memset(adwIndices, 0xFF, sizeof(DWORD)*pMesh->GetNumVertices());

	for (DWORD i = 0; i < pMesh->GetNumVertices(); ++i)
	{
		if (adwIndices[i] != 0xFFFFFFFF)
			continue;
			
		for (DWORD j = 0; j < pMesh->GetNumVertices(); ++j)
		{
			if (i == j || adwIndices[j] != 0xFFFFFFFF)
				continue;

			if (abs(pVerts[i].vPos.x-pVerts[j].vPos.x) < 0.01 &&
				abs(pVerts[i].vPos.y-pVerts[j].vPos.y) < 0.01 &&
				abs(pVerts[i].vPos.z-pVerts[j].vPos.z) < 0.01)
			{
				// Same fucking vertex.
				adwIndices[j] = i;
			}
		}

		adwIndices[i] = i;
	}

	pMesh->UnlockVertexBuffer();

	stream->Write(adwIndices, sizeof(DWORD)*pMesh->GetNumVertices());

	delete[] adwIndices;

	CHUNK_WRITE_END(stream);
}

static void ApplyEffects(FableMod::Gfx::Spatial* pSpatial)
{
	Node* pNode = dynamic_cast<Node*>(pSpatial);

	if (pNode)
	{
		int iCount = pNode->NumChildren();

		for (int i = 0; i < iCount; ++i)
		{
			ApplyEffects(pNode->GetChildAt(i));
		}
	}
	else
	{
		Mesh* pMesh = dynamic_cast<Mesh*>(pSpatial);

		if (pMesh)
		{
			pMesh->RenderFlags() |= MRF_HIGHLIGHT;
		}
	}
}

FableMod::Gfx::Node* GfxTagModel::BuildModel()
{
	FableMod::Gfx::Node* pNode = GfxBaseModel::BuildGfx(m_pRoot, 0);
	ApplyEffects(pNode);
	return pNode;
}

}
}
}