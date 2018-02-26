#pragma once

#include "Texture.h"

namespace FableMod
{
namespace Gfx
{

/// EditableTexture implements functionality for texture editing.
/// The intention is to use it as a painting surface.
class GFX_API EditableTexture : public Texture
{
public:

	EditableTexture();

	virtual ~EditableTexture();

	virtual void Dispose();

	EditableTexture* Duplicate();

	void CopyTo(EditableTexture* pTexture, const RECT* pArea = 0);

	void Blit(
		Texture* pSource,
		const RECT* prcSrc = 0,
		const RECT* prcDst = 0,
		unsigned char ucAlpha = 255);

	void Lock();

	void Clear(D3DCOLOR Color);
	
	void DrawPixel(int iX, int iY, D3DCOLOR Color);

	void DrawRawPixel(int iX, int iY, D3DCOLOR Color);

	D3DCOLOR GetPixel(int iX, int iY);

	D3DCOLOR GetRawPixel(int iX, int iY);

	void DrawThickLine(
		int iX1, int iY1,
		int iX2, int iY2,
		D3DCOLOR Color,
		int iThickness = 1);

	void DrawLine(
		int iX1, int iY1,
		int iX2, int iY2,
		D3DCOLOR Color);

	void DrawRawLine(
		int iX1, int iY1,
		int iX2, int iY2,
		D3DCOLOR Color);

	void DrawFilledRect(
		int iLeft, int iTop,
		int iRight, int iBottom,
		D3DCOLOR Color);

	void DrawCenterRect(int iX, int iY, int iSize, D3DCOLOR Color);

	void DrawFilledCircle(
		int iX,
		int iY,
		int iRadius,
		D3DCOLOR Color);

	void DrawCircle(
		int iX, int iY,
		int iRadius,
		D3DCOLOR Color,
		int iThickness = 1);

	void Unlock();

	LPDIRECT3DTEXTURE9 GetD3DTexture() const { return m_pTexture; }

protected:

	bool ClipLine(int& riX1, int& riY1, int& riX2, int& riY2);

	unsigned int m_uiLocks;
	D3DLOCKED_RECT m_LockedRect;
	D3DSURFACE_DESC m_Desc;
};

GFX_DECLARE_SMARTPOINTER(EditableTexture);

}
}