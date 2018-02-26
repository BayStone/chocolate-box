#pragma once

#include "ModelStructs.h"

using namespace System;

namespace FableMod
{
namespace Gfx
{
namespace Integration
{

public ref class Material
{
public:
	Material(MTRL* mat);

	property unsigned int ID
	{
		unsigned int get();
		void set(unsigned int);
	}
	property String^ Name
	{
		String^ get();
		void set(String^);
	}
	property unsigned int Padding
	{
		unsigned int get();
	}
	property unsigned int BaseTextureID
	{
		unsigned int get();
		void set(unsigned int);
	}
	property unsigned int BumpMapTextureID
	{
		unsigned int get();
		void set(unsigned int);
	}
	property unsigned int ReflectionTextureID
	{
		unsigned int get();
		void set(unsigned int);
	}
	property unsigned int AlphaMapTextureID
	{
		unsigned int get();
		void set(unsigned int);
	}
	property unsigned int TextureFlags
	{
		unsigned int get();
		void set(unsigned int);
	}
	property unsigned int GlowStrength
	{
		unsigned int get();
		void set(unsigned int);
	}
	property unsigned char Unknown2
	{
		unsigned char get();
		void set(unsigned char);
	}
	property unsigned char AlphaEnabled
	{
		unsigned char get();
		void set(unsigned char);
	}
	property unsigned char Unknown3
	{
		unsigned char get();
		void set(unsigned char);
	}
	property unsigned short	Unknown4
	{
		unsigned short get();
		void set(unsigned short);
	}

private:
	
	MTRL* m_Material;
};

}
}
}
