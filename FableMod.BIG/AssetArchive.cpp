#include "Stdafx.h"
#include "AssetBank.h"
#include "AssetArchive.h"

namespace FableMod
{
namespace BIG
{

AssetArchive::~AssetArchive()
{
	Destroy();
}

void AssetArchive::CloseFile()
{
	if (m_ArchiveFile)
	{
		m_ArchiveFile->Close();
		m_ArchiveFile = nullptr;
	}
}

void AssetArchive::Destroy()
{
	CloseFile();
	
	if (m_Banks)
	{
		if (BankCount != 0)
		{
			for (int i = 0; i < BankCount; ++i)
			{
				delete Banks[i];
			}

			m_Banks->Clear();
		}
	}
}

AssetBank^ AssetArchive::Banks::get(int index)
{
	return (AssetBank^)m_Banks[index];
}

int AssetArchive::BankCount::get()
{
	if (m_Banks == nullptr)
		return 0;

	return m_Banks->Count;
}

int AssetArchive::AddBank(AssetBank^ entry)
{
	m_Banks->Add(entry);
	return m_Banks->IndexOf(entry);
}

bool AssetArchive::Modified::get()
{
	for (int i = 0; i < BankCount; ++i)
	{
		if (Banks[i]->Modified)
			return true;
	}
	
	return false;
}

FileStream^ AssetArchive::ArchiveFile::get()
{
	return m_ArchiveFile;
}

String^ AssetArchive::OriginalFileName::get()
{
	return m_OriginalFileName;
}

}
}