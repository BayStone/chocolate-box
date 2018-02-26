#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Collections::Generic;
using namespace System::Collections::ObjectModel;

#include "Element.h"

namespace FableMod
{
namespace TNG
{

public ref class Block abstract : public Element
{
public:

	Block();

	virtual ~Block();

	virtual void Load(
		TNGDefinitions^ definitions, XmlNode^ node) override;

	virtual Element^ Find(String^ name);

	virtual void CopyTo(Element^ element) override;

	void Add(Element^ type);
	
	void Clear();
	
	property bool Modified
	{
		virtual bool get() override;
		virtual void set(bool value) override;
	}

	virtual void ToDefault() override;

	virtual bool IsDefault() override;

	property bool HasDefault
	{
		virtual bool get() override;
	}

	property bool SaveDefault
	{
		virtual bool get() override;
	}
	
	property int ElementCount
	{
		int get() { return m_Elements->Count; }
	}

	property Element^ Elements[int]
	{
		Element^ get(int index) { return m_Elements[index]; }
	}

protected:

	void ChildChanged(Element^ element);

	virtual void HandleChange();

	virtual bool LoadElement(TNGDefinitions^ definitions, XmlNode^ node);

	Collection<Element^>^ m_Elements;
};

}
}
