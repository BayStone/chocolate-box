#include "Stdafx.h"
#include "GfxModel.h"

using namespace FableMod::ContentManagement;
using namespace System::Collections::Specialized;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

ModelSubHeader::ModelSubHeader(char* data)
{
	memset(this, 0, sizeof(*this));

	int offset = 0;
	memcpy(&Physics, data+offset, 48);
	offset+=48;
	LODSizes = new DWORD[LODCount];
	memcpy(LODSizes, data+offset, sizeof(DWORD)*LODCount);
	offset+=sizeof(DWORD)*LODCount;

	memcpy(&padding, data+offset, 4);
	offset+=4;

	offset+=sizeof(DWORD)*(LODCount-1);

	memcpy(&TextureCount, data+offset, 4);
	offset+=4;

	TextureIDs = new DWORD[TextureCount];
	memcpy(TextureIDs, data+offset, sizeof(DWORD)*TextureCount);
	offset+=sizeof(DWORD)*TextureCount;
}

void ModelSubHeader::WriteToBuffer(char* data, unsigned int* length)
{
	int offset =0;

	memcpy(data+offset, &Physics, 48);
	offset+=48;

	memcpy(data+offset, LODSizes,  sizeof(DWORD)*LODCount);
	offset+= sizeof(DWORD)*LODCount;

	memcpy(data+offset, &padding, 4);
	offset+=4;

	for(int i=0;i<(LODCount-1);i++)
	{
		float cutoffdist = 
			0.019375f+System::Math::Pow(8.0f/3.0f,(float)i)*0.005625f;
		memcpy(data+offset,&cutoffdist, 4);
		offset+=4;
	}

	memcpy(data+offset, &TextureCount, 4);
	offset+=4;

	memcpy(data+offset, TextureIDs, sizeof(DWORD)*TextureCount);
	offset+=sizeof(DWORD)*TextureCount;

	*length = offset;
}

ModelSubHeader::~ModelSubHeader()
{
	delete[] LODSizes;
	delete[] TextureIDs;
}

//---------------------------------------------------------------------------

GfxModel::GfxModel(AssetEntry^ entry) :
GfxBaseModel(entry)
{
	m_SubHeader = new ModelSubHeader(entry->GetSubHeader());
	
	m_LODs = gcnew array<GfxModelLOD^>(LODCount);
	
	int offset = 0;
	
	for(int i = 0; i < LODCount-1; ++i)
	{
		m_LODs[i] = gcnew GfxModelLOD(
			entry->GetData()+offset, m_SubHeader->LODSizes[i]);
		
		offset+=m_SubHeader->LODSizes[i];
	}

	m_LODs[LODCount-1] = gcnew GfxModelLOD(
		entry->GetData()+offset, entry->Length-offset);

	if (!entry->Modified)
		entry->Purge();
}

GfxModel::~GfxModel()
{
	if (m_LODs)
	{
		for (int i = 0; i < m_LODs->Length; ++i)
			m_LODs[i]->~GfxModelLOD();
		
		m_LODs = nullptr;
	}

	this->!GfxModel();
}

GfxModel::!GfxModel()
{
	GFX_SAFE_DELETE(m_SubHeader);
}

void GfxModel::CompileToEntry(AssetEntry^ entry)
{
	char* data = new char[1024*1024];

	int offset = 0;
		
	if (m_SubHeader)
	{
		for (int i = 0; i < LODCount-1; ++i)
		{
			m_LODs[i]->Save(
				data+offset, (unsigned int*)&m_SubHeader->LODSizes[i]);
			
			offset += m_SubHeader->LODSizes[i];
		}

		unsigned int tmp;
	
		m_LODs[LODCount-1]->Save(data+offset, &tmp);
		offset += tmp;
	}

	entry->SetData(data, offset);

	unsigned int len = 1024*1024;

	// update used textures
	// figure 256 textures is safe for a single model...
	
	if (m_SubHeader)
	{
		DWORD* textureids = new DWORD[256]; 

		int textcount = 0;

		for(int i=0;i<LODCount;i++)
		{
			CompiledModel* pCompiled = LODs[i]->m_CompiledModel;

			for (int j = 0; j < pCompiled->NumberMaterials; ++j)
			{
				bool textfound = false;
				bool specfound = false;
				bool bumpfound = false;

				if (pCompiled->Materials[j].BASE_Texture_ID)
				{
					for (int k = 0; k < textcount; ++k)
					{
						if (textureids[k] == 
							pCompiled->Materials[j].BASE_Texture_ID)
							textfound = true;
					}
				}
				else
				{
					textfound = true;
				}

				if (pCompiled->Materials[j].BUMPMAP_Texture_ID)
				{
					for (int k = 0; k < textcount; ++k)
					{
						if (textureids[k] == 
							pCompiled->Materials[j].BUMPMAP_Texture_ID)
							bumpfound = true;
					}
				}
				else
				{
					bumpfound = true;
				}

				if (pCompiled->Materials[j].REFLECT_Texture_ID)
				{
					for (int k = 0; k < textcount; ++k)
					{
						if (textureids[k] == 
							pCompiled->Materials[j].REFLECT_Texture_ID)
							specfound = true;
					}
				}
				else
				{
					specfound = true;
				}

				if (!textfound)
				{
					textureids[textcount] = 
						pCompiled->Materials[j].BASE_Texture_ID;
					textcount++;
				}
				if (!bumpfound)
				{
					textureids[textcount] = 
						pCompiled->Materials[j].BUMPMAP_Texture_ID;
					textcount++;
				}
				if (!specfound)
				{
					textureids[textcount] = 
						pCompiled->Materials[j].REFLECT_Texture_ID;
					textcount++;
				}

			}
		}

		delete[] m_SubHeader->TextureIDs;

		m_SubHeader->TextureIDs = textureids;
		m_SubHeader->TextureCount = textcount;

		m_SubHeader->WriteToBuffer(data, &len);

		entry->SetSubHeader(data, len);
	}
}

