#pragma once

#include "Node.h"

namespace FableMod
{
namespace Gfx
{

class GFX_API LODItem
{
public:
	
	FLOAT m_fMin;
	FLOAT m_fMax;
};

/// LOD node implements basic discrete LOD functionality.
class GFX_API LODNode : public Node
{
public:

	LODNode();

	virtual ~LODNode();

	virtual void Update();

	virtual void Dispose();

	virtual Spatial* Copy() const;

protected:

	virtual void OnGetRenderList(Camera* Camera);

	BoundSphere* m_pBound;
};

}
}