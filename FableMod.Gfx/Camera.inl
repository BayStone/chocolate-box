inline void Camera::SetInterest(const D3DXVECTOR3& vInterest)
{
	m_uiFlags |= CF_FRAME_DIRTY;
	m_vInterest = vInterest;
}

inline const D3DXVECTOR3& Camera::GetInterest() const
{
	return m_vInterest;
}

inline void Camera::SetPosition(const D3DXVECTOR3& vPosition)
{
	m_uiFlags |= CF_FRAME_DIRTY;
	m_vPosition = vPosition;
}

inline const D3DXVECTOR3& Camera::GetPosition() const
{
	return m_vPosition;
}

inline void Camera::SetFOV(FLOAT fFOV)
{
	m_fFOV = fFOV;
	m_uiFlags |= CF_PERSPECTIVE_DIRTY;
}

inline void Camera::SetFrustumFar(FLOAT fFar)
{
	m_fFrustumF = fFar;
	m_uiFlags |= CF_FRUSTUM_DIRTY;
}

inline const D3DXMATRIX& Camera::GetViewMatrix() const
{
	return m_matView;
}

inline const D3DXMATRIX& Camera::GetViewInverseMatrix() const
{
	return m_matViewInverse;
}

inline bool Camera::IsUpdated() const
{
	return (m_uiFlags & CF_UPDATED) != 0;
}