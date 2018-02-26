#pragma once

#include <d3d9.h>
#include <d3d9types.h>
#include "GfxConf.h"
#include "Spatial.h"
#include "Device.h"

namespace FableMod
{
namespace Gfx
{

class Node;

/// Manager is the main D3D controller singleton class.
class GFX_API Manager
{
public:

	virtual ~Manager();

	static void Initialize(
		HWND hWnd, unsigned int uiWidth, unsigned int uiHeight);

	static void Dispose();

	static void SetDirectory(LPCTSTR pszDirectory);

	static LPCTSTR GetDirectory();

	static IDirect3D9* GetD3D() { return m_pD3D; }

	static D3DCAPS9* GetCaps() { return &m_D3DCaps; }

	static Device* GetDevice() { return m_pDevice; }

protected:

	Manager();

	static TCHAR* m_pszDirectory;

	static IDirect3D9* m_pD3D;
	static Device* m_pDevice;
	static D3DCAPS9 m_D3DCaps;	
};

}
}