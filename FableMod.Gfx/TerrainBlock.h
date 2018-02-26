#pragma once

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "GfxConf.h"

namespace FableMod
{
namespace Gfx
{
		
class TerrainPage;
class TerrainVertex;
class Camera;
	
/// A piece of terrain.
class GFX_API TerrainBlock
{
public:
	
    unsigned char GetX() const;
    unsigned char GetY() const;
    unsigned char GetStride() const;

    FLOAT GetDelta(int i) const;
    FLOAT GetDeltaMax() const;
    FLOAT GetDeltaL() const;
    FLOAT GetDeltaH() const;

	const D3DXVECTOR3& GetMin() const;
    const D3DXVECTOR3& GetMax() const;

    void SetEven(bool bSet);
    bool GetEven() const;
    
	void SetProcessed(bool bSet);
    bool GetProcessed() const;
    
	void SetVisible(bool bSet);
    bool GetVisible() const;
    
	void SetVisibilityTested(bool bSet);
    bool GetVisibilityTested() const;

    bool BitsSet() const;
    
	void ClearBits();

    void Initialize(
		TerrainPage* pPage, 
		TerrainBlock* pBlock,
        unsigned short usBlock, 
		unsigned char ucX, 
		unsigned char ucY,
        unsigned char ucStride, 
		bool bEven);

    void UpdateBound(
		TerrainPage* pvPage, 
		TerrainBlock* pvBlock,
        unsigned short usBlock, 
		unsigned char ucStride);

    void TestIntersectFrustum(
		const TerrainPage* pPage,
        const Camera* pCamera);

    void ComputeInterval(
		const D3DXVECTOR3& rvModelEye, 
		FLOAT fTolerance);

    void ComputeInterval(
		const D3DXVECTOR3& rvModelDir,
        const D3DXVECTOR3& rvModelEye, 
		FLOAT fTolerance, 
		D3DXVECTOR2& rvLoc,
        FLOAT fSpacing);

    void SimplifyVertices(
		TerrainPage* pkPage,
        const D3DXVECTOR3& rvModelEye, 
		const D3DXVECTOR3& rvModelDir,
        FLOAT fTolerance, 
		bool bCloseAssumption);

    void Disable(TerrainPage* pPage);

    static unsigned short GetParentIndex(unsigned short usChild);
    static unsigned short GetChildIndex(
		unsigned short usParent,
        unsigned short usIndex);
    static bool IsFirstChild(unsigned short usIndex);
    static bool IsSibling(unsigned short usIndex, unsigned short usTest);

protected:
	
    /// Bit flags for m_ucFlags
    enum
    {
        EVEN_PARITY       = 0x01,
        PROCESSED         = 0x02,
        VISIBLE           = 0x04,
        VISIBILITY_TESTED = 0x08,
    	/// All but even parity bit.
        BITS_MASK         = 0x0E  
	};

    void GetVertex9(
		unsigned short usSize, 
		TerrainVertex* pOrigin,
        TerrainVertex* apTVertex[9]);

    unsigned char m_ucX, m_ucY, m_ucStride, m_ucFlags;
    FLOAT m_fDelta[5], m_fDeltaMax;
    FLOAT m_fDeltaL, m_fDeltaH;
    D3DXVECTOR3 m_vMin, m_vMax;
};

#include "TerrainBlock.inl"

}
}