#pragma once

#include "Terrain.h"

namespace FableMod
{
namespace Gfx
{

/// Separate terrain manager class.
/// Meant to be used on the "region" level.
class GFX_API TerrainManager
{
public:

	void Add(Terrain* pTerrain);

	void Remove(Terrain* pTerrain);

	void RemoveAll();

	static TerrainManager* GetInstance();

	void Stitch();

	Terrain* GetLocation(
		const D3DXVECTOR3& vOrigin,
		const D3DXVECTOR3& vDirection,
		unsigned int& ruiX,
		unsigned int& ruiY,
		D3DXVECTOR3* pvPoint = 0) const;

	bool GetHeight(
		const D3DXVECTOR2& vLocation,
		FLOAT* pfHeight) const;

	D3DXVECTOR3 GetWorldCenter() const;

	FLOAT GetMaxHeight();

protected:
	
	TerrainManager();

	Vector<Terrain*> m_aTerrain;
	FLOAT m_fMaxHeight;
};

}
}