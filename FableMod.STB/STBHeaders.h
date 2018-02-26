#pragma once

#include "Common/PackedXYZ.h"

using namespace System;
using namespace FableMod::Common;

namespace FableMod
{
namespace STB
{

#pragma pack(push, STBHeaders_H)
#pragma pack(1)

struct STBLevBaseHeader
{
	unsigned __int32 Magic;
	unsigned __int32 ID;
	unsigned __int32 Unknown;
	unsigned __int32 MapWidth;
	unsigned __int32 MapHeight;
	unsigned __int32 MapWorldOffsetX;
	unsigned __int32 MapWorldOffsetY;
	unsigned __int32 Null;
	unsigned __int32 OffsetPush1;
	unsigned __int32 OffsetPush2;
};

struct STBFloraPointMatrix
{
	float X0,X1,X2,X3;
	float Y0,Y1,Y2,Y3;
	float Z0,Z1,Z2,Z3;
	float R0,R1,R2,R3;
	UInt32 U0,U1,U2,U3;
};

struct STBPoint
{
	float X;
	float Y;
	float Z;
};

struct STBFloraPoint1
{
	float U1;
	float U2;
	float U3;
	float U4;
};

struct STBFloraPoint2
{
	float X;
	float Y;
	float Z;
	float U;
};

struct STBFloraDataHeader
{
	STBPoint Point1;
	STBPoint Point2;
	STBPoint Point3;
	float Unknown1;
	UInt32 PointCount;
	float Unknown2;
};

struct STBPlacement
{
	float X;
	float Y;
	float Z;
	float Rotation;
	float Unknown;
};

struct STBBounds
{
	float MinX;
	float MinY;
	float MinZ;
	float MaxX;
	float MaxY;
	float MaxZ;

public:

	String^ ToString()
	{
		return String::Format("{0}, {1}, {2} -> {3}, {4}, {5}", 
			MinX,
			MinY,
			MinZ,
			MaxX,
			MaxY,
			MaxZ);
	}
};

struct STBChunk
{
	UInt32 BaseOffset;
	UInt32 Size;
	UInt32 Offset;
};

struct STBLevSectionHeader
{
	unsigned __int32 Magic;
	unsigned __int32 Unknown1;
	unsigned __int32 Unknown2;
	unsigned __int32 Checksum;
	unsigned __int32 Unknown3;
	unsigned __int32 Unknown4;
	STBBounds Bounds;
	unsigned __int32 OffsetPush1;
	unsigned __int32 OffsetPush2;
};

struct STBTerrainBlockHeader
{
	UInt32 Offset;
	UInt32 CompressedSize;
	STBBounds Bounds;
	UInt32 Unknown;
};

struct STBTerrainMeshHeader
{
	UInt16 VertexCount;
	UInt16 FaceCount;
	Byte Unknown1;
	UInt32 Texture1;
	UInt32 Texture2;
	UInt32 Texture3;
	UInt32 Unknown2;
	UInt32 Unknown3;
	UInt32 Unknown4;
	Byte Unknown5;
};

struct STBLevVertex
{
	UInt16 X;
	UInt16 Y;
	float Z;
	PackedXYZ Normal;
	Byte U;
	Byte V;
	Byte V2;
};

struct STBLevTable2Entry
{
	UInt16 CellX;
	UInt16 CellY;
	UInt16 CellWidth;
	UInt16 CellHeight;
	Byte Unknown1;
	Byte Unknown2;
	Byte Unknown3;
	STBChunk Chunk;
	STBBounds Bounds;
};

struct STBLevTable3Entry
{
	STBPlacement Place;
	UInt32 SectionCount;
	STBChunk Chunk;
	UInt16 CellX;
	UInt16 CellY;
	UInt16 CellWidth;
	UInt16 CellHeight;
};

struct STBLevOffsetHeader
{
	unsigned __int32 Magic;
	unsigned __int32 LevTableOffset1;
	unsigned __int32 LevTableOffset2;
	unsigned __int32 TableSize2;
	unsigned char Indicator;
	unsigned __int32 OffsetPush;
};

struct STBFloraHeader
{
	unsigned __int32 GraphicsID[3];
	float X;
	float Y;
	unsigned __int32 Unknown1;
	unsigned __int32 Null;
	STBPlacement Place;
	unsigned __int32 NumSections;
	unsigned __int32 Type;
	unsigned char Unknown2;
	unsigned char Unknown3;
	unsigned char Unknown4;
	unsigned char Unknown5;
	unsigned __int32 Unknown6;
};

#pragma pack(pop, STBHeaders_H)

}
}