#pragma once

using namespace System;
using namespace System::IO;

namespace FableMod
{
namespace CLRCore
{

public ref class BufferStream
{
public:

	BufferStream(FileStream^ file);

	BufferStream(const void* pBuffer, int iLength);

	BufferStream();

	~BufferStream();

	void Close();

	int Read(void* pData, int iCount);

	char ReadChar();

	System::UInt32 ReadUInt32();

	char* ReadZString();

	char* ReadString();

	System::String^ MReadZString();

	System::String^ MReadString();

	int Write(const void* pData, int iCount);

	int WriteZString(const char* pszString);

	int WriteUInt32(System::UInt32 uiValue);

	int WriteFloat(float fValue);

	void Seek(int iOffset);

	void Ignore(int iCount);
	
	int Tell() { return m_iIndex; }

	bool IsOpen() { return m_pucBuffer != 0; }

	int GetSize();

	int GetWritten();

	unsigned char* GetData() { return m_pucBuffer; }

	static int BUFFER_GROW = 4096;

protected:

	unsigned char* m_pucBuffer;
	int m_iIndex;
	int m_iLength;
	bool m_bNew;
};

}
}