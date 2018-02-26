#include "Stdafx.h"
#include "ModelManager.h"
#include "TextureManager.h"

using namespace FableMod::ContentManagement;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

ModelManager::ModelManager() :
m_Models(gcnew Dictionary<UInt32, GfxModel^>()),
m_NameModels(gcnew Dictionary<String^, GfxModel^>())
{
}

ModelManager::~ModelManager()
{
	Clear();
}

void ModelManager::Clear()
{
	FM_DBG_LOG0(_T("ModelManager::Clear...\n"));

	for each (KeyValuePair<UInt32, GfxModel^> kvp in m_Models)
		kvp.Value->~GfxModel();
	
	m_Models->Clear();

	m_NameModels->Clear();
}

GfxModel^ ModelManager::AddModel(AssetEntry^ entry)
{
	GfxModel^ model = gcnew GfxModel(entry);
	
	m_Models[entry->ID] = model;
	m_NameModels[entry->DevSymbolName] = model;

	return model;
}

GfxModel^ ModelManager::Get(UInt32 id)
{
	GfxModel^ model = nullptr;

	if (m_Models->TryGetValue(id, model))
	{
		// Already loaded.
		return model;
	}

	ContentObject^ obj = ContentManager::Instance->FindEntry(
		LinkDestination::ModelID, id);

	if (obj != nullptr)
	{
		//Console::WriteLine("Model: {0}", obj->Name);

		return AddModel((AssetEntry^)obj->Object);
	}

	return nullptr;
}

/// For the custom TNG definition work.
GfxModel^ ModelManager::Get(String^ name)
{
	GfxModel^ model = nullptr;

	if (m_NameModels->TryGetValue(name, model))
	{
		// Already loaded.
		return model;
	}

	ContentObject^ obj = ContentManager::Instance->FindEntry(
		LinkDestination::ModelName, name);

	if (obj != nullptr)
	{
		return AddModel((AssetEntry^)obj->Object);
	}

	return nullptr;
}

}
}
}