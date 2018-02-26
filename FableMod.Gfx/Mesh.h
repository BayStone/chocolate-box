#pragma once

#include "Spatial.h"
#include "Boundsphere.h"
#include "Model.h"

namespace FableMod
{
namespace Gfx
{

/// Flags for mesh rendering.
enum EMeshRenderFlags
{
	MRF_HIGHLIGHT = 1,
	MRF_AS_BOX = 2,
	MRF_DISTANCE_TO_BOX = 4,
	MRF_BOX = 8,
	MRF_WIREFRAME = 16,
	MRF_WIREFRAME_OVERLAY = 32,
	MRF_BOUNDSPHERE = 64,
	MRF_AS_STONE = 128,
	MRF_DISABLE_ZTEST = 256,
	MRF_FLORA = 512,
};

/// Mesh represents the base class of objects that have 3d geometry.
class GFX_API Mesh : public Spatial
{
public:
	
	Mesh();

	virtual ~Mesh();

	virtual void Dispose();

	virtual void Render(Camera* pCamera);

	virtual void OnGetRenderList(Camera* pCamera);

	virtual void SetAlpha(FLOAT fAlpha);

	FLOAT GetAlpha() const
	{
		return m_fAlpha;
	}

	void SetModel(Model* pModel);

	Model* GetModel() const { return m_spModel; }

	virtual Spatial* Copy() const;

	virtual bool GetBox(AxisBox* pBox);

	const FLOAT& DistanceToCamera() const { return m_fDistanceToCamera; }
	FLOAT& DistanceToCamera() { return m_fDistanceToCamera; }

	unsigned int& RenderFlags() { return m_uiRenderFlags; }

	const unsigned int& RenderFlags() const { return m_uiRenderFlags; }

	virtual void Pick(
		const D3DXVECTOR3& vOrigin, 
		const D3DXVECTOR3& vDirection,
		PickData& rData);

	BoundSphere* GetModelBound() { return m_pModelBound; }
	
	virtual bool AlphaRequired() const;

protected:

	void RenderWireframeOverlay(Camera* pCamera);

	void RenderHighlight(Camera* pCamera, const float* afInkColor);

	virtual void UpdateWorldTransform();

	virtual void UpdateWorldBound();

	virtual void CopyProperties(Spatial* pObject) const;

	virtual void OnRender(Camera* pCamera);

	FLOAT m_fDistanceToCamera;
	BoundSphere* m_pModelBound;
	ModelPtr m_spModel;
	AxisBox* m_pBox;
	unsigned int m_uiRenderFlags;
	bool m_bUpdateBox;
	FLOAT m_fAlpha;
};

GFX_DECLARE_SMARTPOINTER(Mesh);

}
}