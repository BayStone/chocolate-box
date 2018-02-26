#include "Stdafx.h"
#include "CompiledModel.h"
#include "Helpers.h"

//#define MESH_DEBUG

using namespace FableMod::Data;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

static int DecompressSection(char* dest, char* src, int size)
{
	DWORD len = *(WORD*)src;
	
	if (len == 0xffff)
	{
		len = *(DWORD*)(src+2) + 4;
	}
	
	unsigned int outlen = size;

	if (len == 0)
	{
		src += 2;
		memcpy(dest,src, size);
		return size +2;
	}
	else
	{
		LZO::DecompressChunk(
			(unsigned char*)src, len+5, (unsigned char*)dest, &outlen);
	
		return len+5;
	}
}

static int CompressSection(
	char* dest, char* src, int size, int destlen, 
	bool forceNoCompression, bool forceDWORDLength = false)
{
	unsigned int outlen = destlen;
	
	LZO::CompressChunk(
		(unsigned char*)src, (unsigned int)size, 
		(unsigned char*)dest, &outlen,3, forceDWORDLength);
	
	if (outlen < size && !forceNoCompression)
	{
		return outlen;
	}
	else
	{
		*(WORD*)dest = 0x0000;
		memcpy(dest+2, src, size);
		return size+2;
	}
}

//----------------------------------------------------------------------
// CompiledModel
//----------------------------------------------------------------------

