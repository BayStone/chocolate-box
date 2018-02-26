#include "stdafx.h"
#include "LODNode.h"
#include "Camera.h"
#include "LogFile.h"

namespace FableMod
{
namespace Gfx
{

LODNode::LODNode() :
m_pBound(new BoundSphere())
{
}

LODNode::~LODNode()
{
	Dispose();

	delete m_pBound;
}

void LODNode::Dispose()
{
	Node::Dispose();
}

void LODNode::Update()
{
	// Don't update the children.

	Spatial::Update();

	GetWorldBound()->Copy(m_pBound);
}

void LODNode::OnGetRenderList(Camera* pCamera)
{
	D3DXVECTOR3 vDist(pCamera->GetPosition()-GetWorldTranslation());
	
	const FLOAT fDist = D3DXVec3Length(&vDist);

	Spatial* pSelected = 0;

	FLOAT fStep = pCamera->GetFrustumFar()/m_Children.GetCount();
	FLOAT fMin = 0.0f;

	for (int i = 0; i < m_Children.GetCount(); ++i)
	{
		m_Children[i]->Flags() |= FLAGS_FORCE_CULLING;
	}

	for (int i = 0; i < m_Children.GetCount(); ++i)
	{
		if (fDist >= fMin && fDist < fMin+fStep)
		{
			pSelected = m_Children[i];
			break;
		}
		
		fMin += fStep;
	}

	if (!pSelected)
		return;

	pSelected->Flags() &= ~FLAGS_FORCE_CULLING;
    pSelected->Flags() |= FLAGS_TRANSFORM_DIRTY;
    
	UpdateChildren();

	m_pBound->Copy(pSelected->GetWorldBound());
	
	Node::OnGetRenderList(pCamera);
}


Spatial* LODNode::Copy() const
{
	LODNode* pNode = new LODNode();

	CopyProperties(pNode);

	for (int i = 0; i < m_Children.GetCount(); ++i)
	{
		pNode->AttachChild(m_Children[i]->Copy());
	}

	return pNode;
}

}
}