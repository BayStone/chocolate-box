#include "Stdafx.h"
#include "Level.h"
#include "STBFile.h"

namespace FableMod
{
namespace STB
{

Level::Level() 
: m_pBaseHeader(new STBLevBaseHeader())
, m_pSectionHeader(new STBLevSectionHeader())
, m_pOffsetHeader(new STBLevOffsetHeader())
, m_Flora(gcnew Collection<FableMod::STB::Flora^>())
, m_RootFloraNode(gcnew FloraNode())
{
}

Level::~Level()
{
    delete m_pBaseHeader;
    delete m_pSectionHeader;
    delete m_pOffsetHeader;
}

void Level::Read(FileStream^ file, BBBEntry^ entry)
{
	FileControl::Read(file, m_pBaseHeader, sizeof(*m_pBaseHeader));
	
	//Assert(BaseHeader.Magic == 0x1);
	
	Console::WriteLine("MapSize: {0}, {1}", 
		m_pBaseHeader->MapWidth,
		m_pBaseHeader->MapHeight);
	
	Console::WriteLine("MapOffset: {0}, {1}", 
		m_pBaseHeader->MapWorldOffsetX,
		m_pBaseHeader->MapWorldOffsetY);

	FileControl::Read(file, m_pSectionHeader, sizeof(*m_pSectionHeader));
	
	Console::WriteLine("MapBounds: {0}", 
		m_pSectionHeader->Bounds.ToString());

	//Assert(SectionHeader.Magic == 0xA);

	Byte indicator;
	
	FileControl::Read(file, m_pOffsetHeader, sizeof(*m_pOffsetHeader));

	Console::WriteLine("Test: {0:X}, {1}, {2}, {3}, {4}", 
		m_pOffsetHeader->Magic,
		m_pOffsetHeader->LevTableOffset1,
		m_pOffsetHeader->LevTableOffset2,
		m_pOffsetHeader->TableSize2,
		m_pOffsetHeader->Indicator);

	//Assert(OffsetHeader.Magic == 0x1);
	
	unsigned __int32 len = 0;

	UInt32 sectionSize;
	FileControl::Read(file, &sectionSize, sizeof(sectionSize));
	Console::WriteLine("SectionSize: {0}", sectionSize);

	FileControl::Read(file, &len, sizeof(len));
	Console::WriteLine("start: {0:X}", len);

	Int64 pos = file->Position;

	STBPlacement placement;

	// Center of sorts?
	FileControl::Read(file, &placement, sizeof(placement));
	
	Console::WriteLine("MapPlace: {0}, {1}, {2}, {3}, {4}",
		placement.X,
		placement.Y,
		placement.Z,
		placement.Rotation,
		placement.Unknown);

	UInt32 ui3rdCount;
	FileControl::Read(file, &ui3rdCount, sizeof(ui3rdCount));
	Console::WriteLine("3rd count: {0}", ui3rdCount);

	STBChunk chunk3rd;

	FileControl::Read(file, &chunk3rd, sizeof(chunk3rd));
		
	FileControl::Read(file, &len, sizeof(len));
	//Console::WriteLine("null: {0}", len);

	unsigned __int16 map;

	FileControl::Read(file, &map, sizeof(map));
	Console::WriteLine("mapx: {0}", map);
	FileControl::Read(file, &map, sizeof(map));
	Console::WriteLine("mapy: {0}", map);

	FileControl::Read(file, &indicator, sizeof(indicator));
	
	unsigned __int32 floraCount = 0;
	FileControl::Read(file, &floraCount, sizeof(floraCount));
	Console::WriteLine("FloraCount: {0}", floraCount);

	for (int i = 0; i < floraCount; ++i)
	{
		Flora^ flora = gcnew Flora();

		flora->Read(file);
		
		m_Flora->Add(flora);
	}

	// Read terrain mesh.
	{
		file->Position = 
			entry->FileOffset+
			m_pOffsetHeader->LevTableOffset1;

		ReadTerrain(file, entry);
	}

	/*
	{
		Console::WriteLine("---Table2---");

		STBLevTable2Entry table2Entry;
		table2Entry.ChunkBaseOffset = m_pOffsetHeader->LevTableOffset2; 
		table2Entry.ChunkOffset = 0;

		int tableIdx = 0;

		while (table2Entry.ChunkBaseOffset != 0)
		{
			Console::WriteLine("Table2[{0}]", tableIdx++);
		
			file->Position = 
				entry->FileOffset+
				table2Entry.ChunkBaseOffset+
				table2Entry.ChunkOffset;
	
			FileControl::Read(file, &table2Entry, sizeof(table2Entry));

			Console::WriteLine("  Cells: {0}, {1}, {2}, {3}",
				table2Entry.CellX,
				table2Entry.CellY,
				table2Entry.CellWidth,
				table2Entry.CellHeight);

			Console::WriteLine("  Unknowns: {0}, {1}, {2}",
				table2Entry.Unknown1,
				table2Entry.Unknown2,
				table2Entry.Unknown3);
			
			Console::WriteLine("  Chunk: {0}, {1}, {2}",
				table2Entry.Chunk.BaseOffset,
				table2Entry.Chunk.Size,
				table2Entry.Chunk.Offset);

			Console::WriteLine("  Bounds: {0}, {1}, {2} -> {3}, {4}, {5}", 
				table2Entry.Bounds.MinX,
				table2Entry.Bounds.MinY,
				table2Entry.Bounds.MinZ,
				table2Entry.Bounds.MaxX,
				table2Entry.Bounds.MaxY,
				table2Entry.Bounds.MaxZ);
		}
	}
	*/

	/*
	{
		Console::WriteLine("FloraNodes -----");

		Int64 pos = file->Position;

		file->Position = 
			entry->FileOffset+
			chunk3rd.BaseOffset+
			chunk3rd.Offset;

		m_RootFloraNode->m_CellX = 0;
		m_RootFloraNode->m_CellY = 0;
		m_RootFloraNode->m_CellWidth = m_pBaseHeader->MapWidth;
		m_RootFloraNode->m_CellHeight = m_pBaseHeader->MapHeight;
		m_RootFloraNode->m_Position = Vec3(
			placement.X, placement.Y, placement.Z);

		m_RootFloraNode->ReadChildren(this, " ", file, entry, 5);

		file->Position = pos;
	}
	*/
}

void Level::ReadTerrain(FileStream^ file, BBBEntry^ entry)
{
	m_Blocks = gcnew array<HeightBlock^>(BlockCount);
	
	Console::WriteLine("TerrainBlockCount: {0}", BlockCount);

	for (int i = 0; i < BlockCount; ++i)
	{
		HeightBlock^ block = gcnew HeightBlock();

		block->Read(file, entry);
			
		m_Blocks[i] = block;
	}
}

List<FableMod::STB::Flora^>^ Level::GetFloraOfType(UInt32 type)
{
	List<FableMod::STB::Flora^>^ result =
		gcnew List<FableMod::STB::Flora^>();

	for (int i = 0; i < m_Flora->Count; ++i)
	{
		if (m_Flora[i]->GetHeader()->Type == type)
		{
			result->Add(m_Flora[i]);
		}
	}

	return result;
}

}
}