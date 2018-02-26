#include "Stdafx.h"
#include "GfxThingView.h"
#include "GfxThingController.h"
#include "GfxThingInterface.h"
#include "Defs.h"

using namespace System::Runtime::InteropServices;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

	
static void CreateDirtyRect(EditableTexture* pTexture, 
							int iX, int iY, 
							int iSize, 
							RECT* pArea)
{
	int iX1 = iX-iSize;
	int iY1 = iY-iSize;
	int iX2 = iX+iSize;
	int iY2 = iY+iSize;

	if (iX1 < 0)
		iX1 = 0;
	
	if (iY1 < 0)
		iY1 = 0;

	if (iX2 > pTexture->GetWidth())
		iX2 = pTexture->GetWidth();

	if (iY2 > pTexture->GetHeight())
		iY2 = pTexture->GetHeight();

	pArea->left = iX1;
	pArea->top = iY1;
	pArea->right = iX2;
	pArea->bottom = iY2;
}

void GfxThingView::Initialize()
{
	GfxView::Initialize();

	m_fPlayerHeight = 2;
	m_fPlayerFOV = 45;

	m_pSelection = new FableMod::Gfx::SelectionBox();

	m_pArrow = 0;
	m_bMoving = false;

	m_pvMoveDiff = new D3DXVECTOR3(0,0,0);
	m_pvMovePoint = new D3DXVECTOR3(0,0,0);

	m_pvMousePoint = new D3DXVECTOR2(0,0);

	m_pXArrowMesh = new MeshPtr();
	m_pYArrowMesh = new MeshPtr();
	m_pZArrowMesh = new MeshPtr();

	*m_pXArrowMesh = new Mesh();
	*m_pYArrowMesh = new Mesh();
	*m_pZArrowMesh = new Mesh();

	(*m_pXArrowMesh)->SetRotation(
		D3DXVECTOR3(0, D3DXToRadian(90), 0));

	(*m_pYArrowMesh)->SetRotation(
		D3DXVECTOR3(D3DXToRadian(-90), 0, 0));
	
	ArrowheadModelPtr pModelX = new ArrowheadModel();
	pModelX->SetColor(D3DCOLOR_ARGB(255, 255, 0, 0));
	(*m_pXArrowMesh)->SetModel(pModelX);
	(*m_pXArrowMesh)->Flags() &= ~FLAGS_RENDERABLE;
	(*m_pXArrowMesh)->RenderFlags() |= MRF_DISABLE_ZTEST;

	ArrowheadModelPtr pModelY = new ArrowheadModel();
	pModelY->SetColor(D3DCOLOR_ARGB(255, 0, 255, 0));
	(*m_pYArrowMesh)->SetModel(pModelY);
	(*m_pYArrowMesh)->Flags() &= ~FLAGS_RENDERABLE;
	(*m_pYArrowMesh)->RenderFlags() |= MRF_DISABLE_ZTEST;

	ArrowheadModelPtr pModelZ = new ArrowheadModel();
	pModelZ->SetColor(D3DCOLOR_ARGB(255, 0, 0, 255));
	(*m_pZArrowMesh)->SetModel(pModelZ);
	(*m_pZArrowMesh)->Flags() &= ~FLAGS_RENDERABLE;
	(*m_pZArrowMesh)->RenderFlags() |= MRF_DISABLE_ZTEST;

	m_pAxes = new NodePtr(new Node());
	
	(*m_pAxes)->SetName(_T("GfxThingView_Axes"));

	(*m_pAxes)->AttachChild(*m_pXArrowMesh);
	(*m_pAxes)->AttachChild(*m_pYArrowMesh);
	(*m_pAxes)->AttachChild(*m_pZArrowMesh);

	(*m_pAxes)->Flags() |= FLAGS_FORCE_CULLING;

	GetRoot()->AttachChild(*m_pAxes);

	m_pPlane = new Plane();

	Mode = EditorMode::Normal;

	m_bDirectionAxes = false;

	m_pWorkTexture = 0;
	m_iBrushSize = 1;

	m_NavEditMode = NavigationEditMode::Navigation;

	m_iDrawState = 0;

	m_STBDebugFlora = false;
	m_STBDebugBlocks = false;
}

void GfxThingView::DefaultButtonConfiguration()
{
	GfxView::DefaultButtonConfiguration();
	
	m_BtnCfg->Add("ThingMove", Buttons::LButton);
	m_BtnCfg->Add("ThingCopyMove", Buttons::Ctrl | Buttons::LButton);
	m_BtnCfg->Add("ThingRotate", Buttons::RButton);
	m_BtnCfg->Add("ThingCopyRotate", Buttons::Ctrl | Buttons::RButton);
	
	m_BtnCfg->Add("ThingSelect", Buttons::LButton);
	m_BtnCfg->Add("ThingAddSelect", Buttons::LButton | Buttons::Ctrl);
	m_BtnCfg->Add("ThingFocus", Buttons::DLButton);
	m_BtnCfg->Add("ThingCollisions", Buttons::Shift);
	m_BtnCfg->Add("ThingCreate", Buttons::LButton);

	m_BtnCfg->Add("WalkDraw", Buttons::LButton);
	m_BtnCfg->Add("WalkErase", Buttons::RButton);
	
	// Disable buttons.
	m_BtnCfg->Enable("ThingCreate", false);
	m_BtnCfg->Enable("Walk.+", false);
}

void GfxThingView::Reset()
{
	Mode = EditorMode::Normal;
	ClearSelection();
}

void GfxThingView::SaveChanges()
{
	SaveGroundChanges();

	if (Mode == EditorMode::Navigation)
		ToNavMode();
	else if (Mode == EditorMode::Walk)
		ToWalkMode();
}

void GfxThingView::ResetNavSections()
{
	m_EditMap->LEV->ResetSections();

	if (Mode == EditorMode::Navigation)
		ToNavMode();
}

void GfxThingView::UpdateAll()
{
	if (GetRoot())
		GetRoot()->Update();

	m_pSelection->Update();

	SetupAxes();

	Render();
}

void GfxThingView::OnResetObjects()
{
	GetRoot()->AttachChild(*m_pAxes);
}

