#include "stdafx.h"
#include "Mesh.h"
#include "Camera.h"
#include "Manager.h"
#include "Shaders.h"
#include "Helpers.h"

namespace FableMod
{
namespace Gfx
{

Mesh::Mesh() :
m_spModel(0),
m_pModelBound(new BoundSphere()),
m_pBox(new AxisBox()),
m_uiRenderFlags(MRF_DISTANCE_TO_BOX),
m_bUpdateBox(true),
m_fAlpha(1.0f)
{
}

Mesh::~Mesh()
{
	Dispose();

	delete m_pModelBound;
	delete m_pBox;
}

void Mesh::Dispose()
{
	Spatial::Dispose();

	m_spModel = 0;
}

void Mesh::SetAlpha(FLOAT fAlpha)
{
	m_fAlpha = fAlpha;
}

bool Mesh::AlphaRequired() const
{
	if (fabs(m_fAlpha-1.0f) > 0.01f)
		return true;

	if (m_spModel)
		return m_spModel->AlphaRequired();

	return false;
}

void Mesh::RenderWireframeOverlay(Camera* pCamera)
{
	Device* pDevice = Manager::GetDevice();
	IDirect3DDevice9* pD3DDevice = pDevice->GetD3DDevice();
	
	pD3DDevice->SetRenderState(D3DRS_COLORVERTEX, FALSE);
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	D3DMATERIAL9 Material;

	ZeroMemory(&Material, sizeof(Material));

	Material.Diffuse.r = 0.0f;
	Material.Diffuse.g = 0.0f;
	Material.Diffuse.b = 0.0f;
	Material.Diffuse.a = 0.5f;
	Material.Ambient = Material.Diffuse;

	pD3DDevice->SetMaterial(&Material);

	pD3DDevice->SetRenderState(D3DRS_DEPTHBIAS, 16);

	m_spModel->Render(this, pCamera, true);

	pD3DDevice->SetRenderState(D3DRS_DEPTHBIAS, 0);

	pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	pD3DDevice->SetRenderState(D3DRS_COLORVERTEX, TRUE);
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void Mesh::Render(Camera* pCamera)
{
	static const D3DCOLOR BOX_COLOR = D3DCOLOR_ARGB(255,0,0,0);

	Device* pDevice = Manager::GetDevice();
	IDirect3DDevice9* pD3DDevice = pDevice->GetD3DDevice();
		
	if (!pCamera)
	{
		// No camera. Force render.

		pD3DDevice->SetTransform(D3DTS_WORLD, &GetWorldTransform());

		OnRender(pCamera);

		return;
	}

	DWORD dwTestZ = 0;

	if (RenderFlags() & MRF_DISABLE_ZTEST)
	{
		pD3DDevice->GetRenderState(D3DRS_ZENABLE, &dwTestZ);
		pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	}
	
	unsigned int uiRenderFlags = RenderFlags();

	if (RenderFlags() & MRF_DISTANCE_TO_BOX)
	{
		// Change into box when we are about to be clipped.
		if (sqrtf(DistanceToCamera()) > pCamera->GetFrustumFar()*0.75f)
			RenderFlags() |= MRF_AS_BOX;
	}

	if (RenderFlags() & MRF_AS_BOX)
	{
		AxisBox Box;

		if (GetBox(&Box))
			pDevice->DrawBox(Box.GetMin(),Box.GetMax(),BOX_COLOR);
	}
	else if (RenderFlags() & MRF_AS_STONE)
	{
		pD3DDevice->SetTransform(D3DTS_WORLD, &GetWorldTransform());

		pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		
		D3DMATERIAL9 Material;

		ZeroMemory(&Material, sizeof(Material));

		Material.Diffuse.r = 0.3f;
		Material.Diffuse.g = 0.3f;
		Material.Diffuse.b = 0.35f;
		Material.Diffuse.a = m_fAlpha;
		Material.Ambient = Material.Diffuse;

		pD3DDevice->SetMaterial(&Material);

		m_spModel->Render(this, pCamera, true);

		pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		if (RenderFlags() & MRF_WIREFRAME_OVERLAY)
			RenderWireframeOverlay(pCamera);
	}
	else
	{
		if (RenderFlags() & MRF_WIREFRAME)
			pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		pD3DDevice->SetTransform(D3DTS_WORLD, &GetWorldTransform());

		if ((RenderFlags() & MRF_WIREFRAME) == 0)
		{
			if ((RenderFlags() & MRF_HIGHLIGHT) != 0)
			{
				static const float INK_COLOR[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	
				RenderHighlight(pCamera, INK_COLOR);
			}
			else if ((RenderFlags() & MRF_FLORA) != 0)
			{
				static const float INK_COLOR[] = { 1.0f, 0.1f, 0.1f, 0.1f };
	
				RenderHighlight(pCamera, INK_COLOR);
			}
		}

		// Render normal.
		OnRender(pCamera);

		if (RenderFlags() & MRF_WIREFRAME)
			pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		if (RenderFlags() & MRF_WIREFRAME_OVERLAY)
			RenderWireframeOverlay(pCamera);

		if (RenderFlags() & MRF_BOX)
		{
			AxisBox Box;

			if (GetBox(&Box))
				pDevice->DrawBox(Box.GetMin(),Box.GetMax(),BOX_COLOR);
		}

		if (RenderFlags() & MRF_BOUNDSPHERE)
		{
			pDevice->DrawSphere(
				GetWorldBound()->Center(),
				GetWorldBound()->Radius(),
				D3DXCOLOR(D3DCOLOR_ARGB(64,128,128,0)));
		}
	}

	if (RenderFlags() & MRF_DISABLE_ZTEST)
		pD3DDevice->SetRenderState(D3DRS_ZENABLE, dwTestZ);
	
	RenderFlags() = uiRenderFlags;
}

void Mesh::RenderHighlight(Camera* pCamera, const float* afInkColor)
{
	Device* pDevice = Manager::GetDevice();
	IDirect3DDevice9* pD3DDevice = pDevice->GetD3DDevice();
	
	IDirect3DVertexShader9* pInkShader = pDevice->BuildInkShader();

	if (!pInkShader)
	{
		OutputDebugString(_T("Failed to load ink shader.\n"));
		return;
	}

	D3DXMATRIX matView,matProj,matWorld,matT;

	pD3DDevice->GetTransform(D3DTS_WORLD,&matWorld);
	pD3DDevice->GetTransform(D3DTS_VIEW,&matView);
	pD3DDevice->GetTransform(D3DTS_PROJECTION,&matProj);

	D3DXMatrixMultiply(&matView,&matView,&matProj);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matView);
	
	D3DXMatrixTranspose(&matT,&matWorld);

	LoadMatrix4(pD3DDevice,1,&matT);

	pD3DDevice->SetVertexShaderConstantF(11,afInkColor,1);	
		
	FLOAT fLine = pCamera->GetPixelProjection(
		sqrtf(DistanceToCamera()), 4);
	
	const FLOAT afScale[] = 
	{
		fLine/GetWorldScale(),
		fLine/GetWorldScale(),
		fLine/GetWorldScale(),
		0
	};
	
	pD3DDevice->SetVertexShaderConstantF(5,afScale,1);	

#ifdef GFX_LH
	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
#else		
	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
#endif		
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pD3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
	
	pD3DDevice->SetVertexShader(pInkShader);

	m_spModel->Render(this, pCamera, true);

	pD3DDevice->SetVertexShader(0);

	pD3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

#ifdef GFX_LH
	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
#else		
	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
#endif		
	
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void Mesh::OnRender(Camera* pCamera)
{
	m_spModel->Render(this, pCamera, false);
}

void Mesh::OnGetRenderList(Camera* pCamera)
{
	if (Flags() & FLAGS_RENDERABLE)
		pCamera->AddRenderObject(this);
}

void Mesh::UpdateWorldTransform()
{
	Spatial::UpdateWorldTransform();
	m_bUpdateBox = true;
}

void Mesh::UpdateWorldBound()
{
	GetWorldBound()->Copy(m_pModelBound);

	GetWorldBound()->Transform(GetWorldTransform(), GetWorldScale());
}

void Mesh::CopyProperties(Spatial* pObject) const
{
	Spatial::CopyProperties(pObject);
}

Spatial* Mesh::Copy() const
{
	Mesh* pMesh = new Mesh();

	CopyProperties(pMesh);

	pMesh->m_uiRenderFlags = m_uiRenderFlags;

	pMesh->SetModel(m_spModel);

	return pMesh;
}

void Mesh::SetModel(Model* pModel)
{
	m_spModel = 0;
	
	if (pModel)
	{
		m_spModel = pModel;

		m_spModel->ComputeBoundSphere(m_pModelBound);

		m_spModel->ComputeAxisBox(m_pBox, &GetWorldTransform());

		Flags() |= FLAGS_RENDERABLE;
	}
}

bool Mesh::GetBox(AxisBox* pBox)
{
	if (m_bUpdateBox)
	{
		m_bUpdateBox = false;
		m_pBox->Initialize();
		m_spModel->ComputeAxisBox(m_pBox, &GetWorldTransform());
	}

	pBox->Update(m_pBox);

	return true;
}

void Mesh::Pick(
	const D3DXVECTOR3& vOrigin, 
	const D3DXVECTOR3& vDirection,
	PickData& rData)
{
	if ((Flags() & FLAGS_EXCLUDE_PICK) == 0 &&
		(Flags() & FLAGS_IS_VISIBLE) != 0)
	{
		//if (!GetWorldBound()->Inside(vOrigin))
		//{
			if (GetWorldBound()->RayIntersection(vOrigin, vDirection))
			{
				m_spModel->Pick(this, vOrigin, vDirection, rData);
			}
		//}
	}
}

}
}