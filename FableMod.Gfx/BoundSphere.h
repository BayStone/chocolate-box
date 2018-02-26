#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include "GfxConf.h"
#include "Plane.h"

namespace FableMod
{
namespace Gfx
{

/// Boundsphere implements basic 3d bounding sphere functionality
/// which can be used with view frustum culling, etc.
class GFX_API BoundSphere
{
public:

	BoundSphere();

	virtual ~BoundSphere();

	void Compute(
		unsigned int uiNumVertices, 
		D3DXVECTOR3* avVertices);

	void Copy(BoundSphere* pBound);

	void Merge(BoundSphere* pBound);

	void Transform(
		const D3DXMATRIX& matTransform,
		FLOAT fScale);

	bool Inside(const D3DXVECTOR3& vPoint);

	bool RayIntersection(
		const D3DXVECTOR3& vOrigin,
		const D3DXVECTOR3& vDirection) const;

	bool FindRayIntersection(
		const D3DXVECTOR3& vOrigin,
		const D3DXVECTOR3& vDirection,
		FLOAT& rfT,
		D3DXVECTOR3& rvPoint) const;

	const D3DXVECTOR3& Center() const { return m_vCenter; }
	D3DXVECTOR3& Center() { return m_vCenter; }
	
	const FLOAT& Radius() const { return m_fRadius; }
	FLOAT& Radius() { return m_fRadius; }
	
	Plane::ESide WhichSide(const Plane& Plane);

protected:

	D3DXVECTOR3 m_vCenter;
	FLOAT m_fRadius;
};
}
}