#include "Stdafx.h"
#include "BBBEntry.h"

using namespace System::Diagnostics;
using namespace FableMod::CLRCore;

namespace FableMod
{
namespace BBB
{

BBBEntry::BBBEntry() :
m_Header(new BBBFileHeaderA())
{
}

BBBEntry::~BBBEntry()
{
	delete m_Header;
	m_FileName = nullptr;
	m_DevFileName = nullptr;
}

void BBBEntry::ReadHeader(FileStream^ file)
{
	char* pszTmp = new char[1024];

	m_HeaderOffset = (unsigned __int32)file->Position;

	FileControl::Read(file, m_Header, sizeof(*m_Header));
	
	FileControl::Read(file, pszTmp, m_Header->uiNameSize);

	pszTmp[m_Header->uiNameSize] = 0;

	m_FileName = gcnew String(pszTmp);

	unsigned __int32 uiNull;
	FileControl::Read(file, &uiNull, sizeof(uiNull));

	unsigned __int32 uiOne;
	FileControl::Read(file, &uiOne, sizeof(uiOne));

	unsigned __int32 uiLen;
	FileControl::Read(file, &uiLen, sizeof(uiLen));

    FileControl::Read(file, pszTmp, uiLen);

	pszTmp[uiLen] = 0;

	m_DevFileName = gcnew String(pszTmp);

	unsigned __int32 uiExtra;
	FileControl::Read(file, &uiExtra, sizeof(uiExtra));

	ReadHeaderExtra(file, uiExtra);

	delete[] pszTmp;
}

void BBBEntry::ReadHeaderExtra(FileStream^ file, unsigned __int32 size)
{
	file->Position += size;
}

}
}