#pragma once

#include "GfxConf.h"
#include "TerrainPage.h"
#include "Vector.h"

namespace FableMod
{

namespace LEV
{

class LEVFile;

}

namespace Gfx
{

class Mesh;
class EditableTexture;

/// Map implements a section terrain that's built from terrain pages.
/// Terrain is loaded from a LEV heightfield.
class GFX_API Map : public Mesh
{
public:

	Map();

	virtual void Dispose();

	virtual void GetRenderList(Camera* pCamera);

	void Create(class FableMod::LEV::LEVFile* pLEVFile);

	void Stitch(Terrain* pTerrain);

	void SaveHeightfield(LPCTSTR pszFileName);

	void ApplyWalkable(class FableMod::LEV::LEVFile* pLEVFile);
	
	bool GetLocation(
		const D3DXVECTOR3& vPoint,
		unsigned int& ruiX,
		unsigned int& ruiY);
	
	FLOAT GetHeight(const D3DXVECTOR2& vLocation) const;

    void SetPixelTolerance(Camera* pCamera, FLOAT fTolerance);
	FLOAT GetPixelTolerance() const;

	EditableTexture* GetTexture() { return m_pTexture; }

	unsigned int GetTextureSize() const;

protected:

	virtual void OnRender(Camera* pCamera);

	virtual void UpdateWorldBound();

	Vector<TerrainPage*> m_Pages;
	unsigned int m_uiPagesX;
	unsigned int m_uiPagesY;
	EditableTexture* m_pTexture;
	
	unsigned int m_uiFieldWidth;
	unsigned int m_uiFieldHeight;

	bool m_bSetPixelTolerance;
};

}
}
