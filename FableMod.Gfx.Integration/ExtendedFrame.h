#pragma once

#include <windows.h>
#include <memory.h>
#include <d3d9.h>
#include <d3dx9math.h>

using namespace System;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

struct D3DXEXTENDEDFRAME : public D3DXFRAME
{
	ID3DXMesh* pSkinnedMesh;
	FableMod::Gfx::TexturePtr spTexture;
	FableMod::Gfx::TexturePtr spBumpMap;
	FableMod::Gfx::TexturePtr spReflectMap;
	bool bAlpha;
	D3DXMATRIX* BoneMatrices;
	
	D3DXEXTENDEDFRAME()
	{
		memset(this, 0, sizeof(*this));
	}

	~D3DXEXTENDEDFRAME()
	{
		Destroy();
	}

	void Destroy();
};
	
}
}
}