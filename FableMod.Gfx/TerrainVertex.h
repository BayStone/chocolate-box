#pragma once

#include "GfxConf.h"

namespace FableMod
{
namespace Gfx
{
		
/// TerrainVertex controls whether or not a terrain vertex is meant
/// to be enabled or disabled to what other vertices it's connected to.
class GFX_API TerrainVertex
{
public:
    
	TerrainVertex();

    void SetDependent(int i, TerrainVertex* pkDependent);
    TerrainVertex* GetDependent(int i);
    bool GetEnabled() const;
    void Enable();
    void Disable();

protected:
    TerrainVertex* m_apDependent[2];
    bool m_bEnabled;
};

#include "TerrainVertex.inl"

}
}
