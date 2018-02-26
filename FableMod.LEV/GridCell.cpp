#include "Stdafx.h"
#include "GridCell.h"

namespace FableMod
{
namespace LEV
{

GridCell::GridCell() :
m_Value(0),
m_Level(0),
m_UIDList(gcnew FableMod::LEV::UIDList())
{
}

GridCell::~GridCell()
{
	delete m_UIDList;
}

}
}