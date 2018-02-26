#include "Stdafx.h"
#include "Camera.h"
#include "Manager.h"
#include "Settings.h"

// Take alpha into account when rendering.
#define GFX_ALPHA_RENDER	

namespace FableMod
{
namespace Gfx
{
	
const D3DXVECTOR3 Camera::CAM_BASE_UP(0,0,1);
const D3DXVECTOR3 Camera::CAM_BASE_RIGHT(-1,0,0);
	
static int ModelSorter(const void* pX, const void* pY)
{
	Mesh* pMeshX = *(Mesh**)pX;
	Mesh* pMeshY = *(Mesh**)pY;

#ifdef GFX_ALPHA_RENDER	
	if (pMeshY->AlphaRequired() && !pMeshX->AlphaRequired())
		return 1;
	else if (pMeshX->AlphaRequired() && !pMeshY->AlphaRequired())
		return -1;
#endif

	if (pMeshX->DistanceToCamera() < pMeshY->DistanceToCamera())
		return -1;
	else if (pMeshX->DistanceToCamera() > pMeshY->DistanceToCamera())
		return 1;

	return 0;
}

Camera::Camera() :
	m_RenderObjects(128,16),
	m_uiPlaneState(0xFFFFFFFF),
	m_fFrustumL(GetSettings()->GetFloat(_T("Camera"), _T("Left"), 0.45f)),
	m_fFrustumR(GetSettings()->GetFloat(_T("Camera"), _T("Right"), -0.45f)),
	m_fFrustumT(GetSettings()->GetFloat(_T("Camera"), _T("Top"), -0.45f)),
	m_fFrustumB(GetSettings()->GetFloat(_T("Camera"), _T("Bottom"), 0.45f)),
	m_fFrustumN(GetSettings()->GetFloat(_T("Camera"), _T("Near"), 0.8f)),
	m_fFrustumF(GetSettings()->GetFloat(_T("Camera"), _T("Far"), 10000.0f)),
	m_vPosition(0,1,0),
	m_vInterest(0,0,0),
	m_vRight(CAM_BASE_RIGHT),
	m_vUp(CAM_BASE_UP),
	m_uiFlags(
		CF_FRAME_DIRTY |
		CF_VIEWPORT_DIRTY |
		CF_PERSPECTIVE_DIRTY |
		CF_FRUSTUM_DIRTY)
{
}

Camera::~Camera()
{
}

void Camera::Refresh()
{
	m_uiFlags |= 
		CF_VIEWPORT_DIRTY |
		CF_FRUSTUM_DIRTY |
		CF_PERSPECTIVE_DIRTY |
		CF_FRAME_DIRTY;		
}

void Camera::Update() 
{
	if (m_uiFlags & CF_VIEWPORT_DIRTY)
		UpdateViewport();
	
	if (m_uiFlags & CF_FRUSTUM_DIRTY)
		UpdateFrustum();

	if (m_uiFlags & CF_PERSPECTIVE_DIRTY)
		UpdatePerspective();

	if (m_uiFlags & CF_FRAME_DIRTY)
		UpdateFrame();		
		
	if (m_uiFlags & CF_DIRTY)
	{
		m_uiFlags |= CF_UPDATED;
	}
	else
		m_uiFlags &= ~CF_UPDATED;

	m_uiFlags &= ~CF_FRAME_DIRTY;
	m_uiFlags &= ~CF_VIEWPORT_DIRTY;
	m_uiFlags &= ~CF_PERSPECTIVE_DIRTY;
	m_uiFlags &= ~CF_FRUSTUM_DIRTY;
	m_uiFlags &= ~CF_DIRTY;
}

void Camera::UpdateFrustum()
{
	FLOAT fNSqr = m_fFrustumN*m_fFrustumN;
	FLOAT fLSqr = m_fFrustumL*m_fFrustumL;
	FLOAT fRSqr = m_fFrustumR*m_fFrustumR;
	FLOAT fBSqr = m_fFrustumB*m_fFrustumB;
	FLOAT fTSqr = m_fFrustumT*m_fFrustumT;

	FLOAT fInvLength = 1.0f/(FLOAT)sqrtf(fNSqr+fLSqr);
	m_afCoeffL[0] = m_fFrustumN*fInvLength;
	m_afCoeffL[1] = -m_fFrustumL*fInvLength;

	fInvLength = 1.0f/(FLOAT)sqrtf(fNSqr+fRSqr);
	m_afCoeffR[0] = -m_fFrustumN*fInvLength;
	m_afCoeffR[1] = m_fFrustumR*fInvLength;

	fInvLength = 1.0f/(FLOAT)sqrtf(fNSqr+fBSqr);
	m_afCoeffB[0] = m_fFrustumN*fInvLength;
	m_afCoeffB[1] = -m_fFrustumB*fInvLength;

	fInvLength = 1.0f/(FLOAT)sqrtf(fNSqr+fTSqr);
	m_afCoeffT[0] = -m_fFrustumN*fInvLength;
	m_afCoeffT[1] = m_fFrustumT*fInvLength;
}

void Camera::UpdatePerspective()
{
	const FLOAT ScreenAspect = 
		(FLOAT)GetViewportWidth()/(FLOAT)GetViewportHeight();

#ifdef GFX_LH
	D3DXMatrixPerspectiveFovLH(
#else
	D3DXMatrixPerspectiveFovRH(
#endif
		&m_matProj,
		D3DXToRadian(m_fFOV),
		ScreenAspect,
		m_fFrustumN,
		m_fFrustumF);

	Manager::GetDevice()->GetD3DDevice()->SetTransform(
		D3DTS_PROJECTION,&m_matProj);
}

void Camera::SetViewport(int iLeft,int iTop,int iRight,int iBottom)
{
	m_iPortL = iLeft;
	m_iPortT = iTop;
	m_iPortR = iRight;
	m_iPortB = iBottom;

	UpdateViewport();
}

void Camera::UpdateViewport()
{
	D3DVIEWPORT9 Viewport;

	Viewport.X = m_iPortL;
	Viewport.Y = m_iPortT;
	Viewport.Width  = GetViewportWidth();
	Viewport.Height = GetViewportHeight();
	
	Viewport.MinZ = 0.0f;
	Viewport.MaxZ = 1.0f;

	Manager::GetDevice()->GetD3DDevice()->SetViewport(&Viewport);

	ZeroMemory(&m_matViewport,sizeof(m_matViewport));
	
	m_matViewport._11 = (FLOAT)Viewport.Width/2;
	m_matViewport._41 = Viewport.X+(FLOAT)Viewport.Width/2;
	m_matViewport._22 = -(FLOAT)Viewport.Height/2;
	m_matViewport._42 = (FLOAT)Viewport.Height/2+Viewport.Y;
	m_matViewport._33 = Viewport.MaxZ-Viewport.MinZ;
	m_matViewport._43 = Viewport.MinZ;
	m_matViewport._44 = 1;
}

void Camera::UpdateFrame()
{
	m_vDirection = GetPosition()-GetInterest();

	D3DXVec3Normalize(&m_vDirection, &m_vDirection);

	D3DXVec3Cross(&m_vRight,&CAM_BASE_UP,&m_vDirection);
	D3DXVec3Cross(&m_vUp,&m_vDirection,&m_vRight);

	D3DXVec3Normalize(&m_vUp, &m_vUp);
	D3DXVec3Normalize(&m_vRight, &m_vRight);

	FLOAT fDdE = D3DXVec3Dot(&m_vDirection,&GetPosition());

	// left plane
	m_Planes[CAM_LEFT_PLANE].Normal() = m_afCoeffL[0]*m_vRight+
		m_afCoeffL[1]*m_vDirection;
	m_Planes[CAM_LEFT_PLANE].Constant() = D3DXVec3Dot(
		&GetPosition(),
		&(m_Planes[CAM_LEFT_PLANE].Normal()));

	// right plane	
	m_Planes[CAM_RIGHT_PLANE].Normal() = m_afCoeffR[0]*m_vRight+
		m_afCoeffR[1]*m_vDirection;
	m_Planes[CAM_RIGHT_PLANE].Constant() = D3DXVec3Dot(
		&GetPosition(),
		&(m_Planes[CAM_RIGHT_PLANE].Normal()));

	// bottom plane
	m_Planes[CAM_BOTTOM_PLANE].Normal() = m_afCoeffB[0]*m_vUp+
		m_afCoeffB[1]*m_vDirection;
	m_Planes[CAM_BOTTOM_PLANE].Constant() = D3DXVec3Dot(
		&GetPosition(),
		&(m_Planes[CAM_BOTTOM_PLANE].Normal()));

	// top plane
	m_Planes[CAM_TOP_PLANE].Normal() = m_afCoeffT[0]*m_vUp+
		m_afCoeffT[1]*m_vDirection;
	m_Planes[CAM_TOP_PLANE].Constant() = D3DXVec3Dot(
		&GetPosition(),
		&(m_Planes[CAM_TOP_PLANE].Normal()));

	// far plane
	m_Planes[CAM_FAR_PLANE].Normal() = m_vDirection;
	m_Planes[CAM_FAR_PLANE].Constant() = (fDdE-m_fFrustumF);

	// near plane
	m_Planes[CAM_NEAR_PLANE].Normal() = -m_vDirection;
	m_Planes[CAM_NEAR_PLANE].Constant() = -(fDdE+m_fFrustumN);

#ifdef GFX_LH
	D3DXMatrixLookAtLH(
#else
	D3DXMatrixLookAtRH(
#endif
		&m_matView,
		&GetPosition(),
		&GetInterest(),
		&GetUp());

	Manager::GetDevice()->GetD3DDevice()->SetTransform(D3DTS_VIEW,&m_matView);

	// Towards interest point.
	m_vDirection = -m_vDirection;

	UpdateWorldToScreenMatrix();
}

FLOAT Camera::GetProjectedSize(FLOAT fDistance, FLOAT fSize) const
{
	if (fDistance < m_fFrustumN)
		fDistance = m_fFrustumN;

	FLOAT fWidthProjectionScalar = 
		(FLOAT(m_iPortR-m_iPortL)*0.5f)/tan(D3DXToRadian(m_fFOV)*0.5f);

	return fSize/fDistance*fWidthProjectionScalar;
}

FLOAT Camera::GetPixelProjection(FLOAT fDistance, FLOAT fPixels) const
{
	if (fDistance < m_fFrustumN)
		fDistance = m_fFrustumN;

	FLOAT fHalfWidth = (FLOAT(m_iPortR-m_iPortL)*0.5f);

	FLOAT fWorldHalfWidth = 
		fDistance*tan(D3DXToRadian(m_fFOV)*0.5f);

	return fWorldHalfWidth*(fPixels/fHalfWidth);
}

void Camera::UpdateWorldToScreenMatrix()
{
	if (!D3DXMatrixInverse(&m_matViewInverse, NULL, &m_matView))
	{
		D3DXMatrixIdentity(&m_matViewInverse);
	}
}

bool Camera::ScreenToWorld(
	const D3DXVECTOR2& vPoint, FLOAT fYLevel, D3DXVECTOR3* pvResult)
{
	D3DXVECTOR3 vO,vD;

	if (ScreenToRay(vPoint,&vO,&vD))
	{
		FLOAT fDp = fYLevel;
		
		FLOAT fDenom = D3DXVec3Dot(&CAM_BASE_UP,&vD);

		if (fabs(fDenom) <= 0.0001f)
			return false;
		
		FLOAT fT = (fDp-D3DXVec3Dot(&CAM_BASE_UP,&vO))/fDenom;

		*pvResult = vO+fT*vD;

		return true;
	}
	
	return false;
}
	
bool Camera::ScreenToRay(
	const D3DXVECTOR2& vPoint,
	D3DXVECTOR3* pvOrigin,
	D3DXVECTOR3* pvDirection)
{
	if (vPoint.x < 0 || vPoint.y < 0)
	{
		// Out of range.
		return false;
	}

	const int iWidth  = GetViewportWidth();
	const int iHeight = GetViewportHeight();

	// Compute the vector of the pick ray in screen space.
	D3DXVECTOR3 v;

	v.x = ((((2.0f*vPoint.x)/iWidth)-1)/m_matProj._11);
#ifdef GFX_LH
	v.y = ((((2.0f*vPoint.y)/iHeight)-1)/m_matProj._22);
	v.z = 1.0f;
#else
	v.y = -((((2.0f*vPoint.y)/iHeight)-1)/m_matProj._22);
	v.z = -1.0f;
#endif

	const D3DXMATRIX& m = m_matViewInverse;

	// Transform the screen space pick ray into 3D space.
	pvOrigin->x    = m._41;
	pvOrigin->y    = m._42;
	pvOrigin->z    = m._43;
	pvDirection->x = v.x*m._11+v.y*m._21+v.z*m._31;
	pvDirection->y = v.x*m._12+v.y*m._22+v.z*m._32;
	pvDirection->z = v.x*m._13+v.y*m._23+v.z*m._33;
	
	D3DXVec3Normalize(pvDirection,pvDirection);
	
	return true;
}

void Camera::RenderScene(Spatial* pSpatial)
{
	m_RenderObjects.Clear();

	pSpatial->GetRenderList((Camera*)this);

	qsort(
		(void*)m_RenderObjects.GetData(),
		m_RenderObjects.GetCount(),
		sizeof(Mesh*),
		ModelSorter);
	
	if (Manager::GetDevice()->BeginScene())
	{	
		static bool s_bLightOk = false;

		IDirect3DDevice9* pDevice = Manager::GetDevice()->GetD3DDevice();

		if (!s_bLightOk)
		{
			s_bLightOk = true;
		
			const FLOAT fRadius = pSpatial->GetWorldBound()->Radius();
		
			D3DLIGHT9 light;

			ZeroMemory(&light, sizeof(light));

			light.Type = D3DLIGHT_DIRECTIONAL;

			light.Diffuse.r = 1.0f;
			light.Diffuse.g = 1.0f;
			light.Diffuse.b = 1.0f;

			light.Direction = D3DXVECTOR3(0,0,-1);
		
			light.Range = fRadius*3;
			light.Attenuation0 = 1.0f;

			pDevice->SetLight(0, &light);
		
			pDevice->LightEnable(0, TRUE);
		}

		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		
#ifdef GFX_ALPHA_RENDER	
		int iAlphaStart = -1;

		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		for (int i = 0; i < m_RenderObjects.GetCount(); ++i)
		{
			if (m_RenderObjects[i]->AlphaRequired())
			{
				// Alpha, don't draw.
				iAlphaStart = i;
			}
			else
			{
				m_RenderObjects[i]->Render(this);
			}
		}

		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

		if (iAlphaStart >= 0)
		{
			// Render objects with transparency.

			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			
			for (int i = iAlphaStart; i >= 0; --i)
			{
				m_RenderObjects[i]->Render(this);
			}
	
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			
			for (int i = iAlphaStart; i >= 0; --i)
			{
				m_RenderObjects[i]->Render(this);
			}
		}

#else
		for (int i = 0; i < m_RenderObjects.GetCount(); ++i)
		{
			m_RenderObjects[i]->Render(this);
		}
#endif			
		
		Manager::GetDevice()->EndScene();
	}
	else
	{
		throw Exception(_T("FABLEMOD_GFX_EXCEPTION_BEGINSCENE_FAILED"));
	}
}

unsigned int Camera::GetRenderCount() const
{
	return m_RenderObjects.GetCount();
}

void Camera::SetDistanceToCamera(Mesh* pMesh)
{
	const D3DXVECTOR3 vDist(pMesh->GetWorldTranslation()-GetPosition());	
	pMesh->DistanceToCamera() = D3DXVec3LengthSq(&vDist);
}

void Camera::AddRenderObject(Mesh* pMesh)
{
	SetDistanceToCamera(pMesh);
	m_RenderObjects.Add(pMesh);
}

///////////////////////////////////////////////////////////////////////////////
/// Test if a boundsphere is culled or not.
/// Notice that this functions disables the passed frustum planes.
///////////////////////////////////////////////////////////////////////////////
bool Camera::Culled(BoundSphere* pBound)
{
	unsigned int uiP = CAM_FRUSTUM_PLANES-1;
	unsigned int uiMask = 1 << uiP;

	for (int i = 0; i < CAM_FRUSTUM_PLANES; ++i, --uiP, uiMask >>= 1)
	{
		if (m_uiPlaneState & uiMask)
		{
			// This plane is on.

			Plane::ESide eSide = pBound->WhichSide(m_Planes[uiP]);

			if (eSide == Plane::NEGATIVE_SIDE)
			{
				// Object is on negative side - Cull it.
				return true;
			}
			else if (eSide == Plane::POSITIVE_SIDE)
			{
				// Object is on positive side of plane. There is no need to
				// compare subobjects against this plane, so mark it as
				// inactive.
				m_uiPlaneState &= ~uiMask;
			}
		}
	}
	
	return false;
}

bool Camera::IsVisible(BoundSphere* pBound) const
{
	// Test against all frustum planes.

	Plane::ESide eSide;

	eSide = pBound->WhichSide(m_Planes[CAM_FAR_PLANE]);

	if (eSide == Plane::NEGATIVE_SIDE)
		return false;
	
	eSide = pBound->WhichSide(m_Planes[CAM_NEAR_PLANE]);

	if (eSide == Plane::NEGATIVE_SIDE)
		return false;
	
	eSide = pBound->WhichSide(m_Planes[CAM_LEFT_PLANE]);

	if (eSide == Plane::NEGATIVE_SIDE)
		return false;
	
	eSide = pBound->WhichSide(m_Planes[CAM_RIGHT_PLANE]);

	if (eSide == Plane::NEGATIVE_SIDE)
		return false;
	
	eSide = pBound->WhichSide(m_Planes[CAM_TOP_PLANE]);

	if (eSide == Plane::NEGATIVE_SIDE)
		return false;
	
	eSide = pBound->WhichSide(m_Planes[CAM_BOTTOM_PLANE]);

	if (eSide == Plane::NEGATIVE_SIDE)
		return false;
	
	return true;
}

unsigned int& Camera::PlaneState()
{
	return m_uiPlaneState;
}

const unsigned int& Camera::PlaneState() const
{
	return m_uiPlaneState;
}

}
}