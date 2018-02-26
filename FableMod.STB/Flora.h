#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;

using namespace FableMod::CLRCore;

#include "STBHeaders.h"

namespace FableMod
{
namespace STB
{

public ref class Flora
{
public:

	Flora();

	void Read(FileStream^ file);

	virtual ~Flora();

	property unsigned __int32 GraphicsID[int]
	{
		unsigned __int32 get(int index) 
		{ 
			return m_pHeader->GraphicsID[index]; 
		}
	}

	property Vec3 Position
	{
		Vec3 get() 
		{ 
			return Vec3(
				m_pHeader->X+m_pHeader->Place.X, 
				m_pHeader->Y+m_pHeader->Place.Y, 
				0.0f); 
		}
	}
	
	property Vec3 Offset
	{
		Vec3 get() 
		{ 
			return Vec3(
				m_pHeader->Place.X, 
				m_pHeader->Place.Y, 
				m_pHeader->Place.Z); 
		}
	}

internal:

	STBFloraHeader* GetHeader()
	{
		return m_pHeader;
	}

protected:

	STBFloraHeader* m_pHeader;
};

}
}