CompiledModel::CompiledModel(char* data, unsigned int* length)
{
#ifdef MESH_DEBUG
	Console::WriteLine("Compiledmodel({0})", *length);
#endif

	memset(this, 0, sizeof(*this));
	
	DWORD offset = 0;

	int len = strlen(data);
	m_szName = new char[len+1];
	memcpy(m_szName, data+offset, len+1);
	offset+=len+1;

#ifdef MESH_DEBUG
	Console::WriteLine("Loading compiledmodel \"{0}\"...", 
		gcnew String(m_szName));
#endif

	memcpy(&SkeletonPresent, data+offset, 51);
	offset+=51;

	if (NumberHPNTs)
	{
		HPNTs = new HPNT[NumberHPNTs];
		
		offset += DecompressSection(
			(char*)HPNTs, data+offset, sizeof(HPNT)*NumberHPNTs);
	}

	if (NumberHDMYs)
	{
		HDMYs = new HDMY[NumberHDMYs];
		offset += DecompressSection(
			(char*)HDMYs, data+offset, sizeof(HDMY)*NumberHDMYs);
	}
	
	if (HLPR_Index_Uncompressed)
	{
		char* tmp = new char[HLPR_Index_Uncompressed];
		offset += DecompressSection(tmp, data+offset, HLPR_Index_Uncompressed);
		WORD hpntlen;
		memcpy(&hpntlen, tmp, 2);
		int off = 2;
		int pntcount=0;
		int dmycount=0;

		for(int i=off;i<HLPR_Index_Uncompressed;i++)
		{
			if (tmp[i] == 0x0)
			{
				if (i < hpntlen)
					pntcount++;
				else
					dmycount++;
			}
		}
		
		pntcount--; // last null isnt a string
		dmycount--; // last null isnt a string
		HPNTIndex = new char*[pntcount];
		HDMYIndex = new char*[dmycount];
		HPNT_StringCount = pntcount;
		HDMY_StringCount = dmycount;

		for(int i=0;i<pntcount;i++)
		{
			len = strlen(tmp+off);
			HPNTIndex[i] = new char[len+1];
			memcpy(HPNTIndex[i], tmp+off, len+1);
			off+=len+1;
		}
		
		off++; //skip null
		
		for(int i=0;i<dmycount;i++)
		{
			len = strlen(tmp+off);
			HDMYIndex[i] = new char[len+1];
			memcpy(HDMYIndex[i], tmp+off, len+1);
			off+=len+1;
		}
		
		off++; //skip null...
		
		delete [] tmp;
	}

	memcpy(&NumberMaterials, data+offset, 21);
	offset+=21;

#ifdef MESH_DEBUG
	Console::WriteLine("NumberMaterials {0}", NumberMaterials);
	Console::WriteLine("NumberSubMeshes {0}", NumberSubMeshes);
#endif

	if (NumberBones)
	{
		Bone_Index_Reference = new WORD[NumberBones-1];
		offset += DecompressSection(
			(char*)Bone_Index_Reference, data+offset, 
			sizeof(WORD)*(NumberBones-1));
		
		char* tmp = new char[SizeOfBoneIndex];
		offset += DecompressSection(tmp, data+offset, SizeOfBoneIndex);

		BoneIndex = new char*[NumberBones];
		len = strlen(tmp);
		BoneIndex[0] = new char[len+1];
		memcpy(BoneIndex[0],tmp, len+1);
		for(int i=0;i<NumberBones-1;i++)
		{
			len = strlen(tmp+Bone_Index_Reference[i]);
			BoneIndex[i+1] = new char[len+1];
			memcpy(BoneIndex[i+1], tmp+Bone_Index_Reference[i], len+1);
		}
		delete [] tmp;

		BoneSubChunks1 = new BONE_SUB_CHUNK_1[NumberBones];
		offset += DecompressSection(
			(char*)BoneSubChunks1, data+offset, 
			sizeof(BONE_SUB_CHUNK_1)*(NumberBones));

		BoneSubChunks2 = new BONE_SUB_CHUNK_2[NumberBones];
		offset += DecompressSection(
			(char*)BoneSubChunks2, data+offset, 
			sizeof(BONE_SUB_CHUNK_2)*(NumberBones));

		BoneSubChunks3 = new BONE_SUB_CHUNK_3[NumberBones];
		offset += DecompressSection(
			(char*)BoneSubChunks3, data+offset, 
			sizeof(BONE_SUB_CHUNK_3)*(NumberBones));

	}

	memcpy(&floats2, data+offset, 48);
	offset+=48;

	Materials = new MTRL[NumberMaterials];

	for (int i = 0; i < NumberMaterials; ++i)
	{
		memcpy(&Materials[i].ID, data+offset, 4);
		offset+=4;

		len = strlen(data+offset);
		Materials[i].Name = new char[len+1];
		memcpy(Materials[i].Name, data+offset, len+1);
		offset+=len+1;

		memcpy(&Materials[i].Padding, data+offset, 33);
		offset+=33;
	}

#ifdef MESH_DEBUG
	int iMeshOffset = offset;
#endif

	SubMeshes = new SUBM*[NumberSubMeshes];

	for(int i=0;i<NumberSubMeshes;i++)
	{
		bool hasdyncloth = false;
		SubMeshes[i] = new SUBM;
		memcpy(SubMeshes[i], data+offset, 60);
		offset+=60;

		SubMeshes[i]->SubChunks1 = new SUBM_SUB_CHUNK_1[SubMeshes[i]->Header_count1];
		memcpy(SubMeshes[i]->SubChunks1, data+offset, sizeof(SUBM_SUB_CHUNK_1)*SubMeshes[i]->Header_count1);
		offset+=sizeof(SUBM_SUB_CHUNK_1)*SubMeshes[i]->Header_count1;

		SubMeshes[i]->SubChunks2 = new SUBM_SUB_CHUNK_2*[SubMeshes[i]->Header_count2];
		for(int j=0;j<SubMeshes[i]->Header_count2;j++)
		{
			SubMeshes[i]->SubChunks2[j] = new SUBM_SUB_CHUNK_2;
			memcpy(SubMeshes[i]->SubChunks2[j], data+offset, 19);
			offset+=19;
			SubMeshes[i]->SubChunks2[j]->nBone = new unsigned char[SubMeshes[i]->SubChunks2[j]->nBoneIndice];
			memcpy(SubMeshes[i]->SubChunks2[j]->nBone, data+offset, SubMeshes[i]->SubChunks2[j]->nBoneIndice);
			offset+=SubMeshes[i]->SubChunks2[j]->nBoneIndice;
			if (SubMeshes[i]->SubChunks2[j]->Possible_Weights == 4)
				hasdyncloth = true;
		}
		memcpy(&SubMeshes[i]->floats, data+offset, 40);
		offset+=40;

		if (SubMeshes[i]->DestroyableMeshLevel == 0)
		{
			SubMeshes[i]->Vertices = new char[SubMeshes[i]->Number_Verts*SubMeshes[i]->VertexSize];
			offset+=DecompressSection(SubMeshes[i]->Vertices, data+offset, SubMeshes[i]->Number_Verts*SubMeshes[i]->VertexSize);
			
			SubMeshes[i]->TriangleStrips = new WORD[SubMeshes[i]->TriangleStripLength];
			offset+=DecompressSection((char*)SubMeshes[i]->TriangleStrips, data+offset, sizeof(WORD)*SubMeshes[i]->TriangleStripLength);
		}
		else
		{
			SubMeshes[i]->Vertices = new char[SubMeshes[i]->Number_Verts*SubMeshes[i]->VertexSize*SubMeshes[i]->DestroyableMeshLevel];
			offset+=DecompressSection(SubMeshes[i]->Vertices, data+offset, SubMeshes[i]->Number_Verts*SubMeshes[i]->VertexSize*SubMeshes[i]->DestroyableMeshLevel);
			
			SubMeshes[i]->TriangleStrips = new WORD[SubMeshes[i]->TriangleStripLength*SubMeshes[i]->DestroyableMeshLevel];
			offset+=DecompressSection((char*)SubMeshes[i]->TriangleStrips, data+offset, sizeof(WORD)*SubMeshes[i]->TriangleStripLength*SubMeshes[i]->DestroyableMeshLevel);
		}

		memcpy(&SubMeshes[i]->nClothes, data+offset, 4);
		offset+=4;

		SubMeshes[i]->DynamicClothes = new CLTH*[SubMeshes[i]->nClothes];
		
		for(int j=0;j<SubMeshes[i]->nClothes;j++)
		{
			SubMeshes[i]->DynamicClothes[j] = new CLTH;
			CLTH* c = SubMeshes[i]->DynamicClothes[j]; // local copy
			memcpy(c, data+offset, 12);
			offset+=12;

			char* clthdata = new char[c->sChunk_Whole];
			int clthoffset = 0;

			offset+=DecompressSection(clthdata, data+offset, c->sChunk_Whole);

			memcpy(&c->Unknown4, clthdata+clthoffset, 8);
			clthoffset+=8;

			c->DistanceIndice = (CLTH_Distance*)new char[c->sDistanceIndice];
			memcpy(c->DistanceIndice, clthdata+clthoffset, c->sDistanceIndice);
			clthoffset+=c->sDistanceIndice;
			
			memcpy(&c->Unknown5, clthdata+clthoffset, 16);
			clthoffset+=16;

			c->ParticleIndice = new CLTH_Particle[c->sParticleIndice];
			memcpy(c->ParticleIndice, clthdata+clthoffset, sizeof(CLTH_Particle)*c->sParticleIndice);
			clthoffset+=sizeof(CLTH_Particle)*c->sParticleIndice;

			c->ParticleAlphaIndice = new float[c->sParticleIndice];
			memcpy(c->ParticleAlphaIndice, clthdata+clthoffset, 4*c->sParticleIndice);
			clthoffset+=4*c->sParticleIndice;

			memcpy(&c->Unknown8, clthdata+clthoffset, 23);
			clthoffset+=23;

			//System::Diagnostics::Debug::WriteLine(c->nTriIndice.ToString());
			c->TriIndice = new CLTH_Tri[c->nTriIndice];
			memcpy(c->TriIndice, clthdata+clthoffset, sizeof(CLTH_Tri)*c->nTriIndice);
			clthoffset+= sizeof(CLTH_Tri)*c->nTriIndice;

			memcpy(&c->Unknown10, clthdata+clthoffset, 8);
			clthoffset+=8;

			//System::Diagnostics::Debug::WriteLine(c->UnkCount.ToString());

			c->UnkEntries = new UnknownEntry[c->UnkCount];
			memcpy(c->UnkEntries, clthdata+clthoffset, sizeof(UnknownEntry)*c->UnkCount);
			clthoffset+= sizeof(UnknownEntry)*c->UnkCount;
			
			memcpy(&c->sVertexIndice, clthdata+clthoffset, 4);
			clthoffset+=4;

			//c->set1 = new DWORD[0x1]; // delete me when done
			c->VertexIndice = new CLTH_Vertex[c->sVertexIndice];
			memcpy(c->VertexIndice, clthdata+clthoffset, sizeof(CLTH_Vertex)*c->sVertexIndice);
			clthoffset+=sizeof(CLTH_Vertex)*c->sVertexIndice;

			//c->set1 = new DWORD[0x1]; // delete me when done
			memcpy(&c->sTexCoordIndice, clthdata+clthoffset, 4);
			clthoffset+=4;

			//c->set1 = new DWORD[0x1]; // delete me when done
			c->TexCoordIndice = new CLTH_TexCoord[c->sTexCoordIndice];
			//c->set1 = new DWORD[0x1]; // delete me when done
			memcpy(c->TexCoordIndice, clthdata+clthoffset, sizeof(CLTH_TexCoord)*c->sTexCoordIndice);
			//c->set1 = new DWORD[0x1]; // delete me when done
			clthoffset+=sizeof(CLTH_TexCoord)*c->sTexCoordIndice;

			//c->set1 = new DWORD[0x1]; // delete me when done

			memcpy(&c->count1, clthdata+clthoffset, 4);
			clthoffset+=4;

			//System::Diagnostics::Trace::WriteLine(c->count1.ToString());
			c->set1 = new DWORD[c->count1];
			//System::Diagnostics::Trace::WriteLine(((unsigned int)c->set1).ToString());
			memcpy(c->set1, clthdata+clthoffset, sizeof(DWORD)*c->count1);
			clthoffset+=sizeof(DWORD)*c->count1;

			memcpy(&c->count2, clthdata+clthoffset, 4);
			clthoffset+=4;

			c->set2 = new DWORD[c->count2];
			memcpy(c->set2, clthdata+clthoffset, sizeof(DWORD)*c->count2);
			clthoffset+=sizeof(DWORD)*c->count2;

			memcpy(&c->Unknown9, clthdata+clthoffset, 1);
			clthoffset+=1;

			//for now assume vgrp count is 4
			c->VGRPCount = 4;
			c->VGRPs = new VGRP*[4];
			for(int k=0;k<c->VGRPCount;k++)
			{
				c->VGRPs[k] = new VGRP;
				VGRP* v = c->VGRPs[k]; // local copy

				memcpy(v, clthdata+clthoffset, 4);
				clthoffset+=4;

				v->VertexGroup = new VGRPSub*[v->nVertexGroup];
				for(int m=0;m<v->nVertexGroup;m++)
				{
					v->VertexGroup[m] = new VGRPSub;
					VGRPSub* vs = v->VertexGroup[m];// local copy

					memcpy(vs, clthdata+clthoffset, 8);
					clthoffset+=8;

					vs->Vertices = new VGRP_Vertex[vs->nVertices];
					memcpy(vs->Vertices, clthdata+clthoffset, sizeof(VGRP_Vertex)*vs->nVertices);
					clthoffset+=sizeof(VGRP_Vertex)*vs->nVertices;
				}

			}

			delete [] clthdata;
		}

	}

#ifdef MESH_DEBUG
	Console::WriteLine("Read MeshSize = {0}", offset-iMeshOffset);
	Console::WriteLine("Read Size = {0}", offset);
#endif

	*length = offset;
}