void GfxThingView::Destroy()
{
	GfxView::Destroy();
	
	GFX_SAFE_DELETE(m_pWorkTexture);

	GFX_SAFE_DELETE(m_pPlane);
	GFX_SAFE_DELETE(m_pSelection);
	GFX_SAFE_DELETE(m_pvMoveDiff);

	GFX_SAFE_DELETE(m_pAxes);
	GFX_SAFE_DELETE(m_pXArrowMesh);
	GFX_SAFE_DELETE(m_pYArrowMesh);
	GFX_SAFE_DELETE(m_pZArrowMesh);

	//GFX_SAFE_RELEASE(m_pGridVB);
}

void GfxThingView::FindByUID(String^ uid)
{
	ClearSelection();

	FableMod::TNG::Thing^ thing = ThingController->FindThingUID(uid);

	if (thing)
	{
		GfxThingInterface^ iface = (GfxThingInterface^)thing->Interface;

		if (iface)
		{
			m_pSelection->Add(iface->GetNode());

			SetSelectionHighlight(true);

			CallThingSelected();

			CameraFocus(iface->GetNode(), CameraDirection::Front);

			UpdateAll();
		}
	}
}

bool GfxThingView::AxesOn()
{
	return (GetAxes()->Flags() & FLAGS_FORCE_CULLING) == 0;
}

void GfxThingView::SetupPlane(FableMod::Gfx::Mesh* pArrowMesh)
{
	const D3DXVECTOR3 vC(GetAxes()->GetWorldTranslation());
	
	D3DXVECTOR3 vCamDir(vC-GetCamera()->GetPosition());

	D3DXVec3Normalize(&vCamDir, &vCamDir);

	// Determine the plane normal that we use to move 
	// the object around. We need a plane that can handle
	// the chosen axis.

	if (pArrowMesh == (*m_pXArrowMesh))
	{
		if (fabs(vCamDir.z) >= fabs(vCamDir.y))
		{
			if (vCamDir.z < 0)
				m_pPlane->Normal() = D3DXVECTOR3(0, 0, 1);
			else
				m_pPlane->Normal() = D3DXVECTOR3(0, 0, -1);
		}
		else
		{
			if (vCamDir.y < 0)
				m_pPlane->Normal() = D3DXVECTOR3(0, 1, 0);
			else
				m_pPlane->Normal() = D3DXVECTOR3(0, -1, 0);
		}
	}
	else if (pArrowMesh == (*m_pYArrowMesh))
	{
		if (fabs(vCamDir.z) >= fabs(vCamDir.x))
		{
			if (vCamDir.z < 0)
				m_pPlane->Normal() = D3DXVECTOR3(0, 0, 1);
			else
				m_pPlane->Normal() = D3DXVECTOR3(0, 0, -1);
		}
		else
		{
			if (vCamDir.x < 0)
				m_pPlane->Normal() = D3DXVECTOR3(1, 0, 0);
			else
				m_pPlane->Normal() = D3DXVECTOR3(-1, 0, 0);
		}
	}
	else if (pArrowMesh == (*m_pZArrowMesh))
	{
		if (fabs(vCamDir.y) >= fabs(vCamDir.x))
		{
			if (vCamDir.y < 0)
				m_pPlane->Normal() = D3DXVECTOR3(0, 1, 0);
			else
				m_pPlane->Normal() = D3DXVECTOR3(0, -1, 0);
		}
		else
		{
			if (vCamDir.x < 0)
				m_pPlane->Normal() = D3DXVECTOR3(1, 0, 0);
			else
				m_pPlane->Normal() = D3DXVECTOR3(-1, 0, 0);
		}
	}

	m_pPlane->Constant() =
		D3DXVec3Dot(&m_pPlane->Normal(), &vC);
}

void GfxThingView::SetupAxes()
{
	GetAxes()->Flags() &= ~FLAGS_FORCE_CULLING;

	GetAxes()->SetWorldTranslation(m_pSelection->GetCenter());

	const FLOAT fW = m_pSelection->GetWidth()/1.5f;
	const FLOAT fH = m_pSelection->GetHeight()/1.5f;
	const FLOAT fD = m_pSelection->GetDepth()/1.5f;

	FLOAT fMax = m_pSelection->GetRadius()/8;

	if (fMax < 0.01f)
		fMax = 0.01f;

	(*m_pXArrowMesh)->SetScale(fMax);
	(*m_pYArrowMesh)->SetScale(fMax);
	(*m_pZArrowMesh)->SetScale(fMax);

	(*m_pXArrowMesh)->SetTranslation(D3DXVECTOR3(fW, 0, 0));
	(*m_pYArrowMesh)->SetTranslation(D3DXVECTOR3(0, fD, 0));
	(*m_pZArrowMesh)->SetTranslation(D3DXVECTOR3(0, 0, fH));

	GetAxes()->Update();
}

void GfxThingView::DrawAxes()
{
	static const D3DCOLOR COLORX = D3DCOLOR_ARGB(255,255,0,0);
	static const D3DCOLOR COLORY = D3DCOLOR_ARGB(255,0,255,0);
	static const D3DCOLOR COLORZ = D3DCOLOR_ARGB(255,0,0,255);

	if (!AxesOn())
		return;

	const D3DXVECTOR3 vC(GetAxes()->GetWorldTranslation());
	const D3DXVECTOR3 vX((*m_pXArrowMesh)->GetWorldTranslation());
	const D3DXVECTOR3 vY((*m_pYArrowMesh)->GetWorldTranslation());
	const D3DXVECTOR3 vZ((*m_pZArrowMesh)->GetWorldTranslation());

	Manager::GetDevice()->DrawLine(vC, vX, COLORX);
	Manager::GetDevice()->DrawLine(vC, vY, COLORY);
	Manager::GetDevice()->DrawLine(vC, vZ, COLORZ);

	m_pCamera->SetDistanceToCamera(*m_pXArrowMesh);
	m_pCamera->SetDistanceToCamera(*m_pYArrowMesh);
	m_pCamera->SetDistanceToCamera(*m_pZArrowMesh);

	(*m_pXArrowMesh)->Render(m_pCamera);
	(*m_pYArrowMesh)->Render(m_pCamera);
	(*m_pZArrowMesh)->Render(m_pCamera);
}

