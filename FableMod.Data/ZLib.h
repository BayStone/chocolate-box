#pragma once

using namespace System;

namespace FableMod
{
namespace Data
{

public ref class ZLib
{
public:
	
	static array<System::Byte>^ Compress(
		array<System::Byte>^ data, int offset, int count);

	static array<System::Byte>^ Compress(
		array<System::Byte>^ data, int offset, int count, int strength);

	static array<System::Byte>^ Uncompress(
		array<System::Byte>^ data, int offset, int count, int realSize);

	static int Compress(
		void* dest, int maxCount,
		const void* src, int count,
		int level);

	static int Uncompress(
		void* dest, int maxCount,
		const void* src, int count);

	static unsigned __int32 CRC32(
		unsigned __int32 start, const void* src, int count);

	static unsigned __int32 Adler32(
		unsigned __int32 adler,  const void* data,  int count);
};

}
}