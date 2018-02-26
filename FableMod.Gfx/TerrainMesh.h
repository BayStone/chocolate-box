#pragma once

#include "Mesh.h"

namespace FableMod
{
namespace Gfx
{

/// TerrainMesh represents one block of terrain that can be
/// culled from the view frustum, etc.
class GFX_API TerrainMesh : public Mesh
{
public:
	
	TerrainMesh();

	virtual void Render(Camera* pCamera);

protected:

};

}
}