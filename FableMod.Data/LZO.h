#pragma once

using namespace System;

namespace FableMod
{
namespace Data
{

public ref class LZO
{
public:

	static array<unsigned __int32,2>^ FindChunks(
		unsigned char* data, unsigned __int32 length);
	
	static array<unsigned __int32,2>^ FindChunks(
		array<System::Byte>^ data);
	
	static void DecompressRaw(
		unsigned char* in, unsigned __int32 inlen, 
		unsigned char* out, unsigned __int32* outlen);

	static array<System::Byte>^ DecompressRaw(
		array<System::Byte>^ in, int offset, int count);

	static void DecompressChunk(
		unsigned char* in, unsigned __int32 inlen, 
		unsigned char* out, unsigned __int32* outlen);

	static array<System::Byte>^ DecompressChunk(
		array<System::Byte>^ in, int offset, int count);

	static void CompressRaw(
		unsigned char* in, unsigned __int32 inlen, 
		unsigned char* out, unsigned __int32* outlen);

	static array<System::Byte>^ CompressRaw(
		array<System::Byte>^ in, int offset, int count);

	static void CompressChunk(
		unsigned char* in, unsigned __int32 inlen, 
		unsigned char* out, unsigned __int32* outlen, 
		int trailcount);

	static void CompressChunk(
		unsigned char* in, unsigned __int32 inlen, 
		unsigned char* out, unsigned __int32* outlen, 
		int trailcount, bool forceDwordLength);

	static array<System::Byte>^ CompressChunk(
		array<System::Byte>^ in, int offset, int count, int trailcount);
};

}
}