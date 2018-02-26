#include "Stdafx.h"
#include "GfxModelLOD.h"
#include "GfxManager.h"
#include "GfxTexture.h"
#include "TextureManager.h"
#include "GfxBaseModel.h"
#include "Helpers.h"
#include "AllocateHierarchy.h"

//#define MESH_DEBUG

using namespace System::Runtime::InteropServices;
using namespace System::IO;
using namespace FableMod::Data;
using namespace FableMod::CLRCore;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

GfxModelLOD::GfxModelLOD(char* data, unsigned int length) :
m_pNode(0),
m_SceneRoot(0)
{
	m_CompiledModel = new CompiledModel(data, &length);
}

GfxModelLOD::~GfxModelLOD()
{
	this->!GfxModelLOD();
}

GfxModelLOD::!GfxModelLOD()
{
	DestroyCompiledModel();
	DestroyFrames();

	GFX_SAFE_DELETE(m_pNode);
}

void GfxModelLOD::DestroyFrames()
{
	if (m_SceneRoot)
	{
		m_SceneRoot->Destroy();
		delete m_SceneRoot;
		m_SceneRoot = 0;
	}
}

void GfxModelLOD::DestroyCompiledModel()
{
	if (m_CompiledModel)
	{
		delete m_CompiledModel;
		m_CompiledModel = 0;
	}
}

void GfxModelLOD::Save(char* data, unsigned int* length)
{
	m_CompiledModel->SaveToBuffer(data, (int*)length);
}

FableMod::Gfx::Node* GfxModelLOD::GetGfx()
{
	if (!m_pNode)
	{
		// Create base copy.

		m_pNode = new FableMod::Gfx::NodePtr();

		FableMod::Gfx::Node* pNode = new FableMod::Gfx::Node();

		BuildGfx(pNode);

		DestroyFrames();

		*m_pNode = pNode;
	}

	return (Node*)(*m_pNode)->Copy();
}

FableMod::Gfx::Texture* GfxModelLOD::FindTexture(UInt32 id)
{
	GfxTexture^ gfxTexture = GfxManager::GetTextureManager()->Get(id);

	if (gfxTexture != nullptr)
	{
		return gfxTexture->GetBaseTexture();
	}

	return 0;
}

