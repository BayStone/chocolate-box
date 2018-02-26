#include "stdafx.h"
#include "plane.h"

namespace FableMod
{
namespace Gfx
{

Plane::Plane () :
m_vNormal(0,0,0),
m_fConstant(0.0)
{
}

//----------------------------------------------------------------------------

Plane::Plane(const D3DXVECTOR3& rNormal, FLOAT fConstant) :
m_vNormal(rNormal),
m_fConstant(fConstant)
{
}

//----------------------------------------------------------------------------

Plane::Plane(const D3DXVECTOR3& rNormal, const D3DXVECTOR3& rPoint) :
m_vNormal(rNormal),
m_fConstant(D3DXVec3Dot(&rNormal,&rPoint))
{
}

//----------------------------------------------------------------------------

Plane::Plane(const D3DXVECTOR3& rPoint0, const D3DXVECTOR3& rPoint1,
			   const D3DXVECTOR3& rPoint2)
{
    D3DXVECTOR3 vEdge1 = rPoint1-rPoint0;
    D3DXVECTOR3 vEdge2 = rPoint2-rPoint0;
    
	D3DXVECTOR3 vN1;
	D3DXVec3Normalize(&vN1,&vEdge1);

	D3DXVECTOR3 vN2;
	D3DXVec3Normalize(&vN2,&vEdge2);

	D3DXVec3Cross(&m_vNormal,&vN1,&vN2);
    
	m_fConstant = D3DXVec3Dot(&m_vNormal,&rPoint0);
}

//----------------------------------------------------------------------------

FLOAT Plane::DistanceTo(const D3DXVECTOR3& rPoint) const
{
    return D3DXVec3Dot(&m_vNormal,&rPoint)-m_fConstant;
}

//----------------------------------------------------------------------------

Plane::ESide Plane::WhichSide(const D3DXVECTOR3& rPoint) const
{
    FLOAT fDistance = DistanceTo(rPoint);

    if (fDistance < 0.0)
        return Plane::NEGATIVE_SIDE;

    if (fDistance > 0.0)
        return Plane::POSITIVE_SIDE;

    return Plane::NO_SIDE;
}

}
}