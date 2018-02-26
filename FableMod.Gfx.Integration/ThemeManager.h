#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Collections::Generic;

#include "TextureManager.h"
#include "ModelManager.h"

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

public ref class Theme
{
public:

	property GfxTexture^ Texture 
	{ 
		GfxTexture^ get() { return m_Texture; } 
		void set(GfxTexture^ value) { m_Texture = value; } 
	}

	property GfxModel^ Model 
	{ 
		GfxModel^ get() { return m_Model; } 
		void set(GfxModel^ value) { m_Model = value; } 
	}

private:

	GfxTexture^ m_Texture;
	GfxModel^ m_Model;
};

public ref class ThemeManager
{
public:

	ThemeManager(
		TextureManager^ textureManager,
		ModelManager^ modelManager);

	~ThemeManager();

	void Clear();

	property Theme^ Themes[String^]
	{
		Theme^ get(String^ name);
	}

private:

	void FindModel(Theme^ theme, unsigned __int32 entry);

	Dictionary<String^, Theme^>^ m_Themes;
	
	TextureManager^ m_TextureManager;
	ModelManager^ m_ModelManager;
};

}
}
}