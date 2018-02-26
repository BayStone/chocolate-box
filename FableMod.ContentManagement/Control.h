#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Collections::ObjectModel;
using namespace System::Text;

#include "MemberCollection.h"

namespace FableMod
{
namespace ContentManagement
{

ref class ArrayMember;

public ref class Control
{
public:

	Control(Control^ control);
	Control(UInt32 ID, String^ name);
		
	virtual ~Control();

	void Print(StringBuilder^ sb, String^ tab);

	int ReadIn(char* data, int length, bool IsXBox);
	int Write(char* data, int length, bool IsXBox);

	property UInt32 ID
	{
		UInt32 get() { return m_ID; }
	}

	property String^ Name
	{
		String^ get() { return m_Name; }
	}
	
	property MemberCollection^ Members
	{
		MemberCollection^ get() { return m_Members; }
	}

	virtual String^ ToString() override;

private:
	UInt32 m_ID;
	String^ m_Name;
	MemberCollection^ m_Members;
};

}
}