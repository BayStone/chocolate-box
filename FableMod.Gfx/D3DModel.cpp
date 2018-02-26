#include "Stdafx.h"
#include "D3DModel.h"
#include "Manager.h"
#include "Helpers.h"
#include "LogFile.h"
#include <cassert>

namespace FableMod
{
namespace Gfx
{

D3DModel::D3DModel() :
m_pVB(0),
m_pIB(0),
m_uiNumVertices(0),
m_uiMaxVertices(0),
m_uiVertexGrow(8),
m_uiNumFaces(0),
m_uiMaxFaces(0),
m_uiFaceGrow(8),
m_pucVertices(0),
m_pucIndices(0),
m_pucDiffuse(0)
{
	memset(m_apucTexCoords, 0, sizeof(m_apucTexCoords));
}

D3DModel::~D3DModel()
{
	Dispose();
}

void D3DModel::Dispose()
{
	GFX_SAFE_RELEASE(m_pVB);
	GFX_SAFE_RELEASE(m_pIB);
}

void D3DModel::Create(
	unsigned int uiFVF,
	unsigned int uiVertices,
	unsigned int uiFaces)
{
	m_uiFVF = uiFVF;
	m_uiVertexSize = D3DXGetFVFVertexSize(m_uiFVF);

	AllocateVertices(uiVertices);
	AllocateFaces(uiFaces);

	m_ucTexCoordSets = 0;

	if (m_uiFVF & D3DFVF_TEX1)
		m_ucTexCoordSets = 1;
	else if (m_uiFVF & D3DFVF_TEX2)
		m_ucTexCoordSets = 2;
	else if (m_uiFVF & D3DFVF_TEX3)
		m_ucTexCoordSets = 3;
}

void D3DModel::Render(Mesh* pMesh, Camera* pCamera, bool bSimple)
{
	IDirect3DDevice9* pDevice = Manager::GetDevice()->GetD3DDevice();

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
}

void D3DModel::Reset()
{
	m_uiNumVertices = 0;
	m_uiNumFaces = 0;
}

void D3DModel::AllocateVertices(unsigned int uiNumVertices)
{
	IDirect3DDevice9* pD3DDevice = Manager::GetDevice()->GetD3DDevice();
			
	if (uiNumVertices > m_uiMaxVertices)
	{
		// Reallocate.
		
		unsigned int uiOldMaxVertices = m_uiMaxVertices;

		m_uiMaxVertices = uiNumVertices+m_uiVertexGrow;
		m_uiNumVertices = uiNumVertices;

		if (m_pVB)
		{
			unsigned char* pVerticesOld = 0;
			unsigned char* pVerticesNew = 0;

			GFX_TRYD3D(m_pVB->Lock(0, 0, (void**)&pVerticesOld, 
				D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY));

			LPDIRECT3DVERTEXBUFFER9 pNewVB = 0;

			GFX_TRYD3D(pD3DDevice->CreateVertexBuffer(
				m_uiMaxVertices*GetVertexSize(),
				0,
				m_uiFVF,
				D3DPOOL_DEFAULT,
				&pNewVB,
				0));

			GFX_TRYD3D(pNewVB->Lock(
				0, 0, (void**)&pVerticesNew, D3DLOCK_DISCARD));

			memcpy(
				pVerticesNew, 
				pVerticesOld, 
				uiOldMaxVertices*GetVertexSize());

			pNewVB->Unlock();

			m_pVB->Unlock();

			GFX_SAFE_RELEASE(m_pVB);

			m_pVB = pNewVB;
		}
		else
		{
			GFX_TRYD3D(pD3DDevice->CreateVertexBuffer(
				m_uiMaxVertices*GetVertexSize(),
				0,
				m_uiFVF,
				D3DPOOL_DEFAULT,
				&m_pVB,
				0));
		}
	}
}

unsigned int D3DModel::AddVertex()
{
	AllocateVertices(++m_uiNumVertices);
	
	// Return vertex index.
	return m_uiNumVertices-1;
}

void D3DModel::AllocateFaces(unsigned int uiNumFaces)
{
	IDirect3DDevice9* pD3DDevice = Manager::GetDevice()->GetD3DDevice();
	
	if (uiNumFaces > m_uiMaxFaces)
	{
		// Reallocate.
		
		const unsigned int uiOldMaxFaces = m_uiMaxFaces;

		m_uiMaxFaces = uiNumFaces+m_uiFaceGrow;
		m_uiNumFaces = uiNumFaces;

		if (m_pIB)
		{
			unsigned char* pIndicesOld = 0;
			unsigned char* pIndicesNew = 0;

			GFX_TRYD3D(m_pIB->Lock(0, 0, (void**)&pIndicesOld, 
				D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY));
		
			LPDIRECT3DINDEXBUFFER9 pNewIB = 0;

			GFX_TRYD3D(pD3DDevice->CreateIndexBuffer(
				(m_uiMaxFaces*3)*2,
				0,
				D3DFMT_INDEX16,
				D3DPOOL_DEFAULT,
				&pNewIB,
				0));

			GFX_TRYD3D(pNewIB->Lock(
				0, 0, (void**)&pIndicesNew, D3DLOCK_DISCARD));
		
			memcpy(pIndicesNew, pIndicesOld, (uiOldMaxFaces*3)*2);

			pNewIB->Unlock();

			m_pIB->Unlock();

			GFX_SAFE_RELEASE(m_pIB);

			m_pIB = pNewIB;
		}
		else
		{
			GFX_TRYD3D(pD3DDevice->CreateIndexBuffer(
				(m_uiMaxFaces*3)*2,
				0,
				D3DFMT_INDEX16,
				D3DPOOL_DEFAULT,
				&m_pIB,
				0));
		}
	}
}

void D3DModel::AddFace(unsigned int uiVertex0,
					   unsigned int uiVertex1,
					   unsigned int uiVertex2)
{
	AllocateFaces(++m_uiNumFaces);

	LockFaces();

	SetFace(m_uiNumFaces-1, uiVertex0, uiVertex1, uiVertex2);

	UnlockFaces();
}

void D3DModel::LockVertices()
{
	m_pVB->Lock(0, 0, (void**)&m_pucVertices, D3DLOCK_NOSYSLOCK);

	m_pucNormals = m_pucVertices+sizeof(D3DXVECTOR3);
	
	if (m_uiFVF & D3DFVF_NORMAL)
		m_pucDiffuse = m_pucNormals+sizeof(D3DXVECTOR3);
	else	
		m_pucDiffuse = m_pucNormals;
	
	if (m_uiFVF & D3DFVF_DIFFUSE)
		m_apucTexCoords[0] = m_pucDiffuse+sizeof(D3DCOLOR);
	else	
		m_apucTexCoords[0] = m_pucDiffuse;

	for (unsigned char c = 1; c < m_ucTexCoordSets; ++c)
		m_apucTexCoords[c] = m_apucTexCoords[c-1]+sizeof(D3DXVECTOR2);
}

void D3DModel::UnlockVertices()
{
	m_pVB->Unlock();

	m_pucVertices = 0;
	m_pucDiffuse = 0;
}

void D3DModel::SetVertex(unsigned int uiIndex, const D3DXVECTOR3& vVertex)
{
	assert(m_pucVertices);
	assert(uiIndex < m_uiMaxVertices);
	*((D3DXVECTOR3*)(m_pucVertices+(uiIndex*GetVertexSize()))) = vVertex;
}

void D3DModel::SetNormal(unsigned int uiIndex, const D3DXVECTOR3& vNormal)
{
	assert(m_pucVertices);
	assert(uiIndex < m_uiMaxVertices);
	*((D3DXVECTOR3*)(m_pucNormals+(uiIndex*GetVertexSize()))) = vNormal;
}

void D3DModel::GetVertex(unsigned int uiIndex, D3DXVECTOR3& rvVertex)
{
	assert(m_pucVertices);
	assert(uiIndex < m_uiMaxVertices);
	rvVertex = *((D3DXVECTOR3*)(m_pucVertices+(uiIndex*GetVertexSize())));
}

void D3DModel::SetDiffuse(unsigned int uiIndex, const D3DCOLOR& Color)
{
	assert(m_pucDiffuse);
	assert(uiIndex < m_uiMaxVertices);
	*((D3DCOLOR*)(m_pucDiffuse+(uiIndex*GetVertexSize()))) = Color;
}

void D3DModel::SetTextureCoordinate(unsigned char ucSet,
									unsigned int uiIndex, 
									const D3DXVECTOR2& vUV)
{
	assert(m_apucTexCoords);
	assert(ucSet < m_ucTexCoordSets);
	assert(uiIndex < m_uiMaxVertices);
	*((D3DXVECTOR2*)(m_apucTexCoords[ucSet]+
		(uiIndex*GetVertexSize()))) = vUV;
}

void D3DModel::LockFaces()
{
	m_pIB->Lock(0, 0, (void**)&m_pucIndices, D3DLOCK_NOSYSLOCK);
}

void D3DModel::UnlockFaces()
{
	m_pIB->Unlock();

	m_pucIndices = 0;
}

void D3DModel::SetFace(
	unsigned int uiIndex,
	unsigned int uiVertex0,
	unsigned int uiVertex1,
	unsigned int uiVertex2)
{
	assert(m_pucIndices);
	assert(uiIndex < m_uiMaxFaces);

	unsigned short* pIndices = (unsigned short*)m_pucIndices;

	pIndices += (uiIndex*3);

	*pIndices++ = uiVertex0;
	*pIndices++ = uiVertex1;
	*pIndices++ = uiVertex2;
}

unsigned int D3DModel::GetVertexSize() const
{
	return m_uiVertexSize;
}

void D3DModel::GetVertexBuffer(LPDIRECT3DVERTEXBUFFER9* ppVB)
{
	m_pVB->AddRef();
	*ppVB = m_pVB;
}

void D3DModel::GetIndexBuffer(LPDIRECT3DINDEXBUFFER9* ppIB)
{
	m_pIB->AddRef();
	*ppIB = m_pIB;
}

}
}