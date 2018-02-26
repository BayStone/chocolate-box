#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Collections::ObjectModel;

#include "Member.h"
#include "ArrayMember.h"

namespace FableMod
{
namespace ContentManagement
{

public ref class MemberCollection
{
public:

	MemberCollection();
	MemberCollection(MemberCollection^ source);
	
	virtual ~MemberCollection();

	void Add(BaseMember^ member);
	
	int ReadIn(char* data, int length);
	int Write(char* data, int length);

	property int Count
	{
		int get() { return m_Members->Count; }
	}
	
	property BaseMember^ default[int]
	{
		BaseMember^ get(int index) 
		{ 
			System::Diagnostics::Debug::Assert(index < m_Members->Count);		
			return m_Members[index]; 
		}
	}
	
	property BaseMember^ default[String^]
	{
		BaseMember^ get(String^ name);
	}

	Member^ GetMemberByName(String^ name);
	
private:
	Collection<BaseMember^>^ m_Members;
};

}
}