#pragma once

using namespace System;
using namespace System::Text;

#include "Link.h"

namespace FableMod
{
namespace ContentManagement
{

public ref class BaseMember abstract 
{
public:

	BaseMember()
	{
	}

	BaseMember(
		String^ name, 
		String^ comments) :
	m_Name(name),
	m_Comments(comments)
	{
	}

	BaseMember(BaseMember^ member) :
	m_Name(member->Name),
	m_Comments(member->Comments)
	{
	}

	virtual void FixLinks(
		LinkDestination link, 
		Object^ oldValue,
		Object^ newValue) = 0;

	virtual void Print(StringBuilder^ sb, String^ tab) = 0;

	virtual int ReadIn(char* data, int length) = 0;
	
	int ReadIn(array<Byte>^ data,int offset)
	{
		pin_ptr<System::Byte> ptr = &(data[offset]);
		return ReadIn((char*)ptr, data->Length-offset);
	}

	virtual int Write(char* data, int length) = 0;
	
	virtual ~BaseMember() 
	{
		m_Name = nullptr;
		m_Comments = nullptr;
	}

	property String^ Name
	{
		String^ get() { return m_Name; }
	}
	
	property String^ Comments
	{
		String^ get() { return m_Comments; }
	}

protected:

	String^	m_Name;
	String^	m_Comments;
};

}
}