void GfxModelLOD::CreateFrameFromCompiled()
{
	D3DXMATRIX identity;

	D3DXMatrixIdentity(&identity);
	
	m_SceneRoot = new D3DXEXTENDEDFRAME;
	m_SceneRoot->Name = CopyXFileSafeString("Scene Root");
	m_SceneRoot->pFrameSibling = NULL;
	m_SceneRoot->pMeshContainer = NULL;
	m_SceneRoot->TransformationMatrix = identity;

	D3DXEXTENDEDFRAME* pTopLOD = new D3DXEXTENDEDFRAME;
	
	pTopLOD->Name = CopyXFileSafeString(m_CompiledModel->GetName());
	pTopLOD->pFrameSibling = NULL;
	pTopLOD->pMeshContainer = NULL;
	pTopLOD->TransformationMatrix = identity;

	D3DXFrameAppendChild(m_SceneRoot, pTopLOD);

	LPDIRECT3DDEVICE9 pD3DDevice =
		FableMod::Gfx::Manager::GetDevice()->GetD3DDevice();

	for (int i = 0; i < m_CompiledModel->NumberSubMeshes; ++i)
	{
		SUBM* subm = m_CompiledModel->SubMeshes[i];
		
		D3DXEXTENDEDFRAME* submesh = new D3DXEXTENDEDFRAME;
		
		submesh->Name = CopyXFileSafeString(
			m_CompiledModel->GetMaterialFromID(subm->MaterialID)->Name);

	#ifdef MESH_DEBUG
		Console::WriteLine("SubMesh {0}", i);
		Console::WriteLine(" Name: \"{0}\"", gcnew String(submesh->Name));
		Console::WriteLine(" Material: \"{0}\"", 
			gcnew String(m_CompiledModel->GetMaterialFromID(
				subm->MaterialID)->Name));
	#endif

		submesh->TransformationMatrix = identity;
		
		submesh->pFrameSibling = NULL;
		submesh->pFrameFirstChild = NULL;

		// create our mesh object
		ID3DXMesh* pD3DXMesh = 0;

		if (FAILED(D3DXCreateMeshFVF(
			subm->Number_Faces, 
			subm->Number_Verts, 
			D3DXMESH_MANAGED, 
			D3DFVF_XYZ  | 
			D3DFVF_NORMAL | 
			D3DFVF_TEX1 | 
			D3DFVF_TEXCOORDSIZE2(0), 
			pD3DDevice, 
			&pD3DXMesh)))
		{
			throw gcnew System::Exception("D3DXCreateMeshFVF failed");
		}
		
		//fill our vert buffer
		Vertex* verts;
		pD3DXMesh->LockVertexBuffer(0,(void**)&verts);
		subm->GetVertices(verts);
		pD3DXMesh->UnlockVertexBuffer();

		// get bones for now, since we need to do retarded lookup when we do faces...
		
		VertexBones* bones = NULL;

		if (m_CompiledModel->SkeletonPresent)
		{
			bones = new VertexBones[subm->Number_Verts];
			subm->GetVertexBones(bones);
		}

		//fill our index buffer
		WORD* faces;
		pD3DXMesh->LockIndexBuffer(0,(void**)&faces);
		subm->GetFaces(faces, bones);
		pD3DXMesh->UnlockIndexBuffer();

		// Setup material. One material for one submesh.
		
		D3DXMATERIAL* mat = new D3DXMATERIAL;

		mat->MatD3D.Ambient.a = 0.3f;
		mat->MatD3D.Ambient.r = 0.3f;
		mat->MatD3D.Ambient.g = 0.3f;
		mat->MatD3D.Ambient.b = 0.3f;
		mat->MatD3D.Diffuse.a = 1.0f;
		mat->MatD3D.Diffuse.r = 1.0f;
		mat->MatD3D.Diffuse.g = 1.0f;
		mat->MatD3D.Diffuse.b = 1.0f;
		mat->MatD3D.Emissive.a = 0.0f;
		mat->MatD3D.Emissive.r = 0.0f;
		mat->MatD3D.Emissive.g = 0.0f;
		mat->MatD3D.Emissive.b = 0.0f;
		mat->MatD3D.Specular.a = 0.0f;
		mat->MatD3D.Specular.r = 0.0f;
		mat->MatD3D.Specular.g = 0.0f;
		mat->MatD3D.Specular.b = 0.0f;
		mat->MatD3D.Power = 0.0f;

		//can we find the texture?
		String^ s = "";
		s = String::Concat(m_CompiledModel->GetMaterialFromID(
			subm->MaterialID)->BASE_Texture_ID.ToString(), ".dds");
		IntPtr ps = Marshal::StringToHGlobalAnsi(s);
		mat->pTextureFilename = new char[strlen((char*)ps.ToPointer())+1];
		strcpy(mat->pTextureFilename, (char*)ps.ToPointer());
		Marshal::FreeHGlobal(ps);

		// Get textures.

		FableMod::Gfx::Texture* pTex = 0;
		
		MTRL* pMtrl = m_CompiledModel->GetMaterialFromID(subm->MaterialID);

		if ((pTex = FindTexture(pMtrl->BASE_Texture_ID)) != 0)
		{
			submesh->spTexture = pTex;
		}

		if ((pTex = FindTexture(pMtrl->BUMPMAP_Texture_ID)) != 0)
		{
			submesh->spBumpMap = pTex;
		}

		if ((pTex = FindTexture(pMtrl->REFLECT_Texture_ID)) != 0)
		{
			submesh->spReflectMap = pTex;
		}

		submesh->bAlpha = pMtrl->Alpha_Enabled > 0;

		ID3DXSkinInfo*	pSkinInfo = NULL;

		if (m_CompiledModel->SkeletonPresent)
		{
			// figure out which bones this subm has
			int bonecount=0;
			unsigned char* bonelist = 
				new unsigned char[m_CompiledModel->NumberBones];
			
			for(int j=0;j<subm->Number_Verts;j++)
			{
				for(int m=0;m<4;m++)
				{
					bool found = false;
					for(int k=0;k<bonecount;k++)
					{
						if (bonelist[k] == bones[j].bones[m])
							found = true;
					}
					if (!found)
					{
						bonelist[bonecount] = bones[j].bones[m];
						bonecount++;
					}
				}
			}

			D3DXCreateSkinInfoFVF(
				subm->Number_Verts, 
				D3DFVF_XYZ  | 
				D3DFVF_NORMAL | 
				D3DFVF_TEX1 | 
				D3DFVF_TEXCOORDSIZE2(0), 
				bonecount, 
				&pSkinInfo);
			
			for(int j=0;j<bonecount;j++)
			{
				D3DXMATRIX matrix= identity;// = m_CompiledModel->BoneSubChunks3[bonelist[j]].a;
				m_CompiledModel->BoneSubChunks3[bonelist[j]].GetMatrix(&matrix);
				char* name = CopyXFileSafeString(m_CompiledModel->BoneIndex[bonelist[j]]);
				pSkinInfo->SetBoneName(j, name);
				delete[] name;
				pSkinInfo->SetBoneOffsetMatrix(j, &matrix);
			}
			for(int k=0;k<bonecount;k++)
			{
				int influencecount = 0;
				for(int j=0;j<subm->Number_Verts;j++)
				{
					if (bones[j].bones[0] == bonelist[k] && bones[j].influences[0]  
					|| bones[j].bones[1] == bonelist[k] && bones[j].influences[1]
					|| bones[j].bones[2] == bonelist[k] && bones[j].influences[2]
					|| bones[j].bones[3] == bonelist[k] && bones[j].influences[3])
						influencecount++;
				}
				
				DWORD* vertexindices = new DWORD[influencecount];
				float* weights = new float[influencecount];
				int influencepos = 0;
				for(int j=0;j<subm->Number_Verts;j++)
				{
					if (bones[j].bones[0] == bonelist[k] && bones[j].influences[0])
					{
						vertexindices[influencepos] = j;
						weights[influencepos] = ((float)bones[j].influences[0])/255.0f;
						influencepos++;
					}
					else if (bones[j].bones[1] == bonelist[k] && bones[j].influences[1])
					{
						vertexindices[influencepos] = j;
						weights[influencepos] = ((float)bones[j].influences[1])/255.0f;
						influencepos++;
					}
					else if (bones[j].bones[2] == bonelist[k] && bones[j].influences[2])
					{
						vertexindices[influencepos] = j;
						weights[influencepos] = ((float)bones[j].influences[2])/255.0f;
						influencepos++;
					}
					else if (bones[j].bones[3] == bonelist[k] && bones[j].influences[3])
					{
						vertexindices[influencepos] = j;
						weights[influencepos] = ((float)bones[j].influences[3])/255.0f;
						influencepos++;
					}
				}
				pSkinInfo->SetBoneInfluence(k, influencecount, vertexindices, weights);
				delete[] vertexindices;
				delete[] weights;
			}

			delete[] bonelist;
			delete[] bones;
		}

		submesh->pMeshContainer = new D3DXMESHCONTAINER;
		submesh->pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
		submesh->pMeshContainer->MeshData.pMesh = pD3DXMesh;
		submesh->pMeshContainer->Name = NULL;
		submesh->pMeshContainer->pMaterials = mat;
		submesh->pMeshContainer->NumMaterials = 1;
		submesh->pMeshContainer->pAdjacency = NULL;
		submesh->pMeshContainer->pEffects = NULL;
		submesh->pMeshContainer->pNextMeshContainer = NULL;
		submesh->pMeshContainer->pSkinInfo = pSkinInfo;

		D3DXFrameAppendChild(pTopLOD, submesh);
	}

	D3DXFrameAppendChild(m_SceneRoot, m_CompiledModel->GetHelpers(
		0, &m_SceneRoot->TransformationMatrix));
	
	if (m_CompiledModel->SkeletonPresent && 
		m_CompiledModel->NumberBones > 1)
	{
		D3DXFrameAppendChild(m_SceneRoot, 
			m_CompiledModel->CreateBoneHeirarchy(
				1, &m_SceneRoot->TransformationMatrix));
	}

	D3DXEXTENDEDFRAME* pOrphans = new D3DXEXTENDEDFRAME;
	
	pOrphans->Name = CopyXFileSafeString("Orphan_Helpers");
	pOrphans->TransformationMatrix = identity;
	
	D3DXFrameAppendChild(pOrphans, 
		m_CompiledModel->GetHelpers(-1, &identity));

	D3DXFrameAppendChild(m_SceneRoot, pOrphans);
}

