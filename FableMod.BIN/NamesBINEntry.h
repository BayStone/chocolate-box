#pragma once

using namespace System;
using namespace System::Collections;
using namespace System::IO;

namespace FableMod
{
namespace BIN
{

public ref class NamesBINEntry
{
public:
	
	property unsigned int Offset { unsigned int get(); }
	
	property unsigned int Enum { unsigned int get(); }
	
	property String^ Name { String^ get(); }

internal:

	NamesBINEntry(unsigned int offset, char* name);

	NamesBINEntry(unsigned int offset,unsigned int enumval,  char* name);

	NamesBINEntry(unsigned int offset, String^ name);

private:
	unsigned int		m_Offset;
	unsigned int		m_Enum;
	String^				m_Name;
};

}
}
