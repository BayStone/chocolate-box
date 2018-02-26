#include "SelectionBox.h"
#include "Spatial.h"
#include "Helpers.h"

namespace FableMod
{
namespace Gfx
{

SelectionBox::SObjectInfo::~SObjectInfo()
{
}

SelectionBox::SelectionBox()
{
}

SelectionBox::~SelectionBox()
{
	Clear();
}

void SelectionBox::Initialize()
{
	AxisBox::Initialize();
}

void SelectionBox::Add(Spatial* pSpatial)
{
	if (!HasObject(pSpatial))
	{
		SObjectInfo* pInfo = new SObjectInfo;

		pInfo->spSpatial = pSpatial;
		pInfo->bEnabled = true;

		m_pObjects.Add(pInfo);

		UpdateBox();
		UpdateDiffs();
	}
}

void SelectionBox::Remove(Spatial* pSpatial)
{
	for (int i = 0; i < m_pObjects.GetCount(); ++i)
	{
		if (m_pObjects[i]->spSpatial == pSpatial)
		{
			delete m_pObjects[i];
			m_pObjects.Remove(i);

			UpdateBox();
			UpdateDiffs();

			return;
		}
	}
}

void SelectionBox::StartRotation()
{
	D3DXMatrixIdentity(&m_matRotation);

	for (int i = 0; i < m_pObjects.GetCount(); ++i)
	{
		SObjectInfo* pInfo = m_pObjects[i];

		// Save the matrix.
		pInfo->matRotation = pInfo->spSpatial->GetWorldRotationMatrix();
	}
}

void SelectionBox::AddRotation(const D3DXMATRIX& matRotation)
{
	m_matRotation = m_matRotation*matRotation;

	for (int i = 0; i < m_pObjects.GetCount(); ++i)
	{
		SObjectInfo* pInfo = m_pObjects[i];

		if (pInfo->bEnabled)
		{
			D3DXVECTOR3 vPoint(pInfo->vDiff);

			D3DXVec3TransformCoord(&vPoint, &vPoint, &m_matRotation);

			pInfo->spSpatial->SetWorldTranslation(GetCenter()+vPoint);
			pInfo->spSpatial->SetWorldRotation(
				pInfo->matRotation*m_matRotation);
		}
	}
}

void SelectionBox::SetCenter(const D3DXVECTOR3& vPoint)
{
	FLOAT fW = GetWidth()/2;
	FLOAT fH = GetHeight()/2;
	FLOAT fD = GetDepth()/2;

	m_vMin = D3DXVECTOR3(vPoint.x-fW, vPoint.y-fD, vPoint.z-fH);
	m_vMax = D3DXVECTOR3(vPoint.x+fW, vPoint.y+fD, vPoint.z+fH);

	for (int i = 0; i < m_pObjects.GetCount(); ++i)
	{
		SObjectInfo* pInfo = m_pObjects[i];

		if (pInfo->bEnabled)
			pInfo->spSpatial->SetWorldTranslation(vPoint+pInfo->vDiff);
	}
}

void SelectionBox::StopRotation()
{
	UpdateBox();
	UpdateDiffs();
}

void SelectionBox::Update()
{
	UpdateBox();
	UpdateDiffs();
}

void SelectionBox::Clear()
{
	for (int i = 0; i < m_pObjects.GetCount(); ++i)
		delete m_pObjects[i];

	m_pObjects.Clear();

	Initialize();
}

void SelectionBox::UpdateBox()
{
	Initialize();

	for (int i = 0; i < m_pObjects.GetCount(); ++i)
	{
		SObjectInfo* pInfo = m_pObjects[i];
		pInfo->spSpatial->GetBox(this);
	}
}

void SelectionBox::UpdateDiffs()
{
	const D3DXVECTOR3 vCenter(GetCenter());

	for (int i = 0; i < m_pObjects.GetCount(); ++i)
	{
		SObjectInfo* pInfo = m_pObjects[i];
		pInfo->vDiff = pInfo->spSpatial->GetWorldTranslation()-vCenter;
	}
}

bool SelectionBox::HasObject(Spatial* pSpatial) const
{
	for (int i = 0; i < m_pObjects.GetCount(); ++i)
	{
		SObjectInfo* pInfo = m_pObjects[i];

		if (pInfo->spSpatial == pSpatial)
			return true;
	}

	return false;
}

bool SelectionBox::IsEmpty() const
{
	return m_pObjects.GetCount() == 0;
}

int SelectionBox::GetNumObjects() const
{
	return m_pObjects.GetCount();
}

Spatial* SelectionBox::GetObjectAt(int index) const
{
	return m_pObjects[index]->spSpatial;
}

void SelectionBox::EnableAt(int index, bool bEnabled)
{
	m_pObjects[index]->bEnabled = bEnabled;
}

FLOAT SelectionBox::GetRadius() const
{
	float fMax = GetWidth();

	if (GetHeight() > fMax)
		fMax = GetHeight();
	
	if (GetDepth() > fMax)
		fMax = GetDepth();

	return fMax;
}

bool SelectionBox::HasEnabled() const
{
	for (int i = 0; i < m_pObjects.GetCount(); ++i)
	{
		SObjectInfo* pInfo = m_pObjects[i];

		if (pInfo->bEnabled)
			return true;
	}

	return false;
}

}
}