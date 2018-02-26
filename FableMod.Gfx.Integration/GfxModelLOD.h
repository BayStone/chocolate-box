#pragma once

#include "ModelStructs.h"
#include "CompiledModel.h"
#include "SubMesh.h"
#include "Material.h"

using namespace System;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

public ref class GfxModelLOD : public IDisposable
{
public:
	
	GfxModelLOD(char* data, unsigned int length);
	
	~GfxModelLOD();

	!GfxModelLOD();

	void DestroyCompiledModel();

	void DestroyFrames();

	void Save(char* data, unsigned int* length);
	
	void ExportX(String^ fileName);
	
	void ImportX(String^ fileName);
	void ImportObj(String^ fileName);
	
	property int MaterialCount
	{
		int get();
	}
	
	property Material^ Materials[unsigned int]
	{
		Material^ get(unsigned int index);
	}
	
	Material^ AddMaterial(unsigned int id);
	
	void RemoveMaterial(unsigned int id);
	
	property int SubMeshCount
	{
		int get();
	}

	property SubMesh^ SubMeshes[unsigned int]
	{
		SubMesh^ get(unsigned int index);
	}

	void BuildGfx(FableMod::Gfx::Node* pRoot);

	FableMod::Gfx::Node* GetGfx();

internal:

	CompiledModel* m_CompiledModel;

private:
	
	FableMod::Gfx::Texture* FindTexture(UInt32 id);

	void BuildObjMesh(
		const std::vector<D3DXVECTOR3>& Vertices,
		const std::vector<D3DXVECTOR3>& Normals,
		const std::vector<D3DXVECTOR2>& TexCoords,
		const std::vector<int>& Faces,
		int iMaterial);
	void ImportMeshesFromFrame(D3DXFRAME* frame, const D3DXMATRIX* parentTransform);
	void ImportHelpers(D3DXFRAME* frame, const D3DXMATRIX* parentTransform);
	void ImportBones(D3DXFRAME* frame);
	void ExportTextures(String^ path);
	void CreateFrameFromCompiled();
	D3DXMATRIX* GetBoneTransform(D3DXEXTENDEDFRAME* frame, const char* name);

	bool m_CompiledSynced;
	D3DXEXTENDEDFRAME* m_SceneRoot;
	FableMod::Gfx::NodePtr* m_pNode;
};

}
}
}