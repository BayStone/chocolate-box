#include "Stdafx.h"
#include "Log.h"

namespace FableMod
{
namespace LEV
{

#ifndef LEV_NO_LOG

void Log::Open()
{
	if (++m_iRefs == 1)
	{
		m_Writer = gcnew StreamWriter(
			AppDomain::CurrentDomain->BaseDirectory+"FableMod.LEV.Log");
	}
}

void Log::Close()
{
	if (--m_iRefs == 0)
	{
		m_Writer->Close();
		m_Writer = nullptr;
	}
}

void Log::WriteLine(String^ format, ... array<System::Object^>^ args)
{
	Open();

	if (m_Writer)
	{
		m_Writer->WriteLine(format, args);
	}

	Close();
}

#endif

}
}