#pragma once

namespace FableMod
{
namespace BBB
{

struct BBBHeader
{
	char szBBBB[8];
	
	/// WAD file version?
	unsigned __int32 uiFileVersion;		
	
	/// Sub version?
	unsigned __int32 uiSubVersion;		
	
	/// Size of the blocks read.
	unsigned __int32 uiBlockSize;
	
	/// Number of files in the wad.
	unsigned __int32 uiNumFiles;			
	
	/// Number of files repeated.
	unsigned __int32 uiNumLevels;
	
	/// Start of file header data.
	unsigned __int32 uiListingsOffset;
};

struct BBBFileHeaderA
{
	unsigned __int32 uiMagic;
	
	/// File number in WAD, 1 being the first.
	unsigned __int32 uiFileNo;
	
	/// Always zero?
	unsigned __int32 uiNull1;
	
	/// Total size of the file.
	unsigned __int32 uiSize;
	
	/// Address in the wad.
	unsigned __int32 uiOffset;
	
	/// Always zero?
	unsigned __int32 uiNull2;
	
	/// Size of the name string.
	unsigned __int32 uiNameSize;
};

struct BBBDevHeader
{
	unsigned __int32 uiUnknown1;
	unsigned __int32 uiNull;
	unsigned __int32 uiLevels;	
};

}
}

