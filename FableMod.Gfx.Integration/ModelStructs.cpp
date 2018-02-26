#include "Stdafx.h"
#include "ModelStructs.h"

//#define MESH_DEBUG

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

CLTH::~CLTH()
{
	if (UnkEntries)
		delete[] UnkEntries;

	if (VertexIndice)
		delete[] VertexIndice;

	if (TriIndice)
		delete[] TriIndice;

	if (TexCoordIndice)
		delete[] TexCoordIndice;

	if (ParticleIndice)
		delete[] ParticleIndice;

	if (DistanceIndice)
		delete[] DistanceIndice;

	if (set1)
		delete[] set1;

	if (set2)
		delete[] set2;

	if (VGRPs)
	{
		for (int j = 0; j < VGRPCount; ++j)
			delete VGRPs[j];
		
		delete[] VGRPs;
	}
}

SUBM::~SUBM()
{
	if (SubChunks1)
		delete[] SubChunks1;

	if (SubChunks2)
	{
		for (int j = 0; j < Header_count2; ++j)
			delete SubChunks2[j];
		
		delete[] SubChunks2;
	}

	if (TriangleStrips)
		delete[] TriangleStrips;
	
	if (Vertices)
		delete[] Vertices;

	if (DynamicClothes)
	{
		for (int j = 0; j < nClothes; ++j)
			delete DynamicClothes[j];
		
		delete[] DynamicClothes;
	}
}

