#include "Stdafx.h"
#include "../FableMod.LEV/LEVFile.h"
#include "EditableTexture.h"
#include "D3DModel.h"
#include "Mesh.h"
#include "Camera.h"
#include "TerrainPage.h"
#include "Map.h"
#include "TextureManager.h"
#include "Manager.h"
#include "Settings.h"
#include "Consts.h"
#include <cassert>
#include <tchar.h>

#define GFX_TERRAIN_COMPLEX_PAGES

namespace FableMod
{
namespace Gfx
{

#ifdef GFX_TERRAIN_COMPLEX_PAGES

static const unsigned int NUM_PAGE_SIZES = 4;
static const unsigned int PAGE_SIZES[NUM_PAGE_SIZES] = 
{ 128, 64, 32, 16 };
static const unsigned int PAGE_COUNTS[NUM_PAGE_SIZES] = 
{ 
	128/PAGE_SIZES[NUM_PAGE_SIZES-1], 
	64/PAGE_SIZES[NUM_PAGE_SIZES-1], 
	32/PAGE_SIZES[NUM_PAGE_SIZES-1], 
	16/PAGE_SIZES[NUM_PAGE_SIZES-1], 
};

struct SPage
{
	D3DXVECTOR2 vOrigin;
	D3DXVECTOR2 vTextureOrigin;
	FLOAT* afField;
	bool bUsed;
};

#endif

Map::Map() :
m_Pages(2, 4),
m_pTexture(0),
m_bSetPixelTolerance(true)
{
	RenderFlags() &= ~MRF_DISTANCE_TO_BOX;
	//RenderFlags() |= MRF_WIREFRAME;
	RenderFlags() |= MRF_WIREFRAME_OVERLAY;
}

void Map::Dispose()
{
	if (m_pTexture)
	{
		delete m_pTexture;
		m_pTexture = 0;
	}

	for (unsigned int i = 0; i < m_Pages.GetCount(); ++i)
		delete m_Pages[i];
	
	m_Pages.Clear();

	Mesh::Dispose();
}

void Map::Create(FableMod::LEV::LEVFile* pLEVFile)
{
	const float fSpacing = Settings::GetInstance()->GetFloat(
		_T("Terrain"),_T("Spacing"),1.0f);

	m_uiFieldWidth = pLEVFile->GetWidth();
	m_uiFieldHeight = pLEVFile->GetHeight();

	// Create heightfield.

	FLOAT* afField = new FLOAT[(m_uiFieldWidth+1)*(m_uiFieldHeight+1)];

	FLOAT fMaxHeight = 0.0f;

	for (unsigned int y = 0; y <= m_uiFieldHeight; ++y)
	{
		for (unsigned int x = 0; x <= m_uiFieldWidth; ++x)
		{
			afField[y*(m_uiFieldWidth+1)+x] = 
				fSpacing*pLEVFile->GetHeightAt(x,y);

			if (fMaxHeight < afField[y*(m_uiFieldWidth+1)+x])
				fMaxHeight = afField[y*(m_uiFieldWidth+1)+x];
		}
	}
	
	// Create texture.

	const unsigned int uiTexSize = GetTextureSize();

	m_pTexture = new EditableTexture();
	
	m_pTexture->Create(uiTexSize, uiTexSize, false);
	m_pTexture->Clear(D3DXCOLOR(D3DCOLOR_ARGB(255,255,255,255)));
	
	// Create mesh.

	D3DModel* pModel = new D3DModel();

	pModel->Create(
		D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE, 
		(m_uiFieldWidth+1)*(m_uiFieldHeight+1),
		2*(m_uiFieldWidth)*(m_uiFieldHeight));

	SetModel(pModel);

	FLOAT fRatioW = (FLOAT)(m_uiFieldWidth)/(FLOAT)GetTextureSize();
	FLOAT fRatioH = (FLOAT)(m_uiFieldHeight)/(FLOAT)GetTextureSize();

	const D3DXVECTOR2 vTextureSpacing(
		(1/(FLOAT)m_uiFieldWidth)*fRatioW,
		(1/(FLOAT)m_uiFieldHeight)*fRatioH);

	// Size in world coords.
	const FLOAT fWidth = m_uiFieldWidth*fSpacing;
	const FLOAT fHeight = m_uiFieldHeight*fSpacing;

	// Create bound.
	GetModelBound()->Center() = D3DXVECTOR3(fWidth/2,fHeight/2,0);
	GetModelBound()->Radius() = (fWidth > fHeight) ? fWidth : fHeight;

#ifdef GFX_TERRAIN_COMPLEX_PAGES

	const unsigned int uiPageSize = PAGE_SIZES[NUM_PAGE_SIZES-1];

	const unsigned int uiNumPagesX = m_uiFieldWidth/uiPageSize;
	const unsigned int uiNumPagesY = m_uiFieldHeight/uiPageSize;

	SPage* aPages = new SPage[uiNumPagesX*uiNumPagesY];

	const FLOAT fPageWorldSize = uiPageSize*fSpacing;

	FLOAT fY = 0;
	unsigned int iY = 0;

	for (unsigned int i = 0; i < uiNumPagesY; ++i)
	{
		FLOAT fX = 0;
		unsigned int iX = 0;
		
		for (unsigned int j = 0; j < uiNumPagesX; ++j)
		{
			const unsigned int uiOffset = i*uiNumPagesX+j;
			
			aPages[uiOffset].afField = afField+iY*(m_uiFieldWidth+1)+iX;
			aPages[uiOffset].vOrigin = D3DXVECTOR2(fX,fY);
			aPages[uiOffset].vTextureOrigin =
				D3DXVECTOR2(vTextureSpacing.x*iX,vTextureSpacing.y*iY);
			aPages[uiOffset].bUsed = false;
						
			fX += fPageWorldSize;
			iX += uiPageSize;
		}
	
		fY += fPageWorldSize;
		iY += uiPageSize;
	}

	for (unsigned int y = 0; y < uiNumPagesY; ++y)
	{
		for (unsigned int x = 0; x < uiNumPagesX; ++x)
		{
			const unsigned int uiOffset = y*uiNumPagesX+x;
			
			if (aPages[uiOffset].bUsed)
			{
				// This page has been used.
				continue;
			}

			bool bOk = true;

			for (unsigned int i = 0; i < NUM_PAGE_SIZES; ++i)
			{
				bOk = true;

				const unsigned int uiCount = PAGE_COUNTS[i];
				const unsigned int uiSize = PAGE_SIZES[i];

				for (unsigned int y2 = y; y2 < y+uiCount; ++y2)
				{
					for (unsigned int x2 = x; x2 < x+uiCount; ++x2)
					{
						if (x2 >= uiNumPagesX || y2 >= uiNumPagesY)
						{
							bOk = false;
							break;
						}

						if (aPages[y2*uiNumPagesX+x2].bUsed)
						{
							bOk = false;
							break;
						}
					}
				}

				if (bOk)
				{	
					// Disable pages.

					for (unsigned int y2 = y; y2 < y+uiCount; ++y2)
					{
						for (unsigned int x2 = x; x2 < x+uiCount; ++x2)
						{
							aPages[y2*uiNumPagesX+x2].bUsed = true;
						}
					}

					// Create page.

					TerrainPage* pPage = new TerrainPage(
						this,
						uiSize+1,
						aPages[uiOffset].afField,
						m_uiFieldWidth+1,
						fMaxHeight,
						aPages[uiOffset].vOrigin,
						fSpacing,
						aPages[uiOffset].vTextureOrigin,
						vTextureSpacing);

					m_Pages.Add(pPage);

					// We're done for this offset.
					break;
				}
			}

			// one page has been created.
			assert(bOk);
		}
	}

	for (unsigned int i = 0; i < m_Pages.GetCount(); ++i)
	{
		TerrainPage* pPage1 = m_Pages[i];

		FLOAT fRight = pPage1->GetOrigin().x+
			((pPage1->GetSize()-1)*pPage1->GetSpacing());
		FLOAT fLeft = pPage1->GetOrigin().x;
		FLOAT fTop = pPage1->GetOrigin().y;
		FLOAT fBottom = pPage1->GetOrigin().y+
			((pPage1->GetSize()-1)*pPage1->GetSpacing());
		
		for (unsigned int j = 0; j < m_Pages.GetCount(); ++j)
		{
			if (i == j)
				continue;

			TerrainPage* pPage2 = m_Pages[j];

			if (pPage1->GetSize() == pPage2->GetSize())
			{
				if (abs(fTop-pPage2->GetOrigin().y) < 0.01)
				{
					if (abs(fRight-pPage2->GetOrigin().x) < 0.01)
					{
						pPage1->StitchLR(pPage2);
					}
				}
				else if (abs(fLeft-pPage1->GetOrigin().x) < 0.01)
				{
					if (abs(fBottom-pPage2->GetOrigin().y) < 0.01)
					{
						pPage1->StitchTB(pPage2);
					}
				}
			}
		}
	}

	delete[] aPages;

#else

	// Determine the biggest suitable page size.

	unsigned int uiPageSize = 128;
	
	while (true)
	{
		if (uiPageSize <= m_uiFieldWidth &&
			uiPageSize <= m_uiFieldHeight &&
			(m_uiFieldWidth % uiPageSize) == 0 &&
			(m_uiFieldHeight % uiPageSize) == 0)
		{
			// Done. Found a suitable page size.
			break;
		}

		uiPageSize >>= 1;

		if (uiPageSize == 1)
		{
			throw FableMod::Core::Exception(
				_T("FABLEMOD_GFX_INVALID_HEIGHTMAP"));
		}
	}
	
	m_uiPagesX = m_uiFieldWidth/uiPageSize;
	m_uiPagesY = m_uiFieldHeight/uiPageSize;

	// Page size in world coords.
	const FLOAT fPageWorldSize = uiPageSize*fSpacing;

	FLOAT fY = 0;

	unsigned int iY = 0;

	for (unsigned int i = 0; i < m_uiPagesY; ++i)
	{
		FLOAT fX = 0;
		
		unsigned int iX = 0;
		
		for (unsigned int j = 0; j < m_uiPagesX; ++j)
		{
			TerrainPage* pPage = new TerrainPage(
				this,
				uiPageSize+1,
				m_afField+iY*(m_uiFieldWidth+1)+iX,
				m_uiFieldWidth+1,
				fMaxHeight,
				D3DXVECTOR2(fX,fY),
				fSpacing,
				D3DXVECTOR2(vTextureSpacing.x*iX,vTextureSpacing.y*iY),
				vTextureSpacing,
				pModel);

			m_Pages.Add(pPage);
		
			fX += fPageWorldSize;
			iX += uiPageSize;
		}
	
		fY += fPageWorldSize;
		iY += uiPageSize;
	}
	
	for (unsigned int i = 0; i < m_uiPagesY; ++i)
	{
		for (unsigned int j = 0; j < m_uiPagesX; ++j)
		{
			if (j < m_uiPagesX-1)
			{
				m_Pages[j+m_uiPagesX*i]->StitchLR(
					m_Pages[(j+1)+m_uiPagesX*i]);
			}

			if (i < m_uiPagesY-1)
			{
				m_Pages[j+m_uiPagesX*i]->StitchTB(
					m_Pages[j+m_uiPagesX*(i+1)]);
			}
		}
	}
	
#endif

	delete[] afField;

	ApplyWalkable(pLEVFile);
}

//----------------------------------------------------------------------------

void Map::UpdateWorldBound()
{
	Mesh::UpdateWorldBound();
}

//----------------------------------------------------------------------------

void Map::OnRender(Camera* pCamera)
{
	IDirect3DDevice9* pDevice = Manager::GetDevice();

	pDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_POINT);
	pDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_POINT);