CompiledModel::~CompiledModel()
{
	Destroy();
}

void CompiledModel::Destroy()
{
	DestroySubMeshes();
	DestroyMaterials();
	
	if (m_szName)
	{
		delete[] m_szName;
		m_szName = 0;
	}

	DestroyBones();
	DestroyHelpers();
}

void CompiledModel::SaveToBuffer(char* data, int* length)
{
#ifdef MESH_DEBUG
	Console::WriteLine("Saving compiledmodel \"{0}\"...", 
		gcnew String(m_szName));
#endif

	//char* a= new char;
	DWORD offset = 0;
	int len = strlen(m_szName);
	memcpy(data+offset, m_szName, len+1);
	offset+=len+1;

	// generate helper index
	char* helperindex;

	HLPR_Index_Uncompressed = 0;
	if (HDMY_StringCount + HPNT_StringCount > 0)
	{
		WORD hpntlen = sizeof(WORD);
		for(int i=0;i<HPNT_StringCount;i++)
		{
			hpntlen += strlen(HPNTIndex[i]) + 1;
		}
		//compensate for null
		hpntlen+=1;
		HLPR_Index_Uncompressed = hpntlen;
		for(int i=0;i<HDMY_StringCount;i++)
		{
			HLPR_Index_Uncompressed += strlen(HDMYIndex[i]) + 1;
		}
		//compensate for null
		HLPR_Index_Uncompressed+=1;
		HLPR_Index_Uncompressed;// += sizeof(hpntlen);
		helperindex = new char[HLPR_Index_Uncompressed];
		int hlproffset = 0;
		memcpy(helperindex + hlproffset, &hpntlen, sizeof(hpntlen));
		hlproffset+=sizeof(hpntlen);
		for(int i=0;i<HPNT_StringCount;i++)
		{
			strcpy(helperindex + hlproffset, HPNTIndex[i]);
			hlproffset += strlen(HPNTIndex[i])+1;
		}
		*(helperindex+hlproffset) = 0; // section null term
		hlproffset++;
		for(int i=0;i<HDMY_StringCount;i++)
		{
			strcpy(helperindex + hlproffset, HDMYIndex[i]);
			hlproffset += strlen(HDMYIndex[i])+1;
		}
		*(helperindex+hlproffset) = 0; // section null term
		hlproffset++;
		if (hlproffset != HLPR_Index_Uncompressed)
		{
			throw gcnew System::Exception(
				"Err, indexed, compressed, helper strings are gay");
		}
	}

	memcpy(data+offset, &SkeletonPresent, 51);
	offset+=51;
	//dump hpnts
	if (NumberHPNTs)
	{
		unsigned int outlen = *length - offset;
		outlen = CompressSection((char*)(data+offset), (char*)HPNTs, sizeof(HPNT)*NumberHPNTs, outlen, false);
		offset+= outlen;
	}
	//dump hdmys
	if (NumberHDMYs)
	{
		unsigned int outlen = *length - offset;
		outlen = CompressSection((char*)data+offset, (char*)HDMYs, sizeof(HDMY)*NumberHDMYs, outlen, false);
		offset+= outlen;
	}
	if (HLPR_Index_Uncompressed != 0)
	{
		unsigned int outlen = *length - offset;
		outlen = CompressSection((char*)data+offset, (char*)helperindex, HLPR_Index_Uncompressed, outlen, false);
		offset+= outlen;
		delete helperindex;
	}
	// create bone index
	char* boneindex;
	WORD* boneindexref;
	if (NumberBones)
	{
		boneindexref = new WORD[NumberBones-1];
		SizeOfBoneIndex = strlen(BoneIndex[0])+1;
		for(int i=1;i<NumberBones;i++)
		{
			boneindexref[i-1] = SizeOfBoneIndex;
			SizeOfBoneIndex += strlen(BoneIndex[i])+1;
		}
		boneindex = new char[SizeOfBoneIndex];
		int indexoffset = 0;
		for(int i=0;i<NumberBones;i++)
		{
			strcpy(boneindex+indexoffset, BoneIndex[i]);
			indexoffset += strlen(BoneIndex[i])+1;
		}
		if (indexoffset!=SizeOfBoneIndex)
			throw gcnew System::Exception("bone index gayness");
	}
	memcpy(data+offset, &NumberMaterials, 21);
	offset+=21;
	if (NumberBones)
	{
		unsigned int outlen = *length - offset;
		outlen = CompressSection((char*)data+offset, (char*)boneindexref, sizeof(WORD)*(NumberBones-1), outlen, true);
		offset+= outlen;
		delete boneindexref;

		outlen = *length - offset;
		outlen = CompressSection((char*)data+offset, (char*)boneindex, SizeOfBoneIndex, outlen, false);
		offset+= outlen;
		delete boneindex;


		outlen = *length - offset;
		outlen = CompressSection((char*)(data+offset), (char*)BoneSubChunks1, sizeof(BONE_SUB_CHUNK_1)*NumberBones, outlen, false);
		offset+= outlen;

		outlen = *length - offset;
		outlen = CompressSection((char*)(data+offset), (char*)BoneSubChunks2, sizeof(BONE_SUB_CHUNK_2)*NumberBones, outlen, false);
		offset+= outlen;

		outlen = *length - offset;
		outlen = CompressSection((char*)(data+offset), (char*)BoneSubChunks3, sizeof(BONE_SUB_CHUNK_3)*NumberBones, outlen, false);
		offset+= outlen;
	}
	
	memcpy(data+offset, &floats2,48);
	
	offset+=48;
	
	for(int i=0;i<NumberMaterials;i++)
	{
		memcpy(data+offset, &Materials[i].ID, 4);
		offset+=4;
		strcpy(data+offset, Materials[i].Name);
		offset+=strlen(Materials[i].Name)+1;
		memcpy(data+offset, &Materials[i].Padding, 33);
		offset+=33;
	}

#ifdef MESH_DEBUG
	Console::WriteLine("Saving sub meshes {0}...", NumberSubMeshes);
	int iMeshOffset = offset;
#endif

	for (int i = 0; i < NumberSubMeshes; ++i)
	{
		memcpy(data+offset, SubMeshes[i], 60);
		offset+=60;

		memcpy(data+offset, 
			SubMeshes[i]->SubChunks1, 
			sizeof(SUBM_SUB_CHUNK_1)*SubMeshes[i]->Header_count1);
		
		offset+=sizeof(SUBM_SUB_CHUNK_1)*SubMeshes[i]->Header_count1;
		
		for(int j=0;j<SubMeshes[i]->Header_count2;j++)
		{
			memcpy(data+offset, SubMeshes[i]->SubChunks2[j], 19);
			offset+=19;
			memcpy(data+offset, SubMeshes[i]->SubChunks2[j]->nBone, SubMeshes[i]->SubChunks2[j]->nBoneIndice);
			offset+=SubMeshes[i]->SubChunks2[j]->nBoneIndice;
		}

		memcpy(data+offset,&SubMeshes[i]->floats, 40);
		offset+=40;
		
		if (SubMeshes[i]->DestroyableMeshLevel == 0)
		{
			unsigned int outlen = *length - offset;
			outlen = CompressSection((char*)data+offset, (char*)SubMeshes[i]->Vertices, SubMeshes[i]->VertexSize*SubMeshes[i]->Number_Verts, outlen, false,(SubMeshes[i]->Header_count2 > 1));
			offset+= outlen;

			outlen = *length - offset;
			outlen = CompressSection((char*)data+offset, (char*)SubMeshes[i]->TriangleStrips,  sizeof(WORD)*SubMeshes[i]->TriangleStripLength, outlen, false);
			offset+= outlen;
		}
		else
		{
			unsigned int outlen = *length - offset;
			outlen = CompressSection((char*)data+offset, (char*)SubMeshes[i]->Vertices, SubMeshes[i]->VertexSize*SubMeshes[i]->Number_Verts*SubMeshes[i]->DestroyableMeshLevel, outlen, false);
			offset+= outlen;

			outlen = *length - offset;
			outlen = CompressSection((char*)data+offset, (char*)SubMeshes[i]->TriangleStrips,  sizeof(WORD)*SubMeshes[i]->TriangleStripLength*SubMeshes[i]->DestroyableMeshLevel, outlen, false);
			offset+= outlen;
		}

		//clothes = 0
		//SubMeshes[i]->nClothes = 0;
		memcpy(data+offset, &SubMeshes[i]->nClothes, 4);
		offset+=4;

		// this is where we end, dynamic clothing should come next
		// right now its equivalent to assuming 0 clothes
		
		for(int j=0;j<SubMeshes[i]->nClothes;j++)
		{
			//SubMeshes[i]->DynamicClothes[j] = new CLTH;
			CLTH* c = SubMeshes[i]->DynamicClothes[j]; // local copy
			memcpy(data+offset, c, 12);
			offset+=12;

			char* clthdata = new char[0x40000]; // 256kb
			int clthoffset = 0;

			memcpy(clthdata+clthoffset, &c->Unknown4, 8);
			clthoffset+=8;

			memcpy(clthdata+clthoffset, c->DistanceIndice, c->sDistanceIndice);
			clthoffset+=c->sDistanceIndice;
			
			memcpy(clthdata+clthoffset, &c->Unknown5, 16);
			clthoffset+=16;

			memcpy(clthdata+clthoffset, c->ParticleIndice, sizeof(CLTH_Particle)*c->sParticleIndice);
			clthoffset+=sizeof(CLTH_Particle)*c->sParticleIndice;

			//c->ParticleAlphaIndice = new float[c->sParticleIndice];
			memcpy(clthdata+clthoffset, c->ParticleAlphaIndice, sizeof(float)*c->sParticleIndice);
			clthoffset+=sizeof(float)*c->sParticleIndice;

			memcpy(clthdata+clthoffset, &c->Unknown8, 23);
			clthoffset+=23;

			//System::Diagnostics::Debug::WriteLine(c->nTriIndice.ToString());
			//c->TriIndice = new CLTH_Tri[c->nTriIndice];
			memcpy(clthdata+clthoffset, c->TriIndice, sizeof(CLTH_Tri)*c->nTriIndice);
			clthoffset+= sizeof(CLTH_Tri)*c->nTriIndice;

			memcpy(clthdata+clthoffset, &c->Unknown10, 8);
			clthoffset+=8;

			//System::Diagnostics::Debug::WriteLine(c->UnkCount.ToString());

			//c->UnkEntries = new UnknownEntry[c->UnkCount];
			memcpy(clthdata+clthoffset, c->UnkEntries, sizeof(UnknownEntry)*c->UnkCount);
			clthoffset+= sizeof(UnknownEntry)*c->UnkCount;
			
			memcpy(clthdata+clthoffset, &c->sVertexIndice, 4);
			clthoffset+=4;

			//c->set1 = new DWORD[0x1]; // delete me when done
			//c->VertexIndice = new CLTH_Vertex[c->sVertexIndice];
			memcpy(clthdata+clthoffset, c->VertexIndice, sizeof(CLTH_Vertex)*c->sVertexIndice);
			clthoffset+=sizeof(CLTH_Vertex)*c->sVertexIndice;

			//c->set1 = new DWORD[0x1]; // delete me when done
			memcpy(clthdata+clthoffset, &c->sTexCoordIndice, 4);
			clthoffset+=4;

			//c->TexCoordIndice = new CLTH_TexCoord[c->sTexCoordIndice];
			memcpy(clthdata+clthoffset, c->TexCoordIndice, sizeof(CLTH_TexCoord)*c->sTexCoordIndice);
			clthoffset+=sizeof(CLTH_TexCoord)*c->sTexCoordIndice;


			memcpy(clthdata+clthoffset, &c->count1, 4);
			clthoffset+=4;

			//c->set1 = new DWORD[c->count1];
			memcpy(clthdata+clthoffset, c->set1, sizeof(DWORD)*c->count1);
			clthoffset+=sizeof(DWORD)*c->count1;

			memcpy(clthdata+clthoffset, &c->count2, 4);
			clthoffset+=4;

			//c->set2 = new DWORD[c->count2];
			memcpy(clthdata+clthoffset, c->set2, sizeof(DWORD)*c->count2);
			clthoffset+=sizeof(DWORD)*c->count2;

			memcpy(clthdata+clthoffset, &c->Unknown9, 1);
			clthoffset+=1;

			//for now assume vgrp count is 4
			//c->VGRPCount = 4;
			//c->VGRPs = new VGRP*[4];
			for(int k=0;k<c->VGRPCount;k++)
			{
				//c->VGRPs[k] = new VGRP;
				VGRP* v = c->VGRPs[k]; // local copy

				memcpy(clthdata+clthoffset, v, 4);
				clthoffset+=4;

				//v->VertexGroup = new VGRPSub*[v->nVertexGroup];
				for(int m=0;m<v->nVertexGroup;m++)
				{
					//v->VertexGroup[m] = new VGRPSub;
					VGRPSub* vs = v->VertexGroup[m];// local copy

					memcpy(clthdata+clthoffset, vs, 8);
					clthoffset+=8;

					//vs->Vertices = new VGRP_Vertex[vs->nVertices];
					memcpy(clthdata+clthoffset, vs->Vertices, sizeof(VGRP_Vertex)*vs->nVertices);
					clthoffset+=sizeof(VGRP_Vertex)*vs->nVertices;
				}

			}
			c->sChunk_Whole = clthoffset;
			unsigned int outlen = *length - offset;
			outlen = CompressSection(data+offset, clthdata, c->sChunk_Whole, outlen, false);
			offset += outlen;
			delete [] clthdata;
		}

	}

#ifdef MESH_DEBUG
	Console::WriteLine("MeshSize = {0}", offset-iMeshOffset);
	Console::WriteLine("Size = {0}", offset);
#endif

	*length = offset;
}

