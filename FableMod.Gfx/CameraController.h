#pragma once

#include "GfxConf.h"
#include "Camera.h"

namespace FableMod
{
namespace Gfx
{

/// CameraController implements basic functionality to control 
/// the 3D camera with the mouse and keyboard.
class GFX_API CameraController
{
public:

	CameraController();

	virtual ~CameraController();

	void Setup(Camera* pCamera, 
			   int iWidth, 
			   int iHeight,
			   bool bRighthanded = false);

	void Update();

	void SetZoom(FLOAT fZoom);

	void SetRadius(FLOAT fRadius);

	void OnRotateDown(const D3DXVECTOR2& vMouse);

	void OnLookDown(const D3DXVECTOR2& vMouse);

	void OnMoveDown(const D3DXVECTOR2& vMouse);

	void OnZoom(float fDelta);

	void OnMouseMove(const D3DXVECTOR2& vMouse);

	void OnUp();

protected:

	FLOAT GetMove();
	FLOAT GetRotation();
	void ComputeCamera(float fY, float fP, float fR);

	void Initialize();

	enum EMode
	{
		TM_OFF = 0,
		TM_ROTATE = 1,
		TM_MOVE = 2,
		TM_ZOOM = 4,
		TM_LOOK = 16,
	};

	D3DXVECTOR3 ScreenToVector(const D3DXVECTOR2& vPoints);

	unsigned int			m_uiMode;
	D3DXQUATERNION			m_qNow;
	D3DXQUATERNION			m_qDown;
	D3DXVECTOR3				m_vDown;
	D3DXVECTOR2				m_vCurMouse;
	FLOAT					m_fRadius;
	bool					m_bMoveZ;
	int						m_iWidth;
	int						m_iHeight;
	Camera*					m_pCamera;
	
	FLOAT					m_fZoom;
	FLOAT					m_fZoomFactor;
	
	FLOAT					m_fMove;
	FLOAT					m_fMaxMove;
	FLOAT					m_fMoveAcceleration;
	
	FLOAT					m_fRotation;
	FLOAT					m_fMaxRotation;
	FLOAT					m_fRotationAcceleration;
	
	D3DXVECTOR3				m_vWorld;
};

}
}