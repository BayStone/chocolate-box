#pragma once

#include <windows.h>
#include <d3dx9.h>
#include "Triangle.h"

namespace FableMod
{
namespace Gfx
{

bool RayTriIntersection(
	const D3DXVECTOR3& vOrigin, 
	const D3DXVECTOR3& vDirection, 
	const Triangle& Triangle,
	D3DXVECTOR3& rvPoint,
	FLOAT* pfDist = 0);

}
}