#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;
using namespace FableMod::CLRCore;

#include "BBBHeaders.h"
#include "BBBEntry.h"

namespace FableMod
{
namespace BBB
{

/// <summary>
/// BBB archive loader.
/// </summary>
public ref class BBBFile
{
public:

	BBBFile();

	virtual ~BBBFile();

	void Open(
		String^ fileName, 
		ProgressInterface^ progress);

	void Close();

	void ExtractFile(String^ path, BBBEntry^ entry);
	
	array<unsigned char>^ ExtractData(BBBEntry^ entry);
	
	property int EntryCount { int get(); }

	property BBBEntry^ Entries[int] 
	{ 
		BBBEntry^ get(int index); 
	}
	
	property BBBEntry^ Entries[String^] 
	{ 
		BBBEntry^ get(String^ name); 
	}

protected:

	property FileStream^ File { FileStream^ get() { return m_File; } }

	virtual BBBEntry^ CreateEntry();

	List<BBBEntry^>^ m_Entries;
	BBBHeader* m_Header;
	FileStream^ m_File;
};

}
}