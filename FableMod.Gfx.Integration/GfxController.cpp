#include "Stdafx.h"
#include "../FableMod.Gfx/Node.h"
#include "../FableMod.Gfx/Mesh.h"
#include "../FableMod.Gfx/Camera.h"
#include "../FableMod.Gfx/Manager.h"
#include "../FableMod.Gfx/CameraController.h"
#include "../FableMod.Gfx/OctTree.h"
#include "../FableMod.Gfx/Terrain.h"
#include "../FableMod.Gfx/TerrainManager.h"
#include "../FableMod.Gfx/Settings.h"
#include "../FableMod.Gfx/Consts.h"

#include "GfxView.h"
#include "GfxController.h"
#include "GfxModel.h"

using namespace System::Runtime::InteropServices;

#define USE_OCTTREE

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

GfxController::GfxController() :
m_pRoot(0)
{
	m_Views = gcnew List<GfxView^>();

	ResetObjects();
}

GfxController::~GfxController()
{
	Destroy();
}

void GfxController::Destroy()
{
	unsigned int uiObjectCount = 
		FableMod::Gfx::RootObject::GetNumObjects();

	GFX_SAFE_DELETE(m_pRoot);
	
	for (int i = 0; i < m_Views->Count; ++i)
		m_Views[i]->Destroy();
	
	m_Views->Clear();

	unsigned int uiObjectCountNew = 
		FableMod::Gfx::RootObject::GetNumObjects();

	TerrainManager::GetInstance()->RemoveAll();

	/*
	MessageBox::Show(
		"Objects: "+
		uiObjectCount.ToString()+" and "+
		uiObjectCountNew.ToString());
	*/
}

void GfxController::ResetObjects()
{
	GFX_SAFE_DELETE(m_pRoot);
	
	if (Settings::GetInstance()->GetInt(_T("OctTree"), _T("Use"), 0))
	{
		FableMod::Gfx::OctTree* pOctTree = new FableMod::Gfx::OctTree; 

		pOctTree->Build(
			D3DXVECTOR3(-2000,-2000,-2000),
			D3DXVECTOR3( 2000, 2000, 2000),
			D3DXVECTOR3(500,500,500));
	
		m_pRoot = new FableMod::Gfx::NodePtr(pOctTree);
		(*m_pRoot)->SetName(_T("GfxController_Root_OctTree"));
	}
	else
	{
		m_pRoot = new FableMod::Gfx::NodePtr(new FableMod::Gfx::Node());
		(*m_pRoot)->SetName(_T("GfxController_Root"));
	}

	for (int i = 0; i < m_Views->Count; ++i)
		m_Views[i]->OnResetObjects();
}

/// Add a view to the controller.
void GfxController::AddView(GfxView^ view)
{
	if (m_Views->IndexOf(view) >= 0)
		return;

	m_Views->Add(view);

	view->Controller = this;

	view->Initialize();

	view->FrontCamera();
}

void GfxController::AddModel(GfxBaseModel^ model)
{
	Spatial* pSpatial = model->GetGfx();
	pSpatial->SetScale(0.01f);
	GetRoot()->AttachChild(pSpatial);
}

void GfxController::AddModel(GfxModelLOD^ modelLOD)
{
	Spatial* pSpatial = modelLOD->GetGfx();
	pSpatial->SetScale(0.01f);
	GetRoot()->AttachChild(pSpatial);
}

/// Update all objects.
void GfxController::UpdateObjects()
{
	GetRoot()->Update();

	for (int i = 0; i < m_Views->Count; ++i)
	{
		m_Views[i]->Render();
	}
}

void GfxController::LoadObject(System::String^ filename)
{
	System::IntPtr ptr = Marshal::StringToHGlobalUni(filename);

	D3DXModelPtr spModel = new D3DXModel();

	try
	{
		spModel->Load((LPCTSTR)ptr.ToPointer());
	}
	catch (const FableMod::Gfx::Exception& ex)
	{
		MessageBox::Show(gcnew System::String(ex.GetMsg()));
		Marshal::FreeHGlobal(ptr);
		return;
	}

	MeshPtr spMesh = new Mesh();

	spMesh->SetModel(spModel);

	if (spMesh)
	{
		GetRoot()->AttachChild(spMesh);
	}
}

}
}
}