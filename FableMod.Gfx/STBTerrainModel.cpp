#include "Stdafx.h"
#include "STBTerrainModel.h"
#include "Manager.h"
#include "Helpers.h"

namespace FableMod
{
namespace Gfx
{

STBTerrainModel::STBTerrainModel()
{
	ZeroMemory(&m_Material, sizeof(m_Material));
}

STBTerrainModel::~STBTerrainModel()
{
}

void STBTerrainModel::Create(
	D3DXVECTOR3* pVertices,
	D3DXVECTOR3* pNormals,
	D3DXVECTOR2* pUVs,
	unsigned int uiVertexCount,
	unsigned int* pFaces,
	unsigned int uiFaceCount)
{
	D3DModel::Create(
		D3DFVF_XYZ | 
		D3DFVF_NORMAL |
		D3DFVF_TEX1,
		uiVertexCount,
		uiFaceCount);

	LockVertices();

	for (unsigned int i = 0; i < uiVertexCount; ++i)
	{
		SetVertex(i, pVertices[i]);
		SetNormal(i, pNormals[i]);
		SetTextureCoordinate(0, i, pUVs[i]);
	}

	UnlockVertices();

	LockFaces();
		
	int faceIndex = 0;
	
	for (unsigned int i = 0; i < uiFaceCount; ++i)
	{
		SetFace(i, 
			pFaces[faceIndex+0],
			pFaces[faceIndex+1],
			pFaces[faceIndex+2]);

		faceIndex += 3;
	}

	UnlockFaces();

	CreateBoundSpheres();

	CreateFaceNormals();

	SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
}

void STBTerrainModel::Render(Mesh* pMesh, Camera* pCamera, bool bSimple)
{
	IDirect3DDevice9* pDevice = Manager::GetDevice()->GetD3DDevice();

	DWORD dwCullMode;

	if (!bSimple)
	{
		pDevice->SetMaterial(&m_Material);
		pDevice->SetTexture(0, m_Texture ? m_Texture->GetD3DTexture() : 0);

		pDevice->SetRenderState(D3DRS_COLORVERTEX, FALSE);
		//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	
		pDevice->GetRenderState(D3DRS_CULLMODE, &dwCullMode);

		//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	pDevice->SetFVF(m_uiFVF);

	pDevice->SetStreamSource(0,m_pVB,0,GetVertexSize());

	pDevice->SetIndices(m_pIB);

	GFX_TRYD3D(pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,
		0,
		GetNumVertices(),
		0,
		GetNumFaces()));

	if (!bSimple)
	{
		pDevice->SetRenderState(D3DRS_COLORVERTEX, TRUE);
		//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		pDevice->SetRenderState(D3DRS_CULLMODE, dwCullMode);
	}
}

void STBTerrainModel::SetTexture(Texture* Texture)
{
	m_Texture = Texture;
}

void STBTerrainModel::SetColor(D3DCOLOR Color)
{
	m_Material.Diffuse.r = (FLOAT)((Color >> 16) & 0xFF)/255.0f;
	m_Material.Diffuse.g = (FLOAT)((Color >> 8) & 0xFF)/255.0f;
	m_Material.Diffuse.b = (FLOAT)(Color & 0xFF)/255.0f;
	m_Material.Diffuse.a = 1.0f;
	m_Material.Ambient = m_Material.Diffuse;
}

}
}