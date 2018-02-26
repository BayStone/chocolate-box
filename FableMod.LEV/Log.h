#pragma once

using namespace System;
using namespace System::IO;
using namespace FableMod::CLRCore;

namespace FableMod
{
namespace LEV
{

#ifndef LEV_NO_LOG

private ref class Log
{
public:

	static void Open();

	static void Close();

	static void WriteLine(String^ format, ... array<System::Object^>^ args);

private:

	static int m_iRefs = 0;
	static TextWriter^ m_Writer = nullptr;
};

#endif

}
}