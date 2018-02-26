#include "Stdafx.h"
#include "../FableMod.Gfx/Camera.h"
#include "../FableMod.Gfx/Node.h"
#include "../FableMod.Gfx/D3DXModel.h"
#include "../FableMod.Gfx/Terrain.h"
#include "../FableMod.Gfx/EditableTexture.h"
#include "../FableMod.Gfx/TerrainManager.h"
#include "../FableMod.Gfx/OctTreeNode.h"
#include "../FableMod.Gfx/Manager.h"
#include "../FableMod.Gfx/CameraController.h"
#include "../FableMod.Gfx/Settings.h"
#include "GfxView.h"
#include "GfxManager.h"
#include "GfxController.h"
#include <tchar.h>

using namespace System::Runtime::InteropServices;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

GfxView::GfxView()
{
	InitializeComponent();
	
	m_bInitialized = false;
	m_bCameraInitialized = false;
	m_bActive = false;
	m_bErrors = false;

	Application::Idle += 
		gcnew System::EventHandler(this, &GfxView::GfxView_OnIdle);

	m_BtnCfg = gcnew ButtonConfiguration();

	DefaultButtonConfiguration();

	try	
	{
		// This can override.
		m_BtnCfg->Load(
			AppDomain::CurrentDomain->BaseDirectory+
			"FableMod.Buttons.XML");
	}
	catch (System::Exception^)
	{
	}

	m_BtnCfg->Attach(this);

	m_BtnCfg->OnButtonDown +=
		gcnew ButtonEventHandler(this, &GfxView::BtnCfg_OnButtonDown);
	m_BtnCfg->OnButtonUp +=
		gcnew ButtonEventHandler(this, &GfxView::BtnCfg_OnButtonUp);
    
    m_bRenderBlock = false;
}

void GfxView::DefaultButtonConfiguration()
{
	m_BtnCfg->Add("CameraOrbit", Buttons::MButton);
	m_BtnCfg->Add("CameraMove", Buttons::MButton | Buttons::Shift);
	m_BtnCfg->Add("CameraLook", Buttons::MButton | Buttons::Ctrl);
	
	m_BtnCfg->Add("ViewTop", Buttons::NumPad7);
	m_BtnCfg->Add("ViewFront", Buttons::NumPad1);
	m_BtnCfg->Add("ViewSide", Buttons::NumPad3);
}

void GfxView::Destroy()
{
	m_bInitialized = false;
	m_bCameraInitialized = false;

	GFX_SAFE_DELETE(m_pCameraController);
	GFX_SAFE_DELETE(m_pCamera);
	GFX_SAFE_DELETE(m_pvWorld);
	GFX_SAFE_DELETE(m_pvMouse);
}

void GfxView::SaveScreenshot()
{
	SaveFileDialog^ form = gcnew SaveFileDialog();
	form->Filter = 
		"JPG Files (*.jpg)|*.jpg|PNG Files (*.png)|*.png||";
	form->DefaultExt = "jpg";
	
	if (form->ShowDialog() == DialogResult::OK)
	{
		try
		{
			GfxManager::SaveScreenToFile(form->FileName);
		}
		catch (System::Exception^ ex)
		{
			MessageBox::Show(this, ex->Message);
		}
	}
}

void GfxView::CameraHeartbeat()
{
	m_pCameraController->Update();
}

void GfxView::DrawDistance::set(float value)
{
	m_pCamera->SetFrustumFar(value);
	Render();
}

float GfxView::DrawDistance::get()
{
	return m_pCamera->GetFrustumFar();
}

