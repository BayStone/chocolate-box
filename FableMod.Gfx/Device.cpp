#include "stdafx.h"
#include "Node.h"
#include "Device.h"
#include "Helpers.h"
#include "Manager.h"
#include <tchar.h>
#include <dxerr.h>

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

Device::Device() :
m_pD3DDevice(0),
m_pFont(0),
m_pBoundSphere(0),
m_pVB(0),
m_pInkShader(0),
m_uiNumVertices(0),
m_pPrimitiveSB(0),
m_pSavedPrimitiveSB(0),
m_bReady(false),
m_uiWidth(0),
m_uiHeight(0),
m_hWnd(0),
m_uiPrimitiveScene(0),
m_uiScene(0),
m_bInkOn(false)
{
}

Device::~Device()
{
	Dispose();
}

void Device::Create(HWND hWnd, unsigned int uiWidth, unsigned int uiHeight)
{
	Dispose();

	FM_LOG2(_T("Device::Create(%u, %u)\n"), uiWidth, uiHeight);

	IDirect3D9* pD3D = Manager::GetD3D();
	D3DCAPS9* pCaps = Manager::GetCaps();

	D3DPRESENT_PARAMETERS d3dpp;
			
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.BackBufferWidth = uiWidth;
	d3dpp.BackBufferHeight = uiHeight;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.Windowed = true;
	d3dpp.hDeviceWindow = hWnd;	
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	
	/*
	if (SUCCEEDED(m_pD3D->CheckDeviceMultiSampleType(
		0,
		D3DDEVTYPE_HAL,
		d3dpp.BackBufferFormat,
		FALSE,
		D3DMULTISAMPLE_2_SAMPLES,
		NULL)))
	{
		d3dpp.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;
	}
	*/

	// Assume software vertices.
	DWORD dwBehavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	if (pCaps->DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		// Use hardware for transform and lighting.
		dwBehavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	
	dwBehavior |= D3DCREATE_MULTITHREADED;

	if (FAILED(pD3D->CreateDevice(
		0, 
		D3DDEVTYPE_HAL, 
		hWnd, 
		dwBehavior, 
		&d3dpp,
		&m_pD3DDevice)))
	{
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

		GFX_TRYD3D(pD3D->CreateDevice(
			0, 
			D3DDEVTYPE_HAL, 
			hWnd, 
			dwBehavior, 
			&d3dpp,
			&m_pD3DDevice));
	}
	
	m_hWnd = hWnd;
	m_uiWidth = uiWidth;
	m_uiHeight = uiHeight;

	GetStateBlocks();

	SetDefaultRenderStates();

	m_PresentParams = d3dpp;

	m_bReady = true;
}

void Device::Dispose()
{
	FM_LOG0(_T("Device::Dispose...\n"));

	m_bReady = false;

	GFX_SAFE_RELEASE(m_pInkShader);
	GFX_SAFE_RELEASE(m_pPrimitiveSB);
	GFX_SAFE_RELEASE(m_pSavedPrimitiveSB);
	GFX_SAFE_RELEASE(m_pVB);
	GFX_SAFE_RELEASE(m_pFont);
	GFX_SAFE_RELEASE(m_pBoundSphere);
	GFX_SAFE_RELEASE(m_pD3DDevice);
	
	m_uiNumVertices = 0;

	m_hWnd = 0;
}

void Device::SetDefaultRenderStates()
{
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	m_pD3DDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	m_pD3DDevice->SetRenderState(D3DRS_AMBIENT, 0xFFFFFFFF);
	m_pD3DDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE);
	
	//m_pD3DDevice->SetRenderState(D3DRS_COLORVERTEX, TRUE);
	//m_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	
	m_pD3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	
    m_pD3DDevice->SetRenderState(D3DRS_CLIPPING, TRUE);

	m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	
	m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	
	m_pD3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_NONE);

#ifdef GFX_LH
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
#else
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
#endif
}

