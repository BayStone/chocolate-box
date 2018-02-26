//----------------------------------------------------------------------------

inline D3DXVECTOR3& Triangle::Origin()
{
    return m_vOrigin;
}

//----------------------------------------------------------------------------

inline const D3DXVECTOR3& Triangle::Origin() const
{
    return m_vOrigin;
}

//----------------------------------------------------------------------------

inline D3DXVECTOR3& Triangle::Edge0()
{
    return m_vEdge0;
}

//----------------------------------------------------------------------------

inline const D3DXVECTOR3& Triangle::Edge0() const
{
    return m_vEdge0;
}

//----------------------------------------------------------------------------

inline D3DXVECTOR3& Triangle::Edge1()
{
    return m_vEdge1;
}

//----------------------------------------------------------------------------

inline const D3DXVECTOR3& Triangle::Edge1() const
{
    return m_vEdge1;
}

//----------------------------------------------------------------------------

inline D3DXVECTOR3 Triangle::Normal() const
{
	D3DXVECTOR3 vResult;
    D3DXVec3Cross(&vResult, &Edge0(), &Edge1());
	return vResult;
}

//----------------------------------------------------------------------------

