#include "Stdafx.h"
#include "AssetEntryHeader.h"

namespace FableMod
{
namespace BIG
{

AssetEntryHeader::AssetEntryHeader()
{
	memset(this, 0, sizeof(*this));
}

AssetEntryHeader::AssetEntryHeader(AssetEntryHeader& src)
{
	MagicNumber = src.MagicNumber;
	ID = src.ID;
	Type = src.Type;
	Length = src.Length;
	StartOffset = src.StartOffset;
	DevFileType = src.DevFileType;
	DevNameLength = src.DevNameLength;

	DevSymbolName = strdup(src.DevSymbolName);
	
	DevCRC = src.DevCRC;
	DevSourceCount = src.DevSourceCount;
	DevSources = new DevSourceEntry[DevSourceCount];
	
	for(unsigned int i = 0; i < DevSourceCount; ++i)
	{
		DevSources[i].Length = src.DevSources[i].Length;
		DevSources[i].SourceName = strdup(src.DevSources[i].SourceName);
	}

	SubHeaderSize = src.SubHeaderSize;
	SubHeader = new char[SubHeaderSize];
	memcpy(SubHeader, src.SubHeader, SubHeaderSize);
}

AssetEntryHeader::~AssetEntryHeader()
{
	if (DevSymbolName)
		delete[] DevSymbolName;
	
	if (DevSources)
	{
		for(unsigned int i = 0; i < DevSourceCount; ++i)
		{
			if (DevSources[i].SourceName)
				delete[] DevSources[i].SourceName;
		}
		
		delete DevSources;
	}
	
	if (SubHeader)
		delete SubHeader;
}
		
}
}
