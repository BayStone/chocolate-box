#include "Stdafx.h"
#include <windows.h>
#include <d3dx9.h>
#include "Triangle.h"

namespace FableMod
{
namespace Gfx
{

static const FLOAT gs_fTolerance = 1e-05;

// Macro to decrease the code a bit... 
// ugly but keep things simple.
#define MINIMIZE_SQRDEST \
	if (fSqrDist0 < fSqrDist) \
	{ \
		fSqrDist = fSqrDist0; \
		fR = fR0; \
		fS = fS0; \
		fT = fT0; \
	}
	
//----------------------------------------------------------------------------

static FLOAT VecTriSqrDistance(
	const D3DXVECTOR3& vPoint, 
	const Triangle& Triangle,
    FLOAT* pfSParam,
	FLOAT* pfTParam)
{
    D3DXVECTOR3 vDiff(Triangle.Origin()-vPoint);
  
	FLOAT fA00 = D3DXVec3LengthSq(&Triangle.Edge0());
    FLOAT fA01 = D3DXVec3Dot(&Triangle.Edge0(),&Triangle.Edge1());
    FLOAT fA11 = D3DXVec3LengthSq(&Triangle.Edge1());
    FLOAT fB0 = D3DXVec3Dot(&vDiff,&Triangle.Edge0());
    FLOAT fB1 = D3DXVec3Dot(&vDiff,&Triangle.Edge1());
    FLOAT fC = D3DXVec3LengthSq(&vDiff);
    FLOAT fDet = fabs(fA00*fA11-fA01*fA01);
    FLOAT fS = fA01*fB1-fA11*fB0;
    FLOAT fT = fA01*fB0-fA00*fB1;
    FLOAT fSqrDist;

    if (fS + fT <= fDet)
    {
        if (fS < 0.0)
        {
            if (fT < 0.0)  // region 4
            {
                if (fB0 < 0.0)
                {
                    fT = 0.0;

                    if (-fB0 >= fA00)
                    {
                        fS = 1.0;
                        fSqrDist = fA00+2.0*fB0+fC;
                    }
                    else
                    {
                        fS = -fB0/fA00;
                        fSqrDist = fB0*fS+fC;
                    }
                }
                else
                {
                    fS = 0.0;

                    if (fB1 >= 0.0)
                    {
                        fT = 0.0;
                        fSqrDist = fC;
                    }
                    else if (-fB1 >= fA11)
                    {
                        fT = 1.0;
                        fSqrDist = fA11+2.0*fB1+fC;
                    }
                    else
                    {
                        fT = -fB1/fA11;
                        fSqrDist = fB1*fT+fC;
                    }
                }
            }
            else  // region 3
            {
                fS = 0.0;

                if (fB1 >= 0.0)
                {
                    fT = 0.0;
                    fSqrDist = fC;
                }
                else if (-fB1 >= fA11)
                {
                    fT = 1;
                    fSqrDist = fA11+2.0*fB1+fC;
                }
                else
                {
                    fT = -fB1/fA11;
                    fSqrDist = fB1*fT+fC;
                }
            }
        }
        else if (fT < 0.0)  // region 5
        {
            fT = 0.0;

            if (fB0 >= 0.0)
            {
                fS = 0.0;
                fSqrDist = fC;
            }
            else if (-fB0 >= fA00)
            {
                fS = 1.0;
                fSqrDist = fA00+2.0*fB0+fC;
            }
            else
            {
                fS = -fB0/fA00;
                fSqrDist = fB0*fS+fC;
            }
        }
        else  // region 0
        {
            // minimum at interior point
            FLOAT fInvDet = 1.0/fDet;
            fS *= fInvDet;
            fT *= fInvDet;
            fSqrDist = fS*(fA00*fS+fA01*fT+2.0*fB0) +
                fT*(fA01*fS+fA11*fT+2.0*fB1)+fC;
        }
    }
    else
    {
        FLOAT fTmp0, fTmp1, fNumer, fDenom;

        if (fS < 0.0)  // region 2
        {
            fTmp0 = fA01 + fB0;
            fTmp1 = fA11 + fB1;

            if (fTmp1 > fTmp0)
            {
                fNumer = fTmp1 - fTmp0;
                fDenom = fA00-2.0*fA01+fA11;

                if (fNumer >= fDenom)
                {
                    fS = 1.0;
                    fT = 0.0;
                    fSqrDist = fA00+2.0*fB0+fC;
                }
                else
                {
                    fS = fNumer/fDenom;
                    fT = 1.0 - fS;
                    fSqrDist = fS*(fA00*fS+fA01*fT+2.0*fB0) +
                        fT*(fA01*fS+fA11*fT+2.0*fB1)+fC;
                }
            }
            else
            {
                fS = 0.0;
                
				if (fTmp1 <= 0.0)
                {
                    fT = 1.0;
                    fSqrDist = fA11+2.0*fB1+fC;
                }
                else if (fB1 >= 0.0)
                {
                    fT = 0.0;
                    fSqrDist = fC;
                }
                else
                {
                    fT = -fB1/fA11;
                    fSqrDist = fB1*fT+fC;
                }
            }
        }
        else if (fT < 0.0)  // region 6
        {
            fTmp0 = fA01 + fB1;
            fTmp1 = fA00 + fB0;

            if (fTmp1 > fTmp0)
            {
                fNumer = fTmp1 - fTmp0;
                fDenom = fA00-2.0*fA01+fA11;

                if (fNumer >= fDenom)
                {
                    fT = 1.0;
                    fS = 0.0;
                    fSqrDist = fA11+2.0*fB1+fC;
                }
                else
                {
                    fT = fNumer/fDenom;
                    fS = 1.0 - fT;
                    fSqrDist = fS*(fA00*fS+fA01*fT+2.0*fB0) +
                        fT*(fA01*fS+fA11*fT+2.0*fB1)+fC;
                }
            }
            else
            {
                fT = 0.0;

                if (fTmp1 <= 0.0)
                {
                    fS = 1.0;
                    fSqrDist = fA00+2.0*fB0+fC;
                }
                else if (fB0 >= 0.0)
                {
                    fS = 0.0;
                    fSqrDist = fC;
                }
                else
                {
                    fS = -fB0/fA00;
                    fSqrDist = fB0*fS+fC;
                }
            }
        }
        else  // region 1
        {
            fNumer = fA11 + fB1 - fA01 - fB0;
            
			if (fNumer <= 0.0)
            {
                fS = 0.0;
                fT = 1.0;
                fSqrDist = fA11+2.0*fB1+fC;
            }
            else
            {
                fDenom = fA00-2.0*fA01+fA11;
                
				if (fNumer >= fDenom)
                {
                    fS = 1.0;
                    fT = 0.0;
                    fSqrDist = fA00+2.0*fB0+fC;
                }
                else
                {
                    fS = fNumer/fDenom;
                    fT = 1.0 - fS;
                    fSqrDist = fS*(fA00*fS+fA01*fT+2.0*fB0) +
                        fT*(fA01*fS+fA11*fT+2.0*fB1)+fC;
                }
            }
        }
    }

    if (pfSParam)
        *pfSParam = fS;

    if (pfTParam)
        *pfTParam = fT;

    return fabs(fSqrDist);
}

//----------------------------------------------------------------------------

FLOAT RaySegSqrDistance(
	const D3DXVECTOR3& vRayOrigin,
	const D3DXVECTOR3& vRayDirection,
	const D3DXVECTOR3& vSegOrigin,
	const D3DXVECTOR3& vSegDirection,
	FLOAT* pfRayP, 
	FLOAT* pfSegP)
{
    D3DXVECTOR3 vDiff = vRayOrigin-vSegOrigin;
    FLOAT fA00 = D3DXVec3LengthSq(&vRayDirection);
    FLOAT fA01 = D3DXVec3Dot(&(-vRayDirection),&vSegDirection);
    FLOAT fA11 = D3DXVec3LengthSq(&vSegDirection);
    FLOAT fB0 = D3DXVec3Dot(&vDiff,&vRayDirection);
    FLOAT fC = D3DXVec3LengthSq(&vDiff);
    FLOAT fDet = fabs(fA00*fA11-fA01*fA01);
    FLOAT fB1, fS, fT, fSqrDist, fTmp;

    if (fDet >= gs_fTolerance)
    {
        // ray and segment are not parallel
        fB1 = D3DXVec3Dot(&(-vDiff),&vSegDirection);
        fS = fA01*fB1-fA11*fB0;
        fT = fA01*fB0-fA00*fB1;

        if (fS >= 0.0)
        {
            if (fT >= 0.0)
            {
                if (fT <= fDet)  // region 0
                {
                    // minimum at interior points of ray and segment
                    FLOAT fInvDet = 1.0/fDet;
                    fS *= fInvDet;
                    fT *= fInvDet;
                    fSqrDist = fS*(fA00*fS+fA01*fT+2.0*fB0) +
                        fT*(fA01*fS+fA11*fT+2.0*fB1)+fC;
                }
                else  // region 1
                {
                    fT = 1.0;
                    if (fB0 >= -fA01)
                    {
                        fS = 0.0;
                        fSqrDist = fA11+2.0*fB1+fC;
                    }
                    else
                    {
                        fTmp = fA01 + fB0;
                        fS = -fTmp/fA00;
                        fSqrDist = fTmp*fS+fA11+2.0*fB1+fC;
                    }
                }
            }
            else  // region 5
            {
                fT = 0.0;
                if (fB0 >= 0.0)
                {
                    fS = 0.0;
                    fSqrDist = fC;
                }
                else
                {
                    fS = -fB0/fA00;
                    fSqrDist = fB0*fS+fC;
                }
            }
        }
        else
        {
            if (fT <= 0.0)  // region 4
            {
                if (fB0 < 0.0)
                {
                    fS = -fB0/fA00;
                    fT = 0.0;
                    fSqrDist = fB0*fS+fC;
                }
                else
                {
                    fS = 0.0;
                    if (fB1 >= 0.0)
                    {
                        fT = 0.0;
                        fSqrDist = fC;
                    }
                    else if (-fB1 >= fA11)
                    {
                        fT = 1.0;
                        fSqrDist = fA11+2.0*fB1+fC;
                    }
                    else
                    {
                        fT = -fB1/fA11;
                        fSqrDist = fB1*fT+fC;
                    }
                }
            }
            else if (fT <= fDet)  // region 3
            {
                fS = 0.0;
                if (fB1 >= 0.0)
                {
                    fT = 0.0;
                    fSqrDist = fC;
                }
                else if (-fB1 >= fA11)
                {
                    fT = 1.0;
                    fSqrDist = fA11+2.0*fB1+fC;
                }
                else
                {
                    fT = -fB1/fA11;
                    fSqrDist = fB1*fT+fC;
                }
            }
            else  // region 2
            {
                fTmp = fA01+fB0;
                if (fTmp < 0.0)
                {
                    fS = -fTmp/fA00;
                    fT = 1.0;
                    fSqrDist = fTmp*fS+fA11+2.0*fB1+fC;
                }
                else
                {
                    fS = 0.0;
                    if (fB1 >= 0.0)
                    {
                        fT = 0.0;
                        fSqrDist = fC;
                    }
                    else if (-fB1 >= fA11)
                    {
                        fT = 1.0;
                        fSqrDist = fA11+2*fB1+fC;
                    }
                    else
                    {
                        fT = -fB1/fA11;
                        fSqrDist = fB1*fT+fC;
                    }
                }
            }
        }
    }
    else
    {
        // ray and segment are parallel
        if (fA01 > 0.0)
        {
            // opposite direction vectors
            fT = 0.0;

            if (fB0 >= 0.0)
            {
                fS = 0.0;
                fSqrDist = fC;
            }
            else
            {
                fS = -fB0/fA00;
                fSqrDist = fB0*fS+fC;
            }
        }
        else
        {
            // same direction vectors
            fB1 = D3DXVec3Dot(&(-vDiff),&vSegDirection);
            fT = 1.0;
            fTmp = fA01+fB0;

            if (fTmp >= 0.0)
            {
                fS = 0.0;
                fSqrDist = fA11+2.0*fB1+fC;
            }
            else
            {
                fS = -fTmp/fA00;
                fSqrDist = fTmp*fS+fA11+2.0*fB1+fC;
            }
        }
    }

    if (pfRayP)
        *pfRayP = fS;

    if (pfSegP)
        *pfSegP = fT;

    return fabs(fSqrDist);
}

//----------------------------------------------------------------------------

/// Calculate the square distance between a triangle and a ray.
FLOAT RayTriSqrDistance(
	const D3DXVECTOR3& vOrigin, 
	const D3DXVECTOR3& vDirection, 
	const Triangle& Triangle,
    FLOAT* pfRayP,
	FLOAT* pfTriP0,
	FLOAT* pfTriP1)
{
	D3DXVECTOR3 vDiff(Triangle.Origin()-vOrigin);
    FLOAT fA00 = D3DXVec3LengthSq(&vDirection);
    FLOAT fA01 = D3DXVec3Dot(&(-vDirection),&Triangle.Edge0());
    FLOAT fA02 = D3DXVec3Dot(&(-vDirection),&Triangle.Edge1());
    FLOAT fA11 = D3DXVec3LengthSq(&Triangle.Edge0());
    FLOAT fA12 = D3DXVec3Dot(&Triangle.Edge0(),&Triangle.Edge1());
    FLOAT fA22 = D3DXVec3Dot(&Triangle.Edge1(),&Triangle.Edge1());
    FLOAT fB0  = D3DXVec3Dot(&(-vDiff), &vDirection);
    FLOAT fB1  = D3DXVec3Dot(&vDiff,&Triangle.Edge0());
    FLOAT fB2  = D3DXVec3Dot(&vDiff,&Triangle.Edge1());
    FLOAT fCof00 = fA11*fA22-fA12*fA12;
    FLOAT fCof01 = fA02*fA12-fA01*fA22;
    FLOAT fCof02 = fA01*fA12-fA02*fA11;
    FLOAT fDet = fA00*fCof00+fA01*fCof01+fA02*fCof02;

    FLOAT fSqrDist, fSqrDist0, fR, fS, fT, fR0, fS0, fT0;

    if (fabs(fDet) >= gs_fTolerance)
    {
        FLOAT fCof11 = fA00*fA22-fA02*fA02;
        FLOAT fCof12 = fA02*fA01-fA00*fA12;
        FLOAT fCof22 = fA00*fA11-fA01*fA01;
        FLOAT fInvDet = 1.0/fDet;
        FLOAT fRhs0 = -fB0*fInvDet;
        FLOAT fRhs1 = -fB1*fInvDet;
        FLOAT fRhs2 = -fB2*fInvDet;

        fR = fCof00*fRhs0+fCof01*fRhs1+fCof02*fRhs2;
        fS = fCof01*fRhs0+fCof11*fRhs1+fCof12*fRhs2;
        fT = fCof02*fRhs0+fCof12*fRhs1+fCof22*fRhs2;

        if (fR <= 0.0)
        {
            if (fS+fT <= 1.0)
            {
                if (fS < 0.0)
                {
                    if (fT < 0.0)  // region 4m
                    {
                        // min on face s=0 or t=0 or r=0
                        fSqrDist = RaySegSqrDistance(
							vOrigin, vDirection,
							Triangle.Origin(), Triangle.Edge1(),
							&fR,&fT);
                        fS = 0.0;
                        fSqrDist0 = RaySegSqrDistance(
							vOrigin, vDirection,
							Triangle.Origin(), Triangle.Edge0(),
							&fR0,&fS0);
                        fT0 = 0.0;
						
                        MINIMIZE_SQRDEST
                        
						fSqrDist0 = VecTriSqrDistance(
							vOrigin,Triangle,&fS0,&fT0);
                        fR0 = 0.0;
						
                        MINIMIZE_SQRDEST
                    }
                    else  // region 3m
                    {
                        // min on face s=0 or r=0
                        fSqrDist = RaySegSqrDistance(
							vOrigin, vDirection,
							Triangle.Origin(), 
							Triangle.Edge1(),
							&fR,&fT);
                        fS = 0.0;
                        fSqrDist0 = VecTriSqrDistance(
							vOrigin,Triangle,&fS0,&fT0);
                        fR0 = 0.0;
						
                        MINIMIZE_SQRDEST
                    }
                }
                else if (fT < 0.0)  // region 5m
                {
                    // min on face t=0 or r=0
                    fSqrDist = RaySegSqrDistance(
						vOrigin, vDirection,
						Triangle.Origin(), 
						Triangle.Edge0(),
						&fR,&fS);
                    fT = 0.0;
                    fSqrDist0 = VecTriSqrDistance(
						vOrigin,Triangle,&fS0,&fT0);
                    fR0 = 0.0;
					
                    MINIMIZE_SQRDEST
                }
                else  // region 0m
                {
                    // min on face r=0
                    fSqrDist = VecTriSqrDistance(vOrigin,Triangle,&fS,&fT);
                    fR = 0.0;
                }
            }
            else
            {
                if (fS < 0.0)  // region 2m
                {
                    // min on face s=0 or s+t=1 or r=0
					fSqrDist = RaySegSqrDistance(
						vOrigin, vDirection,
						Triangle.Origin(), 
						Triangle.Edge1(),
						&fR,&fT);
                    fS = 0.0;
                    fSqrDist0 = RaySegSqrDistance(
						vOrigin, vDirection,
						Triangle.Origin()+Triangle.Edge0(), 
						Triangle.Edge1()-Triangle.Edge0(),
						&fR0,&fT0);
                    fS0 = 1.0-fT0;
					
                    MINIMIZE_SQRDEST
                    
					fSqrDist0 = VecTriSqrDistance(
						vOrigin,Triangle,&fS0,&fT0);
                    fR0 = 0.0;
					
                    MINIMIZE_SQRDEST
                }
                else if (fT < 0.0)  // region 6m
                {
                    // min on face t=0 or s+t=1 or r=0
                    fSqrDist = RaySegSqrDistance(
						vOrigin, vDirection,
						Triangle.Origin(), 
						Triangle.Edge0(),
						&fR,&fS);
                    fT = 0.0;
                    fSqrDist0 = RaySegSqrDistance(
						vOrigin, vDirection,
						Triangle.Origin()+Triangle.Edge0(), 
						Triangle.Edge0()-Triangle.Edge0(),
						&fR0,&fT0);
                    fS0 = 1.0-fT0;
					
                    MINIMIZE_SQRDEST
					
                    fSqrDist0 = VecTriSqrDistance(
						vOrigin,Triangle,&fS0,&fT0);
                    fR0 = 0.0;
					
                    MINIMIZE_SQRDEST
                }
                else  // region 1m
                {
                    // min on face s+t=1 or r=0
                    fSqrDist = RaySegSqrDistance(
						vOrigin, vDirection,
						Triangle.Origin()+Triangle.Edge0(), 
						Triangle.Edge0()-Triangle.Edge0(),
						&fR,&fT); 
					fS = 1.0-fT;
                    fSqrDist0 = VecTriSqrDistance(
						vOrigin,Triangle,&fS0,&fT0);
                    fR0 = 0.0;
					
                    MINIMIZE_SQRDEST
                }
            }
        }
        else  // fR > 0
        {
            if (fS+fT <= 1.0)
            {
                if (fS < 0.0)
                {
                    if (fT < 0.0)  // region 4p
                    {
                        // min on face s=0 or t=0
                        fSqrDist = RaySegSqrDistance(
							vOrigin, vDirection,
							Triangle.Origin(), 
							Triangle.Edge1(),
							0,&fT);
                        fS = 0.0;
                        fSqrDist0 = RaySegSqrDistance(
							vOrigin, vDirection,
							Triangle.Origin(), 
							Triangle.Edge0(),
							0,&fS0);
                        fT0 = 0.0;
						
                        if (fSqrDist0 < fSqrDist)
                        {
                            fSqrDist = fSqrDist0;
                            fS = fS0;
                            fT = fT0;
                        }
                    }
                    else  // region 3p
                    {
                        // min on face s=0
                        fSqrDist = RaySegSqrDistance(
							vOrigin, vDirection,
							Triangle.Origin(), 
							Triangle.Edge1(),
							0,&fT);
						fS = 0.0;
                    }
                }
                else if (fT < 0.0)  // region 5p
                {
                    // min on face t=0
                    fSqrDist = RaySegSqrDistance(
						vOrigin, vDirection,
						Triangle.Origin(), 
						Triangle.Edge0(),
						0,&fS);
					fT = 0.0;
                }
                else  // region 0p
                {
                    // ray intersects triangle
                    fSqrDist = 0.0;
                }
            }
            else
            {
                if (fS < 0.0)  // region 2p
                {
                    // min on face s=0 or s+t=1
                    fSqrDist = RaySegSqrDistance(
						vOrigin, vDirection,
						Triangle.Origin(), 
						Triangle.Edge1(),
						0,&fT);
					fS = 0.0;
                    fSqrDist0 = RaySegSqrDistance(
						vOrigin, vDirection,
						Triangle.Origin()+Triangle.Edge0(), 
						Triangle.Edge1()-Triangle.Edge0(),
						0,&fT0);
					fS0 = 1.0-fT0;
					
                    if (fSqrDist0 < fSqrDist)
                    {
                        fSqrDist = fSqrDist0;
                        fS = fS0;
                        fT = fT0;
                    }
                }
                else if (fT < 0.0)  // region 6p
                {
                    // min on face t=0 or s+t=1
                    fSqrDist = RaySegSqrDistance(
						vOrigin, vDirection,
						Triangle.Origin(), 
						Triangle.Edge0(),
						0,&fS);
                    fT = 0.0;
                    fSqrDist0 = RaySegSqrDistance(
						vOrigin, vDirection,
						Triangle.Origin()+Triangle.Edge0(), 
						Triangle.Edge0()-Triangle.Edge0(),
						0,&fT0);
                    fS0 = 1.0-fT0;
					
                    if (fSqrDist0 < fSqrDist)
                    {
                        fSqrDist = fSqrDist0;
                        fS = fS0;
                        fT = fT0;
                    }
                }
                else  // region 1p
                {
                    // min on face s+t=1
                    fSqrDist = RaySegSqrDistance(
						vOrigin, vDirection,
						Triangle.Origin()+Triangle.Edge0(), 
						Triangle.Edge1()-Triangle.Edge0(),
						0,&fT);
					fS = 1.0-fT;
                }
            }
        }
    }
    else
    {
        // ray and triangle are parallel
        fSqrDist = RaySegSqrDistance(
			vOrigin, vDirection,
			Triangle.Origin(), Triangle.Edge0(),
			&fR,&fS);
        fT = 0.0;

        fSqrDist0 = RaySegSqrDistance(
			vOrigin, vDirection,
			Triangle.Origin(), Triangle.Edge1(),
			&fR0,&fT0);
        fS0 = 0.0;
		
        MINIMIZE_SQRDEST

        fSqrDist0 = RaySegSqrDistance(
			vOrigin, vDirection,
			Triangle.Origin()+Triangle.Edge0(), 
			Triangle.Edge1()-Triangle.Edge0(),
			&fR0,&fT0);
        fS0 = 1.0-fT0;
		
		MINIMIZE_SQRDEST

        fSqrDist0 = VecTriSqrDistance(vOrigin,Triangle,&fS0,&fT0);
        fR0 = 0.0;
		
		MINIMIZE_SQRDEST
    }

    if (pfRayP)
        *pfRayP = fR;

    if (pfTriP0)
        *pfTriP0 = fS;

    if (pfTriP1)
        *pfTriP1 = fT;

    return fabs(fSqrDist);
}

//----------------------------------------------------------------------------

bool RayTriIntersection(
	const D3DXVECTOR3& vOrigin, 
	const D3DXVECTOR3& vDirection, 
	const Triangle& Triangle,
	D3DXVECTOR3& rvPoint,
	FLOAT* pfDist)
{
    FLOAT fRayP;
    
	if (RayTriSqrDistance(
		vOrigin,vDirection,Triangle,&fRayP,0,0) <= 1e-12)
    {
        rvPoint = vOrigin+fRayP*vDirection;
        
		if (pfDist)
			*pfDist = fRayP;

		return true;
    }
	
    return false;
}

}
}