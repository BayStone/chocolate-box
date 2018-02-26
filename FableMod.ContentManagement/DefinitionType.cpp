#include "Stdafx.h"
#include "DefinitionType.h"
#include "DefinitionDB.h"
#include "ContentManager.h"

using namespace System::Globalization;
using namespace System::Text;

namespace FableMod
{
namespace ContentManagement
{

CDefLink::CDefLink(Member^ data, Member^ inherited) :
m_Data(data),
m_Inherited(inherited)
{
}

//----------------------------------------------------------------------------

DefinitionType::DefinitionType() :
m_Controls(gcnew Collection<Control^>()),
m_ReadOnlyControls(gcnew ReadOnlyCollection<Control^>(m_Controls))
{
}

DefinitionType::DefinitionType(bool hascdeflisting, String^ name) :
m_Controls(gcnew Collection<Control^>()),
m_ReadOnlyControls(gcnew ReadOnlyCollection<Control^>(m_Controls)),
m_HasCDefListing(hascdeflisting),
m_Name(name)	
{
	CreateCDefs();
}

DefinitionType::DefinitionType(DefinitionType^ deftype) :
m_Controls(gcnew Collection<Control^>()),
m_ReadOnlyControls(gcnew ReadOnlyCollection<Control^>(m_Controls)),
m_HasCDefListing(deftype->HasCDefListing),
m_Name(deftype->Name)
{
	for(int i = 0; i < deftype->Controls->Count; ++i)
	{
		m_Controls->Add(gcnew Control(deftype->Controls[i]));
	}

	CreateCDefs();
}

DefinitionType::~DefinitionType()
{
	m_Controls = nullptr;
	m_ReadOnlyControls = nullptr;
	m_CDefCount = nullptr;
	m_CDefs = nullptr;

	this->!DefinitionType();
}

DefinitionType::!DefinitionType()
{
}

void DefinitionType::CreateCDefs()
{
	if (m_HasCDefListing)
	{
		m_CDefCount = 
			gcnew Member(MemberType::USHORT, "CDefCount", "", nullptr);
		
		m_CDefs = 
			gcnew ArrayMember("CDefs", nullptr, m_CDefCount);
		
		m_CDefs->ElementMembers->Add(
			gcnew Member(MemberType::UINT, "CDefName", "", 
			gcnew Link(LinkDestination::NamesBINEnum, "^C.*Def")));
		
		m_CDefs->ElementMembers->Add(
			gcnew Member(MemberType::INT, "CDefDataEntry", "", 
			gcnew Link(LinkDestination::GameBINEntryID, "^C.*Def")));
		
		m_CDefs->ElementMembers->Add(
			gcnew Member(MemberType::INT, "InheritedEntryID", "", 
			gcnew Link(LinkDestination::GameBINEntryID, "^"+m_Name)));
	}
}

CDefLink^ DefinitionType::CDefLinks::get(String^ name)
{
	if (!m_CDefs)
		return nullptr;

	for (int i = 0; i < m_CDefs->Elements->Count; ++i)
	{
		MemberCollection^ c = m_CDefs->Elements[i];
	
		if (c->Count == 3)
		{
			Member^ member = (Member^)c[0];

			ContentObject^ o = ContentManager::Instance->FindEntry(
				member->Link->To, member->Value);

			if (o != nullptr && o->Name == name)
			{
				return gcnew CDefLink((Member^)c[1], (Member^)c[2]);
			}
		}
	}

	return nullptr;
}

void DefinitionType::Load(DefinitionDB^ database, XmlNode^ defNode)
{
	XmlAttribute^ name = defNode->Attributes["name"];
				
	XmlAttribute^ hascdef = defNode->Attributes["hasCDefListing"];
	
	m_HasCDefListing = bool::Parse(hascdef->InnerText);
	
	m_Name = name->InnerText;
	
	XmlNode^ cont = defNode->FirstChild;
	
	while (cont)
	{
		if (cont->Name == "ControlInstance")
		{
			XmlAttribute^ id = cont->Attributes["id"];
			XmlAttribute^ name = cont->Attributes["name"];

			Control^ control = nullptr;
			
			if (id != nullptr)
			{
				control = database->Controls[UInt32::Parse(
					id->InnerText, 
					NumberStyles::AllowHexSpecifier)];
			}
			else 
				control = database->Controls[name->InnerText];
			
			if (!control)
			{
				String^ contname;
			
				if (id != nullptr)
					contname = "ID = "+id->InnerText;
				else 
					contname = "Name = "+name->InnerText;

				throw gcnew Exception(
					"Error reading DefinitionType["+
					Name+
					"].  ControlInstance referenced non-existing Control structure.  "+
					contname);
			}

			AddControl(control);
		}

		cont = cont->NextSibling;
	}
}

String^ DefinitionType::Print(BINEntry^ entry)
{
	ReadIn(entry->GetData(), entry->Length, entry->IsXBox);

	StringBuilder^ sb = gcnew StringBuilder();

	sb->AppendFormat("DefinitionType: {0} ({1}){2}", 
		entry->Name, entry->Definition, Environment::NewLine);

	for (int i = 0; i < Controls->Count; ++i)
	{
		Controls[i]->Print(sb, "  ");
	}

	return sb->ToString();
}

void DefinitionType::AddControl(Control^ control)
{
	m_Controls->Add(control);
}

Control^ DefinitionType::FindControl(UInt32 id)
{
	for (int i = 0; i < m_Controls->Count; ++i)
	{
		if (m_Controls[i]->ID == id)
			return m_Controls[i];
	}

	return nullptr;
}

Control^ DefinitionType::FindControl(String^ name)
{
	for (int i = 0; i < m_Controls->Count; ++i)
	{
		if (m_Controls[i]->Name == name)
			return m_Controls[i];
	}

	return nullptr;
}

void DefinitionType::FixLinks(LinkDestination link, 
							  Object^ oldValue,
							  Object^ newValue)
{
	if (m_CDefs)
		m_CDefs->FixLinks(link, oldValue, newValue);

	for (int i = 0; i < m_Controls->Count; ++i)
	{
		for (int j = 0; j < m_Controls[i]->Members->Count; ++j)
		{
			m_Controls[i]->Members[j]->FixLinks(link, oldValue, newValue);
		}
	}
}

void DefinitionType::ReadIn(BINEntry^ entry)
{
	ReadIn(entry->GetData(), entry->Length, entry->IsXBox);
}

int DefinitionType::ReadIn(char* data, int length, bool IsXBox)
{
	int offset = 0;

	Bool1 = *(data+offset) != 0;
	offset+=1;
	Bool2 = *(data+offset) != 0;
	offset+=1;
	Bool3 = *(data+offset) != 0;
	offset+=1;

	if (m_HasCDefListing)
	{
		m_CDefs->Elements->Clear();
		offset += m_CDefCount->ReadIn(data+offset,length-offset);
		offset += m_CDefs->ReadIn(data+offset, length-offset);
	}

	for (int i = 0; i < m_Controls->Count; ++i)
	{
		offset+=m_Controls[i]->ReadIn(data+offset, length-offset, IsXBox);
	}

	return offset;
}

void DefinitionType::Write(BINEntry^ entry)
{
	char* data = new char[1048576];
	int len = Write(data, 1048576, entry->IsXBox);
	entry->SetData(data, (unsigned int)len);
	delete[] data;
}

int DefinitionType::Write(char* data, int length, bool IsXBox)
{
	int offset = 0;

	if (Bool1)
		*(data+offset) =1;
	else
		*(data+offset) =0;
	offset+=1;

	if (Bool2)
		*(data+offset) =1;
	else
		*(data+offset) =0;
	offset+=1;

	if (Bool3)
		*(data+offset) =1;
	else
		*(data+offset) =0;
	offset+=1;

	if (m_HasCDefListing)
	{
		m_CDefs->UpdateCount();
		offset += m_CDefCount->Write(data+offset,length-offset);
		offset += m_CDefs->Write(data+offset, length-offset);
	}

	for (int i = 0; i < m_Controls->Count; ++i)
	{
		offset+=m_Controls[i]->Write(data+offset, length-offset, IsXBox);
	}

	return offset;
}

}
}