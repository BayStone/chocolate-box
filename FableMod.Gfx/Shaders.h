#pragma once

#include <d3d9.h>
#include <d3dx9.h>

inline void LoadMatrix4(
	IDirect3DDevice9* pDevice, DWORD dwCReg, D3DXMATRIX* pMatrix)
{
	pDevice->SetVertexShaderConstantF(dwCReg,(const float*)pMatrix,4);
}
