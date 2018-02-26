#include "stdafx.h"
#include "AssetBank.h"
#include "AssetArchive.h"
#include "AssetArchiveItem.h"
#include "AssetEntry.h"
#include "BIGBank.h"

using namespace System::Runtime::InteropServices;
using namespace FableMod::CLRCore;

namespace FableMod
{
namespace BIG
{

AssetEntry::AssetEntry(AssetEntry^ entry, AssetBank^ bank)
{
	if (entry->m_NewHeader)
		m_NewHeader = new AssetEntryHeader(*entry->m_NewHeader);
	else
		m_NewHeader = new AssetEntryHeader(*entry->m_SourceHeader);

	SetData(entry->GetData(), entry->Length);

	m_Bank = bank;
	m_Archive = bank->Archive;
	m_Modified = false;
}

AssetEntry::AssetEntry(
	String^ symbolname, 
	unsigned __int32 id, 
	unsigned __int32 type, 
	AssetBank^ bank)
{
	New(symbolname, id, type, bank);
}

AssetEntry::AssetEntry(
	AssetArchive^ archive, 
	AssetBank^ bank, 
	unsigned int startpos)
{
	m_NewHeader = nullptr;

	if (archive != bank->Archive)
		throw gcnew Exception();

	m_Bank = bank;
	m_Archive = archive;
	m_SourceStartOffset = startpos;
	
	LoadFromArchive();

	m_Modified = false;
}

void AssetEntry::New(String^ symbolname, 
					 unsigned __int32 id, 
					 unsigned __int32 type, 
					 AssetBank^ bank)
{
   	m_NewHeader = new AssetEntryHeader;
	ID = id;
	Type = type;
	DevSymbolName = symbolname;
	Data = gcnew array<Byte>(0);
	m_Bank = bank;
	m_Archive = bank->Archive;
	m_Modified = true;
}

void AssetEntry::Copy(AssetEntry^ srcEntry)
{
    DevSymbolName = srcEntry->DevSymbolName;
	DevCRC = srcEntry->DevCRC;
	DevFileType = srcEntry->DevFileType;
	DevSources = srcEntry->DevSources;
	Type = srcEntry->Type;
	SetData(srcEntry->GetData(), srcEntry->Length);
}

void AssetEntry::LoadFromArchive()
{
	if (m_SourceStartOffset)
	{
		m_Archive->ArchiveFile->Position = m_SourceStartOffset;
		
		m_SourceHeader = new AssetEntryHeader;
		
		FileStream^ file = m_Archive->ArchiveFile;

		// read upto beginging of devsymbol
		FileControl::Read(file, m_SourceHeader, 7*4); 
		
		m_SourceHeader->DevSymbolName = 
			new char[m_SourceHeader->DevNameLength+1];
		
		FileControl::Read(file,
			m_SourceHeader->DevSymbolName, m_SourceHeader->DevNameLength);
		
		m_SourceHeader->DevSymbolName[m_SourceHeader->DevNameLength] = 0x00; 

		FileControl::Read(file, &m_SourceHeader->DevCRC, 2*4);
		
		m_SourceHeader->DevSources = 
			new DevSourceEntry[m_SourceHeader->DevSourceCount];
		
		for(unsigned int i = 0; i < m_SourceHeader->DevSourceCount; ++i)
		{
			DevSourceEntry* pDevEntry = &m_SourceHeader->DevSources[i];

			FileControl::Read(file, &pDevEntry->Length, 4); 

			pDevEntry->SourceName = new char[pDevEntry->Length+1];
			
			FileControl::Read(file, 
				pDevEntry->SourceName, pDevEntry->Length); 
		
			pDevEntry->SourceName[pDevEntry->Length] = 0x00; 
		}
		
		FileControl::Read(file, &m_SourceHeader->SubHeaderSize, 4);
		
		m_SourceHeader->SubHeader = new char[m_SourceHeader->SubHeaderSize];
		
		FileControl::Read(file, 
			m_SourceHeader->SubHeader, m_SourceHeader->SubHeaderSize);
		
		m_SourceLength = file->Position-m_SourceStartOffset;
	}
}

AssetEntry::~AssetEntry()
{
	if (m_NewHeader)
	{
		delete m_NewHeader;
		m_NewHeader = nullptr;
	}
	
	if (m_RecompileHeader)
	{
		delete m_RecompileHeader;
		m_RecompileHeader = nullptr;
	}

	if (m_SourceHeader)
	{
		delete m_SourceHeader;
		m_SourceHeader = nullptr;
	}

	if (m_Data)
	{
		delete[] m_Data;
		m_Data = 0;
	}
}

void AssetEntry::Detach()
{
	if (m_Bank)
		m_Bank->RemoveEntry(this);
}

void AssetEntry::Purge()
{
	if (!m_Modified)
	{
		// We can delete this data.

		if (m_Data)
		{
			delete[] m_Data;
			m_Data = 0;
		}
	}
}

char* AssetEntry::GetData()
{
	if (!m_Data)
	{
		m_Data = new char[Length];
		m_Archive->ArchiveFile->Position = StartOffset;
		FileControl::Read(m_Archive->ArchiveFile, m_Data, Length);
		m_Modified = false;
	}

	return m_Data;
}

unsigned __int32 AssetEntry::GetSubHeaderLength()
{
	if (m_NewHeader)
		return m_NewHeader->SubHeaderSize;
	else
		return m_SourceHeader->SubHeaderSize;
}

char* AssetEntry::GetSubHeader()
{
	if (m_NewHeader)
		return m_NewHeader->SubHeader;
	else
		return m_SourceHeader->SubHeader;
}

array<Byte>^ AssetEntry::Data::get()
{
	char* dat = GetData();

	array<Byte>^ b = gcnew array<Byte>(Length);
	
	for (int i = 0; i < Length; ++i)
	{
		b[i] = dat[i];
	}

	return b;
}

array<Byte>^ AssetEntry::SubHeader::get()
{
	unsigned __int32 length = GetSubHeaderLength();

	if (length > 0)
	{
		char* dat = GetSubHeader();

		array<Byte>^ b = gcnew array<Byte>(length);
		
		for (int i = 0; i < length; ++i)
			b[i] = dat[i];
		
		return b;
	}
	else
	{
		return nullptr;
	}
}

unsigned __int32 AssetEntry::MagicNumber::get()
{
	if (m_NewHeader)
		return m_NewHeader->MagicNumber;
	else
		return m_SourceHeader->MagicNumber;
}

unsigned __int32 AssetEntry::ID::get()
{
	if (m_NewHeader)
		return m_NewHeader->ID;
	else
		return m_SourceHeader->ID;
}

unsigned __int32 AssetEntry::Type::get()
{
	if (m_NewHeader)
		return m_NewHeader->Type;
	else
		return m_SourceHeader->Type;
}

unsigned __int32 AssetEntry::Length::get()
{
	if (m_NewHeader)
		return m_NewHeader->Length;
	else
		return m_SourceHeader->Length;
}

unsigned __int32 AssetEntry::StartOffset::get()
{
	if (m_NewHeader)
		return m_NewHeader->StartOffset;
	else
		return m_SourceHeader->StartOffset;
}

unsigned __int32 AssetEntry::DevFileType::get()
{
	if (m_NewHeader)
		return m_NewHeader->DevFileType;
	else
		return m_SourceHeader->DevFileType;
}

String^	AssetEntry::DevSymbolName::get()
{
	if (m_NewHeader)
		return gcnew String(m_NewHeader->DevSymbolName);
	else
		return gcnew String(m_SourceHeader->DevSymbolName);
}

unsigned __int32 AssetEntry::DevCRC::get()
{
	if (m_NewHeader)
		return m_NewHeader->DevCRC;
	else
		return m_SourceHeader->DevCRC;
}

array<String^>^ AssetEntry::DevSources::get()
{
	AssetEntryHeader* h;
	
	if (m_NewHeader)
		h = m_NewHeader;
	else
		h = m_SourceHeader;
	
	array<String^>^ ret = gcnew array<String^>(h->DevSourceCount);

	for(int i = 0; i < h->DevSourceCount; i++)
	{
		ret[i] = System::Runtime::InteropServices::Marshal::PtrToStringAnsi(IntPtr(h->DevSources[i].SourceName));
	}
	
	return ret;
}

int AssetEntry::GetHeaderLength(AssetEntryHeader* header)
{
	int length = 40;
	length += header->DevNameLength;
	for(int i=0;i<header->DevSourceCount;i++)
		length += header->DevSources[i].Length + 4;
	length += header->SubHeaderSize;
	return length;
}

void AssetEntry::ID::set(unsigned __int32 val)
{
	if(!m_NewHeader)
		m_NewHeader = new AssetEntryHeader(*m_SourceHeader);
	
	m_NewHeader->ID = val;
}

void AssetEntry::Type::set(unsigned __int32 val)
{
	if(!m_NewHeader)
		m_NewHeader = new AssetEntryHeader(*m_SourceHeader);
	
	m_NewHeader->Type = val;
}

void AssetEntry::Length::set(unsigned __int32 val)
{
	if(!m_NewHeader)
		m_NewHeader = new AssetEntryHeader(*m_SourceHeader);
	
	m_NewHeader->Length = val;
}

void AssetEntry::DevSymbolName::set(String^ val)
{
	if (!m_NewHeader)
		m_NewHeader = new AssetEntryHeader(*m_SourceHeader);
	
	//IntPtr pval = System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(val);
	delete [] m_NewHeader->DevSymbolName;

	IntPtr pname = Marshal::StringToHGlobalAnsi(val);
	m_NewHeader->DevSymbolName = new char[val->Length+1];
	strcpy(m_NewHeader->DevSymbolName,(char*)pname.ToPointer());
	Marshal::FreeHGlobal(pname);
	m_NewHeader->DevNameLength = val->Length;
}

void AssetEntry::DevSources::set(array<String^>^ val)
{
	if(!m_NewHeader)
		m_NewHeader = new AssetEntryHeader(*m_SourceHeader);

	if (m_NewHeader->DevSourceCount)
	{
		for(int i=0;i<m_NewHeader->DevSourceCount;i++)
			delete [] m_NewHeader->DevSources[i].SourceName;
		
		delete [] m_NewHeader->DevSources;
	}

	m_NewHeader->DevSourceCount = val->Length;
	m_NewHeader->DevSources = new DevSourceEntry[val->Length];

	for(int i=0;i<m_NewHeader->DevSourceCount;i++)
	{
		m_NewHeader->DevSources[i].Length = val[i]->Length;

		IntPtr pname = Marshal::StringToHGlobalAnsi(val[i]);
		m_NewHeader->DevSources[i].SourceName = new char[val[i]->Length+1];
		strcpy(m_NewHeader->DevSources[i].SourceName, (char*)pname.ToPointer());
		Marshal::FreeHGlobal(pname);
	}
}

void AssetEntry::DevFileType::set(unsigned __int32 type)
{
	if (!m_NewHeader)
		m_NewHeader = new AssetEntryHeader(*m_SourceHeader);
	
	m_NewHeader->DevFileType = type;
}

void AssetEntry::DevCRC::set(unsigned __int32 crc)
{
	if (!m_NewHeader)
		m_NewHeader = new AssetEntryHeader(*m_SourceHeader);
	
	m_NewHeader->DevCRC = crc;
}

void AssetEntry::SetData(char* data, unsigned int len)
{
	delete[] m_Data;
	
	m_Data = new char[len];
	
	memcpy(m_Data, data, len);
	
	if (!m_NewHeader)
		m_NewHeader = new AssetEntryHeader(*m_SourceHeader);

	m_NewHeader->Length = len;

	m_Modified = true;
}

void AssetEntry::Data::set(array<Byte>^ data)
{
	if (data->Length > 0)
	{
		pin_ptr<System::Byte> ptr = &(data[0]);
		SetData((char*)ptr, data->Length);
	}
}

void AssetEntry::SetSubHeader(char* data, unsigned int len)
{
	if (!m_NewHeader)
		m_NewHeader = new AssetEntryHeader(*m_SourceHeader);

	delete[] m_NewHeader->SubHeader;

	if (len > 0)
	{
		m_NewHeader->SubHeader = new char[len];
		
		memcpy(m_NewHeader->SubHeader, data, len);
		
		m_NewHeader->SubHeaderSize = len;
	}
	else
	{
		m_NewHeader->SubHeader = 0;
		m_NewHeader->SubHeaderSize = 0;
	}

	//m_Modified = true;
}

void AssetEntry::SubHeader::set(array<Byte>^ data)
{
	if (data == nullptr || data->Length == 0)
	{
		SetSubHeader(0, 0);
	}
	else
	{
		pin_ptr<System::Byte> ptr = &(data[0]);

		SetSubHeader((char*)ptr, data->Length);
	}
}

bool AssetEntry::Modified::get()
{
	return (m_NewHeader || m_Modified);
}

void AssetEntry::RecompileWriteHeader(FileStream^ out)
{
	if (!m_RecompileHeader)
	{
		if (m_NewHeader)
			m_RecompileHeader = new AssetEntryHeader(*m_NewHeader);
		else
			m_RecompileHeader = new AssetEntryHeader(*m_SourceHeader);
	}

	char* data = new char[GetHeaderLength(m_RecompileHeader)];

	int offset = 0;

	memcpy(data+offset, m_RecompileHeader, 28);
	offset += 28;
	memcpy(data+offset, m_RecompileHeader->DevSymbolName, m_RecompileHeader->DevNameLength);
	offset += m_RecompileHeader->DevNameLength;
	memcpy(data+offset, &m_RecompileHeader->DevCRC, 8);
	offset += 8;

	for(int i=0;i<m_RecompileHeader->DevSourceCount;i++)
	{
		memcpy(data+offset, &m_RecompileHeader->DevSources[i].Length, 4);
		offset += 4;
		memcpy(data+offset, m_RecompileHeader->DevSources[i].SourceName, m_RecompileHeader->DevSources[i].Length);
		offset += m_RecompileHeader->DevSources[i].Length;
	}

	memcpy(data+offset, &m_RecompileHeader->SubHeaderSize,4);
	offset += 4;
	memcpy(data+offset, m_RecompileHeader->SubHeader, m_RecompileHeader->SubHeaderSize);
	offset += m_RecompileHeader->SubHeaderSize;

	// write header in bank
	FileControl::Write(out, data, offset);

	delete[] data;
}

void AssetEntry::RecompileWriteData(FileStream^ out)
{
	if (m_RecompileHeader)
		delete m_RecompileHeader;

	if (m_NewHeader)
		m_RecompileHeader = new AssetEntryHeader(*m_NewHeader);
	else
		m_RecompileHeader = new AssetEntryHeader(*m_SourceHeader);

	m_RecompileHeader->StartOffset = out->Position;

	FableMod::CLRCore::FileControl::Write(out, GetData(), Length);

	if (!Modified)
	{
		delete[] m_Data;
		m_Data = NULL;
	}	
}

AssetBank^ AssetEntry::Bank::get()
{
	return m_Bank;
}

String^ AssetEntry::ToString()
{
	if (Bank->GetType() == BIGBank::typeid)
	{
		return String::Format("{0}::{1}::{2}",
			this->ID, ((BIGBank^)Bank)->Name, this->DevSymbolName);
	}
	else return String::Format("{0}::{1}::{2}",
		this->ID, "UNKNOWN", this->DevSymbolName);
}

}
}