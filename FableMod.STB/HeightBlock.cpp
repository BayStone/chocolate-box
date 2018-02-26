#include "Stdafx.h"
#include "HeightBlock.h"
#include "STBHeaders.h"
#include "STBFile.h"

namespace FableMod
{
namespace STB
{
	
HeightBlock::HeightBlock()
	: m_Min(0.0f, 0.0f, 0.0f)
	, m_Max(0.0f, 0.0f, 0.0f)
{
}

HeightBlock::~HeightBlock()
{
}

void HeightBlock::Read(FileStream^ file, BBBEntry^ entry)
{
	STBTerrainBlockHeader header;
		
	FileControl::Read(file, &header, sizeof(header));

	Console::WriteLine(" Offset: {0}", header.Offset);
	Console::WriteLine(" CompressedSize: {0}", header.CompressedSize);
	Console::WriteLine(" Bounds: {0}", header.Bounds.ToString());
	Console::WriteLine(" Unknown: {0:X}", header.Unknown);

	m_Min = Vec3(
		header.Bounds.MinX,
		header.Bounds.MinY,
		header.Bounds.MinZ);
	
	m_Max = Vec3(
		header.Bounds.MaxX,
		header.Bounds.MaxY,
		header.Bounds.MaxZ);
	
	Int64 filePos = file->Position;

	file->Position =
		entry->FileOffset+
		header.Offset;
	
	BinaryReader^ dataReader = STBFile::GetCompressedData(file);

	UInt16 meshCount = dataReader->ReadUInt16();
	
	m_Meshes = gcnew array<HeightMesh^>(meshCount);

	Console::WriteLine("MeshCount {0}", meshCount);

	for (UInt16 i = 0; i < meshCount; ++i)
	{
		m_Meshes[i] = gcnew HeightMesh();
	}

	for (UInt16 i = 0; i < meshCount; ++i)
	{
		if (!m_Meshes[i]->Read(dataReader, m_Min, m_Max))
		{
			// Can't continue.
			break;
		}
	}

	dataReader->Close();

	file->Position = filePos;
}

}
}