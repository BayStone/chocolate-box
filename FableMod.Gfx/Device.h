#pragma once

#include <d3d9.h>
#include <d3d9types.h>
#include "GfxConf.h"
#include "Mutex.h"

namespace FableMod
{
namespace Gfx
{

class GFX_API Device
{
public:

	enum EDeviceState
	{
		DS_OK = 0,
		DS_RESET = 1,
		DS_NOT_READY = -1,
		DS_LOST = -2,
	};

	Device();

	virtual ~Device();

	void Dispose();

	void Create(HWND hWnd, unsigned int uiWidth, unsigned int uiHeight);

	void ClearBuffers(D3DCOLOR Color);

	void Present(
		HWND hWnd = 0, const RECT* pSrc = 0, const RECT* pDest = 0);

	void SetDefaultRenderStates();

	void ActivateInk(bool bInk);

	void DrawText(
		int iX, int iY, LPCTSTR pszText,
		D3DCOLOR Color);

	void DrawSphere(
		const D3DXVECTOR3& vCenter,
		FLOAT fRadius,
		const D3DCOLORVALUE& Color);

	void DrawLine(
		const D3DXVECTOR3& vPoint1,
		const D3DXVECTOR3& vPoint2,
		const D3DCOLOR& Color);

	void DrawBox(
		const D3DXVECTOR3& vMin,
		const D3DXVECTOR3& vMax,
		const D3DCOLOR& Color);

	EDeviceState CheckState();

	bool Reset();

	void Lock();

	void Unlock();

	bool BeginScene();

	void EndScene();

	bool BeginPrimitiveScene();

	void EndPrimitiveScene();

	IDirect3DVertexShader9* BuildInkShader();

	IDirect3DDevice9* GetD3DDevice() { return m_pD3DDevice; }

	unsigned int GetWidth() { return m_uiWidth; }

	unsigned int GetHeight() { return m_uiHeight; }

	bool IsInkOn() { return m_bInkOn; }

	bool InScene() { return m_uiScene > 0; }

	bool IsReady() { return m_bReady; }

protected:

	void CheckVertexBuffer(unsigned int uiVertices);

	void SetPrimitiveRenderStates();

	void GetStateBlocks();

	IDirect3DDevice9* m_pD3DDevice;
	LPD3DXMESH m_pBoundSphere;
	LPD3DXFONT m_pFont;
	IDirect3DVertexBuffer9* m_pVB;
	unsigned int m_uiNumVertices;
	IDirect3DStateBlock9* m_pPrimitiveSB;
	IDirect3DStateBlock9* m_pSavedPrimitiveSB;
	IDirect3DVertexShader9* m_pInkShader;
	D3DPRESENT_PARAMETERS m_PresentParams;

	unsigned int m_uiScene;
	unsigned int m_uiPrimitiveScene;
	bool m_bInkOn;

	HWND m_hWnd;
	int m_uiWidth;
	int m_uiHeight;
	bool m_bReady;

	Mutex m_Mutex;
};

}
}