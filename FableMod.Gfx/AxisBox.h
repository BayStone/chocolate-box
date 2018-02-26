#pragma once

#include "GfxConf.h"
#include "Plane.h"

namespace FableMod
{
namespace Gfx
{

/// Axis aligned bounding box.
class GFX_API AxisBox
{
public:

	AxisBox();

	virtual ~AxisBox() {} 

	virtual void Initialize();

	void Update(const D3DXVECTOR3& vPoint);

	void Update(const AxisBox* pBox);
	
	bool Inside(const D3DXVECTOR3& vPoint) const;

	const D3DXVECTOR3& GetMin() const;

	const D3DXVECTOR3& GetMax() const;

	const FLOAT GetWidth() const;

	const FLOAT GetHeight() const;

	const FLOAT GetDepth() const;

	D3DXVECTOR3 GetCenter() const;

protected:

	D3DXVECTOR3 m_vMin, m_vMax;
};

}
}