#pragma once

using namespace System;
using namespace System::Collections::ObjectModel;
using namespace System::IO;
using namespace FableMod::CLRCore;
using namespace FableMod::BBB;

#include "Flora.h"
#include "FloraNode.h"
#include "HeightBlock.h"

namespace FableMod
{
namespace STB
{

/// <summary>
/// Fable level graphics information. NOT FINISHED.
/// </summary>
public ref class Level
{
public:

	Level();

	virtual ~Level();

	property Vec3 MapOrigin
	{ 
		Vec3 get() 
		{ 
			return Vec3(
				m_pBaseHeader->MapWorldOffsetX,
				m_pBaseHeader->MapWorldOffsetY,
				0.0f);
		} 
	}

    property int FloraCount 
	{ 
		int get() { return m_Flora->Count; } 
	}

	Flora^ GetFlora(int index)
	{
		return m_Flora[index];
	}
	
	property int BlockCountX
	{ 
		int get() { return m_pBaseHeader->MapWidth/16; } 
	}

	property int BlockCountY
	{ 
		int get() { return m_pBaseHeader->MapHeight/16; } 
	}
	
	property int BlockCount
	{ 
		int get() { return BlockCountX*BlockCountY; } 
	}

	HeightBlock^ GetBlock(int index)
	{
		return m_Blocks[index];
	}
	
	List<Flora^>^ GetFloraOfType(UInt32 type);
	
	property FloraNode^ RootFloraNode
	{
		FloraNode^ get() { return m_RootFloraNode; }
	}

internal:

    void Read(FileStream^ file, BBBEntry^ entry);
	void ReadTerrain(FileStream^ file, BBBEntry^ entry);

private:
	
    STBLevBaseHeader* m_pBaseHeader;
    STBLevSectionHeader* m_pSectionHeader;
    STBLevOffsetHeader* m_pOffsetHeader;
	Collection<Flora^>^ m_Flora;
	array<HeightBlock^>^ m_Blocks;
	FloraNode^ m_RootFloraNode;
};

}
}