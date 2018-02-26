#include "Stdafx.h"
#include "SubMesh.h"

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

SubMesh::SubMesh(SUBM* submesh)
{
	m_Submesh = submesh;
}

unsigned int SubMesh::MaterialID::get()
{
	return m_Submesh->MaterialID;
}

void SubMesh::MaterialID::set(unsigned int id)
{
	m_Submesh->MaterialID = id;
}

unsigned int SubMesh::DestroyableMeshLevel::get()
{
	return m_Submesh->DestroyableMeshLevel;
}

unsigned int SubMesh::VertexCount::get()
{
	return m_Submesh->Number_Verts;
}

unsigned int SubMesh::FaceCount::get()
{
	return m_Submesh->Number_Faces;
}

}
}
}