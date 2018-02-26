#include "Stdafx.h"
#include "Control.h"
#include "ArrayMember.h"
#include "DefinitionDB.h"

namespace FableMod
{
namespace ContentManagement
{

Control::Control(UInt32 ID, String^ name) :
m_ID(ID),
m_Name(name),
m_Members(gcnew MemberCollection())
{
}

Control::Control(Control^ control) :
m_ID(control->ID),
m_Name(control->Name),
m_Members(gcnew MemberCollection(control->Members))
{
}

Control::~Control()
{
	m_Name = nullptr;
	m_Members = nullptr;
}

void Control::Print(StringBuilder^ sb, String^ tab)
{
	sb->AppendFormat("{0}Control: {1}{2}",
		tab, ID.ToString("X")+" - "+Name, Environment::NewLine);

	String^ newTab = tab+"  ";

	for (int i = 0; i < Members->Count; ++i)
	{
		sb->AppendFormat("{0} Index {1}{2}", 
			tab, i, Environment::NewLine);
		Members[i]->Print(sb, newTab);
	}
}

int Control::ReadIn(char* data, int length, bool IsXBox)
{
	int offset = 0;
	if (IsXBox)
	{
		unsigned char control = *(unsigned char*)data;
		offset+=1;

		if ((m_ID >> 24) != control)
		{
			throw gcnew Exception(
				"Control byte mismatch.  Byte=" + (m_ID>>24).ToString("X"));
		}
	}
	else
	{
		unsigned __int32 control = *(unsigned __int32*)data;
		offset+=4;
		
		if ((m_ID >> 24 | 
			(m_ID & 0x00ff0000) >> 8 | 
			(m_ID & 0x0000ff00) << 8 | 
			(m_ID & 0x000000ff) << 24)  != control)
		{
			throw gcnew Exception(
				"Control bytes mismatch. ID=" + m_ID.ToString("X"));
		}
	}
	try
	{
		offset+=m_Members->ReadIn(data+offset, length-offset);
	}
	catch (Exception^ exc)
	{
		throw gcnew Exception(
			"Control ID="+ 
			ID.ToString("X")+Environment::NewLine+exc->Message);
	}

	return offset;
}
int Control::Write(char* data, int length, bool IsXBox)
{
	int offset = 0;

	if (IsXBox)
	{
		*(unsigned char*)data = (m_ID >> 24);
		offset+=1;
	}
	else
	{
		*(unsigned __int32*)data = 
			(m_ID >> 24 | 
			(m_ID & 0x00ff0000) >> 8 | 
			(m_ID & 0x0000ff00) << 8 | 
			(m_ID & 0x000000ff) << 24);
		
		offset+=4;
	}
	
	try
	{
		offset+=m_Members->Write(data+offset, length-offset);
	}
	catch (Exception^ exc)
	{
		throw gcnew Exception(
			"Control ID="+
			ID.ToString("X")+Environment::NewLine+exc->Message);
	}
	return offset;
}

String^ Control::ToString()
{
	if (DefinitionDB::DeveloperModeEnabled())
		return ID.ToString("X")+" - "+Name;
	else
		return Name;
}

}
}