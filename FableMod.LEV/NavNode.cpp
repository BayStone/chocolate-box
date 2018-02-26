#include "Stdafx.h"
#include "NavNode.h"
#include "NavigationNode.h"
#include "DynamicNode.h"
#include "StandardNode.h"
#include "Log.h"

//#define LEV_ORDERED_LINKS
//#define LOG_LINK_GENERATION

namespace FableMod
{
namespace LEV
{

static const float SIZES[] = 
	{ 32.0f, 16.0f, 8.0f, 4.0f, 2.0f, 1.0f, 0.5f };

static const int CELL_SIZES[] = 
	{ 64, 32, 16, 8, 4, 2, 1 };

NavNode::NavNode() :
m_Header(new LEVNodeHeader()),
m_Adjacents(gcnew List<unsigned __int32>())
{
}

NavNode::~NavNode()
{
	delete m_Header;
}

bool NavNode::Load(BufferStream^ fileIn)
{
	fileIn->Read(m_Header, sizeof(*m_Header));

	System::Diagnostics::Debug::Assert(m_Header->Layer < 7);

	return true;
}

bool NavNode::Save(FileStream^ fileOut)
{
	if (Type == NavNodeType::Blank)
	{
		// This is blank.

		unsigned char bytes[3];
		
		bytes[0] = 0;
		bytes[1] = 1;
		bytes[2] = 1;

		FileControl::Write(fileOut, bytes, sizeof(bytes));
	
		return true;
	}

	unsigned char bytes[4];

	bytes[2] = 0;

	if (Type == NavNodeType::Standard)
	{
		bytes[0] = 0;
		bytes[1] = (Layer == 0) ? 1 : 0;
		bytes[3] = 0;
	}
	else if (Type == NavNodeType::Navigation)
	{
		bytes[0] = 0;
		bytes[1] = 0;
		bytes[3] = 1;
	}
	else if (Type == NavNodeType::Dynamic)
	{
		bytes[0] = 1;
		bytes[1] = 0;
		bytes[3] = 1;
	}

	FileControl::Write(fileOut, bytes, sizeof(bytes));
	FileControl::Write(fileOut, m_Header, sizeof(*m_Header));
	
	return true;
}

void NavNode::SaveAdjacents(FileStream^ fileOut)
{
	for (unsigned int i = 0; i < AdjacentCount; ++i)
	{
		unsigned int uiID = Adjacents[i];
		FileControl::Write(fileOut, &uiID, sizeof(uiID));
	}				
}


void NavNode::Print(TextWriter^ writer)
{
	StringBuilder^ sb = gcnew StringBuilder();
	Print(sb);
	writer->Write(sb->ToString());
	sb = nullptr;
}

void NavNode::Print(StringBuilder^ sb)
{
	sb->AppendFormat("Type: {0}; ", Type.ToString());

	if (Type != NavNodeType::Blank)
	{
		sb->AppendFormat("ID: {0}; Size: {1}; Adj: {2}; ", 
			ID, Size, AdjacentCount);
	}
}

void NavNode::AddAdjacent(unsigned __int32 id)
{
	m_Adjacents->Add(id);
}

bool NavNode::HasAdjacent(unsigned __int32 id)
{
	return m_Adjacents->IndexOf(id) >= 0;
}

void NavNode::RemoveAdjacents()
{
	m_Adjacents->Clear();
}

float NavNode::Size::get()
{
	return SIZES[m_Header->Layer];
}

int NavNode::AdjacentCount::get()
{
	return m_Adjacents->Count;
}

unsigned __int32 NavNode::Adjacents::get(int index)
{
	return m_Adjacents[index];
}

static inline bool HitTestX(float x1, float x2, float x3, float x4)
{
	return
		(x3 <= x1 && x3 >= x2 && x4 <= x1 && x4 >= x2) ||
		(x1 <= x3 && x1 >= x4 && x2 <= x3 && x2 >= x4);
}

static inline bool HitTestY(float y1, float y2, float y3, float y4)
{
	return
		(y3 >= y1 && y3 <= y2 && y4 >= y1 && y4 <= y2) ||
		(y1 >= y3 && y1 <= y4 && y2 >= y3 && y2 <= y4);
}

static void FindLinks(
	List<NavNode^>^ nodes, 
	NavNode^ testNode,
	int direction)
{
	// Find links for pTestNode. This is a bit tricky.
	// There has to a simpler way to do this, but I can't
	// bother finding out what the hell it is.

	float fHalfSize = testNode->Size/2;

	const float fXX1 = testNode->X+fHalfSize;
	const float fXX2 = testNode->X-fHalfSize;
	const float fYY1 = testNode->Y-fHalfSize;
	const float fYY2 = testNode->Y+fHalfSize;

#ifdef LOG_LINK_GENERATION
	Log::WriteLine("FindLinks {0}...", testNode->ID);
#endif

	int count = nodes->Count;

	for (int i = 0; i < count; ++i)
	{
		NavNode^ pNavNode = nodes[i];
			
		if (pNavNode == testNode || 
			pNavNode->Type == NavNodeType::Standard ||
			pNavNode->Type == NavNodeType::Blank ||
			testNode->HasAdjacent(pNavNode->ID))
			continue;
		
		if (Math::Abs((int)testNode->Subset-(int)pNavNode->Subset) > 1)
			continue;

		fHalfSize = pNavNode->Size/2;

		float fX1 = pNavNode->X+fHalfSize;
		float fX2 = pNavNode->X-fHalfSize;
		float fY1 = pNavNode->Y-fHalfSize;
		float fY2 = pNavNode->Y+fHalfSize;
		
		// Test if they are they right next to each other. 
		// Where other ends, the other begins...

		bool bLink = false;
		
	#ifdef LOG_LINK_GENERATION
		if (fXX1 == fX2 && HitTestY(fY1,fY2,fYY1,fYY2))
		{
			Log::WriteLine(" Left: {0}", pNavNode->ID);
			bLink = true;
		}
		else if (fXX2 == fX1 && HitTestY(fY1,fY2,fYY1,fYY2))
		{
			Log::WriteLine(" Right: {0}", pNavNode->ID);
			bLink = true;
		}
		else if (fYY2 == fY1 && HitTestX(fX1,fX2,fXX1,fXX2))
		{
			Log::WriteLine(" Bottom: {0}", pNavNode->ID);
			bLink = true;
		}
		else if (fYY1 == fY2 && HitTestX(fX1,fX2,fXX1,fXX2))
		{		
			Log::WriteLine(" Top: {0}", pNavNode->ID);
			bLink = true;
		}
		
		if (bLink)
		{
			testNode->AddAdjacent(pNavNode->ID);
			pNavNode->AddAdjacent(testNode->ID);
		}
	#else
		bool bXOk = HitTestX(fX1,fX2,fXX1,fXX2);
		bool bYOk = HitTestY(fY1,fY2,fYY1,fYY2);
		
		bool bLeft   = (fXX1 == fX2 && bYOk);
		bool bRight  = (fXX2 == fX1 && bYOk);
		bool bTop    = (fYY1 == fY2 && bXOk);
		bool bBottom = (fYY2 == fY1 && bXOk);
		
		if (direction == 0)
		{
			bLink = (bLeft || bRight || bTop || bBottom);
		}
		else if (direction == 1)
			bLink = bLeft;
		else if (direction == 2)
			bLink = bBottom;
		else if (direction == 3)
			bLink = bRight;
		else if (direction == 4)
			bLink = bTop;

		if (bLink)
		{
			testNode->AddAdjacent(pNavNode->ID);
		#ifndef LEV_ORDERED_LINKS
			pNavNode->AddAdjacent(testNode->ID);
		#endif
		}
	#endif
	}
}

void NavNode::GenerateLinks(List<NavNode^>^ nodes)
{
	// Generate links for this node...

#ifdef LOG_LINK_GENERATION
	Log::Open();
#endif

	int count = nodes->Count;

	for (int i = 0; i < count; ++i)
	{
		NavNode^ navNode = nodes[i];
	
		if (navNode->Type == NavNodeType::Navigation ||
			navNode->Type == NavNodeType::Dynamic)
		{
		#ifndef LEV_ORDERED_LINKS
			FindLinks(nodes, navNode, 0);
		#else	
			// Doesn't work: 1-4, 4-1, (1,3,2,4), (1,4,3,2)
			FindLinks(nodes, navNode, 2);
			FindLinks(nodes, navNode, 4);
			FindLinks(nodes, navNode, 3);
			FindLinks(nodes, navNode, 1);
		#endif
		}
	}

#ifdef LOG_LINK_GENERATION
	Log::Close();
#endif
}

void NavNode::GenerateNodes(List<NavNode^>^ nodes, 
							unsigned char subset,
							Grid^ grid,
							unsigned __int32* puiID)
{
	const unsigned int uiCellsX = grid->Width/64;
	const unsigned int uiCellsY = grid->Height/64;
	
	int iCellY = 0;

	for (unsigned int i = 0; i < uiCellsY; ++i)
	{
		//int iCellX = grid->Width-64;
		int iCellX = 0;
		
		for (unsigned int j = 0; j < uiCellsX; ++j)
		{
			NavNode^ pNode = 
				Generate(nodes, subset, 0, grid, iCellX, iCellY, puiID);
			
			if (!pNode)
			{
				// Add a blank one.

				pNode = gcnew NavNode();

				pNode->Type = NavNodeType::Blank;

				nodes->Add(pNode);

				(*puiID)++;
			}

			iCellX += 64;
			//iCellX -= 64;
		}
		
		iCellY += 64;
	}
}

NavNode^ NavNode::Generate(
	List<NavNode^>^ nodes,
	unsigned char subset, 
	unsigned char layer, 
	Grid^ grid, 
	int x, 
	int y,
	unsigned __int32* puiID)
{
	// Generate a node for the given grid location.

	// 64, 32, ... 1
	const int iSize = CELL_SIZES[layer];
	
	const unsigned int uiState = grid->GetNavigationState(x,y,iSize);

	if (uiState == 0)
		return nullptr;

	const float fX = (float)x;
	const float fY = (float)y;
	const float fTopLeftX = ((float)grid->Width/2.0f)-fX/2.0f;
	const float fTopLeftY = fY/2.0f;
	const float fBottomRightX = fTopLeftX-SIZES[layer];
	const float fBottomRightY = fTopLeftY+SIZES[layer];

	const float fLocX = (fTopLeftX+fBottomRightX)/2.0f;
	const float fLocY = (fTopLeftY+fBottomRightY)/2.0f;
		
	if ((uiState & (unsigned int)GridState::FullDynamic) &&
		(uiState & (unsigned int)GridState::PartialSpecial) == 0 &&
		(uiState & (unsigned int)GridState::MultipleLevels) == 0)		
	{
		DynamicNode^ pNavNode = gcnew DynamicNode();
		
		nodes->Add(pNavNode);
		
		pNavNode->ID = (*puiID)++;
		pNavNode->Layer = layer;
		pNavNode->Subset = subset;
		pNavNode->X = fLocX;
		pNavNode->Y = fLocY;
		
		pNavNode->Level = grid->GetCellAt(x,y)->Level;

		if (uiState & (unsigned int)GridState::FullSpecial1)
			pNavNode->Special = 64;
		else if (uiState & (unsigned int)GridState::FullSpecial2)
			pNavNode->Special = 128;
		
		grid->AddToUIDList(pNavNode->UIDList,x,y,iSize);

		return pNavNode;
	}
	else if ((uiState & (unsigned int)GridState::FullNavigation) &&
			 (uiState & (unsigned int)GridState::PartialSpecial) == 0 &&
			 (uiState & (unsigned int)GridState::MultipleLevels) == 0)		
	{
		NavigationNode^ pNavNode = gcnew NavigationNode();
		
		nodes->Add(pNavNode);

		pNavNode->ID = (*puiID)++;
		pNavNode->Layer = layer;
		pNavNode->Subset = subset;
		pNavNode->X = fLocX;
		pNavNode->Y = fLocY;
		
		pNavNode->Level = grid->GetCellAt(x,y)->Level;

		if (uiState & (unsigned int)GridState::FullSpecial1)
			pNavNode->Special = 64;
		else if (uiState & (unsigned int)GridState::FullSpecial2)
			pNavNode->Special = 128;
		
		return pNavNode;
	}

	StandardNode^ pStandardNode = gcnew StandardNode();

	nodes->Add(pStandardNode);

	pStandardNode->Layer = layer;
	pStandardNode->Subset = subset;
	pStandardNode->X = fLocX;
	pStandardNode->Y = fLocY;
	
	const int iSize2 = iSize/2;
		
	NavNode^ pTmp = nullptr;
	
	const unsigned char layer1 = layer+1;

	// This ordering is OK!

	pTmp = Generate(
		nodes, subset, layer1, grid, x+iSize2, y, puiID);		
	pStandardNode->AddAdjacent(((pTmp != nullptr) ? pTmp->ID : 0));
				
	pTmp = Generate(
		nodes, subset, layer1, grid, x, y, puiID);		
	pStandardNode->AddAdjacent(((pTmp != nullptr) ? pTmp->ID : 0));
	
	pTmp = Generate(
		nodes, subset, layer1, grid, x+iSize2, y+iSize2, puiID);		
	pStandardNode->AddAdjacent(((pTmp != nullptr) ? pTmp->ID : 0));
	
	pTmp = Generate(
		nodes, subset, layer1, grid, x, y+iSize2, puiID);		
	pStandardNode->AddAdjacent(((pTmp != nullptr) ? pTmp->ID : 0));
	
	pStandardNode->ID = (*puiID)++;
	
	return pStandardNode;
}

}
}