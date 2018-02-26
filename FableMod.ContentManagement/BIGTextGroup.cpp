#include "stdafx.h"
#include "BIGTextGroup.h"
#include <memory.h>

namespace FableMod
{
namespace ContentManagement
{
	
BIGTextGroup::BIGTextGroup()
{
	m_Entries = 0;
	m_EntryCount = 0;
}

BIGTextGroup::BIGTextGroup(AssetEntry^ ent)
{
	unsigned __int32 count;
	memcpy(&count, ent->GetData(), sizeof(count));
	m_EntryCount = count;
	m_Entries = new unsigned __int32[m_EntryCount];
	memcpy(m_Entries, ent->GetData()+sizeof(count), m_EntryCount*sizeof(*m_Entries));
}

BIGTextGroup::~BIGTextGroup()
{
	if (m_Entries)
		delete[] m_Entries;
}

array<UInt32>^ BIGTextGroup::Entries::get()
{
	array<UInt32>^ ret = gcnew array<UInt32>(m_EntryCount);

	for (int i = 0; i < m_EntryCount; ++i)
	{
		ret[i] = m_Entries[i];
	}
	
	return ret;
}

void BIGTextGroup::Entries::set(array<UInt32>^ entries)
{
	delete[] m_Entries;
	
	m_EntryCount = entries->Length;
	
	m_Entries = new unsigned __int32[m_EntryCount];
	
	for (int i = 0 ; i < m_EntryCount; ++i)
	{
		m_Entries[i] = entries[i];
	}
}

void BIGTextGroup::ApplyToEntry(AssetEntry^ entry)
{
	unsigned __int32 count = m_EntryCount;
	char* dat = new char[m_EntryCount*sizeof(*m_Entries) + sizeof(m_EntryCount)];
	memcpy(dat, &count, sizeof(count));
	memcpy(dat+sizeof(count), m_Entries, m_EntryCount*sizeof(*m_Entries));
	entry->SetData(dat, m_EntryCount*sizeof(*m_Entries) + sizeof(m_EntryCount));
}

}
}