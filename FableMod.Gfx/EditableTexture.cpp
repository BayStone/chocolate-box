#include "stdafx.h"
#include "EditableTexture.h"
#include "Manager.h"
#include <d3dx9.h>
#include <exception>
#include <cassert>
#include <tchar.h>
#include "Helpers.h"

namespace FableMod
{
namespace Gfx
{

EditableTexture::EditableTexture() :
m_uiLocks(0)
{
}

EditableTexture::~EditableTexture()
{
	Dispose();
}

void EditableTexture::Dispose()
{
	Texture::Dispose();
}

EditableTexture* EditableTexture::Duplicate()
{
	D3DSURFACE_DESC desc;
	
	GFX_TRYD3D(m_pTexture->GetLevelDesc(0, &desc));

	EditableTexture* pTexture = new EditableTexture();

	pTexture->Create(desc.Width, desc.Height, false, AlphaRequired());
	
	return pTexture;
}

void EditableTexture::CopyTo(EditableTexture* pTexture, const RECT* pArea)
{
	/*
	Lock();
	pTexture->Lock();

	if (m_Desc.Format == D3DFMT_X8R8G8B8 ||
		m_Desc.Format == D3DFMT_A8R8G8B8)
	{
		RECT rc;
		
		if (pArea)
			rc = *pArea;
		else
		{
			rc.left = 0;
			rc.top = 0;
			rc.right = m_Desc.Width;
			rc.bottom = m_Desc.Height;
		}

		

		for (unsigned int i = rc.top; i < rc.bottom; ++i)
		{
			for (unsigned int j = rc.left; j < rc.right; ++j)
			{
				//DrawRawPixel(j, i, pTexture->GetRawPixel(j, i));
			}
		}
	}

	pTexture->Unlock();
	Unlock();
	*/

	LPDIRECT3DSURFACE9 pSrc,pDst;

	GFX_TRYD3D(m_pTexture->GetSurfaceLevel(0, &pSrc));
	GFX_TRYD3D(pTexture->m_pTexture->GetSurfaceLevel(0, &pDst));

	GFX_TRYD3D(D3DXLoadSurfaceFromSurface(
		pDst, 
		0, 
		pArea, 
		pSrc, 
		0,
		pArea, 
		D3DX_FILTER_NONE, 
		0));

	GFX_SAFE_RELEASE(pSrc);
	GFX_SAFE_RELEASE(pDst);
}

void EditableTexture::Lock()
{
	if (++m_uiLocks == 1)
	{		
		GFX_TRYD3D(m_pTexture->LockRect(0, &m_LockedRect, 0, 0));
		GFX_TRYD3D(m_pTexture->GetLevelDesc(0, &m_Desc));
	}
}


void EditableTexture::Unlock()
{
	if (m_uiLocks == 1)
	{
		m_pTexture->UnlockRect(0);
		m_uiLocks = 0;
	}
	else
		--m_uiLocks;
}

void EditableTexture::Blit(
	Texture* pSource,
	const RECT* prcSrc,
	const RECT* prcDst,
	unsigned char ucAlpha)
{
	if (ucAlpha == 255)
	{
		LPDIRECT3DSURFACE9 pSrc,pDst;

		GFX_TRYD3D(pSource->GetD3DTexture()->GetSurfaceLevel(0, &pSrc));
		GFX_TRYD3D(m_pTexture->GetSurfaceLevel(0, &pDst));

		GFX_TRYD3D(D3DXLoadSurfaceFromSurface(
			pDst, 0, prcDst, pSrc, 0, prcSrc, D3DX_FILTER_LINEAR, 0));

		GFX_SAFE_RELEASE(pSrc);
		GFX_SAFE_RELEASE(pDst);
	}
	else
	{
		D3DSURFACE_DESC Desc;

		pSource->GetD3DTexture()->GetLevelDesc(0, &Desc);

		if (Desc.Format == D3DFMT_DXT1 ||
			Desc.Format == D3DFMT_DXT3)
		{
			// Need to uncompress. Copy the compressed
			// into a temporary texture and use that.

			TexturePtr spTexture = new Texture();

			spTexture->Create(
				Desc.Width, Desc.Height, 1, false, false);

			LPDIRECT3DSURFACE9 pSrc,pDst;

			GFX_TRYD3D(
				pSource->GetD3DTexture()->GetSurfaceLevel(0, &pSrc));
			GFX_TRYD3D(
				spTexture->GetD3DTexture()->GetSurfaceLevel(0, &pDst));

			GFX_TRYD3D(D3DXLoadSurfaceFromSurface(
				pDst, 0, 0, pSrc, 0, 0, D3DX_FILTER_POINT, 0));

			GFX_SAFE_RELEASE(pSrc);
			GFX_SAFE_RELEASE(pDst);

			Blit(spTexture, prcSrc, prcDst, ucAlpha);

			spTexture = 0;
		}

		Lock();
		
		D3DLOCKED_RECT LockRect;

		GFX_TRYD3D(pSource->GetD3DTexture()->LockRect(
			0, &LockRect, 0, 0));
		
		int iSrcWidth,iSrcHeight,iDstWidth,iDstHeight;
		int iSrcX,iSrcY,iDstX,iDstY;

		if (prcSrc)
		{
			iSrcX = prcSrc->left;
			iSrcY = prcSrc->top;
			iSrcWidth = (prcSrc->right-prcSrc->left)+1;
			iSrcHeight = (prcSrc->bottom-prcSrc->top)+1;
		}
		else
		{
			iSrcX = 0;
			iSrcY = 0;
			iSrcWidth = Desc.Width;
			iSrcHeight = Desc.Height;
		}

		if (prcDst)
		{
			iDstX = prcDst->left;
			iDstY = prcDst->top;
			iDstWidth = (prcDst->right-prcDst->left)+1;
			iDstHeight = (prcDst->bottom-prcDst->top)+1;
		}
		else
		{
			iDstX = 0;
			iDstY = 0;
			iDstWidth = m_Desc.Width;
			iDstHeight = m_Desc.Height;
		}

		unsigned char* pSrcBuffer = (unsigned char*)LockRect.pBits;

		int iSrcPitch = LockRect.Pitch;

		unsigned char* pTempSrc = pSrcBuffer+(iSrcPitch*iSrcY);

		int iDstPitch = m_LockedRect.Pitch;

		unsigned char* pTempDst = 
			((unsigned char*)m_LockedRect.pBits)+(iDstPitch*iDstY);

		// Remove width in bytes from the pitch.
		iDstPitch -= iDstWidth*4;

		int iStepX = (iSrcWidth << 16)/iDstWidth;
		int iStepY = (iSrcHeight << 16)/iDstHeight;

		int iIndexX,iIndexY;

		iIndexY = iSrcY;

		unsigned int uiSrcR, uiSrcG, uiSrcB;
		unsigned int uiDstA, uiDstR, uiDstG, uiDstB;
		
		const unsigned int uiAlpha = ucAlpha;
		const unsigned int uiInvAlpha = 255-uiAlpha;
				
		for (int y = 0; y < iDstHeight; ++y)
		{
			iIndexX = iSrcX;
			
			for (int x = 0; x < iDstWidth; ++x)
			{	
				unsigned int uiPixelSrc = *((unsigned int*)
					(pTempSrc+(4*(iIndexX >> 16))));
				
				unsigned int uiPixelDst = *((unsigned int*)pTempDst);

				uiSrcR = (uiPixelSrc >> 16) & 0xFF;
				uiSrcG = (uiPixelSrc >> 8) & 0xFF;
				uiSrcB = uiPixelSrc & 0xFF;
				
				uiDstA = (uiPixelDst >> 24) & 0xFF;
				uiDstR = (uiPixelDst >> 16) & 0xFF;
				uiDstG = (uiPixelDst >> 8) & 0xFF;
				uiDstB = uiPixelDst & 0xFF;
				
				uiSrcR = (uiSrcR << 16)/uiAlpha;
				uiSrcG = (uiSrcG << 16)/uiAlpha;
				uiSrcB = (uiSrcB << 16)/uiAlpha;

				uiDstR = (uiDstR << 16)/uiInvAlpha;
				uiDstG = (uiDstG << 16)/uiInvAlpha;
				uiDstB = (uiDstB << 16)/uiInvAlpha;

				*((unsigned int*)pTempDst) =
					(uiDstA << 24) |
					(((uiDstR+uiSrcR) >> 16) << 16) |
					(((uiDstG+uiSrcG) >> 16) << 8) |
					 ((uiDstB+uiSrcB) >> 16);

				pTempDst += 4;
				iIndexX += iStepX;
			}

			iIndexY += iStepY;
			pTempSrc = pSrcBuffer+(iSrcPitch*(iIndexY >> 16));
			pTempDst += iDstPitch;
		}

		pSource->GetD3DTexture()->UnlockRect(0);

		Unlock();
	}
}

void EditableTexture::Clear(D3DCOLOR Color)
{
	Lock();

	if (m_Desc.Format == D3DFMT_X8R8G8B8 ||
		m_Desc.Format == D3DFMT_A8R8G8B8)
	{
		const unsigned int uiAdd = m_LockedRect.Pitch-(m_Desc.Width*4);

		unsigned char* pucBytes = (unsigned char*)m_LockedRect.pBits;

		for (unsigned int i = 0; i < m_Desc.Height; ++i)
		{
			for (unsigned int j = 0; j < m_Desc.Width; ++j)
			{
				*((unsigned int*)pucBytes) = Color;
				pucBytes += 4;
			}
	
			pucBytes += uiAdd;
		}
	}

	Unlock();
}

void EditableTexture::DrawRawPixel(int iX, 
								   int iY, 
								   D3DCOLOR Color)
{
	if (m_Desc.Format == D3DFMT_X8R8G8B8 ||
		m_Desc.Format == D3DFMT_A8R8G8B8)
	{
		unsigned char* pucBytes = (unsigned char*)m_LockedRect.pBits;

		*((unsigned int*)
			(pucBytes+(iY*m_LockedRect.Pitch)+(iX*4))) = Color;
	}
}

void EditableTexture::DrawPixel(int iX, 
								int iY, 
								D3DCOLOR Color)
{
	Lock();
	
	if (iX < m_Desc.Width && iY < m_Desc.Height && iX >= 0 && iY >= 0)
	{
		DrawRawPixel(iX, iY, Color);
	}

	Unlock();
}

D3DCOLOR EditableTexture::GetRawPixel(int iX, int iY)
{
	D3DCOLOR Color = 0;

	unsigned char* pucBytes = (unsigned char*)m_LockedRect.pBits;

	if (m_Desc.Format == D3DFMT_X8R8G8B8 ||
		m_Desc.Format == D3DFMT_A8R8G8B8)
	{
		Color = *((unsigned int*)
			(pucBytes+(iY*m_LockedRect.Pitch)+(iX*4)));
	}

	return Color;
}

D3DCOLOR EditableTexture::GetPixel(int iX, int iY)
{
	Lock();
	
	D3DCOLOR Color = 0;

	if (iX < m_Desc.Width && iY < m_Desc.Height && iX >= 0 && iY >= 0)
	{
		unsigned char* pucBytes = (unsigned char*)m_LockedRect.pBits;

		if (m_Desc.Format == D3DFMT_X8R8G8B8 ||
			m_Desc.Format == D3DFMT_A8R8G8B8)
		{
			Color = *((unsigned int*)
				(pucBytes+(iY*m_LockedRect.Pitch)+(iX*4)));
		}
	}

	Unlock();

	return Color;
}

bool EditableTexture::ClipLine(int& riX1, int& riY1, int& riX2, int& riY2)
{
#define CLIP_CODE_C 0x0000
#define CLIP_CODE_N 0x0008
#define CLIP_CODE_S 0x0004
#define CLIP_CODE_E 0x0002
#define CLIP_CODE_W 0x0001

#define CLIP_CODE_NE 0x000A
#define CLIP_CODE_SE 0x0006
#define CLIP_CODE_NW 0x0009
#define CLIP_CODE_SW 0x0005

	int iXC1 = riX1;
	int iYC1 = riY1;
	int iXC2 = riX2;
	int iYC2 = riY2;

	int iP1Code = 0;
	int iP2Code = 0;

	const int iMinClipX = 0;
	const int iMinClipY = 0;
	const int iMaxClipX = m_Desc.Width-1;
	const int iMaxClipY = m_Desc.Height-1;

	// Determine codes for p1 and p2.
	if (riY1 < iMinClipY)
		iP1Code |= CLIP_CODE_N;
	else if (riY1 > iMaxClipY)
		iP1Code |= CLIP_CODE_S;

	if (riX1 < iMinClipX)
		iP1Code |= CLIP_CODE_W;
	else if (riX1 > iMaxClipX)
		iP1Code |= CLIP_CODE_E;

	if (riY2 < iMinClipY)
		iP2Code |= CLIP_CODE_N;
	else if (riY2 > iMaxClipY)
		iP2Code |= CLIP_CODE_S;

	if (riX2 < iMinClipX)
		iP2Code |= CLIP_CODE_W;
	else if (riX2 > iMaxClipX)
		iP2Code |= CLIP_CODE_E;

	// Try and trivially reject.
	if ((iP1Code & iP2Code)) 
	{
		return false;
	}
	
	// Test for totally visible, if so leave points untouched.
	if (iP1Code == 0 && iP2Code == 0)
	{
		return true;
	}

	// determine end clip point for p1
	switch (iP1Code)
	{
	case CLIP_CODE_C: 
		break;

	case CLIP_CODE_N:
		{
		iYC1 = iMinClipY;
		iXC1 = riX1+0.5+(iMinClipY-riY1)*(riX2-riX1)/(riY2-riY1);
		} 
		break;

	case CLIP_CODE_S:
		{
		iYC1 = iMaxClipY;
		iXC1 = riX1+0.5+(iMaxClipY-riY1)*(riX2-riX1)/(riY2-riY1);
		} 
		break;

	case CLIP_CODE_W:
		{
		iXC1 = iMinClipX;
		iYC1 = riY1+0.5+(iMinClipX-riX1)*(riY2-riY1)/(riX2-riX1);
		} 
		break;

	case CLIP_CODE_E:
		{
		iXC1 = iMaxClipX;
		iYC1 = riY1+0.5+(iMaxClipX-riX1)*(riY2-riY1)/(riX2-riX1);
		} 
		break;

	// these cases are more complex, must compute 2 intersections
	case CLIP_CODE_NE:
		{
		// north hline intersection
		iYC1 = iMinClipY;
		iXC1 = riX1+0.5+(iMinClipY-riY1)*(riX2-riX1)/(riY2-riY1);

		// test if intersection is valid, of so then done, else compute next
		if (iXC1 < iMinClipX || iXC1 > iMaxClipX)
		{
			// east vline intersection
			iXC1 = iMaxClipX;
			iYC1 = riY1+0.5+(iMaxClipX-riX1)*(riY2-riY1)/(riX2-riX1);
		} // end if

		} 
		break;

	case CLIP_CODE_SE:
	{
		// south hline intersection
		iYC1 = iMaxClipY;
		iXC1 = riX1+0.5+(iMaxClipY-riY1)*(riX2-riX1)/(riY2-riY1);	

		// test if intersection is valid, of so then done, else compute next
		if (iXC1 < iMinClipX || iXC1 > iMaxClipX)
		{
			// east vline intersection
			iXC1 = iMaxClipX;
			iYC1 = riY1+0.5+(iMaxClipX-riX1)*(riY2-riY1)/(riX2-riX1);
		} // end if

		} 
		break;

	case CLIP_CODE_NW: 
	{
	// north hline intersection
	iYC1 = iMinClipY;
	iXC1 = riX1+0.5+(iMinClipY-riY1)*(riX2-riX1)/(riY2-riY1);

	// test if intersection is valid, of so then done, else compute next
	if (iXC1 < iMinClipX || iXC1 > iMaxClipX)
	  {
	  iXC1 = iMinClipX;
	  iYC1 = riY1+0.5+(iMinClipX-riX1)*(riY2-riY1)/(riX2-riX1);	
	  } // end if

	} 
	break;

	case CLIP_CODE_SW:
	{
	// south hline intersection
	iYC1 = iMaxClipY;
	iXC1 = riX1+0.5+(iMaxClipY-riY1)*(riX2-riX1)/(riY2-riY1);	

	// test if intersection is valid, of so then done, else compute next
	if (iXC1 < iMinClipX || iXC1 > iMaxClipX)
	  {
	  iXC1 = iMinClipX;
	  iYC1 = riY1+0.5+(iMinClipX-riX1)*(riY2-riY1)/(riX2-riX1);	
	  } // end if

	} 
	break;

	default:
		break;

	} // end switch

	// determine clip point for p2
	switch(iP2Code)
	{
	case CLIP_CODE_C: 
		break;

	case CLIP_CODE_N:
	{
	iYC2 = iMinClipY;
	iXC2 = riX2+(iMinClipY-riY2)*(riX1-riX2)/(riY1-riY2);
	} 
	break;

	case CLIP_CODE_S:
	{
	iYC2 = iMaxClipY;
	iXC2 = riX2+(iMaxClipY-riY2)*(riX1-riX2)/(riY1-riY2);
	} 
	break;

	case CLIP_CODE_W:
	{
	iXC2 = iMinClipX;
	iYC2 = riY2+(iMinClipX-riX2)*(riY1-riY2)/(riX1-riX2);
	} 
	break;

	case CLIP_CODE_E:
	{
	iXC2 = iMaxClipX;
	iYC2 = riY2+(iMaxClipX-riX2)*(riY1-riY2)/(riX1-riX2);
	} 
	break;

	// these cases are more complex, must compute 2 intersections
	case CLIP_CODE_NE:
	{
	// north hline intersection
	iYC2 = iMinClipY;
	iXC2 = riX2+0.5+(iMinClipY-riY2)*(riX1-riX2)/(riY1-riY2);

	// test if intersection is valid, of so then done, else compute next
	if (iXC2 < iMinClipX || iXC2 > iMaxClipX)
	{
	// east vline intersection
	iXC2 = iMaxClipX;
	iYC2 = riY2+0.5+(iMaxClipX-riX2)*(riY1-riY2)/(riX1-riX2);
	} // end if

	} 
	break;

	case CLIP_CODE_SE:
	{
	// south hline intersection
	iYC2 = iMaxClipY;
	iXC2 = riX2+0.5+(iMaxClipY-riY2)*(riX1-riX2)/(riY1-riY2);	

	// test if intersection is valid, of so then done, else compute next
	if (iXC2 < iMinClipX || iXC2 > iMaxClipX)
	{
	// east vline intersection
	iXC2 = iMaxClipX;
	iYC2 = riY2+0.5+(iMaxClipX-riX2)*(riY1-riY2)/(riX1-riX2);
	} // end if

	} 
	break;

	case CLIP_CODE_NW: 
	{
	// north hline intersection
	iYC2 = iMinClipY;
	iXC2 = riX2+0.5+(iMinClipY-riY2)*(riX1-riX2)/(riY1-riY2);

	// test if intersection is valid, of so then done, else compute next
	if (iXC2 < iMinClipX || iXC2 > iMaxClipX)
	{
	iXC2 = iMinClipX;
	iYC2 = riY2+0.5+(iMinClipX-riX2)*(riY1-riY2)/(riX1-riX2);	
	} // end if

	} 
	break;

	case CLIP_CODE_SW:
	{
	// south hline intersection
	iYC2 = iMaxClipY;
	iXC2 = riX2+0.5+(iMaxClipY-riY2)*(riX1-riX2)/(riY1-riY2);	

	// test if intersection is valid, of so then done, else compute next
	if (iXC2 < iMinClipX || iXC2 > iMaxClipX)
	{
	iXC2 = iMinClipX;
	iYC2 = riY2+0.5+(iMinClipX-riX2)*(riY1-riY2)/(riX1-riX2);	
	} // end if

	} 
	break;

	default:
		break;

	} // end switch

	// do bounds check
	if ((iXC1 < iMinClipX) || (iXC1 > iMaxClipX) ||
		(iYC1 < iMinClipY) || (iYC1 > iMaxClipY) ||
		(iXC2 < iMinClipX) || (iXC2 > iMaxClipX) ||
		(iYC2 < iMinClipY) || (iYC2 > iMaxClipY) )
	{
		return false;
	} 

	riX1 = iXC1;
	riY1 = iYC1;
	riX2 = iXC2;
	riY2 = iYC2;

	return true;
}

void EditableTexture::DrawThickLine(
	int iX1, int iY1,
	int iX2, int iY2,
	D3DCOLOR Color,
	int iThickness)
{
	if (iThickness == 1)
	{
		DrawLine(iX1, iY1, iX2, iY2, Color);
		return;
	}

	Lock();

	int iThick2 = iThickness/2;

	for (int y = -iThick2; y < iThick2; ++y)
	{
		for (int x = -iThick2; x < iThick2; ++x)
		{
			DrawLine(iX1+x, iY1+y, iX2+x, iY2+y, Color);
		}
	}

	DrawFilledCircle(iX1, iY1, iThick2, Color);

	DrawFilledCircle(iX2, iY2, iThick2, Color);

	Unlock();
}

//////////////////////////////////////////////////////////////////////////////
/// Draw a line from one point to another with the given color.
/// Clipping is handled automatically. Drawing is done in software.
//////////////////////////////////////////////////////////////////////////////
void EditableTexture::DrawLine(
	int iX1, int iY1,
	int iX2, int iY2,
	D3DCOLOR Color)
{
	Lock();

	if (ClipLine(iX1, iY1, iX2, iY2))
	{
		DrawRawLine(iX1, iY1, iX2, iY2, Color);
	}

	Unlock();
}

//////////////////////////////////////////////////////////////////////////////
/// Draw a line from one point to another with the given color.
/// No clipping. Drawing is done in software.
//////////////////////////////////////////////////////////////////////////////
void EditableTexture::DrawRawLine(
	int iX1, int iY1,
	int iX2, int iY2,
	D3DCOLOR Color)
{
	Lock();

	unsigned char* pucBuffer = (unsigned char*)m_LockedRect.pBits;

	int iDx = iX2-iX1;
	int iDy = iY2-iY1;
	
	if (m_Desc.Format == D3DFMT_X8R8G8B8 ||
		m_Desc.Format == D3DFMT_A8R8G8B8)
	{
		pucBuffer += (iY1*m_LockedRect.Pitch)+(iX1*4);

		int iXInc = 4;
		int iYInc = m_LockedRect.Pitch;

		if (iDx < 0)
		{
			iXInc = -iXInc;
			iDx = -iDx;
		}
		
		if (iDy < 0)
		{
			iYInc = -iYInc;
			iDy = -iDy;
		}

		const int iDx2 = iDx*2;
		const int iDy2 = iDy*2;

		if (iDx > iDy)
		{
			int iError = iDy2-iDx;

			for (int i = 0; i <= iDx; ++i)
			{
				*((unsigned int*)pucBuffer) = (DWORD)Color;

				if (iError >= 0)
				{
					iError -= iDx2;
					pucBuffer += iYInc;
				}

				iError += iDy2;
				pucBuffer += iXInc;
			}
		}
		else
		{
			int iError = iDx2-iDy;
		
			for (int i = 0; i <= iDy; ++i)
			{
				*((unsigned int*)pucBuffer) = (DWORD)Color;
			
				if (iError >= 0)
				{
					iError -= iDy2;
					pucBuffer += iXInc;
				}

				iError += iDx2;
				pucBuffer += iYInc;
			}
		}
	}
	
	Unlock();
}

void EditableTexture::DrawFilledRect(
	int iLeft, int iTop,
	int iRight, int iBottom,
	D3DCOLOR Color)
{
	Lock();

	int iTmp = 0;

	if (iLeft > iRight)
	{
		iTmp = iLeft;
		iLeft = iRight;
		iRight = iTmp;
	}

	if (iTop > iBottom)
	{
		iTmp = iTop;
		iTop = iBottom;
		iBottom = iTmp;
	}

	if (iLeft < 0)
		iLeft = 0;

	if (iTop < 0)
		iTop = 0;

	if (iRight >= m_Desc.Width)
		iRight = m_Desc.Width-1;

	if (iBottom >= m_Desc.Height)
		iBottom = m_Desc.Height-1;

	unsigned char* pucBuffer = (unsigned char*)m_LockedRect.pBits;
	
	if (m_Desc.Format == D3DFMT_X8R8G8B8 ||
		m_Desc.Format == D3DFMT_A8R8G8B8)
	{
		pucBuffer += (iTop*m_LockedRect.Pitch)+(iLeft*4);

		const int iWidth = (iRight-iLeft+1)*4;
		
		for (int y = iTop; y <= iBottom; ++y)
		{
			memset(pucBuffer, (DWORD)Color,  iWidth);
			pucBuffer += m_LockedRect.Pitch;
		}
	}

	Unlock();
}

//-----------------------------------------------------------------------------

void EditableTexture::DrawCenterRect(int iX, 
									 int iY, 
									 int iSize, 
									 D3DCOLOR Color)
{
	if (iSize == 1)
	{
		DrawPixel(iX,iY,Color);
	}
	else
	{
		const int iSize2 = iSize << 1;

		DrawFilledRect(iX-iSize2,iY-iSize2,iX+iSize2,iY+iSize2,Color);
	}
}
	
//-----------------------------------------------------------------------------

void EditableTexture::DrawFilledCircle(
	int iX,
	int iY,
	int iRadius,
	D3DCOLOR Color)
{
	if (iRadius == 1)
	{
		DrawPixel(iX, iY, Color);
		return;
	}

	Lock();

	for (int x = 0, y = iRadius, dec = 3-2*iRadius; x <= y; ++x)
	{
		// Top, left to right.
		DrawLine(iX+x, iY-y, iX-x, iY-y, Color);

		// Bottom, left to right.
		DrawLine(iX+x, iY+y, iX-x, iY+y, Color);

		// Side, bottom, left to right
		DrawLine(iX+y, iY+x, iX-y, iY+x, Color);
		
		// Side, top, left to right
		DrawLine(iX+y, iY-x, iX-y, iY-x, Color);
		
		if (dec >= 0)
			dec += -4*(y--)+4;

		dec += 4*x+6;
	}

	Unlock();
}

//-----------------------------------------------------------------------------

void EditableTexture::DrawCircle(
	int iX,
	int iY,
	int iRadius,
	D3DCOLOR Color,
	int iThickness)
{
	Lock();

	for (int x = 0, y = iRadius, dec = 3-2*iRadius; x <= y; ++x)
	{
		DrawCenterRect(iX+x,iY+y,iThickness,Color);
		DrawCenterRect(iX+x,iY-y,iThickness,Color);
		DrawCenterRect(iX-x,iY+y,iThickness,Color);
		DrawCenterRect(iX-x,iY-y,iThickness,Color);
		
		DrawCenterRect(iX+y,iY+x,iThickness,Color);
		DrawCenterRect(iX+y,iY-x,iThickness,Color);
		DrawCenterRect(iX-y,iY+x,iThickness,Color);
		DrawCenterRect(iX-y,iY-x,iThickness,Color);
		
		if (dec >= 0)
			dec += -4*(y--)+4;

		dec += 4*x+6;
	}

	Unlock();
}

}
}