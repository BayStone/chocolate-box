#include "stdafx.h"
#include "Boundsphere.h"

namespace FableMod
{
namespace Gfx
{

BoundSphere::BoundSphere() :
m_vCenter(0,0,0),
m_fRadius(0.0f)
{
}

BoundSphere::~BoundSphere()
{
}

void BoundSphere::Copy(BoundSphere* pBound)
{
	Center() = pBound->Center();
	Radius() = pBound->Radius();
}

void BoundSphere::Compute(unsigned int uiNumVertices, D3DXVECTOR3* avVertices)
{
	D3DXVECTOR3 vMin(avVertices[0]), vMax(vMin);
    
	for (unsigned int i = 1; i < uiNumVertices; ++i)
	{
		if (vMin[0] > avVertices[i][0])
            vMin[0] = avVertices[i][0];        
		if (vMin[1] > avVertices[i][1])
            vMin[1] = avVertices[i][1];
        if (vMin[2] > avVertices[i][2])
            vMin[2] = avVertices[i][2];
        if (vMax[0] < avVertices[i][0])
            vMax[0] = avVertices[i][0];
        if (vMax[1] < avVertices[i][1])
            vMax[1] = avVertices[i][1];
        if (vMax[2] < avVertices[i][2])
            vMax[2] = avVertices[i][2];
	}
	
	Center() = 0.5f*(vMin+vMax);
	
	FLOAT fRadiusSqr = 0.0f;
    
	for (unsigned int i = 1; i < uiNumVertices; ++i) 
    {
        D3DXVECTOR3 vDiff(avVertices[i]-Center());
        
		FLOAT fLengthSqr = D3DXVec3LengthSq(&vDiff);
        
		if (fLengthSqr > fRadiusSqr)
		{
            fRadiusSqr = fLengthSqr;
		}
	}

    Radius() = sqrt(fRadiusSqr);
}

void BoundSphere::Merge(BoundSphere* pBound)
{
	if (Radius() < 0.001)
	{
		Copy(pBound);
		return;
	}

	static const FLOAT TOLERANCE = 1e-06f;
	
	D3DXVECTOR3 vC1 = pBound->Center();
	FLOAT fR1 = pBound->Radius();

	if (fR1 < 0.001)
		return;
	
	D3DXVECTOR3 vCenterDiff(vC1-Center());
	FLOAT fRadDiff = fR1-Radius();
	
	FLOAT fLengthSqr  = D3DXVec3LengthSq(&vCenterDiff); 
	FLOAT fRadDiffSqr = fRadDiff*fRadDiff;

	if (fRadDiffSqr >= fLengthSqr)
	{
		if (fRadDiff >= 0.0f)
		{
			Center() = vC1;
			Radius() = fR1;
		}
		
		// Mine is bigger, use it.
	}
	else
	{
		FLOAT fLength = (FLOAT)sqrtf(fLengthSqr);

		if (fLength > TOLERANCE)
		{
			FLOAT fCoeff = (fLength+fRadDiff)/(2.0f*fLength);
			Center() = Center()+fCoeff*vCenterDiff;
		}	

		Radius() = (fLength+fR1+Radius())*0.5f;
	}
}

void BoundSphere::Transform(
	const D3DXMATRIX& matTransform,
	FLOAT fScale)
{
	D3DXVec3TransformCoord(&m_vCenter,&m_vCenter,&matTransform);
	
	Radius() *= fScale;
}

bool BoundSphere::RayIntersection(
	const D3DXVECTOR3& vOrigin,
	const D3DXVECTOR3& vDirection) const
{
	D3DXVECTOR3 vl(Center()-vOrigin);
	
	FLOAT fs = D3DXVec3Dot(&vDirection,&vl);
	FLOAT fl2 = D3DXVec3Dot(&vl,&vl);
	FLOAT fr2 = Radius()*Radius();

	if (fs < 0 && fl2 > fr2)
		return false;
	
	FLOAT fm2 = fl2-(fs*fs);

	if (fm2 > fr2) 
		return false;

	FLOAT fq = (FLOAT)sqrtf(fr2-fm2);
	FLOAT ft = 0.0f;

	if (fl2 > fr2) 
		ft = fs-fq;
	else
		ft = fs+fq;
	
	return ft >= 0.0f;
}

bool BoundSphere::FindRayIntersection(
	const D3DXVECTOR3& vOrigin,
	const D3DXVECTOR3& vDirection,
	FLOAT& rfT,
	D3DXVECTOR3& rvPoint) const
{
	const D3DXVECTOR3 vl(Center()-vOrigin);
	
	const FLOAT fs  = D3DXVec3Dot(&vDirection,&vl);
	const FLOAT fl2 = D3DXVec3Dot(&vl,&vl);
	const FLOAT fr2 = Radius()*Radius();

	if (fs < 0 && fl2 > fr2)
	{
		return false;
	}

	const FLOAT fm2 = fl2-(fs*fs);

	if (fm2 > fr2) 
	{
		return false;
	}

	const FLOAT fq = sqrtf(fr2-fm2);
	FLOAT ft = 0.0f;

	if (fl2 > fr2) 
		ft = fs-fq;
	else
		ft = fs+fq;
	
	rfT     = ft;
	rvPoint = vOrigin+ft*vDirection;

	return ft >= 0.0f;
}	

bool BoundSphere::Inside(const D3DXVECTOR3& vPoint)
{
	D3DXVECTOR3 vTmp(vPoint-Center());
	return D3DXVec3LengthSq(&vTmp) < Radius()*Radius();
}

Plane::ESide BoundSphere::WhichSide(const Plane& Plane)
{
	FLOAT fPseudoDistance = Plane.DistanceTo(Center());

	if (fPseudoDistance < -m_fRadius)
	{
		return Plane::NEGATIVE_SIDE;
	}
	else if (fPseudoDistance >= m_fRadius)
	{
		return Plane::POSITIVE_SIDE;
	}
	else
	{
		return Plane::NO_SIDE;
	}
}

}
}