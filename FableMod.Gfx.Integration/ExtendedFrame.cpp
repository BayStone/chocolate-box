#include "Stdafx.h"
#include "ExtendedFrame.h"

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

void D3DXEXTENDEDFRAME::Destroy()
{
	GFX_SAFE_ARRAYDELETE(Name);
	
	if (pMeshContainer)
	{
		if (pMeshContainer->pMaterials)
		{
			for (int i = 0; i < pMeshContainer->NumMaterials; ++i)
			{
				GFX_SAFE_ARRAYDELETE(
					pMeshContainer->pMaterials[i].pTextureFilename);
			}

			delete[] pMeshContainer->pMaterials;
		}

		GFX_SAFE_RELEASE(pMeshContainer->MeshData.pMesh);

		GFX_SAFE_ARRAYDELETE(pMeshContainer->Name);

		GFX_SAFE_RELEASE(pMeshContainer->pSkinInfo);

		delete pMeshContainer;
		pMeshContainer = 0;
	}

	if (pSkinnedMesh)
	{
		pSkinnedMesh->Release();
		pSkinnedMesh = 0;
	}

	spTexture = 0;
	spBumpMap = 0;
	spReflectMap = 0;

	GFX_SAFE_ARRAYDELETE(BoneMatrices);

	if (pFrameFirstChild)
	{
		((D3DXEXTENDEDFRAME*)pFrameFirstChild)->Destroy();
		delete pFrameFirstChild;
		pFrameFirstChild = 0;
	}

	if (pFrameSibling)
	{
		((D3DXEXTENDEDFRAME*)pFrameSibling)->Destroy();
		delete pFrameSibling;
		pFrameSibling = 0;
	}
}

}
}
}