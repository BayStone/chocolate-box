#include <cassert>

//---------------------------------------------------------------------------
inline unsigned short TerrainPage::GetSize() const
{
    return m_usSize;
}
//---------------------------------------------------------------------------
inline const FLOAT* TerrainPage::GetHeights() const
{
    return m_afHeight;
}
//---------------------------------------------------------------------------
inline const D3DXVECTOR2& TerrainPage::GetOrigin() const
{
    return m_vOrigin;
}
//---------------------------------------------------------------------------
inline FLOAT TerrainPage::GetSpacing() const
{
    return m_fSpacing;
}
//---------------------------------------------------------------------------
inline FLOAT TerrainPage::GetPixelTolerance() const
{
    return m_fPixelTolerance;
}
//---------------------------------------------------------------------------
inline FLOAT TerrainPage::GetX(unsigned char ucX) const
{
    return m_vOrigin.x+m_fSpacing*FLOAT(ucX);
}
//---------------------------------------------------------------------------
inline FLOAT TerrainPage::GetY(unsigned char ucY) const
{
    return m_vOrigin.y+m_fSpacing*FLOAT(ucY);
}
//---------------------------------------------------------------------------
inline FLOAT TerrainPage::GetHeight(unsigned int uiIndex) const
{
	assert(uiIndex < m_usSize*m_usSize);
	return m_afHeight[uiIndex];
}
//---------------------------------------------------------------------------
inline FLOAT TerrainPage::GetHeightAt(unsigned short usX, 
									  unsigned short usY) const
{
	assert(usX < m_usSize);
	assert(usY < m_usSize);
	return m_afHeight[usY*m_usSize+usX];
}
//---------------------------------------------------------------------------
inline FLOAT TerrainPage::GetTextureX(unsigned char ucSet,
									  unsigned char ucIndex) const
{
    return m_vTextureOrigin[ucSet].x+
		m_vTextureSpacing[ucSet].x*FLOAT(ucIndex);
}
//---------------------------------------------------------------------------
inline FLOAT TerrainPage::GetTextureY(unsigned char ucSet,
									  unsigned char ucIndex) const
{
    return m_vTextureOrigin[ucSet].y+
		m_vTextureSpacing[ucSet].y*FLOAT(ucIndex);
}
//---------------------------------------------------------------------------
inline void TerrainPage::SetTextureSpacing(unsigned char ucSet, 
										   const D3DXVECTOR2& vSpacing)
{
	m_vTextureSpacing[ucSet] = vSpacing;
}
//---------------------------------------------------------------------------
inline void TerrainPage::SetTextureOrigin(unsigned char ucSet, 
										  const D3DXVECTOR2& vOrigin)
{
	m_vTextureOrigin[ucSet] = vOrigin;
}