void GfxThingView::ShowSTBTerrain(bool show)
{
	if (m_EditMap == nullptr || !m_EditMap->GetSTBTerrain())
		return;

	if (show)
	{
		m_EditMap->GetTerrain()->Flags() |= FLAGS_FORCE_CULLING;
		m_EditMap->GetSTBTerrain()->Flags() &= ~FLAGS_FORCE_CULLING;
	}
	else
	{
		m_EditMap->GetTerrain()->Flags() &= ~FLAGS_FORCE_CULLING;
		m_EditMap->GetSTBTerrain()->Flags() |= FLAGS_FORCE_CULLING;
	}
}

void GfxThingView::ToNormalMode()
{
	m_BtnCfg->Enable("Walk", false);
	m_BtnCfg->Enable("Thing", true);
	m_BtnCfg->Enable("ThingCreate", false);
	
	SaveGroundChanges();
	
	ThingController->ToGroundEditMode(false);

	if (Mode == EditorMode::Player)
	{
		m_pCamera->SetFOV(m_fSavedFOV);
	}

	ShowSTBTerrain(true);
}

void GfxThingView::SaveGroundChanges()
{
	// Save the changes from the ground texture.

	if ((Mode == EditorMode::Walk || Mode == EditorMode::Navigation) &&
		m_Maps && m_EditMap)
	{
		Terrain* pTerrain = m_EditMap->GetTerrain();

		if (pTerrain->GetTexture(1))
		{
			m_pWorkTexture->CopyTo(pTerrain->GetTexture(1));

			if (Mode == EditorMode::Walk)
				m_EditMap->FinishWalkTexture();
			else if (Mode == EditorMode::Navigation)
				m_EditMap->FinishNavTexture(EditSection, EditSubset);
		}
	
		GFX_SAFE_DELETE(m_pWorkTexture);
	}
}

void GfxThingView::ToGroundMode()
{
	if (Mode == EditorMode::Normal || Mode == EditorMode::Player)
	{
		ClearSelection();
		ThingController->ToGroundEditMode(true);
	}
	else	
		SaveGroundChanges();
}

void GfxThingView::ToWalkMode()
{
	// Note when this is called, the Mode hasn't been set to 
	// the new state yet.
	
	ToGroundMode();

	if (m_Maps && m_EditMap)
	{
		ShowSTBTerrain(false);

		GFX_SAFE_DELETE(m_pWorkTexture);

		m_EditMap->CreateWalkTexture();
			
		EditableTexture* pTexture = 
			m_EditMap->GetTerrain()->GetTexture(1);

		m_pWorkTexture = pTexture->Duplicate();

		pTexture->CopyTo(m_pWorkTexture);
	}

	m_BtnCfg->Enable("Walk", true);
	m_BtnCfg->Enable("Thing", false);

	m_iX = m_iY = -1;
}

void GfxThingView::ToNavMode()
{
	ToGroundMode();

	if (m_Maps && m_EditMap)
	{
		ShowSTBTerrain(false);

		m_EditMap->CreateNavTexture(EditSection, EditSubset);
			
		EditableTexture* pTexture = 
			m_EditMap->GetTerrain()->GetTexture(1);

		if (pTexture)
		{
			GFX_SAFE_DELETE(m_pWorkTexture);

			m_pWorkTexture = pTexture->Duplicate();
			
			pTexture->CopyTo(m_pWorkTexture);
		}
	}

	m_iX = m_iY = -1;
}

void GfxThingView::Mode::set(EditorMode value) 
{ 
	if (value != m_EditorMode)
	{
		if (value == EditorMode::Create)
		{
			m_BtnCfg->Enable("Thing", false);
			m_BtnCfg->Enable("ThingCreate", true);
			Cursor = Cursors::Cross;
		}
		else if (value == EditorMode::Pick)
		{
			m_BtnCfg->Enable("ThingCreate", true);
			Cursor = Cursors::Cross;
		}
		else
		{
			Cursor = Cursors::Default;
		}
		
		if (value == EditorMode::Walk)
		{
			ToWalkMode();
		}
		else if (value == EditorMode::Navigation)
		{
			ToNavMode();
		}
		else if (value == EditorMode::Player)
		{
			ToNormalMode();

			m_fSavedFOV = m_pCamera->GetFOV();
			m_pCamera->SetFOV(m_fPlayerFOV);
		}
		else if (value == EditorMode::Normal)
		{
			ToNormalMode();
		}
		

		// Now set the new state.
		m_EditorMode = value; 
	}
}

void GfxThingView::OnRender()
{
	if ((Mode == EditorMode::Navigation || Mode == EditorMode::Walk) && 
		m_iDrawState != 0) 
	{
		D3DXVECTOR3 vO,vD;

		if (GetCamera()->ScreenToRay(*m_pvMouse, &vO, &vD))
		{	
			unsigned int x, y;

			Terrain* pTerrain = m_EditMap->GetTerrain();

			int iWidth = pTerrain->GetWidth();
			int iHeight = pTerrain->GetHeight();

			if (Mode == EditorMode::Navigation)
			{
				iWidth *= 2;
				iHeight *= 2;
			}

			if (pTerrain->GetLocation(
				vO, vD, iWidth, iHeight, x, y))
			{
				EditableTexture* pTexture = pTerrain->GetTexture(1);
						
				if (!pTexture)
					return;

				RECT rcArea;

				if (m_iX >= 0 && m_iY >= 0)
				{
					CreateDirtyRect(
						pTexture, m_iX, m_iY, m_iBrushSize, &rcArea);

					m_pWorkTexture->CopyTo(pTexture, &rcArea);
				}
				else
					m_pWorkTexture->CopyTo(pTexture);
				
				D3DCOLOR Color = GFXI_COLOR_NONE;

				if (Mode == EditorMode::Walk)
				{
					Color = m_MapColor;
				}
				else
				{
					if (m_iDrawState == 2)
						Color = GFXI_COLOR_EMPTY;
					else if (m_iDrawState == 1)
					{
						if (NavEditMode == NavigationEditMode::Navigation)
							Color = GFXI_COLOR_NAVIGATION;
						else
							Color = GFXI_COLOR_DYNAMIC;
					}
				}

				bool bUpdateWork = false;

				pTexture->Lock();

				if (Color != GFXI_COLOR_NONE)
				{
					int iSize = m_iBrushSize;
					
					if (m_iX >= 0 && m_iY >= 0)
					{
						pTexture->DrawThickLine(
							m_iX, m_iY, 
							x, y,
							Color,
							m_iBrushSize);

						iSize += max(abs((int)(m_iX-x)),abs((int)(m_iY-y)));
					}
					else
					{
						pTexture->DrawFilledCircle(
							x,y,
							m_iBrushSize/2,
							Color);
					}

					CreateDirtyRect(
						pTexture, x, y, iSize, &rcArea);

					bUpdateWork = true;
				}
				else
				{
					pTexture->DrawFilledCircle(
						x,y,
						m_iBrushSize/2,
						GFXI_COLOR_CURSOR);
				}

				m_iX = x;
				m_iY = y;
				
				pTexture->Unlock();

				if (bUpdateWork)
				{
					if (Mode == EditorMode::Navigation)
					{
						m_EditMap->LEV->Modified = true;
					}

					pTexture->CopyTo(m_pWorkTexture, &rcArea);
				}
			}
			else
			{
				m_iX = -1;
				m_iY = -1;
			}
		}

		m_iDrawState = 0;
	}
}

