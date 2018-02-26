#pragma once

#include "GfxController.h"

using namespace System;
using namespace System::IO;
using namespace System::Collections::Generic;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

public ref class GfxThingController : public GfxController
{
public:

	GfxThingController();

	virtual ~GfxThingController();

	virtual void Destroy() override;

	virtual void ResetObjects() override;

	void ResetThings();

	void AddThing(unsigned int id, FableMod::TNG::Thing^ thing);

	bool RemoveThing(unsigned int id);

	bool RemoveThing(FableMod::TNG::Thing^ thing);

	FableMod::TNG::Thing^ FindThing(unsigned int id);

	FableMod::TNG::Thing^ FindThingUID(String^ uid);

	void ToGroundEditMode(bool disable);

	void FreezeAll(bool freeze);

	void ShowOfType(String^ objectType, bool show);

private:

	void OwnerRemove(FableMod::TNG::Thing^ ownerThing);
	void ThingRemove(unsigned int id, FableMod::TNG::Thing^ thing);

	Dictionary<unsigned int, FableMod::TNG::Thing^>^ m_Things;
};

}
}
}