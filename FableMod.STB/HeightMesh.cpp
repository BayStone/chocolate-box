#include "Stdafx.h"
#include "HeightMesh.h"
#include "STBHeaders.h"
#include "STBFile.h"

using namespace System::Diagnostics;

namespace FableMod
{
namespace STB
{
	
HeightMesh::HeightMesh()
	: m_Vertices(gcnew Collection<HeightVertex>())
	, m_Faces(gcnew Collection<int>())
{
}

HeightMesh::~HeightMesh()
{
}

bool HeightMesh::Read(BinaryReader^ reader, Vec3 blockMin, Vec3 blockMax)
{
	STBTerrainMeshHeader header;

	header.VertexCount = 0;
	header.FaceCount = 0;

	Int64 pos = reader->BaseStream->Position;

	FileControl::Read(reader->BaseStream, &header, sizeof(header));
	
	//System::Diagnostics::Debug::Assert(header.VertexCount <= 289);

	m_Texture1 = header.Texture1;
		
	/*
	Console::WriteLine(" vertexCount: {0}", header.VertexCount);
	Console::WriteLine(" faceCount: {0}", header.FaceCount);
	
	Console::WriteLine(" u1: {0}", header.Unknown1);
	Console::WriteLine(" u5: {0}", header.Unknown5);
	
	Console::WriteLine(" tex1: {0}", 
		STBFile::GetTextureEntry(header.Texture1));
	Console::WriteLine(" tex2: {0}",
		STBFile::GetTextureEntry(header.Texture2));
	Console::WriteLine(" tex3: {0}",
		STBFile::GetTextureEntry(header.Texture3));
	*/

	for (UInt16 v = 0; v < header.VertexCount; ++v)
	{
		STBLevVertex vertex;

		FileControl::Read(reader->BaseStream, &vertex, sizeof(vertex));

		Debug::Assert(vertex.X >= blockMin.x && vertex.X <= blockMax.x);
		Debug::Assert(vertex.Y >= blockMin.y && vertex.Y <= blockMax.y);
		Debug::Assert(vertex.Z >= blockMin.z && vertex.Z <= blockMax.z);

		HeightVertex heightVertex;

		heightVertex.Position = Vec3(vertex.X, vertex.Y, vertex.Z);
		heightVertex.Normal = Vec3(
			vertex.Normal.GetX(), 
			vertex.Normal.GetY(), 
			vertex.Normal.GetZ()); 

		Debug::Assert(heightVertex.Normal.z >= 0.0f);

		heightVertex.U = ((float)vertex.U-127.0f)/127.0f;
		heightVertex.V = ((float)vertex.V-127.0f)/127.0f;

		m_Vertices->Add(heightVertex);
	}
	
	UInt16 t = header.FaceCount+2;

	for (UInt16 f = 0; f < t; ++f)
	{
		if (reader->BaseStream->Position+2 > reader->BaseStream->Length)
		{
			Console::WriteLine("ERROR: No data left {0}/{1} {2} {3}", 
				reader->BaseStream->Position, 
				reader->BaseStream->Length,
				header.VertexCount,
				header.FaceCount);
		
			return false;
		}
	
		UInt16 idx = reader->ReadUInt16();
			
		m_Faces->Add(idx);

		if (idx >= header.VertexCount)
		{
			Console::WriteLine("ERROR: HeightMesh {0}/{1} {2} {3} {4}", 
				f, t, idx,
				header.VertexCount,
				header.FaceCount);
			
			UInt16 smallest = 65535;
			
			for (int i = 0; i < m_Faces->Count; ++i)
			{
				if (m_Faces[i] < smallest)
					smallest = m_Faces[i];
			}
			
			Console::WriteLine(" Smallest vertex index: {0}", smallest);
			
			return false;
		}
	}
	
	return true;
}

}
}