void GfxThingView::RenderInterface()
{
	TCHAR szTmp[2048];

	FableMod::Gfx::Device* pDevice = FableMod::Gfx::Manager::GetDevice();
	
	szTmp[0] = '\0';

	if (!m_pSelection->IsEmpty())
	{
		if (m_bDirectionAxes)
		{
			for (int i = 0; i < m_pSelection->GetNumObjects(); ++i)
			{
				Spatial* pSpatial = m_pSelection->GetObjectAt(i);

				FLOAT fLength = pSpatial->GetWorldBound()->Radius()*2;

				const D3DXVECTOR3 c(pSpatial->GetWorldBound()->Center());
				const D3DXVECTOR3 f(pSpatial->GetWorldForward());
				const D3DXVECTOR3 r(pSpatial->GetWorldRight());
				const D3DXVECTOR3 u(pSpatial->GetWorldUp());

				Manager::GetDevice()->DrawLine(
					c, c+r*fLength, D3DCOLOR_ARGB(255,255,0,255));

				Manager::GetDevice()->DrawLine(
					c, c+f*fLength, D3DCOLOR_ARGB(255,255,255,0));

				Manager::GetDevice()->DrawLine(
					c, c+u*fLength, D3DCOLOR_ARGB(255,0,255,255));
			}
		}

		if (!m_bRotating)
		{
			// Draw a bounding box.

			pDevice->DrawBox(
				m_pSelection->GetMin(),
				m_pSelection->GetMax(), 
				D3DCOLOR_ARGB(255, 255, 255, 25));
		}
	}

	if (m_STBDebugFlora)
	{
		STBRenderDebugFlora();
	}

	if (m_STBDebugBlocks)
	{
		STBRenderDebugBlocks();
	}

	DrawAxes();

	if (Mode == EditorMode::Player)
	{
		pDevice->DrawText(
			6, 26, _T("First Person Mode"), D3DCOLOR_ARGB(255,0,0,0));

		pDevice->DrawText(
			5, 25, _T("First Person Mode"), D3DCOLOR_ARGB(255,255,255,255));
	}

	_stprintf(szTmp, _T("Rendered objects: %u"), m_pCamera->GetRenderCount());

	pDevice->DrawText(6, 6, szTmp, D3DCOLOR_ARGB(255,0,0,0));
	pDevice->DrawText(5, 5, szTmp, D3DCOLOR_ARGB(255,255,255,255));
}

void GfxThingView::STBRenderDebugBlocks()
{
	ThingMap^ map = EditMap;

	if (map == nullptr)
		return;

	::FableMod::STB::Level^ level = map->STBLevel;

	if (level == nullptr)
		return;
	
	FableMod::Gfx::Device* pDevice = FableMod::Gfx::Manager::GetDevice();
	
	for (int i = 0; i < level->BlockCount; ++i)
	{
		::FableMod::STB::HeightBlock^ block = level->GetBlock(i);
			
		D3DXVECTOR3 posMin(
			block->Min.x,
			block->Min.y,
			block->Min.z);

		D3DXVECTOR3 posMax(
			block->Max.x,
			block->Max.y,
			block->Max.z);

		pDevice->DrawBox(
			posMin, 
			posMax, 
			D3DCOLOR_ARGB(255, 255, 0, 255));

		D3DXVECTOR3 extent(0.05f, 0.05f, 0.05f);

		/*
		for (int v = 0; v < block->VertexCount; ++v)
		{
			::FableMod::CLRCore::Vec3 vec(block->GetVertex(v).Position);

			D3DXVECTOR3 vertex(vec.x, vec.y, vec.z);

			pDevice->DrawBox(
				vertex-extent, 
				vertex+extent, 
				D3DCOLOR_ARGB(255, 255, 128, 64));
		}
		*/
	}

}

void GfxThingView::STBRenderDebugFlora()
{
	ThingMap^ map = EditMap;

	if (map == nullptr)
		return;

	::FableMod::STB::Level^ level = map->STBLevel;

	if (level == nullptr)
		return;

	STBRenderFloraNode(level->RootFloraNode);
}

static const D3DCOLOR FLORA_COLORS[] =
{
	D3DCOLOR_ARGB(255, 255, 0, 0),
	D3DCOLOR_ARGB(255, 0, 255, 0),
	D3DCOLOR_ARGB(255, 0, 0, 255),
	D3DCOLOR_ARGB(255, 255, 255, 0),
	D3DCOLOR_ARGB(255, 255, 0, 255),
};