MTRL* CompiledModel::GetMaterialFromID(DWORD id)
{
	for(int i=0;i<NumberMaterials;i++)
	{
		if (Materials[i].ID == id)
			return &Materials[i];
	}
	return NULL;
}
/*
void DoTriangleFaces(unsigned short* dat, int start, int len, Face* facebuf,int vertoffset, int material, int* facecount )
{
	for(int i=0;i<len;i++)
	{
		facebuf[i].v1 = vertoffset + dat[start+2+(i*3)];
		facebuf[i].v2 = vertoffset + dat[start+1+(i*3)];
		facebuf[i].v3 = vertoffset + dat[start+(i*3)];
		facebuf[i].m = material;
	}
	*facecount = len;
}*/

//
//void DoTriangleStripFaces(unsigned short* dat, int start, int striplen, Face* facebuf,int vertoffset, int material, int* facecount )
//{
//	unsigned short* verts = dat;
//	//*facebuf = new Face[striplen];
//	*facecount = 0;
//	int degencount = 0;
//	for(int i=start;i<start+striplen;i++)
//	//for(int i=start;*facecount<striplen;i++)
//	{
//		if (i%2 == 0)
//		{
//			if (verts[i] != verts[i+1] && verts[i] != verts[i+2] && verts[i+2] != verts[i+1])
//			{
//				(facebuf)[*facecount].v1 = vertoffset + verts[i+2];
//				(facebuf)[*facecount].v2 = vertoffset + verts[i+1];
//				(facebuf)[*facecount].v3 = vertoffset + verts[i];
//				(facebuf)[*facecount].m = material;
//				(*facecount)++;
//			}
//			else
//				degencount++;
//		}
//		else
//		{
//			if (verts[i] != verts[i+1] && verts[i] != verts[i+2] && verts[i+2] != verts[i+1])
//			{
//				(facebuf)[*facecount].v1 = vertoffset + verts[i+1];
//				(facebuf)[*facecount].v2 = vertoffset + verts[i+2];
//				(facebuf)[*facecount].v3 = vertoffset + verts[i];
//				(facebuf)[*facecount].m = material;
//				(*facecount)++;
//			}
//			else
//				degencount++;
//		}
//	}
//}


