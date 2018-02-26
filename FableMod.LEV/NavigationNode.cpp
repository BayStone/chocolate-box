#include "Stdafx.h"
#include "NavigationNode.h"

namespace FableMod
{
namespace LEV
{

NavigationNode::NavigationNode()
{
	Type = NavNodeType::Navigation;
	Special = 0;
	Level = 1;
}

bool NavigationNode::Load(BufferStream^ fileIn)
{
	NavNode::Load(fileIn);

	unsigned int uiLevel = 0;
	fileIn->Read(&uiLevel, sizeof(uiLevel));

	unsigned char ucSpecial = 0;
	fileIn->Read(&ucSpecial, sizeof(ucSpecial));

	Level = uiLevel;
	Special = ucSpecial;

	System::Diagnostics::Debug::Assert(
		Special == 0 || Special == 64 || Special == 128);

	// TODO: Can't figure out what to do with these.
	Level = 0;
	Special = 0;

	unsigned __int32 uiNumAdjacent = 0;
	fileIn->Read(&uiNumAdjacent, sizeof(uiNumAdjacent));
	
	for (unsigned __int32 i = 0; i < uiNumAdjacent; ++i)
	{
		unsigned __int32 uiID = 0;
		fileIn->Read(&uiID, sizeof(uiID));
		
		AddAdjacent(uiID);
	}

	return true;
}

bool NavigationNode::Save(FileStream^ fileOut)
{
	NavNode::Save(fileOut);

	unsigned int uiLevel = m_Level;
	FileControl::Write(fileOut, &uiLevel, sizeof(uiLevel));

	unsigned __int8 uiUnk2 = 0;

	if (Special > 0)
	{
		if (Subset > 0)
			uiUnk2 = 64;
		else
			uiUnk2 = 128;
	}

	FileControl::Write(fileOut, &uiUnk2, sizeof(uiUnk2));

	unsigned __int32 uiNumAdj = AdjacentCount;
	FileControl::Write(fileOut, &uiNumAdj, sizeof(uiNumAdj));
	
	SaveAdjacents(fileOut);

	return true;
}

void NavigationNode::Print(StringBuilder^ sb)
{
	NavNode::Print(sb);
	//sb->AppendFormat(" Special: {0}\r\n", Special);
	//sb->AppendFormat(" Level: {0}\r\n", Level);
}

}
}