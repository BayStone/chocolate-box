#pragma once

#include "GfxModel.h"

using namespace System;
using namespace System::Collections::Generic;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

public ref class Building
{
	Building() {}

	virtual ~Building()
	{
		m_Interior = nullptr;
		m_Exterior = nullptr;
	}

	property GfxModel^ Interior
	{
		GfxModel^ get() { return m_Interior; }
		void set(GfxModel^ value) { m_Interior = value; }
	}

	property GfxModel^ Exterior
	{
		GfxModel^ get() { return m_Exterior; }
		void set(GfxModel^ value) { m_Exterior = value; }
	}

private:

	GfxModel^ m_Interior;
	GfxModel^ m_Exterior;
};

public ref class ModelManager
{
public:

	ModelManager();

	virtual ~ModelManager();

	void Clear();

	GfxModel^ Get(UInt32 id);

	GfxModel^ Get(String^ name);

protected:

	GfxModel^ AddModel(AssetEntry^ entry);

	Dictionary<UInt32, GfxModel^>^ m_Models;
	Dictionary<String^, GfxModel^>^ m_NameModels;
	Dictionary<String^, Building^>^ m_Buildings;
};

}
}
}
