#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Collections::ObjectModel;

#include "TNGDefinitions.h"
#include "Thing.h"

namespace FableMod
{
namespace TNG
{

ref class Section;

/// <summary>
/// Thing file loader.
/// </summary>
public ref class TNGFile : FableMod::Script::Parser
{
public:
	
	TNGFile(TNGDefinitions^ definitions);

	virtual ~TNGFile();

	void Destroy();

	virtual void Load(String^ fileName) override;

	void Save(String^ fileName);

	void AddSection(Section^ section);

	property int SectionCount { int get() { return m_Sections->Count; } }

	property Section^ Sections[int] 
	{ 
		Section^ get(int index) { return m_Sections[index]; } 
	}

	property TNGDefinitions^ Definitions 
	{ 
		TNGDefinitions^ get() { return m_Definitions; } 
	}

	property Section^ Sections[String^] 
	{ 
		Section^ get(String^ name); 
	}

	property bool Modified
	{
		virtual bool get();
		virtual void set(bool value);
	}

	property String^ FileName
	{
		String^ get();
	}

	Thing^ FindThing(String^ uid);

protected:

	virtual void ParseArgument(String^ argument, String^ value) override;

	virtual void ParseCommand(String^ command) override;

	Collection<Section^>^ m_Sections;
	TNGDefinitions^ m_Definitions;
						  
	enum class ParserState
	{
		Basic = 0,
		Section,
		Thing,
		CTC,
	};

	ParserState m_Parser;
	CTCBlock^ m_CTC;
	Thing^ m_Thing;
	String^ m_FileName;
};

}
}
