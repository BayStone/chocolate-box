#pragma once

#include "BaseMember.h"
#include "Link.h"

using namespace System;
using namespace System::IO;
using namespace System::Xml;

namespace FableMod
{
namespace ContentManagement
{

public enum class MemberType
{
	INT,
	UINT,
	SHORT,
	USHORT,
	CHAR,
	BYTE,
	STRING,
	FLOAT,
	DOUBLE
};

public ref class Member : public BaseMember
{
public:

	Member();

	Member(
		MemberType type, 
		String^ name, 
		String^ comments, 
		FableMod::ContentManagement::Link^ link);
	
	Member(Member^ member);

	virtual ~Member();

	void Load(XmlNode^ memberNode);

	virtual void FixLinks(
		LinkDestination link, 
		Object^ oldValue,
		Object^ newValue) override;

	virtual void Print(StringBuilder^ sb, String^ tab) override;

	virtual int ReadIn(char* data, int length) override;
	virtual int Write(char* data, int length) override;

	property int Size
	{
		int get();
	}

	property Object^ Value
	{
		Object^ get() { return m_Value; }
		void set(Object^ val);
	}
	
	property MemberType Type
	{
		MemberType get() { return m_Type; }
	}

	property FableMod::ContentManagement::Link^ Link
	{
		FableMod::ContentManagement::Link^ get() { return m_Link; }
	}
	
	virtual String^ ToString() override;

private:

	MemberType m_Type;
	FableMod::ContentManagement::Link^ m_Link;
	Object^	m_Value;
};

}
}