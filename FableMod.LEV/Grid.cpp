#include "Stdafx.h"
#include "Grid.h"

namespace FableMod
{
namespace LEV
{

Grid::Grid(int uiWidth, int uiHeight) :
m_Width(uiWidth),
m_Height(uiHeight)
{
	m_Cells = gcnew array<GridCell^>(m_Width*m_Height);

	for (int i = 0; i < m_Cells->Length; ++i)
	{
		m_Cells[i] = gcnew GridCell();
	}
}

Grid::~Grid()
{
	for (int i = 0; i < m_Cells->Length; ++i)
	{
		delete m_Cells[i];
	}

	delete m_Cells;
}

void Grid::SetValueAt(int x, int y, unsigned char value)
{
	GetCellAt(x,y)->Value = value;
}

unsigned char Grid::GetValueAt(int x, int y)
{
	return GetCellAt(x,y)->Value;
}

GridCell^ Grid::GetCellAt(int x, int y)
{
	return m_Cells[y*Width+x];
}

void Grid::SetUIDList(UIDList^ list,
					  int x, 
					  int y,
					  int size)
{
	for (int i = y; i < y+size; ++i)
	{
		for (int j = x; j < x+size; ++j)
		{
			GetCellAt(j,i)->UIDList->Copy(list);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
/// Get UIDs from grid cells to the given UIDList object.
/// @param rList [out] UIDList where the cell UIDs are added.
//////////////////////////////////////////////////////////////////////////////
void Grid::AddToUIDList(UIDList^ listOut,
						int x, 
						int y,
						int size)
{
	for (int i = y; i < y+size; ++i)
	{
		for (int j = x; j < x+size; ++j)
		{
			listOut->Merge(GetCellAt(j,i)->UIDList);
		}
	}
}

int Grid::GetNavigationState(int x, int y, int size)
{
	int auiCounts[4] = { 0,0,0,0 };
	
	int uiResult = 0;
	
	int level = GetCellAt(x,y)->Level;

	for (int i = y; i < y+size; ++i)
	{
		for (int j = x; j < x+size; ++j)
		{
			GridCell^ cell = GetCellAt(j,i);
			
			const unsigned char value = cell->Value; 
			
			if (value & (unsigned char)CellMode::Navigation)
			{
				uiResult |= (unsigned int)GridState::NotEmpty;
				++auiCounts[0];
			}
			else if (value & (unsigned char)CellMode::Dynamic)
			{
				uiResult |= (unsigned int)GridState::NotEmpty;
				++auiCounts[1];
			}

			if (value & (unsigned char)CellMode::Special1)
				++auiCounts[2];

			if (value & (unsigned char)CellMode::Special2)
				++auiCounts[3];

			if (cell->Level != level)
				level = -1;
		}
	}

	const int uiNumCells = size*size;

	if (auiCounts[1] == uiNumCells)
		uiResult |= (unsigned int)GridState::FullDynamic;
	else if (auiCounts[0] == uiNumCells)
		uiResult |= (unsigned int)GridState::FullNavigation;
	
	if (uiResult & (unsigned int)GridState::NotEmpty)
	{
		if (auiCounts[2] == uiNumCells)
			uiResult |= (unsigned int)GridState::FullSpecial1;
		else if (auiCounts[3] == uiNumCells)
			uiResult |= (unsigned int)GridState::FullSpecial2;
		else if (auiCounts[2] > 0 || auiCounts[3] > 0)
			uiResult |= (unsigned int)GridState::PartialSpecial;

		if (level < 0)
			uiResult |= (unsigned int)GridState::MultipleLevels;
	}

	return uiResult;
}

//////////////////////////////////////////////////////////////////////////////
/// Apply cell values to data. Data is assumed to be 
/// the same size as the grid.
//////////////////////////////////////////////////////////////////////////////
void Grid::ToData(unsigned char* aucData)
{
	for (int i = 0; i < Width; ++i)
	{
		for (int j = 0; j < Height; ++j)
		{
			GridCell^ cell = GetCellAt(i,j);
			aucData[j*m_Width+i] = cell->Value;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
/// Apply cell values from data. Data is assumed to be 
/// the same size as the grid.
//////////////////////////////////////////////////////////////////////////////
void Grid::FromData(unsigned char* aucData)
{
	for (int i = 0; i < Width; ++i)
	{
		for (int j = 0; j < Height; ++j)
		{
			GetCellAt(i,j)->Value = aucData[j*Width+i];
		}
	}
}

}
}
