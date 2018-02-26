#include "Stdafx.h"
#include "Camera.h"
#include "Manager.h"
#include "D3DModel.h"
#include "Terrain.h"
#include "TerrainBlock.h"
#include "TerrainPage.h"
#include "TerrainVertex.h"
#include "TerrainManager.h"
#include "Consts.h"
#include "LogFile.h"
#include "Helpers.h"

namespace FableMod
{
namespace Gfx
{

//----------------------------------------------------------------------------

/// pTerrain [in] Parent terrain object.
/// usSize [in] 2^p+1, p <= 7 (usSize can be 3, 5, 9, 17, 33, 65, 129)
TerrainPage::TerrainPage(
	Terrain* pTerrain,
	unsigned short usSize,
    FLOAT* afHeight, 
	unsigned int uiHeightStride,
	const D3DXVECTOR2& vOrigin,
    FLOAT fSpacing,
	const D3DXVECTOR2& vTextureOrigin0,
    const D3DXVECTOR2& vTextureSpacing0,
	const D3DXVECTOR2& vTextureOrigin1,
    const D3DXVECTOR2& vTextureSpacing1)
{
	// usSize = 2^p+1, p <= 7
    assert(
		usSize == 3  || 
		usSize == 5  || 
		usSize == 9  || 
		usSize == 17 ||  
		usSize == 33 || 
		usSize == 65 || 
		usSize == 129);

    // native data
    m_usSize = usSize;

	// Copy the subsection of the heightmap.

	m_afHeight = new FLOAT[m_usSize*m_usSize];

	FLOAT* afSrc = afHeight;
	FLOAT* afDst = m_afHeight;

	for (unsigned short y = 0; y < m_usSize; ++y)
	{
		for (unsigned short x = 0; x < m_usSize; ++x)
		{
			m_afHeight[y*m_usSize+x] = afHeight[y*uiHeightStride+x];
		}
	}

	m_pTerrain = pTerrain;
	m_vOrigin = vOrigin;
    m_fSpacing = fSpacing;
	m_uiVertexQuantity = m_usSize*m_usSize;
    m_vTextureOrigin[0] = vTextureOrigin0;
	m_vTextureSpacing[0] = vTextureSpacing0;
	m_vTextureOrigin[1] = vTextureOrigin1;
	m_vTextureSpacing[1] = vTextureSpacing1;

	m_pBound = new BoundSphere();

    InitializeDerivedData();
}

//----------------------------------------------------------------------------

TerrainPage::~TerrainPage()
{
	FM_DBG_LOG0(_T("TerrainPage::Destructor"));

	GFX_SAFE_ARRAYDELETE(m_afHeight);
	GFX_SAFE_ARRAYDELETE(m_aTVertex);
	GFX_SAFE_ARRAYDELETE(m_aBlocks);
	GFX_SAFE_ARRAYDELETE(m_ausQueue);
	GFX_SAFE_ARRAYDELETE(m_ausLookup);
	GFX_SAFE_DELETE(m_pBound);
}

//----------------------------------------------------------------------------

void TerrainPage::InitializeDerivedData()
{
    m_usSizeM1 = m_usSize-1;
    m_fPixelTolerance = 1.0f;
    m_fWorldTolerance = -1.0f;
    m_fInvSpacing = 1.0f/m_fSpacing;
      
	// for tessellation (mapping of connectivity indices)
	m_ausLookup = new unsigned short[m_uiVertexQuantity];
    
	//m_uiConnectLength = 0;

    // initialize vertex information array
    m_aTVertex = new TerrainVertex[m_uiVertexQuantity];
    memset(m_aTVertex,0,m_uiVertexQuantity*sizeof(TerrainVertex));
	
    // allocate quadtree
    m_usBlockQuantity = m_usSize*(m_usSize-2)/3;
    m_aBlocks = new TerrainBlock[m_usBlockQuantity];

    // initialize quadtree
    unsigned short usStride = (m_usSize-1)/2;
    m_aBlocks[0].Initialize(this,m_aBlocks,0,0,0,usStride,true);
    m_aBlocks[0].UpdateBound(this,m_aBlocks,0,usStride);

    // Allocate queue.
    m_usQueueQuantity = m_usBlockQuantity-m_usBlockQuantity/4;
    m_ausQueue = new unsigned short[m_usQueueQuantity];

    // Root of quadtree is initially active.
    m_ausQueue[0] = 0;
    m_usFront = 0;
    m_usRear = 1;
    m_usItemsInQueue = 1;
}

//----------------------------------------------------------------------------

void TerrainPage::SetPixelTolerance(Camera* pCamera, FLOAT fTolerance)
{
    const FLOAT fWidth = FLOAT(Manager::GetDevice()->GetWidth());
    const FLOAT fRight = pCamera->GetFrustumRight();
    const FLOAT fNear = pCamera->GetFrustumNear();

    m_fPixelTolerance = fTolerance;
    m_fWorldTolerance = 2.0f*fRight*m_fPixelTolerance/(fNear*fWidth);
    m_fWorldTolerance *= m_fWorldTolerance;
}

//////////////////////////////////////////////////////////////////////////////
/// Get height of the terrain at the given location.
//////////////////////////////////////////////////////////////////////////////
bool TerrainPage::GetHeight(const D3DXVECTOR2& vLocation,
							FLOAT* pfHeight) const
{
    FLOAT fXGrid = (vLocation.x-m_vOrigin.x)*m_fInvSpacing;
    
	if (fXGrid < 0.0 || fXGrid > FLOAT(m_usSizeM1))
    {
        // Location not in page.
        return false;
    }

    FLOAT fYGrid = (vLocation.y-m_vOrigin.y)*m_fInvSpacing;
    
	if (fYGrid < 0.0 || fYGrid > FLOAT(m_usSizeM1))
    {
        // Location not in page.
        return false;
    }

    FLOAT fCol = floor(fXGrid);
    unsigned short usCol = (unsigned short)fCol;
    FLOAT fRow = floor(fYGrid);
    unsigned short usRow = (unsigned short)fRow;

    unsigned short usIndex = usCol+m_usSize*usRow;
    FLOAT fDx = fXGrid-fCol;
    FLOAT fDy = fYGrid-fRow;
    FLOAT fH00, fH10, fH01, fH11, fHeight;

	/*
    if ((usCol & 1) == (usRow & 1))
    {
        FLOAT fDiff = fDx-fDy;
        fH00 = m_fMinElevation+m_fMultiplier*m_afHeight[usIndex];
        fH11 = m_fMinElevation+m_fMultiplier*
            m_afHeight[usIndex+1+m_usSize];
        
		if (fDiff > 0.0)
        {
            fH10 = m_fMinElevation+m_fMultiplier*m_afHeight[usIndex+1];
            fHeight = (1.0-fDiff-fDy)*fH00+fDiff*fH10+fDy*fH11;
        }
        else
        {
            fH01 = m_fMinElevation+m_fMultiplier*
                m_afHeight[usIndex+m_usSize];
            fHeight = (1.0+fDiff-fDx)*fH00-fDiff*fH01+fDx*fH11;
        }
    }
    else
    {
        FLOAT fSum = fDx+fDy;
        fH10 = m_fMinElevation+m_fMultiplier*m_afHeight[usIndex+1];
        fH01 = m_fMinElevation+m_fMultiplier*m_afHeight[usIndex+m_uiHeightStride];
        
		if (fSum <= 1.0)
        {
            fH00 = m_fMinElevation+m_fMultiplier*m_afHeight[usIndex];
            fHeight = (1.0-fSum)*fH00+fDx*fH10+fDy*fH01;
        }
        else
        {
            fH11 = m_fMinElevation+m_fMultiplier*
                m_afHeight[usIndex+1+m_uiHeightStride];
            fHeight = (fSum-1.0)*fH11+(1.0-fDy)*fH10+(1.0-fDx)*fH01;
        }
    }
	*/
	
	if ((usCol & 1) == (usRow & 1))
    {
        FLOAT fDiff = fDx-fDy;
        fH00 = m_afHeight[usIndex];
        fH11 = m_afHeight[usIndex+1+m_usSize];
        
		if (fDiff > 0.0)
        {
            fH10 = m_afHeight[usIndex+1];
            fHeight = (1.0f-fDiff-fDy)*fH00+fDiff*fH10+fDy*fH11;
        }
        else
        {
            fH01 = m_afHeight[usIndex+m_usSize];
            fHeight = (1.0f+fDiff-fDx)*fH00-fDiff*fH01+fDx*fH11;
        }
    }
    else
    {
        FLOAT fSum = fDx+fDy;
        fH10 = m_afHeight[usIndex+1];
        fH01 = m_afHeight[usIndex+m_usSize];
        
		if (fSum <= 1.0f)
        {
            fH00 = m_afHeight[usIndex];
            fHeight = (1.0f-fSum)*fH00+fDx*fH10+fDy*fH01;
        }
        else
        {
            fH11 = m_afHeight[usIndex+1+m_usSize];
            fHeight = (fSum-1.0f)*fH11+(1.0f-fDy)*fH10+(1.0f-fDx)*fH01;
        }
    }

	if (pfHeight)
		*pfHeight = fHeight;

    return true;
}

//---------------------------------------------------------------------------

D3DCOLOR TerrainPage::GetHeightColor(unsigned int uiIndex) const
{
	unsigned int c = 128+(127*(GetHeight(uiIndex)/
		TerrainManager::GetInstance()->GetMaxHeight()));

	if (c > 255)
		c = 255;

	return D3DCOLOR_ARGB(255, c, c, c);
}

//////////////////////////////////////////////////////////////////////////////
/// Get the grid location within the page.
//////////////////////////////////////////////////////////////////////////////
bool TerrainPage::GetGridLocation(
	const D3DXVECTOR2& vLocation, 
	unsigned short* pusCol,
	unsigned short* pusRow) const
{
	FLOAT fXGrid = (vLocation.x-m_vOrigin.x)*m_fInvSpacing;
    
	if (fXGrid < 0.0 || fXGrid >= FLOAT(m_usSizeM1))
    {
        // Location not in page.
        return false;
    }

    FLOAT fYGrid = (vLocation.y-m_vOrigin.y)*m_fInvSpacing;
    
	if (fYGrid < 0.0 || fYGrid >= FLOAT(m_usSizeM1))
    {
        // Location not in page.
        return false;
    }

	if (pusCol)
	{
		FLOAT fCol = floor(fXGrid);
		*pusCol = (unsigned short)fCol;
	}

	if (pusRow)
	{
		FLOAT fRow = floor(fYGrid);
	    *pusRow = (unsigned short)fRow;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////
/// Check if terrain page itself is inside frustum.
//////////////////////////////////////////////////////////////////////////////
bool TerrainPage::IntersectFrustum(const Camera* pCamera)
{
    m_aBlocks[0].TestIntersectFrustum(this,pCamera);
    const bool bIntersect = m_aBlocks[0].GetVisible();
    m_aBlocks[0].ClearBits();
    return bIntersect;
}

//---------------------------------------------------------------------------

void TerrainPage::AddToQueue(unsigned short usBlock)
{
    m_ausQueue[m_usRear] = usBlock;
    
	if (++m_usRear == m_usQueueQuantity)
        m_usRear = 0;
    
	++m_usItemsInQueue;
}

//---------------------------------------------------------------------------

unsigned short TerrainPage::RemoveFromQueue()
{
    unsigned short usBlock = m_ausQueue[m_usFront];
    
	if (++m_usFront == m_usQueueQuantity)
        m_usFront = 0;
    
	--m_usItemsInQueue;
    
	return usBlock;
}

//---------------------------------------------------------------------------

unsigned short TerrainPage::ReadFromQueue(unsigned short usIndex)
{
    usIndex += m_usFront;
	
    if (usIndex < m_usQueueQuantity)
        return m_ausQueue[usIndex];
    else
        return m_ausQueue[usIndex-m_usQueueQuantity];
}

//////////////////////////////////////////////////////////////////////////////
/// Reset block states. 
//////////////////////////////////////////////////////////////////////////////
void TerrainPage::ResetBlocks()
{
    unsigned short usQueue, usBlock;
    
	if (m_usFront < m_usRear)
    {
        m_usNumUnprocessed = m_usRear-m_usFront;
        
		for (usQueue = m_usFront; usQueue < m_usRear; ++usQueue)
        {
            usBlock = m_ausQueue[usQueue];
            
			if (m_aBlocks[usBlock].BitsSet())
            {
                m_aBlocks[usBlock].Disable(this);
                m_aBlocks[usBlock].ClearBits();
            }
        }
    }
    else
    {
        m_usNumUnprocessed = m_usQueueQuantity-m_usFront+m_usRear;
        
		for (usQueue = m_usFront; usQueue < m_usQueueQuantity; ++usQueue)
        {
            usBlock = m_ausQueue[usQueue];
			
            if (m_aBlocks[usBlock].BitsSet())
            {
                m_aBlocks[usBlock].Disable(this);
                m_aBlocks[usBlock].ClearBits();
            }
        }
		
        for (usQueue = 0; usQueue < m_usRear; ++usQueue)
        {
            usBlock = m_ausQueue[usQueue];
            
			if (m_aBlocks[usBlock].BitsSet())
            {
                m_aBlocks[usBlock].Disable(this);
                m_aBlocks[usBlock].ClearBits();
            }
        }
    }
}

//---------------------------------------------------------------------------

void TerrainPage::SimplifyBlocks(
	const Camera* pCamera,
    const D3DXVECTOR3& vModelEye, 
	const D3DXVECTOR3& vModelDir,
    bool bCloseAssumption)
{
    while (m_usNumUnprocessed > 0)
    {
        // process the block in the front of queue
        unsigned short usBlock = RemoveFromQueue();
		
        TerrainBlock* pBlock = &m_aBlocks[usBlock];

        if (!pBlock->GetProcessed())
        {
            --m_usNumUnprocessed;

            unsigned short usChild;
            TerrainBlock* pChild;
            D3DXVECTOR2 vCLoc;
            int i;

            if (pBlock->IsFirstChild(usBlock))
            {
                // First child has responsibility for replacing by parent.
                if (pBlock->IsSibling(usBlock,ReadFromQueue(2)))
                {
                    pChild = pBlock;
					
                    if (bCloseAssumption)
                    {
                        for (i = 0; i < 4; ++i, ++pChild)
                        {
                            vCLoc.x = pChild->GetX()*m_fSpacing+m_vOrigin.x;
                            vCLoc.y = pChild->GetY()*m_fSpacing+m_vOrigin.y;
							
                            pChild->ComputeInterval(vModelEye,vModelDir,
                                m_fWorldTolerance,vCLoc,m_fSpacing);
                            
							if (pChild->GetDeltaMax() > pChild->GetDeltaL())
                                break;
                        }
                    }
                    else // distant assumption
                    {
                        for (i = 0; i < 4; ++i, ++pChild)
                        {
                            pChild->ComputeInterval(vModelEye, m_fWorldTolerance);
                            
							if (pChild->GetDeltaMax() > pChild->GetDeltaL())
                                break;
                        }
                    }

                    if (i == 4)
                    {
                        // Remove child blocks (first child already removed).
						
                        for (i = 0; i < 3; ++i)
                        {
                            usChild = RemoveFromQueue();
                            
							if (!m_aBlocks[usChild].GetProcessed())
                                --m_usNumUnprocessed;
                            
							m_aBlocks[usChild].ClearBits();
                        }

                        // add parent block
                        unsigned short usParent = 
							pBlock->GetParentIndex(usBlock);
                        
						AddToQueue(usParent);
                        
						assert(!m_aBlocks[usParent].GetProcessed());
                        
						++m_usNumUnprocessed;
                        
						continue;
                    }
                }
            }

            if (!pBlock->GetVisibilityTested())
                pBlock->TestIntersectFrustum(this,pCamera);

            if (pBlock->GetStride() > 1)
            {
                // subdivide only if bounding box of block intersects frustum
                if (pBlock->GetVisible())
                {
                    usChild = pBlock->GetChildIndex(usBlock,1);
                    pChild = &m_aBlocks[usChild];
                    
					if (bCloseAssumption) 
                    {
                        for (i = 0; i < 4; ++i, ++pChild)
                        {
                            vCLoc.x = pChild->GetX()*m_fSpacing+m_vOrigin.x;
                            vCLoc.y = pChild->GetY()*m_fSpacing+m_vOrigin.y;
                            
							pChild->ComputeInterval(vModelEye,vModelDir,
                                m_fWorldTolerance,vCLoc,m_fSpacing);
							
                            if (pChild->GetDeltaMax() > pChild->GetDeltaL())
                                break;
                        }
                    }
                    else // distant assumption
                    {
                        for (i = 0; i < 4; ++i, ++pChild)
                        {
                            pChild->ComputeInterval(vModelEye,
                                m_fWorldTolerance);
                            
							if (pChild->GetDeltaMax() > pChild->GetDeltaL())
                                break;
                        }
                    }

                    // Subdivide only if children all agree it should happen.
                    if (i < 4)
                    {
                        // Add child blocks (parent already removed)
                        for (i = 0; i < 4; ++i, ++usChild)
                        {
                            // Add child block.
                            AddToQueue(usChild);
                            
							assert(!m_aBlocks[usChild].GetProcessed());
                            
							++m_usNumUnprocessed;
                        }
						
                        continue;
                    }
                }
            }

            // Tag block as processed.
            pBlock->SetProcessed(true);
        }

        // Put processed block at rear of queue.
        AddToQueue(usBlock);
    }
}

//---------------------------------------------------------------------------

void TerrainPage::SimplifyVertices(
	const D3DXVECTOR3& vModelEye,
    const D3DXVECTOR3& vModelDir, 
	bool bCloseAssumption)
{
    unsigned int usQueue, usBlock;

    if (m_usFront < m_usRear)
    {
        for (usQueue = m_usFront; usQueue < m_usRear; ++usQueue)
        {
            usBlock = m_ausQueue[usQueue];
			
            if (m_aBlocks[usBlock].GetVisible())
            {
                m_aBlocks[usBlock].SimplifyVertices(
					this,
					vModelEye,
                    vModelDir,
					m_fWorldTolerance,
					bCloseAssumption);
            }
        }
    }
    else
    {
        for (usQueue = m_usFront; usQueue < m_usQueueQuantity; ++usQueue)
        {
            usBlock = m_ausQueue[usQueue];
            
			if (m_aBlocks[usBlock].GetVisible())
            {
                m_aBlocks[usBlock].SimplifyVertices(
					this,
					vModelEye,
                    vModelDir,
					m_fWorldTolerance,
					bCloseAssumption);
            }
        }
		
        for (usQueue = 0; usQueue < m_usRear; ++usQueue)
        {
            usBlock = m_ausQueue[usQueue];
            
			if (m_aBlocks[usBlock].GetVisible())
            {
                m_aBlocks[usBlock].SimplifyVertices(
					this,
					vModelEye,
                    vModelDir,
					m_fWorldTolerance,
					bCloseAssumption);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
/// Do terrain simplification.
//////////////////////////////////////////////////////////////////////////////
void TerrainPage::Simplify(
	Camera* pCamera,
    const D3DXVECTOR3& vModelEye, 
	const D3DXVECTOR3& vModelDir, 
    bool bCloseAssumption)
{
    if (m_fWorldTolerance == -1.0f)
        SetPixelTolerance(pCamera, m_fPixelTolerance);

    SimplifyBlocks(pCamera,vModelEye,vModelDir,bCloseAssumption);
    SimplifyVertices(vModelEye,vModelDir,bCloseAssumption);
}

//////////////////////////////////////////////////////////////////////////////
/// Add block triangles into the rendering buffer.
//////////////////////////////////////////////////////////////////////////////
void TerrainPage::RenderBlock(TerrainBlock& rBlock)
{
	unsigned short usOrigin = rBlock.GetX()+m_usSize*rBlock.GetY();
    unsigned short usTwoStride = 2*rBlock.GetStride();
    unsigned short usTwoStrideTimesSize = usTwoStride*m_usSize;
    unsigned short usIndex[5] =
    {
        usOrigin,
        usOrigin+usTwoStride,
        usOrigin+rBlock.GetStride()*(m_usSize+1),
        usOrigin+usTwoStrideTimesSize,
        usOrigin+usTwoStrideTimesSize+usTwoStride
    };

	if (rBlock.GetEven())
    {
        RenderTriangle(
			usIndex[0],usIndex[3],usIndex[1]);
        RenderTriangle(
			usIndex[4],usIndex[1],usIndex[3]);
    }
    else
    {
        RenderTriangle(
			usIndex[1],usIndex[0],usIndex[4]);
        RenderTriangle(
			usIndex[3],usIndex[4],usIndex[0]);
    }

}

//////////////////////////////////////////////////////////////////////////////
/// Add a triangle to the rendering buffer.
//////////////////////////////////////////////////////////////////////////////
void TerrainPage::RenderTriangle(
	unsigned short usT, 
	unsigned short usL,
    unsigned short usR)
{
    // Determine if triangle is leaf or interior.
    bool bInterior;
	
	D3DModel* pModel = (D3DModel*)GetTerrain()->GetModel();

    if (usR > usT)
    {
        if (usL > usT)
            bInterior = (usR-usT > 1);
        else
            bInterior = (usT-usL > 1);
    }
    else
    {
        if (usL > usT)
            bInterior = (usL-usT > 1);
        else
            bInterior = (usT-usR > 1);
    }

    if (bInterior)
    {
        // Triangle is part of internal block and can be subdivided.  M is
        // the midpoint of edge <L,R>.
        unsigned short usM = ((usL+usR) >> 1);
        
		if (m_aTVertex[usM].GetEnabled())
        {
            RenderTriangle(usM,usT,usL);
            RenderTriangle(usM,usR,usT);
            return;
        }
    }

	// pack the vertex data
    unsigned char ucX, ucY;
	
    if (m_ausLookup[usT] == (unsigned short)(~0))
    {
        ucX = usT % m_usSize;
        ucY = usT/m_usSize;

		unsigned int uiIndex = pModel->AddVertex();
		
		pModel->SetVertex(uiIndex,
			D3DXVECTOR3(GetX(ucX), GetY(ucY), GetHeight(usT)));

		pModel->SetDiffuse(uiIndex, GetHeightColor(usT));
		
		pModel->SetTextureCoordinate(0, uiIndex,
			D3DXVECTOR2(GetTextureX(0, ucX), GetTextureY(0, ucY)));
		pModel->SetTextureCoordinate(1, uiIndex,
			D3DXVECTOR2(GetTextureX(1, ucX), GetTextureY(1, ucY)));

		m_ausLookup[usT] = uiIndex;
    }

    if (m_ausLookup[usR] == (unsigned short)(~0))
    {
        ucX = usR % m_usSize;
        ucY = usR/m_usSize;
        
   		unsigned int uiIndex = pModel->AddVertex();

		pModel->SetVertex(uiIndex,
			D3DXVECTOR3(GetX(ucX), GetY(ucY), GetHeight(usR)));
		
		pModel->SetDiffuse(uiIndex, GetHeightColor(usR));
		
		pModel->SetTextureCoordinate(0, uiIndex,
			D3DXVECTOR2(GetTextureX(0, ucX), GetTextureY(0, ucY)));
		pModel->SetTextureCoordinate(1, uiIndex,
			D3DXVECTOR2(GetTextureX(1, ucX), GetTextureY(1, ucY)));

		m_ausLookup[usR] = uiIndex;
    }

    if (m_ausLookup[usL] == (unsigned short)(~0))
    {
        ucX = usL % m_usSize;
        ucY = usL/m_usSize;

   		unsigned int uiIndex = pModel->AddVertex();
		pModel->SetVertex(uiIndex,
			D3DXVECTOR3(GetX(ucX), GetY(ucY), GetHeight(usL)));
		pModel->SetDiffuse(uiIndex, GetHeightColor(usL));
		
		pModel->SetTextureCoordinate(0, uiIndex,
			D3DXVECTOR2(GetTextureX(0, ucX), GetTextureY(0, ucY)));
		pModel->SetTextureCoordinate(1, uiIndex,
			D3DXVECTOR2(GetTextureX(1, ucX), GetTextureY(1, ucY)));
		
		m_ausLookup[usL] = uiIndex;
    }

	pModel->AddFace(m_ausLookup[usT], m_ausLookup[usR], m_ausLookup[usL]);
}

//////////////////////////////////////////////////////////////////////////////
/// Add the blocks into the rendering buffer.
//////////////////////////////////////////////////////////////////////////////
void TerrainPage::RenderBlocks()
{
    // Reset dynamic quantities.
    memset(m_ausLookup,0xFF,m_usSize*m_usSize*sizeof(unsigned short));
    
    unsigned short usQueue;
	
    if (m_usFront < m_usRear)
    {
        for (usQueue = m_usFront; usQueue < m_usRear; ++usQueue)
        {
            TerrainBlock& rBlock = m_aBlocks[m_ausQueue[usQueue]];
            
			if (rBlock.GetVisible())
                RenderBlock(rBlock);
        }
    }
    else
    {
        for (usQueue = m_usFront; usQueue < m_usQueueQuantity; ++usQueue)
        {
            TerrainBlock& rBlock = m_aBlocks[m_ausQueue[usQueue]];
            
			if (rBlock.GetVisible())
                RenderBlock(rBlock);
        }
		
        for (usQueue = 0; usQueue < m_usRear; usQueue++)
        {
            TerrainBlock& rBlock = m_aBlocks[m_ausQueue[usQueue]];
            
			if (rBlock.GetVisible())
                RenderBlock(rBlock);
        }
    }
}

//---------------------------------------------------------------------------

void TerrainPage::StitchLR(TerrainPage* pRight)
{
    // 'this' is left page, 'pRight' is right page
    assert(pRight->m_usSize == m_usSize);

    unsigned short usR = m_usSize;
	
    for (unsigned short usY = 1; usY < m_usSizeM1; ++usY, usR += m_usSize)
    {
        unsigned short usL = m_usSizeM1+usR;
        TerrainVertex* pLVertex = &m_aTVertex[usL];
        TerrainVertex* pRVertex = &pRight->m_aTVertex[usR];
        pRVertex->SetDependent(0,pLVertex);
        pLVertex->SetDependent(1,pRVertex);
    }
}

//---------------------------------------------------------------------------

void TerrainPage::StitchTB(TerrainPage* pBottom)
{
    // 'this' is top page, 'pBottom' is bottom page
    assert(pBottom->m_usSize == m_usSize);

    unsigned short usOffset = m_usSize*m_usSizeM1;
    
	for (unsigned short usB = 1; usB < m_usSizeM1; ++usB)
    {
        unsigned short usT = usB+usOffset;
        TerrainVertex* pTVertex = &m_aTVertex[usT];
        TerrainVertex* pBVertex = &pBottom->m_aTVertex[usB];
        pBVertex->SetDependent(1,pTVertex);
        pTVertex->SetDependent(0,pBVertex);
    }
}

//---------------------------------------------------------------------------

void TerrainPage::UnstitchLR(TerrainPage* pRight)
{
    // 'this' is left page, 'pRight' is right page
    assert(pRight->m_usSize == m_usSize);

    unsigned short usR = m_usSize;  // y = 1
    
	for (unsigned short usY = 1; usY < m_usSizeM1; ++usY, usR += m_usSize)
    {
        unsigned short usL = m_usSizeM1+usR;
        TerrainVertex* pLVertex = &m_aTVertex[usL];
        TerrainVertex* pRVertex = &pRight->m_aTVertex[usR];
        pRVertex->SetDependent(0,0);
        pLVertex->SetDependent(1,0);
    }
}

//---------------------------------------------------------------------------

void TerrainPage::UnstitchTB(TerrainPage* pBottom)
{
    // 'this' is top page, 'pBottom' is bottom page
    assert(pBottom->m_usSize == m_usSize);

    unsigned short usOffset = m_usSize*m_usSizeM1;
	
    for (unsigned short usB = 1; usB < m_usSizeM1; ++usB)
    {
        unsigned short usT = usB+usOffset;
        TerrainVertex* pTVertex = &m_aTVertex[usT];
        TerrainVertex* pBVertex = &pBottom->m_aTVertex[usB];
        pBVertex->SetDependent(1,0);
        pTVertex->SetDependent(0,0);
    }
}

//---------------------------------------------------------------------------

}
}