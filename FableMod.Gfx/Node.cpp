#include "stdafx.h"
#include <cassert>
#include "Spatial.h"
#include "Camera.h"
#include "Node.h"
#include "Helpers.h"

static void DebugPrint(LPCTSTR pszTag, LPCTSTR pszName)
{
	
}

namespace FableMod
{
namespace Gfx
{

Node::Node() :
m_Children(16,16)
{
}

Node::~Node()
{
	Dispose();
}

void Node::Dispose()
{
	RemoveChildren();
}

void Node::RemoveChildren()
{
	//FM_DBG_LOG1(_T("Node(%s)::RemoveChildren\n"), GetName());

	for (int i = 0; i < m_Children.GetCount(); ++i)
	{
		/*
		FM_DBG_LOG3(_T(" %3d: \"%s\" Refs = %d\n"),
			i,
			m_Children[i]->GetName(),
			m_Children[i]->GetReferenceCount());
		*/

		m_Children[i]->Parent() = 0;
		m_Children[i] = 0;
	}

	m_Children.Clear();
}

void Node::AttachChild(Spatial* pChild)
{
	assert(pChild != 0 && pChild->Parent() == 0);
	
	m_Children.Add(pChild);

	pChild->Parent() = this;

	pChild->Flags() |= FLAGS_TRANSFORM_DIRTY;
}

void Node::DetachChild(Spatial* pChild)
{
	assert(pChild != 0);
	
	SpatialPtr spSave(pChild);

	m_Children.Remove(m_Children.IndexOf(pChild));

	pChild->Parent() = 0;
}

void Node::SetAlpha(FLOAT fAlpha)
{
	for (int i = 0; i < m_Children.GetCount(); ++i)
	{
		m_Children[i]->SetAlpha(fAlpha);
	}
}

void Node::UpdateChildren()
{
	bool bFirst = true;

	m_pWorldBound->Center() = GetWorldTranslation();
	m_pWorldBound->Radius() = 0.0f;
	
	for (int i = 0; i < m_Children.GetCount(); ++i)
	{
		Spatial* pSpatial = m_Children[i];
		
		if (Flags() & FLAGS_TRANSFORM_UPDATED)
			pSpatial->Flags() |= FLAGS_TRANSFORM_DIRTY;

		pSpatial->Update(false);

		if ((pSpatial->Flags() & FLAGS_FORCE_CULLING) == 0 &&
			(pSpatial->Flags() & FLAGS_EXCLUDE_BOUND_MERGE) == 0 &&
			pSpatial->GetWorldBound()->Radius() > 0.001f)
		{
			if (bFirst)
			{
				m_pWorldBound->Copy(pSpatial->GetWorldBound());
				bFirst = false;
			}
			else
			{
				m_pWorldBound->Merge(pSpatial->GetWorldBound());
			}
		}

		if (pSpatial->IsTransformUpdated())
		{
			Flags() |= FLAGS_UPDATE_VISIBILITY;
		}
	}
}

void Node::Update(bool bInit)
{
	Spatial::Update(bInit);

	UpdateChildren();
}

void Node::OnGetRenderList(Camera* pCamera)
{
	for (int i = 0; i < m_Children.GetCount(); ++i)
	{
		m_Children[i]->GetRenderList(pCamera);
	}
}

bool Node::GetBox(AxisBox* pBox)
{
	Spatial::GetBox(pBox);

	bool bOk = false;

	AxisBox Box;

	for (int i = 0; i < m_Children.GetCount(); ++i)
	{
		Box.Initialize();

		if ((m_Children[i]->Flags() & FLAGS_EXCLUDE_BOUND_MERGE) == 0)
		{
			if (m_Children[i]->GetBox(&Box))
			{
				pBox->Update(Box.GetMin());
				pBox->Update(Box.GetMax());
				bOk = true;
			}
		}
	}	

	return bOk;
}

Spatial* Node::Copy() const
{
	Node* pNode = new Node();

	CopyProperties(pNode);

	for (int i = 0; i < m_Children.GetCount(); ++i)
	{
		pNode->AttachChild(m_Children[i]->Copy());
	}

	return pNode;
}

void Node::Pick(
	const D3DXVECTOR3& vOrigin, 
	const D3DXVECTOR3& vDirection,
	PickData& rData)
{
	if (Flags() & FLAGS_EXCLUDE_PICK)
		return;

	if (!GetWorldBound()->RayIntersection(vOrigin, vDirection))
	{
		// Doesn't hit this, doesn't hit children.
		return;
	}

	for (int i = 0; i < m_Children.GetCount(); ++i)
	{
		m_Children[i]->Pick(vOrigin, vDirection, rData);
	}
}

int Node::NumChildren() const
{
	return m_Children.GetCount();
}

Spatial* Node::GetChildAt(int index) const
{
	return m_Children[index];
}

bool Node::IsLeaf() const
{
	return true;
}

}
}