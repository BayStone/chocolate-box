#include "Stdafx.h"
#include "FileControl.h"

namespace FableMod
{
namespace CLRCore
{

unsigned __int32 FileControl::Read(
	Stream^ Stream, void* pBuffer, unsigned __int32 uiCount)
{
	System::Byte* buffer = (System::Byte*)pBuffer;

	array<System::Byte>^ tmp = gcnew array<System::Byte>(uiCount);

	UInt32 readSize = Stream->Read(tmp, 0, tmp->Length);

	for (UInt32 i = 0; i < readSize; ++i)
	{
		buffer[i] = tmp[i];
	}

	return readSize;
}

unsigned __int32 FileControl::Read(
	FileStream^ File, void* pBuffer, unsigned __int32 uiCount)
{
	DWORD dwReadCount = 0;
	ReadFile((HANDLE)File->Handle,pBuffer,uiCount,&dwReadCount,0);
	return (unsigned __int32)dwReadCount;
}

unsigned __int32 FileControl::Write(
	FileStream^ File, const void* pBuffer, unsigned __int32 uiCount)
{
	DWORD dwWriteCount = 0;
	WriteFile((HANDLE)File->Handle,pBuffer,uiCount,&dwWriteCount,0);
	
	LARGE_INTEGER l;
	l.QuadPart = -(LONGLONG)dwWriteCount;
	SetFilePointerEx((HANDLE)File->Handle, l, 0, FILE_CURRENT);
	File->Position += dwWriteCount;

	return (unsigned __int32)dwWriteCount;
}

void FileControl::WriteNull(FileStream^ File, int count)
{
	for(int i = 0; i < count; ++i)
		File->WriteByte(0);
}

}
}