void GfxModelLOD::ExportTextures(String^ path)
{
	for (int i = 0; i < m_CompiledModel->NumberSubMeshes; ++i)
	{
		SUBM* subm = m_CompiledModel->SubMeshes[i];
			
		MTRL* material = 
			m_CompiledModel->GetMaterialFromID(subm->MaterialID);

		GfxTexture^ gfxTexture = GfxManager::GetTextureManager()->Get(
			material->BASE_Texture_ID);

		if (gfxTexture != nullptr)
		{
			String^ s = String::Concat(
				path, material->BASE_Texture_ID.ToString(), 
				".dds");
				
			gfxTexture->Save(s, 0);
		
			gfxTexture = nullptr;
		}
	}
}

void GfxModelLOD::ExportX(String^ fileName)
{
	if (!m_SceneRoot)
		CreateFrameFromCompiled();

	GfxBaseModel::ExportToX(fileName, m_SceneRoot);

	ExportTextures(
		fileName->Substring(
			0,fileName->Length-Path::GetFileName(fileName)->Length));
}

void GfxModelLOD::BuildGfx(FableMod::Gfx::Node* pRoot)
{
	if (!m_SceneRoot)
		CreateFrameFromCompiled();

	GfxBaseModel::BuildGfx(m_SceneRoot, pRoot);
}

D3DXMATRIX* GfxModelLOD::GetBoneTransform(D3DXEXTENDEDFRAME* frame, const char* name)
{
	D3DXMATRIX* ret = NULL;
	if (frame->Name)
		if (strcmp(frame->Name,name) == 0)
			return new D3DXMATRIX(frame->TransformationMatrix);

	if (frame->pFrameFirstChild)
		ret = GetBoneTransform((D3DXEXTENDEDFRAME*)frame->pFrameFirstChild, name);
	
	if (ret)
	{
		*ret = (*ret) * frame->TransformationMatrix;
		return ret;
	}
	if (frame->pFrameSibling)
		ret = GetBoneTransform((D3DXEXTENDEDFRAME*)frame->pFrameSibling, name);

	if (ret)
	{
		//*ret = frame->TransformationMatrix * (*ret);
		return ret;
	}

	return NULL;

}

