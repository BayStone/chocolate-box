#pragma once

#include "Camera.h"
#include "OctTreeNode.h"
#include "GfxConf.h"

namespace FableMod
{
namespace Gfx
{

/// OctTree represents the root node of an octtree.
class GFX_API OctTree : public OctTreeNode
{
public:

	OctTree();

	~OctTree();

	void AttachChild(Spatial* pChild);

	void UpdateTree();

protected:

	SpatialVector m_UpdateObjects;
};

}
}