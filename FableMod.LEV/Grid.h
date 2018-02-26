#pragma once

#include "GridCell.h"

namespace FableMod
{
namespace LEV
{

public enum class GridState : unsigned int 
{
	Node = 0,
	NotEmpty = 1,
	FullNavigation = 2,
	FullDynamic = 4,
	MultipleLevels = 8,
	PartialSpecial = 16,
	FullSpecial1 = 32,
	FullSpecial2 = (32 << 1),
};

public ref class Grid
{
public:

	Grid(int iWidth, int iHeight);

	~Grid();

	void SetUIDList(
		UIDList^ List,
		int x, 
		int y,
		int size);

	void AddToUIDList(
		UIDList^ listOut,	
		int x, 
		int y, 
		int size);

	void SetValueAt(int iX, int iY, unsigned char ucValue);

	unsigned char GetValueAt(int iX, int iY);
	
	GridCell^ GetCellAt(int iX, int iY);
	
	int GetNavigationState(
		int iX, int iY, int iSize);
	
	property int Width { int get() { return m_Width; } }

	property int Height { int get() { return m_Height; } }

internal:

	void ToData(unsigned char* aucData);

	void FromData(unsigned char* aucData);

protected:

	array<GridCell^>^ m_Cells;
	int m_Width;
	int m_Height;
};


}
}