unsigned int GfxModel::LODCount::get()
{
	if (m_SubHeader)
		return m_SubHeader->LODCount+1;
	else if (m_LODs)
		return m_LODs->Length;
			
	return 0;
}

GfxModelLOD^ GfxModel::LODs::get(int index)
{
	if (index < LODCount)
		return m_LODs[index];
	else
		return nullptr;
}

void GfxModel::LODs::set(int index, GfxModelLOD^ lod)
{
	if (index < LODCount)
		m_LODs[index] = lod;
}

UInt32 GfxModel::Physics::get()
{
	return m_SubHeader->Physics;
}

void GfxModel::Physics::set(UInt32 value)
{
	m_SubHeader->Physics = value;
}

FableMod::Gfx::Node* GfxModel::BuildModel()
{
	//FableMod::Gfx::LODNode* pNode = new FableMod::Gfx::LODNode();
    //for (int i = 0; i < LODCount; ++i)
	//	pNode->AttachChild(m_LODs[i]->GetAsSpatial());
	//return pNode;

	// Just use the highest model.
	return m_LODs[0]->GetGfx();
}

//--------------------------------------------------------------------------
// MOD Package Interface
//--------------------------------------------------------------------------

static AssetEntry^ FindTexture(UInt32 id)
{
	ContentObject^ o = ContentManager::Instance->FindEntry(
		LinkDestination::MainTextureID, id);

	if (o)
		return (AssetEntry^)o->Object;

	System::Diagnostics::Debug::Assert(false);

	return nullptr;
}

static AssetEntry^ FindTexture(String^ name)
{
	ContentObject^ o = ContentManager::Instance->FindEntry(
		LinkDestination::MainTextureName, name);

	if (o)
		return (AssetEntry^)o->Object;

	System::Diagnostics::Debug::Assert(false);

	return nullptr;
}

NameValueCollection^ GfxModel::CreateLinks()
{
	NameValueCollection^ c = gcnew NameValueCollection();

	ContentObject^ o = ContentManager::Instance->FindEntry(
		LinkDestination::ModelID, Physics);
	
	if (o)
	{
		c["PhysicsModel"] = ((AssetEntry^)o->Object)->DevSymbolName;
		o = nullptr;
	}
	
	for (int i = 0; i < LODCount; ++i)
	{
		for (int j = 0; j < LODs[i]->MaterialCount; ++j)
		{
			Material^ mat = LODs[i]->Materials[j];
			
			String^ baseName = 
				"LOD"+i.ToString()+".MAT"+j.ToString();

			if (mat->BaseTextureID != 0)
			{
				AssetEntry^ tex = FindTexture(mat->BaseTextureID);
				c->Add(baseName+".BaseTexture", tex->DevSymbolName);
			}
			
			if (mat->BumpMapTextureID != 0)
			{
				AssetEntry^ tex = FindTexture(mat->BumpMapTextureID);
				c->Add(baseName+".BumpMapTexture", tex->DevSymbolName);
			}
			
			if (mat->ReflectionTextureID != 0)
			{
				AssetEntry^ tex = FindTexture(mat->ReflectionTextureID);
				c->Add(baseName+".SpecularMapTexture", tex->DevSymbolName);
			}
		}
	}	

	return c;
}

void GfxModel::ApplyLinks(NameValueCollection^ c)
{
	String^ physics = c["PhysicsModel"];
			 
	if (physics)
	{
		ContentObject^ o = ContentManager::Instance->FindEntry(
			LinkDestination::ModelName, physics);
	
		if (o)
		{
			Physics = ((AssetEntry^)o->Object)->ID;
			o = nullptr;
		}
	}

	for (int i = 0; i < LODCount; ++i)
	{
		for (int j = 0; j < LODs[i]->MaterialCount; ++j)
		{
			Material^ mat = LODs[i]->Materials[j];
			
			String^ baseName = 
				"LOD"+i.ToString()+".MAT"+j.ToString();

			String^ texName = c[baseName+".BaseTexture"];
			
			if (texName)
			{
				AssetEntry^ tex = FindTexture(texName);
				if (tex)
					mat->BaseTextureID = tex->ID;
			}

			texName = c[baseName+".BumpMapTexture"];
			
			if (texName)
			{
				AssetEntry^ tex = FindTexture(texName);
				if (tex)
					mat->BumpMapTextureID = tex->ID;
			}

			texName = c[baseName+".SpecularMapTexture"];
			
			if (texName)
			{
				AssetEntry^ tex = FindTexture(texName);
				if (tex)
					mat->ReflectionTextureID = tex->ID;
			}
		}
	}
}

}
}
}