void Device::SetPrimitiveRenderStates()
{
	// Set the renderstates for primitive rendering, rectangles, etc.
	static D3DXMATRIX D3DXMATRIX_ID(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);

	m_pD3DDevice->SetTexture(0,NULL);

	m_pD3DDevice->SetTransform(D3DTS_WORLD,&D3DXMATRIX_ID);

	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pD3DDevice->SetRenderState(D3DRS_COLORVERTEX, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
	m_pD3DDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
    m_pD3DDevice->SetRenderState(D3DRS_CLIPPING, TRUE);
    m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pD3DDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, FALSE);
    m_pD3DDevice->SetRenderState(D3DRS_VERTEXBLEND, FALSE);
    m_pD3DDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
    m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	
	m_pD3DDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_MIRROR);
	m_pD3DDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_MIRROR);
	
	m_pD3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_NONE);
	
	m_pD3DDevice->SetSamplerState(1,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(1,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(1,D3DSAMP_MIPFILTER,D3DTEXF_NONE);

	m_pD3DDevice->SetTextureStageState(0,D3DTSS_TEXCOORDINDEX, 0);
    m_pD3DDevice->SetTextureStageState(0,
		D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);
    m_pD3DDevice->SetTextureStageState(1,D3DTSS_COLOROP,D3DTOP_DISABLE);
    m_pD3DDevice->SetTextureStageState(1,D3DTSS_ALPHAOP,D3DTOP_DISABLE);
}

void Device::GetStateBlocks()
{
	// Get renderstate blocks.

	m_pD3DDevice->CreateStateBlock(D3DSBT_ALL, &m_pPrimitiveSB);
	m_pD3DDevice->CreateStateBlock(D3DSBT_ALL, &m_pSavedPrimitiveSB);

	m_pD3DDevice->BeginStateBlock();

	SetPrimitiveRenderStates();
	
	m_pD3DDevice->EndStateBlock(&m_pPrimitiveSB);	
	
	m_pD3DDevice->BeginStateBlock();

	SetPrimitiveRenderStates();
	
	m_pD3DDevice->EndStateBlock(&m_pSavedPrimitiveSB);	
}

Device::EDeviceState Device::CheckState()
{
	if (!m_bReady)
		return DS_NOT_READY;

	HRESULT hr = m_pD3DDevice->TestCooperativeLevel();

	if (hr == D3DERR_DEVICELOST)
	{
		return DS_LOST;
	}
	else if (hr == D3DERR_DEVICENOTRESET)
	{
		return DS_RESET;
	}

	return DS_OK;
}

bool Device::Reset()
{
	HRESULT hr = m_pD3DDevice->Reset(&m_PresentParams);

	if (FAILED(hr))
	{
		FM_LOG0(_T("Device::Reset:\n"));
		FM_LOG1(_T(" Error:       %s\n"), DXGetErrorString(hr));
		FM_LOG1(_T(" Description: %s\n"), DXGetErrorDescription(hr));
	
		return false;
	}

	return true;
}

void Device::Lock()
{
	m_Mutex.Lock();
}

void Device::Unlock()
{
	m_Mutex.Unlock();
}

bool Device::BeginScene()
{
	if (++m_uiScene == 1)
	{
		return SUCCEEDED(m_pD3DDevice->BeginScene());
	}
	else
		return true;
}

void Device::EndScene()
{
	if (m_uiScene > 1)
		--m_uiScene;
	else if (m_uiScene == 1)
	{
		m_pD3DDevice->EndScene();
		m_uiScene = 0;
	}
}

bool Device::BeginPrimitiveScene()
{
	if (BeginScene())
	{
		if (++m_uiPrimitiveScene == 1)
		{
			m_pSavedPrimitiveSB->Capture();
		}

		// Always set the primitive states.
		m_pPrimitiveSB->Apply();

		return true;
	}

	return false;
}

void Device::EndPrimitiveScene()
{
	if (m_uiPrimitiveScene > 1)
	{
		--m_uiPrimitiveScene;
	}
	else if (m_uiPrimitiveScene == 1)
	{
		m_pSavedPrimitiveSB->Apply();
		m_uiPrimitiveScene = 0;
	}

	EndScene();
}

void Device::ActivateInk(bool bInk)
{
	m_bInkOn = bInk;
}

void Device::DrawText(
	int iX, int iY, LPCTSTR pszText,
	D3DCOLOR Color)
{
	if (!m_pFont)
	{
		D3DXCreateFont( 
			m_pD3DDevice, 
			15, 
			0, 
			FW_BOLD, 
			1, 
			FALSE, 
			DEFAULT_CHARSET, 
            OUT_DEFAULT_PRECIS, 
			DEFAULT_QUALITY, 
			DEFAULT_PITCH | FF_DONTCARE, 
			_T("Arial"), 
			&m_pFont);
	}
	
	if (m_pFont)
	{
		RECT rc;
		
		rc.left = iX;
		rc.top = iY;
		rc.bottom = 0;
		rc.right = 0;

		if (BeginScene())
		{
			m_pFont->DrawText(
				NULL, 
				pszText, 
				-1, 
				&rc, 
				DT_NOCLIP, 
				Color);

			EndScene();
		}
	}
}

void Device::DrawSphere(
	const D3DXVECTOR3& vCenter,
	FLOAT fRadius,
	const D3DCOLORVALUE& Color)
{
	/*
	if (!m_pBoundSphere)
	{
		D3DXCreateSphere(
			GetDevice(),
			10.0f,
			8,
			8,
			&m_pBoundSphere,
			0);
	}

	if (m_pBoundSphere)
	{
	*/
	if (FAILED(D3DXCreateSphere(
		m_pD3DDevice,
		fRadius,
		8,
		8,
		&m_pBoundSphere,
		0)))
	{
		return;
	}
	
	D3DXMATRIX matTrn,matScl,matWorld;

	D3DXMatrixTranslation(
		&matTrn,
		vCenter.x,
		vCenter.y,
		vCenter.z);

	matWorld = matTrn;

	if (BeginScene())
	{
		DWORD dwLights;

		m_pD3DDevice->GetRenderState(D3DRS_LIGHTING, &dwLights);
		
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
			
		m_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

		D3DMATERIAL9 Material;

		ZeroMemory(&Material,sizeof(Material));

		Material.Diffuse = Color;
		Material.Ambient = Material.Diffuse;

		m_pD3DDevice->SetMaterial(&Material);
		m_pD3DDevice->SetTexture(0,0);
		
		m_pBoundSphere->DrawSubset(0);
	
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, dwLights);
		
		EndScene();
	}

	m_pBoundSphere->Release();
	m_pBoundSphere = 0;
}