void GfxView::CameraFocus(Spatial* pSpatial, CameraDirection camDirection)
{
	if (!m_bCameraInitialized)
		return;

	const FLOAT fRad = pSpatial->GetWorldBound()->Radius();
	const FLOAT fLen = fRad*1.6f;
	
	m_pCamera->SetInterest(pSpatial->GetWorldBound()->Center());

	const FLOAT fAngle = tan(D3DXToRadian(m_pCamera->GetFOV())*0.5f);
	const FLOAT fRadAngle = fRad/fAngle;

	if (camDirection == CameraDirection::Front)
	{
		m_pCamera->SetPosition(
			m_pCamera->GetInterest()+D3DXVECTOR3(0, fRadAngle, 0));
	}
	else if (camDirection == CameraDirection::Right)
	{
		m_pCamera->SetPosition(
			m_pCamera->GetInterest()+D3DXVECTOR3(-fRadAngle, 0, 0));
	}
	else if (camDirection == CameraDirection::Top)
	{
		m_pCamera->SetPosition(
			m_pCamera->GetInterest()+D3DXVECTOR3(0, 1, fRadAngle));
	}
	else if (camDirection == CameraDirection::Isometric)
	{
		const FLOAT fTmp = fRadAngle*0.5f;
		
		m_pCamera->SetPosition(
			m_pCamera->GetInterest()+D3DXVECTOR3(fTmp, fTmp, fTmp));
	}
}

void GfxView::SetCamera(CameraDirection camDirection)
{
	CameraFocus(GetRoot(), camDirection);
}

void GfxView::RightCamera()
{
	m_SetCamera = CameraDirection::Right;
}

void GfxView::FrontCamera()
{
	m_SetCamera = CameraDirection::Front;
}

void GfxView::TopCamera()
{
	m_SetCamera = CameraDirection::Top;
}

void GfxView::IsometricCamera()
{
	m_SetCamera = CameraDirection::Isometric;
}

void GfxView::Initialize()
{
	if (m_bInitialized)
		return;

	m_pCamera = new ::FableMod::Gfx::Camera();
	m_pCameraController = new ::FableMod::Gfx::CameraController();

	m_pCamera->SetViewport(
		0,
		0,
		::FableMod::Gfx::Manager::GetDevice()->GetWidth(),
		::FableMod::Gfx::Manager::GetDevice()->GetHeight());
	
	m_pCamera->SetFOV(35);

	m_pvWorld = new D3DXVECTOR3(0,0,0);
	m_pvMouse = new D3DXVECTOR2(0,0);

	m_bInitialized = true;
	m_SetCamera = CameraDirection::None;
}

bool GfxView::IsReady()
{
	if (!m_bInitialized || m_bErrors)
		return false;

	::FableMod::Gfx::Device* pDevice = 
		::FableMod::Gfx::Manager::GetDevice();

	if (!pDevice->IsReady())
		return false;

	return true;
}

void GfxView::RenderInterface()
{
	// Stub
}

void GfxView::OnRender()
{
}

void GfxView::Render()
{
	if (!m_bInitialized || m_bRenderBlock || m_bErrors || !IsActive())
		return;

	::FableMod::Gfx::Device* pDevice = 
		::FableMod::Gfx::Manager::GetDevice();

	::FableMod::Gfx::Device::EDeviceState state =
		pDevice->CheckState();

	if (state == ::FableMod::Gfx::Device::DS_NOT_READY ||
		state == ::FableMod::Gfx::Device::DS_LOST)
	{
		Sleep(200);
		return;
	}
	else if (state == ::FableMod::Gfx::Device::DS_RESET)
	{
		// Must reset device.

		// TODO: The objects should be reset... argh.

		if (!pDevice->Reset())
		{
			// Errors.
			m_bErrors = true;
			MessageBox::Show("Failed to reset Direct3D device.");
			return;
		}
	}
	
	pDevice->Lock();

	try
	{
		pDevice->ClearBuffers(D3DCOLOR_ARGB(255,160,160,160));
	
		Node* pRoot = GetRoot();

		if (m_bInitialized)
		{
			if (pRoot)
			{
				pRoot->Update();
				
				if (m_SetCamera != CameraDirection::None)
				{
					if (!m_bCameraInitialized)
					{
						m_pCameraController->Setup(
							m_pCamera, 
							pDevice->GetWidth(), 
							pDevice->GetHeight());
					}

					m_bCameraInitialized = true;					

					SetCamera(m_SetCamera);
						
					m_SetCamera = CameraDirection::None;
				}

				m_pCamera->Update();	
			
				OnRender();

				m_pCamera->RenderScene(pRoot);
			}
			
			RenderInterface();
		}

		/*
		int iWidth = ClientSize.Width;
		int iHeight = ClientSize.Height;

		int iDstWidth = ClientSize.Width;
		int iDstHeight = ClientSize.Height;

		if (iWidth > pDevice->GetWidth())
			iWidth = pDevice->GetWidth();
		
		if (iHeight > pDevice->GetHeight())
			iHeight = pDevice->GetHeight();
		
		RECT rcSrc;
		rcSrc.left = 0;
		rcSrc.top = 0;
		rcSrc.right = iWidth;
		rcSrc.bottom = iHeight;

		RECT rcDst;
		rcDst.left = 0;
		rcDst.top = 0;
		rcDst.right = iDstWidth;
		rcDst.bottom = iDstHeight;
		*/

		pDevice->Present((HWND)Handle.ToPointer()); 
	}
	catch (const FableMod::Gfx::Exception& gfxEx)
	{
		m_bErrors = true;
		MessageBox::Show(String::Format(
			"Graphics Exception: {0}.\r\nRendering stopped.",
			gcnew String(gfxEx.GetMsg())));
	}
	catch (System::Exception^ ex)
	{
		m_bErrors = true;
		MessageBox::Show(String::Format(
			"Exception: {0}.\r\nRendering stopped.", ex->ToString()));
	}

	pDevice->Unlock();
}

