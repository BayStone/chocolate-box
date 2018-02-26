#pragma once

#include "GfxConf.h"
#include "TerrainPage.h"
#include "Vector.h"
#include "Mesh.h"
#include "EditableTexture.h"

namespace FableMod
{
namespace Gfx
{

/// Terrain implements a section terrain that's built from terrain pages.
/// Terrain is loaded from a LEV heightfield.
class GFX_API Terrain : public Mesh
{
public:

	Terrain();

	virtual ~Terrain();

	virtual void Dispose();

	virtual void GetRenderList(Camera* pCamera);

	void Create(
		const FLOAT* pfHeightfield,
		unsigned int uiWidth,
		unsigned int uiHeight);

	void BeginPick();

	void EndPick();

	void Stitch(Terrain* pTerrain);

	void SaveHeightfield(LPCTSTR pszFileName);

	bool GetLocation(
		const D3DXVECTOR3& vOrigin,
		const D3DXVECTOR3& vDirection,
		unsigned int& ruiX,
		unsigned int& ruiY);

	bool GetLocation(
		const D3DXVECTOR3& vPoint,
		unsigned int& ruiX,
		unsigned int& ruiY);

	bool GetLocation(
		const D3DXVECTOR3& vOrigin,
		const D3DXVECTOR3& vDirection,
		unsigned int uiWidth,
		unsigned int uiHeight,
		unsigned int& ruiX,
		unsigned int& ruiY);

	bool GetLocation(
		unsigned int uiWidth,
		unsigned int uiHeight,
		const D3DXVECTOR3& vPoint, 
		unsigned int& ruiX,
		unsigned int& ruiY) const;

	bool IsOver(const D3DXVECTOR3& vPoint);

	bool GetHeight(
		const D3DXVECTOR2& vLocation,
		FLOAT* pfHeight) const;

    void SetPixelTolerance(Camera* pCamera, FLOAT fTolerance);
	FLOAT GetPixelTolerance() const;

	EditableTexture* GetTexture(unsigned int uiIndex = 0) 
	{ 
		return m_spTexture[uiIndex]; 
	}

	void SetTexture(unsigned int uiIndex, EditableTexture* pTexture);
	
	void SetTextureSpacing(
		unsigned char ucSet, 
		const D3DXVECTOR2& vSize,
		EditableTexture* pTexture);

	unsigned int GetTextureSize(unsigned char ucMultiplier = 1) const;

	FLOAT GetWorldSizeX() const { return m_uiFieldWidth*m_fSpacing; }

	FLOAT GetWorldSizeY() const { return m_uiFieldHeight*m_fSpacing; }

	FLOAT GetSpacing() const { return m_fSpacing; }

	unsigned int GetWidth() const { return m_uiFieldWidth; }

	unsigned int GetHeight() const { return m_uiFieldHeight; }

	D3DXVECTOR3 GetWorldCenter() const;

	static unsigned char GetTextureMultiplier();

protected:

	virtual void UpdateWorldTransform();

	virtual void OnRender(Camera* pCamera);

	virtual void UpdateWorldBound();

	static void GetPageCorners(
		Terrain* pTerrain, 
		TerrainPage* pPage,
		FLOAT& rfLeft,
		FLOAT& rfTop,
		FLOAT& rfRight,
		FLOAT& rfBottom);

	Vector<TerrainPage*> m_Pages;
	EditableTexturePtr m_spTexture[2];
	unsigned int m_uiFieldWidth;
	unsigned int m_uiFieldHeight;
	FLOAT m_fSpacing;
	bool m_bSetPixelTolerance;
	unsigned char m_ucCurrentUpdate;	
	bool m_bUpdateTerrain;
	bool m_bBuildModelBound;
	D3DMATERIAL9 m_Material;
	D3DXMATRIX m_matWorldInverse;
	bool m_bUpdateBlocked;

	LPDIRECT3DVERTEXBUFFER9 m_pGridVB;

	static unsigned char sm_ucUpdateFrequency;
	static unsigned char sm_ucTextureMultiplier;
};

GFX_DECLARE_SMARTPOINTER(Terrain);

}
}
