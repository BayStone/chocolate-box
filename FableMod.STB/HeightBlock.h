#pragma once

using namespace System;
using namespace System::Collections::ObjectModel;
using namespace System::IO;
using namespace FableMod::CLRCore;
using namespace FableMod::BBB;

#include "HeightMesh.h"

namespace FableMod
{
namespace STB
{
	
public ref class HeightBlock
{
public:

	HeightBlock();

	virtual ~HeightBlock();
		
	property Vec3 Min
	{
		Vec3 get() { return m_Min; }
	}

	property Vec3 Max
	{
		Vec3 get() { return m_Max; }
	}
	
	property Vec3 Center
	{
		Vec3 get() { return Vec3(
			(m_Max.x+m_Min.x)*0.5f,
			(m_Max.y+m_Min.y)*0.5f,
			(m_Max.z+m_Min.z)*0.5f); }
	}

	property int MeshCount
	{
		int get() { return m_Meshes->Length; }
	}

	HeightMesh^ GetMesh(int index)
	{
		return m_Meshes[index];
	}

internal:

    void Read(FileStream^ file, BBBEntry^ entry);
    
	Vec3 m_Min;
	Vec3 m_Max;
	
	array<HeightMesh^>^ m_Meshes;
};

}
}