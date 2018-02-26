#include "stdafx.h"
#include "NamesBINFile.h"

using namespace FableMod::CLRCore;
using namespace System::Runtime::InteropServices;
using namespace System::IO;

namespace FableMod
{
namespace BIN
{

ref class EntryOffsetComparer : public IComparer
{
public:

	virtual int Compare(Object^ x, Object^ y)
	{
		NamesBINEntry^ xEntry = (NamesBINEntry^)x;
		NamesBINEntry^ yEntry = (NamesBINEntry^)y;

		if (xEntry->Offset < yEntry->Offset)
			return -1;
		else if (xEntry->Offset == yEntry->Offset)
			return 0;
		else
			return 1;
	}
};

ref class EntryOffsetSearch : public IComparer
{
public:

	virtual int Compare(Object^ x, Object^ y)
	{
		NamesBINEntry^ entry = (NamesBINEntry^)x;
		unsigned int offset = (unsigned int)y;

		if (entry->Offset < offset)
			return -1;
		else if (entry->Offset == offset)
			return 0;
		else
			return 1;
	}
};

//---------------------------------------------------------------------------

NamesBINFile::NamesBINFile()
{
}

void NamesBINFile::Load(String^ filename,
						FableMod::CLRCore::ProgressInterface^ progress)
{
	m_OriginalFileName = filename;

	FileStream^ BinFile = File::Open(
		filename, 
		FileMode::Open, 
		FileAccess::ReadWrite, 
		FileShare::Read);

	int offset = 0;
	
	char* data = new char[BinFile->Length];

	FileControl::Read(BinFile, data, BinFile->Length); 

	BinFile->Close();
	BinFile = nullptr;

	m_Header = new NamesBINHeader;
	
	memcpy(m_Header, data+offset, sizeof(NamesBINHeader));
	offset += sizeof(NamesBINHeader);

	m_Count = m_Header->EntryCount;
	
	m_Entries = gcnew ArrayList(m_Count);
	
	if (progress != nullptr)	
		progress->Begin(m_Count);

	for(int i = 0; i < m_Count; ++i)
	{
		unsigned int enumval;
		
		memcpy(&enumval, data+offset, sizeof(enumval));
		
		offset += sizeof(enumval);
		
		m_Entries->Add(
			gcnew NamesBINEntry(offset-20, enumval, data+offset));
		
		offset += strlen(data+offset)+1;

		if (progress != nullptr)	
			progress->Update();
	}
	
	if (IsPC)
	{
		// Sort the entries so we can use binary search.
		// About 50% speed increase. 
		m_Entries->Sort(0, m_Entries->Count, gcnew EntryOffsetComparer());
		m_SearchComparer = gcnew EntryOffsetSearch();
	}

	if (progress != nullptr)	
		progress->End();

	delete[] data;
}

NamesBINFile::~NamesBINFile()
{
	m_Entries->Clear();
	m_Entries = nullptr;

	m_SearchComparer = nullptr;

	delete m_Header;
}
	
NamesBINEntry^ NamesBINFile::GetEntry(unsigned int id)
{
	if (IsPC)
		return GetEntryByOffset(id);
	else
		return GetEntryByEnum(id);
}

NamesBINEntry^ NamesBINFile::GetEntryByEnum(unsigned int enumval)
{
	for(int i = 0; i < EntryCount; ++i)
	{
		if (Entries[i]->Enum == enumval)
			return Entries[i];
	}

	return nullptr;
}

NamesBINEntry^ NamesBINFile::GetEntryByOffset(unsigned int offset)
{
	int index = m_Entries->BinarySearch(
		0, m_Entries->Count, offset, m_SearchComparer);

	if (index >= 0)
		return Entries[index];

	return nullptr;
}

NamesBINEntry^ NamesBINFile::GetEntryByName(String^ name)
{
	for(int i = 0; i < EntryCount; ++i)
	{
		if (Entries[i]->Name == name)
			return Entries[i];
	}

	return nullptr;
}

NamesBINEntry^ NamesBINFile::AddEntry(String^ name)
{
	m_Modified = true;

	unsigned int offset = 
		Entries[m_Count-1]->Offset+Entries[m_Count-1]->Name->Length+5;
	
	NamesBINEntry^ entry = gcnew NamesBINEntry(offset, name);
	
	m_Entries->Add(entry);
	
	++m_Count;

	return entry;
}

void NamesBINFile::Save(String^ fileName)
{
	// null term,last enum, and header
	char* data = new char[Entries[m_Count-1]->Offset+
		Entries[m_Count-1]->Name->Length+25]; 
	
	m_Header->EntryCount = m_Count;
	
	// null term, last enum, last null dword from header
	m_Header->Length = Entries[m_Count-1]->Offset+
		Entries[m_Count-1]->Name->Length+5; 
	
	int offset = 0;

	memcpy(data+offset, m_Header, sizeof(NamesBINHeader));
	
	offset += sizeof(NamesBINHeader);

	for(int i = 0; i < m_Count; ++i)
	{
		IntPtr pname = Marshal::StringToHGlobalAnsi(Entries[i]->Name);

		unsigned int enumval = Entries[i]->Enum;
		memcpy(data+offset, &enumval, 4);
		offset+=4;

		strcpy(data+offset, (char*)pname.ToPointer());
		offset+=Entries[i]->Name->Length+1;

		Marshal::FreeHGlobal(pname);
	}

	FileStream^ binFile = File::Create(fileName);
	
	FileControl::Write(binFile, data, offset);
	
	binFile->Close();
	binFile = nullptr;
	
	delete[] data;
}

NamesBINEntry^ NamesBINFile::Entries::get(int index)
{
	return (NamesBINEntry^)m_Entries[index];
}

String^ NamesBINFile::OriginalFileName::get()
{
	return m_OriginalFileName;
}

bool NamesBINFile::Modified::get()
{
	return m_Modified;
}

void NamesBINFile::Modified::set(bool value)
{
	m_Modified = value;
}

int NamesBINFile::EntryCount::get()
{
	return m_Count;
}

bool NamesBINFile::IsPC::get()
{
	return m_Header->Platform == 0xA8E36C34;
}

}
}