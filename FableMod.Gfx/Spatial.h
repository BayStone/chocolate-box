#pragma once

#include "GfxConf.h"
#include "BoundSphere.h"
#include "AxisBox.h"
#include "RootObject.h"

namespace FableMod
{
namespace Gfx
{

class GFX_API Node;
class GFX_API Camera;

enum ESpatialFlags
{
	FLAGS_NONE = 0,
	/// Set when transform is changed.
	FLAGS_TRANSFORM_DIRTY = 1,
	/// Was the object dirty during last update call.
	FLAGS_TRANSFORM_UPDATED = 2,
	/// Object is visible
	FLAGS_IS_VISIBLE = 4,
	/// Update visibility
	FLAGS_UPDATE_VISIBILITY = 8,
	/// Renderable object
	FLAGS_RENDERABLE = 16,
	/// Force culling, visibility
	FLAGS_FORCE_CULLING = 32,
	/// Disable object culling.
	FLAGS_ALWAYS_VISIBLE = 128,
	/// Do not handle picking.
	FLAGS_EXCLUDE_PICK = 256,
	/// Disable all forms of object simplification.
	FLAGS_DISABLE_SIMPLIFICATION = 512,
	/// Exclude from world bound merge.
	FLAGS_EXCLUDE_BOUND_MERGE = 1024,
	/// Visible only when the camera is inside the worldbound.
	FLAGS_CAMERA_INSIDE_WORLDBOUND = 2048,
	
	FLAGS_RENDERED = 4096,
};

/// Structure for pick aka. getting an object from the 
/// scene graph with a ray data.
struct GFX_API PickData
{
	class Mesh*		pMesh;
	FLOAT			fRange;
	D3DXVECTOR3		vPoint;
	D3DXVECTOR3		vNormal;
	int				iFace;
	
	PickData() :
	fRange(FLT_MAX),
	pMesh(0),
	iFace(-1)
	{
	}
};

/// Scene graph base class.
class GFX_API Spatial : public RootObject
{
public:

	virtual ~Spatial();

	virtual void Dispose();

	virtual void Update(bool bInit = true);

	void Detach();

	virtual void GetRenderList(Camera* pCamera);

	virtual void SetAlpha(FLOAT fAlpha);
	
	virtual FLOAT GetAlpha() const;

	void SetRotation(const D3DXMATRIX& matRotation);
	void SetRotation(const D3DXVECTOR3& vRotation);
	D3DXVECTOR3 GetRotation() const;
	D3DXMATRIX GetRotationMatrix() const;
	
	void SetWorldRotation(const D3DXVECTOR3& vRotation);
	void SetWorldRotation(const D3DXMATRIX& matRotation);
	D3DXVECTOR3 GetWorldRotation() const;
	D3DXMATRIX GetWorldRotationMatrix() const;
	
	void SetTranslation(const D3DXVECTOR3& vTrans);
	const D3DXVECTOR3& GetTranslation() const;
    
	void SetWorldTranslation(const D3DXVECTOR3& vTrans);
	const D3DXVECTOR3& GetWorldTranslation() const;
   	
	void SetScale(FLOAT fScale);
	FLOAT GetScale() const;

	FLOAT GetWorldScale() const;

	void SetDirection(const D3DXVECTOR3& vForward,
					  const D3DXVECTOR3& vUp = D3DXVECTOR3(0,0,1));

	void SetWorldDirection(
		const D3DXVECTOR3& vForward,
		const D3DXVECTOR3& vUp = D3DXVECTOR3(0,0,1));

	const D3DXMATRIX& GetWorldTransform(bool bUpdate = false);
  
	D3DXVECTOR3 GetWorldForward() const;

	D3DXVECTOR3 GetWorldRight() const;

	D3DXVECTOR3 GetWorldUp() const;

	virtual bool GetBox(AxisBox* pBox);

	bool InBox(const D3DXVECTOR3& vPoint);

	BoundSphere* GetWorldBound() const;

	Spatial* GetRoot() const;

	const Node* Parent() const;

	Node*& Parent();
	
	unsigned int& Flags();

	const unsigned int& Flags() const;

	bool IsTransformUpdated() const;

	virtual Spatial* Copy() const = 0;
	
	virtual void Pick(
		const D3DXVECTOR3& vOrigin, 
		const D3DXVECTOR3& vDirection,
		PickData& rPickData);

	const unsigned int& ID() const;

	unsigned int& ID();
	
	virtual bool IsLeaf() const;

	static unsigned int GetNumObjects();

	static unsigned int GetNextID();

	static void ApplyTreeFlags(
		Spatial* pSpatial, 
		bool bFlags, 
		unsigned int uiFlags,
		bool bRenderFlags, 
		unsigned int uiRenderFlags);

protected:
    
	Spatial();

	void GetDirectionMatrix(
		const D3DXVECTOR3& vForward,
		const D3DXVECTOR3& vUp,
		D3DXMATRIX& rMatrix) const;

	virtual void UpdateWorldTransform();

	virtual void OnGetRenderList(Camera* pCamera);

	virtual void UpdateWorldBound();

	virtual void CopyProperties(Spatial* pObject) const;

	Node* m_pParent;
	unsigned int m_uiFlags;
	
	D3DXMATRIX	m_matWorldTransform;
	
	D3DXVECTOR3 m_vTranslation;
	D3DXMATRIX	m_matRotation;
	FLOAT m_fScale;
	
	D3DXVECTOR3 m_vWorldTranslation;
	D3DXMATRIX	m_matWorldRotation;
	FLOAT m_fWorldScale;
	
	BoundSphere* m_pWorldBound;

	unsigned int m_uiID;

	static unsigned int sm_uiNextID;
};

#include "Spatial.inl"

GFX_DECLARE_SMARTPOINTER(Spatial);

}
}