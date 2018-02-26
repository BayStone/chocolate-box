#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;
using namespace FableMod::CLRCore;

#include "Level.h"

namespace FableMod
{
namespace STB
{

/// <summary>
/// Fable STB file loader.
/// </summary>
public ref class STBFile : public FableMod::BBB::BBBFile
{
public:

	STBFile();

	virtual ~STBFile();

	Level^ GetLevel(String^ name);

internal:

	static String^ GetGraphicsEntry(UInt32 id);
	static String^ GetTextureEntry(UInt32 id);

	static BinaryReader^ GetCompressedData(FileStream^ file);

protected:

	void CheckOffsets();

	Dictionary<String^, UInt32>^ m_Offsets;
    Dictionary<UInt32, Level^>^ m_Levels;
};

}
}