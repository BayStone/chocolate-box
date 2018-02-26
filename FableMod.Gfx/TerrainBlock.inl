#include <cassert>

//----------------------------------------------------------------------------
inline unsigned char TerrainBlock::GetX() const
{
    return m_ucX;
}
//----------------------------------------------------------------------------
inline unsigned char TerrainBlock::GetY() const
{
    return m_ucY;
}
//----------------------------------------------------------------------------
inline unsigned char TerrainBlock::GetStride() const
{
    return m_ucStride;
}
//----------------------------------------------------------------------------
inline void TerrainBlock::SetEven(bool bSet)
{
    if (bSet)
        m_ucFlags |= EVEN_PARITY;
    else
        m_ucFlags &= ~EVEN_PARITY;
}
//----------------------------------------------------------------------------
inline bool TerrainBlock::GetEven() const
{
    return(m_ucFlags & EVEN_PARITY) != 0;
}
//----------------------------------------------------------------------------
inline void TerrainBlock::SetProcessed(bool bSet)
{
    if (bSet)
        m_ucFlags |= PROCESSED;
    else
        m_ucFlags &= ~PROCESSED;
}
//----------------------------------------------------------------------------
inline bool TerrainBlock::GetProcessed() const
{
    return (m_ucFlags & PROCESSED) != 0;
}
//----------------------------------------------------------------------------
inline void TerrainBlock::SetVisible(bool bSet)
{
    if (bSet)
        m_ucFlags |= VISIBLE;
    else
        m_ucFlags &= ~VISIBLE;
}
//----------------------------------------------------------------------------
inline bool TerrainBlock::GetVisible() const
{
    return (m_ucFlags & VISIBLE) != 0;
}
//----------------------------------------------------------------------------
inline void TerrainBlock::SetVisibilityTested(bool bSet)
{
    if( bSet )
        m_ucFlags |= VISIBILITY_TESTED;
    else
        m_ucFlags &= ~VISIBILITY_TESTED;
}
//----------------------------------------------------------------------------
inline bool TerrainBlock::GetVisibilityTested() const
{
    return(m_ucFlags & VISIBILITY_TESTED) != 0;
}
//----------------------------------------------------------------------------
inline bool TerrainBlock::BitsSet() const
{
    return(m_ucFlags & BITS_MASK) != 0;
}
//----------------------------------------------------------------------------
inline void TerrainBlock::ClearBits()
{
    // retain parity bit, all others zeroed out
    m_ucFlags &= EVEN_PARITY;
}
//----------------------------------------------------------------------------
inline FLOAT TerrainBlock::GetDelta(int i) const
{
    assert(0 <= i && i <= 4);
    return m_fDelta[i];
}
//----------------------------------------------------------------------------
inline FLOAT TerrainBlock::GetDeltaMax() const
{
    return m_fDeltaMax;
}
//----------------------------------------------------------------------------
inline FLOAT TerrainBlock::GetDeltaL() const
{
    return m_fDeltaL;
}
//----------------------------------------------------------------------------
inline FLOAT TerrainBlock::GetDeltaH() const
{
    return m_fDeltaH;
}
//----------------------------------------------------------------------------
inline const D3DXVECTOR3& TerrainBlock::GetMin() const
{
    return m_vMin;
}
//----------------------------------------------------------------------------
inline const D3DXVECTOR3& TerrainBlock::GetMax() const
{
    return m_vMax;
}
//----------------------------------------------------------------------------
inline unsigned short TerrainBlock::GetParentIndex(unsigned short usChild)
{
    // p =(c-1)/4
    return (usChild-1) >> 2;
}
//----------------------------------------------------------------------------
inline unsigned short TerrainBlock::GetChildIndex(unsigned short usParent,
    unsigned short usIndex)
{
    // c = 4*p+i
    return (usParent << 2)+usIndex;
}
//----------------------------------------------------------------------------
inline bool TerrainBlock::IsFirstChild(unsigned short usIndex)
{
    return usIndex > 0 && ((usIndex-1) % 4) == 0;
}
//----------------------------------------------------------------------------
inline bool TerrainBlock::IsSibling(unsigned short usIndex,
    unsigned short usTest)
{
    // assert:  usIndex > 0 && usTest > 0
    return(usIndex-1) >> 2 == (usTest-1) >> 2;
}
//----------------------------------------------------------------------------
