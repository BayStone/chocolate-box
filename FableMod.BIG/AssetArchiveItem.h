#pragma once

using namespace System;

namespace FableMod
{
namespace BIG
{

ref class AssetArchive;

public ref class AssetArchiveItem
{
public:
	
	property unsigned __int32 SourceStartOffset { unsigned __int32 get(); }

	property unsigned __int32 SourceLength { unsigned __int32 get(); }

	property AssetArchive^ Archive { AssetArchive^ get(); }

protected:

	AssetArchive^		m_Archive;
	unsigned __int32	m_SourceStartOffset;
	unsigned __int32	m_SourceLength;
};

}
}