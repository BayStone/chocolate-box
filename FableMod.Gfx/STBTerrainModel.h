#pragma once

#include "D3DModel.h"

namespace FableMod
{
namespace Gfx
{

class GFX_API STBTerrainModel : public D3DModel
{
public:

	STBTerrainModel();

	virtual ~STBTerrainModel();

	void Create(
		D3DXVECTOR3* pVertices,
		D3DXVECTOR3* pNormals,
		D3DXVECTOR2* pUVs,
		unsigned int uiVertexCount,
		unsigned int* pFaces,
		unsigned int uiFaceCount);

	virtual void Render(Mesh* pMesh, Camera* pCamera, bool bSimple);
	
	void SetColor(D3DCOLOR Color);

	void SetTexture(Texture* Texture);

protected:

	D3DMATERIAL9 m_Material;
	TexturePtr m_Texture;
};

GFX_DECLARE_SMARTPOINTER(STBTerrainModel);

}
}