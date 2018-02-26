#include "Stdafx.h"
#include "Node.h"
#include "Camera.h"
#include "Spatial.h"
#include "Rotation.h"
#include "Settings.h"
#include "LogFile.h"
#include "Mesh.h"

namespace FableMod
{
namespace Gfx
{

unsigned int Spatial::sm_uiNextID = 0;

Spatial::Spatial() :
m_pParent(0),
m_vTranslation(D3DXVECTOR3(0,0,0)),
m_vWorldTranslation(D3DXVECTOR3(0,0,0)),
m_fScale(1),
m_pWorldBound(new BoundSphere()),
m_uiFlags(FLAGS_TRANSFORM_DIRTY),
m_uiID(0)
{
	D3DXMatrixIdentity(&m_matWorldTransform);
	D3DXMatrixIdentity(&m_matWorldRotation);
	D3DXMatrixIdentity(&m_matRotation);
}

Spatial::~Spatial()
{
	FM_LOG1(_T("Spatial Destructor: \"%s\"\n"), GetName());
	delete m_pWorldBound;
}

void Spatial::Dispose()
{
}

void Spatial::Detach()
{
	if (Parent() != 0)
	{
		Parent()->DetachChild(this);
	}
}

void Spatial::SetAlpha(FLOAT fAlpha)
{
}

D3DXVECTOR3 Spatial::GetWorldForward() const
{
	D3DXVECTOR3 vTmp(0,1,0);

	D3DXVec3TransformCoord(&vTmp,&vTmp,&m_matWorldRotation);

	D3DXVec3Normalize(&vTmp, &vTmp);

	return vTmp;
}

D3DXVECTOR3 Spatial::GetWorldRight() const
{
	D3DXVECTOR3 vTmp(-1,0,0);

	D3DXVec3TransformCoord(&vTmp,&vTmp,&m_matWorldRotation);

	D3DXVec3Normalize(&vTmp, &vTmp);

	return vTmp;
}

D3DXVECTOR3 Spatial::GetWorldUp() const
{
	D3DXVECTOR3 vTmp(0,0,1);

	D3DXVec3TransformCoord(&vTmp,&vTmp,&m_matWorldRotation);

	D3DXVec3Normalize(&vTmp, &vTmp);
	
	return vTmp;
}

void Spatial::SetRotation(const D3DXMATRIX& matRotation)
{
	Flags() |= FLAGS_TRANSFORM_DIRTY;

	m_matRotation = matRotation;
}

void Spatial::SetRotation(const D3DXVECTOR3& vRotation)
{
	Flags() |= FLAGS_TRANSFORM_DIRTY;
	
	D3DXMatrixRotationYawPitchRoll(
		&m_matRotation, 
		vRotation.y, 
		vRotation.x, 
		vRotation.z);
}

D3DXVECTOR3 Spatial::GetRotation() const
{
	D3DXVECTOR3 v;

	MatrixToYawPitchRoll(m_matRotation, v.y, v.x, v.z);
	
	return v;
}

D3DXMATRIX Spatial::GetRotationMatrix() const
{
	return m_matRotation;
}

D3DXMATRIX Spatial::GetWorldRotationMatrix() const
{
	return m_matWorldRotation;
}

static bool Vec3Equals(const D3DXVECTOR3& vA, const D3DXVECTOR3& vB)
{
	static const FLOAT EPSILON = 0.0001f;

	return
		fabs(vA.x-vB.x) < EPSILON &&
		fabs(vA.y-vB.y) < EPSILON &&
		fabs(vA.z-vB.z) < EPSILON;
}

void Spatial::GetDirectionMatrix(
	const D3DXVECTOR3& vForward,
	const D3DXVECTOR3& vUp,
	D3DXMATRIX& rmatRotation) const
{
	static const D3DXVECTOR3 VECTORX(-1,0,0);
	static const D3DXVECTOR3 VECTORY(0,1,0);
	static const D3DXVECTOR3 VECTORZ(0,0,1);

	D3DXVECTOR3 vN,vU,vR;
	//FLOAT fAngle;
	D3DXMATRIX matUp;

	vN = vForward;
	vU = vUp;
	
	D3DXVec3Normalize(&vU, &vU);

	/*
	D3DXVECTOR3 vNProj(vN);

	if (fabs(vUp.z) > fabs(vUp.x) && fabs(vUp.z) > fabs(vUp.y))
	{
		vNProj.z = 0;

		D3DXVec3Normalize(&vNProj, &vNProj);
		D3DXVec3Normalize(&vN, &vN);

		if (fabs(vNProj.x) >= 0.0001f)
		{
			fAngle = atanf(vNProj.y/vNProj.x);

			if (vN.x < 0.0f)
				fAngle += D3DX_PI/2;
			else
				fAngle -= D3DX_PI/2;
			
			D3DXMatrixRotationZ(&rmatRotation, fAngle);
		}
		else
		{
			if (vNProj.y >= 0.0f)
				D3DXMatrixIdentity(&rmatRotation);
			else
				D3DXMatrixRotationZ(&rmatRotation, D3DXToRadian(180.0f));
		}

		if (!Vec3Equals(vUp, VECTORZ))
		{
			// Apply up vector. See end of the file.

			D3DXVec3Cross(&vN, &VECTORZ, &vUp);

			D3DXVec3Normalize(&vN, &vN);

			fAngle = acos(D3DXVec3Dot(&VECTORZ, &vUp));

			D3DXMatrixRotationAxis(&matUp, &vN, fAngle);

			rmatRotation = rmatRotation*matUp;
		}
	}
	else if (fabs(vUp.x) > fabs(vUp.z) && fabs(vUp.x) > fabs(vUp.y))
	{*/
		/*
		vNProj.x = 0;

		D3DXVec3Normalize(&vNProj, &vNProj);
		D3DXVec3Normalize(&vN, &vN);

		if (fabs(vNProj.z) >= 0.0001f)
		{
			fAngle = atanf(vNProj.y/vNProj.z);

			if (vN.z < 0.0f)
				fAngle -= D3DX_PI/2;
			else
				fAngle += D3DX_PI/2;
			
			D3DXMatrixRotationX(&rmatRotation, fAngle);
		}
		else
		{
			if (vNProj.y >= 0.0f)
				D3DXMatrixIdentity(&rmatRotation);
			else
				D3DXMatrixRotationX(&rmatRotation, D3DXToRadian(180.0f));
		}

		D3DXMATRIX matUp;

		if (vUp.x > 0.0f)
			D3DXMatrixRotationY(&matUp, D3DXToRadian(90.0f));
		else
			D3DXMatrixRotationY(&matUp, D3DXToRadian(-90.0f));
	
		rmatRotation = matUp*rmatRotation;
		*/

		D3DXVec3Normalize(&vN, &vN);

		D3DXVec3Cross(&vR, &vN, &vU);
		D3DXVec3Normalize(&vR, &vR);

		D3DXMatrixIdentity(&rmatRotation);

		rmatRotation._11 = vR.x;
		rmatRotation._12 = vN.x;
		rmatRotation._13 = vU.x;
		rmatRotation._21 = vR.y;
		rmatRotation._22 = vN.y;
		rmatRotation._23 = vU.y;
		rmatRotation._31 = vR.z;
		rmatRotation._32 = vN.z;
		rmatRotation._33 = vU.z;

		D3DXMatrixInverse(&rmatRotation, 0, &rmatRotation);
	/*
	}
	else if (fabs(vUp.y) > fabs(vUp.x) && fabs(vUp.y) > fabs(vUp.z))
	{
		vNProj.y = 0;

		D3DXVec3Normalize(&vNProj, &vNProj);
		D3DXVec3Normalize(&vN, &vN);

		if (fabs(vNProj.x) >= 0.0001f)
		{
			fAngle = atanf(vNProj.z/vNProj.x);

			if (vN.x < 0.0f)
				fAngle += D3DX_PI/2;
			else
				fAngle -= D3DX_PI/2;
			
			D3DXMatrixRotationY(&rmatRotation, fAngle);
		}
		else
		{
			if (vNProj.z >= 0.0f)
				D3DXMatrixIdentity(&rmatRotation);
			else
				D3DXMatrixRotationY(&rmatRotation, D3DXToRadian(180.0f));
		}
	}
	*/
}

void Spatial::SetDirection(const D3DXVECTOR3& vForward,
						   const D3DXVECTOR3& vUp)
{
	D3DXMATRIX matRot;
	GetDirectionMatrix(vForward, vUp, matRot);
	SetRotation(matRot);
}

void Spatial::SetWorldDirection(const D3DXVECTOR3& vForward,
								const D3DXVECTOR3& vUp)
{
	D3DXMATRIX matRot;
	GetDirectionMatrix(vForward, vUp, matRot);
	SetWorldRotation(matRot);
}

void Spatial::SetWorldTranslation(const D3DXVECTOR3& vTrans)
{
	Flags() |= FLAGS_TRANSFORM_DIRTY;

	if (Parent())
	{
		D3DXMATRIX matInverse;
		
		D3DXMatrixInverse(
			&matInverse, NULL, &Parent()->GetWorldTransform());		

		D3DXVec3TransformCoord(&m_vTranslation, &vTrans, &matInverse);
	}
	else
	{
		m_vTranslation = vTrans;
	}
}

void Spatial::SetWorldRotation(const D3DXVECTOR3& vRotation)
{
	Flags() |= FLAGS_TRANSFORM_DIRTY;

	D3DXMATRIX matRot;

	D3DXMatrixRotationYawPitchRoll(
		&matRot, vRotation.y, vRotation.x, vRotation.z);

	SetWorldRotation(matRot);
}

void Spatial::SetWorldRotation(const D3DXMATRIX& matRotation)
{
	Flags() |= FLAGS_TRANSFORM_DIRTY;

	if (Parent())
	{
		D3DXMATRIX matInverse;

		D3DXMatrixInverse(
			&matInverse, NULL, &Parent()->GetWorldRotationMatrix());		

		m_matRotation = matRotation*matInverse;
	}
	else
	{
		m_matRotation = matRotation;
	}
}

D3DXVECTOR3 Spatial::GetWorldRotation() const
{
	D3DXVECTOR3 v;

	MatrixToYawPitchRoll(
		m_matWorldRotation, v.z, v.x, v.y);
	
	return v;
}

void Spatial::UpdateWorldTransform()
{
	// Scaling doesn't transfer!

	D3DXMATRIX matScale;
	D3DXMATRIX matTranslation;

	D3DXMatrixScaling(
		&matScale, 
		m_fScale,
		m_fScale,
		m_fScale);

	D3DXMatrixTranslation(
		&matTranslation, 
		m_vTranslation.x, m_vTranslation.y, m_vTranslation.z);
	
	m_matWorldTransform =
		matScale*m_matRotation*matTranslation;

	if (Parent())
	{
		D3DXMatrixMultiply(
			&m_matWorldTransform,
			&m_matWorldTransform,
			&Parent()->m_matWorldTransform);
	
		// Keep track of the variables.

		D3DXVec3TransformCoord(
			&m_vWorldTranslation,
			&m_vTranslation,
			&Parent()->m_matWorldTransform);

		m_fWorldScale = m_fScale*Parent()->GetWorldScale();
	
		m_matWorldRotation = m_matRotation*Parent()->m_matWorldRotation;
	
		//m_quatWorldRotation = m_quatRotation*Parent()->m_quatWorldRotation;
	}
	else
	{
		m_vWorldTranslation = m_vTranslation;
		m_fWorldScale = m_fScale;
		m_matWorldRotation = m_matRotation;
		//m_quatWorldRotation = m_quatRotation;
	}
}

void Spatial::Update(bool bInit)
{
	// In some instances the visibility flag will be 1 frame late.
	// For example: Parent force culling.

	if (Flags() & FLAGS_RENDERED)
	{
		Flags() |= FLAGS_IS_VISIBLE;
	}
	else
	{
		Flags() |= FLAGS_UPDATE_VISIBILITY;
		Flags() &= ~FLAGS_IS_VISIBLE;
	}
	
	Flags() &= ~FLAGS_RENDERED;
	
	Flags() &= ~FLAGS_TRANSFORM_UPDATED;
	
	if ((Flags() & FLAGS_TRANSFORM_DIRTY) != 0 || 
		(Parent() != 0 && Parent()->IsTransformUpdated()))
	{
		Flags() &= ~FLAGS_TRANSFORM_DIRTY;
		
		UpdateWorldTransform();

		UpdateWorldBound();

		Flags() |= FLAGS_TRANSFORM_UPDATED;
		
		// We need to update visibility state.
		Flags() |= FLAGS_UPDATE_VISIBILITY;
	}
}

void Spatial::GetRenderList(Camera* pCamera)
{
	if (Flags() & FLAGS_CAMERA_INSIDE_WORLDBOUND)
	{
		if (!GetWorldBound()->Inside(pCamera->GetPosition()))
		{
			Flags() &= ~FLAGS_IS_VISIBLE;
			return;
		}
	}

	if (Flags() & FLAGS_FORCE_CULLING)
	{	
		Flags() &= ~FLAGS_IS_VISIBLE;
		return;
	}
	else if (Flags() & FLAGS_ALWAYS_VISIBLE)
	{
		Flags() |= (FLAGS_RENDERED | FLAGS_IS_VISIBLE);
		OnGetRenderList(pCamera);
		return;
	}

	const unsigned int uiPlanes = pCamera->PlaneState();

	// Does the object think its visible.
	bool bVisible = (Flags() & FLAGS_IS_VISIBLE) != 0;

	if ((Flags() & FLAGS_UPDATE_VISIBILITY) != 0 || pCamera->IsUpdated())
	{
		// We don't want to come here next time.
		Flags() &= ~FLAGS_UPDATE_VISIBILITY;
	
		bVisible = !pCamera->Culled(GetWorldBound());
	}
	
	if (bVisible)
	{		
		Flags() |= (FLAGS_RENDERED | FLAGS_IS_VISIBLE);
		OnGetRenderList(pCamera);
	}
	else
	{
		Flags() &= ~FLAGS_IS_VISIBLE;
	}

	pCamera->PlaneState() = uiPlanes;

	// During the next update the visibility flag is updated
	// based on whether or not the object was rendered.
}

void Spatial::OnGetRenderList(Camera* pCamera)
{
	// Stub
}

void Spatial::UpdateWorldBound()
{
	GetWorldBound()->Center() = D3DXVECTOR3(0,0,0);
	GetWorldBound()->Radius() = 0.0f;
}

void Spatial::Pick(
	const D3DXVECTOR3& vOrigin, 
	const D3DXVECTOR3& vDirection,
	PickData& rData)
{
	// Stub
}

void Spatial::CopyProperties(Spatial* pObject) const
{
	pObject->SetRotation(GetRotation());
	pObject->SetTranslation(GetTranslation());
	pObject->SetScale(GetScale());
}

bool Spatial::GetBox(AxisBox* pBox)
{
	return true;
}

bool Spatial::InBox(const D3DXVECTOR3& vPoint)
{
	// TODO
	return false;
}

Spatial* Spatial::GetRoot() const
{
	if (Parent())
		return Parent()->GetRoot();
	else
		return (Spatial*)this;
}

unsigned int Spatial::GetNextID()
{
	return ++sm_uiNextID;
}

void Spatial::ApplyTreeFlags(
	Spatial* pSpatial, 
	bool bFlags, 
	unsigned int uiFlags,
	bool bRenderFlags, 
	unsigned int uiRenderFlags)
{
	Node* pNode = dynamic_cast<Node*>(pSpatial);

	if (pNode)
	{
		int iCount = pNode->NumChildren();

		for (int i = 0; i < iCount; ++i)
		{
			ApplyTreeFlags(pNode->GetChildAt(i), 
				bFlags, uiFlags, bRenderFlags, uiRenderFlags);
		}
	}
	else
	{
		Mesh* pMesh = dynamic_cast<Mesh*>(pSpatial);

		if (pMesh)
		{
			if (bRenderFlags)
				pMesh->RenderFlags() |= uiRenderFlags;
			else
				pMesh->RenderFlags() &= ~uiRenderFlags;

			if (bFlags)
				pMesh->Flags() |= uiFlags;
			else
				pMesh->Flags() &= ~uiFlags;
		}
	}
}

}
}


/*

MATH

What you are doing is changing the unit vector directional 
components of the model matrix, which actually is more of an 
axis reflection than a rotation. This will cause problems such 
as culling issues and texture coordinate problems since you are 
effectively changing the coordinate system.

If you are trying to convert from one normal vector to another, 
make an axis rotation matrix.  The axis of rotation is the normalized 
cross product between your start and end normal, and the arc cosine 
of the dot product will give you the angle.

For example, to go from the original up vector of (0,1,0) to an up 
vector of (1,0,0), do:

Vector3 axis = Vector3.Normalize(Vector3.Cross(Vector3.Right, Vector3.Up));
float angle = (float)Math.Acos(Vector3.Dot(Vector3.Right, Vector3.Up));
Matrix rotation = Matrix.CreateFromAxisAngle(axis, angle);

*/