void Device::CheckVertexBuffer(unsigned int uiVertices)
{
	if (m_uiNumVertices < uiVertices)
	{
		// Must realloacte.
		m_uiNumVertices = uiVertices;

		// Release old buffer.
		GFX_SAFE_RELEASE(m_pVB);

		m_pD3DDevice->CreateVertexBuffer(
			m_uiNumVertices*PRIMITIVE_VERTEX_SIZE,
			D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
			0,
			D3DPOOL_DEFAULT,
			&m_pVB,
			0);
	}
}

void Device::DrawLine(
	const D3DXVECTOR3& vPoint1,
	const D3DXVECTOR3& vPoint2,
	const D3DCOLOR& Color)
{
	CheckVertexBuffer(2);
	
	if (BeginPrimitiveScene())
	{
		SPrimitiveVertex* pVertices = 0;

		if (SUCCEEDED(m_pVB->Lock(0,0,(VOID**)&pVertices,D3DLOCK_DISCARD)))
		{
			pVertices->vPos  = vPoint1;
			pVertices->Color = Color; 
			pVertices++;
			pVertices->vPos  = vPoint2;
			pVertices->Color = Color; 
			pVertices++;
			
			m_pVB->Unlock();

			m_pD3DDevice->SetVertexShader(0);
			m_pD3DDevice->SetFVF(FVF_PRITIMIVE_VERTEX);
			m_pD3DDevice->SetStreamSource(
				0,m_pVB,0,sizeof(SPrimitiveVertex));
			
			m_pD3DDevice->DrawPrimitive(D3DPT_LINELIST,0,1);
		}

		EndPrimitiveScene();
	}
}

