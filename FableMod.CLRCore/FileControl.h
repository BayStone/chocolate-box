#pragma once

using namespace System;
using namespace System::IO;

namespace FableMod
{
namespace CLRCore
{

public ref class FileControl
{
public:
	static unsigned __int32 Read(
		FileStream^ File, void* pBuffer, unsigned __int32 uiCount);

	static unsigned __int32 Write(
		FileStream^ File, const void* pBuffer, unsigned __int32 uiCount);
	
	static unsigned __int32 Read(
		Stream^ Stream, void* pBuffer, unsigned __int32 uiCount);

	static void WriteNull(FileStream^ File, int count);
};

}
}