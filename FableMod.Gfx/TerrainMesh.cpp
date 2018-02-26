#include "Stdafx.h"
#include "TerrainMesh.h"
#include "Manager.h"

namespace FableMod
{
namespace Gfx
{

TerrainMesh::TerrainMesh()
{
	//RenderFlags() &= ~MRF_DISTANCE_TO_BOX;
}

void TerrainMesh::Render(Camera* pCamera)
{
	RenderFlags() &= ~MRF_HIGHLIGHT;

	/*
	if (DistanceToCamera() > 1000)
	{
		IDirect3DDevice9* pDevice = Manager::GetDevice();

		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		pDevice->SetTransform(D3DTS_WORLD, &GetWorldTransform());

		m_pModel->Render(this, pCamera, true);

		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
	else
	{
	*/
		Mesh::Render(pCamera);
	//}
}

}
}
