#include "Stdafx.h"
#include "NavSection.h"
#include "NavigationNode.h"
#include "DynamicNode.h"
#include "StandardNode.h"
#include "Structs.h"
#include "Log.h"

#ifdef _DEBUG
#define LOG_NODES
#endif

namespace FableMod
{
namespace LEV
{

NavSection::NavSection() :
m_Nodes(gcnew List<NavNode^>()),
m_Grids(gcnew List<Grid^>())
{
}

NavSection::~NavSection()
{
	DisposeGrids();
	DisposeNodes();
	
	delete m_Nodes;	
	delete m_Grids;
}

void NavSection::DisposeNodes()
{
	for (int i = 0; i < m_Nodes->Count; ++i)
		delete m_Nodes[i];

	m_Nodes->Clear();
}

void NavSection::DisposeGrids()
{
	for (int i = 0; i < m_Grids->Count; ++i)
		delete m_Grids[i];

	m_Grids->Clear();
}

static NavNode^ ReadNode(BufferStream^ fileIn)
{
	unsigned char aucBytes[3];

	fileIn->Read(aucBytes, sizeof(aucBytes));
		
	if (!(aucBytes[0] == 0 && aucBytes[1] == 1 && aucBytes[2] == 1))
	{
		unsigned char ucByte4;

		fileIn->Read(&ucByte4, sizeof(ucByte4));
	
		NavNode^ pNode = nullptr;

		if (aucBytes[0] == 0 && ucByte4 == 0)
		{
			pNode = gcnew StandardNode();
		}
		else if (aucBytes[0] == 0 && ucByte4 == 1)
		{
			pNode = gcnew NavigationNode();
		}
		else if (aucBytes[0] == 1 && ucByte4 == 1)
		{
			pNode = gcnew DynamicNode();
		}
		else
		{
			throw gcnew InvalidOperationException(
				"FABLEMOD_EXCEPTION_INVALID_NAVIGATION_NODE");
		}

		System::Diagnostics::Debug::Assert(aucBytes[1] <= 1);

		if (pNode)
		{	
			pNode->Load(fileIn);
			return pNode;
		}
	}
	else
	{
		NavNode^ pNode = gcnew NavNode();
		pNode->Type = NavNodeType::Blank;
		return pNode;
	}

	return nullptr;
}

bool NavSection::Load(BufferStream^ fileIn)
{
	LEVNavSectionHeader SectionHeader;

	fileIn->Read(&SectionHeader, sizeof(SectionHeader));
	
	m_Width = SectionHeader.Width;
	m_Height = SectionHeader.Height;

#ifndef LEV_NO_LOG
	Log::WriteLine("Section FileSize: {0}", SectionHeader.FileSize);
	Log::WriteLine("Section Version: {0}", SectionHeader.FileVersion);
	Log::WriteLine("Section Size: {0}x{1}", m_Width, m_Height);
	Log::WriteLine("Section NumLevels: {0}", SectionHeader.NumLevels);
#endif

	unsigned int uiNumInteractives = 0;
	fileIn->Read(&uiNumInteractives, sizeof(uiNumInteractives));

#ifndef LEV_NO_LOG
	Log::WriteLine("NumInteractives: {0}", uiNumInteractives);
#endif

	/*
	m_vInteractives.reserve(uiNumInteractives);
	
	for (unsigned int i = 0; i < uiNumInteractives; ++i)
	{
		SLEVInteractiveNode* pNode = new SLEVInteractiveNode();
		rFile.Read(pNode,sizeof(SLEVInteractiveNode));
		m_vInteractives.push_back(pNode);
	}
	*/
	fileIn->Seek(
		fileIn->Tell()+uiNumInteractives*sizeof(LEVInteractiveNode));

	unsigned int uiNumSubsets = 0;
	fileIn->Read(&uiNumSubsets, sizeof(uiNumSubsets));

	unsigned int uiNumNodes = 0;
	fileIn->Read(&uiNumNodes, sizeof(uiNumNodes));

#ifndef LEV_NO_LOG 
	Log::WriteLine("NumSubsets: {0}", uiNumSubsets);
	Log::WriteLine("NumNavNodes: {0}", uiNumNodes);
#endif
	
	for (unsigned int i = 0; i < uiNumNodes; ++i)
	{
		NavNode^ pNode = ReadNode(fileIn);

		if (pNode)
		{
			m_Nodes->Add(pNode);
		}
	}

#if defined(LOG_NODES) && !defined(LEV_NO_LOG)
	
	StringBuilder^ sb = gcnew StringBuilder();

	/*
	for (unsigned int i = 0; i < m_Nodes->Count; ++i)
	{
		sb->Length = 0;
		m_Nodes[i]->Print(sb);
		Log::WriteLine(sb->ToString());
	}

	sb = nullptr;
	*/
	
	Dictionary<UInt32, NavNode^>^ nodes = 
		gcnew Dictionary<UInt32, NavNode^>();

	for (unsigned int i = 0; i < m_Nodes->Count; ++i)
		nodes[m_Nodes[i]->ID] = m_Nodes[i];
	
	array<String^>^ grid = gcnew array<String^>(9);

	for (unsigned int i = 0; i < m_Nodes->Count; ++i)
	{
		NavNode^ pNode = m_Nodes[i];

		sb->Length = 0;

		pNode->Print(sb);
		
		for (unsigned int j = 0; j < 9; ++j)
			grid[j] = "";

		grid[4] = pNode->ID.ToString();

		NavNode^ pLink = nullptr;

		for (unsigned int j = 0; j < pNode->AdjacentCount; ++j)
		{
			unsigned __int32 uiAdj = pNode->Adjacents[j];

			if (uiAdj != 0)
			{
				pLink = nodes[uiAdj];

				int iIndex = 0;

				if (pLink->Y < pNode->Y)
					iIndex = 0;
				else if (pLink->Y == pNode->Y)
					iIndex = 3;
				else if (pLink->Y > pNode->Y)
					iIndex = 6;

				if (pLink->X < pNode->X)
					iIndex += 0;
				else if (pLink->X == pNode->X)
					iIndex += 1;
				else if (pLink->X > pNode->X)
					iIndex += 2;
			
				grid[iIndex] += j.ToString()+":"+pLink->ID.ToString()+" ";
			}
		}

		sb->AppendLine("Grid:");

		for (unsigned int y = 0; y < 3; ++y)
		{
			for (unsigned int x = 0; x < 3; ++x)
			{
				sb->AppendFormat("[{0,-15}]", grid[y*3+x]);
			}

			sb->AppendLine("");
		}
	
		Log::WriteLine(sb->ToString());
	}

	nodes = nullptr;

	sb = nullptr;
#endif

	CreateGrids();

	return true;
}

bool NavSection::Save(FileStream^ fileOut)
{
	if (!WriteInteractives(fileOut))
		return false;

	long lSubsetPos = (long)fileOut->Position;

	unsigned __int32 uiNumSubsets = 0;
	FileControl::Write(fileOut, &uiNumSubsets, sizeof(uiNumSubsets));

	unsigned __int32 uiNumNodes = NodeCount;
	FileControl::Write(fileOut, &uiNumNodes, sizeof(uiNumNodes));

	for (int i = 0; i < NodeCount; ++i)
	{
		NavNode^ pNode = Nodes[i];

		if (uiNumSubsets < pNode->Subset+1)
			uiNumSubsets = pNode->Subset+1;

		if (!pNode->Save(fileOut))
			return false;
	}

	if (uiNumNodes > 0)
	{
		long lPos = (long)fileOut->Position;
		fileOut->Position = lSubsetPos;
		FileControl::Write(fileOut, &uiNumSubsets, sizeof(uiNumSubsets));
		fileOut->Position = lPos;
	}

	return true;
}

bool NavSection::WriteInteractives(FileStream^ fileOut)
{
	unsigned int uiNumInteractives = 0;
	FileControl::Write(
		fileOut, &uiNumInteractives, sizeof(uiNumInteractives));
	/*
	unsigned int uiNumInteractives = 0;
	fwrite(&uiNumInteractives, sizeof(uiNumInteractives), 1, *ppFile);

	for (unsigned int i = 0; i < GetNumInteractives(); ++i)
	{
		fwrite(GetInteractiveAt(i), sizeof(SLEVInteractiveNode), 1, *ppFile);
	}
	*/

	return true;
}

void NavSection::AddGrid()
{
	m_Grids->Add(gcnew Grid(Width*2,Height*2));
}

Grid^ NavSection::GetGrid(int subset)
{
	while (subset >= m_Grids->Count)
	{
		AddGrid();
	}

	return Grids[subset];
}

/// Create the grids from the nodes that are attached to the section.
/// When done, the node list will be destroyed. 
void NavSection::CreateGrids()
{
	DisposeGrids();
	
	AddGrid();
	
	for (int i = 0; i < NodeCount; ++i)
	{
		AddNodeToGrid(Nodes[i]);
	}

	// We can now delete the nodes, we don't need them anymore.
	DisposeNodes();
}

/// Blit aka. copy the properties of the node into the grid.
void NavSection::BlitNodeToGrid(
	Grid^ grid,
	NavNode^ node, 
	unsigned char add, 
	unsigned char level)
{
	float fTopLeftX = node->X+node->Size/2;
	float fTopLeftY = node->Y-node->Size/2;
	float fBottomRightX = fTopLeftX-node->Size;
	float fBottomRightY = fTopLeftY+node->Size;

	const int iWidth = grid->Width/2;

	fTopLeftX = iWidth-fTopLeftX;
	fBottomRightX = iWidth-fBottomRightX;

	// Get the cells.
	const int iCellX1 = (int)(fTopLeftX*2);
	const int iCellY1 = (int)(fTopLeftY*2);
	const int iCellX2 = (int)(fBottomRightX*2);
	const int iCellY2 = (int)(fBottomRightY*2);

	for (int i = iCellX1; i < iCellX2; ++i)
	{
		for (int j = iCellY1; j < iCellY2; ++j)
		{
			grid->SetValueAt(i,j, add);
			grid->GetCellAt(i,j)->Level = level;
		}
	}
}

/// Add a node into the appropriate grid.
void NavSection::AddNodeToGrid(NavNode^ pNode)
{
	if (pNode->Type == NavNodeType::Blank)
		return;

	// Add a new grid if needed. Grids represent subsets.

	if (pNode->Subset >= m_Grids->Count)
	{
		while (pNode->Subset >= m_Grids->Count)
		{
			AddGrid();
		}
	}

	Grid^ pGrid = m_Grids[pNode->Subset];

	if (pNode->Type == NavNodeType::Navigation)
	{
		unsigned char ucValue = (unsigned char)CellMode::Navigation;

		NavigationNode^ pNavNode = (NavigationNode^)pNode;
		
		if (pNavNode->Special == 64)
			ucValue |= (unsigned char)CellMode::Special1;
		else if (pNavNode->Special == 128)
			ucValue |= (unsigned char)CellMode::Special2;
	
		BlitNodeToGrid(pGrid, pNode, ucValue, pNavNode->Level);
	}
	else if (pNode->Type == NavNodeType::Dynamic)
	{
		unsigned char ucValue = (unsigned char)CellMode::Dynamic;
		
		DynamicNode^ pDynamicNode = (DynamicNode^)pNode;
		
		if (pDynamicNode->Special == 64)
			ucValue |= (unsigned char)CellMode::Special1;
		else if (pDynamicNode->Special == 128)
			ucValue |= (unsigned char)CellMode::Special2;
		
		BlitNodeToGrid(pGrid, pNode, ucValue, pDynamicNode->Level);
	
		float fTopLeftX = pDynamicNode->X+pDynamicNode->Size/2;
		float fTopLeftY = pDynamicNode->Y-pDynamicNode->Size/2;
		
		fTopLeftX = (pGrid->Width/2)-fTopLeftX;

		// Get the cells.
		int iCellX1 = (int)(fTopLeftX*2);
		int iCellY1 = (int)(fTopLeftY*2);

		pGrid->SetUIDList(
			pDynamicNode->UIDList,
			iCellX1,iCellY1,pDynamicNode->Size*2);
	}

	pNode->RemoveAdjacents();
}

/// Generate the navigation nodes from the grids... it's quite heavy.
void NavSection::CreateNodes()
{
	unsigned char ucSubset = 0;
	
	// Ids are section based.
	unsigned __int32 uiID = 1;

	for (int i = 0; i < GridCount; ++i)
	{
		NavNode::GenerateNodes(m_Nodes, ucSubset++, Grids[i], &uiID);
	}
		
	Console::WriteLine("Generated {0} nodes.", m_Nodes->Count);

	bool bContent = false;

	for (int i = 0; i < NodeCount; ++i)
	{
		if (Nodes[i]->Type != NavNodeType::Blank)
		{
			bContent = true;
			break;
		}
	}

	if (bContent)
		NavNode::GenerateLinks(m_Nodes);
	else
		DisposeNodes();
}

NavNode^ NavSection::Nodes::get(int index)
{
	return m_Nodes[index];
}

Grid^ NavSection::Grids::get(int index)
{
	if (index >= GridCount)
		return nullptr;

	return m_Grids[index];
}

}
}