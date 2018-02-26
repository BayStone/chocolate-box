#pragma once

#include "GfxConf.h"

namespace FableMod
{

namespace LEV
{

class LEVFile;

}

namespace Gfx
{
	
class Camera;
class TerrainBlock;
class TerrainVertex;
class Terrain;
class D3DModel;
class BoundSphere;

/// TerrainPage is a square block of terrain.
class GFX_API TerrainPage
{
public:
    
	TerrainPage(
		Terrain* pTerrain,
		unsigned short usSize, 
		FLOAT* afHeight,
		unsigned int uiHeightStride,
		const D3DXVECTOR2& vOrigin, 
		FLOAT fSpacing,
		const D3DXVECTOR2& vTextureOrigin0, 
		const D3DXVECTOR2& vTextureSpacing0,
		const D3DXVECTOR2& vTextureOrigin1, 
		const D3DXVECTOR2& vTextureSpacing1);

    virtual ~TerrainPage();

	void RenderBlocks();

    unsigned short GetSize() const;
    const float* GetHeights() const;
    const D3DXVECTOR2& GetOrigin() const;
    FLOAT GetSpacing() const;
	
	bool GetGridLocation(
		const D3DXVECTOR2& vLocation, 
		unsigned short* pusCol,
		unsigned short* pusRow) const;

    void SetPixelTolerance(Camera* pCamera, FLOAT fTolerance);
    FLOAT GetPixelTolerance() const;

    bool GetHeight(
		const D3DXVECTOR2& vLocation,
		FLOAT* pfHeight) const;

	FLOAT GetHeightAt(unsigned short usX, unsigned short usY) const;

	void ResetBlocks();
    
	void Simplify(
		Camera* pCamera,
		const D3DXVECTOR3& vModelEye,
        const D3DXVECTOR3& vModelDir, 
		bool bCloseAssumption);

	BoundSphere* GetBound() { return m_pBound; }
	
	Terrain* GetTerrain() const { return m_pTerrain; }

	void StitchLR(TerrainPage* pRight);
    void StitchTB(TerrainPage* pBottom);
    void UnstitchLR(TerrainPage* pRight);
    void UnstitchTB(TerrainPage* pBottom);

    FLOAT GetX(unsigned char ucX) const;
    FLOAT GetY(unsigned char ucY) const;
    FLOAT GetHeight(unsigned int uiIndex) const;
    FLOAT GetTextureX(unsigned char ucSet, unsigned char ucIndex) const;
    FLOAT GetTextureY(unsigned char ucSet, unsigned char ucIndex) const;
    D3DCOLOR GetHeightColor(unsigned int uiIndex) const;

	void SetTextureOrigin(unsigned char ucSet, const D3DXVECTOR2& vOrigin);
	void SetTextureSpacing(unsigned char ucSet, const D3DXVECTOR2& vSpacing);

protected:

    friend TerrainBlock;

    void InitializeDerivedData();

    void AddToQueue(unsigned short usBlock);
    unsigned short RemoveFromQueue();
    unsigned short ReadFromQueue(unsigned short usIndex);

    bool IntersectFrustum(const Camera* pCamera);

    void SimplifyBlocks(
		const Camera* pCamera,
        const D3DXVECTOR3& vModelEye, 
		const D3DXVECTOR3& vModelDir,
        bool bCloseAssumption);

    void SimplifyVertices(
		const D3DXVECTOR3& vModelEye,
        const D3DXVECTOR3& vModelDir, 
		bool bCloseTerrainAssumption);

	void RenderBlock(TerrainBlock& rBlock);
    void RenderTriangle(
		unsigned short usT, 
		unsigned short usL,
        unsigned short usR);
    
    unsigned short m_usSize, m_usSizeM1;
	FLOAT* m_afHeight;
    D3DXVECTOR2 m_vOrigin;
    FLOAT m_fSpacing;
    FLOAT m_fInvSpacing;
    D3DXVECTOR2 m_vTextureOrigin[2];
	D3DXVECTOR2 m_vTextureSpacing[2];

    FLOAT m_fPixelTolerance, m_fWorldTolerance;
    unsigned short* m_ausLookup;

    // (2^p+1) by (2^p+1) array of vertices, row-major order
    TerrainVertex* m_aTVertex;
	unsigned int m_uiVertexQuantity;

    // quadtree of blocks
    unsigned short m_usBlockQuantity;
    TerrainBlock* m_aBlocks;

    // circular queue of indices for active blocks
    unsigned short* m_ausQueue;
    unsigned short m_usQueueQuantity;
    unsigned short m_usFront, m_usRear;
    unsigned short m_usNumUnprocessed;
    unsigned short m_usItemsInQueue;

	BoundSphere* m_pBound;
	Terrain* m_pTerrain;
};

#include "TerrainPage.inl"

}
}