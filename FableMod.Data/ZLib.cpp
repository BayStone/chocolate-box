#include "Stdafx.h"
#include "ZLib.h"

namespace FableMod
{
namespace Data
{

static unsigned char* CreateBuffer(
	array<System::Byte>^ data, int offset, int count)
{
	unsigned char* tmp = new unsigned char[count];

	for (int i = 0; i < count; ++i)
	{
		tmp[i] = data[offset+i];
	}

	return tmp;
}

static array<System::Byte>^ CreateBuffer(unsigned char* data, int count)
{
	array<System::Byte>^ tmp = gcnew array<System::Byte>(count);

	for (int i = 0; i < count; ++i)
	{
		tmp[i] = data[i];
	}

	return tmp;
}

static void CompressData(
	unsigned char* in, unsigned __int32 inlen, 
	unsigned char* out, unsigned __int32* outlen)
{
	int ret = compress(out,(uLongf*)outlen,in,inlen);
		
	if (ret != Z_OK)
		throw gcnew System::InvalidOperationException();
}

static void CompressData(
	unsigned char* in, unsigned __int32 inlen, 
	unsigned char* out, unsigned __int32* outlen, int strength)
{
	int ret = compress2(out,(uLongf*)outlen,in,inlen,strength);
		
	if (ret != Z_OK)
		throw gcnew System::InvalidOperationException();
}

static void UncompressData(
	unsigned char* in, unsigned __int32 inlen, 
	unsigned char* out, unsigned __int32* outlen)
{
	int ret = uncompress(out, (uLongf*)outlen, in, inlen);
	
	if (ret != Z_OK)
		throw gcnew System::InvalidOperationException();
}

array<System::Byte>^ ZLib::Compress(
	array<System::Byte>^ in, int offset, int count)
{
	pin_ptr<System::Byte> ptrIn = &(in[offset]);
	unsigned char* tmpin = ptrIn;

	unsigned int outlen = count*2;
	unsigned char* tmpout = new unsigned char[outlen];

	CompressData(tmpin, count, tmpout, &outlen);

	array<System::Byte>^ ret = CreateBuffer(tmpout,outlen);

	delete[] tmpout;

	return ret;
}

array<System::Byte>^ ZLib::Compress(
	array<System::Byte>^ in, int offset, int count, int strength)
{
	pin_ptr<System::Byte> ptrIn = &(in[offset]);
	unsigned char* tmpin = ptrIn;

	unsigned int outlen = count*2;
	unsigned char* tmpout = new unsigned char[outlen];

	CompressData(tmpin, count, tmpout, &outlen, strength);

	array<System::Byte>^ ret = CreateBuffer(tmpout,outlen);

	delete[] tmpout;

	return ret;
}

array<System::Byte>^ ZLib::Uncompress(
	array<System::Byte>^ in, int offset, int count, int realSize)
{
	pin_ptr<System::Byte> ptrIn = &(in[offset]);
	unsigned char* tmpin = ptrIn;

	unsigned char* tmpout = new unsigned char[realSize];
	unsigned int outlen = realSize;

	UncompressData(tmpin, count, tmpout, &outlen);

	array<System::Byte>^ ret = CreateBuffer(tmpout,outlen);

	delete[] tmpout;

	return ret;
}

int ZLib::Compress(
	void* dest, int maxCount,
	const void* src, int count,
	int level)
{
	unsigned long outlen = maxCount;

	if (compress2(
		(Bytef*)dest, &outlen, (Bytef*)src, count, level) == Z_OK)
	{
		// Return the actual size.
		return (int)outlen;
	}

	return -1;
}

int ZLib::Uncompress(
	void* dest, int maxCount,
	const void* src, int count)
{
	uLongf outlen = maxCount;

	if (uncompress(
		(Bytef*)dest,&outlen,(const Bytef*)src,count) == Z_OK)
	{
		return (int)outlen;
	}

	return -1;
}

unsigned __int32 ZLib::Adler32(unsigned __int32 adler, 
							   const void* data, 
							   int count)
{
	return adler32(adler, (const Bytef*)data, count);
}

unsigned __int32 ZLib::CRC32(unsigned __int32 start, 
							 const void* src, 
							 int count)
{	
	return crc32(start, (const Bytef*)src, count);
}


}
}