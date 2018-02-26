#pragma once

using namespace System;
using namespace System::Collections;

#include "NamesBINEntry.h"

namespace FableMod
{
namespace BIN
{

struct NamesBINHeader
{
	DWORD	Tag;
	DWORD	Platform;
	DWORD	EntryCount;
	DWORD	Length;
	DWORD	Null;
};

/// <summary>
/// Names.bin loader.
/// </summary>
public ref class NamesBINFile
{
public:

	NamesBINFile();

	~NamesBINFile();
	
	void Load(
		String^ filename, 
		FableMod::CLRCore::ProgressInterface^ progress);

	void Save(String^ filename);

	property NamesBINEntry^ Entries[int] { NamesBINEntry^ get(int index); }
	
	property String^ OriginalFileName { String^ get(); }
	
	property int EntryCount { int get(); }
	
	property bool Modified 
	{ 
		bool get(); 
		void set(bool value);
	}

	property bool IsPC { bool get(); }

	NamesBINEntry^ AddEntry(String^ name);
	
	NamesBINEntry^ GetEntryByEnum(unsigned int enumval);
	NamesBINEntry^ GetEntryByOffset(unsigned int offset);
	NamesBINEntry^ GetEntryByName(String^ name);
	NamesBINEntry^ GetEntry(unsigned int id);
	
internal:
private:
	bool			m_Modified;
	String^			m_OriginalFileName;
	NamesBINHeader*	m_Header;
	ArrayList^		m_Entries;
	int				m_Count;
	System::Collections::IComparer^	m_SearchComparer;
};

}
}
