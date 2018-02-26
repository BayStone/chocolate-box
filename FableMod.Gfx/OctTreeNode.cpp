#include "stdafx.h"
#include "OctTreeNode.h"
#include <time.h>
#include "Helpers.h"

#ifdef _DEBUG
#include "Manager.h"
#endif

namespace FableMod
{
namespace Gfx
{

OctTreeNode::OctTreeNode() :
m_vMin(0,0,0),
m_vMax(0,0,0),
m_uiCount(0)
{
	ZeroMemory(m_apOctNodes,sizeof(m_apOctNodes));
}

OctTreeNode::~OctTreeNode()
{
	Dispose();
}

static FLOAT UnitRandom()
{
	// Random number: [0,1]
	return ((FLOAT)rand())/(FLOAT)RAND_MAX;
}

template <typename T>
T TRandom(T Min,T Max)
{
	// Assumption: Max > Min
	// Returns: [Min,Max]
	return (T)((FLOAT)(Max-Min)*UnitRandom())+Min;
}

void OctTreeNode::RemoveChildren()
{
	for (int i = 0; i < 8; ++i)
	{
		GFX_SAFE_DELETE(m_apOctNodes[i]);
	}
}

// Build the octree. Should be called only once per application.
void OctTreeNode::Build(
	const D3DXVECTOR3& vMin, 
	const D3DXVECTOR3& vMax,
	const D3DXVECTOR3& vMinSize)
{
	m_vMin = vMin;
	m_vMax = vMax;

	const D3DXVECTOR3 vSize(vMax-vMin);
	
	RemoveChildren();

	int iIndex = 0;

	m_uiCount = 0;

	srand((unsigned int)time(0));

	if (vSize.x > vMinSize.x && 
		vSize.y > vMinSize.y && 
		vSize.z > vMinSize.z)
	{
		D3DXVECTOR3 vPoint(vMin);
	
		const D3DXVECTOR3 vChildSize(vSize/2);

		for (int z = 0; z < 2; ++z)
		{
			vPoint.x = vMin.x;

			for (int x = 0; x < 2; ++x)
			{
				vPoint.y = vMin.y;
		
				for (int y = 0; y < 2; ++y)
				{
					OctTreeNode* pNode = new OctTreeNode();

					D3DXVECTOR3 vTmp(0,0,0);/*
						TRandom<FLOAT>(0,vChildSize.x/4),
						TRandom<FLOAT>(0,vChildSize.y/4),
						TRandom<FLOAT>(0,vChildSize.y/4));
					*/

					pNode->Build(
						vPoint-vTmp,
						vPoint+vChildSize+vTmp,
						vMinSize);

					m_apOctNodes[iIndex++] = pNode;

					vPoint.y += vChildSize.y;
				}

				vPoint.x += vChildSize.x;
			}

			vPoint.z += vChildSize.z;
		}

		m_uiCount = 8;
	}
}

void OctTreeNode::Update(bool bInit)
{
	Node::Update(bInit);

	// Now update the oct nodes.

	bool bFirst = (m_Children.GetCount() == 0);

	for (int i = 0; i < m_uiCount; ++i)
	{
		OctTreeNode* pNode = m_apOctNodes[i];
		
		if (IsTransformUpdated())
		{
			// OctTree nodes aren't direct children...
			pNode->SetTranslation(GetTranslation());
		}

		pNode->Update(false);

		if (bFirst)
		{
			m_pWorldBound->Copy(pNode->GetWorldBound());
			bFirst = false;
		}
		else
		{
			m_pWorldBound->Merge(pNode->GetWorldBound());
		}

		if (pNode->IsTransformUpdated())
		{
			Flags() |= FLAGS_UPDATE_VISIBILITY;
		}
	}
}

/// Attach the spatial to the tree in the node that fully contains it.
bool OctTreeNode::AttachToTree(Spatial* pChild)
{
	if (!IsInside(pChild))
		return false;

	for (int i = 0; i < m_uiCount; ++i)
	{
		if (m_apOctNodes[i]->AttachToTree(pChild))
			return true;				
	}

	pChild->Detach();
	
	Node::AttachChild(pChild);

	return true;
}

/// Test if a spatial object is completely inside of the node.
bool OctTreeNode::IsInside(Spatial* pSpatial)
{
	FLOAT fRad = pSpatial->GetWorldBound()->Radius();
	D3DXVECTOR3 vCenter = pSpatial->GetWorldBound()->Center();

	/*
	return
		vCenter.x-fRad >= m_vMin.x &&
		vCenter.x+fRad < m_vMax.x &&
		vCenter.y-fRad >= m_vMin.y &&
		vCenter.y+fRad < m_vMax.y &&
		vCenter.z-fRad >= m_vMin.z &&
		vCenter.z+fRad < m_vMax.z;
	*/
	return
		vCenter.x >= m_vMin.x+GetWorldTranslation().x &&
		vCenter.x < m_vMax.x+GetWorldTranslation().x  &&
		vCenter.y >= m_vMin.y+GetWorldTranslation().y &&
		vCenter.y < m_vMax.y+GetWorldTranslation().y  &&
		vCenter.z >= m_vMin.z+GetWorldTranslation().z &&
		vCenter.z < m_vMax.z+GetWorldTranslation().z;
}

void OctTreeNode::OnGetRenderList(Camera* pCamera)
{
	Node::OnGetRenderList(pCamera);

	for (int i = 0; i < m_uiCount; ++i)
	{
		m_apOctNodes[i]->GetRenderList(pCamera);
	}
}

/// Get the nodes that need to be updated aka. the nodes
/// that have moved and need a new octtree node.
void OctTreeNode::GetUpdatable(SpatialVector& rObjects)
{
	for (int i = 0; i < m_Children.GetCount(); ++i)
	{
		Spatial* pChild = m_Children[i];

		if (pChild->IsTransformUpdated() && !IsInside(pChild))
		{
			// Object has moved and isn't inside anymore.
			rObjects.Add(pChild);
		}
	}

	// Handle the kid nodes.
	for (int i = 0; i < m_uiCount; ++i)
	{
		m_apOctNodes[i]->GetUpdatable(rObjects);
	}
}

void OctTreeNode::Pick(
	const D3DXVECTOR3& vOrigin, 
	const D3DXVECTOR3& vDirection,
	PickData& rData)
{
	if (!GetWorldBound()->RayIntersection(vOrigin, vDirection))
	{
		// Doesn't hit this, doesn't hit children.
		return;
	}

	for (int i = 0; i < m_Children.GetCount(); ++i)
	{
		m_Children[i]->Pick(vOrigin, vDirection, rData);
	}

	for (int i = 0; i < m_uiCount; ++i)
	{
		m_apOctNodes[i]->Pick(vOrigin, vDirection, rData);
	}
}

#ifdef _DEBUG
// Red, Green, Blue, Yellow, Purple, Cyan
static const FLOAT LEVEL_R[] = { 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f };
static const FLOAT LEVEL_G[] = { 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f };
static const FLOAT LEVEL_B[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f };

void OctTreeNode::RenderBound(int iLevel)
{
	const D3DXVECTOR3 vSize(m_vMax-m_vMin);
	
	if (iLevel < 6 && m_Children.GetCount() > 0)
	{
		D3DCOLORVALUE Color;
		
		Color.r = LEVEL_R[iLevel];
		Color.g = LEVEL_G[iLevel];
		Color.b = LEVEL_B[iLevel];
		Color.a = 0.3f;
	
		Manager::GetDevice()->DrawSphere(
			GetWorldBound()->Center(),
			GetWorldBound()->Radius(),
			Color);
	}
	
	for (int i = 0; i < m_uiCount; ++i)
	{
		m_apOctNodes[i]->RenderBound(iLevel+1);
	}
}
#endif

}
}