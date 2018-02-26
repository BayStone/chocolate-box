#pragma once

#include "Camera.h"
#include "Node.h"
#include "GfxConf.h"

namespace FableMod
{
namespace Gfx
{

/// OctTreeNode implements basic OctTree functionality.
class GFX_API OctTreeNode : public Node
{
public:

	OctTreeNode();

	~OctTreeNode();

	virtual void RemoveChildren();

	void Build(
		const D3DXVECTOR3& vMin, 
		const D3DXVECTOR3& vMax,
		const D3DXVECTOR3& vMinSize);

	bool IsInside(Spatial* pSpatial);

	bool AttachToTree(Spatial* pChild);

	virtual void Update(bool bInit = true);

#ifdef _DEBUG
	void RenderBound(int iLevel = 0);
#endif

protected:

	virtual void Pick(
		const D3DXVECTOR3& vOrigin, 
		const D3DXVECTOR3& vDirection,
		PickData& rData);

	virtual void OnGetRenderList(Camera* Camera);

	void GetUpdatable(SpatialVector& rObjects);

	OctTreeNode* m_apOctNodes[8];
	unsigned int m_uiCount;
	D3DXVECTOR3 m_vMin;
	D3DXVECTOR3 m_vMax;
};

}
}