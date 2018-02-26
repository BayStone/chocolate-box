#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Collections::ObjectModel;
using namespace FableMod::CLRCore;

namespace FableMod
{
namespace BIG
{

ref class AssetBank;

public ref class AssetArchive abstract
{
public:

	virtual ~AssetArchive();

	void CloseFile();
		
	void Destroy();

	virtual int AddBank(AssetBank^ entry);

	virtual void Save(String^ newfile, ProgressInterface^ progress) = 0;
	
	property AssetBank^ Banks[int] { AssetBank^ get(int index); }

	property String^ OriginalFileName { String^ get(); }
	
	property int BankCount { int get(); }
	
	property int ContentType { virtual int get() = 0; }

	property bool Modified { bool get(); }

	property FileStream^ ArchiveFile { FileStream^ get(); }

protected:

	FileStream^ m_ArchiveFile;
	String^	m_OriginalFileName;
	Collection<AssetBank^>^ m_Banks;
};

}
}