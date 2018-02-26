#include "stdafx.h"
#include <string.h>
#include "Member.h"

using namespace System::Runtime::InteropServices;
using namespace System::Text;

namespace FableMod
{
namespace ContentManagement
{

Member::Member()
{
}

Member::Member(MemberType type, 
			   String^ name, 
			   String^ comments, 
			   FableMod::ContentManagement::Link^ link) :
BaseMember(name, m_Comments),
m_Type(type),
m_Link(link)
{
	if (m_Type == MemberType::STRING)
		m_Value = "";
	else
		m_Value = "0";
}

Member::Member(Member^ member) :
BaseMember(member),
m_Type(member->m_Type),
m_Link(member->m_Link)
{
	if (m_Type == MemberType::STRING)
		m_Value = "";
	else
		Value = "0";
}

Member::~Member()
{
	m_Value = nullptr;
}

void Member::Load(XmlNode^ memberNode)
{
	XmlAttribute^ nameAttr = memberNode->Attributes["name"];
	XmlAttribute^ typeAttr = memberNode->Attributes["type"];
	XmlAttribute^ commentsAttr = memberNode->Attributes["comments"];
	
	m_Type = (MemberType)Enum::Parse(
		MemberType::typeid, typeAttr->InnerText, true);

	m_Name = nameAttr->InnerText;

	if (commentsAttr != nullptr)
		m_Comments = commentsAttr->InnerText;
	else
		m_Comments = "";

	XmlNode^ linkNode = memberNode["Link"];
	
	if (linkNode != nullptr)
	{
		XmlAttribute^ to = linkNode->Attributes["to"];
		XmlAttribute^ restriction = linkNode->Attributes["restriction"];
		
		if (restriction != nullptr)
			m_Link = gcnew FableMod::ContentManagement::Link(
				(LinkDestination)Enum::Parse(
					LinkDestination::typeid, to->InnerText, true), 
				restriction->InnerText);
		else
			m_Link = gcnew FableMod::ContentManagement::Link(
				(LinkDestination)Enum::Parse(
					LinkDestination::typeid, to->InnerText, true), 
				nullptr);
	}
}

int Member::ReadIn(char* data, int length)
{
	if (length < Size)
		throw gcnew Exception("Buffer underrun reading "+Name+".");

	if (m_Type == MemberType::INT)
	{
		m_Value = *(int*)data;
	}
	else if (m_Type == MemberType::UINT)
	{
		m_Value = *(unsigned int*)data;
	}
	else if (m_Type == MemberType::SHORT)
	{
		m_Value = *(short*)data;
	}
	else if (m_Type == MemberType::USHORT)
	{
		m_Value = *(unsigned short*)data;
	}
	else if (m_Type == MemberType::CHAR)
	{
		m_Value = *(char*)data;
	}
	else if (m_Type == MemberType::BYTE)
	{
		m_Value = *(unsigned char*)data;
	}
	else if (m_Type == MemberType::FLOAT)
	{
		m_Value = *(float*)data;
	}
	else if (m_Type == MemberType::DOUBLE)
	{
		m_Value = *(double*)data;
	}
	else if (m_Type == MemberType::STRING)
	{
		m_Value = gcnew String(data);
	}
	return Size;
}

int Member::Write(char* data, int length)
{
	if (length < Size)
		throw gcnew Exception("Buffer underrun writing " + Name + ".");

	if (m_Type == MemberType::INT)
	{
		*(int*)data = (int)m_Value;
	}
	else if (m_Type == MemberType::UINT)
	{
		*(unsigned int*)data = (unsigned int)m_Value;
	}
	else if (m_Type == MemberType::SHORT)
	{
		*(short*)data = (short)m_Value;
	}
	else if (m_Type == MemberType::USHORT)
	{
		*(unsigned short*)data = (unsigned short)m_Value;
	}
	else if (m_Type == MemberType::CHAR)
	{
		*(char*)data = (char)m_Value;
	}
	else if (m_Type == MemberType::BYTE)
	{
		*(unsigned char*)data = (unsigned char)m_Value;
	}
	else if (m_Type == MemberType::FLOAT)
	{
		if (float::IsNaN((float)m_Value))
			*(unsigned int*)data = 0xffffffff;
		else
			*(float*)data = (float)m_Value;
	}
	else if (m_Type == MemberType::DOUBLE)
	{
		*(double*)data = (double)m_Value;
	}
	else if (m_Type == MemberType::STRING)
	{
		IntPtr ps = Marshal::StringToHGlobalAnsi((String^)m_Value);
		strcpy(data, (char*)ps.ToPointer());
		Marshal::FreeHGlobal(ps);
	}
	return Size;
}

int Member::Size::get()
{
	switch(m_Type)
	{
	case MemberType::DOUBLE:
		return 8;
	case MemberType::INT:
	case MemberType::UINT:
	case MemberType::FLOAT:
		return 4;
	case MemberType::SHORT:
	case MemberType::USHORT:
		return 2;
	case MemberType::BYTE:
	case MemberType::CHAR:
		return 1;
	case MemberType::STRING:
		if (m_Value != nullptr)
		{
			String^ val = (String^)m_Value;
			return val->Length + 1;
		}
		else
			return 1;
	}

	return 0;
}

void Member::Value::set(Object^ val)
{
	String^ s = dynamic_cast<String^>(val);
	
	if (m_Type == MemberType::INT)
	{
		if (s != nullptr)
		{
			m_Value = Int32::Parse(s);
		}
		else
		{
			m_Value = (Int32)(val);
		}
	}
	else if (m_Type == MemberType::UINT)
	{
		if (s != nullptr)
		{
			m_Value = UInt32::Parse(s);
		}
		else
		{
			m_Value = (UInt32)(val);
		}
	}
	else if (m_Type == MemberType::SHORT)
	{
		if (s != nullptr)
		{
			m_Value = Int16::Parse(s);
		}
		else
		{
			m_Value = (Int16)(val);
		}
	}
	else if (m_Type == MemberType::USHORT)
	{
		if (s != nullptr)
		{
			m_Value = UInt16::Parse(s);
		}
		else
		{
			m_Value = (UInt16)(val);
		}
	}
	else if (m_Type == MemberType::CHAR)
	{
		if (s != nullptr)
		{
			m_Value = SByte::Parse(s);
		}
		else
		{
			m_Value = (SByte)(val);
		}
	}
	else if (m_Type == MemberType::BYTE)
	{
		if (s != nullptr)
		{
			m_Value = Byte::Parse(s);
		}
		else
		{
			m_Value = (Byte)(val);
		}
	}
	else if (m_Type == MemberType::FLOAT)
	{
		if (s != nullptr)
		{
			m_Value = Single::Parse(s);
		}
		else
		{
			m_Value = (Single)(val);
		}
	}
	else if (m_Type == MemberType::DOUBLE)
	{
		if (s != nullptr)
		{
			m_Value = Double::Parse(s);
		}
		else
		{
			m_Value = (Double)(val);
		}
	}
	else if (m_Type == MemberType::STRING)
	{
		m_Value = s;
	}
}

void Member::FixLinks(
	LinkDestination link, 
	Object^ oldValue,
	Object^ newValue)
{
	if (m_Link != nullptr && 
		m_Link->To == link && 
		m_Value->Equals(oldValue))
	{
		// Update the value.
		Value = newValue;
	}
}

void Member::Print(StringBuilder^ sb, String^ tab) 
{
	sb->AppendFormat("{0}({1}){2}: {3}{4}",
		tab,
		m_Type.ToString()->ToLower(),
		Name,
		m_Value->ToString(),
		Environment::NewLine);
}

String^ Member::ToString()
{
	StringBuilder^ sb = gcnew StringBuilder(64);

	sb->AppendFormat("({0}){1}: {2}",
		m_Type.ToString()->ToLower(),
		Name,
		m_Value->ToString());

	return sb->ToString();
}

}
}