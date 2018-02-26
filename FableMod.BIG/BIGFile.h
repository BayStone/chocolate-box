#pragma once

using namespace System;
using namespace System::Collections;
using namespace System::IO;
using namespace FableMod::CLRCore;

#include "BIGBank.h"
#include "AssetArchive.h"

namespace FableMod
{
namespace BIG
{

struct BIGHeader
{
	char				Type[4];
	unsigned __int32	Version;
	unsigned __int32	BankOffset;
	unsigned __int32	Unknown;

	BIGHeader();
};

/// <summary>
/// BIG archive loader.
/// </summary>
public ref class BIGFile : AssetArchive
{
public:

	BIGFile();
	
	BIGFile(int type);
	
	void Load(String^ filename, ProgressInterface^ progress);

	virtual ~BIGFile();

	virtual void Save(
		String^ newfile, ProgressInterface^ progress) override;
	
	property int ContentType 
	{ 
		virtual int get() override; 
		void set(int value); 
	}
	
	property BIGBank^ Banks[int] { BIGBank^ get(int index); }

	property BIGBank^ Banks[String^] { BIGBank^ get(String^ name); }
	
	BIGBank^ FindBankByID(UInt32 id);
	BIGBank^ FindBankByName(String^ name);

private:
	
	BIGHeader* m_SourceHeader;
	BIGHeader* m_RecompileHeader;
};

}
}
