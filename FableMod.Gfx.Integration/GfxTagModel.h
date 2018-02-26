#pragma once

#include "GfxBaseModel.h"
#include "ExtendedFrame.h"

using namespace System;
using namespace FableMod::BIG;
using namespace FableMod::CLRCore;

#define CHUNK_BEGIN(_stream, _len) \
	int _iPos = (_stream)->Tell(); \
	int _iLength = _len

#define CHUNK_BREAK(_stream) \
	if ((_stream)->Tell()-_iPos >= _iLength) break

#define CHUNK_END(_stream) \
	(_stream)->Seek(_iPos); \
	(_stream)->Ignore(_iLength)
	
#define CHUNK_WRITE_BEGIN(_stream, tag) \
	{ \
		char szTag[5] = #tag; \
		stream->WriteUInt32(CHUNK_TAG( \
			szTag[0],szTag[1],szTag[2],szTag[3])); \
	} \
	int _iPosW = (_stream)->GetWritten(); \
	(_stream)->WriteUInt32(0xFFFFFFFF)

#define CHUNK_WRITE_END(_stream) \
	int _iSizeW = stream->GetWritten(); \
	stream->Seek(_iPosW); \
	stream->WriteUInt32((_iSizeW-_iPosW)-4); \
	stream->Seek(_iSizeW)

#define CHUNK_TAG(_a,_b,_c,_d) \
	(((int)(_d) << 24) | ((int)(_c) << 16) | ((int)(_b) << 8) | (int)(_a))

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

struct SChunk
{
	DWORD dwTag;
	DWORD dwSize;

	bool operator ==(DWORD dwCheck) const 
	{ 
		return dwTag == dwCheck; 
	}
	
	bool operator ==(const char* pszTag) const
	{ 
		return dwTag == 
			CHUNK_TAG(pszTag[0],pszTag[1],pszTag[2],pszTag[3]); 
	}
};

struct SVertex
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNormal;
	D3DXVECTOR2 vTex;
};

public ref class GfxTagModel : GfxBaseModel
{
public:

	GfxTagModel(AssetEntry^ entry);
	
	~GfxTagModel();
	
	void ImportX(String^ fileName);

	void Clear();

	virtual void CompileToEntry(AssetEntry^ entry) override;
	
protected:

	virtual FableMod::Gfx::Node* BuildModel() override;

private:

	bool ReadChunkInfo(
		BufferStream^ stream, SChunk* pChunk);
	void ReadData(BufferStream^ stream);
	
	void Read3DRT(int iLength, BufferStream^ stream);
	void ReadMTLS(int iLength, BufferStream^ stream);
	void ReadMTRL(int iLength, BufferStream^ stream);
	void ReadMTLE(int iLength, BufferStream^ stream);
	void ReadHLPR(
		D3DXEXTENDEDFRAME* pParent, int iLength, BufferStream^ stream);
	void ReadHPNT(
		D3DXEXTENDEDFRAME* pFrame, int iLength, BufferStream^ stream);
	void ReadHCVL(
		D3DXEXTENDEDFRAME* pFrame, int iLength, BufferStream^ stream);
	void ReadSUBM(
		D3DXEXTENDEDFRAME* pParent, int iLength, BufferStream^ stream);
	void ReadTRFM(
		D3DXEXTENDEDFRAME* pFrame, int iLength, BufferStream^ stream);
	void ReadPRIM(
		D3DXEXTENDEDFRAME* pFrame, int iLength, BufferStream^ stream);
    void ReadTRIS(
		int iLength, 
        BufferStream^ stream,
		int* piNumFaces,
		WORD** pawFaces);
	void ReadVERT(
		int iLength, 
        BufferStream^ stream,
		int* piNumVertices,
		SVertex** paVertices);

	void Compile3DRT(BufferStream^ stream);
	void CompileMaterials(BufferStream^ stream);
	void CompileFrame(
		D3DXEXTENDEDFRAME* pFrame, BufferStream^ stream);
	void CompileSUBM(
		D3DXEXTENDEDFRAME* pFrame, BufferStream^ stream);
	void CompileTRFM(
		D3DXEXTENDEDFRAME* pFrame, BufferStream^ stream);
	void CompilePRIM(
		LPD3DXMESH pMesh, BufferStream^ stream);
	void CompileTRIS(
		LPD3DXMESH pMesh, BufferStream^ stream);
	void CompileVERT(
		LPD3DXMESH pMesh, BufferStream^ stream);
	void CompileUNIV(
		LPD3DXMESH pMesh, BufferStream^ stream);	
};

}
}
}