void SUBM::GetVertices(Vertex* vertices)
{
#ifdef MESH_DEBUG
	Console::WriteLine("SUBM::GetVertices...");
	Console::WriteLine(" Vertex Size: {0}", VertexSize);
	Console::WriteLine(" Vertex SubType: {0}", VertexSubType);
	Console::WriteLine(" Vertex Count: {0}", Number_Verts);
	Console::WriteLine(" Padding: {0}", Padding);
#endif

	if (VertexSize == 12 && VertexSubType == 4)
	{
		Vertex_12_4* vertexdata = (Vertex_12_4*)Vertices;
		int count = Number_Verts;
		int i = 0;
		for(int j=0;j<count;j++)
		{
			vertices[j].x = vertexdata[j].Coordinates.GetX();// + xyz[0];
			vertices[j].y = vertexdata[j].Coordinates.GetY();// + xyz[1];
			vertices[j].z = vertexdata[j].Coordinates.GetZ();// + xyz[2];
			
			vertices[j].x*=floats[0];
			vertices[j].y*=floats[1];
			vertices[j].z*=floats[2];

			vertices[j].x+=floats[4];
			vertices[j].y+=floats[5];
			vertices[j].z+=floats[6];

			vertices[j].nx = vertexdata[j].NormalVector.GetX();
			vertices[j].ny = vertexdata[j].NormalVector.GetY();
			vertices[j].nz = vertexdata[j].NormalVector.GetZ();
			
			vertices[j].tu = vertexdata[j].tu;
			vertices[j].tv = vertexdata[j].tv;
		}
	}
	else if (VertexSize == 20 && VertexSubType == 4)
	{
		Vertex_20_4* vertexdata = (Vertex_20_4*)Vertices;
		int count = Number_Verts;
		for(int j=0;j<count;j++)
		{
			vertices[j].x = vertexdata[j].Coordinates.GetX();// + xyz[0];
			vertices[j].y = vertexdata[j].Coordinates.GetY();// + xyz[1];
			vertices[j].z = vertexdata[j].Coordinates.GetZ();// + xyz[2];

			vertices[j].x*=floats[0];
			vertices[j].y*=floats[1];
			vertices[j].z*=floats[2];

			vertices[j].x+=floats[4];
			vertices[j].y+=floats[5];
			vertices[j].z+=floats[6];
			
			vertices[j].nx = vertexdata[j].NormalVector.GetX();
			vertices[j].ny = vertexdata[j].NormalVector.GetY();
			vertices[j].nz = vertexdata[j].NormalVector.GetZ();
			
			vertices[j].tu = vertexdata[j].tu;
			vertices[j].tv = vertexdata[j].tv;
		}
	}
	else if (VertexSize == 20 && VertexSubType == 6)
	{
		Vertex_20_6* vertexdata = (Vertex_20_6*)Vertices;
		int count = Number_Verts;
		for(int j=0;j<count;j++)
		{
			vertices[j].x = vertexdata[j].Coordinates.GetX();// + xyz[0];
			vertices[j].y = vertexdata[j].Coordinates.GetY();// + xyz[1];
			vertices[j].z = vertexdata[j].Coordinates.GetZ();// + xyz[2];

			vertices[j].x*=floats[0];
			vertices[j].y*=floats[1];
			vertices[j].z*=floats[2];

			vertices[j].x+=floats[4];
			vertices[j].y+=floats[5];
			vertices[j].z+=floats[6];
			
			vertices[j].nx = vertexdata[j].NormalVector.GetX();
			vertices[j].ny = vertexdata[j].NormalVector.GetY();
			vertices[j].nz = vertexdata[j].NormalVector.GetZ();
			
			vertices[j].tu = vertexdata[j].tu;
			vertices[j].tv = vertexdata[j].tv;
		}
	}
	else if (VertexSize == 20 && VertexSubType == 20)
	{
		Vertex_20_20* vertexdata = (Vertex_20_20*)Vertices;
		int count = Number_Verts;
		for(int j=0;j<count;j++)
		{
			vertices[j].x = vertexdata[j].Coordinates.x;// + xyz[0];
			vertices[j].y = vertexdata[j].Coordinates.y;// + xyz[1];
			vertices[j].z = vertexdata[j].Coordinates.z;// + xyz[2];
			
			vertices[j].x*=floats[0];
			vertices[j].y*=floats[1];
			vertices[j].z*=floats[2];

			vertices[j].x+=floats[4];
			vertices[j].y+=floats[5];
			vertices[j].z+=floats[6];

			vertices[j].nx = vertexdata[j].NormalVector.GetX();
			vertices[j].ny = vertexdata[j].NormalVector.GetY();
			vertices[j].nz = vertexdata[j].NormalVector.GetZ();
			
			vertices[j].tu = vertexdata[j].tu;
			vertices[j].tv = vertexdata[j].tv;
		}
	}
	else if (VertexSize == 28 && VertexSubType == 6)
	{
		Vertex_28_6* vertexdata = (Vertex_28_6*)Vertices;
		int count = Number_Verts;
		for(int j=0;j<count;j++)
		{
			vertices[j].x = vertexdata[j].Coordinates.GetX();// + xyz[0];
			vertices[j].y = vertexdata[j].Coordinates.GetY();// + xyz[1];
			vertices[j].z = vertexdata[j].Coordinates.GetZ();// + xyz[2];
			
			vertices[j].x*=floats[0];
			vertices[j].y*=floats[1];
			vertices[j].z*=floats[2];

			vertices[j].x+=floats[4];
			vertices[j].y+=floats[5];
			vertices[j].z+=floats[6];

			vertices[j].nx = vertexdata[j].NormalVector.GetX();
			vertices[j].ny = vertexdata[j].NormalVector.GetY();
			vertices[j].nz = vertexdata[j].NormalVector.GetZ();
			
			vertices[j].tu = vertexdata[j].tu;
			vertices[j].tv = vertexdata[j].tv;
		}
	}
	else if (VertexSize == 28 && VertexSubType == 20)
	{
		Vertex_28_20* vertexdata = (Vertex_28_20*)Vertices;
		int count = Number_Verts;
		for(int j=0;j<count;j++)
		{
			vertices[j].x = vertexdata[j].Coordinates.x;// + xyz[0];
			vertices[j].y = vertexdata[j].Coordinates.y;// + xyz[1];
			vertices[j].z = vertexdata[j].Coordinates.z;// + xyz[2];
			
			vertices[j].x*=floats[0];
			vertices[j].y*=floats[1];
			vertices[j].z*=floats[2];

			vertices[j].x+=floats[4];
			vertices[j].y+=floats[5];
			vertices[j].z+=floats[6];

			vertices[j].nx = vertexdata[j].NormalVector.GetX();
			vertices[j].ny = vertexdata[j].NormalVector.GetY();
			vertices[j].nz = vertexdata[j].NormalVector.GetZ();
			
			vertices[j].tu = vertexdata[j].tu;
			vertices[j].tv = vertexdata[j].tv;
		}
	}
	else if (VertexSize == 28 && VertexSubType == 22)
	{
		Vertex_28_22* vertexdata = (Vertex_28_22*)Vertices;
		int count = Number_Verts;
		for(int j=0;j<count;j++)
		{
			vertices[j].x = vertexdata[j].Coordinates.x;// + xyz[0];
			vertices[j].y = vertexdata[j].Coordinates.y;// + xyz[1];
			vertices[j].z = vertexdata[j].Coordinates.z;// + xyz[2];
			
			vertices[j].x*=floats[0];
			vertices[j].y*=floats[1];
			vertices[j].z*=floats[2];

			vertices[j].x+=floats[4];
			vertices[j].y+=floats[5];
			vertices[j].z+=floats[6];

			vertices[j].nx = vertexdata[j].NormalVector.GetX();
			vertices[j].ny = vertexdata[j].NormalVector.GetY();
			vertices[j].nz = vertexdata[j].NormalVector.GetZ();
			
			vertices[j].tu = vertexdata[j].tu;			
			vertices[j].tv = vertexdata[j].tv;
		}
	}
	else if (VertexSize == 36 && VertexSubType == 4)
	{
		Vertex_36_4* vertexdata = (Vertex_36_4*)Vertices;
		int count = Number_Verts;
		for(int j=0;j<count;j++)
		{
			vertices[j].x = vertexdata[j].Coordinates.x;// + xyz[0];
			vertices[j].y = vertexdata[j].Coordinates.y;// + xyz[1];
			vertices[j].z = vertexdata[j].Coordinates.z;// + xyz[2];
			
			vertices[j].x*=floats[0];
			vertices[j].y*=floats[1];
			vertices[j].z*=floats[2];

			vertices[j].x+=floats[4];
			vertices[j].y+=floats[5];
			vertices[j].z+=floats[6];
			
			vertices[j].nx = vertexdata[j].NormalVector.x;
			vertices[j].ny = vertexdata[j].NormalVector.y;
			vertices[j].nz = vertexdata[j].NormalVector.z;
			
			vertices[j].tu = vertexdata[j].tu;
			vertices[j].tv = vertexdata[j].tv;
		}
	}
	else if (VertexSize == 36 && VertexSubType == 22)
	{
		Vertex_36_22* vertexdata = (Vertex_36_22*)Vertices;
		int count = Number_Verts;
		for(int j=0;j<count;j++)
		{
			vertices[j].x = vertexdata[j].Coordinates.x;// + xyz[0];
			vertices[j].y = vertexdata[j].Coordinates.y;// + xyz[1];
			vertices[j].z = vertexdata[j].Coordinates.z;// + xyz[2];
			
			vertices[j].x*=floats[0];
			vertices[j].y*=floats[1];
			vertices[j].z*=floats[2];

			vertices[j].x+=floats[4];
			vertices[j].y+=floats[5];
			vertices[j].z+=floats[6];

			vertices[j].nx = vertexdata[j].NormalVector.GetX();
			vertices[j].ny = vertexdata[j].NormalVector.GetY();
			vertices[j].nz = vertexdata[j].NormalVector.GetZ();
			
			vertices[j].tu = vertexdata[j].tu;
			vertices[j].tv = vertexdata[j].tv;
		}
	}
	else
	{
		throw gcnew System::InvalidOperationException(
			"Unknown compiledmodel vertex format");
	}
}

