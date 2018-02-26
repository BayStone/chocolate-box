#pragma once

#include "Vector.h"
#include "Spatial.h"
#include "BoundSphere.h"
#include "Mesh.h"
#include "Plane.h"

namespace FableMod
{
namespace Gfx
{

/// Camera is the 3D camera.
class GFX_API Camera
{
public:

	Camera();

	virtual ~Camera();

	void RenderScene(Spatial* pSpatial);

	void Update();

	void Refresh();

	void SetDistanceToCamera(Mesh* pMesh);

	void AddRenderObject(Mesh* pMesh);

	bool ScreenToWorld(
		const D3DXVECTOR2& vPoint, FLOAT fYLevel, D3DXVECTOR3* pvResult);

	bool ScreenToRay(
		const D3DXVECTOR2& pvPoint,
		D3DXVECTOR3* pvOrigin,
		D3DXVECTOR3* pvDirection);

	FLOAT GetProjectedSize(FLOAT fDistance, FLOAT fSize) const;

	FLOAT GetPixelProjection(FLOAT fDistance, FLOAT fPixels) const;

	void SetPosition(const D3DXVECTOR3& vPosition);

	const D3DXVECTOR3& GetPosition() const;

	void SetInterest(const D3DXVECTOR3& vInterest);

	const D3DXVECTOR3& GetInterest() const;

	void SetUp(const D3DXVECTOR3& vUp) { m_vUp = vUp; }

	bool Culled(BoundSphere* pBound);

	bool IsVisible(BoundSphere* pBound) const;

	void SetViewport(int iLeft,int iTop,int iRight,int iBottom);

	void SetFOV(FLOAT fFOV);

	FLOAT GetFOV() const { return m_fFOV; }

	void SetFrustumFar(FLOAT fFar);

	FLOAT GetFrustumFar() const { return m_fFrustumF; }

	FLOAT GetFrustumNear() const { return m_fFrustumN; }

	FLOAT GetFrustumRight() const { return m_fFrustumR; }

	int GetViewportWidth() const { return m_iPortR-m_iPortL; }

	int GetViewportHeight() const { return m_iPortB-m_iPortT; }

	const D3DXVECTOR3& GetRight() const { return m_vRight; }

	const D3DXVECTOR3& GetDirection() const { return m_vDirection; }

	const D3DXVECTOR3& GetUp() const { return m_vUp; }

	unsigned int& PlaneState();

	const unsigned int& PlaneState() const;

	unsigned int GetRenderCount() const;

	const D3DXMATRIX& GetViewMatrix() const;

	const D3DXMATRIX& GetViewInverseMatrix() const;

	bool IsUpdated() const;

	static const D3DXVECTOR3 CAM_BASE_UP;
	static const D3DXVECTOR3 CAM_BASE_RIGHT;
	
protected:

	void UpdateViewport();

	void UpdatePerspective();

	void UpdateWorldToScreenMatrix();

	void UpdateFrustum();

	void UpdateFrame();

	enum ECameraFlags
	{
		CF_UPDATED = 1,
		CF_DIRTY = 2,
		CF_FRAME_DIRTY = (CF_DIRTY | 4),
		CF_VIEWPORT_DIRTY = (CF_DIRTY | 8),
		CF_PERSPECTIVE_DIRTY = (CF_DIRTY | 16),
		CF_FRUSTUM_DIRTY = (CF_PERSPECTIVE_DIRTY | 32),
	};

	unsigned int m_uiFlags;
	
	Vector<Mesh*> m_RenderObjects;
	
	D3DXVECTOR3	m_vUp;
	D3DXVECTOR3 m_vRight;
	D3DXVECTOR3 m_vDirection;
	D3DXVECTOR3 m_vInterest;
	D3DXVECTOR3 m_vPosition;

	D3DXMATRIX m_matViewport;
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matViewInverse;
	D3DXMATRIX m_matProj;
	
	FLOAT 
		m_fFrustumL,
		m_fFrustumR,
		m_fFrustumT,
		m_fFrustumB,
		m_fFrustumN,
		m_fFrustumF;

	FLOAT m_fFOV;

	FLOAT 
		m_afCoeffL[2], 
		m_afCoeffR[2], 
		m_afCoeffB[2], 
		m_afCoeffT[2];
	
	int
		m_iPortL,
		m_iPortR,
		m_iPortT,
		m_iPortB;
	
	enum
	{
		CAM_LEFT_PLANE = 0,
		CAM_RIGHT_PLANE = 1,
		CAM_BOTTOM_PLANE = 2,
		CAM_TOP_PLANE = 3,
		CAM_FAR_PLANE = 4,
		CAM_NEAR_PLANE = 5,
		CAM_FRUSTUM_PLANES = 6,
	};

	unsigned int m_uiPlaneState;
	Plane m_Planes[6];
};

#include "Camera.inl"

}
}