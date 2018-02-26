#pragma once

using namespace System;
using namespace System::Collections;
using namespace System::IO;

namespace FableMod
{
namespace BIN
{

ref class BINFile;

public ref class BINEntry
{
public:

	~BINEntry();
	
	property bool Modified 
	{ 
		bool get(); 
		void set(bool value);
	}
	
	property int Length { int get(); }
	
	property bool IsXBox { bool get(); }
	
	property unsigned int NameEnum { unsigned int get(); }

	property int ID { int get(); }
	
	property String^ Definition
	{
		String^ get();
		void set(String^ value);
	}
	
	property String^ Name
	{
		String^ get();
		void set(String^ value);
	}

	property array<System::Byte>^ Data
	{
		array<System::Byte>^ get();
		void set(array<System::Byte>^ value);
	}

	void SetData(char* data, unsigned int length);
	
	char* GetData();

internal:

	BINEntry(
		BINFile^ bin,
		String^ definition, 
		String^ name, 
		unsigned int nameEnum, 
		char* data, 
		int datalen);
	
	BINEntry(
		BINFile^ bin,
		String^ definition, 
		String^ name, 
		char* data, 
		int datalen);
	
private:

	bool			m_Modified;
	unsigned int	m_NameEnum;
	char*			m_Data;
	int				m_Length;
	String^			m_Definition;
	String^			m_Name;
	BINFile^		m_BIN;
};

}
}