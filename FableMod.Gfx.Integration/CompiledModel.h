#pragma once

#include <windows.h>
#include <memory.h>
#include <d3d9.h>
#include <d3dx9math.h>
#include "ModelStructs.h"
#include "ExtendedFrame.h"

using namespace System;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

#pragma pack(push,1)

class CompiledModel
{
public:

	CompiledModel(char* data, unsigned int* length); 
	~CompiledModel();
	D3DXEXTENDEDFRAME* CreateBoneHeirarchy(int bonenum, const D3DXMATRIX* parentTransform);
	D3DXEXTENDEDFRAME* GetHelpers(int bonenum, const D3DXMATRIX* parentTransform);
	void SaveToBuffer(char* data, int* length);
	MTRL* GetMaterialFromID(DWORD id);
	MTRL* AddMaterial(DWORD id);
	void RemoveMaterial(DWORD id);
	void Destroy();
	void DestroyMaterials();
	void DestroySubMeshes();
	void DestroyBones();
	void DestroyHelpers();
	int FindHPNTString(DWORD crc);
	int FindHDMYString(DWORD crc);
	int FindBone(DWORD crc);
	int FindBoneString(DWORD crc);
	DWORD AddHPNTString(char* name);
	DWORD AddHDMYString(char* name);
	SUBM* AddSubMesh();
	HPNT* AddHPNT(char* name);
	HDMY* AddHDMY(char* name);
	int AddBone(char* bonename);

	const char* GetName() const { return m_szName; }

	char*					m_szName;
	unsigned char			SkeletonPresent; //Should be Yes
	float					floats1[10]; //Model Origin?? Listed in Sub-header...

	WORD					NumberHPNTs;
	WORD					NumberHDMYs;
	DWORD					HLPR_Index_Uncompressed;
	WORD					padding;
	//WORD					HPNT_Compressed;
	HPNT*					HPNTs;
	//WORD					HDMY_Compressed;
	HDMY*					HDMYs;
	//WORD					HLPR_Index_Compressed;
	//WORD					HPNTIndexSize;

	WORD					HPNT_StringCount;
	char**					HPNTIndex;//[HPNTIndexSize-2]; //Subtract the size

	WORD					HDMY_StringCount;
	char**					HDMYIndex;//[HLPR_Index_Uncompressed-HPNTIndexSize]; 
	//Rest of helper index deduced

	DWORD					NumberMaterials;
	DWORD					NumberSubMeshes;
	DWORD					NumberBones;
	DWORD					SizeOfBoneIndex;
	unsigned char			Unknown1;
	WORD					Unknown2;
	WORD					Unknown3;
	//WORD					Compressed;
	WORD*					Bone_Index_Reference;//[NumberBones-1];
	//WORD					BoneIndexCompressed;
	char**					BoneIndex;//[SizeOfBoneIndex];
	//WORD					CompressedSize;
	BONE_SUB_CHUNK_1*		BoneSubChunks1;//[NumberBones];
	//WORD					CompressedSize;
	BONE_SUB_CHUNK_2*		BoneSubChunks2;//[NumberBones];
	//WORD					CompressedSize;
	BONE_SUB_CHUNK_3*		BoneSubChunks3;//[NumberBones];
	
	float					floats2[12]; //3x4 matrix, commonly identity
	MTRL*					Materials;//[NumberMaterials]
	SUBM**					SubMeshes;//[NumberSubMeshes]

private:

	void ClearSubMeshes();
	void ClearBones();
	void ClearHelpers();
};
#pragma pack(pop)

}
}
}