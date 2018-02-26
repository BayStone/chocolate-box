#include <cassert>

//----------------------------------------------------------------------------

inline void TerrainVertex::SetDependent(
	int i,
    TerrainVertex* pDependent)
{
    assert(0 <= i && i <= 1);
    m_apDependent[i] = pDependent;
}

//----------------------------------------------------------------------------

inline TerrainVertex* TerrainVertex::GetDependent(int i)
{
    assert(0 <= i && i <= 1);
    return m_apDependent[i];
}

//----------------------------------------------------------------------------

inline bool TerrainVertex::GetEnabled() const
{
    return m_bEnabled;
}

//----------------------------------------------------------------------------
