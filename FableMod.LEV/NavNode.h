#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Text;
using namespace FableMod::CLRCore;

#include "Grid.h"
#include "Structs.h"

namespace FableMod
{
namespace LEV
{

public enum class NavNodeType
{
	Blank = 0,
	Standard,
	Navigation,
	Dynamic,
};

ref class NavNode;

/// <summary>
/// NPC navigation base class.
/// </summary>
public ref class NavNode
{
public:

	NavNode();

	virtual ~NavNode();

	virtual bool Load(BufferStream^ fileIn);

	virtual bool Save(FileStream^ fileOut);

	void Print(TextWriter^ writer);

	virtual void Print(StringBuilder^ sb);

	void SaveAdjacents(FileStream^ fileOut);

	void AddAdjacent(unsigned __int32 id);

	bool HasAdjacent(unsigned __int32 id);

	void RemoveAdjacents();

	property int AdjacentCount { int get(); }

	property unsigned __int32 Adjacents[int] 
	{ 
		unsigned __int32 get(int index); 
	}

	property unsigned __int32 ID
	{
		unsigned __int32 get() { return m_Header->ID; }
		void set(unsigned __int32 id) { m_Header->ID = id; }
	}

	property unsigned __int8 Layer
	{
		unsigned __int8 get() { return m_Header->Layer; }
		void set(unsigned __int8 layer) { m_Header->Layer = layer; }
	}

	property float Size
	{
		float get();
	}

	property float X
	{
		float get() { return m_Header->X; }
		void set(float x) { m_Header->X = x; }
	}

	property float Y
	{
		float get() { return m_Header->Y; }
		void set(float y) { m_Header->Y = y; }
	}

	property unsigned __int32 Subset
	{
		unsigned __int32 get() { return m_Header->Subset; }
		void set(unsigned __int32 subset) { m_Header->Subset = subset; }
	}

	property NavNodeType Type
	{
		NavNodeType get() { return m_Type; }
		void set(NavNodeType value) { m_Type = value; }
	}

	static void GenerateLinks(List<NavNode^>^ nodes);

	static void GenerateNodes(
		List<NavNode^>^ nodes, 
		unsigned char subset,
		Grid^ grid,
		unsigned __int32* puiID);

	static NavNode^ Generate(
		List<NavNode^>^ nodes, 
		unsigned char subset, 
		unsigned char layer, 
		Grid^ grid, 
		int x, 
		int y,
		unsigned __int32* puiID);

protected:

	NavNodeType m_Type;
	LEVNodeHeader* m_Header;
	List<unsigned __int32>^ m_Adjacents;
};

}
}