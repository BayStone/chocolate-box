#pragma once

#pragma pack(push, fablemod_lev_structs_h)
#pragma pack(1)

namespace FableMod
{
namespace LEV
{

struct LEVHeader
{
	unsigned __int32	HeaderSize;
	unsigned __int16	Version;
	unsigned __int8		Pad[3];
	unsigned __int32	Reserved1;
	unsigned __int32	ObsoleteOffset;
	unsigned __int32	Reserved2;
	unsigned __int32	NavOffset;
};

struct LEVMapHeader
{
	unsigned __int8		HeaderSize;
	unsigned __int8		Version;
	unsigned __int8		Pad[3];
	unsigned __int32	UsedIDsLo;
	unsigned __int32	UsedIDsHi;
	unsigned __int32	Width;
	unsigned __int32	Height;
	unsigned __int8		Boolean;
};

struct LEVCellHeader
{
	unsigned __int32	Size;
	unsigned __int8		Version;
	float				Height;
	unsigned __int8		Zero;
	unsigned __int8		Themes[3];
	unsigned __int8		Strength[2];
	unsigned __int8		Walkable;
	unsigned __int8		Camera;
	unsigned __int8		Reserved1;
	unsigned __int8		Reserved2;
	unsigned __int8		Reserved3;
	unsigned __int8		Reserved4;
};

struct LEVSoundCellHeader
{
	unsigned __int32	Size;
	unsigned __int8		Version;
	unsigned __int8		Themes[3];
	unsigned __int8		Strength[2];
	unsigned __int8		Index;
};

struct LEVNavSectionHeader
{
	unsigned __int32 FileSize;
	unsigned __int32 FileVersion;
	float			 Width;
	float			 Height;
	unsigned __int32 NumLevels;
};

struct LEVNavHeader
{
	unsigned __int32 StartOfNav;
	unsigned __int32 NumSections;
};

struct LEVNodeHeader
{
	unsigned char		Layer;      
	unsigned char		Subset;
	float				X;
	float				Y;
	unsigned __int32	ID;                    
};

struct LEVInteractiveNode
{
	float				X;
	float				Y;
	unsigned __int32	Subset;
};

}
}

#pragma pack(pop,fablemod_lev_structs_h)