	D3DMATERIAL9 Material;

	ZeroMemory(&Material, sizeof(Material));

	Material.Diffuse.r = 1.0f;
	Material.Diffuse.g = 1.0f;
	Material.Diffuse.b = 1.0f;
	Material.Diffuse.a = 1.0f;
	Material.Ambient = Material.Diffuse;

	pDevice->SetMaterial(&Material);

	// Setup texture.
	pDevice->SetTexture(0, m_pTexture->GetD3DTexture());

	Mesh::OnRender(pCamera);
	
	pDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
}

//----------------------------------------------------------------------------

void Map::GetRenderList(Camera* pCamera)
{
	// Simplify the pages and create the triangle list.

	if (m_bSetPixelTolerance)
	{
		m_bSetPixelTolerance = false;
		SetPixelTolerance(pCamera,
			GetSettings()->GetFloat(_T("Terrain"),_T("PixelTolerance"), 2.0f));
	}

	D3DXMATRIX matInverse;
	D3DXMatrixInverse(&matInverse,0,&GetWorldTransform());

	D3DXVECTOR3 vCamEye(pCamera->GetPosition());
	D3DXVECTOR3 vCamDir(pCamera->GetDirection());

	D3DXVec3TransformCoord(&vCamEye,&vCamEye,&matInverse);
	D3DXVec3TransformNormal(&vCamDir,&vCamDir,&matInverse);
	
	D3DModel* pModel = (D3DModel*)GetModel();

	pModel->Reset();
	pModel->LockVertices();
	pModel->LockFaces();

	for (unsigned int i = 0; i < m_Pages.GetCount(); ++i)
	{
		TerrainPage* pPage = m_Pages[i];
		
		pPage->ResetBlocks();

		pPage->Simplify(pCamera, vCamEye, vCamDir, false);

		pPage->RenderBlocks();
	}

	pModel->UnlockVertices();
	pModel->UnlockFaces();

	Mesh::GetRenderList(pCamera);
}

