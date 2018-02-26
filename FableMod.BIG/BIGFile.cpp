#include "StdAfx.h"
#include "BIGFile.h"

using namespace System::Runtime::InteropServices;
using namespace FableMod::CLRCore;

namespace FableMod
{
namespace BIG
{
		

BIGHeader::BIGHeader()
{
	memset(this, 0, sizeof(*this));
}

BIGFile::BIGFile(int type)
{
	m_SourceHeader = new BIGHeader;
	
	m_SourceHeader->Type[0] = 'B';
	m_SourceHeader->Type[0] = 'I';
	m_SourceHeader->Type[0] = 'G';
	m_SourceHeader->Type[0] = 'B';
	m_SourceHeader->Version = 0x65;
	m_SourceHeader->Unknown = type;

	m_Banks = gcnew Collection<AssetBank^>();
}

BIGFile::BIGFile()
{
	// TODO?
}

BIGFile::~BIGFile()
{
	CloseFile();

	if (m_SourceHeader)
		delete m_SourceHeader;

	if (m_RecompileHeader)
		delete m_RecompileHeader;
}

void BIGFile::Load(String^ filename, ProgressInterface^ progress)
{   
	m_OriginalFileName = filename;
	m_SourceHeader = new BIGHeader;
	
	m_Banks = gcnew Collection<AssetBank^>();

	m_ArchiveFile = File::Open(
		filename, FileMode::Open, FileAccess::ReadWrite, FileShare::Read);
	
	FileControl::Read(
		m_ArchiveFile, m_SourceHeader, sizeof(*m_SourceHeader));

	m_ArchiveFile->Position = m_SourceHeader->BankOffset;
	
	unsigned __int32 SourceBankCount = 0;
	
	FileControl::Read(
		m_ArchiveFile, &SourceBankCount, sizeof(SourceBankCount));

	if (progress)
		progress->Begin(SourceBankCount);

	for (unsigned int i = 0; i < SourceBankCount; ++i)
	{
		m_Banks->Add(gcnew BIGBank(this, ArchiveFile->Position, progress));
	}

	if (progress)
		progress->End();
}

int BIGFile::ContentType::get()
{
	return m_SourceHeader->Unknown;
}

void BIGFile::ContentType::set(int value)
{
	m_SourceHeader->Unknown = value;
}

BIGBank^ BIGFile::Banks::get(int index)
{
	return (BIGBank^)(AssetArchive::Banks[index]);
}

BIGBank^ BIGFile::Banks::get(String^ name)
{
	for (int i = 0; i < BankCount; ++i)
	{
		if (Banks[i]->Name == name)
			return Banks[i];
	}

	return nullptr;
}

BIGBank^ BIGFile::FindBankByID(UInt32 id)
{
	for (int i = 0; i < BankCount; ++i)
	{
		BIGBank^ bank = Banks[i];
		
		if (bank->ID == id)
			return bank;
	}

	return nullptr;
}

BIGBank^ BIGFile::FindBankByName(String^ name)
{
	return Banks[name];
}

void BIGFile::Save(String^ newfile, ProgressInterface^ progress)
{
	if (m_OriginalFileName == newfile)
		throw gcnew Exception("Use a different filename");

	FileStream^ out = File::Create(newfile);

	int bs = Banks[0]->BlockSize;

	int len = sizeof(BIGHeader)/bs;

	if (sizeof(BIGHeader) % bs)
		len += bs;
	
	// write nulls for now, we will write the header 
	// in at the end when we know all the data required
	FileControl::WriteNull(out, len);

	if (progress)
		progress->Begin(BankCount);
	
	for(int i = 0; i < BankCount; ++i)
	{
		Banks[i]->RecompileWriteContent(out, progress);
	}

	if (progress)
		progress->End();
	
	for(int i = 0; i < BankCount; ++i)
	{
		Banks[i]->RecompileWriteEntries(out, nullptr);
	}

	if (m_RecompileHeader)
		delete m_RecompileHeader;

	m_RecompileHeader = new BIGHeader(*m_SourceHeader);
	m_RecompileHeader->BankOffset = out->Position;

	unsigned __int32 bankCount = BankCount;
	
	FileControl::Write(out, &bankCount, sizeof(bankCount));

	for(int i = 0; i < BankCount; ++i)
	{
		Banks[i]->RecompileWriteHeader(out, nullptr);
	}

	// back to beginning to write our new big header
	out->Position = 0;
	FileControl::Write(out, m_RecompileHeader, sizeof(*m_RecompileHeader));

	out->Close();
}

}
}
