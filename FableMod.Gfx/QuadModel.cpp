#include "Stdafx.h"
#include "QuadModel.h"
#include "Manager.h"

namespace FableMod
{
namespace Gfx
{

LPDIRECT3DINDEXBUFFER9 QuadModel::sm_pIB = 0;
LPDIRECT3DVERTEXBUFFER9 QuadModel::sm_pVB = 0;
D3DMATERIAL9 QuadModel::sm_Material;

QuadModel::QuadModel(Texture* pTexture, FLOAT fSize)
{
	if (!sm_pVB)
	{
		// First object.

		D3DModel::Create(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1, 4, 2);

		LockVertices();

		const FLOAT fSize2 = fSize/2;

		// TL, TR, BR, BL
		SetVertex(0, D3DXVECTOR3( fSize2, -fSize2, 0));
		SetVertex(1, D3DXVECTOR3(-fSize2, -fSize2, 0));
		SetVertex(2, D3DXVECTOR3(-fSize2,  fSize2, 0));
		SetVertex(3, D3DXVECTOR3( fSize2,  fSize2, 0));
		
		const D3DXVECTOR3 vNormal(0, 0, 1);

		SetNormal(0, vNormal);
		SetNormal(1, vNormal);
		SetNormal(2, vNormal);
		SetNormal(3, vNormal);

		SetTextureCoordinate(0, 0, D3DXVECTOR2(1,1));
		SetTextureCoordinate(0, 1, D3DXVECTOR2(0,1));
		SetTextureCoordinate(0, 2, D3DXVECTOR2(0,0));
		SetTextureCoordinate(0, 3, D3DXVECTOR2(1,0));
		
		UnlockVertices();

		LockFaces();

		SetFace(0, 0, 3, 1);
		SetFace(1, 1, 3, 2);

		UnlockFaces();
		
		m_spTexture = pTexture;

		ZeroMemory(&sm_Material, sizeof(sm_Material));
		
		sm_Material.Diffuse.r = 1.0f;
		sm_Material.Diffuse.g = 1.0f;
		sm_Material.Diffuse.b = 1.0f;
		sm_Material.Diffuse.a = 1.0f;
		sm_Material.Ambient = sm_Material.Diffuse;

		D3DModel::GetVertexBuffer(&sm_pVB);
		D3DModel::GetIndexBuffer(&sm_pIB);

		// We need the count to match the number of objects.
		//sm_pVB->Release();
		//sm_pIB->Release();
	}
	else
	{
		// Share some stuff.

		m_pVB = sm_pVB;
		m_pVB->AddRef();

		m_pIB = sm_pIB;
		m_pIB->AddRef();
		
		m_uiNumVertices = 4;
		m_uiNumFaces = 2;
		m_uiMaxVertices = m_uiNumVertices;
		m_uiMaxFaces = m_uiNumFaces;
		m_uiFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
		m_uiVertexSize = D3DXGetFVFVertexSize(m_uiFVF);

		m_spTexture = pTexture;
	}
}

QuadModel::~QuadModel()
{
}

void QuadModel::Render(Mesh* pMesh, Camera* pCamera, bool bSimple)
{
	IDirect3DDevice9* pD3DDevice = Manager::GetDevice()->GetD3DDevice();

	pD3DDevice->SetMaterial(&sm_Material);
	
	if (m_spTexture)
		pD3DDevice->SetTexture(0, m_spTexture->GetD3DTexture());
	else
		pD3DDevice->SetTexture(0, 0);

	pD3DDevice->SetRenderState(D3DRS_COLORVERTEX, FALSE);
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	
	D3DModel::Render(pMesh, pCamera, bSimple);

	pD3DDevice->SetRenderState(D3DRS_COLORVERTEX, TRUE);
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

bool QuadModel::AlphaRequired() const
{
	if (m_spTexture)
		return m_spTexture->AlphaRequired();

	return false;
}

}
}