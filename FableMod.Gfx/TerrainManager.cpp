#include "Stdafx.h"
#include "EditableTexture.h"
#include "D3DModel.h"
#include "Mesh.h"
#include "Camera.h"
#include "TerrainPage.h"
#include "Terrain.h"
#include "TerrainManager.h"
#include "Consts.h"
#include "Settings.h"

namespace FableMod
{
namespace Gfx
{

TerrainManager::TerrainManager() :
m_fMaxHeight(-1),
m_aTerrain(4,4)
{
}

void TerrainManager::Add(Terrain* pTerrain)
{
	pTerrain->Update();

	m_aTerrain.Add(pTerrain);
}

void TerrainManager::Stitch()
{
	for (int i = 0; i < m_aTerrain.GetCount(); ++i)
	{
		for (int j = 0; j < m_aTerrain.GetCount(); ++j)
		{
			if (i != j)
				m_aTerrain[i]->Stitch(m_aTerrain[j]);
		}
	}
}

void TerrainManager::Remove(Terrain* pTerrain)
{
	int iIndex = m_aTerrain.IndexOf(pTerrain);

	if (iIndex > 0)
		m_aTerrain.Remove(iIndex);
}

void TerrainManager::RemoveAll()
{
	m_aTerrain.Clear();
}

Terrain* TerrainManager::GetLocation(
	const D3DXVECTOR3& vOrigin,
	const D3DXVECTOR3& vDirection,
	unsigned int& ruiX,
	unsigned int& ruiY,
	D3DXVECTOR3* pvPoint) const
{
	FableMod::Gfx::PickData Data;

	for (int i = 0; i < m_aTerrain.GetCount(); ++i)
	{
		Data.pMesh = 0;

		m_aTerrain[i]->Pick(vOrigin, vDirection, Data);

		if (Data.pMesh)
		{
			if (pvPoint)
				*pvPoint = Data.vPoint;

			if (m_aTerrain[i]->GetLocation(Data.vPoint, ruiX, ruiY))
				return m_aTerrain[i];
		}
	}

	return 0;
}

D3DXVECTOR3 TerrainManager::GetWorldCenter() const
{
	D3DXVECTOR3 vCenter(0.0f, 0.0f, 0.0f);

	if (m_aTerrain.GetCount() == 0)
		return vCenter;

	for (int i = 0; i < m_aTerrain.GetCount(); ++i)
	{
		vCenter += m_aTerrain[i]->GetWorldCenter();
	}

	vCenter /= m_aTerrain.GetCount();

	return vCenter;
}

bool TerrainManager::GetHeight(const D3DXVECTOR2& vLocation,
							   FLOAT* pfHeight) const
{
	for (int i = 0; i < m_aTerrain.GetCount(); ++i)
	{
		if (m_aTerrain[i]->GetHeight(vLocation, pfHeight))
			return true;
	}

	return false;
}

FLOAT TerrainManager::GetMaxHeight()
{
	if (m_fMaxHeight < 0)
	{
		m_fMaxHeight = Settings::GetInstance()->GetFloat(
			_T("Terrain"), _T("MaxHeight"), 100.0f);
	}

	return m_fMaxHeight;
}

TerrainManager* TerrainManager::GetInstance()
{
	static TerrainManager s_Manager;
	return &s_Manager;
}

}
}