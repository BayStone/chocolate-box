#pragma once

using namespace System;
using namespace System::IO;

#include "VariableBlock.h"
#include "CTCBlock.h"
#include "Variable.h"
#include "TNGDefinitions.h"
#include "ThingInterface.h"

namespace FableMod
{
namespace TNG
{

ref class Section;

/// <summary>
/// Thing controller class.
/// </summary>
public ref class Thing : public TNGDefinitionType 
{
public:

	Thing(String^ name);

	virtual ~Thing();

	virtual void Destroy() override;

	virtual void Save(TextWriter^ writer) override;

	void Create(TNGDefinitions^ definitions, String^ defType);

	void BeginCreate(
		TNGDefinitions^ definitions, 
		String^ defType);

	CTCBlock^ ApplyCTC(
		TNGDefinitions^ definitions,
		String^ name);

	void ApplyVariable(
		TNGDefinitions^ definitions,
		String^ argument, 
		String^ value);

	void ApplyCTCVariable(
		TNGDefinitions^ definitions,
		CTCBlock^ ctc, 
		String^ argument, 
		String^ value);

	void EndCreate(TNGDefinitions^ definitions);
	
	void LockUpdate();

	void UnlockUpdate();

	void Detach();

	virtual void CopyTo(Element^ element) override;

	property CTCBlock^ CTCs[String^] { CTCBlock^ get(String^ name); }
	
	property String^ DefinitionType 
	{ 
		String^ get() { return m_DefType; } 
		void set(String^ value);
	}

	property String^ UID 
	{ 
		String^ get() { return m_UID; } 
		void set(String^ value);
	}

	property ThingInterface^ Interface
	{ 
		ThingInterface^ get() { return m_Interface; } 
		void set(ThingInterface^ value) { m_Interface = value; }
	}

	property int Player 
	{ 
		int get() { return m_Player; } 
		void set(int value);
	}

	property Collection<CTCBlock^>^ CTCs
	{
		Collection<CTCBlock^>^ get();
	}

	property bool Modified
	{
		virtual bool get() override;
		virtual void set(bool value) override;
	}

	property FableMod::TNG::Section^ Section
	{
		FableMod::TNG::Section^  get()
		{
			return m_Section;
		}

		void set(FableMod::TNG::Section^ value)
		{
			m_Section = value;
		}
	}

	property FableMod::ContentManagement::DefinitionType^ BinDefinitionType
	{
		FableMod::ContentManagement::DefinitionType^ get() 
		{ 
			return m_BINDefType; 
		}
	}

protected:

	void AddDefinitionTypeCTCs(
		TNGDefinitions^ definitions,
		FableMod::ContentManagement::DefinitionType^ defType);

	virtual void HandleChange() override;

	virtual Element^ Factory() override;

	String^ m_Graphic;
	String^ m_DefType;
	String^ m_UID;
	int m_Player;
	bool m_Modified;
	bool m_LockUpdate;
	ThingInterface^ m_Interface;
	FableMod::TNG::Section^ m_Section;
	FableMod::ContentManagement::DefinitionType^ m_BINDefType;
};

}
}
