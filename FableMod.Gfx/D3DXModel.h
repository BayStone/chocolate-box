#pragma once

#include "Model.h"
#include "Texture.h"

namespace FableMod
{
namespace Gfx
{

class Mesh;

/// Model class that uses the D3DXMESH class.
class GFX_API D3DXModel : public Model
{
public:

	struct GFX_API SMaterial
	{
		D3DMATERIAL9 D3DMaterial;
		TexturePtr spTexture;
		bool bAlpha;
	};

	D3DXModel();

	virtual ~D3DXModel();

	void Load(LPCTSTR pFilename);

	virtual void Dispose();

	virtual void Render(Mesh* pMesh, Camera* pCamera, bool bSimple);

	void BuildSphere(FLOAT fRadius, D3DXCOLOR Color);

	void Build(
		LPD3DXMESH pMesh,
		SMaterial* pMaterials, 
		DWORD dwNumMaterials);

	unsigned int GetNumVertices() const { return m_pMesh->GetNumVertices(); }

	unsigned int GetNumFaces() const { return m_pMesh->GetNumFaces(); }

	unsigned int GetVertexSize() const { return m_pMesh->GetNumBytesPerVertex(); }

	virtual bool AlphaRequired() const;

protected:

	void DisposeMaterials();

	void SimpleRender();

	virtual void GetVertexBuffer(LPDIRECT3DVERTEXBUFFER9* ppVB);
	
	virtual void GetIndexBuffer(LPDIRECT3DINDEXBUFFER9* ppIB);

    LPD3DXMESH m_pMesh;
	SMaterial* m_aMaterials;
	DWORD m_dwNumMaterials;
	bool m_bAlphaRequired;
};

GFX_DECLARE_SMARTPOINTER(D3DXModel);

}
}