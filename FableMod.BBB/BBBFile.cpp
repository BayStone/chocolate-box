#include "Stdafx.h"
#include "BBBFile.h"

using namespace FableMod::CLRCore;

namespace FableMod
{
namespace BBB
{

BBBFile::BBBFile()
{
	m_Entries = gcnew List<BBBEntry^>();
	m_Header = new BBBHeader();
}

BBBFile::~BBBFile()
{
	Close();

	m_Entries->Clear();

	delete m_Entries;

	delete m_Header;
}

/// <summary>
/// Load the BBB file. Notice that the file stays open, until closed.
/// </summary>
void BBBFile::Open(String^ fileName,
				   ProgressInterface^ progress)
{
	Close();

	m_File = gcnew FileStream(
		fileName, FileMode::Open, FileAccess::Read, FileShare::Read);

	FileControl::Read(m_File, m_Header, sizeof(*m_Header));

	m_File->Position = m_Header->uiListingsOffset;
	
	if (progress != nullptr)
		progress->Begin(m_Header->uiNumFiles);

	BBBDevHeader DevHeader;

	FileControl::Read(m_File, &DevHeader, sizeof(DevHeader));

	for (unsigned __int32 i = 0; i < m_Header->uiNumLevels; ++i)
	{
		BBBEntry^ entry = CreateEntry();

		entry->ReadHeader(m_File);

		m_Entries->Add(entry);
	}

	if (progress != nullptr)
		progress->End();
}

void BBBFile::Close()
{
	if (m_File != nullptr)
	{
		m_File->Close();
		m_File = nullptr;
	}

	m_Entries->Clear();
}


void BBBFile::ExtractFile(String^ path, BBBEntry^ entry)
{
	String^ fullPath = Path::Combine(path,entry->FileName);

	Directory::CreateDirectory(Path::GetDirectoryName(fullPath));

	FileStream^ stream = gcnew FileStream(
		fullPath, 
		FileMode::Create, 
		FileAccess::Write, 
		FileShare::None);

	m_File->Position = entry->FileOffset;

	unsigned __int32 blockSize = m_Header->uiBlockSize;
	unsigned __int32 bytesLeft = entry->FileSize;

	unsigned char* pTmp = new unsigned char[blockSize];

	do
	{
		FileControl::Read(m_File, pTmp, blockSize);

		if (bytesLeft >= blockSize)
		{
			FileControl::Write(stream, pTmp, blockSize);
			bytesLeft -= blockSize;
		}
		else
		{
			FileControl::Write(stream, pTmp, bytesLeft);
			bytesLeft = 0;	
		}
	}
	while (bytesLeft > 0);

	delete[] pTmp;

	stream->Close();
}

array<unsigned char>^ BBBFile::ExtractData(BBBEntry^ entry)
{
	m_File->Position = entry->FileOffset;

	array<unsigned char>^ data = 
		gcnew array<unsigned char>(entry->FileSize);

	m_File->Read(data, 0, entry->FileSize);

	return data;
}

BBBEntry^ BBBFile::Entries::get(int index)
{
	return m_Entries[index];
}

BBBEntry^ BBBFile::Entries::get(String^ name)
{
	for (int i = 0; i < EntryCount; ++i)
	{
		if (String::Compare(Entries[i]->FileName, name, true) == 0)
			return Entries[i];
	}

	return nullptr;
}

int BBBFile::EntryCount::get()
{
	return m_Entries->Count;
}

BBBEntry^ BBBFile::CreateEntry()
{
	return gcnew BBBEntry();
}

}
}