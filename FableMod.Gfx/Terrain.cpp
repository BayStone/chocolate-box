#include "Stdafx.h"
#include "EditableTexture.h"
#include "D3DModel.h"
#include "Mesh.h"
#include "Camera.h"
#include "TerrainPage.h"
#include "Terrain.h"
#include "TerrainManager.h"
#include "Manager.h"
#include "Settings.h"
#include "Consts.h"
#include "Helpers.h"
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
	D3DXVECTOR2 vTextureOrigin0;
	D3DXVECTOR2 vTextureOrigin1;
	FLOAT* afField;
	bool bUsed;
};

#endif

unsigned char Terrain::sm_ucUpdateFrequency = 0;
unsigned char Terrain::sm_ucTextureMultiplier = 0;

Terrain::Terrain() :
m_Pages(2, 4),
m_pGridVB(0),
m_bSetPixelTolerance(true),
m_bBuildModelBound(true),
m_bUpdateTerrain(true),
m_bUpdateBlocked(false)
{
	if (sm_ucUpdateFrequency == 0)
	{
		sm_ucUpdateFrequency = (unsigned char)
			GetSettings()->GetInt(_T("Terrain"),_T("UpdateFrequency"),1);
	}
	
	m_ucCurrentUpdate = sm_ucUpdateFrequency;

	RenderFlags() &= ~MRF_DISTANCE_TO_BOX;

	if (GetSettings()->GetInt(_T("Terrain"),_T("WireFrameOverlay"),1) != 0)
		RenderFlags() |= MRF_WIREFRAME_OVERLAY;
	
	ZeroMemory(&m_Material, sizeof(m_Material));

	m_Material.Diffuse.r = 1.0f;
	m_Material.Diffuse.g = 1.0f;
	m_Material.Diffuse.b = 1.0f;
	m_Material.Diffuse.a = 1.0f;
	m_Material.Ambient = m_Material.Diffuse;
	
	D3DXMatrixIdentity(&m_matWorldInverse);

	SetName(_T("TERRAIN"));
}

Terrain::~Terrain()
{
	FM_DBG_LOG1(_T("Terrain(%s)::Destructor\n"), GetName());
	
	Dispose();
}

void Terrain::Dispose()
{
	TerrainManager::GetInstance()->Remove(this);

	m_spTexture[0] = 0;
	m_spTexture[1] = 0;

	for (unsigned int i = 0; i < m_Pages.GetCount(); ++i)
		delete m_Pages[i];

	m_Pages.Clear();
	
	GFX_SAFE_RELEASE(m_pGridVB);
	
	Mesh::Dispose();
}