//----------------------------------------------------------------------------

void Map::SetPixelTolerance(Camera* pCamera, FLOAT fTolerance)
{
	for (unsigned int i = 0; i < m_Pages.GetCount(); ++i)
	{
		m_Pages[i]->SetPixelTolerance(pCamera, fTolerance);
	}
}

//----------------------------------------------------------------------------

FLOAT Map::GetPixelTolerance() const
{
	return m_Pages[0]->GetPixelTolerance();
}

/////////////////////////////////////////////////////////////////////////////
/// Return the coordinate within the terrain if possible.
/////////////////////////////////////////////////////////////////////////////
bool Map::GetLocation(
	const D3DXVECTOR3& vPoint,
	unsigned int& ruiX,
	unsigned int& ruiY)
{
	D3DXVECTOR3 vInv;

	D3DXMATRIX matInverse;
	D3DXMatrixInverse(&matInverse,0,&GetWorldTransform());

	D3DXVec3TransformCoord(&vInv, &vPoint, &matInverse);

	unsigned short usCol,usRow;

	for (unsigned int i = 0; i < m_Pages.GetCount(); ++i)
	{
		TerrainPage* pPage = m_Pages[i];
	
		if (pPage->GetGridLocation(
			D3DXVECTOR2(vInv.x,vInv.y),&usCol,&usRow))
		{
			ruiX = usCol+pPage->GetOrigin().x/pPage->GetSpacing();
			ruiY = usRow+pPage->GetOrigin().y/pPage->GetSpacing();
			return true;
		}
	}

	return false;
}

