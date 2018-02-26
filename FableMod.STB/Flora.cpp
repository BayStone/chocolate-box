#include "Stdafx.h"
#include "Flora.h"
#include "STBFile.h"

using namespace FableMod::CLRCore;

namespace FableMod
{
namespace STB
{

Flora::Flora() :
m_pHeader(new STBFloraHeader())
{
}

Flora::~Flora()
{
	delete m_pHeader;
}

void Flora::Read(FileStream^ file)
{
	FileControl::Read(file, m_pHeader, sizeof(*m_pHeader));
	
	/*
#ifdef _DEBUG
	Console::WriteLine(" Gfx1: {0} {1}", 
		m_pHeader->GraphicsID[0], 
		STBFile::GetGraphicsEntry(m_pHeader->GraphicsID[0]));
	
	Console::WriteLine(" Gfx2: {0} {1}", 
		m_pHeader->GraphicsID[1], 
		STBFile::GetGraphicsEntry(m_pHeader->GraphicsID[1]));

	Console::WriteLine(" Gfx3: {0} {1}", 
		m_pHeader->GraphicsID[2], 
		STBFile::GetGraphicsEntry(m_pHeader->GraphicsID[2]));

	Console::WriteLine(" Pos: {0}, {1}", m_pHeader->X, m_pHeader->Y);
	Console::WriteLine(" Place: {0}, {1}, {2}, {3}, {4}", 
		m_pHeader->Place.X, 
		m_pHeader->Place.Y,
		m_pHeader->Place.Z,
		m_pHeader->Place.Rotation,
		m_pHeader->Place.Unknown);

	Console::WriteLine(" u1: {0}", m_pHeader->Unknown1);
	Console::WriteLine(" ns: {0}", m_pHeader->NumSections);
	Console::WriteLine(" type: {0}", m_pHeader->Type);

	Console::WriteLine(" Unknowns: {0}, {1}, {2}, {3}", 
		m_pHeader->Unknown2, 
		m_pHeader->Unknown3, 
		m_pHeader->Unknown4, 
		m_pHeader->Unknown5);
#endif
	*/
}

}
}