#include "stdafx.h"
#include "Model.h"
#include "Manager.h"
#include "Mesh.h"
#include "Camera.h"
#include "Helpers.h"
#include "Triangle.h"
#include "Intersection.h"

namespace FableMod
{
namespace Gfx
{

Model::Model() :
m_avFaceNormals(0),
m_aBoundSpheres(0),
m_uiNumBoundSpheres(0),
m_uiNumFaceNormals(0)
{
}

void Model::Dispose()
{
	GFX_SAFE_ARRAYDELETE(m_avFaceNormals);
	GFX_SAFE_ARRAYDELETE(m_aBoundSpheres);
	m_uiNumBoundSpheres = 0;
	m_uiNumFaceNormals = 0;

	RootObject::Dispose();
}

void Model::ComputeBoundSphere(BoundSphere* pSphere)
{
	pSphere->Center() = D3DXVECTOR3(0,0,0);
	pSphere->Radius() = 0.0f;

	LPDIRECT3DVERTEXBUFFER9 pVB;
	
	GetVertexBuffer(&pVB);

	LPVOID pVertices;

	if (SUCCEEDED(pVB->Lock(0, 0, &pVertices, D3DLOCK_NOSYSLOCK)))
	{
		unsigned char* pBuffer = (unsigned char*)pVertices;

		const unsigned int uiNumVertices = GetNumVertices();
		
		D3DXVECTOR3 vVertex(*((D3DXVECTOR3*)pBuffer));
		D3DXVECTOR3 vMin(vVertex),vMax(vVertex);
		
		for (unsigned int i = 0; i < uiNumVertices; ++i)
		{
			vVertex = *((D3DXVECTOR3*)pBuffer);

			if (vMin[0] > vVertex[0])
				vMin[0] = vVertex[0];        
			if (vMin[1] > vVertex[1])
				vMin[1] = vVertex[1];
			if (vMin[2] > vVertex[2])
				vMin[2] = vVertex[2];
			if (vMax[0] < vVertex[0])
				vMax[0] = vVertex[0];
			if (vMax[1] < vVertex[1])
				vMax[1] = vVertex[1];
			if (vMax[2] < vVertex[2])
				vMax[2] = vVertex[2];

			pBuffer += GetVertexSize();
		}

		pSphere->Center() = (vMin+vMax)*0.5f;
		
		FLOAT fRadiusSqr = 0.0f;
    
		pBuffer = (unsigned char*)pVertices;

		for (unsigned int i = 0; i < uiNumVertices; ++i) 
		{
			vVertex = *((D3DXVECTOR3*)pBuffer);

			D3DXVECTOR3 vDiff(vVertex-pSphere->Center());
	        
			FLOAT fLengthSqr = D3DXVec3LengthSq(&vDiff);
	        
			if (fLengthSqr > fRadiusSqr)
				fRadiusSqr = fLengthSqr;
			
			pBuffer += GetVertexSize();
		}

		pSphere->Radius() = sqrt(fRadiusSqr);

		/*
		assert(SUCCEEDED(D3DXComputeBoundingSphere(
			(D3DXVECTOR3*)pVertices, 
			GetNumVertices(),
			GetVertexSize(),
			&pSphere->Center(), 
			&pSphere->Radius())));
		*/

		pVB->Unlock();
	}

	pVB->Release();
}

void Model::CreateFaceNormals()
{
	if (m_uiNumFaceNormals < GetNumFaces())
	{
		if (m_avFaceNormals)
			delete[] m_avFaceNormals;

		m_avFaceNormals = new D3DXVECTOR3[GetNumFaces()];
		m_uiNumFaceNormals = GetNumFaces();
	}

	LPDIRECT3DVERTEXBUFFER9 pVB;

	GetVertexBuffer(&pVB);

	LPDIRECT3DINDEXBUFFER9 pIB;

	GetIndexBuffer(&pIB);

	LPVOID pVertices;
	LPVOID pFaces;

	pVB->Lock(0, 0, &pVertices, D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY);
	pIB->Lock(0, 0, &pFaces, D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY);
	
	unsigned short* pwFaces = (unsigned short*)pFaces;
	unsigned char* pVerts = (unsigned char*)pVertices;

	const unsigned int uiVertexSize = GetVertexSize();

	for (unsigned int i = 0; i < GetNumFaces(); ++i)
	{
		D3DXVECTOR3 v0(*((D3DXVECTOR3*)
			(pVerts+(*pwFaces++)*uiVertexSize)));

		D3DXVECTOR3 v1(*((D3DXVECTOR3*)
			(pVerts+(*pwFaces++)*uiVertexSize)));
	
		D3DXVECTOR3 v2(*((D3DXVECTOR3*)
			(pVerts+(*pwFaces++)*uiVertexSize)));

		Triangle Tri(v0,v1,v2);

		m_avFaceNormals[i] = Tri.Normal();
	}

	pIB->Unlock();
	pVB->Unlock();

	pIB->Release();
	pVB->Release();
}

void Model::DisposeFaceNormals()
{
	m_uiNumFaceNormals = 0;
	GFX_SAFE_ARRAYDELETE(m_avFaceNormals);
}

void Model::ComputeAxisBox(AxisBox* pBox, const D3DXMATRIX* pmatWorld)
{
	LPDIRECT3DVERTEXBUFFER9 pMeshVB = 0;
	
	GetVertexBuffer(&pMeshVB);

	LPVOID pVertices;

	if (SUCCEEDED(pMeshVB->Lock(0, 0, &pVertices, 
		D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY)))
	{
		unsigned char* pBuffer = (unsigned char*)pVertices;

		const unsigned int uiNumVertices = GetNumVertices();
		
		for (unsigned int i = 0; i < uiNumVertices; ++i)
		{
			D3DXVECTOR3 vVertex(*((D3DXVECTOR3*)pBuffer));

			D3DXVec3TransformCoord(&vVertex,&vVertex,pmatWorld);
	
			pBox->Update(vVertex);
			
			pBuffer += GetVertexSize();
		}
		
		pMeshVB->Unlock();
	}

	pMeshVB->Release();
}

void Model::CreateBoundSpheres()
{
	if (m_uiNumBoundSpheres < GetNumFaces())
	{
		if (m_aBoundSpheres)
			delete[] m_aBoundSpheres;

		m_aBoundSpheres = new BoundSphere[GetNumFaces()];
		m_uiNumBoundSpheres = GetNumFaces();
	}

	LPDIRECT3DVERTEXBUFFER9 pVB;

	GetVertexBuffer(&pVB);

	LPDIRECT3DINDEXBUFFER9 pIB;

	GetIndexBuffer(&pIB);

	LPVOID pVertices;
	LPVOID pFaces;

	pVB->Lock(0, 0, &pVertices, D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY);
	pIB->Lock(0, 0, &pFaces, D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY);
	
	unsigned short* pwFaces = (unsigned short*)pFaces;
	unsigned char* pVerts = (unsigned char*)pVertices;

	const unsigned int uiVertexSize = GetVertexSize();

	for (unsigned int i = 0; i < GetNumFaces(); ++i)
	{
		D3DXVECTOR3 v0(*((D3DXVECTOR3*)
			(pVerts+(*pwFaces++)*uiVertexSize)));

		D3DXVECTOR3 v1(*((D3DXVECTOR3*)
			(pVerts+(*pwFaces++)*uiVertexSize)));
	
		D3DXVECTOR3 v2(*((D3DXVECTOR3*)
			(pVerts+(*pwFaces++)*uiVertexSize)));

		const D3DXVECTOR3 vC = (v0+v1+v2)/3;

		m_aBoundSpheres[i].Center() = vC;
		
		FLOAT fDist0 = D3DXVec3Length(&(v0-vC));
		FLOAT fDist1 = D3DXVec3Length(&(v1-vC));
		FLOAT fDist2 = D3DXVec3Length(&(v2-vC));

		if (fDist1 > fDist0)
			fDist0 = fDist1;

		if (fDist2 > fDist0)
			fDist0 = fDist2;

		m_aBoundSpheres[i].Radius() = fDist0;
	}

	pIB->Unlock();
	pVB->Unlock();

	pIB->Release();
	pVB->Release();
}

void Model::DisposeBoundSpheres()
{
	m_uiNumBoundSpheres = 0;
	GFX_SAFE_ARRAYDELETE(m_aBoundSpheres);
}

void Model::Pick(
	Mesh* pMesh,
	const D3DXVECTOR3& vOrigin, 
	const D3DXVECTOR3& vDirection,
	PickData& rData)
{
	LPDIRECT3DVERTEXBUFFER9 pVB;

	GetVertexBuffer(&pVB);

	LPDIRECT3DINDEXBUFFER9 pIB;

	GetIndexBuffer(&pIB);

	LPVOID pVertices;
	LPVOID pFaces;

	pVB->Lock(0, 0, &pVertices, D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY);
	pIB->Lock(0, 0, &pFaces, D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY);
	
	unsigned short* pwFaces = (unsigned short*)pFaces;
	unsigned char* pVerts = (unsigned char*)pVertices;

	const unsigned int uiVertexSize = GetVertexSize();

	// Convert the ray to model space...

	const D3DXMATRIX* pmatWorld = &pMesh->GetWorldTransform();

	D3DXMATRIX matInverse;
	D3DXMatrixInverse(&matInverse, 0, pmatWorld);

	D3DXVECTOR3 vOriginInv,vDirInv;

	D3DXVec3TransformCoord(&vOriginInv, &vOrigin, &matInverse);
	D3DXVec3TransformNormal(&vDirInv, &vDirection, &matInverse);

	FLOAT fU,fV,fDist;
	D3DXVECTOR3 v0,v1,v2,vTmp;

	PickData TmpData;

	if (m_avFaceNormals && m_aBoundSpheres)
	{
		// Optimizations used.

		for (unsigned int i = 0; i < GetNumFaces(); ++i)
		{
			v0 = *((D3DXVECTOR3*)
				(pVerts+(*pwFaces++)*uiVertexSize));

			v1 = *((D3DXVECTOR3*)
				(pVerts+(*pwFaces++)*uiVertexSize));
		
			v2 = *((D3DXVECTOR3*)
				(pVerts+(*pwFaces++)*uiVertexSize));

			// First, visibility check...
			if (D3DXVec3Dot(&m_avFaceNormals[i],&vDirInv) <= 0.0f)
			{
				// Then a bound sphere check...
				if (m_aBoundSpheres[i].RayIntersection(vOriginInv,vDirInv))
				{
					if (!D3DXIntersectTri(
						&v0, &v1, &v2, 
						&vOriginInv, &vDirInv, 
						&fU, &fV, 
						&fDist))
						continue;
					
					vTmp = v0*(1-(fU+fV))+v1*fU+v2*fV;
					
					D3DXVec3TransformCoord(&vTmp,&vTmp,pmatWorld);

					const FLOAT fRange = D3DXVec3LengthSq(&(vOrigin-vTmp));

					if (fRange < TmpData.fRange)
					{
						TmpData.fRange = fRange;
						TmpData.iFace = i;
						TmpData.vPoint = vTmp;
						TmpData.vNormal = m_avFaceNormals[i];
					}
				}
			}
		}
	}
	else
	{
		D3DXVECTOR3 vN;

		for (unsigned int i = 0; i < GetNumFaces(); ++i)
		{
			v0 = *((D3DXVECTOR3*)
				(pVerts+(*pwFaces++)*uiVertexSize));

			v1 = *((D3DXVECTOR3*)
				(pVerts+(*pwFaces++)*uiVertexSize));
		
			v2 = *((D3DXVECTOR3*)
				(pVerts+(*pwFaces++)*uiVertexSize));

			D3DXVec3Cross(&vN, &(v1-v0), &(v2-v0));

			// First, visibility check...
			if (D3DXVec3Dot(&vN,&vDirInv) <= 0.0f)
			{
				if (!D3DXIntersectTri(
					&v0, &v1, &v2, 
					&vOriginInv, &vDirInv, 
					&fU, &fV, 
					&fDist))
					continue;
				
				vTmp = v0*(1-(fU+fV))+v1*fU+v2*fV;
				
				D3DXVec3TransformCoord(&vTmp,&vTmp,pmatWorld);

				const FLOAT fRange = D3DXVec3LengthSq(&(vOrigin-vTmp));

				if (fRange < TmpData.fRange)
				{
					TmpData.fRange = fRange;
					TmpData.iFace = i;
					TmpData.vPoint = vTmp;
					TmpData.vNormal = vN;
				}
			}
		}
	}

	TmpData.fRange = sqrt(TmpData.fRange);

	if (TmpData.fRange < rData.fRange)
	{
		TmpData.pMesh = pMesh;
		D3DXVec3Normalize(&TmpData.vNormal, &TmpData.vNormal);

		memcpy(&rData, &TmpData, sizeof(TmpData));
	}

	pIB->Unlock();
	pVB->Unlock();

	pIB->Release();
	pVB->Release();
}

}
}