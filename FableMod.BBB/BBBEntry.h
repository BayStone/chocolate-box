#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;
using namespace FableMod::CLRCore;

#include "BBBHeaders.h"

namespace FableMod
{
namespace BBB
{

public ref class BBBEntry
{
public:

	BBBEntry();

	virtual ~BBBEntry();

	void ReadHeader(FileStream^ file);

	property String^ FileName { String^ get() { return m_FileName; } }

	property unsigned __int32 FileOffset 
	{ 
		unsigned __int32 get() { return m_Header->uiOffset; } 
	}

	property unsigned __int32 FileSize
	{ 
		unsigned __int32 get() { return m_Header->uiSize; } 
	}

protected:

	virtual void ReadHeaderExtra(
		FileStream^ file, unsigned __int32 size);

	String^ m_FileName;
	String^ m_DevFileName;
	unsigned __int32 m_HeaderOffset;
	BBBFileHeaderA* m_Header;
};

}
}