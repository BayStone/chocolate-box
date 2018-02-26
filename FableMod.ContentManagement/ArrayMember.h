#pragma once

#include "BaseMember.h"

using namespace System;
using namespace System::IO;
using namespace System::Collections::ObjectModel;

namespace FableMod
{
namespace ContentManagement
{

ref class Member;
ref class MemberCollection;

public ref class ArrayMember : public BaseMember
{
public:

	ArrayMember(String^ name, String^ comments, Member^ count);
	ArrayMember(ArrayMember^ arr, MemberCollection^ cont);
	
	virtual ~ArrayMember();

	void UpdateCount();
	
	virtual void FixLinks(
		LinkDestination link, 
		Object^ oldValue,
		Object^ newValue) override;

	virtual void Print(StringBuilder^ sb, String^ tab) override; 

	virtual int ReadIn(char* data, int length) override;
	virtual int Write(char* data, int length) override;

	void Sort(int sortElement);

	property String^ ElementCount
	{
		String^ get() { return m_CountMember->Name; }
	}
	
	/// The collection of members for one array "cell".
	property MemberCollection^ ElementMembers
	{
		MemberCollection^ get() { return m_ElementMembers; }
	}
	
	property Collection<MemberCollection^>^ Elements
	{
		Collection<MemberCollection^>^ get() { return m_Elements; }
	}
	
	MemberCollection^ CreateElement();
	
	virtual String^ ToString() override;

private:
	
	String^	m_Comments;
	Member^	m_CountMember;
	MemberCollection^ m_ElementMembers;
	Collection<MemberCollection^>^	m_Elements;
};

}
}