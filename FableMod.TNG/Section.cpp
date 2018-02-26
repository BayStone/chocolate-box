#include "Stdafx.h"
#include "Section.h"

namespace FableMod
{
namespace TNG
{

Section::Section(String^ name) :
m_Name(name),
m_Modified(false),
m_Things(gcnew Collection<Thing^>())
{
}

Section::~Section()
{
	Destroy();

	m_Things = nullptr;
	m_Name = nullptr;
}

void Section::Destroy()
{
	if (m_Things)
	{
		for (int i = 0; i < m_Things->Count; ++i)
		{
			m_Things[i]->Section = nullptr;
			delete m_Things[i];
		}

		m_Things->Clear();
	}
}	

void Section::Save(TextWriter^ writer)
{
	writer->WriteLine("XXXSectionStart {0};", Name);
	writer->WriteLine("");

	for (int i = 0; i < m_Things->Count; ++i)
	{
		m_Things[i]->Save(writer);
	}

	writer->WriteLine("XXXSectionEnd;");
	writer->WriteLine("");
}

void Section::AddThing(Thing^ thing)
{
	m_Things->Add(thing);
	thing->Section = this;
	m_Modified = true;
}

void Section::RemoveThing(Thing^ thing)
{
	thing->Section = nullptr;
	
	int index = m_Things->IndexOf(thing);
	
	if (index >= 0)
	{
		Console::WriteLine("Section({0})::RemoveThing({1})",
			Name, thing->DefinitionType);

		m_Things->RemoveAt(index);

		m_Modified = true;
	}
}

Thing^ Section::FindThing(String^ uid)
{
	Thing^ thing = nullptr;

	for (int i = 0; i < m_Things->Count; ++i)
	{
		if (m_Things[i]->UID == uid)
			return m_Things[i];
	}

	return nullptr;
}

Collection<Thing^>^ Section::Things::get()
{
	return m_Things;
}

bool Section::Modified::get()
{	
	if (m_Modified)
		return true;

	for (int i = 0; i < m_Things->Count; ++i)
	{
		if (m_Things[i]->Modified)
			return true;
	}

	return false;
}

void Section::Modified::set(bool value)
{
	m_Modified = value;

	for (int i = 0; i < m_Things->Count; ++i)
	{
		m_Things[i]->Modified = value;
	}
}

}
}