void GfxModelLOD::ImportX(String^ fileName)
{
	D3DXFRAME* frame;

	ID3DXAnimationController* ancont;
	
	AllocateHierarchy* parc = new AllocateHierarchy();

	LPDIRECT3DDEVICE9 pD3DDevice =
		FableMod::Gfx::Manager::GetDevice()->GetD3DDevice();

	IntPtr ptr = Marshal::StringToHGlobalUni(fileName);
	
	try
	{
		D3DXLoadMeshHierarchyFromX(
			(LPCTSTR)ptr.ToPointer(),
			D3DXMESH_MANAGED, 
			pD3DDevice,
			parc,
			NULL,
			&frame,
			&ancont);
	}
	catch (System::Exception^ e)
	{
		System::Windows::Forms::MessageBox::Show(
			e->Message, 
			"Exception during X-File parsing.", 
			System::Windows::Forms::MessageBoxButtons::OK, 
			System::Windows::Forms::MessageBoxIcon::Error);
		
		Marshal::FreeHGlobal(ptr);
		
		delete parc;
		
		return;
	}

	Marshal::FreeHGlobal(ptr);

	m_CompiledModel->DestroySubMeshes();
	m_CompiledModel->DestroyMaterials();
	m_CompiledModel->DestroyBones();
	m_CompiledModel->DestroyHelpers();
	
	D3DXMATRIX identity;
	D3DXMatrixIdentity(&identity);

	D3DXFRAME* movementdummy = D3DXFrameFind(frame, "Movement_dummy");

	if (!movementdummy)
		movementdummy = D3DXFrameFind(frame, "Movement");
	if (!movementdummy)
		movementdummy = D3DXFrameFind(frame, "Bow_bone_main");
	if (!movementdummy)
		movementdummy = D3DXFrameFind(frame, "root_bone");
	if (!movementdummy)
		movementdummy = D3DXFrameFind(frame, "Main_Root_Bone");

	if (movementdummy)
	{
		char* bonename = StringFromXFileString(frame->Name);
		int scenebonenum = m_CompiledModel->AddBone(bonename);
		m_CompiledModel->BoneSubChunks2[scenebonenum].SetMatrix(&frame->TransformationMatrix);
		m_CompiledModel->BoneSubChunks1[scenebonenum].Parent = -1;
		
		D3DXMATRIX mat;
		D3DXMatrixIdentity(&mat);
		m_CompiledModel->BoneSubChunks1[scenebonenum].SetMatrix(&mat);
		m_CompiledModel->BoneSubChunks3[scenebonenum].SetMatrix(&mat);
		

		delete [] bonename;

		bonename = StringFromXFileString(movementdummy->Name);
		int bonenum = m_CompiledModel->AddBone(bonename);
		m_CompiledModel->BoneSubChunks2[bonenum].SetMatrix(&movementdummy->TransformationMatrix);
		m_CompiledModel->BoneSubChunks1[bonenum].Parent = scenebonenum;
		m_CompiledModel->BoneSubChunks1[scenebonenum].nChildren++;

		
		D3DXFRAME* boneoffset = movementdummy->pFrameFirstChild;
		while(boneoffset)
		{
			if (strcmp(boneoffset->Name, "BONE_OFFSET_MATRIX") == 0)
				break;
			boneoffset = boneoffset->pFrameSibling;
		}
		if (boneoffset)
		{
			m_CompiledModel->BoneSubChunks1[bonenum].SetMatrix(&boneoffset->TransformationMatrix);
			m_CompiledModel->BoneSubChunks3[bonenum].SetMatrix(&boneoffset->TransformationMatrix);
		}

		delete [] bonename;

		ImportBones(movementdummy);
	}

	ImportMeshesFromFrame(frame, &identity);

	ImportHelpers(frame, &identity);

	CreateFrameFromCompiled();

	GFX_SAFE_DELETE(m_pNode);
}

static int ParseVertexIndex(String^ faceWord)
{
	int index = faceWord->IndexOf("//");
	
	if (index >= 0)
		faceWord = faceWord->Substring(0, index);
	
	return int::Parse(faceWord)-1;
}

static void ParseVertexIndices(String^ faceWord,
							   int& riVertex,
							   int& riNormal,
							   int& riTexCoord)
{
	array<wchar_t>^ split = gcnew array<wchar_t>(1);
	split[0] = '/';
	
	array<String^>^ indices = faceWord->Split(split);

	if (indices && indices->Length == 3)
	{
		try
		{
			riVertex = int::Parse(indices[0])-1;
		}
		catch (...)
		{
			riVertex = -1;
		}
		
		try
		{
			riTexCoord = int::Parse(indices[1])-1;
		}
		catch (...)
		{
			riTexCoord = -1;
		}
		
		try
		{
			riNormal = int::Parse(indices[2])-1;
		}
		catch (...)
		{
			riNormal = -1;
		}
	}
}


