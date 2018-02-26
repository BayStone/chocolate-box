#pragma once

using namespace System;
using namespace System::IO;

namespace FableMod
{
namespace TNG
{

ref class Thing;

/// <summary>
/// Interface of a thing... graphics, etc.
/// </summary>
public ref class ThingInterface abstract : IDisposable
{
public:

	ThingInterface(Thing^ thing) : m_Thing(thing) {}

	virtual ~ThingInterface() {}

	!ThingInterface() { Destroy(); }

	virtual void Destroy() {}

	virtual void Update() = 0;

	virtual void UpdateThing() = 0;

	property FableMod::TNG::Thing^ Thing
	{ 
		FableMod::TNG::Thing^ get() { return m_Thing; } 
	}

protected:

	FableMod::TNG::Thing^ m_Thing;
};

}
}
