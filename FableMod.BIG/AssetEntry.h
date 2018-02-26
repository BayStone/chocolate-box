#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Collections::ObjectModel;

#include "AssetArchiveItem.h"
#include "AssetEntryHeader.h"

namespace FableMod
{
namespace BIG
{

ref class AssetBank;
ref class AssetArchive;

public ref class AssetEntry : public AssetArchiveItem
{
public:
	
	AssetEntry(AssetEntry^ entry, AssetBank^ bank);
	AssetEntry(String^ symbolname, unsigned __int32 id, unsigned __int32 type, AssetBank^ bank);
	AssetEntry(AssetArchive^ archive, AssetBank^ bank, unsigned int startpos);

	virtual ~AssetEntry();

	void New(
		String^ symbolname, 
		unsigned __int32 id, 
		unsigned __int32 type, 
		AssetBank^ bank);
	
	void Detach();

	void Purge();

	char* GetData();

	property array<Byte>^ Data
	{
		array<Byte>^ get();
		void set(array<Byte>^ data);
	}

    void Copy(AssetEntry^ srcEntry);
    
	void SetData(char* data, unsigned int len);
	
	char* GetSubHeader();
	
	unsigned __int32 GetSubHeaderLength();

	void SetSubHeader(char* data, unsigned int len);
	
	property array<Byte>^ SubHeader
	{
		array<Byte>^ get();
		void set(array<Byte>^ data);
	}
	
	property unsigned __int32 MagicNumber
	{
		unsigned __int32 get();
	}
	
	property unsigned __int32 ID
	{
		unsigned __int32 get();
		void set(unsigned __int32 val);
	}
	
	property unsigned __int32 Type
	{
		unsigned __int32 get();
		void set(unsigned __int32 val);
	}
	
	property unsigned __int32 Length
	{
		unsigned __int32 get();
		void set(unsigned __int32 val);
	}

	property unsigned __int32 StartOffset
	{
		unsigned __int32 get();
		//void set(unsigned __int32 val);
	}
	
	//--[dev info]--
	
	property unsigned __int32 DevFileType
	{
		unsigned __int32 get();
		void set(unsigned __int32 val);
	}
	
	property String^ DevSymbolName
	{
		String^ get();
		void set(String^ data);
	}
	
	property unsigned __int32 DevCRC
	{
		unsigned __int32 get();
		void set(unsigned __int32 val);
	}
	
	property array<String^>^ DevSources
	{
		array<String^>^ get();
		void set(array<String^>^ sources);
	}
	
	//--[/dev info]--

	property bool Modified
	{
		bool get();
	}

	property AssetBank^	Bank
	{
		AssetBank^ get();
	}

	virtual String^ ToString() override;

	void RecompileWriteHeader(FileStream^ out);
	void RecompileWriteData(FileStream^ out);
	
protected:
	
	void LoadFromArchive();
	int GetHeaderLength(AssetEntryHeader* header);	
	
	AssetBank^ m_Bank;
	bool m_Modified;
	AssetEntryHeader* m_SourceHeader;
	AssetEntryHeader* m_NewHeader;
	AssetEntryHeader* m_RecompileHeader;
	char* m_Data;
};

}
}