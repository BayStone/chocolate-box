#pragma once

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::IO;
using namespace FableMod::CLRCore;

#include "NamesBINFile.h"
#include "BINEntry.h"

namespace FableMod
{
namespace BIN
{

#pragma pack(push,1)
	
struct BINHeader
{
	unsigned char	UseNamesBin;
	unsigned int	ID;
	unsigned int	Platform;
	unsigned int	EntryCount;
};

#pragma pack(pop)

struct BINMainTableEntry
{
	unsigned int	DefinitionOffset;
	unsigned int	NameIdentifier;  // pc/xbox dependant
	unsigned int	DefinitionCount;
};

struct BINSecondTableEntry
{
	unsigned int	StartingEntry;
	unsigned int	ChunkOffset;
};

/// <summary>
/// BIN "archive" loader.
/// </summary>
public ref class BINFile
{
public:

	BINFile(NamesBINFile^ names);
	
	~BINFile();
	
	void Load(String^ filename,
			  ProgressInterface^ progress);
	
	void Save(
		String^ filename, 
		ProgressInterface^ progress);
	
	void Save(
		String^ filename, 
		NamesBINFile^ names,
		ProgressInterface^ progress);

	property BINEntry^ Entries[int] { BINEntry^ get(int index); }
	
	property String^ OriginalFileName { String^ get(); }
	
	property int EntryCount { int get(); }
	
	property bool Modified { bool get(); }

	property bool IsXBox { bool get(); }

	int GetEntryIndex(BINEntry^ entry);

	BINEntry^ GetEntryByEnum(unsigned int enumval);
	
	BINEntry^ GetEntryByName(String^ name);

	array<BINEntry^>^ GetEntriesByDefinition(String^ Definition);

	BINEntry^ AddEntry(
		String^ Name, String^ Definition, array<System::Byte>^ data);
	
	BINEntry^ AddEntry(
		String^ Name, String^ Definition, char* data, unsigned int datalen);

	bool RemoveEntry(BINEntry^ entry);

internal:

private:
	
	bool			m_Modified;
	String^			m_OriginalFileName;
	NamesBINFile^	m_Names;
	unsigned char*	m_Header;
	ArrayList^		m_Entries;
	int				m_Count;

	Dictionary<String^, BINEntry^>^ m_EntryDictionary;
	System::Collections::IComparer^ m_SearchComparer;
};

}
}