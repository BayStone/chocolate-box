#pragma once

#pragma pack(push,1)

using namespace System;

namespace FableMod
{
namespace BIG
{

struct DevSourceEntry
{
	unsigned __int32 Length;
	char*			 SourceName;
};

struct AssetEntryHeader
{
	unsigned __int32 MagicNumber;
	unsigned __int32 ID;
	unsigned __int32 Type;
	unsigned __int32 Length;
	unsigned __int32 StartOffset;
	//--[dev info]--
	unsigned __int32 DevFileType;
	unsigned __int32 DevNameLength;
	char*			 DevSymbolName;
	unsigned __int32 DevCRC;
	unsigned __int32 DevSourceCount;
	DevSourceEntry*	 DevSources;
	//--[/dev info]--
	unsigned __int32 SubHeaderSize;
	char*			 SubHeader;

	AssetEntryHeader();
	
	AssetEntryHeader(AssetEntryHeader& src);
	
	~AssetEntryHeader();
};
		
}
}

#pragma pack(pop)