void SUBM::GetFaces(WORD* faces, VertexBones* bones)
{
#ifdef MESH_DEBUG
	Console::WriteLine("SUBM::GetFaces...");
	Console::WriteLine(" Face Count: {0}", Number_Faces);
	Console::WriteLine(" Header1 Count: {0}", Header_count1);
	Console::WriteLine(" Header2 Count: {0}", Header_count2);
	Console::WriteLine(" Header1 Count Dup: {0}", Header_count1_dupe);
	Console::WriteLine(" Header2 Count Dup: {0}", Header_count2_dupe);
	Console::WriteLine(" TriStrip Length: {0}", TriangleStripLength);
	Console::WriteLine(" Indices: {0}", 
		TriangleStripLength*DestroyableMeshLevel);
#endif

	WORD* data = TriangleStrips;

	/*
#ifdef _DEBUG
	for (int i = 0; i < TriangleStripLength*DestroyableMeshLevel; ++i)
		Console::WriteLine("  Index[{0}] = {1}", i, data[i]);
#endif
	*/

	// fill in REAL bone numbers after looking them up in the 
	// look-up table provided for every god damn fucking 
	// peice of shit sub-mesh sub-chunk...
		
	int facepos = 0;
	int face = 0;
	WORD wStart = 0;
	
	for (int i = 0; i < Header_count1; ++i)
	{
		SUBM_SUB_CHUNK_1* pChunk1 = &SubChunks1[i];

	#ifdef MESH_DEBUG
		Console::WriteLine("Chunk {0}", i);
		Console::WriteLine(" sFaceVertexIndices: {0}", 
			pChunk1->sFaceVertexIndices);
		Console::WriteLine(" Ignored_FaceVertexIndices: {0}", 
			pChunk1->Ignored_FaceVertexIndices);
		Console::WriteLine(" nFaceVertexIndices: {0}", 
			pChunk1->nFaceVertexIndices);
		Console::WriteLine(" End FaceVertexIndices: {0}", 
			pChunk1->sFaceVertexIndices+pChunk1->nFaceVertexIndices*3);
	#endif
		
		/*
		if (!bSet)
		{
			wStart = data[pChunk1->sFaceVertexIndices];
			bSet = true;
		}
		*/

		if (pChunk1->Ignored_FaceVertexIndices == 1)
		{
			bool bMod2 = (pChunk1->sFaceVertexIndices % 2) == 0;

			for (int j = pChunk1->sFaceVertexIndices;
				j < pChunk1->sFaceVertexIndices+pChunk1->nFaceVertexIndices; 
				++j)
			{
				if ((j % 2) == 0)
				{
					if (data[j] != data[j+1] && 
						data[j] != data[j+2] && 
						data[j+2] != data[j+1])
					{
						if (bMod2)
						{
							faces[facepos++] = data[j+2];
							faces[facepos++] = data[j+1];
							faces[facepos++] = data[j+0];
						}
						else
						{
							faces[facepos++] = data[j+0];
							faces[facepos++] = data[j+1];
							faces[facepos++] = data[j+2];
						}

						++face;
					}
				}
				else
				{
					if (data[j] != data[j+1] && 
						data[j] != data[j+2] && 
						data[j+2] != data[j+1])
					{
						if (bMod2)
						{
							faces[facepos++] = data[j+1];
							faces[facepos++] = data[j+2];
							faces[facepos++] = data[j+0];
						}
						else
						{
							faces[facepos++] = data[j+0];
							faces[facepos++] = data[j+2];
							faces[facepos++] = data[j+1];
						}

						++face;
					}
				}
			}
		}
		else
		{			
			for (int j = pChunk1->sFaceVertexIndices; 
				j < pChunk1->sFaceVertexIndices+
					pChunk1->nFaceVertexIndices*3; 
				j += 3)
			{
				faces[facepos++] = data[j+2];
				faces[facepos++] = data[j+1];
				faces[facepos++] = data[j+0];
				
				++face;

				/*
			#ifdef _DEBUG
				Console::WriteLine("Face {0}: {1}, {2}, {3}",
					face,
					faces[facepos-3],
					faces[facepos-2],
					faces[facepos-1]);
			#endif
				*/
			}
		}
	}

	WORD wSmallest = 65535;

	for (int i = 0; i < face*3; ++i)
	{
		if (faces[i] < wSmallest)
			wSmallest = faces[i];
	}

	for (int i = 0; i < face*3; ++i)
	{
		faces[i] -= wSmallest;
	}

	for(int i=0;i<Header_count2;i++)
	{
		DWORD dwFix = 0;
		
		if (SubChunks2[i]->Ignored_FaceVertexIndices == 1)
		{
			for(int j=SubChunks2[i]->sFaceVertexIndices;j<SubChunks2[i]->sFaceVertexIndices+SubChunks2[i]->nFaceVertexIndices;j++)
			{
				if (j%2 == 0)
				{
					if (data[j] != data[j+1] && data[j] != data[j+2] && data[j+2] != data[j+1])
					{
						if (SubChunks2[i]->sFaceVertexIndices%2 == 0)
						{
							faces[facepos++] = data[j+2]-dwFix;
							faces[facepos++] = data[j+1]-dwFix;
							faces[facepos++] = data[j+0]-dwFix;
						}
						else
						{
							faces[facepos++] = data[j+0]-dwFix;
							faces[facepos++] = data[j+1]-dwFix;
							faces[facepos++] = data[j+2]-dwFix;
						}
					}
				}
				else
				{
					if (data[j] != data[j+1] && data[j] != data[j+2] && data[j+2] != data[j+1])
					{
						if (SubChunks2[i]->sFaceVertexIndices%2 == 0)
						{
							faces[facepos++] = data[j+1]-dwFix;
							faces[facepos++] = data[j+2]-dwFix;
							faces[facepos++] = data[j+0]-dwFix;
						}
						else
						{
							faces[facepos++] = data[j+0]-dwFix;
							faces[facepos++] = data[j+2]-dwFix;
							faces[facepos++] = data[j+1]-dwFix;
						}
					}
				}
				
				if (bones)
				{
					
					if (!bones[faces[facepos-3]].converted)
					{
						bones[faces[facepos-3]].bones[0] = SubChunks2[i]->nBone[bones[faces[facepos-3]].bones[0]/3];
						bones[faces[facepos-3]].bones[1] = SubChunks2[i]->nBone[bones[faces[facepos-3]].bones[1]/3];
						bones[faces[facepos-3]].bones[2] = SubChunks2[i]->nBone[bones[faces[facepos-3]].bones[2]/3];
						bones[faces[facepos-3]].bones[3] = SubChunks2[i]->nBone[bones[faces[facepos-3]].bones[3]/3];
						bones[faces[facepos-3]].converted = true;
					}
					if (!bones[faces[facepos-2]].converted)
					{
						bones[faces[facepos-2]].bones[0] = SubChunks2[i]->nBone[bones[faces[facepos-2]].bones[0]/3];
						bones[faces[facepos-2]].bones[1] = SubChunks2[i]->nBone[bones[faces[facepos-2]].bones[1]/3];
						bones[faces[facepos-2]].bones[2] = SubChunks2[i]->nBone[bones[faces[facepos-2]].bones[2]/3];
						bones[faces[facepos-2]].bones[3] = SubChunks2[i]->nBone[bones[faces[facepos-2]].bones[3]/3];
						bones[faces[facepos-2]].converted = true;
					}
					if (!bones[faces[facepos-1]].converted)
					{
						bones[faces[facepos-1]].bones[0] = SubChunks2[i]->nBone[bones[faces[facepos-1]].bones[0]/3];
						bones[faces[facepos-1]].bones[1] = SubChunks2[i]->nBone[bones[faces[facepos-1]].bones[1]/3];
						bones[faces[facepos-1]].bones[2] = SubChunks2[i]->nBone[bones[faces[facepos-1]].bones[2]/3];
						bones[faces[facepos-1]].bones[3] = SubChunks2[i]->nBone[bones[faces[facepos-1]].bones[3]/3];
						bones[faces[facepos-1]].converted = true;
					}
				}
			}
		}
		else
		{
			//memcpy(faces+facepos, data+SubChunks2[i]->sFaceVertexIndices, SubChunks2[i]->nFaceVertexIndices*6);
			for(int j=SubChunks2[i]->sFaceVertexIndices;j<SubChunks2[i]->sFaceVertexIndices+SubChunks2[i]->nFaceVertexIndices*3;j+=3)
			{
				faces[facepos++] = data[j+2]-dwFix;
				faces[facepos++] = data[j+1]-dwFix;
				faces[facepos++] = data[j+0]-dwFix;
				
				if (bones)
				{
					
					if (!bones[faces[facepos-3]].converted)
					{
						bones[faces[facepos-3]].bones[0] = SubChunks2[i]->nBone[bones[faces[facepos-3]].bones[0]/3];
						bones[faces[facepos-3]].bones[1] = SubChunks2[i]->nBone[bones[faces[facepos-3]].bones[1]/3];
						bones[faces[facepos-3]].bones[2] = SubChunks2[i]->nBone[bones[faces[facepos-3]].bones[2]/3];
						bones[faces[facepos-3]].bones[3] = SubChunks2[i]->nBone[bones[faces[facepos-3]].bones[3]/3];
						bones[faces[facepos-3]].converted = true;
					}
					if (!bones[faces[facepos-2]].converted)
					{
						bones[faces[facepos-2]].bones[0] = SubChunks2[i]->nBone[bones[faces[facepos-2]].bones[0]/3];
						bones[faces[facepos-2]].bones[1] = SubChunks2[i]->nBone[bones[faces[facepos-2]].bones[1]/3];
						bones[faces[facepos-2]].bones[2] = SubChunks2[i]->nBone[bones[faces[facepos-2]].bones[2]/3];
						bones[faces[facepos-2]].bones[3] = SubChunks2[i]->nBone[bones[faces[facepos-2]].bones[3]/3];
						bones[faces[facepos-2]].converted = true;
					}
					if (!bones[faces[facepos-1]].converted)
					{
						bones[faces[facepos-1]].bones[0] = SubChunks2[i]->nBone[bones[faces[facepos-1]].bones[0]/3];
						bones[faces[facepos-1]].bones[1] = SubChunks2[i]->nBone[bones[faces[facepos-1]].bones[1]/3];
						bones[faces[facepos-1]].bones[2] = SubChunks2[i]->nBone[bones[faces[facepos-1]].bones[2]/3];
						bones[faces[facepos-1]].bones[3] = SubChunks2[i]->nBone[bones[faces[facepos-1]].bones[3]/3];
						bones[faces[facepos-1]].converted = true;
					}
				}
			}
		}
	}
}

