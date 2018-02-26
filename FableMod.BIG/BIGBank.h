#pragma once

using namespace System;
using namespace System::Collections;
using namespace System::Collections::ObjectModel;
using namespace System::IO;
using namespace FableMod::CLRCore;

#include "AssetBank.h"

namespace FableMod
{
namespace BIG
{
	
struct TypeCountEntry
{
	unsigned __int32 Type;
	unsigned __int32 Count;
};

struct BIGBankHeader
{
	unsigned __int32	ID;
	unsigned __int32	EntryCount;
	unsigned __int32	StartOffset;
	unsigned __int32	Length;
	unsigned __int32	BlockSize;
	
	BIGBankHeader();
};

public ref class BIGBank : public AssetBank
{
public:

	BIGBank(
		String^ name, 
		unsigned __int32 id, 
		unsigned __int32 blocksize, 
		AssetArchive^ archive);

	property String^ Name
	{
		String^ get();
		void set(String^ data);
	}

	property unsigned __int32 ID
	{
		unsigned __int32 get();
		void set(unsigned __int32 value);
	}

	property unsigned __int32 BlockSize
	{
		unsigned __int32 get();
		void set(unsigned __int32 value);
	}

	property unsigned __int32 Length
	{
		unsigned __int32 get();
	}

	property unsigned __int32 EntryStartOffset
	{
		unsigned __int32 get();
	}

	virtual int AddEntry(AssetEntry^ entry) override;

	virtual bool RemoveEntry(AssetEntry^ entry) override;

	virtual void RecompileWriteContent(
		FileStream^ out, ProgressInterface^ progress) override;
	
	virtual void RecompileWriteEntries(
		FileStream^ out, ProgressInterface^ progress) override;
	
	virtual void RecompileWriteHeader(
		FileStream^ out, ProgressInterface^ progress) override;

internal:
	
	BIGBank(
		AssetArchive^ archive, 
		unsigned int startpos,
		ProgressInterface^ progress);

private:

	BIGBankHeader* GetNewHeader();

	String^	m_Name;
	BIGBankHeader* m_SourceHeader;
	BIGBankHeader* m_NewHeader;
	BIGBankHeader* m_RecompileHeader;
};
	
}
}