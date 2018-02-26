#pragma once

using namespace System;
using namespace FableMod;

#include "Map.h"
#include "GfxThingController.h"

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

public ref class ThingMap : public Map
{
public:

	ThingMap();

	virtual ~ThingMap();

	virtual void Destroy() override;

	void Create(
		GfxThingController^ controller,
		String^ name,
		int mapX, int mapY, 
		FableMod::LEV::LEVFile^ lev,
		FableMod::STB::Level^ stbLevel,
		FableMod::TNG::TNGFile^ tng); 

	void AddThing(
		GfxThingController^ controller,
		FableMod::TNG::Thing^ thing,
		String^ section,
		float x, float y, float z,
		float nx, float ny, float nz);

	void AddThing(
		GfxThingController^ controller,
		FableMod::TNG::Thing^ thing,
		String^ section);

	property ::FableMod::TNG::TNGFile^ TNG 
	{  
		FableMod::TNG::TNGFile^ get() { return m_Tng; } 
	}
	
	property bool Modified
	{
		virtual bool get() override;
	}

protected:

	bool ValidObject(FableMod::TNG::Thing^ thing);

private:

	FableMod::TNG::TNGFile^ m_Tng;
};

}
}
}