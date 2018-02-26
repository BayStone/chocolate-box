#include "stdafx.h"
#include "DefinitionDB.h"
#include "ArrayMember.h"

using namespace System::IO;
using namespace System::Globalization;

namespace FableMod
{
namespace ContentManagement
{

DefinitionDB::DefinitionDB() :
m_Controls(gcnew Collection<Control^>()),
m_Definitions(gcnew Dictionary<String^, DefinitionType^>())
{
	//m_ReadOnlyControls = gcnew ReadOnlyCollection<Control^>(m_Controls);
	//m_ControlDictionary = gcnew Dictionary<UInt32, Control^>();
}

void DefinitionDB::Load(String^ filename)
{	
	XmlDocument^ defdoc = gcnew XmlDocument();

	defdoc->Load(filename);
	
	LoadDefs(defdoc);
}

DefinitionDB::~DefinitionDB()
{
	Clear();
}

void DefinitionDB::Clear()
{
	m_Controls->Clear();
	m_Definitions->Clear();
}

void DefinitionDB::LoadControlStructures(XmlNode^ root)
{
	XmlNode^ controls = root["ControlStructures"];

	if (controls)
	{
		XmlNode^ cont = controls->FirstChild;
	
		while (cont)
		{
			if (cont->Name == "Control")
			{
				XmlAttribute^ name = cont->Attributes["name"];
				XmlAttribute^ id = cont->Attributes["id"];

				Control^ control = gcnew Control(
					UInt32::Parse(
						id->InnerText, 
						NumberStyles::AllowHexSpecifier), 
						name->InnerText);
				
				XmlNode^ mem = cont->FirstChild;
				
				LoadMemberCollection(mem, control->Members);
				
				m_Controls->Add(control);
			}

			cont = cont->NextSibling;
		}
	}		
}

void DefinitionDB::LoadDefinitionTypes(XmlNode^ root)
{
	XmlNode^ defs = root["DefinitionTypes"];

	if (defs != nullptr)
	{
		XmlNode^ def = defs->FirstChild;
	
		while (def != nullptr)
		{
			if (def->Name == "DefinitionType")
			{
				DefinitionType^ defType = gcnew DefinitionType();

				defType->Load(this, def);

				m_Definitions[defType->Name] = defType;
			}

			def = def->NextSibling;
		}
	}
}

void DefinitionDB::LoadDefs(XmlDocument^ defdoc)
{
	XmlNode^ root = defdoc["DefinitionListing"];
	
	XmlAttribute^ versionAttr = root->Attributes["version"];

	if (versionAttr != nullptr)
		m_Version = versionAttr->InnerText;
	else
		m_Version = gcnew String("UNKNOWN VERSION");

	if (root != nullptr)
	{
		LoadControlStructures(root);	
		LoadDefinitionTypes(root);	
	}
}

void DefinitionDB::LoadMemberCollection(XmlNode^ firstmem, 
										MemberCollection^ mc)
{
	XmlNode^ mem = firstmem;

	while (mem != nullptr)
	{
		if (mem->Name == "Member")
		{
			Member^ member = gcnew Member();

			member->Load(mem);

			mc->Add(member);
		}
		else if (mem->Name == "Array")
		{
			XmlAttribute^ arrname = mem->Attributes["name"];
			XmlAttribute^ arrcomments = mem->Attributes["comments"];
			XmlAttribute^ arrcount = mem->Attributes["elementcount"];
			
			String^ comments = nullptr;
			
			if (arrcomments)
				comments = arrcomments->InnerText;

			Member^ count = mc->GetMemberByName(arrcount->InnerText);
			
			ArrayMember^ arr = gcnew ArrayMember(
				arrname->InnerText, comments, count);
			
			LoadMemberCollection(mem->FirstChild, arr->ElementMembers);
			
			mc->Add(arr);
		}

		mem = mem->NextSibling;
	}
}

DefinitionType^ DefinitionDB::GetDefinition(String^ name)
{
	DefinitionType^ defType = nullptr;

	if (m_Definitions->TryGetValue(name, defType))
	{
		return gcnew DefinitionType(defType);
	}

	return nullptr;
}

array<DefinitionType^>^ DefinitionDB::GetDefinitions()
{
	array<DefinitionType^>^ defs = 
		gcnew array<DefinitionType^>(m_Definitions->Count);

	int iIndex = 0;

	for each (KeyValuePair<String^, DefinitionType^> kvp in m_Definitions)
		defs[iIndex++] = kvp.Value;
	
	return defs;
}

Control^ DefinitionDB::Controls::get(String^ name)
{
	for (int i = 0; i < m_Controls->Count; ++i)
	{
		if (m_Controls[i]->Name == name)
			return gcnew Control(m_Controls[i]);
	}
	
	return nullptr;
}

Control^ DefinitionDB::Controls::get(UInt32 id)
{
	for (int i = 0; i < m_Controls->Count; ++i)
	{
		if (m_Controls[i]->ID == id)
			return gcnew Control(m_Controls[i]);
	}
	
	return nullptr;
}

}
}