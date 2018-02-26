#include "Stdafx.h"
#include "Section.h"
#include "TNGFile.h"

namespace FableMod
{
namespace TNG
{

TNGFile::TNGFile(TNGDefinitions^ definitions) :
m_Sections(gcnew Collection<Section^>()),
m_Parser(ParserState::Basic),
m_Definitions(definitions)
{
}

TNGFile::~TNGFile()
{
#ifdef _DEBUG
	Console::WriteLine("TNGFile({0})::Destructor", FileName);
#endif

	Destroy();

	m_Sections = nullptr;
}

void TNGFile::Destroy()
{
	if (m_Sections)
	{
		for (int i = 0; i < m_Sections->Count; ++i)
			delete m_Sections[i];
	
		m_Sections->Clear();
	}
}

void TNGFile::Load(String^ fileName)
{
	Parser::Load(fileName);

	m_FileName = fileName;

	// Reset flag.
	Modified = false;
}

void TNGFile::Save(String^ fileName)
{
	FileStream^ file = File::Create(fileName);

	TextWriter^ writer = gcnew StreamWriter(file);

	writer->WriteLine("Version 2;");
	writer->WriteLine("");

	for (int i = 0; i < m_Sections->Count; ++i)
	{
		m_Sections[i]->Save(writer);
	}

	writer->Close();
	file->Close();
}

void TNGFile::ParseArgument(String^ argument, String^ value)
{
	if (m_Parser == ParserState::Basic)
	{
		if (argument == "XXXSectionStart")
		{
			AddSection(gcnew Section(value));

			m_Parser = ParserState::Section;
		}
	}
	else if (m_Parser == ParserState::Section)
	{
		Section^ section = m_Sections[m_Sections->Count-1];

		if (argument == "NewThing")
		{
			m_Thing = gcnew Thing(value);
			
			m_Parser = ParserState::Thing;
		}
	}
	else if (m_Parser == ParserState::Thing)
	{
		if (argument == "DefinitionType")
		{
			m_Thing->BeginCreate(m_Definitions, value);
		}
		else
		{
			m_Thing->ApplyVariable(m_Definitions, argument, value);
		}
	}
	else if (m_Parser == ParserState::CTC && m_CTC != nullptr)
	{
		m_Thing->ApplyCTCVariable(
			m_Definitions, m_CTC, argument, value);
	}
}

void TNGFile::ParseCommand(String^ command)
{
	if (m_Parser == ParserState::Section)
	{
		if (command == "XXXSectionEnd")
		{
			m_Parser = ParserState::Basic;
		}
	}
	else if (m_Parser == ParserState::Thing)
	{
		if (command->StartsWith("StartCTC"))
		{
			m_CTC = m_Thing->ApplyCTC(
				m_Definitions, command->Substring(5));

			m_Parser = ParserState::CTC;
		}
		else if (command == "EndThing")
		{			
			Section^ section = m_Sections[m_Sections->Count-1];

			m_Thing->EndCreate(m_Definitions);
			
			section->AddThing(m_Thing);

			m_Parser = ParserState::Section;
		}
	}
	else if (m_Parser == ParserState::CTC)
	{
		if (command->StartsWith("EndCTC"))
		{
			m_Parser = ParserState::Thing;
		}
	}
}

Thing^ TNGFile::FindThing(String^ uid)
{
	for (int i = 0; i < m_Sections->Count; ++i)
	{
		Thing^ thing = m_Sections[i]->FindThing(uid);

		if (thing)
			return thing;
	}

	return nullptr;
}

bool TNGFile::Modified::get()
{
	for (int i = 0; i < m_Sections->Count; ++i)
	{
		if (m_Sections[i]->Modified)
			return true;
	}

	return false;
}

void TNGFile::Modified::set(bool value)
{
	for (int i = 0; i < m_Sections->Count; ++i)
	{
		m_Sections[i]->Modified = value;
	}
}

Section^ TNGFile::Sections::get(String^ name)
{
	for (int i = 0; i < m_Sections->Count; ++i)
	{
		if (m_Sections[i]->Name == name)
			return m_Sections[i];
	}

	return nullptr;
}

void TNGFile::AddSection(Section^ section)
{
	m_Sections->Add(section);
	section->TNGFile = this;
}

String^ TNGFile::FileName::get()
{
	return m_FileName;
}

}
}