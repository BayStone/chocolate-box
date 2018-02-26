#include "stdafx.h"
#include "ImpostorNode.h"
#include "Camera.h"
#include "Device.h"
#include "Manager.h"
#include "QuadModel.h"
#include "Settings.h"
#include "LogFile.h"
#include "Helpers.h"

namespace FableMod
{
namespace Gfx
{

static void AdjustedProjectionMatrix(
	D3DXMATRIX& mat, 
	FLOAT fFOV, 
	FLOAT fAspect,
    FLOAT fNearPlane, 
	FLOAT fFarPlane,
    FLOAT fPixDx, 
	FLOAT fPixDy, 
	FLOAT fVPWidth, 
    FLOAT fVPHeight)
{
	if (fabs(fFarPlane-fNearPlane) < 0.01f)
        return;
    
    if (fabs(sin(fFOV/2)) < 0.01f)
        return;

    float h = 1.0f*(cosf(fFOV/2)/sinf(fFOV/2));

    float fTop    = fNearPlane/h;
    float fBottom = -fTop;
    float fRight  = fTop*fAspect;
    float fLeft   = -fRight;

    float fXWSize = fRight-fLeft;
    float fYWSize = fTop-fBottom;

    float fDx = -(fPixDx*fXWSize/fVPWidth);
    float fDy = -(fPixDy*fYWSize/fVPHeight);
 
	float Q = fFarPlane/(fFarPlane-fNearPlane);

	fLeft   += fDx;
	fRight  += fDx;
	fTop    += fDy;
	fBottom += fDy;

    ZeroMemory(&mat, sizeof(D3DMATRIX));
    mat._11 = (2.0f*fNearPlane)/(fRight-fLeft);
    mat._22 = (2.0f*fNearPlane)/(fTop-fBottom);
    mat._31 = (fRight+fLeft)/(fRight-fLeft);
    mat._32 = (fTop+fBottom)/(fTop-fBottom);
    mat._33 = -Q;
    mat._34 = -1.0f;
    mat._43 = -Q*fNearPlane;
}

IDirect3DSurface9* ImpostorNode::sm_pDepthStencil = 0;
FLOAT ImpostorNode::sm_fSizeRatio = -1;
unsigned char ImpostorNode::sm_ucUpdateFrequency = 0;
D3DXMATRIX ImpostorNode::sm_matProj;
bool ImpostorNode::sm_bEnabled = false;
int ImpostorNode::sm_iTextureSize = 0;

ImpostorNode::ImpostorNode() :
m_spTexture(0),
m_spMesh(0),
m_ucUpdate(1)
{
	if (sm_iTextureSize == 0)
	{
		sm_fSizeRatio = GetSettings()->GetFloat(
			_T("Impostor"), _T("Divider"), 8);

		sm_ucUpdateFrequency = (unsigned char)
			GetSettings()->GetInt(
			_T("Impostor"), _T("UpdateFrequency"), 4);

		sm_bEnabled = GetSettings()->GetInt(
			_T("Impostor"), _T("Enabled"), 1) != 0;

		sm_iTextureSize = GetSettings()->GetInt(
			_T("Impostor"), _T("TextureSize"), 128);
	}
}

ImpostorNode::~ImpostorNode()
{
	Dispose();
}

void ImpostorNode::Dispose()
{
	Node::Dispose();

	GFX_SAFE_RELEASE(sm_pDepthStencil);
}

void ImpostorNode::Update(bool bInit)
{
	Node::Update(bInit);
}

Spatial* ImpostorNode::Copy() const
{
	ImpostorNode* pNode = new ImpostorNode();

	CopyProperties(pNode);

	for (int i = 0; i < m_Children.GetCount(); ++i)
	{
		pNode->AttachChild(m_Children[i]->Copy());
	}

	return pNode;
}

void ImpostorNode::OnGetRenderList(Camera* pCamera)
{
	Device* pDevice = Manager::GetDevice();
	IDirect3DDevice9* pD3DDevice = pDevice->GetD3DDevice();
	
	D3DXVECTOR3 vDist(pCamera->GetPosition()-GetWorldTranslation());
	
	FLOAT fSize = pCamera->GetProjectedSize(
		D3DXVec3Length(&vDist), GetWorldBound()->Radius());

	if (sm_bEnabled &&
		(Flags () & FLAGS_DISABLE_SIMPLIFICATION) == 0 &&
		fSize <= (pDevice->GetWidth()/sm_fSizeRatio))
	{
		if (++m_ucUpdate == sm_ucUpdateFrequency)
		{
			// Update the texture.
			
			m_ucUpdate = 0;

			if (!sm_pDepthStencil)
			{
				FM_LOG0(_T("Created impostor depth/stencil surface.\n"));

				GFX_TRYD3D(pD3DDevice->CreateDepthStencilSurface(
					sm_iTextureSize,
					sm_iTextureSize,
					D3DFMT_D24S8,
					D3DMULTISAMPLE_NONE,
					0,
					FALSE,
					&sm_pDepthStencil,
					0));
				
				AdjustedProjectionMatrix(
					sm_matProj,
					D3DX_PI/4, 
					1.0f, 
					1.0f, 
					100000.0f, 
					0.0f, 
					0.0f, 
					sm_iTextureSize, 
					sm_iTextureSize);
			}

			if (!m_spMesh)
			{
				// Create the impostor mesh.

				FM_LOG0(_T("Created an impostor mesh.\n"));

				m_spMesh = new Mesh();

				TexturePtr spTexture = new Texture();
				
				spTexture->Create(
					sm_iTextureSize, 
					sm_iTextureSize, 1, true, true);

				QuadModelPtr spModel = new QuadModel(spTexture, 1);

				m_spMesh->SetModel(spModel);
			}
			
			if (!m_spTexture)
			{
				// Set the target texture.

				m_spTexture = 
					((QuadModel*)m_spMesh->GetModel())->GetTexture();
			
				sm_pDepthStencil->AddRef();
			}

			RenderToTexture(pCamera);

			// Set up the impostor.
			
			m_spMesh->SetTranslation(GetWorldBound()->Center());

			D3DXMATRIX matInverse(pCamera->GetViewInverseMatrix());

			matInverse._41 = 0.0f;
			matInverse._42 = 0.0f;
			matInverse._43 = 0.0f;

			m_spMesh->SetRotation(matInverse);

			m_spMesh->SetScale(GetWorldBound()->Radius()*2.0f);

			m_spMesh->Update();
		}

		if (m_spTexture && m_spMesh)
		{
			pCamera->AddRenderObject(m_spMesh);
			return;
		}
	}
	else
	{
		if (m_spTexture)
		{
			// We have used an importor for this object, so
			// now release the texture.

			FM_LOG0(_T("Impostor off.\n"));

			m_spTexture = 0;
			
			// Release my lock on the buffer.
			if (sm_pDepthStencil)
				sm_pDepthStencil->Release();

			m_ucUpdate = 0;
		}
	}

	Node::OnGetRenderList(pCamera);
}

static void RenderTree(Spatial* pSpatial)
{
	Node* pNode = dynamic_cast<Node*>(pSpatial);

	if (pNode)
	{
		for (int i = 0; i < pNode->NumChildren(); ++i)
		{
			RenderTree(pNode->GetChildAt(i));
		}
	}
	else
	{
		Mesh* pMesh = dynamic_cast<Mesh*>(pSpatial);

		if (pMesh)
		{
			pMesh->Render(0);
		}
	}
}

void ImpostorNode::RenderToTexture(Camera* pCamera)
{
	static const FLOAT TAN_ANGLE = tan((D3DX_PI/4.0f)/2.0f);
	static const D3DCOLOR CLEAR_COLOR = D3DCOLOR_ARGB(0, 0, 0, 0);

	Device* pDevice = Manager::GetDevice();
	IDirect3DDevice9* pD3DDevice = pDevice->GetD3DDevice();
	
	bool bInScene = pDevice->InScene();

	if (bInScene)	
	{
		// Stop it.	
		GFX_TRYD3D(pD3DDevice->EndScene());
	}

	D3DXMATRIX matViewOld,matProjOld;
	D3DVIEWPORT9 ViewPortOld;
	IDirect3DSurface9* pRenderTargetOld;
	IDirect3DSurface9* pDepthBufferOld;
	D3DXMATRIX matView;
	D3DVIEWPORT9 Viewport;
	
	pD3DDevice->GetTransform(D3DTS_VIEW,       &matViewOld);
	pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProjOld);
	pD3DDevice->GetViewport(&ViewPortOld);

