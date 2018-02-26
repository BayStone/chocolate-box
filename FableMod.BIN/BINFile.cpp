#include "stdafx.h"
#include "BINFile.h"

using namespace FableMod::Data;
using namespace FableMod::CLRCore;
using namespace System::Runtime::InteropServices;

namespace FableMod
{
namespace BIN
{

ref class EntrySort : public System::Collections::IComparer
{
public:

	virtual int Compare(Object^ x, Object^ y)
	{
		BINEntry^ xEntry = (BINEntry^)x;
		BINEntry^ yEntry = (BINEntry^)y;

		if (xEntry->NameEnum < yEntry->NameEnum)
			return -1;
		else if (xEntry->NameEnum == yEntry->NameEnum)
			return 0;
		else
			return 1;
	}
};

ref class EntrySearch : public System::Collections::IComparer
{
public:

	virtual int Compare(Object^ x, Object^ y)
	{
		BINEntry^ entry = (BINEntry^)x;
		unsigned int id = (unsigned int)y;

		if (entry->NameEnum < id)
			return -1;
		else if (entry->NameEnum == id)
			return 0;
		else
			return 1;
	}
};

BINFile::BINFile(NamesBINFile^ names) :
m_SearchComparer(gcnew EntrySearch()),
m_Names(names)
{
}

void BINFile::Load(String^ filename,
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

	m_Header = new unsigned char[9];
	
	FileControl::Read(BinFile, data, BinFile->Length); 

	BinFile->Close();
	BinFile = nullptr;
	
	memcpy(m_Header, data+offset, 9);
	offset+=9;

	unsigned int entrycount = 0;
	memcpy(&entrycount, data+offset, sizeof(entrycount));
	offset+=sizeof(entrycount);

	BINMainTableEntry* maintable = new BINMainTableEntry[entrycount];
	memcpy(maintable, data+offset, sizeof(BINMainTableEntry)*entrycount);
	offset+=sizeof(BINMainTableEntry)*entrycount;

	unsigned int entrycount2;
	memcpy(&entrycount2, data+offset, sizeof(entrycount2));
	offset+=sizeof(entrycount2);

	BINSecondTableEntry* secondtable = new BINSecondTableEntry[entrycount2];
	memcpy(secondtable, data+offset, sizeof(BINSecondTableEntry)*entrycount2);
	offset+=sizeof(BINSecondTableEntry)*entrycount2;

	unsigned int datablocklength;
	memcpy(&datablocklength, data+offset, sizeof(datablocklength));
	offset+=sizeof(datablocklength);

	m_Entries = gcnew ArrayList(entrycount);
	
	m_Count = entrycount;

	char* tmpdata = new char[65536];

	int index = 0;

	// Set interface.
	if (progress != nullptr)	
		progress->Begin(entrycount2-1);

	for (int i = 0; i < entrycount2-1; ++i)
	{
		BINSecondTableEntry* pSecondEntry = &secondtable[i];
		
		int outlen = ZLib::Uncompress(
			tmpdata, 
			65536, 
			(unsigned char*)data+offset+pSecondEntry->ChunkOffset, 
			secondtable[i+1].ChunkOffset-pSecondEntry->ChunkOffset);
			
		int index = 0;
		
		for (index = pSecondEntry->StartingEntry; 
			 index < secondtable[i+1].StartingEntry-1; ++index)
		{
			BINMainTableEntry* pMainEntry = &maintable[index];

			int dataoffset = ((WORD*)tmpdata)
				[index-pSecondEntry->StartingEntry];
			
			int datalen = ((WORD*)tmpdata)
				[index-pSecondEntry->StartingEntry+1]-dataoffset;
			
			String^ Definition = m_Names->GetEntryByOffset(
				pMainEntry->DefinitionOffset)->Name;

			String^ Name = "";
			
			unsigned int enumval = 0;
			
			NamesBINEntry^ namesentry;
			
			if (IsXBox)
				namesentry = m_Names->GetEntryByEnum(
					pMainEntry->NameIdentifier);
			else
				namesentry = m_Names->GetEntryByOffset(
					pMainEntry->NameIdentifier);

			if (IsXBox)
			{
				enumval = pMainEntry->NameIdentifier;
				Name = gcnew String(tmpdata+dataoffset);
				datalen -= strlen(tmpdata+dataoffset)+1;
				dataoffset += strlen(tmpdata+dataoffset)+1;
			}
			else
			{
				if (namesentry != nullptr)
				{
					enumval = namesentry->Enum;
					Name = namesentry->Name;
				}
			}
			
			m_Entries->Add(gcnew BINEntry(
				this,
				Definition, 
				Name,
				enumval,
				tmpdata+dataoffset, 
				datalen));
		}
		
		int dataoffset = ((WORD*)tmpdata)
			[index-pSecondEntry->StartingEntry];
		
		int datalen = outlen - dataoffset;
		
		String^ Definition = m_Names->GetEntryByOffset(
			maintable[index].DefinitionOffset)->Name;
		
		String^ Name = "";
		
		unsigned int enumval = 0;

		NamesBINEntry^ namesentry;
		
		if (IsXBox)
			namesentry = m_Names->GetEntryByEnum(
				maintable[index].NameIdentifier);
		else
			namesentry = m_Names->GetEntryByOffset(
				maintable[index].NameIdentifier);

		if (IsXBox)
		{
			enumval = maintable[index].NameIdentifier;
			Name = gcnew String(tmpdata+dataoffset);
			datalen -= strlen(tmpdata+dataoffset)+1;
			dataoffset += strlen(tmpdata+dataoffset)+1;
		}
		else
		{
			if (namesentry != nullptr)
			{
				enumval = namesentry->Enum;
				Name = namesentry->Name;
			}
		}

		m_Entries->Add(gcnew BINEntry(
			this, 
			Definition, 
			Name, 
			enumval, 
			tmpdata+dataoffset, 
			datalen));
	
		if (progress != nullptr)
			progress->Update();
	}

	if (progress != nullptr)
		progress->End();

	delete[] maintable;
	delete[] secondtable;
	delete[] tmpdata;
	delete[] data;
}

BINFile::~BINFile()
{
	m_Entries->Clear();
	m_Entries = nullptr;

	delete m_Header;

	if (m_EntryDictionary)
	{
		m_EntryDictionary->Clear();
		m_EntryDictionary = nullptr;
	}
}

BINEntry^ BINFile::Entries::get(int index)
{
	if (index > m_Entries->Count)
		return nullptr;

	return (BINEntry^)m_Entries[index];
}

String^ BINFile::OriginalFileName::get()
{
	return m_OriginalFileName;
}

int BINFile::EntryCount::get()
{
	return m_Count;
}

int BINFile::GetEntryIndex(BINEntry^ entry)
{
	return m_Entries->IndexOf(entry);
}

bool BINFile::IsXBox::get()
{
	return m_Header[1] == 0xAA;
}

bool BINFile::Modified::get()
{
	if (m_Modified)
		return true;

	for (int i = 0; i < EntryCount; ++i)
	{
		if (Entries[i]->Modified)
			return true;
	}
	
	return false;
}

BINEntry^ BINFile::GetEntryByEnum(unsigned int enumval)
{
	for (int i = 0; i < EntryCount; ++i)
	{
		if (Entries[i]->NameEnum == enumval)
			return Entries[i];
	}

	return nullptr;
}

BINEntry^ BINFile::GetEntryByName(String^ name)
{
	if (m_EntryDictionary == nullptr)
	{
		m_EntryDictionary = gcnew Dictionary<String^, BINEntry^>();
	
		// Add entries to the dictionary.
		for (int i = 0; i < EntryCount; ++i)
			m_EntryDictionary[Entries[i]->Name] = Entries[i];
	}

	BINEntry^ entry = nullptr;

	if (m_EntryDictionary->TryGetValue(name, entry))
		return entry;

	return nullptr;
}

array<BINEntry^>^ BINFile::GetEntriesByDefinition(String^ Definition)
{
	ArrayList^ tmp = gcnew ArrayList();

	for (int i = 0; i < EntryCount; ++i)
	{
		if (Entries[i]->Definition == Definition)
			tmp->Add(Entries[i]);
	}
	
	array<BINEntry^>^ ret = gcnew array<BINEntry^>(tmp->Count);
	
	for (int i = 0; i < tmp->Count; ++i)
		ret[i] = (BINEntry^)tmp[i];
	
	tmp->Clear();

	return ret;
}

BINEntry^ BINFile::AddEntry(String^ Name, 
							String^ Definition, 
							array<System::Byte>^ data)
{
	pin_ptr<System::Byte> ptr = &(data[0]);

	BINEntry^ entry = AddEntry(
		Name, Definition, (char*)ptr, (unsigned int)data->Length);
	
	return entry;
}

BINEntry^ BINFile::AddEntry(String^ Name, 
							String^ Definition, 
							char* data, 
							unsigned int datalen)
{
	m_Modified = true;

	BINEntry^ entry = gcnew BINEntry(
		this, Definition, Name, data, datalen);

	m_Entries->Add(entry);

	m_Count = m_Entries->Count;

	// Release the dictionary, we need to rebuild it.
	m_EntryDictionary = nullptr;

	return entry;
}


bool BINFile::RemoveEntry(BINEntry^ entry)
{
	int index = m_Entries->IndexOf(entry);

	if (index >= 0)
	{
		m_Modified = true;
		m_Entries->RemoveAt(index);
		m_Count = m_Entries->Count;

		return true;
	}

	return false;
}

void BINFile::Save(String^ filename, ProgressInterface^ progress)
{
	Save(filename, m_Names, progress);
}

/// Save the bin file and add new name entries into 
/// the given names.bin file.
void BINFile::Save(String^ filename, 
				   NamesBINFile^ names,
				   ProgressInterface^ progress)
{
	const int MaxChunkSize = 32768;
	
	BINMainTableEntry* maintable = new BINMainTableEntry[m_Count];
	memset(maintable, 0, sizeof(BINMainTableEntry)*m_Count);
	unsigned int secondtablecount = 0;

	// this should be plenty large buffer for this purpose
	BINSecondTableEntry* secondtable = new BINSecondTableEntry[1024]; 
	char** secondtablebuffers = new char*[1024];
	int secondtablebuffersizes[1024];

	unsigned __int16* chunkoffsets = new unsigned __int16[2048];
	char* tmpdata = new char[MaxChunkSize];
	char* tmpcompresseddata = new char[MaxChunkSize];
	int tmpoffset = 0;
	int currentchunkentrycount = 0;
	int tmptotallength = 0;

	System::Collections::Generic::SortedList<DWORD, int>^ defcounts = 
		gcnew System::Collections::Generic::SortedList<DWORD, int>();

	if (progress != nullptr)
		progress->Begin(EntryCount);

	for (int i = 0; i < EntryCount; ++i)
	{
		BINEntry^ entry = Entries[i];
		// setup main table
		
		if (IsXBox)
		{
			maintable[i].NameIdentifier = entry->NameEnum;
		}
		else
		{
			if (entry->Name != "")
			{
				// Check names.bin for the name.
				
				NamesBINEntry^ ent = names->GetEntryByName(entry->Name);
				
				if (ent == nullptr)
				{
					ent = names->AddEntry(entry->Name);
				}

				maintable[i].NameIdentifier = ent->Offset;
			}
			else
				maintable[i].NameIdentifier = 0xffffffff;
		}
		
		// Check names.bin for the definition.
				
		NamesBINEntry^ ent = names->GetEntryByName(entry->Definition);
		
		if (ent == nullptr)
			ent = names->AddEntry(entry->Definition);
		
		maintable[i].DefinitionOffset = ent->Offset;

		// set definition count, per def type
		int defcount = 0;
		
		if (defcounts->TryGetValue(ent->Offset, defcount))
		{
			maintable[i].DefinitionCount = defcount;
			defcount++;
			defcounts[ent->Offset] = defcount;
		}
		else
		{
			maintable[i].DefinitionCount = 0;
			defcounts[ent->Offset] = 1;
		}

		// end setup main table

		//setup second table and compressed data
		int entrylen = entry->Length;
		
		if (IsXBox)
		{
			if (entry->NameEnum != 0)
			{
				entrylen += entry->Name->Length+1;
			}
		}
		
		// this will cause overflow over the 32kb chunks they seem to use
		if (tmpoffset+entrylen+2*(currentchunkentrycount+1) > MaxChunkSize) 
		{
			//prep input
			for (int j = 0; j < currentchunkentrycount; ++j)
			{
				chunkoffsets[j] += 2*currentchunkentrycount;
			}
			
			memmove(tmpdata+chunkoffsets[0], tmpdata, tmpoffset);
			tmpoffset += chunkoffsets[0];
			memcpy(tmpdata, chunkoffsets, chunkoffsets[0]);

			unsigned long outlen = ZLib::Compress(
				tmpcompresseddata,
				MaxChunkSize,
				tmpdata,
				tmpoffset,
				1);
			
			secondtablebuffers[secondtablecount] = new char[outlen];
			memcpy(secondtablebuffers[secondtablecount], tmpcompresseddata, outlen);
			secondtablebuffersizes[secondtablecount] = outlen;

			secondtable[secondtablecount].StartingEntry = i - currentchunkentrycount;
			secondtable[secondtablecount].ChunkOffset = tmptotallength;
			tmptotallength += outlen;

			//reset offsets, counts, etc for next chunk
			secondtablecount++;
			tmpoffset = 0;
			currentchunkentrycount = 0;
		}
		
		chunkoffsets[currentchunkentrycount] = tmpoffset;
		currentchunkentrycount++;
		
		if (IsXBox)
		{
			IntPtr pname = Marshal::StringToHGlobalAnsi(entry->Name);
			strcpy(tmpdata+tmpoffset, (char*)pname.ToPointer());
			tmpoffset += entry->Name->Length+1;
			Marshal::FreeHGlobal(pname);
		}
		
		memcpy(tmpdata+tmpoffset, entry->GetData(), entry->Length);
		tmpoffset+=entry->Length;

		if (progress != nullptr)
			progress->Update();
	}
	
	if (progress != nullptr)
		progress->End();

	// make sure we save last chunk that was in progress, only if it has entries in it though
	if (currentchunkentrycount != 0) 
	{
		//prep input
		for (int i = 0; i < currentchunkentrycount; ++i)
		{
			chunkoffsets[i] += 2*currentchunkentrycount;
		}
		
		memmove(tmpdata+chunkoffsets[0], tmpdata, tmpoffset);
		tmpoffset += chunkoffsets[0];
		memcpy(tmpdata, chunkoffsets, chunkoffsets[0]);

		//compress and store away
		int outlen = ZLib::Compress(
				tmpcompresseddata,
				MaxChunkSize,
				tmpdata,
				tmpoffset,
				1);
		
		secondtablebuffers[secondtablecount] = new char[outlen];
		memcpy(secondtablebuffers[secondtablecount], tmpcompresseddata, outlen);
		secondtablebuffersizes[secondtablecount] = outlen;

		secondtable[secondtablecount].StartingEntry = m_Count - currentchunkentrycount;
		secondtable[secondtablecount].ChunkOffset = tmptotallength;
		tmptotallength += outlen;

		//reset offsets, counts, etc for next chunk
		secondtablecount++;
		tmpoffset = 0;
		currentchunkentrycount = 0;
	}

	delete defcounts;

	// add final entry that should just point to end of file
	secondtable[secondtablecount].StartingEntry = m_Count;
	secondtable[secondtablecount].ChunkOffset = tmptotallength;
	secondtablebuffers[secondtablecount] = new char[0];
	secondtablebuffersizes[secondtablecount] = 0;
	secondtablecount++;

	// finally, start actually compiling the data into final form
	char* data = new char[sizeof(BINMainTableEntry)*m_Count+
		sizeof(BINSecondTableEntry)*secondtablecount+tmptotallength+21];
	
	int offset = 0;
	
	unsigned int entrycount = m_Count;
	//unsigned int entrycount2 = secondtablecount;

	memcpy(data+offset, m_Header, 9);
	offset+=9;

	memcpy(data+offset, &entrycount, sizeof(entrycount));
	offset+=sizeof(entrycount);

	memcpy(data+offset,maintable,sizeof(BINMainTableEntry)*entrycount);
	offset+=sizeof(BINMainTableEntry)*entrycount;

	memcpy(data+offset, &secondtablecount, sizeof(secondtablecount));
	offset+=sizeof(secondtablecount);

	memcpy(data+offset,secondtable,sizeof(BINSecondTableEntry)*secondtablecount);
	offset+=sizeof(BINSecondTableEntry)*secondtablecount;

	memcpy(data+offset, &tmptotallength, sizeof(tmptotallength));
	offset+=sizeof(tmptotallength);

	// for each chunk (last entry in second table is empty)
	for (int i = 0; i < secondtablecount; ++i)
	{
		memcpy(data+offset, secondtablebuffers[i], secondtablebuffersizes[i]);
		offset+=secondtablebuffersizes[i];
	}

	// Write the data into the file.
	
	FileStream^ BinFile = File::Create(filename);
	
	FileControl::Write(BinFile, data, offset);
	
	BinFile->Close();

	// Cleanup time.
	
	delete[] data;
	delete[] tmpdata;
	delete[] tmpcompresseddata;
	delete[] chunkoffsets;
	
	for (int i = 0; i < secondtablecount; ++i)
	{
		delete secondtablebuffers[i];
	}
	
	delete[] secondtablebuffers;
}

}
}
