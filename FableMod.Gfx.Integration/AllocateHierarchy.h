#pragma once

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

class AllocateHierarchy : public ID3DXAllocateHierarchy
{
public:	
	
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);

	STDMETHOD(CreateMeshContainer)(
		THIS_ LPCSTR Name, 
		CONST D3DXMESHDATA * pMeshData, 
		CONST D3DXMATERIAL * pMaterials, 
		CONST D3DXEFFECTINSTANCE * pEffectInstances,
		DWORD NumMaterials, 
		CONST DWORD * pAdjacency, 
		LPD3DXSKININFO pSkinInfo, 
		LPD3DXMESHCONTAINER * ppNewMeshContainer);

	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);

	STDMETHOD(DestroyMeshContainer)(
		THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree);
};

}
}
}