#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Collections::ObjectModel;
using namespace FableMod::CLRCore;

#include "AssetArchiveItem.h"
#include "AssetEntry.h"

namespace FableMod
{
namespace BIG
{

public ref class AssetBank abstract : public AssetArchiveItem
{
public:

	AssetBank();

	virtual ~AssetBank();

	unsigned int GetNewID();
	
	AssetEntry^ FindEntryByID(UInt32 id);
	
	AssetEntry^ FindEntryBySymbolName(String^ name);
	
	AssetEntry^ NewEntry(String^ symbolName, unsigned int type);

	virtual int AddEntry(AssetEntry^ entry);
	
	virtual bool RemoveEntry(AssetEntry^ entry);

	property AssetEntry^ Entries[int] { AssetEntry^ get(int index); }
	
	property int EntryCount { int get(); }
	
	property bool Modified { bool get(); }

	property AssetArchive^ Archive { AssetArchive^ get(); }

	virtual void RecompileWriteContent(
		FileStream^ out, ProgressInterface^ progress) = 0;
	virtual void RecompileWriteEntries(
		FileStream^ out, ProgressInterface^ progress) = 0;
	virtual void RecompileWriteHeader(
		FileStream^ out, ProgressInterface^ progress) = 0;

protected:
	
	bool m_Modified;
	Collection<AssetEntry^>^ m_Entries;
};

}
}