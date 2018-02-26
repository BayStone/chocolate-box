#include "Stdafx.h"
#include "STBFile.h"
#include "STBHeaders.h"
#include "Flora.h"

using System::Diagnostics::Debug;
using namespace FableMod::BBB;

namespace FableMod
{
namespace STB
{

STBFile::STBFile() 
: m_Offsets(nullptr)
, m_Levels(gcnew Dictionary<UInt32, Level^>())
{
}

STBFile::~STBFile()
{
}

void STBFile::CheckOffsets()
{
	if (!m_Offsets)
	{
		m_Offsets = gcnew Dictionary<String^, UInt32>();

		BBBEntry^ entry = Entries["__STATIC_MAP_COMMON_HEADER__"];

		File->Position = entry->FileOffset;

		BinaryReader^ reader = gcnew BinaryReader(File);

		UInt32 numEntries = reader->ReadUInt32();

		char szTmp[256];
		int index = 0;

		for (UInt32 i = 0; i < numEntries; ++i)
		{
			char c = 0;

			index = 0;

			do
			{
				c = reader->ReadByte();
				szTmp[index++] = c;
			}
			while (c != 0);
		
			String^ name = (gcnew String(szTmp))->ToLower();

			UInt32 offset = entry->FileOffset+reader->ReadUInt32();

			//Console::WriteLine("{0} {1}", name, offset);

			m_Offsets[name] = offset;
		}

		reader = nullptr;
	}
}

Level^ STBFile::GetLevel(String^ name)
{
	CheckOffsets();

    String^ lowerName = name->ToLower();
    
	UInt32 offset = 0;

	if (!m_Offsets->TryGetValue(lowerName, offset))
	{
		Console::WriteLine("STB: Level {0} not found", name);
		return nullptr;
	}

	Level^ level = nullptr;
    
    if (!m_Levels->TryGetValue(offset, level))
	{
        File->Position = offset;

	    level = gcnew Level();

		BBBEntry^ levelEntry = Entries[name];

        level->Read(File, levelEntry);
    
        m_Levels[offset] = level;
    }
    
	return level;
}

String^ STBFile::GetTextureEntry(UInt32 id)
{
	FableMod::ContentManagement::ContentObject^ o =
		FableMod::ContentManagement::ContentManager::Instance->FindEntry(
			FableMod::ContentManagement::LinkDestination::MainTextureID,
			id);

	if (o)
	{
		return o->Name;
	}

	return gcnew String("NONE");
}

String^ STBFile::GetGraphicsEntry(UInt32 id)
{
	FableMod::ContentManagement::ContentObject^ o =
		FableMod::ContentManagement::ContentManager::Instance->FindEntry(
			FableMod::ContentManagement::LinkDestination::ModelID,
			id);

	if (o)
	{
		return o->Name;
	}

	return gcnew String("NONE");
}

BinaryReader^ STBFile::GetCompressedData(FileStream^ file)
{
	UInt32 uncompressedSize;
	UInt32 compressedSize;

	FileControl::Read(file, &uncompressedSize, sizeof(uncompressedSize));
	FileControl::Read(file, &compressedSize, sizeof(compressedSize));
		
	Debug::Assert(uncompressedSize > compressedSize);
	Debug::Assert(uncompressedSize > 0);
	Debug::Assert(compressedSize > 0);

	array<System::Byte>^ compressedData = gcnew array<System::Byte>(compressedSize);
	file->Read(compressedData, 0, compressedData->Length);

	array<System::Byte>^ uncompressedData = 
		::FableMod::Data::LZO::DecompressRaw(
			compressedData,
			0,
			compressedData->Length);

	Debug::Assert(uncompressedData->Length == uncompressedSize);

	MemoryStream^ dataStream = gcnew MemoryStream(
		uncompressedData, 0, uncompressedData->Length, false);

	BinaryReader^ dataReader = gcnew BinaryReader(dataStream);

	return dataReader;
}

}
}