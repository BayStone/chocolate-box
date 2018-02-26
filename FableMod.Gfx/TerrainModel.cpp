#include "Stdafx.h"
#include <cassert>
#include "../FableMod.LEV/LEVFile.h"
#include "Mesh.h"
#include "TerrainModel.h"
#include "Manager.h"
#include "Helpers.h"

namespace FableMod
{
namespace Gfx
{

TerrainModel::TerrainModel(Terrain* pTerrain) :
m_pTerrain(pTerrain)
{
}

TerrainModel::~TerrainModel()
{
	Dispose();
}

void TerrainModel::Dispose()
{
	D3DXModel::Dispose();
}

void TerrainModel::GetLocation(
	unsigned int uiFace, 
	unsigned int& ruiX, 
	unsigned int& ruiY) const
{
	const unsigned int uiSize = m_uiSize-1;

	// Give the coordinate inside the block.
	ruiY = uiFace/(uiSize*2);
	ruiX = (uiFace-(ruiY*(uiSize*2)))/2;

	assert(ruiY < uiSize);
	assert(ruiX < uiSize);
}

static void SetupFacesAndNormals(LPD3DXMESH pD3DMesh, int iBlockSize)
{
	// Generate indices...

	LPDIRECT3DINDEXBUFFER9 pMeshIB = 0;

	pD3DMesh->GetIndexBuffer(&pMeshIB);

	D3DINDEXBUFFER_DESC IBDesc;

	pMeshIB->GetDesc(&IBDesc);

	void* pFaceData = 0;

	GFX_TRYD3D(pMeshIB->Lock(0, 0, &pFaceData, D3DLOCK_DISCARD));
	
	unsigned int uiVertex = 0;

	if (IBDesc.Format == D3DFMT_INDEX16)
	{
		unsigned short* pFaces = (unsigned short*)pFaceData;

		for (unsigned int i = 0; i < iBlockSize; ++i)
		{
			for (unsigned int j = 0; j < iBlockSize; ++j)
			{
				// CCW! There's another one down.
 
				*pFaces++ = uiVertex+iBlockSize+1;			
				*pFaces++ = uiVertex+1;			
				*pFaces++ = uiVertex;			
				
				*pFaces++ = uiVertex+iBlockSize+1;			
				*pFaces++ = uiVertex+iBlockSize+2;			
				*pFaces++ = uiVertex+1;		
				
				if (j == iBlockSize-1)
				{
					// Jump over the extra vertex at the end of the line.
					uiVertex += 2;
				}
				else
					++uiVertex;
			}
		}
	}
	else if (IBDesc.Format == D3DFMT_INDEX32)
	{
		unsigned int* pFaces = (unsigned int*)pFaceData;

		for (unsigned int i = 0; i < iBlockSize; ++i)
		{
			for (unsigned int j = 0; j < iBlockSize; ++j)
			{
				// CCW!

				*pFaces++ = uiVertex+iBlockSize+1;			
				*pFaces++ = uiVertex+1;			
				*pFaces++ = uiVertex;			
				
				*pFaces++ = uiVertex+iBlockSize+1;			
				*pFaces++ = uiVertex+iBlockSize+2;			
				*pFaces++ = uiVertex+1;			
				
				if (j == iBlockSize-1)
				{
					// Jump over the extra vertex at the end of the line.
					uiVertex += 2;
				}
				else
					++uiVertex;
			}
		}
	}

	pMeshIB->Unlock();
	pMeshIB->Release();

	// Generate normals...

	D3DXComputeTangentFrameEx(
		pD3DMesh,
		D3DX_DEFAULT,
		0,
		D3DX_DEFAULT,
		0,
		D3DX_DEFAULT,
		0,
		D3DDECLUSAGE_NORMAL,
		0,
		D3DXTANGENT_GENERATE_IN_PLACE | D3DXTANGENT_CALCULATE_NORMALS,
		NULL,
		-1.01f,
		-0.01f,
		-1.01f,
		NULL,
		NULL);
}

void TerrainModel::Create(
	FableMod::LEV::LEVFile* pLEVFile,
	int iX,
	int iY,
	int iBlockSize,
	float fCellSize)
{
	D3DVERTEXELEMENT9 aDeclaration[MAX_FVF_DECL_SIZE];

	D3DXDeclaratorFromFVF(
		D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1,
		aDeclaration);

	const unsigned int uiNumVertices = (iBlockSize+1)*(iBlockSize+1);
	const unsigned int uiNumFaces = uiNumVertices*2;

	GFX_TRYD3D(D3DXCreateMesh(
		uiNumFaces,
		uiNumVertices,
		D3DXMESH_VB_SYSTEMMEM | D3DPOOL_DEFAULT |
		D3DXMESH_IB_SYSTEMMEM | D3DPOOL_DEFAULT,
		aDeclaration,
		Manager::GetDevice(),
		&m_pMesh));
	
	// Generate vertices...

	LPDIRECT3DVERTEXBUFFER9 pMeshVB = 0;

	m_pMesh->GetVertexBuffer(&pMeshVB);

	unsigned char* pVerts;

	GFX_TRYD3D(pMeshVB->Lock(0, 0, (VOID**)&pVerts, D3DLOCK_DISCARD));
	
	const FLOAT fBlockWorldSize = iBlockSize*fCellSize;

	FLOAT fY = -fBlockWorldSize/2;

	int y = iY;

	FLOAT fRatioW = pLEVFile->GetWidth()/
		(FLOAT)GetTerrain()->GetTextureSize();
	FLOAT fRatioH = pLEVFile->GetHeight()/
		(FLOAT)GetTerrain()->GetTextureSize();

	for (unsigned int i = 0; i <= iBlockSize; ++i)
	{
		FLOAT fX = fBlockWorldSize/2;
		
		int x = iX+iBlockSize;
		
		for (unsigned int j = 0; j <= iBlockSize; ++j)
		{
			const float fHeight = pLEVFile->GetHeightAt(x,y);

			*((D3DXVECTOR3*)pVerts)	= D3DXVECTOR3(fX, fY, fHeight*fCellSize);
			
			*((D3DXVECTOR3*)(pVerts+12)) = D3DXVECTOR3(0, 0, 1);

			int iG = 32+255*(fHeight/100.0f);

			if (iG > 255)
				iG = 255;

			*((D3DCOLOR*)(pVerts+24)) = D3DCOLOR_ARGB(255,iG,iG,iG);

			*((D3DXVECTOR2*)(pVerts+28)) = D3DXVECTOR2(
				(x/(float)pLEVFile->GetWidth())*fRatioW,
				(y/(float)pLEVFile->GetHeight())*fRatioH);

			fX -= fCellSize;
			--x;

			pVerts += m_pMesh->GetNumBytesPerVertex();
		}
		
		++y;

		fY += fCellSize;
	}

	pMeshVB->Unlock();
	pMeshVB->Release();

	SetupFacesAndNormals(m_pMesh, iBlockSize);

	m_uiSize = iBlockSize+1;
}

void TerrainModel::ComputeBarycenter(const D3DXVECTOR2& vV0,
									 const D3DXVECTOR2& vV1, 
									 const D3DXVECTOR2& vV2, 
									 const D3DXVECTOR2& vP,
									 FLOAT afBary[3])
{
    const D3DXVECTOR2 vV02(vV0-vV2);
    const D3DXVECTOR2 vV12(vV1-vV2);
    const D3DXVECTOR2 vPV2(vP-vV2);

	const FLOAT fM00 = D3DXVec2Dot(&vV02,&vV02);
    const FLOAT fM01 = D3DXVec2Dot(&vV02,&vV12);
    const FLOAT fM11 = D3DXVec2Dot(&vV12,&vV12);
    const FLOAT fR0 = D3DXVec2Dot(&vV02,&vPV2);
    const FLOAT fR1 = D3DXVec2Dot(&vV12,&vPV2);
    const FLOAT fDet = fM00*fM11-fM01*fM01;

	assert(abs(fDet) > 0.0);
    
	const FLOAT fInvDet = 1.0/fDet;

    afBary[0] = (fM11*fR0-fM01*fR1)*fInvDet;
    afBary[1] = (fM00*fR1-fM01*fR0)*fInvDet;
    afBary[2] = 1.0-afBary[0]-afBary[1];
}

//----------------------------------------------------------------------------

bool TerrainModel::InTriangle(const D3DXVECTOR2& vV0,
							  const D3DXVECTOR2& vV1, 
							  const D3DXVECTOR2& vV2, 
							  const D3DXVECTOR2& vTest)
{
    const FLOAT fEpsilon = 1e-06;

	if (abs(vV1.x-vV0.x) < fEpsilon &&
		abs(vV1.y-vV0.y) < fEpsilon &&
		abs(vV2.x-vV0.x) < fEpsilon &&
		abs(vV2.y-vV0.y) < fEpsilon)
	{
		// Same points.
		return false;
	}

    // test against normal to first edge
    D3DXVECTOR2 vDir(vTest-vV0);
    D3DXVECTOR2 vNor(vV0.y-vV1.y, vV1.x-vV0.x);
    if (D3DXVec2Dot(&vDir,&vNor) < -fEpsilon)
        return false;

    // test against normal to second edge
    vDir = vTest-vV1;
    vNor = D3DXVECTOR2(vV1.y-vV2.y, vV2.x-vV1.x);
    if (D3DXVec2Dot(&vDir,&vNor) < -fEpsilon)
        return false;

    // test against normal to third edge
    vDir = vTest-vV2;
    vNor = D3DXVECTOR2(vV2.y-vV0.y, vV0.x-vV2.x);
    if (D3DXVec2Dot(&vDir,&vNor) < -fEpsilon)
        return false;

    return true;
}

void TerrainModel::BuildHeightfield(
	const D3DXMATRIX* pWorldMatrix,
	const D3DXVECTOR2& vWorldBase, 
	EditableTexture* pTexture,
	unsigned int uiTexX,
	unsigned int uiTexY,
	FLOAT fCellSize)
{
	LPDIRECT3DVERTEXBUFFER9 pVB;

	m_pMesh->GetVertexBuffer(&pVB);

	LPDIRECT3DINDEXBUFFER9 pIB;

	m_pMesh->GetIndexBuffer(&pIB);

	LPVOID pVertices;
	LPVOID pFaces;

	pVB->Lock(0, 0, &pVertices, D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY);
	pIB->Lock(0, 0, &pFaces, D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY);
	
	unsigned char* pVerts = (unsigned char*)pVertices;
	unsigned short* pwFaces = (unsigned short*)pFaces;

	const unsigned int uiVertexSize = m_pMesh->GetNumBytesPerVertex();

	for (unsigned int y = 0; y < m_uiSize; ++y)
	{
		for (unsigned int x = 0; x < m_uiSize; ++x)
		{
			D3DXVECTOR3 v = *((D3DXVECTOR3*)pVerts);

			D3DXVec3TransformCoord(&v, &v, pWorldMatrix);
		
			const FLOAT fHeight = v.z;

			int c = 32+255*((fHeight/fCellSize)/100.0f);

			if (c > 255)
				c = 255;

			pTexture->DrawPixel(
				uiTexX-x,
				uiTexY+y,
				D3DXCOLOR(D3DCOLOR_ARGB(255,c,c,c)));

			pVerts += uiVertexSize;
		}
	}

	/*

	// Build heightfield based on triangles rather than 
	// basic vertex locations.

	D3DXVECTOR3 v0,v1,v2;

	D3DXVECTOR2 vV0,vV1,vV2;

	unsigned int i = 0;

	char* pucDone = new char[m_uiSize*m_uiSize];

	memset(pucDone, 0, m_uiSize*m_uiSize);

	for (; i < m_pMesh->GetNumFaces(); ++i)
	{
		v0 = *((D3DXVECTOR3*)(pVerts+(*pwFaces++)*uiVertexSize));
		v1 = *((D3DXVECTOR3*)(pVerts+(*pwFaces++)*uiVertexSize));
		v2 = *((D3DXVECTOR3*)(pVerts+(*pwFaces++)*uiVertexSize));

		D3DXVec3TransformCoord(&v0, &v0, pWorldMatrix);
		D3DXVec3TransformCoord(&v1, &v1, pWorldMatrix);
		D3DXVec3TransformCoord(&v2, &v2, pWorldMatrix);
		
		vV0 = D3DXVECTOR2(v0.x, v0.y);
		vV1 = D3DXVECTOR2(v1.x, v1.y);
		vV2 = D3DXVECTOR2(v2.x, v2.y);

		FLOAT fY = vWorldBase.y;

		unsigned int uiY = uiTexY;

		for (unsigned int y = 0; y < m_uiSize; ++y)
		{
			FLOAT fX = vWorldBase.x;

			unsigned int uiX = uiTexX;

			for (unsigned int x = 0; x < m_uiSize; ++x)
			{
				const D3DXVECTOR2 vPoint(fX, fY);

				if (!pucDone[y*m_uiSize+x] && 
					InTriangle(vV0, vV1, vV2, vPoint))
				{
					pucDone[y*m_uiSize+x] = 1;

					FLOAT afBary[3];
    
					ComputeBarycenter(vV0, vV1, vV2, vPoint, afBary);

					D3DXVECTOR3 vResult = 
						v0*afBary[0]+v1*afBary[1]+v2*afBary[2];

					const FLOAT fHeight = vResult.z;

					int c = 32+255*((fHeight/fCellSize)/100.0f);

					if (c > 255)
						c = 255;

					pTexture->DrawPixel(
						uiX,
						uiY,
						D3DXCOLOR(D3DCOLOR_ARGB(255,c,c,c)));
				}
			
				fX -= fCellSize;
				--uiX;
			}
		
			fY += fCellSize;
			++uiY;
		}
	}

	delete pucDone;
	*/

	pIB->Unlock();
	pVB->Unlock();

	pIB->Release();
	pVB->Release();
}

//----------------------------------------------------------------------------

void TerrainModel::Render(Mesh* pMesh, Camera* pCamera, bool bSimple)
{
	IDirect3DDevice9* pDevice = Manager::GetDevice();

	if (!bSimple)
	{
		pDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_POINT);
		pDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
	
		D3DMATERIAL9 Material;

		ZeroMemory(&Material, sizeof(Material));

		Material.Diffuse.r = 1.0f;
		Material.Diffuse.g = 1.0f;
		Material.Diffuse.b = 1.0f;
		Material.Diffuse.a = 1.0f;
		Material.Ambient = Material.Diffuse;

		pDevice->SetMaterial(&Material);

		// Setup texture.
		pDevice->SetTexture(0, 
			GetTerrain()->GetTexture()->GetD3DTexture());
		
		m_pMesh->DrawSubset(0);
		
		pDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
		pDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	}
	else
		SimpleRender();
}

}
}