void GfxThingView::STBRenderFloraNode(::FableMod::STB::FloraNode^ node)
{
	ThingMap^ map = EditMap;

	if (map == nullptr)
		return;

	::FableMod::STB::Level^ level = map->STBLevel;

	D3DXVECTOR3 origin(
		level->MapOrigin.x,
		level->MapOrigin.y,
		0.0f);

	D3DXVECTOR3 posMin(
		node->CellX, 
		node->CellY, 
		node->Position.z-1.0f-0.5f);
	
	D3DXVECTOR3 posMax(
		node->CellX+node->CellWidth, 
		node->CellY+node->CellHeight, 
		node->Position.z-1.0f+0.5f);

	FableMod::Gfx::Device* pDevice = FableMod::Gfx::Manager::GetDevice();

	pDevice->DrawBox(
		origin+posMin, 
		origin+posMax, 
		FLORA_COLORS[node->Depth]);

	for (int i = 0; i < node->ChildCount; ++i)
	{
		STBRenderFloraNode(node->GetChild(i));
	}

	for (int i = 0; i < node->PointCount; ++i)
	{
		::FableMod::STB::FloraPoint fp = node->GetPoint(i);

		D3DXVECTOR3 pos(
			fp.Position.x,
			fp.Position.y,
			fp.Position.z-1.0f);
		
		D3DXVECTOR3 extent(0.1f, 0.1f, 0.1f);

		pDevice->DrawBox(
			pos-extent, 
			pos+extent, 
			D3DCOLOR_ARGB(255, 64, 128, 255));
	}
}

void GfxThingView::CameraHeartbeat()
{
	GfxView::CameraHeartbeat();

	if (Mode == EditorMode::Player)
	{
		D3DXVECTOR3 vCam(m_pCamera->GetPosition());
		D3DXVECTOR3 vDir(
			m_pCamera->GetInterest()-m_pCamera->GetPosition());

		FLOAT fHeight;

		if (TerrainManager::GetInstance()->GetHeight(
			D3DXVECTOR2(vCam.x, vCam.y), &fHeight))
		{
			vCam.z = fHeight+m_fPlayerHeight;

			m_pCamera->SetPosition(vCam);
			m_pCamera->SetInterest(vCam+vDir);
		}
	}
}

static Spatial* GetIDObject(Spatial* pSpatial)
{
	if (pSpatial->ID() != 0)
		return pSpatial;

	if (pSpatial->Parent())
		return GetIDObject(pSpatial->Parent());

	return 0;
}

FableMod::Gfx::Mesh* GfxThingView::GetMeshAtMouse()
{
	D3DXVECTOR3 vO,vD;
	
	if (GetCamera()->ScreenToRay(*m_pvMouse, &vO, &vD))
	{
		FableMod::Gfx::PickData Data;

		GetRoot()->Pick(vO, vD, Data);

		Terrain* pTerrain = dynamic_cast<Terrain*>(Data.pMesh);

		if (pTerrain)
			return 0;
		
		return Data.pMesh;
	}

	return 0;
}

FableMod::Gfx::Mesh* GfxThingView::GetArrowAtMouse()
{
	D3DXVECTOR3 vO,vD;
	
	if (GetCamera()->ScreenToRay(*m_pvMouse, &vO, &vD))
	{
		FableMod::Gfx::PickData Data;

		GetAxes()->Pick(vO, vD, Data);

		return Data.pMesh;
	}

	return 0;
}


void GfxThingView::UpdateSelectedThings()
{
	for (int i = 0; i < m_pSelection->GetNumObjects(); ++i)
	{
		FableMod::TNG::Thing^ t = 
			ThingController->FindThing(
				m_pSelection->GetObjectAt(i)->ID());

		((GfxThingInterface^)t->Interface)->UpdateThing();
	}
}

void GfxThingView::StopMove()
{
	if (m_bMoving)
	{
		m_bMoving = false;

		UpdateSelectedThings();
	
		if (m_pArrow)
		{
			m_pArrow->RenderFlags() &= ~MRF_HIGHLIGHT;
			m_pArrow = 0;
		}
	}
}

void GfxThingView::StopRotation()
{
	if (m_bRotating)
	{
		m_pSelection->StopRotation();

		m_bRotating = false;

		UpdateSelectedThings();
	
		if (m_pArrow)
		{
			m_pArrow->RenderFlags() &= ~MRF_HIGHLIGHT;
			m_pArrow = 0;
		}
	}
}

float GfxThingView::GetPlaneDistance(
	const D3DXVECTOR3* pvOrigin,
	const D3DXVECTOR3* pvDirection,
	Plane* pPlane)
{
	float fDenom = D3DXVec3Dot(&pPlane->Normal(),pvDirection);

	if (fabs(fDenom) > 0.0001f)
	{
		return (pPlane->Constant()-
			D3DXVec3Dot(&pPlane->Normal(),pvOrigin))/fDenom;
	}

	return FLT_MAX-1;
}

void GfxThingView::UpdateMovePoint()
{
	D3DXVECTOR3 vO,vD;

	if (GetCamera()->ScreenToRay(*m_pvMouse, &vO, &vD))
	{
		float fDist = GetPlaneDistance(&vO, &vD, m_pPlane);
		
		if (fabs(fDist-FLT_MAX) < 2)
			return;

		*m_pvMovePoint = vO+vD*fDist;

		if (m_pArrow == (*m_pXArrowMesh))
		{
			m_pvMovePoint->y = GetAxes()->GetWorldTranslation().y;
			m_pvMovePoint->z = GetAxes()->GetWorldTranslation().z;
		}
		else if (m_pArrow == (*m_pYArrowMesh))
		{
			m_pvMovePoint->x = GetAxes()->GetWorldTranslation().x;
			m_pvMovePoint->z = GetAxes()->GetWorldTranslation().z;
		}
		else if (m_pArrow == (*m_pZArrowMesh))
		{
			m_pvMovePoint->x = GetAxes()->GetWorldTranslation().x;
			m_pvMovePoint->y = GetAxes()->GetWorldTranslation().y;
		}
	}
}

void GfxThingView::SelectDuplicates()
{
	// Called after we copy a bunch of objects.

	Spatial** pTemp = new Spatial*[m_pSelection->GetNumObjects()];

	int iCount = m_pSelection->GetNumObjects();
	
	for (int i = 0; i < iCount; ++i)
	{
		FableMod::TNG::Thing^ t = ThingController->FindThing(
			m_pSelection->GetObjectAt(i)->ID());

		GfxThingInterface^ gfx = (GfxThingInterface^)t->Interface;

		FableMod::TNG::Thing^ t2 = (FableMod::TNG::Thing^)t->Duplicate();

		GfxThingInterface^ gfx2 = gcnew GfxThingInterface(gfx->Map, t2);

		if (!gfx2->Create(ThingController))
		{
			// Failed.
			delete t2;
			delete gfx2;
			continue;
		}

		pTemp[i] = gfx2->GetNode();

		gfx->Highlight = false;
		gfx2->Highlight = true;

		gfx2->Update();
	}

	// Select the new objects.

	m_pSelection->Clear();

	for (int i = 0; i < iCount; ++i)
	{
		if (pTemp[i])
			m_pSelection->Add(pTemp[i]);
	}

	delete[] pTemp;

	CallThingSelected();
}

