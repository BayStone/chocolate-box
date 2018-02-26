#pragma once

using namespace System;
using namespace System::Collections::Generic;

#include "GfxTexture.h"

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

public ref class TextureManager
{
public:

	TextureManager();

	virtual ~TextureManager();

	void Clear();

	GfxTexture^ Get(UInt32 id);

protected:

	Dictionary<UInt32, GfxTexture^>^ m_Textures;
};

}
}
}