void GfxModelLOD::ImportObj(String^ fileName)
{
	TextReader^ stream = gcnew StreamReader(fileName);
	
	m_CompiledModel->DestroySubMeshes();
	m_CompiledModel->DestroyMaterials();
	m_CompiledModel->DestroyBones();
	m_CompiledModel->DestroyHelpers();

	LPDIRECT3DDEVICE9 pD3DDevice =
		FableMod::Gfx::Manager::GetDevice()->GetD3DDevice();

	std::vector<D3DXVECTOR3> TmpVertices;
	TmpVertices.reserve(8000);
	std::vector<D3DXVECTOR3> TmpNormals;
	TmpNormals.reserve(8000);
	std::vector<D3DXVECTOR2> TmpTexCoords;
	TmpTexCoords.reserve(8000);
	
	std::vector<D3DXVECTOR3> Vertices;
	Vertices.reserve(8000);
	std::vector<D3DXVECTOR3> Normals;
	Normals.reserve(8000);
	std::vector<D3DXVECTOR2> TexCoords;
	TexCoords.reserve(8000);
	std::vector<int> Faces;
	Faces.reserve(8000);
	std::vector<int> FaceMaterials;
	FaceMaterials.reserve(8000);
	int iMaterial = -1;
	
	String^ line = nullptr;

	array<wchar_t>^ split = gcnew array<wchar_t>(2);
	split[0] = ' ';
	split[1] = '\n';

	bool bMaxToObj = false;
	bool bBlender = false;

	System::Globalization::CultureInfo^ culture = 
		gcnew System::Globalization::CultureInfo("en-US");
		
	while ((line = stream->ReadLine()) != nullptr)
	{
		array<String^>^ words = line->Split(split);

		for (int i = 0; i < words->Length; ++i)
		{
			String^ word = words[i];

			if (word == "#")
			{
				if (line == "# Max2Obj Version 4.0 Mar 10th, 2001")
					bMaxToObj = true;
				else if (line == "# www.blender3d.org")
					bBlender = true;

				break;
			}
			else if (word == "mtllib")
			{
				// TODO
				break;
			}
			else if (word == "usemtl")
			{
				if (Vertices.size() > 0)
				{
					BuildObjMesh(
						Vertices, 
						Normals, 
						TexCoords, 
						Faces,
						iMaterial);
				}
			
				Vertices.clear();
				TexCoords.clear();
				Normals.clear();
				Faces.clear();
				FaceMaterials.clear();

				// Set current material ID.
				iMaterial = m_CompiledModel->NumberMaterials;
			
				// Add a new material...

				IntPtr ptr = Marshal::StringToHGlobalAnsi(words[i+1]);
				char* pszName = (char*)ptr.ToPointer();

				MTRL* mat = m_CompiledModel->AddMaterial(
					m_CompiledModel->NumberMaterials);
				
				mat->Name = new char[strlen(pszName)+1];
				strcpy(mat->Name, pszName);
				
				Marshal::FreeHGlobal(ptr);

				break;
			}
			else if (word == "g" || word == "o")
			{
				// Start new object.

				if (Vertices.size() > 0)
				{
					BuildObjMesh(
						Vertices, 
						Normals, 
						TexCoords, 
						Faces,
						iMaterial);
				}
			
				Vertices.clear();
				TexCoords.clear();
				Normals.clear();
				Faces.clear();
				FaceMaterials.clear();
				
				break;
			}
			else if (word == "v")
			{
				TmpVertices.push_back(D3DXVECTOR3(
					float::Parse(words[i+1], culture),
					float::Parse(words[i+2], culture),
					float::Parse(words[i+3], culture)));
			
				break;
			}
			else if (word == "vn")
			{
				D3DXVECTOR3 n(
					float::Parse(words[i+1], culture),
					float::Parse(words[i+2], culture),
					float::Parse(words[i+3], culture));
				
				TmpNormals.push_back(n);
				
				break;
			}
			else if (word == "vt")
			{
				float x = float::Parse(words[i+1], culture);
				float y = float::Parse(words[i+2], culture);

				if (bBlender)
				{
					y *= -1.0f;
				}

				TmpTexCoords.push_back(D3DXVECTOR2(x, y));

				break;
			}
			else if (word == "f")
			{
				if (words->Length == 5)
				{
					throw gcnew System::Exception(
						"Triangulated mesh required");
				}
				else if (words->Length == 4)
				{
					int iVertex,iTex,iNormal;

					for (int j = 1; j <= 3; ++j)
					{
						ParseVertexIndices(
							words[i+j], iVertex, iNormal, iTex);

						Vertices.push_back(TmpVertices[iVertex]);

						if (iNormal >= 0 && !TmpNormals.empty())
							Normals.push_back(TmpNormals[iNormal]);

						if (iTex >= 0)
							TexCoords.push_back(TmpTexCoords[iTex]);
					}	

					int iIndex = Vertices.size();

					Faces.push_back(iIndex+2);
					Faces.push_back(iIndex+1);	
					Faces.push_back(iIndex+0);	
					
					FaceMaterials.push_back(iMaterial);
				}
				else
				{
					throw gcnew System::Exception(
						"File format not supported");
				}

				break;
			}
		}
	}

	if (iMaterial < 0)
	{
		iMaterial = m_CompiledModel->NumberMaterials;
		MTRL* mat = m_CompiledModel->AddMaterial(
			m_CompiledModel->NumberMaterials);
		mat->Name = new char[8];
		strcpy(mat->Name, "DEFAULT");
	}

	if (Vertices.size() > 0)
	{
		BuildObjMesh(
			Vertices, 
			Normals, 
			TexCoords, 
			Faces,
			iMaterial);
	}

	stream->Close();

	CreateFrameFromCompiled();

	GFX_SAFE_DELETE(m_pNode);
}

