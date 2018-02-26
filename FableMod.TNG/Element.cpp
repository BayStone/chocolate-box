#include "Stdafx.h"
#include "Element.h"
#include "TNGDefinitions.h"

using namespace System::Globalization;

namespace FableMod
{
namespace TNG
{

Element::Element() :
m_SaveDefault(true)
{
}

Element::~Element()
{
	m_Name = nullptr;
	m_Comments = nullptr;
	this->!Element();
}

Element::!Element()
{
	Destroy();
}

void Element::Destroy()
{
}

void Element::OnChanged(Element^ element)
{
	Changed(element);
}

void Element::Save(TextWriter^ writer)
{
}

Element^ Element::Duplicate()
{
	Element^ element = Factory();

	CopyTo(element);

	return element;
}

Element^ Element::Factory()
{
	throw gcnew Exception("FableMod::TNG: Element factory called");
	return nullptr;
}

void Element::CopyTo(Element^ element)
{
	element->m_Name = m_Name;
	element->m_Comments = m_Comments;
	element->m_SaveDefault = m_SaveDefault;
}

void Element::Load(TNGDefinitions^ definitions, XmlNode^ node)
{
	XmlAttribute^ inheritsAttr = node->Attributes["inherits"];

	if (inheritsAttr != nullptr)
		HandleInheritance(definitions, inheritsAttr->InnerText);
	
	m_Name = node->Attributes["name"]->InnerText;
	
	XmlAttribute^ commentsAttr = node->Attributes["comments"];

	if (commentsAttr != nullptr)
		m_Comments = commentsAttr->InnerText;

	XmlAttribute^ saveDefaultAttr = node->Attributes["savedefault"];

	if (saveDefaultAttr != nullptr)
		m_SaveDefault = bool::Parse(saveDefaultAttr->InnerText);
}

void Element::HandleInheritance(TNGDefinitions^ definitions, String^ name)
{
	Element^ element = (Element^)definitions->Find(name);

	if (element != nullptr)
	{
		element->CopyTo(this);
	}
}

}
}