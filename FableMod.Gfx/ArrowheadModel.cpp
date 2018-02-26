#include "Stdafx.h"
#include "ArrowheadModel.h"
#include "Manager.h"

namespace FableMod
{
namespace Gfx
{

ArrowheadModel::ArrowheadModel()
{
	ZeroMemory(&m_Material, sizeof(m_Material));

	Create();
}

ArrowheadModel::~ArrowheadModel()
{
}

void ArrowheadModel::Create()
{
	D3DModel::Create(D3DFVF_XYZ | D3DFVF_NORMAL, 5, 6);

	LockVertices();

	const FLOAT fSize = 1.2f;
	const FLOAT fSize2 = 0.5f;

	SetVertex(0, D3DXVECTOR3(-fSize2, -fSize2,     0));
	SetVertex(1, D3DXVECTOR3( fSize2, -fSize2,     0));
	SetVertex(2, D3DXVECTOR3( fSize2,  fSize2,     0));
	SetVertex(3, D3DXVECTOR3(-fSize2,  fSize2,     0));
	SetVertex(4, D3DXVECTOR3(      0,       0, fSize));

	SetNormal(0, D3DXVECTOR3(-fSize2, -fSize2, -fSize2));
	SetNormal(1, D3DXVECTOR3( fSize2, -fSize2, -fSize2));
	SetNormal(2, D3DXVECTOR3( fSize2,  fSize2, -fSize2));
	SetNormal(3, D3DXVECTOR3(-fSize2,  fSize2, -fSize2));
	SetNormal(4, D3DXVECTOR3(      0,       0,  fSize));

	UnlockVertices();

	LockFaces();

	SetFace(0, 0, 3, 1);
	SetFace(1, 1, 3, 2);

	SetFace(2, 4, 0, 1);
	SetFace(3, 4, 3, 0);
	SetFace(4, 4, 2, 3);
	SetFace(5, 4, 1, 2);

	UnlockFaces();

	SetColor(D3DCOLOR_ARGB(255, 255, 0, 0));
}

void ArrowheadModel::Render(Mesh* pMesh, Camera* pCamera, bool bSimple)
{
	IDirect3DDevice9* pD3DDevice = Manager::GetDevice()->GetD3DDevice();

	pD3DDevice->SetMaterial(&m_Material);
	pD3DDevice->SetTexture(0, 0);

	pD3DDevice->SetRenderState(D3DRS_COLORVERTEX, FALSE);
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	
	D3DModel::Render(pMesh, pCamera, bSimple);

	pD3DDevice->SetRenderState(D3DRS_COLORVERTEX, TRUE);
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}


void ArrowheadModel::SetColor(D3DCOLOR Color)
{
	m_Material.Diffuse.r = (FLOAT)((Color >> 16) & 0xFF)/255.0f;
	m_Material.Diffuse.g = (FLOAT)((Color >> 8) & 0xFF)/255.0f;
	m_Material.Diffuse.b = (FLOAT)(Color & 0xFF)/255.0f;
	m_Material.Diffuse.a = 1.0f;
	m_Material.Ambient = m_Material.Diffuse;
}

}
}