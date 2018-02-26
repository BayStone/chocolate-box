#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Collections::ObjectModel;

using namespace FableMod::BIN;

#include "Control.h"

namespace FableMod
{
namespace ContentManagement
{

ref class DefinitionDB;

public ref class CDefLink
{
public:

	property Object^ DataID
	{
		Object^ get() { return m_Data->Value; }
		void set(Object^ value) { m_Data->Value = value; }
	}

	property Object^ InheritedID
	{
		Object^ get() { return m_Inherited->Value; }
		void set(Object^ value) { m_Inherited->Value = value; }
	}

internal:

	CDefLink(Member^ data, Member^ inherited);

private:

	Member^ m_Data;
	Member^ m_Inherited;
};

public ref class DefinitionType : public IDisposable
{
public:
	
	DefinitionType();
	DefinitionType(DefinitionType^ deftype);
	DefinitionType(bool hascdeflisting, String^ name);
	
	virtual ~DefinitionType();

	!DefinitionType();

	void Load(DefinitionDB^ database, XmlNode^ defNode);

	void AddControl(Control^ control);

	void FixLinks(
		LinkDestination link, 
		Object^ oldValue,
		Object^ newValue);

	int ReadIn(char* data, int length, bool IsXBox);
	void ReadIn(BINEntry^ entry);

	int Write(char* data, int length, bool IsXBox);
	void Write(BINEntry^ entry);

	String^ Print(BINEntry^ entry);
	
	Control^ FindControl(UInt32 id);

	Control^ FindControl(String^ name);

	property bool HasCDefListing
	{
		bool get() { return m_HasCDefListing; }
	}

	property String^ Name
	{
		String^ get() { return m_Name; }
	}
	
	property ReadOnlyCollection<Control^>^ Controls
	{
		ReadOnlyCollection<Control^>^ get() { return m_ReadOnlyControls; }
	}

	property ArrayMember^ CDefs
	{
		ArrayMember^ get() { return m_CDefs; }
	}
	
	property CDefLink^ CDefLinks[String^]
	{
		CDefLink^ get(String^ name);
	}

	property bool Bool1;
	property bool Bool2;
	property bool Bool3;

private:

	void CreateCDefs();

	Member^	m_CDefCount;
	ArrayMember^ m_CDefs;
	bool m_HasCDefListing;
	String^	m_Name;
	Collection<Control^>^ m_Controls;
	ReadOnlyCollection<Control^>^ m_ReadOnlyControls;
};

}
}
