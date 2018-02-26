#pragma once

#include "NavSection.h"

using namespace System;
using namespace System::IO;
using namespace System::Collections::Generic;
using namespace FableMod::CLRCore;

namespace FableMod
{
namespace LEV
{

/// <summary>
/// Fable level file loader.
/// </summary>
public ref class LEVFile : public IDisposable
{
public:

	LEVFile();

	~LEVFile();

	!LEVFile();

	void Destroy();

	void Open(
		String^ filename,
		ProgressInterface^ progress);
	
	void Save(
		String^ filename, 
		ProgressInterface^ progress);

	void LoadNavigation();

	void ResetSections();

	property int SectionCount { int get(); }

	property NavSection^ Sections[int] { NavSection^ get(int index); }
	
	property NavSection^ Sections[String^] { NavSection^ get(String^ name); }
	
	property int ThemeCount { int get(); }

	property String^ Themes[int] { String^ get(int index); }
	
	property int Width { int get(); }

	property int Height { int get(); }

	property float Heights[int, int] { float get(int x, int y); }
	
	property bool Walks[int, int] 
	{ 
		bool get(int x, int y); 
		void set(int x, int y, bool value);
	}
	
	property int GroundThemeCount
	{ 
		int get(); 
	}

	property String^ GroundThemeNames[int] 
	{ 
		String^ get(int index); 
	}

	property UInt32 GroundThemeValues[int] 
	{ 
		UInt32 get(int index); 
	}

	property unsigned char GroundThemeIndices[int, int, int] 
	{ 
		unsigned char get(int x, int y, int index); 
	}
	
	property unsigned char GroundThemeStrengths[int, int, int] 
	{ 
		unsigned char get(int x, int y, int index); 
	}

	property bool Modified
	{
		bool get();
		void set(bool value);
	}

	property String^ FileName
	{
		String^ get() { return m_FileName; }
	}

protected:

	void ReadNavigation(BufferStream^ fileIn);

	void WriteNavigation(FileStream^ fileOut, ProgressInterface^ progress);

	String^ m_FileName;
	LEVHeader* m_pHeader;
	LEVMapHeader* m_pMapHeader;
	array<String^>^ m_Themes;
	char** m_apszGroundThemeNames;
	unsigned __int32* m_auiGroundThemeValues;
	unsigned char* m_pucPalette2;
	LEVCellHeader* m_aCells;
	LEVSoundCellHeader* m_aSoundCells;
	unsigned __int32 m_uiCellsOffset;
	unsigned char* m_pucData;
	unsigned __int32 m_uiDataSize;
	unsigned char* m_pucNavData;
	unsigned __int32 m_uiNavDataSize;
	List<NavSection^>^ m_Sections;
	BufferStream^ m_File;
	bool m_Modified;
	unsigned __int32 m_uiVersion;
};

}
}