void Device::DrawBox(
	const D3DXVECTOR3& vMin,
	const D3DXVECTOR3& vMax,
	const D3DCOLOR& Color)
{
	CheckVertexBuffer(10);
	
	if (BeginPrimitiveScene())
	{
		m_pD3DDevice->SetVertexShader(0);
		m_pD3DDevice->SetFVF(FVF_PRITIMIVE_VERTEX);
			
		SPrimitiveVertex* pVertices = 0;

		if (SUCCEEDED(m_pVB->Lock(0,0,(VOID**)&pVertices,D3DLOCK_DISCARD)))
		{
			// Back - topleft
			pVertices->vPos  = D3DXVECTOR3(vMax.x,vMax.y,vMax.z);
			pVertices->Color = Color; 
			pVertices++;

			// Back - topright
			pVertices->vPos  = D3DXVECTOR3(vMin.x,vMax.y,vMax.z);
			pVertices->Color = Color; 
			pVertices++;
			
			// Back - bottomright
			pVertices->vPos  = D3DXVECTOR3(vMin.x,vMax.y,vMin.z);
			pVertices->Color = Color; 
			pVertices++;

			// Back - bottomleft
			pVertices->vPos  = D3DXVECTOR3(vMax.x,vMax.y,vMin.z);
			pVertices->Color = Color; 
			pVertices++;
			
			// Back - topleft
			pVertices->vPos  = D3DXVECTOR3(vMax.x,vMax.y,vMax.z);
			pVertices->Color = Color; 
			pVertices++;

			// Front - topleft
			pVertices->vPos  = D3DXVECTOR3(vMax.x,vMin.y,vMax.z);
			pVertices->Color = Color; 
			pVertices++;

			// Front - topright
			pVertices->vPos  = D3DXVECTOR3(vMin.x,vMin.y,vMax.z);
			pVertices->Color = Color; 
			pVertices++;
			
			// Front - bottomright
			pVertices->vPos  = D3DXVECTOR3(vMin.x,vMin.y,vMin.z);
			pVertices->Color = Color; 
			pVertices++;

			// Front - bottomleft
			pVertices->vPos  = D3DXVECTOR3(vMax.x,vMin.y,vMin.z);
			pVertices->Color = Color; 
			pVertices++;
			
			// Front - topleft
			pVertices->vPos  = D3DXVECTOR3(vMax.x,vMin.y,vMax.z);
			pVertices->Color = Color; 
			pVertices++;

			m_pVB->Unlock();

			m_pD3DDevice->SetStreamSource(
				0,m_pVB,0,sizeof(SPrimitiveVertex));
			
			m_pD3DDevice->DrawPrimitive(D3DPT_LINESTRIP,0,9);
		}

		
		if (SUCCEEDED(m_pVB->Lock(0,0,(VOID**)&pVertices,D3DLOCK_DISCARD)))
		{
			// Back - topright
			pVertices->vPos  = D3DXVECTOR3(vMin.x,vMax.y,vMax.z);
			pVertices->Color = Color; 
			pVertices++;
			
			// Front - topright
			pVertices->vPos  = D3DXVECTOR3(vMin.x,vMin.y,vMax.z);
			pVertices->Color = Color; 
			pVertices++;
			
			// Back - bottomright
			pVertices->vPos  = D3DXVECTOR3(vMin.x,vMax.y,vMin.z);
			pVertices->Color = Color; 
			pVertices++;

			// Front - bottomright
			pVertices->vPos  = D3DXVECTOR3(vMin.x,vMin.y,vMin.z);
			pVertices->Color = Color; 
			pVertices++;

			// Back - bottomleft
			pVertices->vPos  = D3DXVECTOR3(vMax.x,vMax.y,vMin.z);
			pVertices->Color = Color; 
			pVertices++;
			
			// Front - bottomleft
			pVertices->vPos  = D3DXVECTOR3(vMax.x,vMin.y,vMin.z);
			pVertices->Color = Color; 
			pVertices++;
			
			m_pVB->Unlock();

			m_pD3DDevice->SetStreamSource(
				0,m_pVB,0,sizeof(SPrimitiveVertex));
			
			m_pD3DDevice->DrawPrimitive(D3DPT_LINELIST,0,3);
		}

		EndPrimitiveScene();
	}
}


IDirect3DVertexShader9* Device::BuildInkShader()
{
	if (!m_pInkShader)
	{
		ID3DXBuffer* pShaderBuffer = 0;

		TCHAR szTmp[MAX_PATH];

		_stprintf_s(szTmp,MAX_PATH,_T("%s%s"),
			Manager::GetDirectory(),_T("fablemod_ink.vsh"));

		HRESULT hr = D3DXAssembleShaderFromFile(
			szTmp,
			0,
			0,
			0,
			&pShaderBuffer,
			0);
		
		if (FAILED(hr))
		{
			GFX_SAFE_RELEASE(pShaderBuffer);
			return 0;
		}

		// Create the shader.
		hr = m_pD3DDevice->CreateVertexShader(
			(DWORD*)pShaderBuffer->GetBufferPointer(),
			&m_pInkShader);
		
		GFX_SAFE_RELEASE(pShaderBuffer);
		
		if (FAILED(hr))
		{
			return 0;
		}
	}

	return m_pInkShader;
}

void Device::ClearBuffers(D3DCOLOR Color)
{
	m_pD3DDevice->Clear(
		0, 
		NULL, 
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
		Color, 
		1.0f, 
		0);
}

void Device::Present(HWND hWnd, const RECT* pSrc, const RECT* pDest)
{
	if (!hWnd)
		hWnd = m_hWnd;

	Lock();

	m_pD3DDevice->Present(pSrc, pDest, hWnd, NULL);

	Unlock();
}

}
}