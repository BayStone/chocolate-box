#include "Stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include "BufferStream.h"
#include "FileControl.h"

namespace FableMod
{
namespace CLRCore
{

BufferStream::BufferStream(FileStream^ file) :
m_iIndex(-1),
m_iLength(0),
m_pucBuffer(0)
{
	m_iLength = (int)file->Length;

	m_pucBuffer = new unsigned char[m_iLength];

	FileControl::Read(file, m_pucBuffer, m_iLength);

	m_iIndex = 0;
	m_bNew = true;
}

BufferStream::BufferStream(const void* pBuffer, int iLength)
{
	m_pucBuffer = (unsigned char*)pBuffer;
	m_iIndex = 0;
	m_iLength = iLength;
	m_bNew = false;
}

BufferStream::BufferStream()
{
	m_iLength = BUFFER_GROW;
	m_pucBuffer = new unsigned char[m_iLength];
	m_iIndex = 0;
	m_bNew = false;
}

BufferStream::~BufferStream()
{
	Console::WriteLine("BufferStream::Destructor");
	Close();
}

// Close the file aka. free the used memory.
void BufferStream::Close()
{
	if (m_bNew && m_pucBuffer)
		delete[] m_pucBuffer;
	
	m_pucBuffer = 0;
	
	m_iLength = 0;
	m_iIndex = -1;
}

/// Read a block of data from the current position.
/// @returns the number of bytes read.
int BufferStream::Read(void* pData, int iCount)
{
	int iLeft = m_iLength-m_iIndex;

	if (iLeft < 0)
		return 0;
	
	if (iCount > iLeft)
		iCount = iLeft;

	memcpy(pData, m_pucBuffer+m_iIndex, iCount);

	m_iIndex += iCount;

	return iCount;
}

System::UInt32 BufferStream::ReadUInt32()
{
	System::UInt32 i;
	Read(&i, sizeof(i));
	return i;
}

char BufferStream::ReadChar()
{
	return m_pucBuffer[m_iIndex++];
}

char* BufferStream::ReadZString()
{
	int iLen = strlen((char*)(m_pucBuffer+m_iIndex));
	char* pTmp = new char[iLen+1];
	Read(pTmp, iLen+1);
	return pTmp;
}

char* BufferStream::ReadString()
{
	UInt32 iLen = ReadUInt32();
	char* pTmp = new char[iLen+1];
	Read(pTmp, iLen);
	pTmp[iLen] = 0;
	return pTmp;
}

System::String^ BufferStream::MReadZString()
{
	char* pszTmp = ReadZString();
	System::String^ sTmp = gcnew System::String(pszTmp);
	delete[] pszTmp;
	return sTmp;
}

System::String^ BufferStream::MReadString()
{
	char* pszTmp = ReadString();
	System::String^ sTmp = gcnew System::String(pszTmp);
	delete[] pszTmp;
	return sTmp;
}

int BufferStream::Write(const void* pData, int iCount)
{
	if (m_iIndex+iCount >= m_iLength)
	{
		int iLen = m_iIndex+iCount+BUFFER_GROW;

		unsigned char* pucTemp = new unsigned char[iLen];

		memcpy(pucTemp, m_pucBuffer, m_iLength);
		
		//memset(pucTemp+m_iIndex, 0xFF, iLen-iIndex);

		delete[] m_pucBuffer;

		m_pucBuffer = pucTemp;

		m_iLength = iLen;
	}

	memcpy(m_pucBuffer+m_iIndex, pData, iCount);

	m_iIndex += iCount;

	return iCount;
}

int BufferStream::WriteZString(const char* pszString)
{
	return Write(pszString, strlen(pszString)+1);
}

int BufferStream::WriteUInt32(System::UInt32 uiValue)
{
	return Write(&uiValue, sizeof(uiValue));
}

int BufferStream::WriteFloat(float fValue)
{
	return Write(&fValue, sizeof(fValue));
}

/// Jump to the given offset (from the beginning) in the file.
void BufferStream::Seek(int iOffset)
{
	m_iIndex = iOffset;
}

void BufferStream::Ignore(int iCount)
{
	m_iIndex += iCount;
}

int BufferStream::GetSize()
{
	return m_iLength;
}

int BufferStream::GetWritten()
{
	return m_iIndex;
}

}
}