void Terrain::Create(const FLOAT* pfHeightfield,
					 unsigned int uiWidth,
					 unsigned int uiHeight)
{
	m_fSpacing = Settings::GetInstance()->GetFloat(
		_T("Terrain"),_T("Spacing"),1.0f);

	m_uiFieldWidth = uiWidth;
	m_uiFieldHeight = uiHeight;

	// Create texture.

	const unsigned int uiTexSize0 = GetTextureSize(GetTextureMultiplier());
	const unsigned int uiTexSize1 = GetTextureSize();

	m_spTexture[0] = new EditableTexture();
	m_spTexture[0]->Create(uiTexSize0, uiTexSize0, 1, true);

	// Create mesh.

	D3DModel* pModel = new D3DModel();

	pModel->Create(
		D3DFVF_XYZ | D3DFVF_TEX2 | D3DFVF_DIFFUSE, 
		(m_uiFieldWidth+1)*(m_uiFieldHeight+1),
		2*(m_uiFieldWidth)*(m_uiFieldHeight));

	SetModel(pModel);

	const FLOAT fFieldWidth = m_uiFieldWidth;
	const FLOAT fFieldHeight = m_uiFieldHeight;

	const FLOAT fTexSize0 = 1.0f/(FLOAT)uiTexSize0;
	const FLOAT fTexSize1 = 1.0f/(FLOAT)uiTexSize1;

	const FLOAT fRatioW = fFieldWidth*fTexSize0;
	const FLOAT fRatioH = fFieldHeight*fTexSize0;

	const D3DXVECTOR2 vTextureSpacing0(
		(1/(fFieldWidth/GetTextureMultiplier()))*fRatioW,
		(1/(fFieldHeight/GetTextureMultiplier()))*fRatioH);
	
	const D3DXVECTOR2 vTextureSpacing1(
		(1/fFieldWidth)*(fFieldWidth*fTexSize1),
		(1/fFieldHeight)*(fFieldHeight*fTexSize1));
	
	// Size in world coords.
	const FLOAT fWidth = fFieldWidth*m_fSpacing;
	const FLOAT fHeight = fFieldHeight*m_fSpacing;

#ifdef GFX_TERRAIN_COMPLEX_PAGES

	// What the hell did this do?!

	const unsigned int uiPageSize = PAGE_SIZES[NUM_PAGE_SIZES-1];

	const unsigned int uiNumPagesX = m_uiFieldWidth/uiPageSize;
	const unsigned int uiNumPagesY = m_uiFieldHeight/uiPageSize;

	// Create temporary pages.

	SPage* aPages = new SPage[uiNumPagesX*uiNumPagesY];

	const FLOAT fPageWorldSize = uiPageSize*m_fSpacing;

	FLOAT fY = 0;
	unsigned int iY = 0;

	for (unsigned int i = 0; i < uiNumPagesY; ++i)
	{
		FLOAT fX = 0;
		unsigned int iX = 0;
		
		for (unsigned int j = 0; j < uiNumPagesX; ++j)
		{
			const unsigned int uiOffset = i*uiNumPagesX+j;
			
			aPages[uiOffset].afField = 
				((FLOAT*)pfHeightfield)+iY*(m_uiFieldWidth+1)+iX;
			aPages[uiOffset].vOrigin = D3DXVECTOR2(fX,fY);
			aPages[uiOffset].vTextureOrigin0 =
				D3DXVECTOR2(vTextureSpacing0.x*iX,vTextureSpacing0.y*iY);
			aPages[uiOffset].vTextureOrigin1 =
				D3DXVECTOR2(vTextureSpacing1.x*iX,vTextureSpacing1.y*iY);
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
						aPages[uiOffset].vOrigin,
						m_fSpacing,
						aPages[uiOffset].vTextureOrigin0,
						vTextureSpacing0,
						aPages[uiOffset].vTextureOrigin1,
						vTextureSpacing1);

					m_Pages.Add(pPage);

					// We're done for this offset.
					break;
				}
			}

			// One page has been created.
			assert(bOk);
		}
	}

	delete[] aPages;

#else

	// Determine the biggest suitable square page size.

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
	const FLOAT fPageWorldSize = uiPageSize*m_fSpacing;

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
				((FLOAT*)pfHeightfield)+iY*(m_uiFieldWidth+1)+iX,
				m_uiFieldWidth+1,
				D3DXVECTOR2(fX,fY),
				m_fSpacing,
				D3DXVECTOR2(vTextureSpacing.x*iX,vTextureSpacing.y*iY),
				vTextureSpacing);

			m_Pages.Add(pPage);
		
			fX += fPageWorldSize;
			iX += uiPageSize;
		}
	
		fY += fPageWorldSize;
		iY += uiPageSize;
	}
#endif

	Stitch(this);

	TerrainManager::GetInstance()->Add(this);
}

//----------------------------------------------------------------------------

void Terrain::UpdateWorldBound()
{
	GetModel()->ComputeBoundSphere(m_pModelBound);
	Mesh::UpdateWorldBound();
}

//----------------------------------------------------------------------------

struct GridVertex
{
	D3DXVECTOR3 vPos;
	D3DCOLOR Diffuse;
};

void Terrain::OnRender(Camera* pCamera)
{
	static const DWORD GRID_FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	static const DWORD GRID_VERTEXSIZE = D3DXGetFVFVertexSize(GRID_FVF);
	
	IDirect3DDevice9* pDevice = Manager::GetDevice()->GetD3DDevice();

	pDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	
	pDevice->SetMaterial(&m_Material);

	if (m_spTexture[1])
	{
		// Setup texture.
		GFX_TRYD3D(pDevice->SetTexture(0, m_spTexture[0]->GetD3DTexture()));
	
		Mesh::OnRender(pCamera);

		// Setup texture.
		GFX_TRYD3D(pDevice->SetTexture(0, m_spTexture[1]->GetD3DTexture()));
	
		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);

		// Set texture stages.
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
		pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);

		pDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_POINT);
		pDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
	
		Mesh::OnRender(pCamera);

		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
		pDevice->SetTextureStageState(0,
			D3DTSS_COLORARG1,D3DTA_TEXTURE);			
		pDevice->SetTextureStageState(0,
			D3DTSS_COLORARG2,D3DTA_CURRENT);			
		pDevice->SetTextureStageState(0,
			D3DTSS_COLOROP,D3DTOP_MODULATE);			
		
		pDevice->SetTextureStageState(0,
			D3DTSS_ALPHAARG1,D3DTA_TEXTURE);			
		pDevice->SetTextureStageState(0,
			D3DTSS_ALPHAARG2,D3DTA_CURRENT);			
		pDevice->SetTextureStageState(0,
			D3DTSS_ALPHAOP,D3DTOP_MODULATE);			
	}
	else
	{
		// Setup texture.
		GFX_TRYD3D(pDevice->SetTexture(0, m_spTexture[0]->GetD3DTexture()));
	
		Mesh::OnRender(pCamera);
	}

	pDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
}