void HDMY::GetMatrix(D3DXMATRIX* mat)
{
	float z[16] = {	1.0f,0.0f,0.0f,0.0f,
					0.0f,1.0f,0.0f,0.0f,
					0.0f,0.0f,1.0f,0.0f,
					0.0f,0.0f,0.0f,1.0f};
	z[0] = floats[0];
	z[1] = floats[1];
	z[2] = floats[2];
	
	z[4] = floats[3];
	z[5] = floats[4];
	z[6] = floats[5];
	
	z[8] = floats[6];
	z[9] = floats[7];
	z[10] = floats[8];
	
	z[12] = floats[9];
	z[13] = floats[10];
	z[14] = floats[11];
	*mat = z;
}
void HDMY::SetMatrix(const D3DXMATRIX* mat)
{
	floats[0] = (*mat)[0];
	floats[1] = (*mat)[1];
	floats[2] = (*mat)[2];

	floats[3] = (*mat)[4];
	floats[4] = (*mat)[5];
	floats[5] = (*mat)[6];

	floats[6] = (*mat)[8];
	floats[7] = (*mat)[9];
	floats[8] = (*mat)[10];

	floats[9] = (*mat)[12];
	floats[10] = (*mat)[13];
	floats[11] = (*mat)[14];
}
void HPNT::GetMatrix(D3DXMATRIX* mat)
{
	float z[16] = {	1.0f,0.0f,0.0f,0.0f,
					0.0f,1.0f,0.0f,0.0f,
					0.0f,0.0f,1.0f,0.0f,
					0.0f,0.0f,0.0f,1.0f};
	z[12] = floats[0];
	z[13] = floats[1];
	z[14] = floats[2];
	*mat = z;
}
void HPNT::SetMatrix(const D3DXMATRIX* mat)
{
	floats[0] = (*mat)[12];
	floats[1] = (*mat)[13];
	floats[2] = (*mat)[14];
}

void BONE_SUB_CHUNK_1::GetMatrix(D3DXMATRIX* mat)
{
	float z[16] = {	1.0f,0.0f,0.0f,0.0f,
					0.0f,1.0f,0.0f,0.0f,
					0.0f,0.0f,1.0f,0.0f,
					0.0f,0.0f,0.0f,1.0f};
	z[0] = floats[0];
	z[1] = floats[1];
	z[2] = floats[2];
	
	z[4] = floats[3];
	z[5] = floats[4];
	z[6] = floats[5];
	
	z[8] = floats[6];
	z[9] = floats[7];
	z[10] = floats[8];
	
	z[12] = floats[9];
	z[13] = floats[10];
	z[14] = floats[11];
	*mat = z;
}
void BONE_SUB_CHUNK_1::SetMatrix(const D3DXMATRIX* mat)
{
	floats[0] = (*mat)[0];
	floats[1] = (*mat)[1];
	floats[2] = (*mat)[2];

	floats[3] = (*mat)[4];
	floats[4] = (*mat)[5];
	floats[5] = (*mat)[6];

	floats[6] = (*mat)[8];
	floats[7] = (*mat)[9];
	floats[8] = (*mat)[10];

	floats[9] = (*mat)[12];
	floats[10] = (*mat)[13];
	floats[11] = (*mat)[14];
}

