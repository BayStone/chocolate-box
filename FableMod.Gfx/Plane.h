#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include "GfxConf.h"

namespace FableMod
{
namespace Gfx
{

class GFX_API Plane
{
public:
    
	Plane();
	Plane(const D3DXVECTOR3& rNormal, FLOAT fConstant);
	Plane(const D3DXVECTOR3& rNormal, const D3DXVECTOR3& rPoint);
	Plane(const D3DXVECTOR3& rPoint0, const D3DXVECTOR3& rPoint1,
		   const D3DXVECTOR3& rPoint2);
	
	D3DXVECTOR3& Normal();
	const D3DXVECTOR3& Normal() const;

	FLOAT& Constant();
	const FLOAT& Constant() const;

	enum ESide
	{
		NO_SIDE,
		POSITIVE_SIDE,
		NEGATIVE_SIDE
	};

	ESide WhichSide(const D3DXVECTOR3& rPoint) const;

	FLOAT DistanceTo(const D3DXVECTOR3& rPoint) const;

protected:

	// Plane is Dot(m_normal,(x,y,z)) = m_fConstant
	D3DXVECTOR3	m_vNormal;
	FLOAT		m_fConstant;
};

#include "plane.inl"

}
}