//----------------------------------------------------------------------------

void Terrain::GetRenderList(Camera* pCamera)
{
	// Simplify the pages and create the triangle list.

	if (m_bSetPixelTolerance)
	{
		m_bSetPixelTolerance = false;
		SetPixelTolerance(pCamera,
			GetSettings()->GetFloat(_T("Terrain"),_T("PixelTolerance"), 2.0f));
	}

	if (!m_bUpdateBlocked && 
		++m_ucCurrentUpdate >= sm_ucUpdateFrequency)
	{
		// Reset the update variables.
		m_ucCurrentUpdate = 0;

		D3DXVECTOR3 vCamEye(pCamera->GetPosition());
		D3DXVECTOR3 vCamDir(pCamera->GetDirection());

		D3DXVec3TransformCoord(&vCamEye,&vCamEye,&m_matWorldInverse);
		D3DXVec3TransformNormal(&vCamDir,&vCamDir,&m_matWorldInverse);
		
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

		if (m_bBuildModelBound)
		{
			m_bBuildModelBound = false;
			UpdateWorldBound();
		}
	}
	
	Mesh::GetRenderList(pCamera);
}

//////////////////////////////////////////////////////////////////////////////
/// Prepare for an extensive picking process.
/// Block terrain updates and speed up the pick.
//////////////////////////////////////////////////////////////////////////////
void Terrain::BeginPick()
{
	m_bUpdateBlocked = true;
	GetModel()->CreateFaceNormals();
	GetModel()->CreateBoundSpheres();
}

//----------------------------------------------------------------------------

void Terrain::EndPick()
{
	m_bUpdateBlocked = false;
	GetModel()->DisposeFaceNormals();
	GetModel()->DisposeBoundSpheres();
}

//----------------------------------------------------------------------------

void Terrain::SetPixelTolerance(Camera* pCamera, FLOAT fTolerance)
{
	for (unsigned int i = 0; i < m_Pages.GetCount(); ++i)
	{
		m_Pages[i]->SetPixelTolerance(pCamera, fTolerance);
	}
}

//----------------------------------------------------------------------------

FLOAT Terrain::GetPixelTolerance() const
{
	return m_Pages[0]->GetPixelTolerance();
}

bool Terrain::GetLocation(
	const D3DXVECTOR3& vOrigin,
	const D3DXVECTOR3& vDirection,
	unsigned int& ruiX,
	unsigned int& ruiY)
{
	FableMod::Gfx::PickData Data;

	Pick(vOrigin, vDirection, Data);

	if (Data.pMesh)
	{
		if (GetLocation(Data.vPoint, ruiX, ruiY))
			return true;
	}

	return false;
}

