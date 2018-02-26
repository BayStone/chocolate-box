#pragma once
using namespace System;
using namespace System::IO;
using namespace System::Collections::ObjectModel;
using namespace System::Collections::Generic;
using namespace System::Xml;

#include "DefinitionType.h"
#include "membercollection.h"

namespace FableMod
{
namespace ContentManagement
{

public ref class DefinitionDB
{
public:

	DefinitionDB();
	
	virtual ~DefinitionDB();
	
	void Clear();

	void Load(String^ filename);

	DefinitionType^ GetDefinition(String^ name);
	
	property Control^ Controls[UInt32]
	{
		Control^ get(UInt32 index);
	}

	property Control^ Controls[String^]
	{
		Control^ get(String^ name);
	}

	array<DefinitionType^>^ GetDefinitions();

	property String^ Version { String^ get() { return m_Version; } }

	static void EnableDeveloperMode(bool on) { s_DeveloperMode = on; }

	static bool DeveloperModeEnabled() { return s_DeveloperMode; }

private:

	void LoadDefs(XmlDocument^ defdoc);

	void LoadMemberCollection(XmlNode^ firstmem, MemberCollection^ mc); 

	void LoadControlStructures(XmlNode^ root); 
	void LoadDefinitionTypes(XmlNode^ root);
	
	Collection<Control^>^ m_Controls;
	Dictionary<String^, DefinitionType^>^ m_Definitions;
	String^ m_Version;

	static bool s_DeveloperMode = false;
};

}
}