void BONE_SUB_CHUNK_3::GetMatrix(D3DXMATRIX* mat)
{
	float z[16] ;/*= {	0.0f,0.0f,0.0f,0.0f,
					0.0f,0.0f,0.0f,0.0f,
					0.0f,0.0f,0.0f,0.0f,
					0.0f,0.0f,0.0f,0.0f};*/
	z[0] = a[0];
	z[1] = a[4];
	z[2] = a[8];
	z[3] = a[12];

	z[4] = a[1];
	z[5] = a[5];
	z[6] = a[9];
	z[7] = a[13];

	z[8] = a[2];
	z[9] = a[6];
	z[10] = a[10];
	z[11] = a[14];

	z[12] = a[3];
	z[13] = a[7];
	z[14] = a[11];
	z[15] = a[15];

	*mat = z; 
}

void BONE_SUB_CHUNK_3::SetMatrix(const D3DXMATRIX* mat)
{
	a[0] = (*mat)[0];
	a[4] = (*mat)[1];
	a[8] = (*mat)[2];
	a[12] = (*mat)[3];
	
	a[1] = (*mat)[4];
	a[5] = (*mat)[5];
	a[9] = (*mat)[6];
	a[13] = (*mat)[7];
	
	a[2] = (*mat)[8];
	a[6] = (*mat)[9];
	a[10] = (*mat)[10];
	a[14] = (*mat)[11];
	
	a[3] = (*mat)[12];
	a[7] = (*mat)[13];
	a[11] = (*mat)[14];
	a[15] = (*mat)[15];
}
void BONE_SUB_CHUNK_2::GetMatrix(D3DXMATRIX* mat)
{
	D3DXMatrixTransformation(mat, NULL, NULL, &D3DXVECTOR3(a[8],a[9],a[10]), NULL, &D3DXQUATERNION(a[0],a[1],a[2],-a[3]), &D3DXVECTOR3(a[4],a[5],a[6]));
}
void BONE_SUB_CHUNK_2::SetMatrix(const D3DXMATRIX* mat)
{
	D3DXVECTOR3 scale;
	D3DXQUATERNION rotation;
	D3DXVECTOR3 translation;
	D3DXMatrixDecompose(&scale, &rotation, &translation, mat);
	a[8] = scale[0];
	a[9] = scale[1];
	a[10] = scale[2];

	a[0] = rotation[0];
	a[1] = rotation[1];
	a[2] = rotation[2];
	a[3] = -rotation[3];

	a[4] = translation[0];
	a[5] = translation[1];
	a[6] = translation[2];
	//D3DXMatrixTransformation(mat, NULL, NULL, &D3DXVECTOR3(a[8],a[9],a[10]), NULL, &D3DXQUATERNION(a[0],a[1],a[2],-a[3]), &D3DXVECTOR3(a[4],a[5],a[6]));
}
D3DXEXTENDEDFRAME* CompiledModel::CreateBoneHeirarchy(int bonenum, const D3DXMATRIX* parentTransform)
{
	D3DXEXTENDEDFRAME* node = new D3DXEXTENDEDFRAME;

	node->Name = CopyXFileSafeString(BoneIndex[bonenum]);
	node->pFrameSibling = NULL;
	node->pMeshContainer = NULL;
	node->pFrameFirstChild = NULL;
	float f[16] = {	1.0f,0.0f,0.0f,0.0f,
					0.0f,1.0f,0.0f,0.0f,
					0.0f,0.0f,1.0f,0.0f,
					0.0f,0.0f,0.0f,1.0f};

	BoneSubChunks2[bonenum].GetMatrix(&node->TransformationMatrix);//level == 6 || level == 7 || level == 9 || level == 12 || level == 13 || level == 14);//!(level%3));
	
	
	D3DXMATRIX transform = node->TransformationMatrix * (*parentTransform);
	//D3DXMATRIXMultiply(&transform, node->TransformationMatrix, *parentTransform);
	//node->TransformationMatrix = f;
	D3DXFRAME* lastchild = new D3DXEXTENDEDFRAME;// = GetHelpers(bonenum, transform);
	lastchild->Name = CopyXFileSafeString("BONE_OFFSET_MATRIX");
	lastchild->pFrameSibling = NULL;
	lastchild->pMeshContainer = NULL;
	lastchild->pFrameFirstChild = NULL;
	BoneSubChunks1[bonenum].GetMatrix(&lastchild->TransformationMatrix);
	node->pFrameFirstChild = lastchild;

	D3DXEXTENDEDFRAME* childx;


	for(int i=0;i<NumberBones;i++)
	{
		if (BoneSubChunks1[i].Parent == bonenum)
		{
			childx = CreateBoneHeirarchy(i, &transform);
			if (!lastchild)
			{
				node->pFrameFirstChild = childx;
				lastchild = childx;
			}
			else
			{
				lastchild->pFrameSibling = childx;
				lastchild = childx;
			}
		}
	}
	
	childx = GetHelpers(bonenum, &transform);
	if (!lastchild)
	{
		node->pFrameFirstChild = childx;
		lastchild = childx;
	}
	else
	{
		lastchild->pFrameSibling = childx;
		lastchild = childx;
	}

	return node;
}