bool Terrain::GetLocation(
	const D3DXVECTOR3& vOrigin,
	const D3DXVECTOR3& vDirection,
	unsigned int uiWidth,
	unsigned int uiHeight,
	unsigned int& ruiX,
	unsigned int& ruiY)
{
	FableMod::Gfx::PickData Data;

	Pick(vOrigin, vDirection, Data);

	if (Data.pMesh)
	{
		if (GetLocation(uiWidth, uiHeight, Data.vPoint, ruiX, ruiY))
			return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////
/// Return the coordinate within the terrain if possible.
/////////////////////////////////////////////////////////////////////////////
bool Terrain::GetLocation(
	const D3DXVECTOR3& vPoint,
	unsigned int& ruiX,
	unsigned int& ruiY)
{
	return GetLocation(
		m_uiFieldWidth,
		m_uiFieldHeight,
		vPoint,
		ruiX,
		ruiY);
}

bool Terrain::GetLocation(
	unsigned int uiWidth,
	unsigned int uiHeight,
	const D3DXVECTOR3& vPoint, 
	unsigned int& ruiX,
	unsigned int& ruiY) const
{
	D3DXVECTOR3 vInv;

	D3DXVec3TransformCoord(&vInv, &vPoint, &m_matWorldInverse);

	FLOAT fSpacingW = ((FLOAT)m_uiFieldWidth/(FLOAT)uiWidth);
	FLOAT fSpacingH = ((FLOAT)m_uiFieldHeight/(FLOAT)uiHeight);
	
	FLOAT fInvSpacingW = 1.0f/fSpacingW;
	FLOAT fInvSpacingH = 1.0f/fSpacingH;

	FLOAT fXGrid = vInv.x*fInvSpacingW;
    
	if (fXGrid < 0.0 || fXGrid >= (FLOAT)uiWidth)
        return false;
   
    FLOAT fYGrid = vInv.y*fInvSpacingH;
    
	if (fYGrid < 0.0 || fYGrid >= (FLOAT)uiHeight)
        return false;
   
	ruiX = floor(fXGrid);
	ruiY = floor(fYGrid);
	
	return true;
}


bool Terrain::IsOver(const D3DXVECTOR3& vPoint)
{
	D3DXVECTOR3 vInv;

	D3DXVec3TransformCoord(&vInv, &vPoint, &m_matWorldInverse);

	unsigned short usCol,usRow;

	for (unsigned int i = 0; i < m_Pages.GetCount(); ++i)
	{
		TerrainPage* pPage = m_Pages[i];
	
		if (pPage->GetGridLocation(
			D3DXVECTOR2(vInv.x,vInv.y),&usCol,&usRow))
		{
			return true;
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////
/// Get the height of any given point in the terrain.
/// @param vLocation [in] Given in world coordinates.
//////////////////////////////////////////////////////////////////////////////
bool Terrain::GetHeight(const D3DXVECTOR2& vLocation,
						FLOAT* pfHeight) const
{
	D3DXVECTOR2 vLocInv;

	D3DXVec2TransformCoord(&vLocInv, &vLocation, &m_matWorldInverse);

	for (unsigned int i = 0; i < m_Pages.GetCount(); ++i)
	{
		if (m_Pages[i]->GetHeight(vLocInv, pfHeight))
			return true;
	}

	return false;
}

//----------------------------------------------------------------------------

void Terrain::UpdateWorldTransform()
{
	Mesh::UpdateWorldTransform();
	D3DXMatrixInverse(&m_matWorldInverse,0,&GetWorldTransform());
}

//////////////////////////////////////////////////////////////////////////////
/// Return the world coordinates of the terrain page corners.
//////////////////////////////////////////////////////////////////////////////
void Terrain::GetPageCorners(
	Terrain* pTerrain, 
	TerrainPage* pPage,
	FLOAT& rfLeft,
	FLOAT& rfTop,
	FLOAT& rfRight,
	FLOAT& rfBottom)
{
	D3DXVECTOR2 vOrigin(pPage->GetOrigin());
	
	vOrigin += D3DXVECTOR2(
		pTerrain->GetWorldTranslation().x,
		pTerrain->GetWorldTranslation().y);

	rfRight = vOrigin.x+((pPage->GetSize()-1)*pPage->GetSpacing());
	rfLeft = vOrigin.x;
	rfTop = vOrigin.y;
	rfBottom = vOrigin.y+((pPage->GetSize()-1)*pPage->GetSpacing());
}

//----------------------------------------------------------------------------

void Terrain::Stitch(Terrain* pTerrain)
{
	FLOAT fL1,fT1,fR1,fB1;
	FLOAT fL2,fT2,fR2,fB2;

	FM_LOG1(_T("Stitching %u pages.\n"), m_Pages.GetCount());

	for (unsigned int i = 0; i < m_Pages.GetCount(); ++i)
	{
		TerrainPage* pPage1 = m_Pages[i];

		GetPageCorners(this, pPage1, fL1, fT1, fR1, fB1);

		FM_LOG2(_T("Page[%u]: Size = %u\n"), i, pPage1->GetSize());
						
		for (unsigned int j = 0; j < pTerrain->m_Pages.GetCount(); ++j)
		{
			if (pTerrain == this && i == j)
				continue;

			TerrainPage* pPage2 = pTerrain->m_Pages[j];

			GetPageCorners(pTerrain, pPage2, fL2, fT2, fR2, fB2);
			
			if (pPage1->GetSize() == pPage2->GetSize())
			{
				// We can only stitch pages with the same size.
				// This could be fixed though.

				if (fabs(fT1-fT2) < 0.01)
				{
					if (fabs(fR1-fL2) < 0.01)
					{
						FM_LOG1(_T(" StitchLR to %u\n"), j);
						pPage1->StitchLR(pPage2);
					}
				}
				else if (fabs(fL1-fL2) < 0.01)
				{
					if (fabs(fB1-fT2) < 0.01)
					{
						FM_LOG1(_T(" StitchTB %u\n"), j);
						pPage1->StitchTB(pPage2);
					}
				}
			}
		}
	}
}

//----------------------------------------------------------------------------

void Terrain::SetTextureSpacing(unsigned char ucSet, 
								const D3DXVECTOR2& vSize,
								EditableTexture* pTexture)
{
	const FLOAT fFieldWidth = m_uiFieldWidth;
	const FLOAT fFieldHeight = m_uiFieldHeight;

	const FLOAT fRatioW = vSize.x/fFieldWidth;
	const FLOAT fRatioH = vSize.y/fFieldHeight;

	const D3DXVECTOR2 vSpacing(
		fRatioW/pTexture->GetWidth(), fRatioH/pTexture->GetHeight());
	
	/*
	const D3DXVECTOR2 vSpacing(
		(1/fFieldWidth)*(fFieldWidth*fTexSizeW),
		(1/fFieldHeight)*(fFieldHeight*fTexSizeH));
	
	const D3DXVECTOR2 vSpacing(
		(1/vSize.x)*(fFieldWidth/vSize.x),
		(1/vSize.y)*(fFieldHeight/vSize.y));
	*/
	
	for (unsigned int i = 0; i < m_Pages.GetCount(); ++i)
	{
		D3DXVECTOR2 vOrigin(m_Pages[i]->GetOrigin());

		vOrigin /= m_Pages[i]->GetSpacing();
		
		vOrigin.x *= vSpacing.x;
		vOrigin.y *= vSpacing.y;

		m_Pages[i]->SetTextureOrigin(ucSet, vOrigin);
			
		m_Pages[i]->SetTextureSpacing(ucSet, vSpacing); 
	}
}

//----------------------------------------------------------------------------

void Terrain::SetTexture(unsigned int uiIndex, EditableTexture* pTexture)
{
	m_spTexture[uiIndex] = pTexture; 
}

//----------------------------------------------------------------------------

void Terrain::SaveHeightfield(LPCTSTR pszFileName)
{
	EditableTexturePtr spTexture = new EditableTexture();

	spTexture->Create(GetTextureSize(), GetTextureSize(), false);
	spTexture->Clear(D3DXCOLOR(D3DCOLOR_ARGB(255,0,0,0)));
	
	spTexture->Lock();
	
	for (unsigned int i = 0; i < m_Pages.GetCount(); ++i)
	{
		TerrainPage* pPage = m_Pages[i];

		for (unsigned short y = 0; y < pPage->GetSize(); ++y)
		{
			for (unsigned short x = 0; x < pPage->GetSize(); ++x)
			{
				spTexture->DrawPixel(
					pPage->GetOrigin().x+x,
					pPage->GetOrigin().y+y,
					pPage->GetHeightColor(y*pPage->GetSize()+x));
			}
		}
	}

	spTexture->Unlock();

	D3DXSaveTextureToFile(
		pszFileName,
		D3DXIFF_BMP,
		spTexture->GetD3DTexture(),
		NULL);

	spTexture = 0;
}

//----------------------------------------------------------------------------

D3DXVECTOR3 Terrain::GetWorldCenter() const
{
	return GetWorldTranslation()+
		D3DXVECTOR3(GetWorldSizeX()/2, GetWorldSizeY()/2, 0);
}

//----------------------------------------------------------------------------

static unsigned int ToPower2(unsigned int uiValue)
{
	unsigned int uiSize = 16;
	
	while (uiSize < uiValue)
		uiSize <<= 1;

	return uiSize;
}

//----------------------------------------------------------------------------

unsigned int Terrain::GetTextureSize(unsigned char ucMultiplier) const
{
	unsigned int uiTexSize = m_uiFieldWidth;

	if (m_uiFieldHeight > uiTexSize)
		uiTexSize = m_uiFieldHeight;

	uiTexSize = ToPower2(uiTexSize*ucMultiplier);

	if (uiTexSize > Manager::GetCaps()->MaxTextureWidth)
		uiTexSize = Manager::GetCaps()->MaxTextureWidth;

	if (uiTexSize > Manager::GetCaps()->MaxTextureHeight)
		uiTexSize = Manager::GetCaps()->MaxTextureHeight;
	
	return uiTexSize;
}

unsigned char Terrain::GetTextureMultiplier()
{
	if (sm_ucTextureMultiplier == 0)
	{
		sm_ucTextureMultiplier = (unsigned char)
			GetSettings()->GetInt(_T("Terrain"),_T("TextureMultiplier"),1);
	}

	return sm_ucTextureMultiplier;
}

//----------------------------------------------------------------------------

}
}