void GfxModelLOD::BuildObjMesh(
	const std::vector<D3DXVECTOR3>& Vertices,
	const std::vector<D3DXVECTOR3>& Normals,
	const std::vector<D3DXVECTOR2>& TexCoords,
	const std::vector<int>& Faces,
	int iMaterial)
{
	SUBM* pMesh = m_CompiledModel->AddSubMesh(); 

	int iNumFaces = Faces.size()/3;

	WORD* awFaces = new WORD[Faces.size()];
			
	for (int i = 0; i < Faces.size(); ++i)
		awFaces[i] = Faces[i];
	
	Vertex_20_20* aCompVerts = new Vertex_20_20[Vertices.size()];

	const bool bNormals = Normals.size() == Vertices.size();
	const bool bTex = TexCoords.size() == Vertices.size();

	for (int i = 0; i < Vertices.size(); ++i)
	{
		aCompVerts[i].Coordinates.x = Vertices[i].x;
		aCompVerts[i].Coordinates.y = Vertices[i].y;
		aCompVerts[i].Coordinates.z = Vertices[i].z;

		if (bNormals)
		{
			aCompVerts[i].NormalVector.SetX(Normals[i].x);
			aCompVerts[i].NormalVector.SetY(Normals[i].y);
			aCompVerts[i].NormalVector.SetZ(Normals[i].z);
		}

		if (bTex)
		{
			aCompVerts[i].tu = TexCoords[i].x;
			aCompVerts[i].tv = TexCoords[i].y;
		}
	}

	pMesh->Number_Faces = iNumFaces;
	pMesh->Number_Verts = Vertices.size();

	pMesh->TriangleStrips = awFaces;
	pMesh->TriangleStripLength = Faces.size();

	pMesh->floats[0] = 1.0f;
	pMesh->floats[1] = 1.0f;
	pMesh->floats[2] = 1.0f;
	pMesh->floats[3] = 1.0f;

	pMesh->Vertices = (char*)aCompVerts;

	pMesh->VertexSize = 20;
	pMesh->VertexSubType = 20;

	pMesh->floatsa[0] = 1.9544531;
	pMesh->floatsa[1] = -73.631340;
	pMesh->floatsa[2] = 496.84158;
	pMesh->floatsa[3] = 1592.5869;
	pMesh->floatsa[4] = 0.00073368527;

	pMesh->Header_count1 = 1;
	pMesh->Header_count1_dupe = 1;
	pMesh->SubChunks1 = new SUBM_SUB_CHUNK_1;
	memset(pMesh->SubChunks1, 0, sizeof(SUBM_SUB_CHUNK_1));
	pMesh->SubChunks1->nFaceVertexIndices = iNumFaces;
	pMesh->SubChunks1->sFaceVertexIndices = 0;
	pMesh->SubChunks1->Ignored_FaceVertexIndices = 0; 

	pMesh->MaterialID = iMaterial;
}