	pD3DDevice->GetRenderTarget(0,&pRenderTargetOld);
	pD3DDevice->GetDepthStencilSurface(&pDepthBufferOld);
	
	// New render target.

	IDirect3DSurface9* pRenderTarget = 0;

	GFX_TRYD3D(m_spTexture->GetD3DTexture()->GetSurfaceLevel(
		0,&pRenderTarget));

	GFX_TRYD3D(pD3DDevice->SetRenderTarget(0,pRenderTarget));
	GFX_TRYD3D(pD3DDevice->SetDepthStencilSurface(sm_pDepthStencil));

	Viewport.X = 0;
	Viewport.Y = 0;
	Viewport.Width = sm_iTextureSize;
	Viewport.Height = sm_iTextureSize;
	Viewport.MinZ = 0.0f;
	Viewport.MaxZ = 1.0f;

	pD3DDevice->SetViewport(&Viewport);
	
	GFX_TRYD3D(pD3DDevice->Clear(				
		0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		CLEAR_COLOR, 1.0f, 0L));

	// Start the scene.

	D3DXVECTOR3 vCam(pCamera->GetPosition());

	D3DXVECTOR3 vCenter(GetWorldBound()->Center());

	const FLOAT fRadius = GetWorldBound()->Radius();
		
	D3DXVECTOR3 vDir(vCam-vCenter);

	D3DXVec3Normalize(&vDir, &vDir);
	
	vDir *= (fRadius/TAN_ANGLE)*1.1f;

	D3DXMatrixLookAtRH(
		&matView,
		&(vCenter+vDir),
		&vCenter,
		&Camera::CAM_BASE_UP);

	GFX_TRYD3D(pD3DDevice->BeginScene());

	pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
	pD3DDevice->SetTransform(D3DTS_PROJECTION, &sm_matProj);

	RenderTree(this);

	GFX_TRYD3D(pD3DDevice->EndScene());

	GFX_TRYD3D(pD3DDevice->SetRenderTarget(0,pRenderTargetOld));
	GFX_TRYD3D(pD3DDevice->SetDepthStencilSurface(pDepthBufferOld));
	
	pRenderTargetOld->Release();
	pDepthBufferOld->Release();
	pRenderTarget->Release();

	// Restore old stuff.

	pD3DDevice->SetViewport(&ViewPortOld);
	pD3DDevice->SetTransform(D3DTS_VIEW, &matViewOld);
	pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProjOld);

	if (bInScene)
	{
		// Restore.
		GFX_TRYD3D(pD3DDevice->BeginScene());
	}
}

}
}