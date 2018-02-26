#include "stdafx.h"
#include "OctTree.h"

namespace FableMod
{
namespace Gfx
{
	OctTree::OctTree() :
	m_UpdateObjects(128,16)
	{
	}

	OctTree::~OctTree()
	{
		Dispose();
	}
	
	void OctTree::AttachChild(Spatial* pChild)
	{
		AttachToTree(pChild);
	}

	void OctTree::UpdateTree()
	{
		m_UpdateObjects.Clear();

		GetUpdatable(m_UpdateObjects);

		for (int i = 0; i < m_UpdateObjects.GetCount(); ++i)
		{
			AttachToTree(m_UpdateObjects[i]);
		}
	}
}
}