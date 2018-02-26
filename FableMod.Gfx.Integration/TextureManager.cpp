#include "Stdafx.h"
#include "TextureManager.h"

using namespace FableMod::ContentManagement;
using namespace FableMod::BIG;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

TextureManager::TextureManager() :
m_Textures(gcnew Dictionary<UInt32, GfxTexture^>())
{
}

TextureManager::~TextureManager()
{
	Clear();
}

void TextureManager::Clear()
{
	FM_DBG_LOG0(_T("TextureManager::Clear...\n"));

	for each (KeyValuePair<UInt32, GfxTexture^> kvp in m_Textures)
		kvp.Value->~GfxTexture();
	
	m_Textures->Clear();
}

GfxTexture^ TextureManager::Get(UInt32 id)
{
	GfxTexture^ texture = nullptr;

	if (m_Textures->TryGetValue(id, texture))
	{
		// Already loaded.
		return texture;
	}

	ContentObject^ obj = ContentManager::Instance->FindEntry(
		LinkDestination::MainTextureID, id);

	if (obj != nullptr)
	{
		// Create a new texture.
		texture = gcnew GfxTexture((AssetEntry^)obj->Object);		
		m_Textures[id] = texture;
	}

	return texture;
}

}
}
}