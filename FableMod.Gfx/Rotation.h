#pragma once

#include <windows.h>
#include <d3dx9.h>

namespace FableMod
{
namespace Gfx
{

bool MatrixToYawPitchRoll(
	const D3DXMATRIX& m,
	FLOAT& rfYAngle, FLOAT& rfPAngle, FLOAT& rfRAngle);

bool MatrixToPitchYawRoll(
	const D3DXMATRIX& m,
	FLOAT& rfYAngle, FLOAT& rfPAngle, FLOAT& rfRAngle);

}
}