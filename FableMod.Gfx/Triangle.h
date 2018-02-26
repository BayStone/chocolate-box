#pragma once

#include <d3dx9.h>
#include "GfxConf.h"

namespace FableMod
{
namespace Gfx
{

/// Triangle implements a basic triangle structure.
class GFX_API Triangle
{
public:
    Triangle();

    Triangle(
		const D3DXVECTOR3& v1,
		const D3DXVECTOR3& v2,
		const D3DXVECTOR3& v3);

	D3DXVECTOR3& Origin();
    const D3DXVECTOR3& Origin() const;

    D3DXVECTOR3& Edge0();
    const D3DXVECTOR3& Edge0() const;

    D3DXVECTOR3& Edge1();
    const D3DXVECTOR3& Edge1() const;

    D3DXVECTOR3 Normal() const;
    
protected:
	
    D3DXVECTOR3 m_vOrigin;
    D3DXVECTOR3 m_vEdge0;
    D3DXVECTOR3 m_vEdge1;
};

#include "Triangle.inl"

}
}
