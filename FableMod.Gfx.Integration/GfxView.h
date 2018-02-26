#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "GfxController.h"
#include "ButtonConfiguration.h"

struct D3DXVECTOR2;

namespace FableMod
{
namespace Gfx 
{

class Node;
class Camera;
class CameraController;
class Mesh;
class Terrain;
class EditableTexture;

namespace Integration
{

public enum class CameraDirection
{
	None = 0,
	Front,
	Top,
	Right,
	Isometric
};

/// <summary>
/// GfxView is a base class for DirectX graphics editors. 
/// </summary>
public ref class GfxView : public System::Windows::Forms::UserControl
{
public:
	
	GfxView();

	void SaveScreenshot();
	
	virtual void OnResetObjects() {}
	void Render();
	void TopCamera();
	void RightCamera();
	void FrontCamera();	
	void IsometricCamera();	
	void Activate(bool activate) 
	{ 
		bool old = m_bActive;

		m_bActive = activate; 

		if (old != m_bActive && m_bActive && m_pCamera)
		{
			// Refresh the camera. 
			// Might have been on other windows.
			m_pCamera->Refresh();
			Render();
		}
	}
	
	void CameraFocus(Spatial* pSpatial, CameraDirection camDirection);

	virtual void Destroy();

	property GfxController^ Controller
	{
		GfxController^ get() { return m_Controller; }
		void set(GfxController^ value) { m_Controller = value; }
	}

	property float DrawDistance
	{
		float get();
		void set(float value);
	}

internal:

	virtual void Initialize();

protected:
	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~GfxView()
	{
		if (components)
		{
			delete components;
		}
	}

	::FableMod::Gfx::Node* GetRoot() { return m_Controller->GetRoot(); }
	::FableMod::Gfx::Camera* GetCamera() { return m_pCamera; }

	virtual void RenderInterface();
	virtual void CameraHeartbeat();
	virtual void DefaultButtonConfiguration();
	virtual void OnRender();

	void SetCamera(CameraDirection camDirection);

protected:

	void UpdateMouse(int x, int y);

	bool IsReady();
	bool IsActive() { return m_bActive; }

	::FableMod::Gfx::Camera* m_pCamera;
	D3DXVECTOR3* m_pvWorld;
	D3DXVECTOR2* m_pvMouse;
	CameraDirection m_SetCamera;
	::FableMod::Gfx::CameraController* m_pCameraController;
	ButtonConfiguration^ m_BtnCfg;
    
    bool m_bRenderBlock;
    
private: System::Windows::Forms::Timer^  timerCameraUpdate;
protected: 

	GfxController^ m_Controller;

	virtual void BtnCfg_OnButtonDown(
		System::String^ button, 
		System::Windows::Forms::MouseEventArgs^ e);
	virtual void BtnCfg_OnButtonUp(
		System::String^ button, 
		System::Windows::Forms::MouseEventArgs^ e);

	virtual void OnMouseDown(System::Windows::Forms::MouseEventArgs^  e) override;
	virtual void OnMouseMove(System::Windows::Forms::MouseEventArgs^  e) override;
	virtual void OnMouseUp(System::Windows::Forms::MouseEventArgs^  e) override;
private: System::ComponentModel::IContainer^  components;
protected: 

private:
	/// <summary>
	/// Required designer variable.
	/// </summary>

	
	bool m_bCameraInitialized;
	bool m_bInitialized;
	bool m_bActive;
	bool m_bErrors;

#pragma region Windows Form Designer generated code
	/// <summary>
	/// Required method for Designer support - do not modify
	/// the contents of this method with the code editor.
	/// </summary>
	void InitializeComponent(void)
	{
		this->components = (gcnew System::ComponentModel::Container());
		this->timerCameraUpdate = (gcnew System::Windows::Forms::Timer(this->components));
		this->SuspendLayout();
		// 
		// timerCameraUpdate
		// 
		this->timerCameraUpdate->Enabled = true;
		this->timerCameraUpdate->Interval = 30;
		this->timerCameraUpdate->Tick += gcnew System::EventHandler(this, &GfxView::timerCameraUpdate_Tick);
		// 
		// GfxView
		// 
		this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		this->Name = L"GfxView";
		this->Size = System::Drawing::Size(442, 315);
		this->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &GfxView::GfxView_MouseWheel);
		this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &GfxView::GfxView_Paint);
		this->ResumeLayout(false);

	}
#pragma endregion

private: System::Void GfxView_MouseWheel(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);

private: System::Void GfxView_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
		  if (!Focused)
			Render();
	 }
private: System::Void GfxView_OnIdle(System::Object^  sender, System::EventArgs^  e) {
		if (Focused)
			Render();
	 }
private: System::Void timerCameraUpdate_Tick(System::Object^  sender, System::EventArgs^  e) {
			if (Focused && m_bCameraInitialized)
				CameraHeartbeat();
		 }
};

}
}
}