bool GfxThingView::CheckLockedInPlace(bool begin)
{
	for (int i = 0; i < m_pSelection->GetNumObjects(); ++i)
	{
		FableMod::TNG::Thing^ t = 
			ThingController->FindThing(
				m_pSelection->GetObjectAt(i)->ID());

		if (((GfxThingInterface^)t->Interface)->LockedInPlace)
		{
			m_pSelection->EnableAt(i, !begin);
		}
	}

	return m_pSelection->HasEnabled();
}

void GfxThingView::SetSelectionHighlight(bool on)
{
	for (int i = 0; i < m_pSelection->GetNumObjects(); ++i)
	{
		FableMod::TNG::Thing^ t = ThingController->FindThing(
			m_pSelection->GetObjectAt(i)->ID());

		if (t)
			((GfxThingInterface^)t->Interface)->Highlight = on;
	}
}

void GfxThingView::ClearSelection()
{
	// Remove the highlight effects from the objects.
	SetSelectionHighlight(false);
	
	m_pSelection->Clear();
	
	// Hide the axes.
	GetAxes()->Flags() |= FLAGS_FORCE_CULLING;

	ThingSelected(nullptr);
}

array<FableMod::TNG::Thing^>^ GfxThingView::GetSelectedThings()
{
	int iCount = m_pSelection->GetNumObjects();

	array<FableMod::TNG::Thing^>^ things = 
		gcnew array<FableMod::TNG::Thing^>(iCount);

	for (int i = 0; i < iCount; ++i)
	{
		things[i] = ThingController->FindThing(
			m_pSelection->GetObjectAt(i)->ID());
	}

	return things;
}

void GfxThingView::CallThingSelected()
{
	if (m_pSelection->GetNumObjects() > 0)
	{
		// Delegate.
		ThingSelected(GetSelectedThings());
	}
}

void GfxThingView::SelectThing(bool add)
{
	Mesh* pMesh = GetMeshAtMouse();
	
	if (pMesh)
	{
		// Hit a mesh.

		Spatial* pObject = GetIDObject(pMesh);

		if (pObject)
		{
			// It's a thing, well, should be.

			FableMod::TNG::Thing^ thing = 
				ThingController->FindThing(pObject->ID());
			
			if (thing)
			{
				if (add && !m_pSelection->IsEmpty())
				{
					// Add to selection.

					m_pSelection->Add(pObject);

					SetSelectionHighlight(true);
					
					SetupAxes();
				}
				/*
				else if (m_BtnCfg->IsDown("ThingAddSelect")&& m_pSelection->HasObject(pObject))
				{
					// Remove from selection.

					((GfxThingInterface^)
						thing->Interface)->Highlight = false;

					m_pSelection->Remove(pObject);

					SetupAxes();
				}
				*/
				else
				{
					// New selection.

					ClearSelection();
					
					m_pSelection->Add(pObject);
				
					SetSelectionHighlight(true);
					
					SetupAxes();
				}

				// Do selection event.
				CallThingSelected();
			}
		}
	}
}

bool GfxThingView::ButtonDownNormal(String^ button)
{
	if (m_pArrow)
	{
		if (CheckLockedInPlace(true))
		{
			if (button == "ThingMove" || button == "ThingCopyMove")
			{
				if (button == "ThingCopyMove")
					SelectDuplicates();						
				
				SetupPlane(m_pArrow);

				m_bMoving = true;

				UpdateMovePoint();

				*m_pvMoveDiff = 
					GetAxes()->GetWorldTranslation()-*m_pvMovePoint;
			}
			else if (button == "ThingRotate" || button == "ThingCopyRotate")
			{
				if (button == "ThingCopyRotate")
					SelectDuplicates();
					
				m_pSelection->StartRotation();

				m_bRotating = true;
	
				*m_pvMousePoint = *m_pvMouse;
			}
		}

		return true;
	}
	else if (button == "ThingSelect")
	{
		SelectThing(false);
	}	
	else if (button == "ThingAddSelect")
	{
		SelectThing(true);
	}
	else if (button == "ThingFocus" && !m_pSelection->IsEmpty())
	{
		Mesh* pMesh = GetMeshAtMouse();
			
		if (pMesh)
		{
			Spatial* pObject = GetIDObject(pMesh);

			if (pObject)
			{
				/*
				if (ctrlDown && !shiftDown)
				{
					ApplyOwnership(
						ThingController->FindThing(pObject->ID()));
				}
				else if (ctrlDown && shiftDown)
				{
					RemoveOwnership(
						ThingController->FindThing(pObject->ID()));

					RemoveSelectionHighlight();

					m_pSelection->Remove(pObject);

					AddSelectionHighlight();

					SetupAxes();
				}
				else
				{
				*/
				if (m_pSelection->HasObject(pObject))
				{
					CameraFocus(pObject, CameraDirection::Front);
				}
				//}
			}
		}
	}

	return false;
}

void GfxThingView::OnMouseUp(System::Windows::Forms::MouseEventArgs^  e)
{
	GfxView::OnMouseUp(e);
}