void GfxView::UpdateMouse(int x, int y)
{
	if (!IsReady())
		return;

	FableMod::Gfx::Device* pDevice = FableMod::Gfx::Manager::GetDevice();

	int iWidth = pDevice->GetWidth();
	int iHeight = pDevice->GetHeight();
	/*
	int iWidth = ClientSize.Width;
	int iHeight = ClientSize.Height;

	if (iWidth > pDevice->GetWidth())
		iWidth = pDevice->GetWidth();
	
	if (iHeight > pDevice->GetHeight())
		iHeight = pDevice->GetHeight();
	*/
	m_pvMouse->x = x*((float)iWidth/(float)ClientSize.Width);
	m_pvMouse->y = y*((float)iHeight/(float)ClientSize.Height);
	 
	m_pCamera->ScreenToWorld(*m_pvMouse, 0, m_pvWorld);
}

void GfxView::BtnCfg_OnButtonDown(
	System::String^ button, System::Windows::Forms::MouseEventArgs^ e)
{
	if (!IsReady() || !e)
		return;

	if (!Focused)
		Focus();

	UpdateMouse(e->X, e->Y);

	if (button == "CameraMove")
	{
		m_pCameraController->OnMoveDown(*m_pvMouse);
	}
	else if (button == "CameraOrbit")
	{
		m_pCameraController->OnRotateDown(*m_pvMouse);
	}
	else if (button == "CameraLook")
	{
		m_pCameraController->OnLookDown(*m_pvMouse);
	}
}

void GfxView::BtnCfg_OnButtonUp(
	System::String^ button, System::Windows::Forms::MouseEventArgs^ e)
{
	if (!IsReady())
		return;
	
	if (button->StartsWith("Camera"))
	{
		m_pCameraController->OnUp();
	}
	else if (button == "ViewFront")
		FrontCamera();
	else if (button == "ViewTop")
		TopCamera();
	else if (button == "ViewSide")
		RightCamera();
}

System::Void GfxView::OnMouseMove(System::Windows::Forms::MouseEventArgs^  e)
{
	if (!IsReady())
		return;

	UpdateMouse(e->X, e->Y);
	
	m_pCameraController->OnMouseMove(*m_pvMouse);
	 
	UserControl::OnMouseMove(e);
}

System::Void GfxView::OnMouseDown(System::Windows::Forms::MouseEventArgs^  e)
{ 
	UserControl::OnMouseDown(e);
}

System::Void GfxView::OnMouseUp(System::Windows::Forms::MouseEventArgs^  e)
{
	UserControl::OnMouseUp(e);
}

System::Void GfxView::GfxView_MouseWheel(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
	if (!IsReady())
		return;

	m_pCameraController->OnZoom((float)e->Delta/120);
}

}
}
}