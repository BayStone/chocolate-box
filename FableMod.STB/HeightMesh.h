#pragma once

using namespace System;
using namespace System::Collections::ObjectModel;
using namespace System::IO;
using namespace FableMod::CLRCore;
using namespace FableMod::BBB;

#include "STBHeaders.h"

namespace FableMod
{
namespace STB
{

public value struct HeightVertex
{
	Vec3 Position;
	Vec3 Normal;
	float U;
	float V;
};

public ref class HeightMesh
{
public:

	HeightMesh();

	virtual ~HeightMesh();
	
	property int VertexCount
	{
		int get() { return m_Vertices->Count; }
	}

	HeightVertex GetVertex(int index)
	{
		return m_Vertices[index];
	}

	property int FaceCount
	{
		int get() { return m_Faces->Count; }
	}

	int GetFace(int index)
	{
		return m_Faces[index];
	}

	property UInt32 Texture
	{
		UInt32 get() { return m_Texture1; }
	}

internal:

    bool Read(BinaryReader^ reader, Vec3 blockMin, Vec3 blockMax);
    
	Collection<HeightVertex>^ m_Vertices;
	Collection<int>^ m_Faces;
	UInt32 m_Texture1;
};

}
}