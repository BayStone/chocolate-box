#include "Stdafx.h"
#include "MemberCollection.h"

namespace FableMod
{
namespace ContentManagement
{

MemberCollection::MemberCollection() :
m_Members(gcnew Collection<BaseMember^>())
{
}

MemberCollection::MemberCollection(MemberCollection^ mc) :
m_Members(gcnew Collection<BaseMember^>())
{
	for (int i = 0; i < mc->Count; ++i)
	{
		if (mc[i]->GetType() == Member::typeid)
		{
			m_Members->Add(gcnew Member((Member^)mc[i]));
		}
		else if (mc[i]->GetType() == ArrayMember::typeid)
		{
			m_Members->Add(gcnew ArrayMember((ArrayMember^)mc[i], this));
		}
	}
}

MemberCollection::~MemberCollection()
{
	m_Members->Clear();
	m_Members = nullptr;
}

Member^ MemberCollection::GetMemberByName(String^ name)
{
	for (int i = 0; i < m_Members->Count; ++i)
	{
		if (m_Members[i]->GetType() == Member::typeid)
		{
			if (((Member^)m_Members[i])->Name == name)
				return (Member^)m_Members[i];
		}
	}

	return nullptr;
}

void MemberCollection::Add(BaseMember^ member)
{
	m_Members->Add(member);
}

int MemberCollection::ReadIn(char* data, int length)
{
	int offset = 0;

	for (int i = 0; i < m_Members->Count; ++i)
	{
		offset += m_Members[i]->ReadIn(data+offset, length-offset);
	}

	return offset;
}

int MemberCollection::Write(char* data, int length)
{
	int offset = 0;

	for (int i = 0; i < m_Members->Count; ++i)
	{
		if (m_Members[i]->GetType() == ArrayMember::typeid)
			((ArrayMember^)m_Members[i])->UpdateCount();
	}

	for (int i = 0; i < m_Members->Count; ++i)
	{
		offset += m_Members[i]->Write(data+offset, length-offset);
	}

	return offset;
}

BaseMember^ MemberCollection::default::get(String^ name)
{
	for (int i = 0; i < m_Members->Count; ++i)
	{
		if (m_Members[i]->Name == name)
			return m_Members[i];
	}
	
	return nullptr;
}

}
}