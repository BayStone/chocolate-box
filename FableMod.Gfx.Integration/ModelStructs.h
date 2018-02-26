#pragma once

#include <windows.h>
#include <memory.h>
#include <d3d9.h>
#include <d3dx9math.h>
#include "Fixed.h"
#include "Common/PackedXYZ.h"

using namespace System;
using namespace FableMod::Common;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

#pragma pack(push,1)

struct HPNT
{
	long		HPNTCRC;
	float		floats[3];
	long		Parent;
	void GetMatrix(D3DXMATRIX* mat);
	void SetMatrix(const D3DXMATRIX* mat);
};

struct HDMY
{
	DWORD		HDMYCRC; //Used in Bone data
	float		floats[12];
	long		Parent;
	
	void GetMatrix(D3DXMATRIX* mat);
	void SetMatrix(const D3DXMATRIX* mat);
};

struct BONE_SUB_CHUNK_1
{
	DWORD		BoneCRC;
	DWORD		Parent;
	DWORD		nChildren;
	float		floats[12];
	void GetMatrix(D3DXMATRIX* mat);
	void SetMatrix(const D3DXMATRIX* mat);
};

struct BONE_SUB_CHUNK_2
{
	float		a[12];
	void GetMatrix(D3DXMATRIX* mat);
	void SetMatrix(const D3DXMATRIX* mat);
};

struct BONE_SUB_CHUNK_3
{
	float		a[16]; // should be transform matrix for bones
	void GetMatrix(D3DXMATRIX* mat);
	void SetMatrix(const D3DXMATRIX* mat);
};

struct MTRL
{
	DWORD					ID;
	char*					Name;
	DWORD					Padding;
	DWORD					BASE_Texture_ID; //From Texture.big
	DWORD					BUMPMAP_Texture_ID;
	DWORD					REFLECT_Texture_ID;
	DWORD					AlphaTextureID;
	DWORD					TextureFlags;
	DWORD					Glow_Strength;
	unsigned char			Unknown2;
	unsigned char			Alpha_Enabled;
	unsigned char			Unknown3;
	WORD					Unknown4;

	MTRL() :
	Name(0)
	{
	}

	~MTRL()
	{
	}
};

typedef CTFixed<short, 11> TexCoordv1;
typedef CTFixed<short, 11> TexCoordv2;

struct Vertex_12_4
{
	PackedXYZ		Coordinates;
	PackedXYZ		NormalVector;

	TexCoordv1	tu;
	TexCoordv1	tv;
};

struct Vertex_20_4
{
	PackedXYZ		Coordinates;
	//D3DVECTOR		Coordinates;

	unsigned char	BoneIndices[4]; //Bone Number?? 1 char padding
	unsigned char	BoneWeights[4]; //Vert weights?? Adds up to 255 1 char padding

	PackedXYZ		NormalVector;

	TexCoordv1	tu;
	TexCoordv1	tv;
};

struct Vertex_20_6
{
	PackedXYZ		Coordinates;
	//D3DVECTOR		Coordinates;

	//unsigned char	BoneIndices[4]; //Bone Number?? 1 char padding
	//unsigned char	BoneWeights[4]; //Vert weights?? Adds up to 255 1 char padding

	PackedXYZ		NormalVector;

	TexCoordv1	tu;
	TexCoordv1	tv;
	
	DWORD			Unknown[2];
};

struct Vertex_20_20
{
	//PackedXYZ		Coordinates;
	D3DVECTOR		Coordinates;

	//unsigned char	BoneIndice[4]; //Bone Number?? 1 char padding
	//unsigned char	BoneWeight[4]; //Vert weights?? Adds up to 255 1 char padding

	PackedXYZ		NormalVector;

	TexCoordv2	tu;
	TexCoordv2	tv;
};

struct Vertex_28_20
{

	//PackedXYZ		Coordinates;
	D3DVECTOR		Coordinates;

	unsigned char	BoneIndices[4]; //Bone Number?? 1 char padding
	unsigned char	BoneWeights[4]; //Vert weights?? Adds up to 255 1 char padding

	PackedXYZ		NormalVector;

	TexCoordv1	tu;
	TexCoordv1	tv;
};

struct Vertex_28_22
{

	//PackedXYZ		Coordinates;
	D3DVECTOR		Coordinates;

	//unsigned char	BoneIndices[4]; //Bone Number?? 1 char padding
	//unsigned char	BoneWeights[4]; //Vert weights?? Adds up to 255 1 char padding

	PackedXYZ		NormalVector;

	TexCoordv1	tu;
	TexCoordv1	tv;
	DWORD		Unknown[2];
};

struct Vertex_28_6
{

	PackedXYZ		Coordinates;
	//D3DVECTOR		Coordinates;

	unsigned char	BoneIndices[4]; //Bone Number?? 1 char padding
	unsigned char	BoneWeights[4]; //Vert weights?? Adds up to 255 1 char padding

	PackedXYZ		NormalVector;

	TexCoordv1 tu;
	TexCoordv1 tv;

	DWORD			Unknown[2];
};

struct Vertex_36_22
{
	D3DVECTOR		Coordinates;

	unsigned char	BoneIndices[4]; //Bone Number?? 1 char padding
	unsigned char	BoneWeights[4]; //Vert weights?? Adds up to 255 1 char padding

	PackedXYZ		NormalVector;

	TexCoordv1 tu;
	TexCoordv1 tv;
	DWORD			Unknown1;
	DWORD			Unknown2;
};

struct Vertex_36_4
{
	D3DVECTOR		Coordinates;
	D3DVECTOR		NormalVector;

	float			tu;
	float			tv;

	DWORD			MeshLevel;
};

