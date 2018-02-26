#include "stdafx.h"
#include "Node.h"
#include "Manager.h"
#include "Helpers.h"
#include <tchar.h>

namespace FableMod
{
namespace Gfx
{

static const unsigned int PRIMITIVE_VERTEX_SIZE =
	sizeof(D3DXVECTOR3)+
	sizeof(D3DCOLOR);

static const DWORD FVF_PRITIMIVE_VERTEX = D3DFVF_XYZ | D3DFVF_DIFFUSE;

struct SPrimitiveVertex
{
	D3DXVECTOR3	vPos;
	D3DCOLOR	Color;
};

IDirect3D9* Manager::m_pD3D = 0;
D3DCAPS9 Manager::m_D3DCaps;	
Device* Manager::m_pDevice = 0;
TCHAR* Manager::m_pszDirectory = 0;

Manager::Manager()
{
}

Manager::~Manager()
{
	Dispose();
}

void Manager::Initialize(HWND hWnd, 
						 unsigned int uiWidth, 
						 unsigned int uiHeight)
{
	Dispose();

	FM_LOG2(_T("Manager::Initialize(%u,%u)\n"), uiWidth, uiHeight);

	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!m_pD3D)
	{	
		throw Exception(_T("FABLEMOD_GFX_D3D_INITIALIZE_FAILED"));
	}

	m_pD3D->GetDeviceCaps(0,D3DDEVTYPE_HAL,&m_D3DCaps);

	m_pDevice = new Device();

	// Create the base device.
	m_pDevice->Create(hWnd, uiWidth, uiHeight);
}

void Manager::Dispose()
{
	FM_LOG0(_T("Manager::Dispose...\n"));

	GFX_SAFE_DELETE(m_pDevice);
	GFX_SAFE_RELEASE(m_pD3D);
}

void Manager::SetDirectory(LPCTSTR pszDirectory)
{
	delete[] m_pszDirectory;

	size_t size = _tcslen(pszDirectory)+1;

	m_pszDirectory = new TCHAR[size];
	
	_tcscpy_s(m_pszDirectory, size, pszDirectory);
}

LPCTSTR Manager::GetDirectory()
{
	return m_pszDirectory;
}

}
}