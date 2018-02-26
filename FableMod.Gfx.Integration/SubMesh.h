#pragma once

#include "ModelStructs.h"

using namespace System;
using namespace FableMod::BIG;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

public ref class SubMesh
{
public:

	SubMesh(SUBM* submesh);
	
	property unsigned int MaterialID
	{
		unsigned int get();
		void set(unsigned int id);
	}
	
	property unsigned int DestroyableMeshLevel
	{
		unsigned int get();
	}
	
	property unsigned int VertexCount
	{
		unsigned int get();
	}

	property unsigned int FaceCount
	{
		unsigned int get();
	}

private:

	SUBM* m_Submesh;
};

}
}
}