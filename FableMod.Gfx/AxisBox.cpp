#include "AxisBox.h"

namespace FableMod
{
namespace Gfx
{

AxisBox::AxisBox()
{
	Initialize();
}

void AxisBox::Initialize()
{
	m_vMax = D3DXVECTOR3(-FLT_MAX,-FLT_MAX,-FLT_MAX);
	m_vMin = D3DXVECTOR3( FLT_MAX, FLT_MAX, FLT_MAX);
}

void AxisBox::Update(const D3DXVECTOR3& vPoint)
{
	if (m_vMin.x > vPoint.x)
		m_vMin.x = vPoint.x;
	if (m_vMin.y > vPoint.y)
		m_vMin.y = vPoint.y;
	if (m_vMin.z > vPoint.z)
		m_vMin.z = vPoint.z;

	if (m_vMax.x < vPoint.x)
		m_vMax.x = vPoint.x;
	if (m_vMax.y < vPoint.y)
		m_vMax.y = vPoint.y;
	if (m_vMax.z < vPoint.z)
		m_vMax.z = vPoint.z;
}

void AxisBox::Update(const AxisBox* pBox)
{
	Update(pBox->GetMin());
	Update(pBox->GetMax());
}

bool AxisBox::Inside(const D3DXVECTOR3& vPoint) const
{
	return false;
}

const D3DXVECTOR3& AxisBox::GetMin() const
{
	return m_vMin;
}

const D3DXVECTOR3& AxisBox::GetMax() const
{
	return m_vMax;
}

const FLOAT AxisBox::GetWidth() const
{
	return m_vMax.x-m_vMin.x;
}

const FLOAT AxisBox::GetHeight() const
{
	return m_vMax.z-m_vMin.z;
}

const FLOAT AxisBox::GetDepth() const
{
	return m_vMax.y-m_vMin.y;
}

D3DXVECTOR3 AxisBox::GetCenter() const
{
	return (m_vMin+m_vMax)/2;
}

}
}