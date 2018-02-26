#pragma once

#include "Grid.h"
#include "NavNode.h"
#include "Structs.h"

using namespace System;
using namespace System::IO;
using namespace System::Collections::Generic;
using namespace FableMod::CLRCore;

namespace FableMod
{
namespace LEV
{

/// NavSection represents one section of npc navigation data.
/// i.e. each TNG section has its own NavSection.
public ref class NavSection
{
public:

	NavSection();

	~NavSection();

	bool Load(BufferStream^ fileIn);

	bool Save(FileStream^ fileOut);

	void AddGrid();

	void CreateGrids();

	void CreateNodes();

	Grid^ GetGrid(int subset);

	property int NodeCount { int get() { return m_Nodes->Count; } }
	
	property NavNode^ Nodes[int] { NavNode^ get(int index); }
	
	property int GridCount { int get() { return m_Grids->Count; } }
	
	property Grid^ Grids[int] { Grid^ get(int index); }
	
	property float Width
	{
		float get() { return m_Width; }
		void set(float value) { m_Width = value; }
	}

	property float Height
	{
		float get() { return m_Height; }
		void set(float value) { m_Height = value; }
	}

	property String^ Name
	{
		String^ get() { return m_Name; }
		void set(String^ name) { m_Name = gcnew String(name); }
	}

	protected:

protected:

	void DisposeNodes();
	void DisposeGrids();
	
	void AddNodeToGrid(NavNode^ node);
	
	void BlitNodeToGrid(
		Grid^ grid, 
		NavNode^ node,  
		unsigned char add, 
		unsigned char level);

	bool WriteInteractives(FileStream^ fileOut);

	List<Grid^>^ m_Grids;
	List<NavNode^>^ m_Nodes;
	float m_Width;
	float m_Height;
	String^ m_Name;
};

}
}