void GfxModelLOD::ImportMeshesFromFrame(D3DXFRAME* frame, const D3DXMATRIX* parentTransform)
{
	D3DXMATRIX transform;
	transform = frame->TransformationMatrix*(*parentTransform);

	if (frame->pMeshContainer)
	{
		if (frame->pMeshContainer->MeshData.pMesh)
		{
			LPD3DXMESH pMesh = frame->pMeshContainer->MeshData.pMesh;
				
			Vertex* verts;
			WORD* faces;
			WORD* tmpfaces;
			
			SUBM* subm = m_CompiledModel->AddSubMesh();
					
			DWORD numfaces = pMesh->GetNumFaces();
			DWORD numverts = pMesh->GetNumVertices();
				
			faces = new WORD[numfaces*3];
			
			pMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&tmpfaces);
			
			for (int i = 0; i < numfaces*3; i += 3)
			{
				faces[i]   = tmpfaces[i+2];
				faces[i+1] = tmpfaces[i+1];
				faces[i+2] = tmpfaces[i];
			}

			pMesh->UnlockIndexBuffer();

			subm->Number_Faces = numfaces;
			subm->Number_Verts = numverts;

			subm->TriangleStrips = faces;
			subm->TriangleStripLength = numfaces*3;

			subm->floats[0] = 1.0f;
			subm->floats[1] = 1.0f;
			subm->floats[2] = 1.0f;
			subm->floats[3] = 1.0f;

			if (frame->pMeshContainer->pSkinInfo)
			{
				Vertex_28_20* compverts;

				compverts = new Vertex_28_20[numverts];
				memset(compverts, 0, sizeof(Vertex_28_20)*numverts);

				pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&verts);

				for(int i=0;i<numverts;i++)
				{
					D3DXVECTOR4 tmpvec;

					tmpvec.w = 1.0;
					tmpvec.x = verts[i].x;
					tmpvec.y = verts[i].y;
					tmpvec.z = verts[i].z;

					D3DXVec4Transform(&tmpvec, &tmpvec, &transform);

					compverts[i].Coordinates.x = tmpvec.x;
					compverts[i].Coordinates.y = tmpvec.y;
					compverts[i].Coordinates.z = tmpvec.z;

					compverts[i].NormalVector.SetX(verts[i].nx);
					compverts[i].NormalVector.SetY(verts[i].ny);
					compverts[i].NormalVector.SetZ(verts[i].nz);

					compverts[i].tu = verts[i].tu;
					compverts[i].tv = verts[i].tv;
				}
				
				pMesh->UnlockVertexBuffer();

				int numbones = 
					frame->pMeshContainer->pSkinInfo->GetNumBones();
				
				int maxinfluences = 0;
				
				for(int i=0;i<numbones;i++)
				{
					char* pBoneName = StringFromXFileString(frame->pMeshContainer->pSkinInfo->GetBoneName(i));
					int boneindex = this->m_CompiledModel->FindBone( 0xffffffff - ZLib::CRC32(0xffffffff, (const unsigned char*)pBoneName, strlen(pBoneName)));
					D3DXMATRIX* mat = frame->pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);
					m_CompiledModel->BoneSubChunks1[boneindex].SetMatrix(mat);
					m_CompiledModel->BoneSubChunks3[boneindex].SetMatrix(mat);
					int numinfluences = frame->pMeshContainer->pSkinInfo->GetNumBoneInfluences(i);
					DWORD* vertindices = new DWORD[numinfluences];
					float* weights = new float[numinfluences];
					frame->pMeshContainer->pSkinInfo->GetBoneInfluence(i, vertindices, weights);

					for(int j=0;j<numinfluences;j++)
					{
						Vertex_28_20* v = &compverts[vertindices[j]];

						int n = 0;
						for(;n<4;n++)
							if (v->BoneWeights[n] == 0)
								break;

						v->BoneIndices[n] = boneindex;
						v->BoneWeights[n] = (unsigned char)System::Math::Round((weights[j]*255));
						if (n+1 > maxinfluences)
							maxinfluences = n+1;
					}
					delete [] pBoneName;
					delete [] vertindices;
					delete [] weights;
				}

				unsigned char* usedbones = new unsigned char[m_CompiledModel->NumberBones];
				int numusedbones = 0;
				for(int i=0;i<numverts;i++)
				{
					for(int j=0;j<4;j++)
					{
						if (compverts[i].BoneWeights[j] == 0)
						{
							compverts[i].BoneIndices[j] = 0;
							continue;
						}

						int boneindex = compverts[i].BoneIndices[j];
						bool found = false;
						// search bone buffer for current bone, remember location.
						int k=0;
						for(;k<numusedbones;k++)
						{
							if (usedbones[k] == boneindex)
							{
								found = true;
								break;
							}
						}
						if (!found)
						{
							// if not found, add it
							usedbones[numusedbones] = boneindex;
							numusedbones++;
						}
						// convert bone indices from model to local subm sub chunk bone buffer
						compverts[i].BoneIndices[j] = k*3;
					}
				}

				subm->Vertices = (char*)compverts;

				subm->VertexSize = 28;
				subm->VertexSubType = 20;

				subm->floatsa[0] = 0.014189702;
				subm->floatsa[1] = 6.4840946;
				subm->floatsa[2] = 140.26567;
				subm->floatsa[3] = 33.878929;
				subm->floatsa[4] = 0.016182739;

				subm->Header_count2 = 1;
				subm->Header_count2_dupe = 1;
				subm->SubChunks2 = new SUBM_SUB_CHUNK_2*;
				subm->SubChunks2[0] = new SUBM_SUB_CHUNK_2;
				memset((*subm->SubChunks2), 0, sizeof(**subm->SubChunks2));

				subm->SubChunks2[0]->nFaceVertexIndices = numfaces;
				subm->SubChunks2[0]->sFaceVertexIndices = 0;
				// this will make it do triangle list, not strip
				subm->SubChunks2[0]->Ignored_FaceVertexIndices = 0; 
				subm->SubChunks2[0]->nBoneIndice = numusedbones;
				subm->SubChunks2[0]->nBone = new unsigned char[numusedbones];
				memcpy(subm->SubChunks2[0]->nBone, usedbones, numusedbones);
				delete [] usedbones;

				subm->SubChunks2[0]->Unknown = 1; // don't ask why

				// i think this is max influences, oh well
				subm->SubChunks2[0]->Possible_Weights = maxinfluences; 
			}
			else 
			{
				Vertex_20_20* compverts = new Vertex_20_20[numverts];
				
				pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&verts);

				for(int i=0;i<numverts;i++)
				{
					D3DXVECTOR4 tmpvec;

					tmpvec.w = 1.0;
					tmpvec.x = verts[i].x;
					tmpvec.y = verts[i].y;
					tmpvec.z = verts[i].z;

					D3DXVec4Transform(&tmpvec, &tmpvec, &transform);

					compverts[i].Coordinates.x = tmpvec.x;
					compverts[i].Coordinates.y = tmpvec.y;
					compverts[i].Coordinates.z = tmpvec.z;

					compverts[i].NormalVector.SetX(verts[i].nx);
					compverts[i].NormalVector.SetY(verts[i].ny);
					compverts[i].NormalVector.SetZ(verts[i].nz);

					compverts[i].tu = verts[i].tu;
					compverts[i].tv = verts[i].tv;
				}

				pMesh->UnlockVertexBuffer();

				subm->Vertices = (char*)compverts;

				subm->VertexSize = 20;
				subm->VertexSubType = 20;

				subm->floatsa[0] = 1.9544531;
				subm->floatsa[1] = -73.631340;
				subm->floatsa[2] = 496.84158;
				subm->floatsa[3] = 1592.5869;
				subm->floatsa[4] = 0.00073368527;

				subm->Header_count1 = 1;
				subm->Header_count1_dupe = 1;
				subm->SubChunks1 = new SUBM_SUB_CHUNK_1;
				memset(subm->SubChunks1, 0, sizeof(SUBM_SUB_CHUNK_1));
				subm->SubChunks1->nFaceVertexIndices = numfaces;
				subm->SubChunks1->sFaceVertexIndices = 0;
				subm->SubChunks1->Ignored_FaceVertexIndices = 0; 
				// this will make it do triangle list, not strip
			}

			subm->MaterialID = m_CompiledModel->NumberMaterials;
			MTRL* mat = m_CompiledModel->AddMaterial(
				m_CompiledModel->NumberMaterials);
			mat->Name = new char[strlen(frame->Name)+1];
			strcpy(mat->Name, frame->Name);

			D3DXMATERIAL d3dmat = frame->pMeshContainer->pMaterials[0];

			if (d3dmat.pTextureFilename)
			{
				String^ textureFile = 
					gcnew String(d3dmat.pTextureFilename);
				
				try
				{
					UInt32 id = UInt32::Parse(textureFile->Substring(
						0, textureFile->IndexOf('.')));

					if (FindTexture(id))
						mat->BASE_Texture_ID = id;
				}
				catch (...)
				{
				}
			}
		}
	}
	
	if (frame->pFrameFirstChild)
		ImportMeshesFromFrame(frame->pFrameFirstChild, &transform);
	
	if (frame->pFrameSibling)
		ImportMeshesFromFrame(frame->pFrameSibling, &transform);
}
void GfxModelLOD::ImportHelpers(D3DXFRAME* frame, const D3DXMATRIX* parentTransform)
{
	D3DXFRAME* child = frame->pFrameFirstChild;
	char* name = StringFromXFileString(frame->Name);
	D3DXMATRIX transform = frame->TransformationMatrix * (*parentTransform);

	while(child != NULL)
	{
		if (strncmp("HPNT_", child->Name, 5) == 0)
		{
			char* hpntname = StringFromXFileString(child->Name+5);
			HPNT* hpnt = m_CompiledModel->AddHPNT(hpntname);
			DWORD parentcrc = 0xffffffff - ZLib::CRC32(0xffffffff, (unsigned char*)name, strlen(name));
			hpnt->Parent = m_CompiledModel->FindBone(parentcrc);
			D3DXMATRIX hpnttransform = child->TransformationMatrix * transform;
			//D3DXMATRIX hpnttransforma = transform * child->TransformationMatrix;
			//D3DXMATRIX hpnttransformb = child->TransformationMatrix * transform;
			hpnt->SetMatrix(&hpnttransform);
			delete [] hpntname;
		}
		else if (strncmp("HDMY_", child->Name, 5) == 0)
		{
			char* hdmyname = StringFromXFileString(child->Name+5);
			HDMY* hdmy = m_CompiledModel->AddHDMY(hdmyname);
			DWORD parentcrc = 0xffffffff - ZLib::CRC32(0xffffffff, (unsigned char*)name, strlen(name));
			hdmy->Parent = m_CompiledModel->FindBone(parentcrc);
			D3DXMATRIX hdmytransform = child->TransformationMatrix * transform;
			hdmy->SetMatrix(&hdmytransform);
			delete [] hdmyname;
		}
		else
		{
			ImportHelpers(child, &transform);
		}
		child = child->pFrameSibling;
	}
	delete [] name;
}
void GfxModelLOD::ImportBones(D3DXFRAME* frame)
{
	D3DXFRAME* child = frame->pFrameFirstChild;
	char* name = StringFromXFileString(frame->Name);
	while (child!=NULL)
	{
		if (strncmp(child->Name, "HPNT_",5) != 0 && strncmp(child->Name, "HDMY_",5) != 0 && strcmp(child->Name, "BONE_OFFSET_MATRIX"))
		{
			char* bonename = StringFromXFileString(child->Name);
			int bonenum = m_CompiledModel->AddBone(bonename);
			m_CompiledModel->BoneSubChunks2[bonenum].SetMatrix(&child->TransformationMatrix);

			D3DXFRAME* boneoffset = child->pFrameFirstChild;
			while(boneoffset)
			{
				if (strcmp(boneoffset->Name, "BONE_OFFSET_MATRIX") == 0)
					break;
				boneoffset = boneoffset->pFrameSibling;
			}
			if (boneoffset)
			{
				m_CompiledModel->BoneSubChunks1[bonenum].SetMatrix(&boneoffset->TransformationMatrix);
				m_CompiledModel->BoneSubChunks3[bonenum].SetMatrix(&boneoffset->TransformationMatrix);
			}

			//m_CompiledModel->BoneSubChunks3[bonenum].SetMatrix(&child->TransformationMatrix);
			DWORD parentcrc = 0xffffffff - ZLib::CRC32(0xffffffff, (unsigned char*)name, strlen(name));
			m_CompiledModel->BoneSubChunks1[bonenum].Parent = m_CompiledModel->FindBone(parentcrc);
			m_CompiledModel->BoneSubChunks1[m_CompiledModel->BoneSubChunks1[bonenum].Parent].nChildren++;

			delete [] bonename;
			ImportBones(child);
		}
		child = child->pFrameSibling;
	}

	//child = frame->pFrameFirstChild;
	//while (child!=NULL)
	//{
	//	if (strncmp(child->Name, "HPNT_",5) != 0 && strncmp(child->Name, "HDMY_",5) != 0)
	//	{
	//		ImportBones(child);
	//	}
	//	child = child->pFrameSibling;
	//}

	delete [] name;
}
int GfxModelLOD::MaterialCount::get()
{
	return m_CompiledModel->NumberMaterials;
}
Material^ GfxModelLOD::Materials::get(unsigned int index)
{
	if (index < m_CompiledModel->NumberMaterials)
		return gcnew Material(&m_CompiledModel->Materials[index]);
	else
		return nullptr;
}
Material^ GfxModelLOD::AddMaterial(unsigned int id)
{
	return gcnew Material(m_CompiledModel->AddMaterial(id));
}
void GfxModelLOD::RemoveMaterial(unsigned int id)
{
	m_CompiledModel->RemoveMaterial(id);
}

int GfxModelLOD::SubMeshCount::get()
{
	return m_CompiledModel->NumberSubMeshes;
}

SubMesh^ GfxModelLOD::SubMeshes::get(unsigned int index)
{
	if (index < m_CompiledModel->NumberSubMeshes)
		return gcnew SubMesh(m_CompiledModel->SubMeshes[index]);
	else
		return nullptr;
}

}
}
}