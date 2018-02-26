#include "Stdafx.h"
#include "AllocateHierarchy.h"
#include "ExtendedFrame.h"

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

HRESULT AllocateHierarchy::CreateFrame(
	LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	*ppNewFrame = NULL;

    D3DXEXTENDEDFRAME* pNewFrame = new D3DXEXTENDEDFRAME;

	pNewFrame->Name = new char[strlen(Name)+1];
	strcpy(pNewFrame->Name, Name);

    D3DXMatrixIdentity(&pNewFrame->TransformationMatrix);
    
	*ppNewFrame = pNewFrame;
	
	return S_OK;
}

HRESULT AllocateHierarchy::CreateMeshContainer(
	THIS_ 
	LPCSTR Name,
    CONST D3DXMESHDATA *pMeshData,
    CONST D3DXMATERIAL *pMaterials,
    CONST D3DXEFFECTINSTANCE *pEffectInstances,
    DWORD NumMaterials,
    CONST DWORD *pAdjacency,
    LPD3DXSKININFO pSkinInfo,
    LPD3DXMESHCONTAINER* ppNewMeshContainer)
{    
	D3DXMESHCONTAINER* pMeshContainer = new D3DXMESHCONTAINER;
	
	ZeroMemory(pMeshContainer, sizeof(D3DXMESHCONTAINER));

	*ppNewMeshContainer = NULL;

	pMeshContainer->Name = new char[strlen(Name)+1];
	strcpy(pMeshContainer->Name, Name);

	if (pMeshData->Type != D3DXMESHTYPE_MESH)
	{
		DestroyMeshContainer(pMeshContainer);
		return E_FAIL;
	}

	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
	
	// Get the Direct3D device, luckily this is held in the mesh itself
	LPDIRECT3DDEVICE9 pDevice = NULL;
	
	pMeshData->pMesh->GetDevice(&pDevice);

	D3DVERTEXELEMENT9 Declaration[MAX_FVF_DECL_SIZE];
	
	if (FAILED(pMeshData->pMesh->GetDeclaration(Declaration)))
	{
		DestroyMeshContainer(pMeshContainer);
		return E_FAIL;
	}

	pMeshData->pMesh->CloneMeshFVF(
		D3DXMESH_MANAGED, 
		D3DFVF_XYZ  | 
		D3DFVF_NORMAL | 
		D3DFVF_TEX1 | 
		D3DFVF_TEXCOORDSIZE2(0), 
		pDevice, 
		&pMeshContainer->MeshData.pMesh);
	
	pDevice->Release();
	
	pMeshContainer->NumMaterials = 
		NumMaterials == 0 ? 1 : NumMaterials;
	
	pMeshContainer->pMaterials = 
		new D3DXMATERIAL[pMeshContainer->NumMaterials];
	
	if (NumMaterials > 0)
	{
		for(DWORD i = 0; i < NumMaterials; ++i)
		{
			pMeshContainer->pMaterials[i].MatD3D = pMaterials[i].MatD3D;
			
			if (pMaterials[i].pTextureFilename)
			{
				pMeshContainer->pMaterials[i].pTextureFilename = 
					new char[strlen(pMaterials[i].pTextureFilename)+1];
				
				strcpy(pMeshContainer->pMaterials[i].pTextureFilename, 
					pMaterials[i].pTextureFilename);
			}
		}
	}
	else    
	{
		ZeroMemory(&pMeshContainer->pMaterials[0], sizeof(D3DMATERIAL9));
		
		pMeshContainer->pMaterials->MatD3D.Diffuse.a = 1.0f;
		pMeshContainer->pMaterials->MatD3D.Diffuse.r = 1.0f;
		pMeshContainer->pMaterials->MatD3D.Diffuse.g = 1.0f;
		pMeshContainer->pMaterials->MatD3D.Diffuse.b = 1.0f;
		pMeshContainer->pMaterials->MatD3D.Ambient =
			pMeshContainer->pMaterials->MatD3D.Diffuse;
    }

	// If there is skin data associated with the mesh copy it over
	if (pSkinInfo)
	{
		// save off the SkinInfo
	    pMeshContainer->pSkinInfo = pSkinInfo;
	    pSkinInfo->AddRef();
	}
	else	
	{
		// No skin info so null all the pointers
		pMeshContainer->pSkinInfo = NULL;
	}

	*ppNewMeshContainer = pMeshContainer;    

	return S_OK;
}

HRESULT AllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree) 
{
	((D3DXEXTENDEDFRAME*)pFrameToFree)->Destroy();
    return S_OK; 
}

HRESULT AllocateHierarchy::DestroyMeshContainer(
	LPD3DXMESHCONTAINER pMeshContainerBase)
{
	delete[] pMeshContainerBase->Name;
	delete pMeshContainerBase;

    return S_OK;
}

}
}
}