#include "Stdafx.h"
#include "TerrainBlock.h"
#include "TerrainPage.h"
#include "TerrainVertex.h"
#include "Mesh.h"
#include "Terrain.h"
#include "Camera.h"
#include "Consts.h"

namespace FableMod
{
namespace Gfx
{

//---------------------------------------------------------------------------

void TerrainBlock::GetVertex9(
	unsigned short usSize,
    TerrainVertex* pVOrigin, 
	TerrainVertex* apTVertex[9])
{
    unsigned short usOrigin = m_ucX+usSize*m_ucY;
    unsigned short usTwoStride = 2*m_ucStride;
    unsigned short usSizePlus1 = usSize+1;
    unsigned short usSizePlus2 = usSize+2;
    unsigned short usTwoSizePlus1 = 2*usSize+1;
    unsigned short usSizeTimesStride = usSize*m_ucStride;
    unsigned short usSizePlus1TimesStride = usSizePlus1*m_ucStride;
    unsigned short usSizePlus2TimesStride = usSizePlus2*m_ucStride;
    unsigned short usSizeTimesTwoStride = usSize*usTwoStride;
    unsigned short usTwoSizePlus1TimesStride = usTwoSizePlus1*m_ucStride;
    unsigned short usSizePlus1TimesTwoStride = usSizePlus1*usTwoStride;

    pVOrigin += usOrigin;
    apTVertex[0] = &pVOrigin[0];
    apTVertex[1] = &pVOrigin[m_ucStride];
    apTVertex[2] = &pVOrigin[usTwoStride];
    apTVertex[3] = &pVOrigin[usSizeTimesStride];
    apTVertex[4] = &pVOrigin[usSizePlus1TimesStride];
    apTVertex[5] = &pVOrigin[usSizePlus2TimesStride];
    apTVertex[6] = &pVOrigin[usSizeTimesTwoStride];
    apTVertex[7] = &pVOrigin[usTwoSizePlus1TimesStride];
    apTVertex[8] = &pVOrigin[usSizePlus1TimesTwoStride];
}

//---------------------------------------------------------------------------

void TerrainBlock::Initialize(
	TerrainPage* pPage,
    TerrainBlock* pBlock, 
	unsigned short usBlock, 
	unsigned char ucX,
    unsigned char ucY, 
	unsigned char ucStride, 
	bool bEven)
{
    m_ucX = ucX;
    m_ucY = ucY;
    m_ucStride = ucStride;
    m_ucFlags = 0;
    m_fDeltaL = 0.0;
    m_fDeltaH = GFX_INFINITY;
    SetEven(bEven);

    TerrainVertex* aTVertex[9];
	
    GetVertex9(pPage->m_usSize,pPage->m_aTVertex,aTVertex);

    // set vertex dependencies
    aTVertex[1]->SetDependent(0,aTVertex[4]);
    aTVertex[3]->SetDependent(1,aTVertex[4]);
    aTVertex[5]->SetDependent(0,aTVertex[4]);
    aTVertex[7]->SetDependent(1,aTVertex[4]);
	
    if (bEven)
    {
        aTVertex[4]->SetDependent(0,aTVertex[6]);
        aTVertex[4]->SetDependent(1,aTVertex[2]);
    }
    else
    {
        aTVertex[4]->SetDependent(0,aTVertex[0]);
        aTVertex[4]->SetDependent(1,aTVertex[8]);
    }

    // Recurse on children (if any).
    if (ucStride > 1)
    {
        // child stride is half the parent stride
        unsigned char ucChildStride = (ucStride >> 1);

        // process child 00
        unsigned short usChild = GetChildIndex(usBlock,1);
        TerrainBlock* pChild = &pBlock[usChild];
        pChild->Initialize(
			pPage,pBlock,usChild,ucX,ucY,ucChildStride,true);

        // process child 01
        usChild++;
        pChild++;
        pChild->Initialize(
			pPage,pBlock,usChild,ucX+ucStride,ucY,ucChildStride,false);

        // process child 10
        usChild++;
        pChild++;
        pChild->Initialize(
			pPage,pBlock,usChild,ucX,ucY+ucStride,ucChildStride,false);

        // process child 11
        usChild++;
        pChild++;
        pChild->Initialize(
			pPage,pBlock,usChild,ucX+ucStride,ucY+ucStride,ucChildStride,true);
    }
}

//---------------------------------------------------------------------------

/*
void TerrainBlock::UpdateBoundingBox(
	TerrainPage* pPage,
    TerrainBlock* pBlock, 
	unsigned short usBlock, 
	unsigned char ucStride)
{
    D3DXVECTOR3 avPoint[9];
    avPoint[0].x = pPage->GetX(m_ucX);
    avPoint[0].y = pPage->GetY(m_ucY);
    avPoint[1].x = pPage->GetX(m_ucX+m_ucStride);
    avPoint[1].y = avPoint[0].y;
    avPoint[2].x = pPage->GetX(m_ucX+2*m_ucStride);
    avPoint[2].y = avPoint[0].y;
    avPoint[3].x = avPoint[0].x;
    avPoint[3].y = pPage->GetY(m_ucY+m_ucStride);
    avPoint[4].x = avPoint[1].x;
    avPoint[4].y = avPoint[3].y;
    avPoint[5].x = avPoint[2].x;
    avPoint[5].y = avPoint[3].y;
    avPoint[6].x = avPoint[0].x;
    avPoint[6].y = pPage->GetY(m_ucY+2*m_ucStride);
    avPoint[7].x = avPoint[1].x;
    avPoint[7].y = avPoint[6].y;
    avPoint[8].x = avPoint[2].x;
    avPoint[8].y = avPoint[6].y;

    unsigned short usOrigin = m_ucX+pPage->m_usSize*m_ucY;
    avPoint[0].z = pPage->GetHeight(usOrigin);
    avPoint[1].z = pPage->GetHeight(usOrigin+ucStride);
    avPoint[2].z = pPage->GetHeight(usOrigin+2*ucStride);
    usOrigin += pPage->m_usSize*ucStride;
    avPoint[3].z = pPage->GetHeight(usOrigin);
    avPoint[4].z = pPage->GetHeight(usOrigin+ucStride);
    avPoint[5].z = pPage->GetHeight(usOrigin+2*ucStride);
    usOrigin += pPage->m_usSize*ucStride;
    avPoint[6].z = pPage->GetHeight(usOrigin);
    avPoint[7].z = pPage->GetHeight(usOrigin+ucStride);
    avPoint[8].z = pPage->GetHeight(usOrigin+2*ucStride);

    // compute (x,y) components of bounding box
    m_vMin.x = avPoint[0].x;
    m_vMin.y = avPoint[0].y;
    m_vMax.x = avPoint[8].x;
    m_vMax.y = avPoint[8].y;
    
    // compute delta max
    FLOAT fC0 = avPoint[0].z;
    FLOAT fMd = avPoint[1].z;
    FLOAT fC1 = avPoint[2].z;
    m_fDelta[0] = 0.5*(fC0+fC1)-fMd;
    m_fDelta[0] *= m_fDelta[0];
    
    fC0 = avPoint[8].z;
    fMd = avPoint[5].z;
    m_fDelta[1] = 0.5*(fC0+fC1)-fMd;
    m_fDelta[1] *= m_fDelta[1];
    
    fMd = avPoint[7].z;
    fC1 = avPoint[6].z;
    m_fDelta[2] = 0.5*(fC0+fC1)-fMd;
    m_fDelta[2] *= m_fDelta[2];
    
    fC0 = avPoint[0].z;
    fMd = avPoint[3].z;
    m_fDelta[3] = 0.5*(fC0+fC1)-fMd;
    m_fDelta[3] *= m_fDelta[3];
    
    fMd = avPoint[4].z;
    if (GetEven())
    {
        fC0 = avPoint[2].z;
        fC1 = avPoint[6].z;
    }
    else
    {
        fC0 = avPoint[0].z;
        fC1 = avPoint[8].z;
    }
    m_fDelta[4] = 0.5*(fC0+fC1)-fMd;
    m_fDelta[4] *= m_fDelta[4];

    m_fDeltaMax = m_fDelta[0];
    for (int i = 1; i < 5; i++)
    {
        if (m_fDelta[i] > m_fDeltaMax )
            m_fDeltaMax = m_fDelta[i];
    }

    // recurse on children (if any)
    if (ucStride > 1 )
    {
        // child stride is half the parent stride
        unsigned char ucChildStride = ( ucStride >> 1 );

        // process child 00
        unsigned short usChild = GetChildIndex(usBlock,1);
        TerrainBlock* pChild = &pBlock[usChild];
        pChild->UpdateBoundingBox(pPage,pBlock,usChild,ucChildStride);
        if (pChild->m_fDeltaMax > m_fDeltaMax )
            m_fDeltaMax = pChild->m_fDeltaMax;
        m_vMin.z = pChild->m_vMin.z;
        m_vMax.z = pChild->m_vMax.z;

        // process child 01
        usChild++;
        pChild++;
        pChild->UpdateBoundingBox(pPage,pBlock,usChild,ucChildStride);
        if (pChild->m_fDeltaMax > m_fDeltaMax )
            m_fDeltaMax = pChild->m_fDeltaMax;
        if (pChild->m_vMin.z < m_vMin.z )
            m_vMin.z = pChild->m_vMin.z;
        if (pChild->m_vMax.z > m_vMax.z )
            m_vMax.z = pChild->m_vMax.z;

        // process child 10
        usChild++;
        pChild++;
        pChild->UpdateBoundingBox(pPage,pBlock,usChild,ucChildStride);
        if (pChild->m_fDeltaMax > m_fDeltaMax )
            m_fDeltaMax = pChild->m_fDeltaMax;
        if (pChild->m_vMin.z < m_vMin.z )
            m_vMin.z = pChild->m_vMin.z;
        if (pChild->m_vMax.z > m_vMax.z )
            m_vMax.z = pChild->m_vMax.z;

        // process child 11
        usChild++;
        pChild++;
        pChild->UpdateBoundingBox(pPage,pBlock,usChild,ucChildStride);
        if (pChild->m_fDeltaMax > m_fDeltaMax )
            m_fDeltaMax = pChild->m_fDeltaMax;
        if (pChild->m_vMin.z < m_vMin.z )
            m_vMin.z = pChild->m_vMin.z;
        if (pChild->m_vMax.z > m_vMax.z )
            m_vMax.z = pChild->m_vMax.z;
    }
    else
    {
        // compute z components of bounding box at leaf node of quadtree
        m_vMin.z = avPoint[0].z;
        m_vMax.z = m_vMin.z;
        for (unsigned short usIndex = 1; usIndex < 9; usIndex++)
        {
            FLOAT fZ = avPoint[usIndex].z;
            if (fZ < m_vMin.z )
                m_vMin.z = fZ;
            if (fZ > m_vMax.z )
                m_vMax.z = fZ;
        }
    }
}

//---------------------------------------------------------------------------

bool TerrainBlock::TestIntersect(
	const MgcBox3& rkBox,
    const MgcFrustum& rkFrustum)
{
    // for convenience
    const D3DXVECTOR3* akA = rkBox.Axes();
    const FLOAT* afE = rkBox.Extents();

    D3DXVECTOR3 vDiff = rkBox.Center()-rkFrustum.Origin();

    FLOAT afA[3], afB[3], afC[3], afD[3];
    FLOAT afNA[3], afNB[3], afNC[3], afND[3];
    FLOAT afNApLC[3], afNAmLC[3], afNBpUC[3], afNBmUC[3];
    FLOAT afLC[3], afLD[3], afUC[3], afUD[3], afLBpUA[3], afLBmUA[3];
    FLOAT fDdD, fR, fP, fMin, fMax, fMTwoLF, fMTwoUF, fLB, fUA, fTmp;
    int i, j;

    // M = D
    afD[2] = vDiff.Dot(rkFrustum.DVector());
    for (i = 0; i < 3; i++)
        afC[i] = akA[i].Dot(rkFrustum.DVector());
    fR = afE[0]*abs(afC[0]) +
         afE[1]*abs(afC[1]) +
         afE[2]*abs(afC[2]);
    if (afD[2]+fR < rkFrustum.DMin() || afD[2]-fR > rkFrustum.DMax())
        return false;

    // M = n*L-l*D
    for (i = 0; i < 3; i++)
    {
        afA[i] = akA[i].Dot(rkFrustum.LVector());
        afLC[i] = rkFrustum.LBound()*afC[i];
        afNA[i] = rkFrustum.DMin()*afA[i];
        afNAmLC[i] = afNA[i]-afLC[i];
    }
    afD[0] = vDiff.Dot(rkFrustum.LVector());
    fR = afE[0]*abs(afNAmLC[0]) +
         afE[1]*abs(afNAmLC[1]) +
         afE[2]*abs(afNAmLC[2]);
    afND[0] = rkFrustum.DMin()*afD[0];
    afLD[2] = rkFrustum.LBound()*afD[2];
    fDdD = afND[0]-afLD[2];
    fMTwoLF = rkFrustum.GetMTwoLF();
    if (fDdD+fR < fMTwoLF || fDdD > fR )
        return false;

    // M = -n*L-l*D
    for (i = 0; i < 3; i++)
        afNApLC[i] = afNA[i]+afLC[i];
    fR = afE[0]*abs(afNApLC[0]) +
         afE[1]*abs(afNApLC[1]) +
         afE[2]*abs(afNApLC[2]);
    fDdD = -(afND[0]+afLD[2]);
    if (fDdD+fR < fMTwoLF || fDdD > fR )
        return false;

    // M = n*U-u*D
    for (i = 0; i < 3; i++)
    {
        afB[i] = akA[i].Dot(rkFrustum.UVector());
        afUC[i] = rkFrustum.UBound()*afC[i];
        afNB[i] = rkFrustum.DMin()*afB[i];
        afNBmUC[i] = afNB[i]-afUC[i];
    }
    afD[1] = vDiff.Dot(rkFrustum.UVector());
    fR = afE[0]*abs(afNBmUC[0]) +
         afE[1]*abs(afNBmUC[1]) +
         afE[2]*abs(afNBmUC[2]);
    afND[1] = rkFrustum.DMin()*afD[1];
    afUD[2] = rkFrustum.UBound()*afD[2];
    fDdD = afND[1]-afUD[2];
    fMTwoUF = rkFrustum.GetMTwoUF();
    if (fDdD+fR < fMTwoUF || fDdD > fR )
        return false;

    // M = -n*U-u*D
    for (i = 0; i < 3; i++)
        afNBpUC[i] = afNB[i]+afUC[i];
    fR = afE[0]*abs(afNBpUC[0]) +
         afE[1]*abs(afNBpUC[1]) +
         afE[2]*abs(afNBpUC[2]);
    fDdD = -(afND[1]+afUD[2]);
    if (fDdD+fR < fMTwoUF || fDdD > fR )
        return false;

    // M = A[i]
    for (i = 0; i < 3; i++)
    {
        fP = rkFrustum.LBound()*abs(afA[i]) +
             rkFrustum.UBound()*abs(afB[i]);
        afNC[i] = rkFrustum.DMin()*afC[i];
        fMin = afNC[i]-fP;
        if (fMin < 0.0 )
            fMin *= rkFrustum.GetDRatio();
        fMax = afNC[i]+fP;
        if (fMax > 0.0 )
            fMax *= rkFrustum.GetDRatio();
        fDdD = afA[i]*afD[0]+afB[i]*afD[1]+afC[i]*afD[2];
        if (fDdD+afE[i] < fMin || fDdD-afE[i] > fMax )
            return false;
    }

    // M = Cross(L,A[i])
    for (i = 0; i < 3; i++)
    {
        fP = rkFrustum.UBound()*abs(afC[i]);
        fMin = afNB[i]-fP;
        if (fMin < 0.0 )
            fMin *= rkFrustum.GetDRatio();
        fMax = afNB[i]+fP;
        if (fMax > 0.0 )
            fMax *= rkFrustum.GetDRatio();
        fDdD = -afC[i]*afD[1]+afB[i]*afD[2];
        fR = afE[0]*abs(afB[i]*afC[0]-afB[0]*afC[i]) +
             afE[1]*abs(afB[i]*afC[1]-afB[1]*afC[i]) +
             afE[2]*abs(afB[i]*afC[2]-afB[2]*afC[i]);
        if (fDdD+fR < fMin || fDdD-fR > fMax )
            return false;
    }

    // M = Cross(U,A[i])
    for (i = 0; i < 3; i++)
    {
        fP = rkFrustum.LBound()*abs(afC[i]);
        fMin = -afNA[i]-fP;
        if (fMin < 0.0 )
            fMin *= rkFrustum.GetDRatio();
        fMax = -afNA[i]+fP;
        if (fMax > 0.0 )
            fMax *= rkFrustum.GetDRatio();
        fDdD = afC[i]*afD[0]-afA[i]*afD[2];
        fR = afE[0]*abs(afA[i]*afC[0]-afA[0]*afC[i]) +
             afE[1]*abs(afA[i]*afC[1]-afA[1]*afC[i]) +
             afE[2]*abs(afA[i]*afC[2]-afA[2]*afC[i]);
        if (fDdD+fR < fMin || fDdD-fR > fMax )
            return false;
    }

    // M = Cross(n*D+l*L+u*U,A[i])
    for (i = 0; i < 3; i++)
    {
        fLB = rkFrustum.LBound()*afB[i];
        fUA = rkFrustum.UBound()*afA[i];
        afLBpUA[i] = fLB+fUA;
        afLBmUA[i] = fLB-fUA;
    }
    for (i = 0; i < 3; i++)
    {
        fP = rkFrustum.LBound()*abs(afNBmUC[i]) +
             rkFrustum.UBound()*abs(afNAmLC[i]);
        fTmp = rkFrustum.DMin()*afLBmUA[i];
        fMin = fTmp-fP;
        if (fMin < 0.0 )
            fMin *= rkFrustum.GetDRatio();
        fMax = fTmp+fP;
        if (fMax > 0.0 )
            fMax *= rkFrustum.GetDRatio();
        fDdD = -afD[0]*afNBmUC[i]+afD[1]*afNAmLC[i]+afD[2]*afLBmUA[i];
        fR = 0.0;
        for (j = 0; j < 3; j++)
        {
            fR += afE[j]*abs(-afA[j]*afNBmUC[i]+ afB[j]*afNAmLC[i]
               +afC[j]*afLBmUA[i]);
        }
        if (fDdD+fR < fMin || fDdD-fR > fMax )
            return false;
    }

    // M = Cross(n*D+l*L-u*U,A[i])
    for (i = 0; i < 3; i++)
    {
        fP = rkFrustum.LBound()*abs(afNBpUC[i]) +
             rkFrustum.UBound()*abs(afNAmLC[i]);
        fTmp = rkFrustum.DMin()*afLBpUA[i];
        fMin = fTmp-fP;
        if (fMin < 0.0 )
            fMin *= rkFrustum.GetDRatio();
        fMax = fTmp+fP;
        if (fMax > 0.0 )
            fMax *= rkFrustum.GetDRatio();
        fDdD = -afD[0]*afNBpUC[i]+afD[1]*afNAmLC[i]+afD[2]*afLBpUA[i];
        fR = 0.0;
        for (j = 0; j < 3; j++)
        {
            fR += afE[j]*abs(-afA[j]*afNBpUC[i]+ afB[j]*afNAmLC[i]
               +afC[j]*afLBpUA[i]);
        }
        if (fDdD+fR < fMin || fDdD-fR > fMax )
            return false;
    }

    // M = Cross(n*D-l*L+u*U,A[i])
    for (i = 0; i < 3; i++)
    {
        fP = rkFrustum.LBound()*abs(afNBmUC[i]) +
             rkFrustum.UBound()*abs(afNApLC[i]);
        fTmp = -rkFrustum.DMin()*afLBpUA[i];
        fMin = fTmp-fP;
        if (fMin < 0.0 )
            fMin *= rkFrustum.GetDRatio();
        fMax = fTmp+fP;
        if (fMax > 0.0 )
            fMax *= rkFrustum.GetDRatio();
        fDdD = -afD[0]*afNBmUC[i]+afD[1]*afNApLC[i]-afD[2]*afLBpUA[i];
        fR = 0.0;
        for (j = 0; j < 3; j++)
        {
            fR += afE[j]*abs(-afA[j]*afNBmUC[i]+ afB[j]*afNApLC[i]
               -afC[j]*afLBpUA[i]);
        }
        if (fDdD+fR < fMin || fDdD-fR > fMax )
            return false;
    }

    // M = Cross(n*D-l*L-u*U,A[i])
    for (i = 0; i < 3; i++)
    {
        fP = rkFrustum.LBound()*abs(afNBpUC[i]) +
             rkFrustum.UBound()*abs(afNApLC[i]);
        fTmp = -rkFrustum.DMin()*afLBmUA[i];
        fMin = fTmp-fP;
        if (fMin < 0.0 )
            fMin *= rkFrustum.GetDRatio();
        fMax = fTmp+fP;
        if (fMax > 0.0 )
            fMax *= rkFrustum.GetDRatio();
        fDdD = -afD[0]*afNBpUC[i]+afD[1]*afNApLC[i]-afD[2]*afLBmUA[i];
        fR = 0.0;
        for (j = 0; j < 3; j++)
        {
            fR += afE[j]*abs(-afA[j]*afNBpUC[i]+ afB[j]*afNApLC[i]
               -afC[j]*afLBmUA[i]);
        }
        if (fDdD+fR < fMin || fDdD-fR > fMax )
            return false;
    }

    return true;
}

//---------------------------------------------------------------------------

void TerrainBlock::TestIntersectFrustum(
	const TerrainPage* pPage,
    const Camera* pCamera)
{
    SetVisibilityTested(true);

    // centered-form bounding box of page in terrain model space
    MgcBox3 kBox;
    kBox.Center() = 0.5*(m_vMin+m_vMax);
    kBox.Extent(0) = 0.5*(m_vMax.x-m_vMin.x);
    kBox.Extent(1) = 0.5*(m_vMax.y-m_vMin.y);
    kBox.Extent(2) = 0.5*(m_vMax.z-m_vMin.z);
    kBox.Axis(0) = D3DXVECTOR3::UNIT_X;
    kBox.Axis(1) = D3DXVECTOR3::UNIT_Y;
    kBox.Axis(2) = D3DXVECTOR3::UNIT_Z;

    // inverse transform to camera space
    D3DXVECTOR3 vDiff = kBox.Center()-pkCamera->GetLocation();
    kBox.Center() = D3DXVECTOR3(vDiff.Dot(pkCamera->GetLeft()),
        vDiff.Dot(pkCamera->GetUp()),vDiff.Dot(pkCamera->GetDirection()));
    kBox.Axis(0) = D3DXVECTOR3(pkCamera->GetLeft().x,pkCamera->GetUp().x,
        pkCamera->GetDirection().x);
    kBox.Axis(1) = D3DXVECTOR3(pkCamera->GetLeft().y,pkCamera->GetUp().y,
        pkCamera->GetDirection().y);
    kBox.Axis(2) = D3DXVECTOR3(pkCamera->GetLeft().z,pkCamera->GetUp().z,
        pkCamera->GetDirection().z);

    // default frustum matches camera coordinate frame
    MgcFrustum kFrustum;
    kFrustum.LBound() = pkCamera->GetFrustumRight();
    kFrustum.UBound() = pkCamera->GetFrustumTop();
    kFrustum.DMin() = pkCamera->GetFrustumNear();
    kFrustum.DMax() = pkCamera->GetFrustumFar();
    kFrustum.Update();

    SetVisible(TestIntersect(kBox,kFrustum));
    //SetVisible(true);
}

*/

//---------------------------------------------------------------------------

void TerrainBlock::UpdateBound(
	TerrainPage* pPage,
    TerrainBlock* pBlock, 
	unsigned short usBlock, 
	unsigned char ucStride)
{
    D3DXVECTOR3 avPoint[9];
	
    avPoint[0].x = pPage->GetX(m_ucX);
    avPoint[0].y = pPage->GetY(m_ucY);
    avPoint[1].x = pPage->GetX(m_ucX+m_ucStride);
    avPoint[1].y = avPoint[0].y;
    avPoint[2].x = pPage->GetX(m_ucX+2*m_ucStride);
    avPoint[2].y = avPoint[0].y;
    avPoint[3].x = avPoint[0].x;
    avPoint[3].y = pPage->GetY(m_ucY+m_ucStride);
    avPoint[4].x = avPoint[1].x;
    avPoint[4].y = avPoint[3].y;
    avPoint[5].x = avPoint[2].x;
    avPoint[5].y = avPoint[3].y;
    avPoint[6].x = avPoint[0].x;
    avPoint[6].y = pPage->GetY(m_ucY+2*m_ucStride);
    avPoint[7].x = avPoint[1].x;
    avPoint[7].y = avPoint[6].y;
    avPoint[8].x = avPoint[2].x;
    avPoint[8].y = avPoint[6].y;

    unsigned short usOrigin = m_ucX+pPage->m_usSize*m_ucY;

    avPoint[0].z = pPage->GetHeight(usOrigin);
    avPoint[1].z = pPage->GetHeight(usOrigin+ucStride);
    avPoint[2].z = pPage->GetHeight(usOrigin+2*ucStride);
    usOrigin += pPage->m_usSize*ucStride;
    avPoint[3].z = pPage->GetHeight(usOrigin);
    avPoint[4].z = pPage->GetHeight(usOrigin+ucStride);
    avPoint[5].z = pPage->GetHeight(usOrigin+2*ucStride);
    usOrigin += pPage->m_usSize*ucStride;
    avPoint[6].z = pPage->GetHeight(usOrigin);
    avPoint[7].z = pPage->GetHeight(usOrigin+ucStride);
    avPoint[8].z = pPage->GetHeight(usOrigin+2*ucStride);

	m_vMin.x = avPoint[0].x;
    m_vMin.y = avPoint[0].y;
    m_vMax.x = avPoint[8].x;
    m_vMax.y = avPoint[8].y;

	// compute delta max
    FLOAT fC0 = avPoint[0].z;
    FLOAT fMd = avPoint[1].z;
    FLOAT fC1 = avPoint[2].z;
    m_fDelta[0] = 0.5*(fC0+fC1)-fMd;
    m_fDelta[0] *= m_fDelta[0];
    
    fC0 = avPoint[8].z;
    fMd = avPoint[5].z;
    m_fDelta[1] = 0.5*(fC0+fC1)-fMd;
    m_fDelta[1] *= m_fDelta[1];
    
    fMd = avPoint[7].z;
    fC1 = avPoint[6].z;
    m_fDelta[2] = 0.5*(fC0+fC1)-fMd;
    m_fDelta[2] *= m_fDelta[2];
    
    fC0 = avPoint[0].z;
    fMd = avPoint[3].z;
    m_fDelta[3] = 0.5*(fC0+fC1)-fMd;
    m_fDelta[3] *= m_fDelta[3];
    
    fMd = avPoint[4].z;
	
    if (GetEven())
    {
        fC0 = avPoint[2].z;
        fC1 = avPoint[6].z;
    }
    else
    {
        fC0 = avPoint[0].z;
        fC1 = avPoint[8].z;
    }
	
    m_fDelta[4] = 0.5*(fC0+fC1)-fMd;
    m_fDelta[4] *= m_fDelta[4];

    m_fDeltaMax = m_fDelta[0];
	
    for (int i = 1; i < 5; i++)
    {
        if (m_fDelta[i] > m_fDeltaMax)
            m_fDeltaMax = m_fDelta[i];
    }

    // Recurse on children (if any).
    if (ucStride > 1 )
    {
        // child stride is half the parent stride
        unsigned char ucChildStride = ( ucStride >> 1 );

        // process child 00
        unsigned short usChild = GetChildIndex(usBlock,1);
        TerrainBlock* pChild = &pBlock[usChild];
        pChild->UpdateBound(pPage,pBlock,usChild,ucChildStride);
        if (pChild->m_fDeltaMax > m_fDeltaMax )
            m_fDeltaMax = pChild->m_fDeltaMax;
        m_vMin.z = pChild->m_vMin.z;
        m_vMax.z = pChild->m_vMax.z;

        // process child 01
        usChild++;
        pChild++;
        pChild->UpdateBound(pPage,pBlock,usChild,ucChildStride);
        if (pChild->m_fDeltaMax > m_fDeltaMax )
            m_fDeltaMax = pChild->m_fDeltaMax;
        if (pChild->m_vMin.z < m_vMin.z )
            m_vMin.z = pChild->m_vMin.z;
        if (pChild->m_vMax.z > m_vMax.z )
            m_vMax.z = pChild->m_vMax.z;

        // process child 10
        usChild++;
        pChild++;
        pChild->UpdateBound(pPage,pBlock,usChild,ucChildStride);
        if (pChild->m_fDeltaMax > m_fDeltaMax )
            m_fDeltaMax = pChild->m_fDeltaMax;
        if (pChild->m_vMin.z < m_vMin.z )
            m_vMin.z = pChild->m_vMin.z;
        if (pChild->m_vMax.z > m_vMax.z )
            m_vMax.z = pChild->m_vMax.z;

        // process child 11
        usChild++;
        pChild++;
        pChild->UpdateBound(pPage,pBlock,usChild,ucChildStride);
        if (pChild->m_fDeltaMax > m_fDeltaMax )
            m_fDeltaMax = pChild->m_fDeltaMax;
        if (pChild->m_vMin.z < m_vMin.z )
            m_vMin.z = pChild->m_vMin.z;
        if (pChild->m_vMax.z > m_vMax.z )
            m_vMax.z = pChild->m_vMax.z;
    }
    else
    {
        // compute z components of bounding box at leaf node of quadtree
        m_vMin.z = avPoint[0].z;
        m_vMax.z = m_vMin.z;
        
		for (unsigned short usIndex = 1; usIndex < 9; ++usIndex)
        {
            const FLOAT fZ = avPoint[usIndex].z;
            
			if (fZ < m_vMin.z)
                m_vMin.z = fZ;
			
            if (fZ > m_vMax.z)
                m_vMax.z = fZ;
        }
    }
}

//---------------------------------------------------------------------------

void TerrainBlock::TestIntersectFrustum(
	const TerrainPage* pPage,
    const Camera* pCamera)
{
	SetVisibilityTested(true);
	
	// Calculate bounding sphere.
	
	BoundSphere Sphere;
	
	const D3DXVECTOR3 vMin(GetMin().x, GetMin().y, GetMin().z);
	const D3DXVECTOR3 vMax(GetMax().x, GetMax().y, GetMax().z);

	Sphere.Center() = 0.5f*(vMin+vMax);
	
	const D3DXVECTOR3 vTmp(vMax-Sphere.Center());
    
	FLOAT fRad = fabs(vTmp.x);
	
	if (fabs(vTmp.y) > fRad)
		fRad = fabs(vTmp.y);
	
	if (fabs(vTmp.z) > fRad)
		fRad = fabs(vTmp.z);
	
	Sphere.Radius() = fRad;
	
	D3DXVec3TransformCoord(
		&Sphere.Center(),
		&Sphere.Center(),
		&pPage->GetTerrain()->GetWorldTransform());

	SetVisible(pCamera->IsVisible(&Sphere));
}

//---------------------------------------------------------------------------

void TerrainBlock::ComputeInterval(
	const D3DXVECTOR3& vModelEye,
    FLOAT fTolerance)
{
    // distant terrain assumption

    if (fTolerance > 0.0 )
    {
        // compute hmin = |eye.z-clamp(bmin.z,eye.z,bmax.z)|
        FLOAT fHMin;
        if (vModelEye.z < m_vMin.z )
        {
            fHMin = vModelEye.z-m_vMin.z;
            fHMin *= fHMin;
        }
        else if (vModelEye.z > m_vMax.z )
        {
            fHMin = vModelEye.z-m_vMax.z;
            fHMin *= fHMin;
        }
        else
        {
            fHMin = 0.0;
        }

        // compute hmax = max{|eye.z-bmin.z|,|eye.z-bmax.z|}
        FLOAT fHMax = vModelEye.z-m_vMin.z;
        fHMax *= fHMax;
        FLOAT fTmp = vModelEye.z-m_vMax.z;
        fTmp *= fTmp;
		
        if (fTmp > fHMax)
            fHMax = fTmp;

        // compute rmin and rmax
        FLOAT fDxMin = vModelEye.x-m_vMin.x;
        FLOAT fDxMax = vModelEye.x-m_vMax.x;
        FLOAT fDyMin = vModelEye.y-m_vMin.y;
        FLOAT fDyMax = vModelEye.y-m_vMax.y;
        fDxMin *= fDxMin;
        fDxMax *= fDxMax;
        fDyMin *= fDyMin;
        fDyMax *= fDyMax;
        FLOAT fRMin = 0.0, fRMax = 0.0;

        if (vModelEye.x < m_vMin.x)
        {
            fRMin += fDxMin;
            fRMax += fDxMax;
        }
        else if (vModelEye.x <= m_vMax.x)
        {
            fRMax += ( fDxMax >= fDxMin ? fDxMax : fDxMin );
        }
        else
        {
            fRMin += fDxMax;
            fRMax += fDxMin;
        }

        if (vModelEye.y < m_vMin.y)
        {
            fRMin += fDyMin;
            fRMax += fDyMax;
        }
        else if (vModelEye.y <= m_vMax.y)
        {
            fRMax += (fDyMax >= fDyMin ? fDyMax : fDyMin);
        }
        else
        {
            fRMin += fDyMax;
            fRMax += fDyMin;
        }

        // compute fmin
        FLOAT fDenom = fRMin+fHMax;
        FLOAT fFMin =
            (fDenom > 0.0 ? fRMin/(fDenom*fDenom) : GFX_INFINITY);
        fDenom = fRMax+fHMax;
        fTmp = (fDenom > 0.0 ? fRMax/(fDenom*fDenom) : GFX_INFINITY);
        if (fTmp < fFMin )
            fFMin = fTmp;
        
        // compute fmax
        FLOAT fFMax;
        if (fRMin >= fHMin )
        {
            fDenom = fRMin+fHMin;
            fFMax =
                (fDenom > 0.0 ? fRMin/(fDenom*fDenom) : GFX_INFINITY);
        }
        else if (fRMax <= fHMin )
        {
            fDenom = fRMax+fHMin;
            fFMax =
                (fDenom > 0.0 ? fRMax/(fDenom*fDenom) : GFX_INFINITY);
        }
        else
        {
            fFMax = (fHMin > 0.0 ? 0.25/fHMin : GFX_INFINITY);
        }
        
        m_fDeltaL = fTolerance/fFMax;
        m_fDeltaH = (fFMin > 0.0 ? fTolerance/fFMin : GFX_INFINITY);
    }
    else  // fTolerance == 0.0
    {
        m_fDeltaL = 0.0;
        m_fDeltaH = GFX_INFINITY;
    }
}

//---------------------------------------------------------------------------

void TerrainBlock::ComputeInterval(
	const D3DXVECTOR3& vModelEye,
    const D3DXVECTOR3& vModelDir, 
	FLOAT fTolerance, 
	D3DXVECTOR2& rvLoc,
    FLOAT fSpacing)
{
    // close terrain assumption

    if (fTolerance > 0.0)
    {
        // compute fmin and fmax

        // temporary quantities
        FLOAT fLmEx = rvLoc.x-vModelEye.x;
        FLOAT fLmEy = rvLoc.y-vModelEye.y;
        FLOAT fTmp = fSpacing*FLOAT(m_ucStride);
        FLOAT fXSum = fLmEx+fTmp;
        FLOAT fYSum = fLmEy+fTmp;
        FLOAT fXDif = fLmEx-fTmp;
        FLOAT fYDif = fLmEy-fTmp;

        // find corners of block closest to and farthest from eye
        FLOAT fFMin = fabs(vModelDir.x*fXDif+vModelDir.y*fYDif);
        FLOAT fFMax = fFMin;

        fTmp = fabs(vModelDir.x*fXSum+vModelDir.y*fYDif);
        if (fTmp >= fFMin )
            fFMin = fTmp;
        else 
            fFMax = fTmp;

        fTmp = fabs(vModelDir.x*fXSum+vModelDir.y*fYSum);
        if (fTmp >= fFMin )
            fFMin = fTmp;
        else 
            fFMax = fTmp;

        fTmp = fabs(vModelDir.x*fXDif+vModelDir.y*fYSum);
        if (fTmp >= fFMin )
            fFMin = fTmp;
        else
            fFMax = fTmp;

        m_fDeltaL = fTolerance*fFMax;
        m_fDeltaH = fTolerance*fFMin;
    }
    else  // fTolerance == 0.0
    {
        m_fDeltaL = 0.0;
        m_fDeltaH = GFX_INFINITY;
    }
}

//---------------------------------------------------------------------------

void TerrainBlock::SimplifyVertices(
	TerrainPage* pPage,
    const D3DXVECTOR3& vModelEye, 
	const D3DXVECTOR3& vModelDir,
    FLOAT fTolerance, 
	bool bCloseAssumption)
{
    unsigned short usSize = pPage->m_usSize;
    unsigned short usOrigin = m_ucX+usSize*m_ucY;
	TerrainVertex* pVOrigin = pPage->m_aTVertex+usOrigin;
    TerrainVertex* pTVertex;
    unsigned short usOffset;
    D3DXVECTOR3 vDiff;
    FLOAT fRSqr, fLenSqr, fDistDir;

    // simplify at (stride,0)
    if (m_fDeltaL <= m_fDelta[0])
    {
        pTVertex = pVOrigin+m_ucStride;
		
        if (m_fDelta[0] <= m_fDeltaH)
        {
            // test vertex delta against tolerance
            if (!pTVertex->GetEnabled())
            {
                vDiff.x = vModelEye.x-pPage->GetX(m_ucX+m_ucStride);
                vDiff.y = vModelEye.y-pPage->GetY(m_ucY);
                
				if (bCloseAssumption) 
                {
                    fDistDir = vModelDir.x*vDiff.x+vModelDir.y*vDiff.y;
                    
					if (m_fDelta[0] > fTolerance*fDistDir*fDistDir)
                        pTVertex->Enable();
                }
                else // distant assumption
                {
                    vDiff.z = vModelEye.z-pPage->GetHeight(
						usOrigin+m_ucStride);
                    fRSqr = vDiff.x*vDiff.x+vDiff.y*vDiff.y;
                    fLenSqr = fRSqr+vDiff.z*vDiff.z;
                    
					if (m_fDelta[0]*fRSqr > fTolerance*fLenSqr*fLenSqr)
                        pTVertex->Enable();
                }
            }
        }
        else
        {
            if (!pTVertex->GetEnabled())
                pTVertex->Enable();
        }
    }

    // simplify at (2*stride,stride)
    if (m_fDeltaL <= m_fDelta[1])
    {
        usOffset = (usSize+2)*m_ucStride;
        pTVertex = pVOrigin+usOffset;
		
        if (m_fDelta[1] <= m_fDeltaH)
        {
            // test vertex delta against tolerance
            if (!pTVertex->GetEnabled())
            {
                vDiff.x = vModelEye.x-pPage->GetX(m_ucX+2*m_ucStride);
                vDiff.y = vModelEye.y-pPage->GetY(m_ucY+m_ucStride);
				
                if (bCloseAssumption) 
                {
                    fDistDir = vModelDir.x*vDiff.x+vModelDir.y*vDiff.y;
                    
					if (m_fDelta[1] > fTolerance*fDistDir*fDistDir)
                        pTVertex->Enable();
                }
                else // distant assumption
                {
                    vDiff.z = vModelEye.z-pPage->GetHeight(usOrigin+usOffset);
                    fRSqr = vDiff.x*vDiff.x+vDiff.y*vDiff.y;
                    fLenSqr = fRSqr+vDiff.z*vDiff.z;
                    
					if (m_fDelta[1]*fRSqr > fTolerance*fLenSqr*fLenSqr)
                        pTVertex->Enable();
                }
            }
        }
        else
        {
            if (!pTVertex->GetEnabled())
            {
                pTVertex->Enable();
            }
        }
    }

    // simplify at (stride,2*stride)
    if (m_fDeltaL <= m_fDelta[2])
    {
        usOffset = (2*usSize+1)*m_ucStride;
        pTVertex = pVOrigin+usOffset;
		
        if (m_fDelta[2] <= m_fDeltaH)
        {
            // test vertex delta against tolerance
            if (!pTVertex->GetEnabled())
            {
                vDiff.x = vModelEye.x-pPage->GetX(m_ucX+m_ucStride);
                vDiff.y = vModelEye.y-pPage->GetY(m_ucY+2*m_ucStride);
                
				if (bCloseAssumption) 
                {
                    fDistDir = vModelDir.x*vDiff.x+vModelDir.y*vDiff.y;
                    
					if (m_fDelta[2] > fTolerance*fDistDir*fDistDir)
                        pTVertex->Enable();
                }
                else // distant assumption
                {
                    vDiff.z = vModelEye.z-pPage->GetHeight(usOrigin+usOffset);
                    fRSqr = vDiff.x*vDiff.x+vDiff.y*vDiff.y;
                    fLenSqr = fRSqr+vDiff.z*vDiff.z;
					
                    if (m_fDelta[2]*fRSqr > fTolerance*fLenSqr*fLenSqr)
                        pTVertex->Enable();
                }
            }
        }
        else
        {
            if (!pTVertex->GetEnabled())
                pTVertex->Enable();
        }
    }

    // simplify at (0,stride)
    if (m_fDeltaL <= m_fDelta[3])
    {
        usOffset = usSize*m_ucStride;
        pTVertex = pVOrigin+usOffset;
        
		if (m_fDelta[3] <= m_fDeltaH )
        {
            // test vertex delta against tolerance
            if (!pTVertex->GetEnabled())
            {
                vDiff.x = vModelEye.x-pPage->GetX(m_ucX);
                vDiff.y = vModelEye.y-pPage->GetY(m_ucY+m_ucStride);
                if (bCloseAssumption ) 
                {
                    fDistDir = vModelDir.x*vDiff.x+vModelDir.y*vDiff.y;
                    if (m_fDelta[3] > fTolerance*fDistDir*fDistDir )
                        pTVertex->Enable();
                }
                else // distant terrain assumption
                {
                    vDiff.z = vModelEye.z-pPage->GetHeight(usOrigin+usOffset);
                    fRSqr = vDiff.x*vDiff.x+vDiff.y*vDiff.y;
                    fLenSqr = fRSqr+vDiff.z*vDiff.z;
                    if (m_fDelta[3]*fRSqr > fTolerance*fLenSqr*fLenSqr )
                        pTVertex->Enable();
                }
            }
        }
        else
        {
            if (!pTVertex->GetEnabled())
                pTVertex->Enable();
        }
    }

    // Simplify at (stride,stride)
    if (m_fDeltaL <= m_fDelta[4])
    {
        usOffset = (usSize+1)*m_ucStride;
        pTVertex = pVOrigin+usOffset;
		
        if (m_fDelta[4] <= m_fDeltaH)
        {
            // Test vertex delta against tolerance.
            if (!pTVertex->GetEnabled())
            {
                vDiff.x = vModelEye.x-pPage->GetX(m_ucX+m_ucStride);
                vDiff.y = vModelEye.y-pPage->GetY(m_ucY+m_ucStride);
                
				if (bCloseAssumption) 
                {
                    fDistDir = vModelDir.x*vDiff.x+vModelDir.y*vDiff.y;
                    if (m_fDelta[4] > fTolerance*fDistDir*fDistDir )
                        pTVertex->Enable();
                }
                else // distant terrain assumption
                {
                    vDiff.z = vModelEye.z-pPage->GetHeight(usOrigin+usOffset);
                    fRSqr = vDiff.x*vDiff.x+vDiff.y*vDiff.y;
                    fLenSqr = fRSqr+vDiff.z*vDiff.z;
                    
					if (m_fDelta[4]*fRSqr > fTolerance*fLenSqr*fLenSqr)
                        pTVertex->Enable();
                }
            }
        }
        else
        {
            if (!pTVertex->GetEnabled())
                pTVertex->Enable();
        }
    }

    // Enable the corner vertices.
    if (GetEven())
    {
        pTVertex = pVOrigin+2*m_ucStride;
		
        if (!pTVertex->GetEnabled())
            pTVertex->Enable();

        pTVertex = pVOrigin+2*usSize*m_ucStride;
        
		if (!pTVertex->GetEnabled())
            pTVertex->Enable();
    }
    else
    {
        pTVertex = pVOrigin;
		
        if (!pTVertex->GetEnabled())
            pTVertex->Enable();

        pTVertex = pVOrigin+2*(usSize+1)*m_ucStride;
		
        if (!pTVertex->GetEnabled())
            pTVertex->Enable();
    }
}

//---------------------------------------------------------------------------

void TerrainBlock::Disable(TerrainPage* pPage)
{
    TerrainVertex* aTVertex[9];
    
	GetVertex9(pPage->m_usSize,pPage->m_aTVertex,aTVertex);
    
	for (int i = 0; i < 9; i++)
    {
        if (aTVertex[i]->GetEnabled())
            aTVertex[i]->Disable();
    }
}

//---------------------------------------------------------------------------

}
}