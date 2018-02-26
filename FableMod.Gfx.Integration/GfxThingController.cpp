#include "Stdafx.h"
#include "GfxThingController.h"
#include "GfxThingInterface.h"

using namespace System::Runtime::InteropServices;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

GfxThingController::GfxThingController() :
m_Things(gcnew Dictionary<unsigned int, FableMod::TNG::Thing^>())
{
}

GfxThingController::~GfxThingController()
{
	Destroy();
}

void GfxThingController::Destroy()
{
	GfxController::Destroy();
	ResetThings();
}

void GfxThingController::ResetThings()
{	
	for each (KeyValuePair<
			unsigned int, FableMod::TNG::Thing^> kvp in m_Things)
	{
		kvp.Value->~Thing();
	}

	m_Things->Clear();
}

void GfxThingController::ResetObjects()
{
	ResetThings();
	GfxController::ResetObjects();
}

/// Add a view to the controller.
void GfxThingController::AddThing(unsigned int id,
								  FableMod::TNG::Thing^ thing)
{
	if (!m_Things->ContainsKey(id))
		m_Things[id] = thing;

	GetRoot()->AttachChild(
		((GfxThingInterface^)thing->Interface)->GetNode());
}

void GfxThingController::OwnerRemove(FableMod::TNG::Thing^ ownerThing)
{
	String^ ownerUID = ownerThing->UID;

	for each (KeyValuePair<
			unsigned int, FableMod::TNG::Thing^> kvp in m_Things)
	{
		FableMod::TNG::Thing^ thing = kvp.Value;

		FableMod::TNG::CTCBlock^ block = thing->CTCs["CTCOwnedEntity"];

		if (block)
		{
			String^ uid = block->Variables["OwnerUID"]->StringValue;

			if (uid == ownerUID)
				block->Variables["OwnerUID"]->Value = "0";
		}
	}
}

void GfxThingController::ThingRemove(unsigned int id,
									 FableMod::TNG::Thing^ thing)
{
	((GfxThingInterface^)thing->Interface)->Highlight = false;

	OwnerRemove(thing);

	m_Things->Remove(id);

	thing->Detach();

	thing->Destroy();
}

bool GfxThingController::RemoveThing(unsigned int id)
{
	FableMod::TNG::Thing^ thing = nullptr;

	if (m_Things->TryGetValue(id, thing))
	{
		ThingRemove(id, thing);
		return true;
	}

	return false;
}

bool GfxThingController::RemoveThing(FableMod::TNG::Thing^ thing)
{
	for each (KeyValuePair<
		unsigned int, FableMod::TNG::Thing^> kvp in m_Things)
	{
		if (kvp.Value == thing)
		{
			ThingRemove(kvp.Key, thing);
			return true;
		}
	}

	return false;
}

FableMod::TNG::Thing^ GfxThingController::FindThing(unsigned int id)
{
	FableMod::TNG::Thing^ thing = nullptr;

	if (m_Things->TryGetValue(id, thing))
		return thing;

	return nullptr;
}

FableMod::TNG::Thing^ GfxThingController::FindThingUID(String^ uid)
{
	for each (KeyValuePair<
		unsigned int, FableMod::TNG::Thing^> kvp in m_Things)
	{
		if (kvp.Value->UID == uid)
			return kvp.Value;
	}

	return nullptr;
}

void GfxThingController::ToGroundEditMode(bool ground)
{
	for each (KeyValuePair<
		unsigned int, FableMod::TNG::Thing^> kvp in m_Things)
	{
		FableMod::TNG::Thing^ thing = kvp.Value;
		
		if (thing->Name == "Marker" || 
			thing->Name == "AICreature" ||
			thing->Name == "Holy Site" ||
			thing->Name == "Thing")
		{
			// We don't need these.
			((GfxThingInterface^)thing->Interface)->Show(!ground);
		}
		else
		{
			((GfxThingInterface^)thing->Interface)->GroundMode(ground);
		}
	}
}

void GfxThingController::FreezeAll(bool freeze)
{
	for each (KeyValuePair<
		unsigned int, FableMod::TNG::Thing^> kvp in m_Things)
	{
		FableMod::TNG::Thing^ thing = kvp.Value;

		((GfxThingInterface^)thing->Interface)->Freezed = freeze;
	}
}

void GfxThingController::ShowOfType(String^ objectType, bool show)
{
	for each (KeyValuePair<
		unsigned int, FableMod::TNG::Thing^> kvp in m_Things)
	{
		FableMod::TNG::Thing^ thing = kvp.Value;

		if (thing->Name == objectType)
		{
			((GfxThingInterface^)thing->Interface)->Show(show);
		}
	}
}

}
}
}