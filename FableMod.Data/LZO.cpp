#include "Stdafx.h"
#include "LZO.h"

namespace FableMod
{
namespace Data
{

array<unsigned __int32,2>^ LZO::FindChunks(unsigned char* data, unsigned __int32 length)
{
	array<unsigned __int32,2>^ ret =  gcnew array<unsigned __int32,2>(0,2);
	for(unsigned __int32 i=0;i<length-7;i++)
	{
		unsigned short soffset = (data[i+1] << 8) + data[i];
		if (soffset == 0xffff)
		{
			unsigned __int32 loffset = (data[i+5] << 24) + (data[i+4] << 16) + (data[i+3] << 8) + data[i+2];
			if (i+6+loffset < length)
			{
				if (data[i+3+loffset] == 0x11 && data[i+3+loffset+1] == 0x00 && data[i+3+loffset+2] == 0x00)
				{
					//add actually decompress and check for error
					array<unsigned __int32,2>^ b = gcnew array<unsigned __int32,2>(ret->GetLength(0)+1,2);
					for(int j=0;j<ret->GetLength(0);j++)
					{
						b[j,0] = ret[j,0];
						b[j,1] = ret[j,1];
					}
					//ret->CopyTo(b, ret->Length);
					b[b->GetLength(0)-1,0] = i;
					b[b->GetLength(0)-1,1] = loffset+6;
					ret = b;
					i += loffset+6;
					
				}
			}
		}
		else if (soffset > 3)
		{
			if (i+2+soffset < length)
			{
				if (data[i+soffset-1] == 0x11 && data[i+soffset] == 0x00 && data[i+soffset+1] == 0x00)
				{
					//add actually decompress and check for error
					array<unsigned __int32,2>^ b = gcnew array<unsigned __int32,2>(ret->GetLength(0)+1,2);
					for(int j=0;j<ret->GetLength(0);j++)
					{
						b[j,0] = ret[j,0];
						b[j,1] = ret[j,1];
					}
					//ret->CopyTo(b, ret->Length);
					b[b->GetLength(0)-1,0] = i;
					b[b->GetLength(0)-1,1] = soffset+2;
					ret = b;
					i += soffset+6;
					
				}
			}
		}
	}
	return ret;
}

array<unsigned __int32,2>^ LZO::FindChunks(array<System::Byte>^ data)
{
	//__crt_dll_initialize();
	lzo_init();
	array<unsigned __int32,2>^ ret =  gcnew array<unsigned __int32,2>(0,2) ;
	for(unsigned __int32 i=0;i<data->Length-7;i++)
	{
		unsigned short soffset = (data[i+1] << 8) + data[i];
		if (soffset == 0xffff)
		{
			unsigned __int32 loffset = (data[i+5] << 24) + (data[i+4] << 16) + (data[i+3] << 8) + data[i+2];
			if (i+6+loffset <= data->Length)
			{
				if (data[i+3+loffset] == 0x11 && data[i+3+loffset+1] == 0x00 && data[i+3+loffset+2] == 0x00)
				{
					try
					{
						DecompressRaw(data,i+6,loffset);
					}
					catch (Exception^ e)
					{
						continue;
					}

					array<unsigned __int32,2>^ b = gcnew array<unsigned __int32,2>(ret->GetLength(0)+1,2);
					for(int j=0;j<ret->GetLength(0);j++)
					{
						b[j,0] = ret[j,0];
						b[j,1] = ret[j,1];
					}
					//ret->CopyTo(b, ret->Length);
					b[b->GetLength(0)-1,0] = i;
					b[b->GetLength(0)-1,1] = loffset+6;
					ret = b;
					i += loffset+6;
					
				}
			}
		}
		else if (soffset > 3)
		{
			if (i+2+soffset <= data->Length)
			{
				if (data[i+soffset-1] == 0x11 && data[i+soffset] == 0x00 && data[i+soffset+1] == 0x00)
				{
					//add actually decompress and check for error
					try
					{
						DecompressRaw(data,i+2,soffset);
					}
					catch (Exception^ e)
					{
						continue;
					}

					array<unsigned __int32,2>^ b = gcnew array<unsigned __int32,2>(ret->GetLength(0)+1,2);
					for(int j=0;j<ret->GetLength(0);j++)
					{
						b[j,0] = ret[j,0];
						b[j,1] = ret[j,1];
					}
					//ret->CopyTo(b, ret->Length);
					b[b->GetLength(0)-1,0] = i;
					b[b->GetLength(0)-1,1] = soffset+2;
					ret = b;
					i += soffset+2;
					
				}
			}
		}
	}
	return ret;
}

void LZO::DecompressRaw(unsigned char* in, 
						unsigned __int32 inlen, 
						unsigned char* out, 
						unsigned __int32* outlen)
{
	lzo_init();
	
	int ret = lzo1x_decompress_safe(
		in, inlen, out, (lzo_uint*)outlen, NULL);
	
	if (ret != 0)
	{
		throw gcnew Exception(String::Format(
			"FableMod::Data::LZO: DecompressRaw failed {0}", ret));
	}
}

array<System::Byte>^ LZO::DecompressRaw(array<System::Byte>^ in, int offset, int count)
{
	//__crt_dll_initialize();
	//lzo_init();
	pin_ptr<System::Byte> pIn = &(in[offset]);
	unsigned char* p = pIn;
	//unsigned char* tmpin = new unsigned char[count];
	//for(int i=0;i<count;i++)
		//tmpin[i] = in[offset+i];
	
	//array<System::Byte>^ out = gcnew array<System::Byte>(count*10)
	unsigned char* tmpout = new unsigned char[count*10]; // ten times should be safe...
	unsigned int outlen = count*10;
	DecompressRaw(p,count,tmpout,&outlen);

	array<System::Byte>^ out = gcnew array<System::Byte>(outlen);
	pin_ptr<System::Byte> pOut = &out[0];
	p = pOut;
	//for(int i=0;i<outlen;i++)
	//	out[i] = tmpout[i];
	memcpy(p, tmpout, outlen); 
	//delete [] tmpin;
	delete [] tmpout;
	return out;
}

void LZO::DecompressChunk(unsigned char* in, unsigned __int32 inlen, unsigned char* out, unsigned __int32* outlen)
{
	unsigned short soffset = (in[1] << 8) + in[0];
	if (soffset == 0xffff)
	{
		unsigned __int32 loffset = (in[5] << 24) + (in[4] << 16) + (in[3] << 8) + in[2];
		
		DecompressRaw(in+ 6, loffset, out, outlen);

		int i;
		for(i=0;i+loffset+6<inlen;i++)
			out[*outlen+i] = in[loffset+6+i];
		*outlen += i;

	}
	else
	{
		if (soffset != 0)
			DecompressRaw(in+ 2, soffset, out, outlen);
		else
			*outlen = 0;

		int i;
		for(i=0;i+soffset+2<inlen;i++)
			out[*outlen+i] = in[soffset+2+i];
		*outlen += i;
	}
}

array<System::Byte>^ LZO::DecompressChunk(array<System::Byte>^ in, int offset, int count)
{
	unsigned short soffset = (in[offset+1] << 8) + in[offset];
	if (soffset == 0xffff)
	{
		unsigned __int32 loffset = (in[offset+5] << 24) + (in[offset+4] << 16) + (in[offset+3] << 8) + in[offset+2];
		
		array<System::Byte>^ d = DecompressRaw(in, offset+6, loffset);
		
		array<System::Byte>^ full = 
			gcnew array<System::Byte>(d->Length + count - loffset - 6);
		
		d->CopyTo(full, 0);
		
		for(int i=0;i+loffset+6<count;i++)
			full[d->Length+i] = in[offset+loffset+6+i];

		delete d;

		return full;
	}
	else
	{
		array<System::Byte>^ d = DecompressRaw(in, offset+2, soffset);
		array<System::Byte>^ full = gcnew array<System::Byte>(d->Length + count - soffset - 2);
		
		d->CopyTo(full, 0);
		
		for(int i=0;i+soffset+2<count;i++)
			full[d->Length+i] = in[offset+soffset+2+i];
		
		delete d;

		return full;
	}

	return nullptr;
}

void LZO::CompressRaw(unsigned char* in, unsigned __int32 inlen, unsigned char* out, unsigned __int32* outlen)
{
	//__crt_dll_initialize();
	lzo_init();
	unsigned char* tmp = new unsigned char[LZO1X_999_MEM_COMPRESS];
	int ret = lzo1x_999_compress(in, inlen, out, (lzo_uint*)outlen, tmp);
	delete tmp;
	if (ret != 0)
		throw gcnew Exception("FableMod::Data::LZO: CompressRaw failed");
}

array<System::Byte>^ LZO::CompressRaw(array<System::Byte>^ in, int offset, int count)
{
	pin_ptr<System::Byte> pIn = &(in[offset]);
	//unsigned char* tmp = new unsigned char[count];

	unsigned char* p = pIn;
	//for(int i=0;i<count;i++)
	//	tmp[i] = in[offset+i];

	unsigned char* out = new unsigned char[count*2];
	unsigned int outlen = count*2;

	CompressRaw(p, count, out, &outlen);

	array<System::Byte>^ ret = gcnew array<System::Byte>(outlen);
	pin_ptr<System::Byte> pOut = &ret[0];
	p = pOut;
	memcpy(p, out, outlen);
	//for(int i=0;i<outlen;i++)
	//	ret[i] = out[i];

	return ret;
}

void LZO::CompressChunk(unsigned char* in, unsigned __int32 inlen, unsigned char* out, unsigned __int32* outlen, int trailcount)
{
	CompressChunk( in,inlen,out,outlen, trailcount, false);
}

void LZO::CompressChunk(unsigned char* in, unsigned __int32 inlen, unsigned char* out, unsigned __int32* outlen, int trailcount, bool forceDwordLength)
{
	unsigned int outlen2 = *outlen - trailcount;
	CompressRaw(in, inlen-trailcount,out+6,&outlen2);
	if (outlen2 >= 0xffff || forceDwordLength)
	{
		*((WORD*)out) = 0xffff;
		*((DWORD*)(out+2)) = outlen2;
		memcpy(out+outlen2+6,in+inlen-trailcount,trailcount);
		*outlen = outlen2+trailcount+6;
	}
	else
	{
		*((WORD*)out) = outlen2;
		memmove(out+2, out+6, outlen2);  // pull it back in
		memcpy(out+outlen2+2,in+inlen-trailcount,trailcount);
		*outlen = outlen2+trailcount+2;
	}
}

array<System::Byte>^ LZO::CompressChunk(array<System::Byte>^ in, int offset, int count, int trailcount)
{
	throw gcnew Exception("FableMod::Data::LZO: Not implemented");
	return nullptr;
}

}
}