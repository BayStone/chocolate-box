#include "Stdafx.h"
#include "ThingMap.h"
#include "GfxThingInterface.h"
#include "GfxManager.h"
#include "ModelManager.h"

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

ThingMap::ThingMap()
{
}

ThingMap::~ThingMap()
{
	Destroy();
}

void ThingMap::Destroy()
{
	Map::Destroy();

	if (m_Tng)
	{
		m_Tng->Destroy();
		m_Tng = nullptr;
	}
}

bool ThingMap::ValidObject(FableMod::TNG::Thing^ thing)
{
	return
		thing->Name == "Object" || 
		thing->Name == "Building" ||
		thing->Name == "Holy Site" ||
		thing->Name == "Thing" ||
		thing->Name == "Marker" ||
		thing->Name == "AICreature";
}

void ThingMap::Create(GfxThingController^ controller,
					  String^ name,
					  int mapX, int mapY, 
					  FableMod::LEV::LEVFile^ lev,
					  FableMod::STB::Level^ stbLevel,
					  FableMod::TNG::TNGFile^ tng)
{
	Map::Create(controller, name, mapX, mapY, lev, stbLevel);

	m_Tng = tng;
	m_STBLevel = stbLevel;

	for (int i = 0; i < m_Tng->SectionCount; ++i)
	{
		FableMod::TNG::Section^ section = m_Tng->Sections[i];

		for (int j = 0; j < section->Things->Count; ++j)
		{
			FableMod::TNG::Thing^ thing = section->Things[j];

			if (ValidObject(thing))
			{
				GfxThingInterface^ thingInterface = 
					gcnew GfxThingInterface(this, thing);

				if (!thingInterface->Create(controller))
				{
					// Failed.
					delete thingInterface;
/*
					throw gcnew System::Exception(String::Format(
						"Failed to create interface for {0}",
						thing->DefinitionType));
				*/
					continue;
				}

				thingInterface->UpdateInterface();

				thingInterface = nullptr;
			}

			thing = nullptr;
		}

		section = nullptr;
	}

	for (int i = 0; i < m_Tng->SectionCount; ++i)
	{
		FableMod::TNG::Section^ section = m_Tng->Sections[i];

		for (int j = 0; j < section->Things->Count; ++j)
		{
			FableMod::TNG::Thing^ thing = section->Things[j];

			GfxThingInterface^ thingInterface = 
				(GfxThingInterface^)thing->Interface;
		
			if (thingInterface)
			{
				// Complete update.
				thingInterface->Update();
			}

			thing = nullptr;
		}

		section = nullptr;
	}
}

void ThingMap::AddThing(
	GfxThingController^ controller,
	FableMod::TNG::Thing^ thing,
	String^ section,
	float x, float y, float z,
	float nx, float ny, float nz)
{
	if (!TNG->Sections[section])
		TNG->AddSection(gcnew FableMod::TNG::Section(section));
	
	TNG->Sections[section]->AddThing(thing);
	
	if (ValidObject(thing))
	{
		// Setup the initial stuff.

		FableMod::TNG::CTCBlock^ block = nullptr;
	
		if (thing->Name == "AICreature")
			block = thing->CTCs["CTCPhysicsNavigator"];
		else
			block = thing->CTCs["CTCPhysicsStandard"];

		if (block != nullptr)
		{
			try
			{	
				block->Variables["PositionX"]->Value = x-this->X;
				block->Variables["PositionY"]->Value = y-this->Y;
				block->Variables["PositionZ"]->Value = z;
			
				if (fabs(ny) > fabs(nx) && fabs(ny) > fabs(nz))
				{
					// Pointing y. Need a fix.
					block->Variables["RHSetForwardX"]->Value = 1.0f;
					block->Variables["RHSetForwardY"]->Value = 0.0f;
					block->Variables["RHSetForwardZ"]->Value = 0.0f;
				}

				block->Variables["RHSetUpX"]->Value = nx;
				block->Variables["RHSetUpY"]->Value = ny;
				block->Variables["RHSetUpZ"]->Value = nz;
			}
			catch (...)
			{
			}
		}

		if (thing->Name == "AICreature")
		{
			thing->Variables["InitialPosX"]->Value = x;
			thing->Variables["InitialPosY"]->Value = y;
			thing->Variables["InitialPosZ"]->Value = z;
		}

		// Create the interface object...

		GfxThingInterface^ thingInterface = 
			gcnew GfxThingInterface(this, thing);

		if (!thingInterface->Create(controller))
		{
			// Failed.
			delete thingInterface;

			throw gcnew System::Exception(String::Format(
				"Failed to create interface for {0}",
				thing->DefinitionType));
		}

		thingInterface->Update();

		thingInterface = nullptr;
	}
}

void ThingMap::AddThing(
	GfxThingController^ controller,
	FableMod::TNG::Thing^ thing,
	String^ section)
{
	if (!TNG->Sections[section])
		TNG->AddSection(gcnew FableMod::TNG::Section(section));

	TNG->Sections[section]->AddThing(thing);
	
	if (ValidObject(thing))
	{
		// Create the interface object...

		GfxThingInterface^ thingInterface = 
			gcnew GfxThingInterface(this, thing);

		if (!thingInterface->Create(controller))
		{
			// Failed.
			delete thingInterface;

			throw gcnew System::Exception(String::Format(
				"Failed to create interface for {0}",
				thing->DefinitionType));
		}

		thingInterface->Update();

		thingInterface = nullptr;
	}
}

bool ThingMap::Modified::get()
{
	if (Map::Modified)
		return true;

	return m_Tng->Modified;
}

}
}
}