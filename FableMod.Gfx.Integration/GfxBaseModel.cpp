#include "Stdafx.h"
#include "GfxBaseModel.h"
#include "Helpers.h"

using namespace System::Runtime::InteropServices;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

GfxBaseModel::GfxBaseModel(AssetEntry^ entry) :
m_pRoot(0),
m_pNode(0)
{
}

GfxBaseModel::~GfxBaseModel()
{
	GFX_SAFE_DELETE(m_pRoot);
	GFX_SAFE_DELETE(m_pNode);
}

FableMod::Gfx::Node* GfxBaseModel::BuildGfx(D3DXEXTENDEDFRAME* pFrame,
											FableMod::Gfx::Node* pParent)
{
	// Build the Gfx base object.

	FableMod::Gfx::Node* pNode = new FableMod::Gfx::Node();

	D3DXMATRIX matFix;
	
	/*
	pNode->SetTranslation(D3DXVECTOR3(
		pFrame->TransformationMatrix._41,
		pFrame->TransformationMatrix._42,
		pFrame->TransformationMatrix._43));

	matFix = pFrame->TransformationMatrix;
	matFix._41 = 0.0f;
	matFix._42 = 0.0f;
	matFix._43 = 0.0f;
	//D3DXMatrixRotationZ(&matFix, D3DXToRadian(-180));
	pNode->SetRotation(matFix);
	*/

	if (pFrame->pMeshContainer)
	{
		LPD3DXMESHCONTAINER pContainer = pFrame->pMeshContainer;

		if (pContainer->MeshData.pMesh)
		{
			FableMod::Gfx::D3DXModelPtr spModel = 
				new FableMod::Gfx::D3DXModel();
		
			FableMod::Gfx::D3DXModel::SMaterial Mat;

			Mat.D3DMaterial = pContainer->pMaterials->MatD3D;
			Mat.bAlpha = pFrame->bAlpha;
			Mat.spTexture = pFrame->spTexture;

			spModel->Build(pContainer->MeshData.pMesh, &Mat, 1);

			FableMod::Gfx::MeshPtr spMesh = new FableMod::Gfx::Mesh();

			spMesh->SetModel(spModel);

			spMesh->SetTranslation(D3DXVECTOR3(
				pFrame->TransformationMatrix._41,
				pFrame->TransformationMatrix._42,
				pFrame->TransformationMatrix._43));
			
			// Make objects face +y.
			// Why are they facing the wrong direction?
			// Is the viewport code in the camera incorrect
			// or are the models incorrect?
			D3DXMatrixRotationZ(&matFix, D3DXToRadian(-180));
			spMesh->SetRotation(matFix);
			
			pNode->AttachChild(spMesh);
		}
	}

	if (pFrame->pFrameFirstChild)
	{
		BuildGfx(
			(D3DXEXTENDEDFRAME*)pFrame->pFrameFirstChild, 
			pNode);
	}

	if (pFrame->pFrameSibling)
	{
		BuildGfx(
			(D3DXEXTENDEDFRAME*)pFrame->pFrameSibling, 
			pNode);
	}

	if (pParent)
		pParent->AttachChild(pNode);

	return pNode;
}

void GfxBaseModel::ExportX(String^ fileName)
{
	ExportToX(fileName, m_pRoot);
}

void GfxBaseModel::ExportToX(String^ fileName, D3DXEXTENDEDFRAME* pRoot)
{
	System::IntPtr ptr = Marshal::StringToHGlobalUni(fileName);

	if (FAILED(D3DXSaveMeshHierarchyToFile(
		(LPCTSTR)ptr.ToPointer(), 
		D3DXF_FILEFORMAT_TEXT, 
		pRoot, 
		NULL, 
		NULL)))
	{
		throw gcnew System::Exception("Failed to export file");
	}
	
	Marshal::FreeHGlobal(ptr);
}

FableMod::Gfx::Node* GfxBaseModel::BuildModel()
{
	return GfxBaseModel::BuildGfx(m_pRoot, 0);
}

FableMod::Gfx::Node* GfxBaseModel::GetGfx()
{
	if (!m_pNode)
		m_pNode = new FableMod::Gfx::NodePtr(BuildModel());
	
	FableMod::Gfx::Node* pNode = (FableMod::Gfx::Node*)(*m_pNode)->Copy();

	pNode->Update();

	return pNode;
}

}
}
}