#pragma once

#include "GfxConf.h"
#include "AxisBox.h"
#include "Vector.h"
#include "Spatial.h"

namespace FableMod
{
namespace Gfx
{

class Spatial;

/// Axis aligned box. For object selection.
/// It's questionable whether it even should be here.
class GFX_API SelectionBox : public AxisBox
{
public:

	SelectionBox();

	~SelectionBox();

	virtual void Initialize();

	void Update();

	void Add(Spatial* pSpatial);

	void Remove(Spatial* pSpatial);

	void StartRotation();

	void StopRotation();
	
	void AddRotation(const D3DXMATRIX& matRotation);

	void SetCenter(const D3DXVECTOR3& vPoint);

	void Clear();

	bool IsEmpty() const;

	bool HasObject(Spatial* pSpatial) const;

	int GetNumObjects() const;

	Spatial* GetObjectAt(int index) const;

	void EnableAt(int index, bool bEnabled);

	FLOAT GetRadius() const;

	bool HasEnabled() const;

protected:

	void UpdateBox();
	void UpdateDiffs();

	struct SObjectInfo
	{
		SpatialPtr spSpatial;
		D3DXVECTOR3 vDiff;
		D3DXMATRIX matRotation;
		bool bEnabled;

		~SObjectInfo();
	};

	Vector<SObjectInfo*> m_pObjects;
	D3DXMATRIX m_matRotation;
};

}
}