D3DXEXTENDEDFRAME* CompiledModel::GetHelpers(int bonenum, const D3DXMATRIX* parentTransform)
{
	D3DXEXTENDEDFRAME* Helper = NULL;// = new D3DXEXTENDEDFRAME;
	D3DXEXTENDEDFRAME* First = NULL;
		//DummyRoot->Name = CopyXFileSafeString("Dummy Root");
	for(int i=0;i<NumberHDMYs;i++)
	{
		if (HDMYs[i].Parent == bonenum)
		{
			if (Helper)
			{
				D3DXEXTENDEDFRAME* tmp = new D3DXEXTENDEDFRAME;
				memset(tmp, 0, sizeof(*tmp));
				Helper->pFrameSibling = tmp;
				Helper = tmp;
			}
			else
			{
				Helper = new D3DXEXTENDEDFRAME;
				First = Helper;
			}
			int nameindex = FindHDMYString(HDMYs[i].HDMYCRC);
			char* tmpname = new char[1024];
			strcpy(tmpname, "HDMY_");

			if (nameindex > -1)
				strcpy(tmpname+5, HDMYIndex[nameindex]);
			else
				strcpy(tmpname+5, "NAME_NOT_FOUND");

			Helper->Name = CopyXFileSafeString(tmpname);
			delete [] tmpname;

			D3DXMATRIX inverse, transform;
			D3DXMatrixInverse(&inverse, NULL, parentTransform);
			HDMYs[i].GetMatrix(&transform);
			D3DXMatrixMultiply(&Helper->TransformationMatrix, &transform, &inverse);
		}
	}
	for(int i=0;i<NumberHPNTs;i++)
	{
		if (HPNTs[i].Parent == bonenum)
		{
			if (Helper)
			{
				D3DXEXTENDEDFRAME* tmp = new D3DXEXTENDEDFRAME;
				memset(tmp, 0, sizeof(*tmp));
				Helper->pFrameSibling = tmp;
				Helper = tmp;
			}
			else
			{
				Helper = new D3DXEXTENDEDFRAME;
				First = Helper;
			}
			int nameindex = FindHPNTString(HPNTs[i].HPNTCRC);
			char* tmpname = new char[1024];
			strcpy(tmpname, "HPNT_");

			if (nameindex > -1)
				strcpy(tmpname+5, HPNTIndex[nameindex]);
			else
				strcpy(tmpname+5, "NAME_NOT_FOUND");

			Helper->Name = CopyXFileSafeString(tmpname);
			delete [] tmpname;

			D3DXMATRIX inverse, transform;
			D3DXMatrixInverse(&inverse, NULL, parentTransform);
			HPNTs[i].GetMatrix(&transform);
			D3DXMatrixMultiply(&Helper->TransformationMatrix, &transform,&inverse);
		}
	}
	return First;
}
MTRL* CompiledModel::AddMaterial(DWORD id)
{
	for (int i = 0; i < NumberMaterials; ++i)
	{
		if (Materials[i].ID == id)
		{
			Console::WriteLine("add duplicate material");
			return 0;
		}
	}

	MTRL* newmats = new MTRL[NumberMaterials+1];
	
	memset(newmats, 0, sizeof(MTRL)*(NumberMaterials+1));
		
	if (Materials)
	{
		memcpy(newmats, Materials, sizeof(MTRL)*NumberMaterials);
		delete[] Materials;
	}

	++NumberMaterials;
	
	Materials = newmats;
	Materials[NumberMaterials-1].ID = id;
	
	return &Materials[NumberMaterials-1];
}
void CompiledModel::RemoveMaterial(DWORD id)
{	
	for (int i = 0;i < NumberMaterials; i++)
	{
		if (Materials[i].ID == id)
		{
			--NumberMaterials;
			MTRL* newmats = new MTRL[NumberMaterials];
			memcpy(newmats, Materials, sizeof(MTRL)*i);
			memcpy(newmats, Materials+i+1, 
				sizeof(MTRL)*(NumberMaterials-i));
			delete[] Materials;
			Materials = newmats;
		}
	}
}

void CompiledModel::DestroyMaterials()
{
	for (int i = 0;i < NumberMaterials; ++i)
		delete[] Materials[i].Name;

	GFX_SAFE_ARRAYDELETE(Materials);

	NumberMaterials = 0;
}

void CompiledModel::ClearSubMeshes()
{
	if (SubMeshes)
	{
		for (int i = 0; i < NumberSubMeshes; ++i)
			delete SubMeshes[i];

		delete[] SubMeshes;
		SubMeshes = 0;
	}
}

void CompiledModel::DestroySubMeshes()
{
	ClearSubMeshes();
	NumberSubMeshes = 0;
}

void CompiledModel::ClearHelpers()
{
	
	if (HPNTs)
	{
		delete[] HPNTs;
		HPNTs = 0;
	}

	if (HDMYs)
	{
		delete[] HDMYs;
		HDMYs = 0;
	}

	if (HPNTIndex)
	{
		for (WORD i = 0; i < HPNT_StringCount; ++i)
			delete[] HPNTIndex[i];

		delete[] HPNTIndex;
		HPNTIndex = 0;
	}

	if (HDMYIndex)
	{
		for (WORD i = 0; i < HDMY_StringCount; ++i)
			delete[] HDMYIndex[i];

		delete[] HDMYIndex;
		HDMYIndex = 0;
	}
}

void CompiledModel::DestroyHelpers()
{
	ClearHelpers();

	HPNT_StringCount = 0;
	HDMY_StringCount = 0;
	HLPR_Index_Uncompressed = 0;
	NumberHPNTs = 0;
	NumberHDMYs = 0;
}

void CompiledModel::ClearBones()
{
	for(int i=0;i<NumberBones;i++)
	{
		delete [] BoneIndex[i];
	}

	delete [] BoneIndex;
	delete [] BoneSubChunks1;
	delete [] BoneSubChunks2;
	delete [] BoneSubChunks3;
	delete [] Bone_Index_Reference;

	BoneIndex = NULL;
	BoneSubChunks1 = NULL;
	BoneSubChunks2 = NULL;
	BoneSubChunks3 = NULL;
	Bone_Index_Reference = NULL;
}

void CompiledModel::DestroyBones()
{
	ClearBones();

	NumberBones = 0;
	SizeOfBoneIndex = 0;
	SkeletonPresent = false;
}

int CompiledModel::AddBone(char* bonename)
{
	BONE_SUB_CHUNK_1* subs1 = new BONE_SUB_CHUNK_1[NumberBones+1];
	BONE_SUB_CHUNK_2* subs2 = new BONE_SUB_CHUNK_2[NumberBones+1];
	BONE_SUB_CHUNK_3* subs3 = new BONE_SUB_CHUNK_3[NumberBones+1];

	memset(subs1, 0, sizeof(*subs1)*NumberBones);
	memset(subs2, 0, sizeof(*subs2)*NumberBones);
	memset(subs3, 0, sizeof(*subs3)*NumberBones);

	if (BoneSubChunks1)
	{
		memcpy(subs1, BoneSubChunks1, sizeof(BONE_SUB_CHUNK_1)*NumberBones);
		memcpy(subs2, BoneSubChunks2, sizeof(BONE_SUB_CHUNK_2)*NumberBones);
		memcpy(subs3, BoneSubChunks3, sizeof(BONE_SUB_CHUNK_3)*NumberBones);

		delete[] BoneSubChunks1;
		delete[] BoneSubChunks2;
		delete[] BoneSubChunks3;
	}

	NumberBones++;

	BoneSubChunks1 = subs1;
	BoneSubChunks2 = subs2;
	BoneSubChunks3 = subs3;

	// set crc
	BoneSubChunks1[NumberBones-1].BoneCRC = 0xffffffff - ZLib::CRC32(0xffffffff, (unsigned char*)bonename, strlen(bonename));

	// add string to index
	char** newindex = new char*[NumberBones];
	memcpy(newindex, BoneIndex, sizeof(*BoneIndex)*(NumberBones-1));
	if (BoneIndex)
		delete BoneIndex;
	BoneIndex = newindex;
	BoneIndex[NumberBones-1] = new char[strlen(bonename)+1];
	strcpy(BoneIndex[NumberBones-1], bonename);

	return NumberBones-1;
}

