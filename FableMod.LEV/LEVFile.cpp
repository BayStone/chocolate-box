#include "Stdafx.h"
#include <memory.h>
#include <string.h>
#include "Structs.h"
#include "NavigationNode.h"
#include "NavNode.h"
#include "LEVFile.h"
#include "Log.h"

//#define LEV_NO_LOG

using namespace System::Runtime::InteropServices;

namespace FableMod
{
namespace LEV
{

static const int PALETTE_SIZE = 33792;
static const int THEME_COUNT = PALETTE_SIZE/(128+4);

LEVFile::LEVFile() :
m_pucPalette2(0),
m_aCells(0),
m_aSoundCells(0),
m_pucNavData(0),
m_pucData(0),
m_Modified(false),
m_apszGroundThemeNames(0),
m_auiGroundThemeValues(0),
m_pHeader(new LEVHeader),
m_pMapHeader(new LEVMapHeader),
m_Sections(gcnew List<NavSection^>())
{
	m_pMapHeader->Width = 0;
	m_pMapHeader->Height = 0;
}

//-----------------------------------------------------------------------------

LEVFile::~LEVFile()
{
	Destroy();
	this->!LEVFile();
}

//-----------------------------------------------------------------------------

LEVFile::!LEVFile()
{
}

//-----------------------------------------------------------------------------

void LEVFile::Destroy()
{
	if (m_aCells)
	{
		delete[] m_aCells;
		m_aCells = 0;
	}

	if (m_aSoundCells)
	{
		delete[] m_aSoundCells;
		m_aCells = 0;
	}

	if (m_pucData)
	{
		delete[] m_pucData;
		m_pucData = 0;
	}

	if (m_pucNavData)
	{
		delete[] m_pucNavData;
		m_pucNavData = 0;
	}

	if (m_pHeader)
	{
		delete m_pHeader;
		m_pHeader = 0;
	}

	if (m_pMapHeader)
	{
		delete m_pMapHeader;
		m_pMapHeader = 0;
	}

	if (m_pucPalette2)
	{
		delete[] m_pucPalette2;
		m_pucPalette2 = 0;
	}

	if (m_Sections != nullptr)
	{
		for (int i = 0; i < m_Sections->Count; ++i)
			delete m_Sections[i];
		
		delete m_Sections;

		m_Sections = nullptr;
	}

	m_Themes = nullptr;

	if (m_apszGroundThemeNames)
	{
		for (int i = 0; i < THEME_COUNT; ++i)
			delete[] m_apszGroundThemeNames[i];
		
		delete[] m_apszGroundThemeNames;
		delete[] m_auiGroundThemeValues;

		m_apszGroundThemeNames = 0;
		m_auiGroundThemeValues = 0;
	}

	if (m_File != nullptr)
	{
		delete m_File;
		m_File = nullptr;
	}
}

//-----------------------------------------------------------------------------

void LEVFile::Open(String^ filename,
				   ProgressInterface^ progress)
{
	FileStream^ file = File::Open(
		filename, 
		FileMode::Open, 
		FileAccess::ReadWrite, 
		FileShare::Read);

#ifndef LEV_NO_LOG 
	Log::Open();
	Log::WriteLine("Reading file {0}", filename);
#endif

	m_File = gcnew BufferStream(file);

	// Close the actual file. We keep the buffer.
	file->Close();
	delete file;

	m_File->Read(m_pHeader, sizeof(*m_pHeader));
	
	if (m_pHeader->Version != 6404)
	{
		throw gcnew Exception(
			gcnew String("FABLEMOD_LEV_INVALID_VERSION"));
	}


#ifndef LEV_NO_LOG 
	Log::WriteLine("HeaderSize: {0}", m_pHeader->HeaderSize);
	Log::WriteLine("Version: {0}", m_pHeader->Version);
	Log::WriteLine("Pad: {0}, {1}, {2}", 
		m_pHeader->Pad[0],
		m_pHeader->Pad[1],
		m_pHeader->Pad[2]);
	Log::WriteLine("Reserved1: {0}", m_pHeader->Reserved1);
	Log::WriteLine("ObsOffset: {0}", m_pHeader->ObsoleteOffset);
	Log::WriteLine("Reserved2: {0}", m_pHeader->Reserved2);
	Log::WriteLine("NavOffset: {0}", m_pHeader->NavOffset);
#endif

	m_File->Read(m_pMapHeader, sizeof(*m_pMapHeader));
	
	if (m_pMapHeader->Version != 8)
	{	
		throw gcnew Exception(
			gcnew String("FABLEMOD_LEV_INVALID_MAP_VERSION"));
	}

	if (!(m_pMapHeader->Pad[2] == 8 || m_pMapHeader->Pad[2] == 9))
	{	
		throw gcnew Exception(
			gcnew String("FABLEMOD_LEV_INVALID_MAP_PAD"));
	}


#ifndef LEV_NO_LOG 
	Log::WriteLine("MapHeaderSize: {0}", 
		m_pMapHeader->HeaderSize);
	Log::WriteLine("MapVersion: {0}", 
		m_pMapHeader->Version);
	Log::WriteLine("MapPad: {0}, {1}, {2}", 
		m_pMapHeader->Pad[0],
		m_pMapHeader->Pad[1],
		m_pMapHeader->Pad[2]);
	Log::WriteLine("UsedID Lo, Hi: {0}, {1}", 
		m_pMapHeader->UsedIDsLo, m_pMapHeader->UsedIDsHi);
	Log::WriteLine("MapSize: {0}x{1}", 
		m_pMapHeader->Width, m_pMapHeader->Height);
	Log::WriteLine("Boolean: {0}", 
		m_pMapHeader->Boolean);
#endif

	m_apszGroundThemeNames = new char*[THEME_COUNT];
	m_auiGroundThemeValues = new unsigned __int32[THEME_COUNT];

	for (unsigned __int32 i = 0; i < THEME_COUNT; ++i)
	{
		m_apszGroundThemeNames[i] = new char[128];

		m_File->Read(m_apszGroundThemeNames[i], 128);

		m_auiGroundThemeValues[i] = m_File->ReadUInt32();
	}

	unsigned __int32 uiVersion, uiNumThemes;
	
	m_File->Read(&uiVersion, sizeof(uiVersion));
	m_File->Read(&uiNumThemes, sizeof(uiNumThemes));
	
	m_uiVersion = uiVersion;

#ifndef LEV_NO_LOG 
	Log::WriteLine("Version: {0}", uiVersion);
#endif

	m_pucPalette2 = new unsigned char[PALETTE_SIZE];

	m_File->Read(m_pucPalette2, PALETTE_SIZE);
	
	if (m_pMapHeader->Pad[2] == 9)
	{
		unsigned __int32 uiCRC = 0;
		m_File->Read(&uiCRC, sizeof(uiCRC));
	}
	
	if (uiNumThemes > 0)
	{
		// Load sound themes.

		m_Themes = gcnew array<String^>(uiNumThemes-1);
			
		char* pszTmp = new char[512];

		for (unsigned __int32 i = 0; i < uiNumThemes-1; ++i)
		{
			unsigned __int32 uiLen = 0;

			m_File->Read(&uiLen, sizeof(uiLen));
		
			m_File->Read(pszTmp, uiLen);

			pszTmp[uiLen] = '\0';

			m_Themes[i] = gcnew String(pszTmp);
		}

		delete[] pszTmp;
	}

	m_uiCellsOffset = m_File->Tell();

	const unsigned uiNumCells = 
		(m_pMapHeader->Width+1)*(m_pMapHeader->Height+1);

	m_aCells = new LEVCellHeader[uiNumCells];

#ifndef LEV_NO_LOG 
	Log::WriteLine("CellsOffset: {0}", m_File->Tell());
#endif

	m_File->Read(m_aCells, sizeof(LEVCellHeader)*uiNumCells);

#ifndef LEV_NO_LOG 
	Log::WriteLine("AfterCellsOffset: {0}", m_File->Tell());
#endif
	
	/*
	const unsigned int uiNumSoundCells = 
		m_pMapHeader->Width*m_pMapHeader->Height;

	m_aSoundCells = new LEVSoundCellHeader[uiNumSoundCells];

	m_File->Read(m_aSoundCells, 
		sizeof(LEVSoundCellHeader)*uiNumSoundCells);
	*/

	m_uiDataSize = m_pHeader->NavOffset-m_File->Tell();
	m_pucData = new unsigned char[m_uiDataSize];
	m_File->Read(m_pucData, m_uiDataSize);

#ifndef LEV_NO_LOG 
	Log::WriteLine("UnknownDataSize: {0}", m_uiDataSize);
#endif

	LoadNavigation();

	m_FileName = filename;

#ifndef LEV_NO_LOG
	Log::Close();
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// Load navigation information into memory. 
////////////////////////////////////////////////////////////////////////////////
void LEVFile::LoadNavigation()
{
	if (m_File != nullptr && m_File->IsOpen())
	{
		ReadNavigation(m_File);

		m_File->Close();
		delete m_File;
		m_File = nullptr;
	}
}

//-----------------------------------------------------------------------------

void LEVFile::ReadNavigation(BufferStream^ fileIn)
{
	LEVNavHeader NavHeader;

	fileIn->Seek(m_pHeader->NavOffset);

	// Read to data buffer, needed for saving.
	m_uiNavDataSize = fileIn->GetSize()-m_pHeader->NavOffset;
	m_pucNavData = new unsigned char[m_uiNavDataSize];
	fileIn->Read(m_pucNavData, m_uiNavDataSize);

	fileIn->Seek(m_pHeader->NavOffset);

	fileIn->Read(&NavHeader, sizeof(NavHeader));

#ifndef LEV_NO_LOG 
	Log::Open();
	Log::WriteLine("Reading navigation data...");
	Log::WriteLine("NumNavSections: {0}", NavHeader.NumSections);
#endif

	char* pszName = new char[256];

	for (unsigned i = 0; i < NavHeader.NumSections; ++i)
	{
		// Read the section name.

		unsigned __int32 uiLen = 0;
		fileIn->Read(&uiLen, sizeof(uiLen));
		fileIn->Read(pszName, uiLen);
		pszName[uiLen] = '\0';

		NavSection^ pSection = gcnew NavSection();

		pSection->Name = gcnew String(pszName);
			
	#ifndef LEV_NO_LOG 
		Log::WriteLine(" [{0}] Name: {1}", i, pSection->Name);
	#endif

		// Now, jump to where the actual section data is...

		unsigned int uiSectionOffset = 0;
		fileIn->Read(&uiSectionOffset, sizeof(uiSectionOffset));
	
	#ifndef LEV_NO_LOG 
		Log::WriteLine("SectionOffset[{0}] = {1}",
			i, uiSectionOffset);
	#endif
		
		// Save this position.
		unsigned int uiCurrentPos = fileIn->Tell();

		fileIn->Seek(uiSectionOffset);
	
		if (!pSection->Load(fileIn))
		{
			delete[] pszName;
			delete pSection;
			
			throw gcnew Exception(
				gcnew String("FABLEMOD_LEV_SECTION_LOAD_FAILED"));
		}

	#ifndef LEV_NO_LOG 
		Log::WriteLine("SectionDataSize[{0}] = {1}",
			i, fileIn->Tell()-uiSectionOffset);
	#endif
		
		fileIn->Seek(uiCurrentPos);
		
		m_Sections->Add(pSection);
	}

	delete[] pszName;

	if (m_Sections->Count == 0)
	{
		NavSection^ pSection = gcnew NavSection();

		pSection->Name = "NULL";
		pSection->Width = Width;
		pSection->Height = Height;
		
		m_Sections->Add(pSection);
	}

#ifndef LEV_NO_LOG 
	Log::Close();
#endif
}

void LEVFile::Save(String^ filename,
				   ProgressInterface^ progress)
{
	// Make sure the nav data is loaded.
	LoadNavigation();

	FileStream^ file = File::Create(filename);

	if (progress)
		progress->Begin(2);

	FileControl::Write(file, m_pHeader, sizeof(*m_pHeader));
	FileControl::Write(file, m_pMapHeader, sizeof(*m_pMapHeader));
	
	for (unsigned __int32 i = 0; i < THEME_COUNT; ++i)
	{
		FileControl::Write(file, m_apszGroundThemeNames[i], 128);
		unsigned int uiValue = m_auiGroundThemeValues[i];
		FileControl::Write(file, &uiValue, sizeof(uiValue));
	}
	
	unsigned __int32 uiVersion = m_uiVersion;
	FileControl::Write(file, &uiVersion, sizeof(uiVersion));
	
	unsigned __int32 NumThemes = m_Themes->Length+1;
	FileControl::Write(file, &NumThemes, sizeof(NumThemes));
	
	FileControl::Write(file, m_pucPalette2, PALETTE_SIZE);
	
	if (m_pMapHeader->Pad[2] == 9)
	{
		// Don't need the CRC, the game doesn't check it.
		unsigned __int32 crc = 0;
		FileControl::Write(file, &crc, sizeof(crc));
	}

	for (int i = 0; i < m_Themes->Length; ++i)
	{
		IntPtr ptr = Marshal::StringToHGlobalAnsi(m_Themes[i]);
		
		char* pszTmp = (char*)ptr.ToPointer();

		unsigned __int32 len = strlen(pszTmp);

		FileControl::Write(file, &len, sizeof(len));
		FileControl::Write(file, pszTmp, len);
	
		Marshal::FreeHGlobal(ptr);
	}

	const unsigned uiNumCells = 
		(m_pMapHeader->Width+1)*(m_pMapHeader->Height+1);

	FileControl::Write(file, m_aCells, sizeof(LEVCellHeader)*uiNumCells); 

	/*
	const unsigned uiNumSoundCells = 
		m_pMapHeader->Width*m_pMapHeader->Height;

	FileControl::Write(file, m_aSoundCells, 
		sizeof(LEVSoundCellHeader)*uiNumSoundCells); 
	*/
	FileControl::Write(file, m_pucData, m_uiDataSize); 

	LEVHeader Header = *m_pHeader;

	//Header.ObsoleteOffset = file->Position;
	Header.NavOffset = file->Position;

	if (progress)
		progress->Update();

	FileControl::Write(file, m_pucNavData, m_uiNavDataSize); 

	//WriteNavigation(file, progress);

	file->Position = 0;

	FileControl::Write(file, &Header, sizeof(Header));
	
	file->Close();
	
	if (progress)
		progress->End();
}

//-----------------------------------------------------------------------------

void LEVFile::ResetSections()
{
	m_Sections->Clear();

	NavSection^ pSection = gcnew NavSection();

	pSection->Name = "NULL";
	pSection->Width = Width;
	pSection->Height = Height;
	
	m_Sections->Add(pSection);

	pSection->CreateGrids();
}

//-----------------------------------------------------------------------------

void LEVFile::WriteNavigation(FileStream^ fileOut,
							  ProgressInterface^ progress)
{
	if (progress)
		progress->Begin(3);

	unsigned __int32 uiNumNodes = 0;

	if (progress)
		progress->Begin(SectionCount);

	for (int i = 0; i < SectionCount; ++i)
	{
		Sections[i]->CreateNodes();
		uiNumNodes += Sections[i]->NodeCount;
	
		if (progress)
			progress->Update();
	}

	if (progress)
		progress->End();

	LEVNavHeader NavHeader;

	NavHeader.StartOfNav = 0;
	NavHeader.NumSections = 0;

	if (uiNumNodes == 0)
	{
		NavHeader.StartOfNav = 
			(unsigned __int32)fileOut->Position+sizeof(NavHeader);
		
		FileControl::Write(fileOut, &NavHeader, sizeof(NavHeader));
	
		if (progress)
			progress->Update();
	}
	else
	{
		if (progress)
			progress->Begin(3);
		
		unsigned __int32 uiNumLevels = 0;
	
		unsigned int i;

		for (i = 0; i < SectionCount; ++i)
		{
			NavSection^ pSection = Sections[i];

			if (pSection->NodeCount > 0)
			{
				for (unsigned int j = 0; j < pSection->NodeCount; ++j)
				{
					NavNode^ pNode = pSection->Nodes[j];

					if (pNode->Type == NavNodeType::Navigation ||
						pNode->Type == NavNodeType::Dynamic)
					{
						NavigationNode^ pNavNode = (NavigationNode^)pNode;
						
						if (uiNumLevels < pNavNode->Level)
							uiNumLevels = pNavNode->Level;
					}
				}

				++NavHeader.NumSections;
			}
		}
		
		// Save the header position, we'll patch it later.
		long lHeaderPos = fileOut->Position;

		FileControl::Write(fileOut, &NavHeader, sizeof(NavHeader));
		
		// Save section names and fake offsets.
		// Save only the ones that have content.
		for (i = 0; i < SectionCount; ++i)
		{
			if (Sections[i]->NodeCount > 0)
			{
				unsigned __int32 uiLen = Sections[i]->Name->Length;

				IntPtr ptr = Marshal::StringToHGlobalAnsi(
					Sections[i]->Name);
		
				FileControl::Write(fileOut, &uiLen, sizeof(uiLen));
				FileControl::Write(fileOut, ptr.ToPointer(), uiLen);
				
				Marshal::FreeHGlobal(ptr);

				unsigned __int32 uiOffset = 0;
				FileControl::Write(fileOut, &uiOffset, sizeof(uiOffset));
			}
		}

		if (progress)
			progress->Update();
		
		// Get the current offset, start of the first section.
		NavHeader.StartOfNav = (unsigned __int32)fileOut->Position;

		unsigned __int32* puiOffsets = 
			new unsigned __int32[SectionCount];

		unsigned __int32* puiSizes = 
			new unsigned __int32[SectionCount];

		if (progress)
			progress->Begin(SectionCount);
		
		for (i = 0; i < SectionCount; ++i)
		{
			if (Sections[i]->NodeCount > 0)
			{
				unsigned __int32 uiPos = (unsigned __int32)fileOut->Position;

				// Save the navdata offset.
				puiOffsets[i] = uiPos;
			
				// Save the header here, we need to update it later.
				LEVNavSectionHeader Header;
				FileControl::Write(fileOut, &Header, sizeof(Header));

				// Save the data.
				if (!Sections[i]->Save(fileOut))
				{
					delete[] puiOffsets;
				}

				puiSizes[i] = 4+puiOffsets[i]+
					(unsigned __int32)fileOut->Position-uiPos;
			}
			
			if (progress)
				progress->Update();
		}

		if (progress)
			progress->End();
		
		// Damn LH!
		unsigned __int32 uiEndTag = 0xACBDEF12;
		FileControl::Write(fileOut, &uiEndTag, sizeof(uiEndTag));
		
		// This should be the final filesize.
		unsigned __int32 uiFileSize = fileOut->Position;

		fileOut->Position = lHeaderPos;
		
		// Next, resave the nav header, section headers and the offsets...

		FileControl::Write(fileOut, &NavHeader, sizeof(NavHeader));
		
		for (i = 0; i <  SectionCount; ++i)
		{
			if (Sections[i]->NodeCount > 0)
			{
				// Jump to the beginning of the navigation data.
				fileOut->Position += 4+Sections[i]->Name->Length;

				FileControl::Write(
					fileOut, &puiOffsets[i], sizeof(puiOffsets[i]));
				
				// Write the header...

				long lPos = fileOut->Position;

				fileOut->Position = puiOffsets[i];

				LEVNavSectionHeader Header;

				Header.FileSize = puiSizes[i];
				Header.FileVersion = 8;
				Header.NumLevels = uiNumLevels+1;
				Header.Width = Width;
				Header.Height = Height;
				
				FileControl::Write(fileOut, &Header, sizeof(Header));

				fileOut->Position = lPos;
			}
		}
		
		if (progress)
			progress->Update();
		
		if (progress)
			progress->End();
		
		delete[] puiOffsets;
	}

	// Restore grids.

	if (progress)
		progress->Begin(SectionCount);

	for (int i = 0; i < SectionCount; ++i)
	{
		Sections[i]->CreateGrids();
	
		if (progress)
			progress->Update();
	}

	if (progress)
		progress->End();

	if (progress)
		progress->End();
}

int LEVFile::SectionCount::get()
{
	return m_Sections->Count;
}

NavSection^ LEVFile::Sections::get(int index)
{
	return m_Sections[index];
}

NavSection^ LEVFile::Sections::get(String^ name)
{
	for (int i = 0; i < m_Sections->Count; ++i)
	{
		if (m_Sections[i]->Name == name)
			return m_Sections[i];
	}

	return nullptr;
}

int LEVFile::ThemeCount::get()
{
	return m_Themes->Length;
}

String^ LEVFile::Themes::get(int index)
{
	return m_Themes[index];
}

int LEVFile::Width::get()
{
	return m_pMapHeader->Width;
}

int LEVFile::Height::get()
{
	return m_pMapHeader->Height;
}

float LEVFile::Heights::get(int x, int y)
{
	return m_aCells[y*(Width+1)+x].Height*2048.0f;
}

bool LEVFile::Walks::get(int x, int y)
{
	return m_aCells[y*(Width+1)+x].Walkable != 0;
}

void LEVFile::Walks::set(int x, int y, bool value)
{
	const int offset = y*(Width+1)+x;
	const int val = value ? 1 : 0;

	if (m_aCells[offset].Walkable != val)
		Modified = true;

	m_aCells[offset].Walkable = val;
}

unsigned char LEVFile::GroundThemeIndices::get(int x, int y, int index)
{
	return m_aCells[y*(Width+1)+x].Themes[index];
}

unsigned char LEVFile::GroundThemeStrengths::get(int x, int y, int index)
{
	return m_aCells[y*(Width+1)+x].Strength[index];
}

String^ LEVFile::GroundThemeNames::get(int index)
{
	return gcnew String(m_apszGroundThemeNames[index]);
}

UInt32 LEVFile::GroundThemeValues::get(int index)
{
	return m_auiGroundThemeValues[index];
}

int LEVFile::GroundThemeCount::get()
{
	return THEME_COUNT;
}

bool LEVFile::Modified::get()
{
	return m_Modified;
}

void LEVFile::Modified::set(bool value)
{
	m_Modified = value;
}

}
}