#include "Stdafx.h"
#include "AssetBank.h"

namespace FableMod
{
namespace BIG
{

AssetBank::AssetBank() :
m_Modified(false)
{
}

AssetBank::~AssetBank()
{
	if (m_Entries)
	{
		for (int i = 0; i < EntryCount; ++i)
		{	
			delete ((AssetEntry^)m_Entries[i]);
		}
		
		m_Entries->Clear();
		m_Entries = nullptr;
	}
}

AssetArchive^ AssetBank::Archive::get()
{
	return m_Archive;
}

unsigned int AssetBank::GetNewID()
{
	unsigned int ret = 1;
	
	for (int i = 0; i < EntryCount; ++i)
	{
		if (Entries[i]->ID >= ret)
			ret = Entries[i]->ID+1;
	}

	return ret;
}

AssetEntry^ AssetBank::NewEntry(String^ symbolName, unsigned int type)
{
	AssetEntry^ entry = gcnew AssetEntry(
		symbolName,
		GetNewID(),
		type,
		this);

	AddEntry(entry);

	return entry;
}

int AssetBank::AddEntry(AssetEntry^ entry)
{
	m_Modified = true;
	
	m_Entries->Add(entry);

	return m_Entries->IndexOf(entry);
}

bool AssetBank::RemoveEntry(AssetEntry^ entry)
{
	int index = m_Entries->IndexOf(entry);

	if (index >= 0)
	{
		m_Modified = true;
		m_Entries->RemoveAt(index);
		return true;
	}

	return false;
}

AssetEntry^ AssetBank::Entries::get(int index)
{
	return (AssetEntry^)m_Entries[index];
}

AssetEntry^ AssetBank::FindEntryByID(UInt32 id)
{
	for (int i = 0; i < EntryCount; ++i)
	{	
		if (static_cast<AssetEntry^>(m_Entries[i])->ID == id)
			return (AssetEntry^)m_Entries[i];
	}

	return nullptr;
}

AssetEntry^ AssetBank::FindEntryBySymbolName(String^ name)
{
	for (int i = 0; i < EntryCount; ++i)
	{	
		if (static_cast<AssetEntry^>(m_Entries[i])->DevSymbolName == name)
			return (AssetEntry^)m_Entries[i];
	}

	return nullptr;
}

int AssetBank::EntryCount::get()
{
	return m_Entries->Count;
}

bool AssetBank::Modified::get()
{
	if (m_Modified)
		return true;

	if (m_Entries)
	{
		for (int i = 0; i < EntryCount; ++i)
		{	
			if (Entries[i]->Modified)
				return true;
		}
	}

	return false;
}

}
}
