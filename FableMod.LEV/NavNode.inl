inline unsigned int NavNode::GetNumAdjacents() const
{
	return m_Adjacent.size();
}

inline unsigned int NavNode::GetAdjacentAt(unsigned int uiIndex)
{
	return m_Adjacent[uiIndex];
}

inline void NavNode::SetID(unsigned int uiID)
{
	m_Header.ID = uiID;
}

inline void NavNode::SetLayer(unsigned char ucLayer)
{
	m_Header.Layer = ucLayer;
}

inline void NavNode::SetSubset(unsigned char ucSubset)
{
	m_Header.Subset = ucSubset;
}

inline void NavNode::SetX(float fX)
{
	m_Header.X = fX;
}

inline void NavNode::SetY(float fY)
{
	m_Header.Y = fY;
}

inline void NavNode::SetType(ENavNodeType eType)
{
	m_eType = eType;
}

inline unsigned int NavNode::GetID() const
{
	return m_Header.ID;
}

inline unsigned char NavNode::GetLayer() const
{
	return m_Header.Layer;
}

inline unsigned char NavNode::GetSubset() const
{
	return m_Header.Subset;
}

inline float NavNode::GetX() const
{
	return m_Header.X;
}

inline float NavNode::GetY() const
{
	return m_Header.Y;
}

inline ENavNodeType NavNode::GetType() const
{
	return m_eType;
}