SUBM* CompiledModel::AddSubMesh()
{
	SUBM** subs = new SUBM*[NumberSubMeshes+1];
	
	if (SubMeshes)
	{
		memcpy(subs, SubMeshes, sizeof(SUBM*)*NumberSubMeshes);
		delete[] SubMeshes;
	}

	++NumberSubMeshes;
	
	SubMeshes = subs;
	SubMeshes[NumberSubMeshes-1] = new SUBM;

	return SubMeshes[NumberSubMeshes-1];
}

int CompiledModel::FindHPNTString(DWORD crc)
{
	for(int i=0;i<HPNT_StringCount;i++)
	{
		if (0xffffffff - ZLib::CRC32(0xffffffff, (unsigned char*)HPNTIndex[i], strlen(HPNTIndex[i])) == crc) 
			return i;
	}
	return -1;
}
int CompiledModel::FindHDMYString(DWORD crc)
{
	for(int i=0;i<HDMY_StringCount;i++)
	{
		//DWORD dcrc = ZLib::CRC32(0, (unsigned char*)HDMYIndex[i], strlen(HDMYIndex[i]));
		if (0xffffffff - ZLib::CRC32(0xffffffff, (unsigned char*)HDMYIndex[i], strlen(HDMYIndex[i])) == crc) 
			return i;
	}
	return -1;
}
int CompiledModel::FindBone(DWORD crc)
{
	for(int i=0;i<NumberBones;i++)
	{
		//DWORD dcrc = ZLib::CRC32(0, (unsigned char*)HDMYIndex[i], strlen(HDMYIndex[i]));
		if (BoneSubChunks1[i].BoneCRC == crc) 
			return i;
	}
	return -1;
}
int CompiledModel::FindBoneString(DWORD crc)
{
	for(int i=0;i<NumberBones;i++)
	{
		//DWORD dcrc = ZLib::CRC32(0, (unsigned char*)HDMYIndex[i], strlen(HDMYIndex[i]));
		if (0xffffffff - ZLib::CRC32(0xffffffff, (unsigned char*)BoneIndex[i], strlen(BoneIndex[i])) == crc) 
			return i;
	}
	return -1;
}
DWORD CompiledModel::AddHPNTString(char* name)
{
	HPNT_StringCount++;
	char** newindex = new char*[HPNT_StringCount];
	memcpy(newindex, HPNTIndex, sizeof(*HPNTIndex)*(HPNT_StringCount-1));
	if (HPNTIndex)
		delete HPNTIndex;
	HPNTIndex = newindex;
	HPNTIndex[HPNT_StringCount-1] = new char[strlen(name)+1];
	strcpy(HPNTIndex[HPNT_StringCount-1],name);

	int i,j,flag;
	char* temp;
	for(i=HPNT_StringCount-1; i>0; i--)
	{
		flag = 1;
		for(j=0; i>j; j++)
		{
			if(strcmp(HPNTIndex[j],HPNTIndex[j+1])>0)
			{
				flag = 0;
				temp = HPNTIndex[j];
				HPNTIndex[j] = HPNTIndex[j+1];
				HPNTIndex[j+1] = temp;
			}
		}
		if(flag)
			break;
	}

	return 0xffffffff - ZLib::CRC32(0xffffffff, (unsigned char*)name, strlen(name));
}
DWORD CompiledModel::AddHDMYString(char* name)
{
	HDMY_StringCount++;
	char** newindex = new char*[HDMY_StringCount];
	memcpy(newindex, HDMYIndex, sizeof(*HDMYIndex)*(HDMY_StringCount-1));
	if (HDMYIndex)
		delete HDMYIndex;
	HDMYIndex = newindex;
	HDMYIndex[HDMY_StringCount-1] = new char[strlen(name)+1];
	strcpy(HDMYIndex[HDMY_StringCount-1],name);
	
	int i,j,flag;
	char* temp;
	for(i=HDMY_StringCount-1; i>0; i--)
	{
		flag = 1;
		for(j=0; i>j; j++)
		{
			if(strcmp(HDMYIndex[j],HDMYIndex[j+1])>0)
			{
				flag = 0;
				temp = HDMYIndex[j];
				HDMYIndex[j] = HDMYIndex[j+1];
				HDMYIndex[j+1] = temp;
			}
		}
		if(flag)
			break;
	}

	return 0xffffffff - ZLib::CRC32(0xffffffff, (unsigned char*)name, strlen(name));
}
HPNT* CompiledModel::AddHPNT(char* name)
{
	NumberHPNTs++;
	HPNT* points = new HPNT[NumberHPNTs];
	memcpy(points, HPNTs, sizeof(HPNT)*(NumberHPNTs-1));
	if (HPNTs)
		delete HPNTs;
	HPNTs = points;

	DWORD crc = 0xffffffff - ZLib::CRC32(0xffffffff, (unsigned char*)name, strlen(name));
	if (FindHPNTString(crc) == -1)
		AddHPNTString(name);
	HPNTs[NumberHPNTs-1].HPNTCRC = crc;


	
	int myhpnt = NumberHPNTs-1;
	int i,j,flag;
	HPNT temp;
	for(i=NumberHPNTs-1; i>0; i--)
	{
		flag = 1;
		for(j=0; i>j; j++)
		{
			if((unsigned __int32)HPNTs[j].HPNTCRC>(unsigned __int32)HPNTs[j+1].HPNTCRC)
			{
				flag = 0;
				temp = HPNTs[j];
				HPNTs[j] = HPNTs[j+1];
				HPNTs[j+1] = temp;

				if (myhpnt == j)
					myhpnt++;
				else if (myhpnt == j+1)
					myhpnt--;
			}
		}
		if(flag)
			break;
	}

	return &HPNTs[myhpnt];
}
HDMY* CompiledModel::AddHDMY(char* name)
{
	NumberHDMYs++;
	HDMY* newdummy = new HDMY[NumberHDMYs];
	memcpy(newdummy, HDMYs, sizeof(HDMY)*(NumberHDMYs-1));
	if (HDMYs)
		delete HDMYs;
	HDMYs = newdummy;

	DWORD crc = 0xffffffff - ZLib::CRC32(0xffffffff, (unsigned char*)name, strlen(name));
	if (FindHDMYString(crc) == -1)
		AddHDMYString(name);
	HDMYs[NumberHDMYs-1].HDMYCRC = crc;

	int myhdmy = NumberHDMYs-1;
	int i, j,flag;
	HDMY temp;
	for(i=NumberHDMYs-1; i>0; i--)
	{
		flag = 1;
		for(j=0; i>j; j++)
		{
			if((unsigned __int32)HDMYs[j].HDMYCRC>(unsigned __int32)HDMYs[j+1].HDMYCRC)
			{
				flag = 0;
				temp = HDMYs[j];
				HDMYs[j] = HDMYs[j+1];
				HDMYs[j+1] = temp;

				if (myhdmy == j)
					myhdmy++;
				else if (myhdmy == j+1)
					myhdmy--;
			}
		}
		if(flag)
			break;
	}

	return &HDMYs[myhdmy];
}

}
}
}