void SUBM::GetVertexBones(VertexBones* bones)
{
	if (VertexSize == 20 && VertexSubType == 4)
	{
		Vertex_20_4* vertexdata = (Vertex_20_4*)Vertices;
		int count = Number_Verts;
		for(int j=0;j<count;j++)
		{
			memcpy(bones+j, vertexdata[j].BoneIndices, 8);
			bones[j].converted = false;
		}
	}
	else if (VertexSize == 28 && VertexSubType == 6)
	{
		Vertex_28_6* vertexdata = (Vertex_28_6*)Vertices;
		int count = Number_Verts;
		for(int j=0;j<count;j++)
		{
			memcpy(bones+j, vertexdata[j].BoneIndices, 8);
			bones[j].converted = false;
		}
	}
	else if (VertexSize == 28 && VertexSubType == 20)
	{
		Vertex_28_20* vertexdata = (Vertex_28_20*)Vertices;
		int count = Number_Verts;
		for(int j=0;j<count;j++)
		{
			memcpy(bones+j, vertexdata[j].BoneIndices, 8);
			bones[j].converted = false;
		}
	}
	else if (VertexSize == 36 && VertexSubType == 22)
	{
		Vertex_36_22* vertexdata = (Vertex_36_22*)Vertices;
		int count = Number_Verts;
		for(int j=0;j<count;j++)
		{
			memcpy(bones+j, vertexdata[j].BoneIndices, 8);
			bones[j].converted = false;
		}
	}
	else
	{
		throw gcnew System::Exception(
			"Sorry, just had an aneurysm: Unknown vertex format.");
	}
}

}
}
}