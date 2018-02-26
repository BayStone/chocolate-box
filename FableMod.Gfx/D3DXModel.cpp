#include "stdafx.h"
#include "D3DXModel.h"
#include "Manager.h"
#include "Mesh.h"
#include "Camera.h"
#include "Helpers.h"

namespace FableMod
{
namespace Gfx
{

D3DXModel::D3DXModel() :
m_aMaterials(0),
m_pMesh(0),
m_bAlphaRequired(false),
m_dwNumMaterials(0)
{
}

D3DXModel::~D3DXModel()
{
	Dispose();
}

void D3DXModel::Dispose()
{
	DisposeMaterials();

	GFX_SAFE_RELEASE(m_pMesh);

	Model::Dispose();
}

void D3DXModel::DisposeMaterials()
{
	GFX_SAFE_ARRAYDELETE(m_aMaterials);
	m_dwNumMaterials = 0;
}

void D3DXModel::Load(LPCTSTR pFilename)
{
	Dispose();

	LPD3DXBUFFER pD3DXMtrlBuffer = 0;

	GFX_TRYD3D(D3DXLoadMeshFromX(
		pFilename,
		D3DXMESH_VB_SYSTEMMEM | D3DPOOL_DEFAULT |
		D3DXMESH_IB_SYSTEMMEM | D3DPOOL_DEFAULT,
		Manager::GetDevice()->GetD3DDevice(),
		NULL,
		&pD3DXMtrlBuffer,
		NULL,
		&m_dwNumMaterials,
		&m_pMesh));

	D3DXMATERIAL* d3dxMaterials = 
		(D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();

	m_aMaterials = new SMaterial[m_dwNumMaterials];
	
	TCHAR szTmp[MAX_PATH];

	for (DWORD i = 0; i < m_dwNumMaterials; ++i)
	{
		m_aMaterials[i].D3DMaterial = d3dxMaterials[i].MatD3D;
		m_aMaterials[i].D3DMaterial.Ambient = d3dxMaterials[i].MatD3D.Diffuse;

		MultiByteToWideChar(
			CP_ACP,
			MB_ERR_INVALID_CHARS,
			d3dxMaterials[i].pTextureFilename,
			-1,
			szTmp,
			MAX_PATH);

		m_aMaterials[i].spTexture = new Texture();
		m_aMaterials[i].spTexture->Load(szTmp);
	}

	pD3DXMtrlBuffer->Release();

	CreateFaceNormals();
	CreateBoundSpheres();
}

void D3DXModel::BuildSphere(FLOAT fRadius, D3DXCOLOR Color)
{
	Dispose();

	GFX_TRYD3D(D3DXCreateSphere(
		Manager::GetDevice()->GetD3DDevice(),
		1,
		6,
		6,
		&m_pMesh,
		NULL));

	m_dwNumMaterials = 1;

	m_aMaterials = new SMaterial[m_dwNumMaterials];
	
	ZeroMemory(&m_aMaterials[0].D3DMaterial, 
		sizeof(m_aMaterials[0].D3DMaterial));

	m_aMaterials[0].D3DMaterial.Diffuse.a = FLOAT((Color >> 24) & 0xFF)/255.0f;
	m_aMaterials[0].D3DMaterial.Diffuse.r = FLOAT((Color >> 16) & 0xFF)/255.0f;
	m_aMaterials[0].D3DMaterial.Diffuse.g = FLOAT((Color >> 8) & 0xFF)/255.0f;
	m_aMaterials[0].D3DMaterial.Diffuse.b = FLOAT(Color & 0xFF)/255.0f;
	m_aMaterials[0].D3DMaterial.Ambient = m_aMaterials[0].D3DMaterial.Diffuse;

	m_bAlphaRequired = m_aMaterials[0].D3DMaterial.Diffuse.a < 1.0f;
}

void D3DXModel::Build(
	LPD3DXMESH pMesh,
	SMaterial* aMaterials, 
	DWORD dwNumMaterials)
{
	Dispose();

	m_pMesh = pMesh;
	m_pMesh->AddRef();

	m_dwNumMaterials = dwNumMaterials;

	m_aMaterials = new SMaterial[m_dwNumMaterials];
	
	m_bAlphaRequired = false;

	for (DWORD i = 0; i < m_dwNumMaterials; ++i) 
	{
		m_aMaterials[i].D3DMaterial = aMaterials[i].D3DMaterial;
		m_aMaterials[i].spTexture = aMaterials[i].spTexture;
		m_aMaterials[i].bAlpha = aMaterials[i].bAlpha;

		/*
		if (m_aMaterials[i].spTexture)
		{
			if (m_aMaterials[i].spTexture->AlphaRequired())
				m_bAlphaRequired = true;
		}
		*/

		if (m_aMaterials[i].bAlpha)
			m_bAlphaRequired = true;
	}
}

void D3DXModel::SimpleRender()
{
	IDirect3DDevice9* pDevice = Manager::GetDevice()->GetD3DDevice();

	pDevice->SetFVF(m_pMesh->GetFVF());

	LPDIRECT3DVERTEXBUFFER9 pVB;

	if (SUCCEEDED(m_pMesh->GetVertexBuffer(&pVB)))
	{
		pDevice->SetStreamSource(
			0,pVB,0,m_pMesh->GetNumBytesPerVertex());

		LPDIRECT3DINDEXBUFFER9 pIB;

		if (SUCCEEDED(m_pMesh->GetIndexBuffer(&pIB)))
		{
			pDevice->SetIndices(pIB);

			pDevice->SetTexture(0,0);	

			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST,
				0,
				0,
				m_pMesh->GetNumVertices(),
				0,
				m_pMesh->GetNumFaces());
			
			pIB->Release();
		}

		pVB->Release();
	}
}

void D3DXModel::Render(Mesh* pMesh, Camera* pCamera, bool bSimple)
{
	IDirect3DDevice9* pDevice = Manager::GetDevice()->GetD3DDevice();

	if (bSimple)
	{
		SimpleRender();
	}
	else
	{
		if (m_dwNumMaterials == 0)
		{
			D3DMATERIAL9 Material;

			ZeroMemory(&Material, sizeof(Material));

			Material.Diffuse.r = 1.0f;
			Material.Diffuse.g = 1.0f;
			Material.Diffuse.b = 1.0f;
			Material.Diffuse.a = pMesh->GetAlpha();
			Material.Ambient = Material.Diffuse;

			pDevice->SetMaterial(&Material);

			SimpleRender();

			return;
		}
		
		DWORD dwLights = 0;

		pDevice->GetRenderState(D3DRS_LIGHTING, &dwLights);
		
		bool bLightsOn = false;

		const FLOAT fAlpha = pMesh->GetAlpha();

		for (DWORD i = 0; i < m_dwNumMaterials; ++i)
		{
			m_aMaterials[i].D3DMaterial.Diffuse.a *= fAlpha;
			m_aMaterials[i].D3DMaterial.Ambient.a *= fAlpha;

			pDevice->SetMaterial(&m_aMaterials[i].D3DMaterial);

			if (m_aMaterials[i].spTexture)
			{
				if (bLightsOn)
				{
					pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
					bLightsOn = false;
				}

				pDevice->SetTexture(0, 
					m_aMaterials[i].spTexture->GetD3DTexture());
			}
			else
			{
				if (!bLightsOn)
				{
					pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
					bLightsOn = true;
				}

				pDevice->SetTexture(0, 0);
			}

			m_pMesh->DrawSubset(i);

			m_aMaterials[i].D3DMaterial.Diffuse.a /= fAlpha;
			m_aMaterials[i].D3DMaterial.Ambient.a /= fAlpha;
		}

		pDevice->SetRenderState(D3DRS_LIGHTING, dwLights);
	}
}

void D3DXModel::GetVertexBuffer(LPDIRECT3DVERTEXBUFFER9* pVB)
{
	m_pMesh->GetVertexBuffer(pVB);
}

void D3DXModel::GetIndexBuffer(LPDIRECT3DINDEXBUFFER9* pIB)
{
	m_pMesh->GetIndexBuffer(pIB);
}

bool D3DXModel::AlphaRequired() const
{
	return m_bAlphaRequired;
}

}
}