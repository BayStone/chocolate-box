#include "Stdafx.h"
#include "BIGBank.h"
#include "AssetArchive.h"

using namespace FableMod::CLRCore;
using namespace System::Runtime::InteropServices;

namespace FableMod
{
namespace BIG
{

BIGBankHeader::BIGBankHeader()
{
	memset(this, 0, sizeof(*this));
}

BIGBank::BIGBank(String^ name, 
				 unsigned __int32 id, 
				 unsigned __int32 blocksize, 
				 AssetArchive^ archive)
{
	m_NewHeader = new BIGBankHeader;
	ID = id;
	BlockSize = blocksize;
	Name = name;
	m_Archive = archive;
	m_Entries = gcnew Collection<AssetEntry^>();
}

BIGBank::BIGBank(AssetArchive^ archive, 
				 unsigned int startpos,
				 ProgressInterface^ progress)
{
	FileStream^ file = archive->ArchiveFile;

	m_Archive = archive;
	m_Entries = gcnew Collection<AssetEntry^>();
	m_Archive->ArchiveFile->Position = startpos;
	m_SourceStartOffset = startpos;

	m_Name = "";
	
	Byte read = file->ReadByte();

	while (read != 0)
	{
		m_Name = String::Concat(m_Name, gcnew String(Char(read),1));
		read = file->ReadByte();
	}

	m_SourceHeader = new BIGBankHeader;
	
	FileControl::Read(file, m_SourceHeader, sizeof(*m_SourceHeader));

	int prevpos = file->Position;
	
	m_SourceLength = prevpos-startpos;

	file->Position = m_SourceHeader->StartOffset;

	unsigned int enumcount;
	
	FileControl::Read(file, &enumcount, sizeof(enumcount));

	file->Position += enumcount*8;
	
	if (progress)
		progress->Begin(m_SourceHeader->EntryCount);

	for (unsigned int i = 0; i < m_SourceHeader->EntryCount; ++i)
	{
		m_Entries->Add(gcnew AssetEntry(
			m_Archive, this, file->Position));
	
		if (progress)
			progress->Update();
	}

	if (progress)
		progress->End();

	file->Position = prevpos;
}

BIGBankHeader* BIGBank::GetNewHeader()
{
	if (!m_NewHeader)
		m_NewHeader = new BIGBankHeader(*m_SourceHeader);

	return m_NewHeader;
}

void BIGBank::RecompileWriteContent(FileStream^ out,
									ProgressInterface^ progress)
{
	if (progress != nullptr)
		progress->Begin(EntryCount);

	for (int i = 0; i < EntryCount; ++i)
	{
		AssetEntry^ entry = Entries[i];
		
		entry->RecompileWriteData(out);
		
		// now make sure we have the correct block size
		int len = BlockSize - entry->Length % BlockSize;

		// write needed padding to fill block
		if (len < BlockSize)
		{
			for (int i = 0; i < len; ++i)
			{
				out->WriteByte(0);
			}
		}
	
		if (progress != nullptr)
			progress->Update();
	}

	if (progress != nullptr)
		progress->End();

}

void BIGBank::RecompileWriteEntries(FileStream^ out,
									ProgressInterface^ progress)
{
	// when this is called should be pointing to the end of the data section
	if (m_RecompileHeader)
		delete m_RecompileHeader;
	
	if (m_NewHeader)
		m_RecompileHeader = new BIGBankHeader(*m_NewHeader);
	else
		m_RecompileHeader = new BIGBankHeader(*m_SourceHeader);

	m_RecompileHeader->StartOffset = out->Position;

	if (m_SourceHeader)
	{		
		unsigned int count = 0;
		TypeCountEntry typecounts[1024];
		
		memset(typecounts, 0, sizeof(unsigned int)*1024);

		for (int i = 0; i < EntryCount; ++i)
		{
			unsigned int type = Entries[i]->Type;
			bool found = false;
			int j;
			for(j=0;j<count;j++)
			{
				if (typecounts[j].Type == type)
				{
					found = true;
					break;
				}
			}
			if (found)
				typecounts[j].Count++;
			else
			{
				typecounts[count].Type = type;
				typecounts[count].Count++;
				count++;
			}
		}

		FileControl::Write(out, &count, sizeof(count));
		FileControl::Write(out, &typecounts, count*sizeof(TypeCountEntry));
	}
	else
	{
		unsigned int enumcount = 0;
		FileControl::Write(out, &enumcount, sizeof(enumcount));
	}

	if (progress != nullptr)
		progress->Begin(EntryCount);

	for (int i = 0; i < EntryCount; ++i)
	{
		Entries[i]->RecompileWriteHeader(out);
	
		if (progress != nullptr)
			progress->Update();
	}
	
	if (progress != nullptr)
		progress->End();

	m_RecompileHeader->Length = 
		out->Position-m_RecompileHeader->StartOffset;
}

void BIGBank::RecompileWriteHeader(FileStream^ out, 
								   ProgressInterface^ progress)
{
	if (!m_RecompileHeader)
		if (m_NewHeader)
			m_RecompileHeader = new BIGBankHeader(*m_NewHeader);
		else
			m_RecompileHeader = new BIGBankHeader(*m_SourceHeader);

	IntPtr pname = Marshal::StringToHGlobalAnsi(m_Name);
	FileControl::Write(out, pname.ToPointer(), m_Name->Length+1);
	Marshal::FreeHGlobal(pname);

	FileControl::Write(out, m_RecompileHeader, sizeof(*m_RecompileHeader));
}

int BIGBank::AddEntry(AssetEntry^ entry)
{
	m_Modified = true;

	m_Entries->Add(entry);

	GetNewHeader()->EntryCount = m_Entries->Count;
	
	return m_Entries->IndexOf(entry);
}

bool BIGBank::RemoveEntry(AssetEntry^ entry)
{
	if (AssetBank::RemoveEntry(entry))
	{
		GetNewHeader()->EntryCount = m_Entries->Count;
		return true;
	}

	return false;
}

unsigned __int32 BIGBank::BlockSize::get()
{
	if (m_NewHeader)
		return m_NewHeader->BlockSize;
	else
		return m_SourceHeader->BlockSize;
}

void BIGBank::BlockSize::set(unsigned __int32 blocksize)
{
	GetNewHeader()->BlockSize = blocksize;
}

unsigned __int32 BIGBank::EntryStartOffset::get()
{
	if (m_NewHeader)
		return m_NewHeader->StartOffset;
	else
		return m_SourceHeader->StartOffset;
}

unsigned __int32 BIGBank::ID::get()
{
	if (m_NewHeader)
		return m_NewHeader->ID;
	else
		return m_SourceHeader->ID;
}

void BIGBank::ID::set(unsigned __int32 id)
{
	GetNewHeader()->ID = id;
}

unsigned __int32 BIGBank::Length::get()
{
	if (m_NewHeader)
		return m_NewHeader->Length;
	else
		return m_SourceHeader->Length;
}

String^ BIGBank::Name::get()
{
	return m_Name;
}

void BIGBank::Name::set(String^ name)
{
	m_Name = name;
}

}
}