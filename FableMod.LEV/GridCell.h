#pragma once

#include "UIDList.h"

namespace FableMod
{
namespace LEV
{

public enum class CellMode : unsigned char 
{
	Navigation = 1,
	Dynamic = 2,
	Magic = 4,
	Special1 = 8,
	Special2 = (8 << 1),
};

public ref class GridCell
{
public:

	GridCell();

	~GridCell();

	property unsigned char Value
	{
		unsigned char get() { return m_Value; }
		void set(unsigned char value) { m_Value = value; }
	}

	property unsigned char Level
	{
		unsigned char get() { return m_Level; }
		void set(unsigned char value) { m_Level = value; }
	}

	property FableMod::LEV::UIDList^ UIDList
	{ 
		FableMod::LEV::UIDList^ get() { return m_UIDList; }
	}
	
protected:

	FableMod::LEV::UIDList^ m_UIDList;
	unsigned char m_Value;
	unsigned char m_Level;
};

}
}
