#include "Stdafx.h"
#include "TNGFile.h"

namespace FableMod
{
namespace TNG
{

Block::Block() :
m_Elements(gcnew Collection<Element^>())
{
}

Block::~Block()
{
	m_Elements = nullptr;
}

void Block::HandleChange()
{
}

void Block::ChildChanged(Element^ element)
{
	// Call a handler function.
	HandleChange();

	// Send to my listeners.
	OnChanged(this);
}

void Block::Load(TNGDefinitions^ definitions, XmlNode^ node)
{
	Element::Load(definitions, node);

	XmlNode^ child = node->FirstChild;

	while (child != nullptr)
	{
		LoadElement(definitions, child);
		child = child->NextSibling;
	}
}

bool Block::LoadElement(TNGDefinitions^ definitions, XmlNode^ node)
{
	return false;
}

void Block::CopyTo(Element^ element)
{
	Element::CopyTo(element);

	Block^ block = (Block^)element;

	for each (Element^ e in m_Elements)
	{
		block->Add(e->Duplicate());
	}
}

void Block::Clear()
{
	if (m_Elements)
		m_Elements->Clear();
}

Element^ Block::Find(String^ name)
{
	for (int i = 0; i < m_Elements->Count; ++i)
	{
		if (m_Elements[i]->Name == name)
			return m_Elements[i];
	}
	
	return nullptr;
}

void Block::Add(Element^ element)
{
	m_Elements->Add(element);

	element->Changed += gcnew ElementChangedHandler(
		this, &Block::ChildChanged);
}

bool Block::Modified::get()
{
	for (int i = 0; i < m_Elements->Count; ++i)
	{
		if (m_Elements[i]->Modified)
			return true;
	}
	
	return false;
}

void Block::Modified::set(bool value)
{
	for (int i = 0; i < m_Elements->Count; ++i)
	{
		m_Elements[i]->Modified = value;
	}
}

bool Block::IsDefault()
{
	// All or nothing.

	for (int i = 0; i < m_Elements->Count; ++i)
	{
		if (m_Elements[i]->HasDefault)
		{
			if (!m_Elements[i]->IsDefault())
				return false;
		}
	}

	return true;
}

void Block::ToDefault()
{
	for (int i = 0; i < m_Elements->Count; ++i)
	{
		m_Elements[i]->ToDefault();
	}
}

bool Block::HasDefault::get()
{
	for (int i = 0; i < m_Elements->Count; ++i)
	{
		if (m_Elements[i]->HasDefault)
			return true;
	}

	return false;
}

bool Block::SaveDefault::get()
{
	if (!m_SaveDefault)
	{
		// Block controls children.
		return false;
	}

	for (int i = 0; i < m_Elements->Count; ++i)
	{
		if (m_Elements[i]->SaveDefault)
			return true;
	}

	return true;
}

}
}