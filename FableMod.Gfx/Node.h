#pragma once

#include "Vector.h"
#include "GfxConf.h"
#include "Spatial.h"

namespace FableMod
{
namespace Gfx
{

typedef Vector<SpatialPtr> SpatialVector;

/// Node represents the scene graph class that can contain children.
class GFX_API Node : public Spatial
{
public:

	Node();

	virtual ~Node();

	void Dispose();

	virtual void RemoveChildren();

	virtual void AttachChild(Spatial* pChild);

	void DetachChild(Spatial* pChild);

	virtual void SetAlpha(FLOAT fAlpha);

	virtual void Update(bool bInit = true);

	virtual bool GetBox(AxisBox* pBox);

	virtual Spatial* Copy() const;

	virtual void Pick(
		const D3DXVECTOR3& vOrigin, 
		const D3DXVECTOR3& vDirection,
		PickData& rData);

	int NumChildren() const;

	Spatial* GetChildAt(int index) const;

	virtual bool IsLeaf() const;

protected:

	virtual void OnGetRenderList(Camera* Camera);

	void UpdateChildren();

	SpatialVector m_Children;
};

GFX_DECLARE_SMARTPOINTER(Node);

}
}