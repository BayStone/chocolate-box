#pragma once

using namespace System;

namespace FableMod
{
namespace Common
{

#pragma pack(push, PackedXYZ_H)
#pragma pack(1)

struct PackedXYZ
{
public:

	void SetBits(UInt32 bits)
	{
		val = bits;
	}

	float GetX()
	{
		int x = (val & 0x7FF);
		if (x > 1023)
		{
			x -= 2048;
		}
		return x/1023.0f;
	}

	float GetY()
	{
		int y = (val & 0x3FF800) >> 11;
		if (y > 1023)
		{
			y -= 2048;
		}
		return y/1023.0f;
	}

	float GetZ()
	{
		int z = (val & 0xFFC00000) >> 22;
		if (z > 511)
		{
			z -= 1024;
		}
		return z/511.0f;
	}

	void SetX(float f)
	{
		UInt32 v = (UInt32)(f*1023.0f);
		if ( v < 0)
			v +=2048;
		val = (val & 0xFFFFF800) | v;
	}
	
	void SetY(float f)
	{
		UInt32 v = (UInt32)(f*1023.0f);
		if ( v < 0)
			v +=2048;
		val = (val & 0xFFC007FF) | (v << 11);
	}

	void SetZ(float f)
	{
		UInt32 v = (UInt32)(f*511.0f);
		if ( v < 0)
			v +=1024;
		val = (val & 0x3FFFFF) | (v << 22);
	}

	UInt32 val;
};

#pragma pack(pop, PackedXYZ_H)

}
}