bool GfxThingView::MouseMoveNormal(System::Windows::Forms::MouseEventArgs^  e)
{
	bool ctrlDown = 
		(Control::ModifierKeys & Keys::Control) == Keys::Control;
	bool altDown = 
		(Control::ModifierKeys & Keys::Alt) == Keys::Alt;
	bool shiftDown =
		(Control::ModifierKeys & Keys::Shift) == Keys::Shift;
		
	if (Mode == EditorMode::Normal)
	{
		if (m_bRotating)
		{
			D3DXVECTOR3 vRot(0, 0, 0);

			float fDelta = D3DXToRadian(m_pvMouse->y-m_pvMousePoint->y);

			if (m_pArrow == (*m_pXArrowMesh))
				vRot.x = fDelta;
			else if (m_pArrow == (*m_pYArrowMesh))
				vRot.y = fDelta;
			else if (m_pArrow == (*m_pZArrowMesh))
				vRot.z = fDelta;
			
			D3DXQUATERNION quatRot;

			D3DXQuaternionRotationYawPitchRoll(
					&quatRot, vRot.y, vRot.x, vRot.z);

			D3DXMATRIX matRot;

			D3DXMatrixRotationQuaternion(&matRot, &quatRot);

			m_pSelection->AddRotation(matRot);

			*m_pvMousePoint = *m_pvMouse;
		}
		else if (m_bMoving)
		{
			UpdateMovePoint();

			D3DXVECTOR3 vPoint(*m_pvMovePoint+*m_pvMoveDiff);

			if (m_BtnCfg->IsDown("ThingCollisions"))
			{	
				// Check surfaces...

				for (int i = 0; i < m_pSelection->GetNumObjects(); ++i)
				{
					m_pSelection->GetObjectAt(i)->Flags() |=
						FLAGS_EXCLUDE_PICK;
				}
				
				FableMod::Gfx::PickData Data;

				const D3DXVECTOR3 vCenter(m_pSelection->GetCenter());
				const D3DXVECTOR3 vTrans(GetAxes()->GetWorldTranslation());

				if (m_pArrow == (*m_pZArrowMesh) && vTrans.z > vPoint.z)
				{
					// Going down z...

					const float fH = m_pSelection->GetHeight()/2;

					const D3DXVECTOR3 vBottom(
						vCenter+D3DXVECTOR3(0, 0, -fH));
			
					GetRoot()->Pick(vCenter, D3DXVECTOR3(0, 0, -1), Data);

					if (Data.pMesh && Data.vPoint.z >= vBottom.z)
						vPoint.z = Data.vPoint.z+fH;
				}
				else if (m_pArrow == (*m_pXArrowMesh))
				{
					const float fW = m_pSelection->GetWidth()/2;
					
					if (vTrans.x > vPoint.x)
					{
						// Going right.

						const D3DXVECTOR3 vRight(
							vCenter+D3DXVECTOR3(-fW, 0, 0));
					
						GetRoot()->Pick(vCenter, D3DXVECTOR3(-1, 0, 0), Data);

						if (Data.pMesh && Data.vPoint.x >= vRight.x)
							vPoint.x = Data.vPoint.x+fW;
					}
					else
					{
						const D3DXVECTOR3 vLeft(
							vCenter+D3DXVECTOR3(fW, 0, 0));
					
						GetRoot()->Pick(vCenter, D3DXVECTOR3(1, 0, 0), Data);

						if (Data.pMesh && Data.vPoint.x <= vLeft.x)
							vPoint.x = Data.vPoint.x-fW;
					}
				}
				else if (m_pArrow == (*m_pYArrowMesh))
				{
					const float fD = m_pSelection->GetDepth()/2;
					
					if (vTrans.y > vPoint.y)
					{
						const D3DXVECTOR3 vTmp(
							vCenter+D3DXVECTOR3(0, -fD, 0));
					
						GetRoot()->Pick(vCenter, D3DXVECTOR3(0, -1, 0), Data);

						if (Data.pMesh && Data.vPoint.y >= vTmp.y)
							vPoint.y = Data.vPoint.y+fD;
					}
					else
					{
						const D3DXVECTOR3 vTmp(
							vCenter+D3DXVECTOR3(0, fD, 0));
					
						GetRoot()->Pick(vCenter, D3DXVECTOR3(0, 1, 0), Data);

						if (Data.pMesh && Data.vPoint.y <= vTmp.y)
							vPoint.y = Data.vPoint.y-fD;
					}
				}

				for (int i = 0; i < m_pSelection->GetNumObjects(); ++i)
				{
					m_pSelection->GetObjectAt(i)->Flags() &=
						~FLAGS_EXCLUDE_PICK;
				}
			}
		
			GetAxes()->SetWorldTranslation(vPoint);
			m_pSelection->SetCenter(vPoint);
		}
		else if (AxesOn())
		{
			// Reset arrow flags.
			(*m_pXArrowMesh)->RenderFlags() &= ~MRF_HIGHLIGHT;
			(*m_pYArrowMesh)->RenderFlags() &= ~MRF_HIGHLIGHT;
			(*m_pZArrowMesh)->RenderFlags() &= ~MRF_HIGHLIGHT;

			m_pArrow = GetArrowAtMouse();

			if (m_pArrow)
			{
				D3DXVECTOR3 vCamDir(
					GetCamera()->GetPosition()-
					GetAxes()->GetWorldTranslation());

				D3DXVec3Normalize(&vCamDir, &vCamDir);
					
				if (m_pArrow == (*m_pXArrowMesh) && 
					fabs(vCamDir.x) < 0.95)
				{
					m_pArrow->RenderFlags() |= MRF_HIGHLIGHT;
				}
				else if (m_pArrow == (*m_pYArrowMesh) && 
						fabs(vCamDir.y) < 0.95)
				{
					m_pArrow->RenderFlags() |= MRF_HIGHLIGHT;
				}
				else if (m_pArrow == (*m_pZArrowMesh) && 
						fabs(vCamDir.z) < 0.95)
				{
					m_pArrow->RenderFlags() |= MRF_HIGHLIGHT;
				}
			}
		}
	}

	return false;
}

void GfxThingView::OnMouseMove(System::Windows::Forms::MouseEventArgs^  e)
{
	if (!IsReady())
		return;

	bool shiftDown =
		(Control::ModifierKeys & Keys::Shift) == Keys::Shift;
	
	UpdateMouse(e->X, e->Y);

	if (Mode == EditorMode::Normal)
	{
		if (MouseMoveNormal(e))
			return;
	}
	else if (Mode == EditorMode::Player)
	{	
		m_pCameraController->OnMouseMove(*m_pvMouse);
	}
	else if (Mode == EditorMode::Navigation || Mode == EditorMode::Walk)
	{	
		if (e->Button == ::MouseButtons::Right)
			m_iDrawState = 2;
		else if (e->Button == ::MouseButtons::Left)
			m_iDrawState = 1;
		else
			m_iDrawState = -1;
	}

	GfxView::OnMouseMove(e);
}

