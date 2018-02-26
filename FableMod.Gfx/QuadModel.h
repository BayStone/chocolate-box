#pragma once

#include "D3DModel.h"

namespace FableMod
{
namespace Gfx
{

class GFX_API QuadModel : public D3DModel
{
public:

	QuadModel(Texture* pTexture, FLOAT fSize);

	virtual ~QuadModel();

	virtual void Render(Mesh* pMesh, Camera* pCamera, bool bSimple);

	virtual bool AlphaRequired() const;

	Texture* GetTexture() { return m_spTexture; }

protected:

	TexturePtr m_spTexture;
	
	static D3DMATERIAL9 sm_Material;
	static LPDIRECT3DVERTEXBUFFER9 sm_pVB;
	static LPDIRECT3DINDEXBUFFER9 sm_pIB;
};

GFX_DECLARE_SMARTPOINTER(QuadModel);

}
}