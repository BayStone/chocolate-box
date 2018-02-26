#pragma once

#include "GfxConf.h"
#include "AxisBox.h"
#include "RootObject.h"

namespace FableMod
{
namespace Gfx
{

class Mesh;
class Camera;
class BoundSphere;

/// Base class for models. Models are separated from the 
/// scenegraph mesh object so they can be easily shared.
class GFX_API Model : public RootObject
{
public:

	virtual void Dispose();

	virtual void Render(Mesh* pMesh, Camera* pCamera, bool bSimple = false) = 0;

	virtual void Pick(
		class Mesh* pMesh,
		const D3DXVECTOR3& vOrigin, 
		const D3DXVECTOR3& vDirection,
		struct PickData& rData);

	void CreateFaceNormals();
	void DisposeFaceNormals();

	void CreateBoundSpheres();
	void DisposeBoundSpheres();
	
	virtual void ComputeBoundSphere(BoundSphere* pSphere);

	virtual void ComputeAxisBox(AxisBox* pBox, const D3DXMATRIX* pmatWorld);

	virtual unsigned int GetNumVertices() const = 0;

	virtual unsigned int GetNumFaces() const = 0;

	virtual unsigned int GetVertexSize() const = 0;

	virtual bool AlphaRequired() const { return false; }

protected:

	Model();

	virtual void GetVertexBuffer(LPDIRECT3DVERTEXBUFFER9* ppVB) = 0;
	
	virtual void GetIndexBuffer(LPDIRECT3DINDEXBUFFER9* ppIB) = 0;
	
	D3DXVECTOR3* m_avFaceNormals;
	BoundSphere* m_aBoundSpheres;
	unsigned int m_uiNumBoundSpheres;
	unsigned int m_uiNumFaceNormals;
};

GFX_DECLARE_SMARTPOINTER(Model);

}
}