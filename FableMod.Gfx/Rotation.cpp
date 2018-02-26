#include "Stdafx.h"
#include "Rotation.h"

namespace FableMod
{
namespace Gfx
{

bool MatrixToYawPitchRoll(
	const D3DXMATRIX& m,
	FLOAT& rfYAngle, FLOAT& rfPAngle, FLOAT& rfRAngle)
{
  	static const FLOAT HALF_PI = D3DX_PI/2;

	// YXZ

	rfPAngle = asin(-m._23);

    if (rfPAngle < HALF_PI)
    {
        if (rfPAngle > -HALF_PI)
        {
            rfYAngle = atan2(m._13,m._33);
            rfRAngle = -atan2(m._21,m._22);
            return true;
        }
        else
        {
            // WARNING. Not a unique solution.
			// Example: 200 degrees of rotation is the same as -160.
            float fRmY = atan2(-m._12,m._11);
            rfRAngle = 0.0;  // any angle works
            rfYAngle = rfRAngle-fRmY;
            return false;
        }
    }
    else
    {
        // WARNING.  Not a unique solution.
        float fRpY = atan2(-m._12,m._11);
        rfRAngle = 0.0;  // any angle works
        rfYAngle = fRpY-rfRAngle;
        return false;
    }
	/*
	// ZXY

	rfPAngle = asin(m._32);
    if ( rfPAngle < HALF_PI )
    {
        if ( rfPAngle > -HALF_PI )
        {
			rfYAngle = atan2(-m._12,m._22);
			rfRAngle = atan2(-m._31,m._33);
            return true;
        }
        else
        {
            // WARNING.  Not a unique solution.
            float fRmY = atan2(m._13,m._11);
            rfRAngle = 0.0;  // any angle works
            rfYAngle = rfRAngle - fRmY;
            return false;
        }
    }
    else
    {
        // WARNING.  Not a unique solution.
		float fRpY = atan2(m._13,m._11);
        rfRAngle = 0.0;  // any angle works
        rfYAngle = fRpY - rfRAngle;
        return false;
    }
	*/
}

bool MatrixToPitchYawRoll(
	const D3DXMATRIX& m,
	FLOAT& rfYAngle, FLOAT& rfPAngle, FLOAT& rfRAngle)
{
	rfPAngle = asin(m._13);

    if ( rfPAngle < D3DX_PI/2 )
    {
        if ( rfPAngle > -D3DX_PI/2 )
        {
			rfYAngle = atan2(-m._23, m._33);
			rfRAngle = atan2(-m._12, m._11);
            return true;
        }
        else
        {
            // WARNING.  Not a unique solution.
			float fRmY = atan2(m._21, m._22);
            rfRAngle = 0.0;  // any angle works
            rfYAngle = rfRAngle - fRmY;
            return false;
        }
    }
    else
    {
        // WARNING.  Not a unique solution.
		float fRpY = atan2(m._21, m._22);
        rfRAngle = 0.0;  // any angle works
        rfYAngle = fRpY - rfRAngle;
        return false;
    }
}

}
}