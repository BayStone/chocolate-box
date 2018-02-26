#pragma once

#include "NavigationNode.h"
#include "UIDList.h"

using namespace System;
using namespace System::IO;
using namespace FableMod::CLRCore;

namespace FableMod
{
namespace LEV
{

/// Leaf of the NPC navigation hierarchy. Dynamic node is usable
/// only when the attached objects (through the UID list) do not exist.
public ref class DynamicNode : public NavigationNode
{
public:

	DynamicNode()
		:
	m_UIDList(gcnew FableMod::LEV::UIDList())
	{
		Type = NavNodeType::Dynamic;
	}

	virtual ~DynamicNode()
	{
		delete m_UIDList;
	}

	virtual bool Load(BufferStream^ fileIn) override
	{
		NavigationNode::Load(fileIn);
		
		UIDList->Load(fileIn);
	
		return true;
	}

	virtual bool Save(FileStream^ fileOut) override
	{
		NavigationNode::Save(fileOut);

		UIDList->Save(fileOut);
	
		return true;
	}

	virtual void Print(StringBuilder^ sb) override
	{
		NavigationNode::Print(sb);
		sb->AppendFormat("UIDCount: {0}; ", UIDList->Count);
	}

	property FableMod::LEV::UIDList^ UIDList 
	{
		FableMod::LEV::UIDList^ get() { return m_UIDList; }
	}
	
protected:

	FableMod::LEV::UIDList^ m_UIDList;
};

}
}