//----------------------------------------------------------------------------

FLOAT Map::GetHeight(const D3DXVECTOR2& vLocation) const
{
	D3DXVECTOR2 vLocInv;

	D3DXMATRIX matInverse;
	D3DXMatrixInverse(&matInverse,0,&GetWorldTransform());

	D3DXVec2TransformCoord(&vLocInv, &vLocation, &matInverse);

	for (unsigned int i = 0; i < m_Pages.GetCount(); ++i)
	{
		const FLOAT fHeight = m_Pages[i]->GetHeight(vLocInv);

		if (abs(fHeight-GFX_INFINITY) > 0.001)
			return fHeight;
	}

	return GFX_INFINITY;
}

//----------------------------------------------------------------------------

void Map::Stitch(Terrain* pTerrain)
{
	// TODO
}

//----------------------------------------------------------------------------

void Map::SaveHeightfield(LPCTSTR pszFileName)
{
	EditableTexture* pTexture = new EditableTexture();

	pTexture->Create(GetTextureSize(), GetTextureSize(), false);
	pTexture->Clear(D3DXCOLOR(D3DCOLOR_ARGB(255,0,0,0)));
	
	pTexture->Lock();
	
	for (unsigned int i = 0; i < m_Pages.GetCount(); ++i)
	{
		TerrainPage* pPage = m_Pages[i];

		for (unsigned short y = 0; y < pPage->GetSize(); ++y)
		{
			for (unsigned short x = 0; x < pPage->GetSize(); ++x)
			{
				pTexture->DrawPixel(
					pPage->GetOrigin().x+x,
					pPage->GetOrigin().y+y,
					pPage->GetHeightColor(y*pPage->GetSize()+x));
			}
		}
	}

	pTexture->Unlock();

	D3DXSaveTextureToFile(
		pszFileName,
		D3DXIFF_BMP,
		pTexture->GetD3DTexture(),
		NULL);

	delete pTexture;
}

//----------------------------------------------------------------------------

void Map::ApplyWalkable(FableMod::LEV::LEVFile* pLEVFile)
{
	GetTexture()->Lock();

	const D3DXCOLOR WALKCOLOR(D3DCOLOR_ARGB(255,0,0,0));

	for (unsigned int i = 0; i <= pLEVFile->GetHeight(); ++i)
	{
		for (unsigned int j = 0; j <= pLEVFile->GetWidth(); ++j)
		{
			if (pLEVFile->GetWalkableAt(j,i))
				GetTexture()->DrawPixel(j,i,WALKCOLOR);
		}
	}

	GetTexture()->Unlock();
}

//----------------------------------------------------------------------------

unsigned int Map::GetTextureSize() const
{
	unsigned int uiTexSize = 64;
	
	while (uiTexSize < (m_uiFieldWidth+1) || uiTexSize < (m_uiFieldHeight+1))
		uiTexSize <<= 1;
	
	return uiTexSize;
}

}
}