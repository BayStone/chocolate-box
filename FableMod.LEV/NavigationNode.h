#pragma once

#include "NavNode.h"

using namespace System;
using namespace System::IO;
using namespace FableMod::CLRCore;

namespace FableMod
{
namespace LEV
{

/// Leaf of NPC navigation node hierarchy.
public ref class NavigationNode : public NavNode
{
public:

	NavigationNode();

	virtual bool Load(BufferStream^ fileIn) override;
	
	virtual bool Save(FileStream^ fileOut) override;

	virtual void Print(StringBuilder^ sb) override;

	property unsigned char Special
	{
		unsigned char get() { return m_Special; }
		void set(unsigned char value) { m_Special = value; }
	}

	property unsigned int Level
	{
		unsigned int get() { return m_Level; }
		void set(unsigned int value) { m_Level = value; }
	}

protected:

	unsigned char m_Special;
	unsigned int m_Level;
};

}
}
