#include "stdafx.h"
#include "BINEntry.h"
#include "BINFile.h"

using namespace System::Runtime::InteropServices;
using namespace FableMod::Data;

namespace FableMod
{
namespace BIN
{

BINEntry::BINEntry(BINFile^ bin,
				   String^ definition, 
				   String^ name, 
				   unsigned int nameEnum, 
				   char* data, 
				   int datalen)
{
	m_BIN = bin;
	m_Modified = false;
	m_Name = name;
	m_NameEnum = nameEnum;
	m_Definition = definition;
	m_Data = new char[datalen];
	memcpy(m_Data, data, datalen);
	m_Length = datalen;
}

BINEntry::BINEntry(BINFile^ bin,
				   String^ definition, 
				   String^ name, 
				   char* data, 
				   int datalen)
{
	m_BIN = bin;

	m_Modified = false;
	
	Name = name;
	
	m_Definition = definition;
	
	m_Data = new char[datalen];
	
	memcpy(m_Data, data, datalen);
	
	m_Length = datalen;
}

BINEntry::~BINEntry()
{
	delete[] m_Data;
	m_Length = 0;
	m_Data = 0;

	m_Definition = nullptr;
	m_Name = nullptr;
}

bool BINEntry::IsXBox::get()
{
	return m_BIN->IsXBox;
}

bool BINEntry::Modified::get()
{
	return m_Modified;
}

void BINEntry::Modified::set(bool value)
{
	m_Modified = value;
}

int BINEntry::Length::get()
{
	return m_Length;
}

int BINEntry::ID::get()
{
	return m_BIN->GetEntryIndex(this);
}

unsigned int BINEntry::NameEnum::get()
{
	return m_NameEnum;
}

String^ BINEntry::Definition::get()
{
	return m_Definition;
}

void BINEntry::Definition::set(String^ value)
{
	m_Definition = value;
}

String^ BINEntry::Name::get()
{
	return m_Name;
}

void BINEntry::Name::set(String^ value)
{
	m_Name = value;
	
	IntPtr pname = Marshal::StringToHGlobalAnsi(m_Name);
	
	m_NameEnum = 0xFFFFFFFF-ZLib::CRC32(
		0xFFFFFFFF, pname.ToPointer(), m_Name->Length);
	
	Marshal::FreeHGlobal(pname);
}

array<System::Byte>^ BINEntry::Data::get()
{
	array<System::Byte>^ ret = gcnew array<System::Byte>(Length);
	
	for (int i = 0; i < Length; ++i)
		ret[i] = ((unsigned char*)m_Data)[i];
	
	return ret;
}

void BINEntry::Data::set(array<System::Byte>^ data)
{
	pin_ptr<System::Byte> ptr = &(data[0]);

	SetData((char*)ptr, (unsigned int)data->Length);
}

void BINEntry::SetData(char* data, unsigned int length)
{
	m_Modified = true;
	delete[] m_Data;
	m_Data = new char[length];
	memcpy(m_Data, data, length);
	m_Length = length;
}

char* BINEntry::GetData()
{
	return m_Data;
}

}
}
