#include "Stdafx.h"
#include "CameraController.h"
#include "Settings.h"
#include "Rotation.h"
#include "LogFile.h"

namespace FableMod
{
namespace Gfx
{

//-----------------------------------------------------------------------------

static inline bool KeyDown(int iKey)
{
	return (GetAsyncKeyState(iKey) & 0x8000) != 0;
}

//-----------------------------------------------------------------------------

CameraController::CameraController() :
m_pCamera(0)
{
}

//-----------------------------------------------------------------------------

CameraController::~CameraController()
{
}

//-----------------------------------------------------------------------------

void CameraController::Setup(Camera* pCamera, 
					  int iWidth, 
					  int iHeight,
					  bool bRighthanded)
{
	m_pCamera = pCamera;
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	
	Initialize();
}

//-----------------------------------------------------------------------------

void CameraController::Initialize()
{
	D3DXQuaternionIdentity(&m_qNow);
	/*
	m_qNow.x = -m_pCamera->GetDirection().x;
	m_qNow.y = -m_pCamera->GetDirection().y;
	m_qNow.z = -m_pCamera->GetDirection().z;
	m_qNow.w = 0.0f;
	*/

	m_fRadius = Settings::GetInstance()->GetFloat(
		_T("CameraController"),_T("RotationRadius"),80.0f);
    
	m_fZoom = Settings::GetInstance()->GetFloat(
		_T("CameraController"),_T("Zoom"),1);

	m_fZoomFactor = Settings::GetInstance()->GetFloat(
		_T("CameraController"),_T("ZoomFactor"),10.0f);
	
	m_fMaxMove = Settings::GetInstance()->GetFloat(
		_T("CameraController"),_T("MaxMoveSpeed"),5.0f);
	m_fMoveAcceleration = Settings::GetInstance()->GetFloat(
		_T("CameraController"),_T("MoveAcceleration"),0.5f);
	
	m_fMaxRotation = Settings::GetInstance()->GetFloat(
		_T("CameraController"),_T("MaxLookSpeed"),5.0f);
	m_fRotationAcceleration = Settings::GetInstance()->GetFloat(
			_T("CameraController"),_T("LookAcceleration"),0.5f);

	m_fMove = 0.0f;
	m_fRotation = 0.0f;

	m_uiMode = TM_OFF;
}

FLOAT CameraController::GetMove()
{
	m_fMove += m_fMoveAcceleration;

	if (m_fMove > m_fMaxMove)
		m_fMove = m_fMaxMove;

	return m_fMove;
}

FLOAT CameraController::GetRotation()
{
	m_fRotation += m_fRotationAcceleration;

	if (m_fRotation > m_fMaxRotation)
		m_fRotation = m_fMaxRotation;

	return m_fRotation;
}

static D3DXVECTOR3 RotateCamera(const D3DXVECTOR3& vView,
								const D3DXVECTOR3& vAxis,
								FLOAT fAngle)
{
	D3DXQUATERNION qTemp, qCon, qView, qResult;

	qTemp.x = vAxis.x*sinf(fAngle);
	qTemp.y = vAxis.y*sinf(fAngle);
	qTemp.z = vAxis.z*sinf(fAngle);
	qTemp.w = cosf(fAngle);

	qView.x = vView.x;
	qView.y = vView.y;
	qView.z = vView.z;
	qView.w = 0;

	D3DXQuaternionConjugate(&qCon, &qTemp);

	D3DXQuaternionMultiply(&qTemp, &qTemp, &qView);
	D3DXQuaternionMultiply(&qResult, &qTemp, &qCon);

	return D3DXVECTOR3(qResult.x, qResult.y, qResult.z);
}

void CameraController::Update()
{
	// Handle keys.

	const D3DXVECTOR3 vRight(m_pCamera->GetRight());
	const D3DXVECTOR3 vDirection(m_pCamera->GetDirection());
	
	D3DXVECTOR3 vEyeToAt(
		m_pCamera->GetInterest()-m_pCamera->GetPosition());
	
	D3DXVECTOR3 vAtToEye(
		m_pCamera->GetPosition()-m_pCamera->GetInterest());
	
	if (KeyDown(VK_ADD))
	{
		OnZoom(m_fZoom);
	}
	else if (KeyDown(VK_SUBTRACT))
	{
		OnZoom(-m_fZoom);
	}
	else /*if (!KeyDown(VK_LCONTROL) && !KeyDown(VK_RCONTROL))*/
	{
		if (m_uiMode != TM_MOVE)
		{
			D3DXVECTOR3 vMoveR(0,0,0);
			D3DXVECTOR3 vMoveF(0,0,0);
			bool bKeys = false;

			const FLOAT fMove = GetMove();

			if (KeyDown('A'))
			{
				vMoveR = -vRight*fMove;
				bKeys = true;
			}
			else if (KeyDown('D'))
			{
				vMoveR = vRight*fMove;
				bKeys = true;
			}
			
			if (KeyDown(VK_LSHIFT) || KeyDown(VK_RSHIFT))
			{
				if (KeyDown('W'))
				{
					vMoveF.z = fMove;
					bKeys = true;
				}
				else if (KeyDown('S'))
				{
					vMoveF.z = -fMove;
					bKeys = true;
				}
			}
			else
			{
				if (KeyDown('W'))
				{
					vMoveF = vDirection*fMove;
					vMoveF.z = 0.0;
					bKeys = true;
				}
				else if (KeyDown('S'))
				{
					vMoveF = (-vDirection)*fMove;
					vMoveF.z = 0.0;
					bKeys = true;
				}
			}

			if (bKeys)
			{
				const D3DXVECTOR3 vMove((vMoveR+vMoveF)/2);

				m_pCamera->SetPosition(m_pCamera->GetPosition()+vMove);
				m_pCamera->SetInterest(m_pCamera->GetPosition()+vEyeToAt);
			}
			else
				m_fMove = 0.0f;
		}

		D3DXMATRIX matRot;
		D3DXVECTOR3 vAxis;
		
		bool bKeys2 = false;
		const FLOAT fRot = GetRotation();
		float fYaw = 0.0f;
		float fPitch = 0.0f;

		if (KeyDown(VK_LEFT))
		{
			fYaw -= fRot;
			bKeys2 = true;
		}
		else if (KeyDown(VK_RIGHT))
		{
			fYaw += fRot;
			bKeys2 = true;
		}
		
		if (KeyDown(VK_UP))
		{
			fPitch -= fRot;
			bKeys2 = true;
		}
		else if (KeyDown(VK_DOWN))
		{
			fPitch += fRot;
			bKeys2 = true;
		}

		if (bKeys2)
		{
			fYaw = D3DXToRadian(fYaw);
			fPitch = D3DXToRadian(fPitch);

			D3DXVECTOR3 vAxis;
			D3DXVec3Cross(
				&vAxis, 
				&vDirection, 
				&m_pCamera->GetUp());

			D3DXVec3Normalize(&vAxis, &vAxis);

			D3DXVECTOR3 v(vDirection);

			v = RotateCamera(v, vAxis, fPitch);
			v = RotateCamera(v, D3DXVECTOR3(0, 0, 1), fYaw);
		
			v *= D3DXVec3Length(&vEyeToAt);

			m_pCamera->SetInterest(m_pCamera->GetPosition()+v);
		}
		else
			m_fRotation = 0.0f;
	}
}

//-----------------------------------------------------------------------------

void CameraController::SetZoom(FLOAT fZoom)
{
	m_fZoom = fZoom;
}

//-----------------------------------------------------------------------------

void CameraController::SetRadius(FLOAT fRadius)
{
	m_fRadius = fRadius;
}

//-----------------------------------------------------------------------------

inline D3DXQUATERNION QuatFromBallPoints(
	const D3DXVECTOR3& vFrom, const D3DXVECTOR3& vTo)
{
	D3DXVECTOR3 vPart;
    float fDot = D3DXVec3Dot(&vFrom, &vTo);
    D3DXVec3Cross(&vPart, &vFrom, &vTo);

    return D3DXQUATERNION(vPart.x, vPart.y, vPart.z, fDot);
}

//-----------------------------------------------------------------------------

void CameraController::OnMouseMove(const D3DXVECTOR2& vMouse)
{
	D3DXVECTOR3 vCamPos(m_pCamera->GetPosition());

	const D3DXVECTOR3 vCamDir(m_pCamera->GetInterest()-vCamPos);

	if (m_uiMode & TM_ROTATE)
	{
		m_qNow = m_qDown*QuatFromBallPoints(
			m_vDown, ScreenToVector(vMouse));
	
		D3DXMATRIX matRot;

		D3DXMatrixRotationQuaternion(&matRot, &m_qNow);
	
		D3DXMatrixInverse(&matRot, 0, &matRot);

		D3DXVECTOR3 vDir(0,0,D3DXVec3Length(&vCamDir));
		D3DXVec3TransformNormal(&vDir, &vDir, &matRot);
		
		m_pCamera->SetPosition(m_pCamera->GetInterest()+vDir);
	}
	else if (m_uiMode & TM_LOOK)
	{
		m_qNow = m_qDown*QuatFromBallPoints(
			m_vDown, ScreenToVector(vMouse));
	
		D3DXMATRIX matRot;

		D3DXMatrixRotationQuaternion(&matRot, &m_qNow);
	
		D3DXMatrixInverse(&matRot, 0, &matRot);

		D3DXVECTOR3 vDir(0,0,-D3DXVec3Length(&vCamDir));
		D3DXVec3TransformNormal(&vDir, &vDir, &matRot);
		
		m_pCamera->SetInterest(m_pCamera->GetPosition()+vDir);
	}
	else if (m_uiMode & TM_MOVE)
	{
		D3DXVECTOR3 vWorld;

        if (m_pCamera->ScreenToWorld(vMouse, 0, &vWorld))
		{
    		vCamPos += (m_vWorld-vWorld)/2;
			
			m_vCurMouse = vMouse;
		
			m_pCamera->SetPosition(vCamPos);
			m_pCamera->SetInterest(vCamPos+vCamDir);
		}
	}
}

//-----------------------------------------------------------------------------

void CameraController::OnZoom(FLOAT fDelta)
{
	// Zoom. Also makes sure the interest point is adjust accordingly,
	// when necessary.

	D3DXVECTOR3 vCamPos(m_pCamera->GetPosition());
	D3DXVECTOR3 vCamDir(m_pCamera->GetInterest()-vCamPos);

	const FLOAT fLen = D3DXVec3Length(&vCamDir);
 
    D3DXVec3Normalize(&vCamDir, &vCamDir);

    vCamPos += (vCamDir*fDelta)*(fLen/m_fZoomFactor);

	m_pCamera->SetPosition(vCamPos);
}

//-----------------------------------------------------------------------------

void CameraController::OnRotateDown(const D3DXVECTOR2& vMouse)
{
	m_vDown = ScreenToVector(vMouse);
	
	D3DXQuaternionRotationMatrix(&m_qNow, &m_pCamera->GetViewMatrix()); 
	
	m_qDown = m_qNow;
		
	m_uiMode = TM_ROTATE;
}

//-----------------------------------------------------------------------------

void CameraController::OnLookDown(const D3DXVECTOR2& vMouse)
{
	m_vDown = ScreenToVector(vMouse);
	
	D3DXQuaternionRotationMatrix(&m_qNow, &m_pCamera->GetViewMatrix()); 
	
	m_qDown = m_qNow;

	m_uiMode = TM_LOOK;
}

//-----------------------------------------------------------------------------

void CameraController::OnMoveDown(const D3DXVECTOR2& vMouse)
{
	m_vCurMouse = vMouse;
	m_uiMode = TM_MOVE;
	m_pCamera->ScreenToWorld(vMouse, 0, &m_vWorld);
}

//-----------------------------------------------------------------------------

void CameraController::OnUp()
{
	m_uiMode = TM_OFF;
}

//-----------------------------------------------------------------------------

D3DXVECTOR3 CameraController::ScreenToVector(
	const D3DXVECTOR2& vPoint)
{
	FLOAT fX = (vPoint.x-m_iWidth/2)/(m_fRadius*m_iWidth/2);
    FLOAT fY = -(vPoint.y-m_iHeight/2)/(m_fRadius*m_iHeight/2);

	FLOAT fZ   = 0.0f;
	FLOAT fMag = fX*fX+fY*fY;

    if (fMag > 1.0f)
    {
		FLOAT fScale = 1.0f/sqrt(fMag);
        fX *= fScale;
        fY *= fScale;
    }
    else
        fZ = sqrt(1.0f-fMag);

    // Return vector, again Fable.
    return D3DXVECTOR3(fX, fY, fZ);
}

//-----------------------------------------------------------------------------

}
}