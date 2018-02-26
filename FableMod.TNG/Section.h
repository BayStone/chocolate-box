#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Collections::Generic;
using namespace System::Collections::ObjectModel;

#include "Thing.h"
#include "TNGFile.h"

namespace FableMod
{
namespace TNG
{

public ref class Section
{
public:

	Section(String^ name);

	virtual ~Section();

	void Destroy();

	void Save(TextWriter^ writer);

	void AddThing(Thing^ thing);

	void RemoveThing(Thing^ thing);

	Thing^ FindThing(String^ uid);

	property Collection<Thing^>^ Things
	{
		Collection<Thing^>^ get();
	}
	
	property String^ Name { String^ get() { return m_Name; } }

	property bool Modified
	{
		virtual bool get();
		virtual void set(bool value);
	}

	property FableMod::TNG::TNGFile^ TNGFile
	{
		FableMod::TNG::TNGFile^ get() { return m_File; }
		void set(FableMod::TNG::TNGFile^ value) { m_File = value; }
	}

protected:

	String^ m_Name;
	Collection<Thing^>^ m_Things;
	FableMod::TNG::TNGFile^ m_File;
	bool m_Modified;
};


}
}