struct SUBM_SUB_CHUNK_1
{
	DWORD							nFaceVertexIndices;
	DWORD							sFaceVertexIndices;
	WORD							Ignored_FaceVertexIndices;
	unsigned char					padding;
	DWORD							MTRL_ID;
};

struct SUBM_SUB_CHUNK_2
{
	DWORD							nFaceVertexIndices;
	DWORD							sFaceVertexIndices;
	WORD							Ignored_FaceVertexIndices;
	unsigned char					padding;
	DWORD							nVertices; //??
	WORD							Possible_Weights; //??
	unsigned char					Unknown;
	unsigned char					nBoneIndice;
	unsigned char*					nBone;//[nBoneIndice];

	SUBM_SUB_CHUNK_2() :
	nBone(0)
	{
	}

	~SUBM_SUB_CHUNK_2()
	{
		if (nBone)
			delete[] nBone;
	}
};

struct VGRP_Vertex
{
	DWORD					Vertice;
	float					unk;
};

struct VGRPSub
{
	DWORD					Group;
	DWORD					nVertices;
	VGRP_Vertex*			Vertices;//[nVertices];

	VGRPSub() :
	Vertices(0)
	{
	}

	~VGRPSub()
	{
		if (Vertices)
			delete[] Vertices;
	}
};

struct CLTH_Distance
{
	DWORD	Unknown1;
	DWORD	Unknown2;
	DWORD	Unknown3;
	DWORD	VertIndexA;
	DWORD	VertIndexB;
	float	dist;
	float	strength;
};

struct CLTH_Particle
{
	float	X;
	float	Y;
	float	Z;
};

struct CLTH_Vertex
{
	float	X;
	float	Y;
	float	Z;
};

struct CLTH_Tri
{
	WORD	a;
	WORD	b;
	WORD	c;
};

struct CLTH_TexCoord
{
	float	X;
	float	Y;
};

struct VGRP
{
	DWORD					nVertexGroup;
	VGRPSub**				VertexGroup;//[nVertexGroup];

	VGRP()
	{
		memset(this, 0, sizeof(*this));
	}

	~VGRP()
	{
		if (VertexGroup)
		{
			for (DWORD i = 0; i < nVertexGroup; ++i)
				delete VertexGroup[i];

			delete[] VertexGroup;
		}
	}
};

struct UnknownEntry
{
	WORD	v1;
	WORD	v2;
};

struct CLTH
{
	//DWORD				Unknown1; should be clth count
	DWORD				Unknown1;
	DWORD				Unknown2;
	DWORD				sChunk_Whole;
	//WORD				Unknown3;
	DWORD				Unknown4;
	DWORD				sDistanceIndice;
	CLTH_Distance*		DistanceIndice;//[sDistanceIndice/28];
	float				Unknown5;
	DWORD				Unknown6;
	float				Unknown7;
	DWORD				sParticleIndice;
	CLTH_Particle*		ParticleIndice;//[sParticleIndice];
	float*				ParticleAlphaIndice;//[sParticleIndice];
	DWORD				Unknown8;
	float				WindStrength; //strength
	char				EnableDragging; //enable
	char				RotationalDragging; //rotational
	float				StrengthDragging; //strength
	char				EnableAcceleration; //enable
	float				AccelerationDampening; //damping
	DWORD				nTriIndice;
	CLTH_Tri*			TriIndice;//[nTriIndice]
	DWORD				Unknown10; // looks like padding it
	DWORD				UnkCount;
	UnknownEntry*		UnkEntries;//[count];
	DWORD				sVertexIndice;
	CLTH_Vertex*		VertexIndice;//[sVertexIndice];
	DWORD				sTexCoordIndice;
	CLTH_TexCoord*		TexCoordIndice;//[sTexCoordIndice];
	DWORD				count1;
	DWORD*				set1;//[count1];
	DWORD				count2;
	DWORD*				set2;//[count2];
	char				Unknown9;

	// end of directly readable data
	DWORD				VGRPCount;
	VGRP**				VGRPs;

	CLTH()
	{
		memset(this, 0, sizeof(*this));
	}

	~CLTH();
};

struct Face
{
	unsigned short v1;
	unsigned short v2;
	unsigned short v3;
	unsigned short m;
};

struct Vertex
{
	float		x;
	float		y;
	float		z;

	float		nx;
	float		ny;
	float		nz;

	float		tu;
	float		tv;
};

struct VertexBones
{
	unsigned char	bones[4];
	unsigned char	influences[4];
	bool			converted;
};

struct SUBM
{
	DWORD					MaterialID;
	DWORD					DestroyableMeshLevel;
	float					floatsa[5];
	DWORD					Header_count1;
	DWORD					Header_count2;
	DWORD					Number_Verts;
	DWORD					Number_Faces;
	DWORD					TriangleStripLength;
	DWORD					VertexSubType;
	DWORD					Header_count1_dupe;
	DWORD					Header_count2_dupe;
	SUBM_SUB_CHUNK_1*		SubChunks1;//[Header_count1];
	SUBM_SUB_CHUNK_2**		SubChunks2;//[Header_count2];
	float					floats[8];
	DWORD					VertexSize;
	DWORD					Padding;
	char*					Vertices;
	/*
	Vertex_12*				Vertices12;
	Vertex_20*				Vertices20;
	Vertex_28*				Vertices28;
	Vertex_36*				Vertices36;
	*/
	WORD*					TriangleStrips;
	DWORD					nClothes;
	CLTH**					DynamicClothes;

	SUBM()
	{
		memset(this, 0, sizeof(*this));
	}

	~SUBM();

	void GetVertices(Vertex* vertices);
	void GetFaces(WORD* faces, VertexBones* bones = 0);
	void GetVertexBones(VertexBones* bones);
};

#pragma pack(pop)

}
}
}