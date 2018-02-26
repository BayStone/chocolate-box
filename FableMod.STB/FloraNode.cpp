#include "Stdafx.h"
#include "FloraNode.h"
#include "STBHeaders.h"
#include "STBFile.h"

using namespace System::Diagnostics;

namespace FableMod
{
namespace STB
{
	
FloraNode::FloraNode()
	: m_CellX(0)
	, m_CellY(0)
	, m_CellWidth(0)
	, m_CellHeight(0)
	, m_Children(gcnew Collection<FloraNode^>())
	, m_Points(gcnew Collection<FloraPoint>())
	, m_Depth(0)
{
}

FloraNode::~FloraNode()
{
}

static void DebugWriteLine(String^ format, ... array<System::Object^>^ args)
{
	Console::WriteLine(format, args);
}

void FloraNode::ReadChildren(
	Level^ level, String^ depth, FileStream^ file, BBBEntry^ entry, UInt32 childCount)
{
	for (UInt32 c = 0; c < childCount; ++c)
	{
		UInt32 sectionCount;

		FileControl::Read(file, &sectionCount, sizeof(sectionCount));

		//DebugWriteLine("{0}Section[{1}] {2}", depth, c, sectionCount);

		if (sectionCount == 0)
			continue;

		FloraNode^ child = gcnew FloraNode();
		
		child->m_Depth = m_Depth+1;

		child->Read(level, depth+" ", file, entry);
		
		m_Children->Add(child);
	}

	//DebugWriteLine("{0}ChildCount: {1}", depth, m_Children->Count);
}

static void ReadPadding(String^ tag, BinaryReader^ reader, int offset, int count)
{
	Int64 pos = reader->BaseStream->Position;
		
	reader->BaseStream->Position += offset;

	for (int i = 0; i < count; ++i)
	{	
		Int64 p = reader->BaseStream->Position;

		if (p+4 > reader->BaseStream->Length)
		{
			Console::WriteLine("End of data at {0}.", i);
			break;
		}

		UInt16 b = reader->ReadUInt32();
		
		reader->BaseStream->Position = p;

		float c = reader->ReadSingle();
		
		Console::WriteLine("{0}[{1}] {2} / {3}", tag, i, b, c);
	}

	reader->BaseStream->Position = pos;
}

void FloraNode::ReadFloraPoints(
	Level^ level, String^ depth, FileStream^ file)
{		
	Vec3 origin = level->MapOrigin;

	float minX = origin.x+CellX;
	float minY = origin.y+CellY;
	float maxX = origin.x+CellX+CellWidth;
	float maxY = origin.y+CellY+CellHeight;

	BinaryReader^ dataReader = STBFile::GetCompressedData(file);

	UInt32 sectionCount = dataReader->ReadUInt32();
	Console::WriteLine("sectionCount: {0}", sectionCount);
		
	bool readInfo = true;
			
	FloraPoint floraPoint;
	
	UInt32 floraIndex = 0;

	for (UInt32 sc = 0; sc < sectionCount; ++sc)
	{
		Console::WriteLine("Section[{0}/{1}]", sc, sectionCount);

		// 0,1,1,1 - normal
		// 0,1,3,0 - weird data
		// 0,0,2,1 - next one seems to be missing numbers #1,2,3
		// 1,0,1,2 - weird ending (where's matrix)

		if (dataReader->BaseStream->Position+4 > dataReader->BaseStream->Length)
		{
			Console::WriteLine("!!!! FLORA ERROR: Version overflow");
			return;
		}

		UInt32 version = dataReader->ReadUInt32();
		Console::WriteLine("Version: {0} HEX:{1:X}", version, version);
			
		if (version > 4)
		{
			Console::WriteLine("!!!! FLORA ERROR: Version error");
			Debug::Assert(false);
			return;
		}

		UInt32 insides = 1;
			
		bool readInfo = version == 0 || sc == 0;

		if (readInfo)
		{
			floraIndex = dataReader->ReadUInt32();
			Console::WriteLine("FloraIndex: {0} HEX:{1}", floraIndex, floraIndex);
		
			if (floraIndex >= level->FloraCount)
			{
				ReadPadding("FI", dataReader, -16, 100);
				Console::WriteLine("!!!! FLORA ERROR: FloraIndex");
				return;
			}
			
			//Debug::Assert(floraIndex < level->FloraCount);
			
			insides = dataReader->ReadUInt32();
			Console::WriteLine("insides: {0}", insides);
		}

		for (UInt32 ic = 0; ic < insides; ++ic)
		{
			Int64 sectionPos = dataReader->BaseStream->Position;
			
			Console::WriteLine("Inside[{0}]", ic);

			UInt32 version3 = 1;

			if (readInfo)
			{
				version3 = dataReader->ReadUInt32();
				Console::WriteLine("Version3: {0}", version3);
			}

			STBFloraDataHeader fdh;
			FileControl::Read(dataReader->BaseStream, &fdh, sizeof(fdh));
		
			Console::WriteLine("PointCount: {0}", fdh.PointCount);

			if (fdh.PointCount > 256)
			{
				//ReadPadding("P", dataReader, -sizeof(fdh)-16, 100);
				Console::WriteLine("!!!! FLORA ERROR: PointCount");
				return;
			}

			//Debug::Assert(fdh.PointCount < 256);

			for (UInt32 i = 0; i < fdh.PointCount; ++i)
			{
				STBFloraPoint1 pt;
				FileControl::Read(dataReader->BaseStream, &pt, sizeof(pt));
				//Console::WriteLine("Pos1 {0} {1} {2} {3}", pt.U1, pt.U2, pt.U3, pt.U4);
			}
		
			for (UInt32 i = 0; i < fdh.PointCount; ++i)
			{
				STBFloraPoint2 pt;
				FileControl::Read(dataReader->BaseStream, &pt, sizeof(pt));
				//Console::WriteLine("Pos2 {0} {1} {2} {3}", pt.X, pt.Y, pt.Z, pt.U);

				int fix = 16;

				if (pt.X < minX-fix || pt.Y < minY-fix || pt.X > maxX+fix || pt.Y > maxY+fix)
				{
					Console::WriteLine("Pos2 {0} {1} {2} {3}", pt.X, pt.Y, pt.Z, pt.U);
					Console::WriteLine("Bounds {0} {1} {2} {3}", minX, minY, maxX, maxY);
					Console::WriteLine("!!!! FLORA ERROR: Point bounds");
					return;
				}

				floraPoint.Position = Vec3(pt.X, pt.Y, pt.Z);
				floraPoint.Rotation = pt.U;
				floraPoint.FloraIndex = floraIndex;
				m_Points->Add(floraPoint);
			}
	
			Int64 sectionEndPos = dataReader->BaseStream->Position;
	
			Byte a = dataReader->ReadByte();
			Byte b = dataReader->ReadByte();
			Byte c = dataReader->ReadByte();
			
			Console::WriteLine("Bytes: {0} {1} {2}", a, b, c);
			
			for (int b = 0; b < 48; ++b)
			{
				Byte by = dataReader->ReadByte();
				//Console::WriteLine("B[{0}]: {1}", b, by);
			}
		
			{
				if (fdh.PointCount == 21)
				{
					ReadPadding("X0", dataReader, 2, 300);
					ReadPadding("X1", dataReader, 1, 300);
				}

				//Debug::Assert(false);

				// Values before matrix:
				// 9: 38*4
			}

			//Int32 padSize = (fdh.PointCount/4)*48;
			
			//dataReader->BaseStream->Position += padSize;
		
			if (dataReader->BaseStream->Position+4 > dataReader->BaseStream->Length)
			{
				Console::WriteLine("!!!! FLORA ERROR: MATRIX OVERFLOW");
				return;
			}

			int matrixCount = dataReader->ReadUInt32();
			
			Console::WriteLine("matrixCount: {0}", matrixCount);

			/*
			if (matrixCount > 32)
			{
				ReadPadding("M", dataReader, 0, -(dataReader->BaseStream->Position-sectionEndPos), 100);
			}
			*/

			//Debug::Assert(matrixCount < 32);

			if (matrixCount > 1000)
			{
				Console::WriteLine("!!!! FLORA ERROR: MATRIX");
				return;
			}

			for (int i = 0; i < matrixCount; ++i)
			{
				STBFloraPointMatrix matrix;
				FileControl::Read(dataReader->BaseStream, &matrix, sizeof(matrix));
				/*
				Console::WriteLine("Mx {0} {1} {2} {3}", matrix.X0, matrix.X1, matrix.X2, matrix.X3);
				Console::WriteLine("My {0} {1} {2} {3}", matrix.Y0, matrix.Y1, matrix.Y2, matrix.Y3);
				Console::WriteLine("Mz {0} {1} {2} {3}", matrix.Z0, matrix.Z1, matrix.Z2, matrix.Z3);
				Console::WriteLine("Mu {0} {1} {2} {3}", matrix.R0, matrix.R1, matrix.R2, matrix.R3);
				Console::WriteLine("Mu {0} {1} {2} {3}", matrix.U0, matrix.U1, matrix.U2, matrix.U3);
				*/
			}

		}
	}

	if (dataReader->BaseStream->Position < dataReader->BaseStream->Length)
	{
		Console::WriteLine("!!!! DATA STILL LEFT {0} {1}",
			dataReader->BaseStream->Position, dataReader->BaseStream->Length);
	}

	Console::WriteLine("Final point count: {0}", m_Points->Count); 

	dataReader->Close();
}

void FloraNode::ReadPoints(
	Level^ level, String^ depth, FileStream^ file, BBBEntry^ entry)
{
	UInt32 count;

	FileControl::Read(file, &count, sizeof(count));
	//DebugWriteLine("{0}Count: {1}", depth, count);

	for (UInt32 j = 0; j < count; ++j)
	{
		DebugWriteLine("{0}Point[{1}]", depth, j);

		STBChunk c;

		FileControl::Read(file, &c, sizeof(c));
			
		String^ depth2 = depth+" ";

		STBPlacement p;
			
		FileControl::Read(file, &p, sizeof(p));
		
		DebugWriteLine("{0}Chunk: {1}, {2}, {3}",
			depth2,
			c.BaseOffset,
			c.Size,
			c.Offset);
			
		/*
		DebugWriteLine("{0}Place: {1}, {2}, {3}, {4}, {5}",
			depth2,
			p.X,
			p.Y,
			p.Z,
			p.Rotation,
			p.Unknown);
		*/

		UInt32 pad;

		FileControl::Read(file, &pad, sizeof(pad));
		DebugWriteLine("{0}pad: {1:X}", depth2, pad);

		Byte pad8;

		FileControl::Read(file, &pad8, sizeof(pad8));
		DebugWriteLine("{0}pad1: {1}", depth2, pad8);
			
		FileControl::Read(file, &pad8, sizeof(pad8));
		DebugWriteLine("{0}pad2: {1}", depth2, pad8);
			
		FileControl::Read(file, &pad8, sizeof(pad8));
		DebugWriteLine("{0}pad3: {1}", depth2, pad8);
			
		FileControl::Read(file, &pad8, sizeof(pad8));
		DebugWriteLine("{0}pad4: {1}", depth2, pad8);

		Int64 filePos = file->Position;
	
		file->Position = 
			entry->FileOffset+c.BaseOffset+c.Offset;

		ReadFloraPoints(level, depth+" ", file);

		file->Position = filePos;
	}
}

void FloraNode::Read(Level^ level, String^ depth, FileStream^ file, BBBEntry^ entry)
{
	STBLevTable3Entry table3Entry;
		
	FileControl::Read(file, &table3Entry, sizeof(table3Entry));

	DebugWriteLine("{0}Place: {1}, {2}, {3}, {4}, {5}",
		depth,
		table3Entry.Place.X,
		table3Entry.Place.Y,
		table3Entry.Place.Z,
		table3Entry.Place.Rotation,
		table3Entry.Place.Unknown);

	DebugWriteLine("{0}SectionCount: {1}",
		depth,
		table3Entry.SectionCount);

	DebugWriteLine("{0}Cells: {1}, {2}, {3}, {4}",
		depth,
		table3Entry.CellX,
		table3Entry.CellY,
		table3Entry.CellWidth,
		table3Entry.CellHeight);
			
	DebugWriteLine("{0}Chunk: {1}, {2}, {3}",
		depth,
		table3Entry.Chunk.BaseOffset,
		table3Entry.Chunk.Size,
		table3Entry.Chunk.Offset);
	
	m_CellX = table3Entry.CellX;
	m_CellY = table3Entry.CellY;
	m_CellWidth = table3Entry.CellWidth;
	m_CellHeight = table3Entry.CellHeight;
	
	m_Position = Vec3(
		table3Entry.Place.X,
		table3Entry.Place.Y,
		table3Entry.Place.Z);

	Int64 pos = file->Position;

	file->Position = 
		entry->FileOffset+
		table3Entry.Chunk.BaseOffset+
		table3Entry.Chunk.Offset;

	if (table3Entry.CellWidth > 16 || table3Entry.CellHeight > 16)
	{
		// Read children.
		ReadChildren(level, depth+" ", file, entry, 5);
	}
	else
	{
		ReadPoints(level, depth+" ", file, entry);
	}

	file->Position = pos;
}

}
}