void GfxThingView::OnMouseDown(System::Windows::Forms::MouseEventArgs^  e)
{
	GfxView::OnMouseDown(e);
}

void GfxThingView::RemoveOwnership(FableMod::TNG::Thing^ parent)
{
	array<FableMod::TNG::Thing^>^ things = GetSelectedThings();

	int iRemoved = 0;
	
	for (int i = 0; i < things->Length; ++i)
	{
		if (things[i] == parent)
			continue;

		// Get the CTC, this will apply it, if it's not there.
		FableMod::TNG::CTCBlock^ block = 
			things[i]->ApplyCTC(nullptr, "CTCOwnedEntity");

		if (block)
		{
			String^ uid = block->Variables["OwnerUID"]->StringValue;

			if (uid == parent->UID)
			{
				 block->Variables["OwnerUID"]->Value = "0";
				 ++iRemoved;
			}
		}
	}

	if (iRemoved > 0)
	{
		MessageBox::Show(
			String::Format("Removed {0}:{1} as the owner of {2} thing(s).",
				parent->DefinitionType, 
				parent->UID,
				iRemoved),
			"Ownership");
	}
}

void GfxThingView::ApplyOwnership(FableMod::TNG::Thing^ parent)
{
	array<FableMod::TNG::Thing^>^ things = GetSelectedThings();

	int iAdded = 0;
	
	for (int i = 0; i < things->Length; ++i)
	{
		if (things[i] == parent)
			continue;

		// Get the CTC, this will apply it, if it's not there.
		FableMod::TNG::CTCBlock^ block = 
			things[i]->ApplyCTC(nullptr, "CTCOwnedEntity");

		if (block)
		{
			String^ uid = block->Variables["OwnerUID"]->StringValue;

			if (uid != parent->UID)
			{
				 block->Variables["OwnerUID"]->Value = parent->UID;
				 ++iAdded;
			}
		}
	}

	if (iAdded > 0)
	{
		MessageBox::Show(
			String::Format("Made {0}:{1} as the owner of {2} thing(s).",
				parent->DefinitionType, 
				parent->UID,
				iAdded),
			"Ownership");
	}
}

void GfxThingView::OnMouseDoubleClick(System::Windows::Forms::MouseEventArgs^  e)
{
	GfxView::OnMouseDoubleClick(e);
}

void GfxThingView::OnKeyUp(System::Windows::Forms::KeyEventArgs^ e)
{
	if (e->KeyCode == ::Keys::Delete && !m_pSelection->IsEmpty())
	{
		DialogResult res = MessageBox::Show(
			"Are you sure?", 
			"Delete selection",
			MessageBoxButtons::YesNoCancel,
			MessageBoxIcon::Warning);
		
		if (res != DialogResult::Yes)
			return;

        m_bRenderBlock = true;
        
		for (int i = 0; i < m_pSelection->GetNumObjects(); ++i)
		{
			unsigned int uiID = m_pSelection->GetObjectAt(i)->ID();

			FableMod::TNG::Thing^ thing = ThingController->FindThing(uiID);

			if (!thing)
				continue;
			
			// Check for teleporter special case...

			FableMod::TNG::CTCBlock^ block = 
				thing->CTCs["CTCActionUseScriptedHook"];

			if (block)
			{
				FableMod::TNG::Variable^ var =
					block->Variables["EntranceConnectedToUID"];

				FableMod::TNG::Thing^ marker = 
					ThingController->FindThingUID(var->StringValue);
			
				if (marker)
				{
					if (MessageBox::Show(
						"Delete the teleporter marker?",
						"Delete Object",
						MessageBoxButtons::YesNo) == DialogResult::Yes)
					{
						ThingController->RemoveThing(marker);
					}
				}
			}

			if (ThingController->RemoveThing(uiID))
			{
				//
			}
		}

		ClearSelection();
        
        m_bRenderBlock = false;
	}
	else if (e->KeyCode == Keys::Space)
	{
		if (Mode == EditorMode::Normal)
			Mode = EditorMode::Player;
		else if (Mode == EditorMode::Player)
			Mode = EditorMode::Normal;
	}

	GfxView::OnKeyUp(e);
}

void GfxThingView::BtnCfg_OnButtonDown(
	System::String^ button, System::Windows::Forms::MouseEventArgs^ e)
{
	if (Mode == EditorMode::Normal)
	{
		ButtonDownNormal(button);
	}
	else if (Mode == EditorMode::Walk)
	{
		if (button == "WalkDraw")
			m_MapColor = GFXI_COLOR_WALK;
		else if (button == "WalkErase")
			m_MapColor = GFXI_COLOR_EMPTY;
	}

	GfxView::BtnCfg_OnButtonDown(button, e);
}

void GfxThingView::BtnCfg_OnButtonUp(
	System::String^ button, System::Windows::Forms::MouseEventArgs^ e)
{
	if (button->StartsWith("Thing"))
	{
		if (Mode == EditorMode::Create && button == "ThingCreate")
		{
			D3DXVECTOR3 vO,vD;
			
			if (GetCamera()->ScreenToRay(*m_pvMouse, &vO, &vD))
			{
				FableMod::Gfx::PickData Data;

				GetRoot()->Pick(vO, vD, Data);

				if (Data.pMesh)
				{
					// Event!
					ThingCreated(
						Data.vPoint.x,
						Data.vPoint.y,
						Data.vPoint.z,
						Data.vNormal.x,
						Data.vNormal.y,
						Data.vNormal.z);
				}
			}
		}
		else if (Mode == EditorMode::Pick && button == "ThingCreate")
		{
			Mesh* pMesh = GetMeshAtMouse();
			
			if (pMesh)
			{
				Spatial* pObject = GetIDObject(pMesh);

				if (pObject)
				{
					FableMod::TNG::Thing^ thing = 
						ThingController->FindThing(pObject->ID());
			
					if (thing)
					{
						ThingPicked(thing);
					}
				}
			}
		}
		else
		{
			StopMove();
			StopRotation();

			CheckLockedInPlace(false);
		}
	}
	else if (Mode == EditorMode::Walk)
	{
		m_MapColor = GFXI_COLOR_NONE;
	}
	
	GfxView::BtnCfg_OnButtonUp(button, e);
}

}
}
}