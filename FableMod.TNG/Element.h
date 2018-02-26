#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Xml;

namespace FableMod
{
namespace TNG
{

ref class TNGDefinitions;
ref class Element;

public delegate void ElementChangedHandler(Element^ element);

public ref class Element abstract
{
public:

	Element();

	virtual ~Element();
	
	!Element();

	virtual void Destroy();

	virtual Element^ Duplicate();

	virtual void Load(TNGDefinitions^ definitions, XmlNode^ node);

	virtual void Save(TextWriter^ writer);

	virtual void CopyTo(Element^ element);

	property String^ Name 
	{ 
		String^ get() { return m_Name; } 
		void set(String^ value) { m_Name = value; } 
	}
	
	property String^ Comments { String^ get() { return m_Comments; } }

	property bool Modified
	{
		virtual bool get() { return false; }
		virtual void set(bool value) {}
	}

	virtual void ToDefault() {}

	virtual bool IsDefault() { return false; }

	property bool HasDefault
	{
		virtual bool get() { return false; }
	}

	property bool SaveDefault
	{
		virtual bool get() { return m_SaveDefault; }
	}

	event ElementChangedHandler^ Changed;

protected:

	void OnChanged(Element^ element);

	void HandleInheritance(TNGDefinitions^ definitions, String^ name);

	virtual Element^ Factory();

	String^ m_Name;
	String^ m_Comments;
	bool m_SaveDefault;
};

}
}
