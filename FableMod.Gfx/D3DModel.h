#pragma once

#include "Model.h"
#include "Texture.h"

namespace FableMod
{
namespace Gfx
{

class GFX_API D3DModel : public Model
{
public:

	D3DModel();

	virtual ~D3DModel();

	virtual void Dispose();

	void Create(
		unsigned int uiFVF,
		unsigned int uiVertices,
		unsigned int uiFaces);

	virtual void Render(Mesh* pMesh, Camera* pCamera, bool bSimple);
	
	void Reset();

	unsigned int AddVertex();

	void AddFace(
		unsigned int uiVertex0,
		unsigned int uiVertex1,
		unsigned int uiVertex2);

	void LockVertices();

	void UnlockVertices();

	void SetVertex(unsigned int uiIndex, const D3DXVECTOR3& vVertex);

	void SetNormal(unsigned int uiIndex, const D3DXVECTOR3& vNormal);

	void GetVertex(unsigned int uiIndex, D3DXVECTOR3& rvVertex);

	void SetTextureCoordinate(
		unsigned char ucSet, 
		unsigned int uiIndex, 
		const D3DXVECTOR2& vUV);

	void SetDiffuse(unsigned int uiIndex, const D3DCOLOR& Color);

	void SetFace(
		unsigned int uiIndex,
		unsigned int uiVertex0,
		unsigned int uiVertex1,
		unsigned int uiVertex2);

	void LockFaces();

	void UnlockFaces();

	virtual unsigned int GetNumVertices() const { return m_uiNumVertices; }

	virtual unsigned int GetNumFaces() const { return m_uiNumFaces; }

	virtual unsigned int GetMaxVertices() const { return m_uiMaxVertices; }

	virtual unsigned int GetMaxFaces() const { return m_uiMaxFaces; }

	virtual unsigned int GetVertexSize() const;

protected:

	void AllocateVertices(unsigned int uiNumVertices);

	void AllocateFaces(unsigned int uiNumFaces);

	virtual void GetVertexBuffer(LPDIRECT3DVERTEXBUFFER9* ppVB);
	
	virtual void GetIndexBuffer(LPDIRECT3DINDEXBUFFER9* ppIB);

	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;
	
	unsigned int m_uiNumVertices;
	unsigned int m_uiMaxVertices;
	unsigned int m_uiVertexGrow;

	unsigned int m_uiNumFaces;
	unsigned int m_uiMaxFaces;
	unsigned int m_uiFaceGrow;

	unsigned int m_uiFVF;
	unsigned int m_uiVertexSize;

	unsigned char m_ucTexCoordSets;

	unsigned char* m_pucVertices;
	unsigned char* m_pucNormals;
	unsigned char* m_pucDiffuse;
	unsigned char* m_apucTexCoords[8];
	unsigned char* m_pucIndices;
};

GFX_DECLARE_SMARTPOINTER(D3DModel);

}
}