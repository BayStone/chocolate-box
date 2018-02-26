
//---------------------------------------------------------------------------

inline void Spatial::SetTranslation(const D3DXVECTOR3& vTranslation)
{
	Flags() |= FLAGS_TRANSFORM_DIRTY;
	m_vTranslation = vTranslation;
}

//---------------------------------------------------------------------------

inline const D3DXVECTOR3& Spatial::GetTranslation() const
{
	return m_vTranslation;
}

//---------------------------------------------------------------------------

inline void Spatial::SetScale(FLOAT fScale)
{
	Flags() |= FLAGS_TRANSFORM_DIRTY;
	m_fScale = fScale;
}

//---------------------------------------------------------------------------

inline FLOAT Spatial::GetScale() const
{
	return m_fScale;
}

//---------------------------------------------------------------------------

inline FLOAT Spatial::GetWorldScale() const
{
	return m_fWorldScale;
}

//---------------------------------------------------------------------------

inline const D3DXMATRIX& Spatial::GetWorldTransform(bool bUpdate)
{
	if (bUpdate)
		UpdateWorldTransform();

	return m_matWorldTransform;
}

//---------------------------------------------------------------------------

inline const D3DXVECTOR3& Spatial::GetWorldTranslation() const
{
	return m_vWorldTranslation;
}

//---------------------------------------------------------------------------

inline BoundSphere* Spatial::GetWorldBound() const
{
    return m_pWorldBound;
}

//---------------------------------------------------------------------------

inline const Node* Spatial::Parent() const
{
    return m_pParent;
}

//---------------------------------------------------------------------------

inline Node*& Spatial::Parent()
{
    return m_pParent;
}

//---------------------------------------------------------------------------

inline bool Spatial::IsTransformUpdated() const
{
	return (m_uiFlags & FLAGS_TRANSFORM_UPDATED) ? true : false;
}

//---------------------------------------------------------------------------

inline unsigned int& Spatial::Flags()
{
	return m_uiFlags;
}

//---------------------------------------------------------------------------

inline const unsigned int& Spatial::Flags() const
{
	return m_uiFlags;
}

//---------------------------------------------------------------------------

inline const unsigned int& Spatial::ID() const
{
	return m_uiID;
}

//---------------------------------------------------------------------------

inline unsigned int& Spatial::ID()
{
	return m_uiID;
}

//---------------------------------------------------------------------------

inline FLOAT Spatial::GetAlpha() const
{
	return 1.0f;
}

//---------------------------------------------------------------------------

inline bool Spatial::IsLeaf() const
{
	return true;
}