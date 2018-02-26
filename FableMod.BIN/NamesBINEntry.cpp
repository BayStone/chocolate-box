#include "stdafx.h"
#include "NamesBINEntry.h"

using namespace System::Runtime::InteropServices;
using namespace FableMod::Data;

namespace FableMod
{
namespace BIN
{

NamesBINEntry::NamesBINEntry(unsigned int offset, 
							 char* name)
{
	m_Offset = offset;
	m_Enum = 0xFFFFFFFF-ZLib::CRC32(0xFFFFFFFF, name, strlen(name));
	m_Name = gcnew String(name);
}

NamesBINEntry::NamesBINEntry(unsigned int offset,
							 unsigned int enumval,
							 char* name)
{
	m_Offset = offset;
	m_Enum = enumval;
	m_Name = gcnew String(name);
}

NamesBINEntry::NamesBINEntry(unsigned int offset, String^ name)
{
	m_Offset = offset;
	IntPtr pname = Marshal::StringToHGlobalAnsi(name);
	m_Enum = 0xFFFFFFFF-ZLib::CRC32(0xFFFFFFFF, 
		pname.ToPointer(), name->Length);
	Marshal::FreeHGlobal(pname);
	m_Name = name;
}

unsigned int NamesBINEntry::Offset::get()
{
	return m_Offset;
}

unsigned int NamesBINEntry::Enum::get()
{
	return m_Enum;
}

String^ NamesBINEntry::Name::get()
{
	return m_Name;
}

}
}