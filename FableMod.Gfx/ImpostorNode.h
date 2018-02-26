#pragma once

#include "Node.h"
#include "Mesh.h"
#include "Texture.h"

namespace FableMod
{
namespace Gfx
{

class GFX_API ImpostorNode : public Node
{
public:

	ImpostorNode();

	virtual ~ImpostorNode();

	virtual void Update(bool bInit = true);

	virtual void Dispose();

	virtual Spatial* Copy() const;

protected:

	void RenderToTexture(Camera* pCamera);

	virtual void OnGetRenderList(Camera* Camera);

	TexturePtr m_spTexture;
	MeshPtr m_spMesh;
	unsigned char m_ucUpdate;
	
	static D3DXMATRIX sm_matProj;
	static IDirect3DSurface9* sm_pDepthStencil;
	static FLOAT sm_fSizeRatio;
	static unsigned char sm_ucUpdateFrequency;
	static bool sm_bEnabled;
	static int sm_iTextureSize;
};

GFX_DECLARE_SMARTPOINTER(ImpostorNode);

}
}