#include "Stdafx.h"
#include "Triangle.h"

namespace FableMod
{
namespace Gfx
{

Triangle::Triangle()
{
    // no initialization for efficiency
}

Triangle::Triangle(
	const D3DXVECTOR3& v1,
	const D3DXVECTOR3& v2,
	const D3DXVECTOR3& v3) :
m_vOrigin(v1),
m_vEdge0(v2-v1),
m_vEdge1(v3-v1)
{
}

}
}