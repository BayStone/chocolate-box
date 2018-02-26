#pragma once

#include "NavNode.h"

using namespace System;
using namespace System::IO;
using namespace FableMod::CLRCore;

namespace FableMod
{
namespace LEV
{

public ref class StandardNode : public NavNode
{
public:

	StandardNode()
	{
		Type = NavNodeType::Standard;
	}

	virtual bool Load(BufferStream^ fileIn) override
	{
		NavNode::Load(fileIn);

		unsigned __int32 auiAdjacent[4];
		fileIn->Read(auiAdjacent, sizeof(auiAdjacent));
	
		AddAdjacent(auiAdjacent[0]);
		AddAdjacent(auiAdjacent[1]);
		AddAdjacent(auiAdjacent[2]);
		AddAdjacent(auiAdjacent[3]);
	
		return true;
	}

	virtual bool Save(FileStream^ fileOut) override
	{
		NavNode::Save(fileOut);

		SaveAdjacents(fileOut);

		return true;
	}
};

}
}
