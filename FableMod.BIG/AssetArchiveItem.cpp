#include "stdafx.h"
#include "AssetArchive.h"
#include "AssetArchiveItem.h"

namespace FableMod
{
namespace BIG
{
		
unsigned __int32 AssetArchiveItem::SourceStartOffset::get()
{
	return m_SourceStartOffset;
}

unsigned __int32 AssetArchiveItem::SourceLength::get()
{
	return m_SourceLength;
}

AssetArchive^ AssetArchiveItem::Archive::get()
{
	return m_Archive;
}

}
}