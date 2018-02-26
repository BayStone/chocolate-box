#include "Stdafx.h"
#include "Thing.h"
#include "TNGDefinitionType.h"
#include "Variable.h"
#include "ElementArray.h"
#include "CTCBlock.h"
#include "Section.h"
#include "UIDManager.h"

using namespace FableMod::ContentManagement;
using namespace FableMod::BIN;

namespace FableMod
{
namespace TNG
{

Thing::Thing(String^ name) :
m_Modified(false),
m_LockUpdate(false)
{
	m_Name = name;
}

Thing::~Thing()
{
	/*
#ifdef _DEBUG
	Console::WriteLine("TNG::Thing({0})::Destructor", DefinitionType);
#endif
	*/

	Destroy();

	m_UID = nullptr;
	m_DefType = nullptr;
}

void Thing::Destroy()
{
	Clear();

	if (m_Interface)
	{
		m_Interface->Destroy();	
		m_Interface = nullptr;
	}

	m_BINDefType = nullptr;
}

void Thing::Save(TextWriter^ writer)
{
	writer->WriteLine("NewThing {0};", Name); 

	writer->WriteLine("Player {0};", Player); 
	writer->WriteLine("UID {0};", UID); 
	writer->WriteLine("DefinitionType \"{0}\";", DefinitionType); 

	VariableBlock::Save(writer);

	writer->WriteLine("EndThing;"); 
	writer->WriteLine(""); 
}

void Thing::LockUpdate()
{
	m_LockUpdate = true;
}

void Thing::UnlockUpdate()
{
	m_LockUpdate = false;
}

void Thing::Detach()
{
	if (m_Section)
		m_Section->RemoveThing(this);

	if (Interface != nullptr)
		Interface->Update();
}

void Thing::HandleChange()
{
	if (!m_LockUpdate && Interface != nullptr)
		Interface->Update();

	ComplexBlock::HandleChange();
}

CTCBlock^ Thing::CTCs::get(String^ name)
{
	return (CTCBlock^)Find(name);
}

void Thing::Create(TNGDefinitions^ definitions, String^ defType)
{
	BeginCreate(definitions, defType);
	EndCreate(definitions);
}

void Thing::BeginCreate(TNGDefinitions^ definitions,
						String^ defType)
{
	if (defType->StartsWith("\"") && defType->EndsWith("\""))
		defType = defType->Substring(1, defType->Length-2);
	
	m_DefType = defType;
	
	ContentManager^ mgr = ContentManager::Instance;

	m_BINDefType = mgr->FindObjectDefinitionType(m_DefType);

	// Now, try to find the proper TNG definition...

	TNGDefinitionType^ tngDefType = 
		(TNGDefinitionType^)definitions->Find(m_DefType);

	if (tngDefType == nullptr && m_BINDefType)
	{
		tngDefType = (TNGDefinitionType^)
			definitions->Find(m_BINDefType->Name);
	}

	if (tngDefType == nullptr)
	{
		// Finally try AICreature, Village, Building, etc.
		tngDefType = (TNGDefinitionType^)definitions->Find(Name);
	}

	if (tngDefType)
	{
		// Copy all the stuff from the deftype to here.
		// It overwrites the name, so, preserve that.
		
		String^ oldName = Name;
		FableMod::ContentManagement::DefinitionType^ oldDefType = 
			m_BINDefType;
		
		tngDefType->CopyTo(this);
	
		m_Name = oldName;
		m_BINDefType = oldDefType;

		tngDefType = nullptr;
	}
}

void Thing::EndCreate(TNGDefinitions^ definitions)
{
	// Add the CTCs from the game.bin entry.
	// We need to do this last to prevent order problems.
	if (m_BINDefType)
		AddDefinitionTypeCTCs(definitions, m_BINDefType);
}

void Thing::AddDefinitionTypeCTCs(
	TNGDefinitions^ definitions,
	FableMod::ContentManagement::DefinitionType^ defType)
{
	ContentManager^ mgr = ContentManager::Instance;

	Control^ control = defType->FindControl(0x5B9E9F42);

	if (control == nullptr)
		return;
	
	ArrayMember^ ctclist = (ArrayMember^)control->Members["CTC List"];
	
	if (ctclist == nullptr)
		return;

	for (int i = 0; i < ctclist->Elements->Count; ++i)
	{
		MemberCollection^ mc = ctclist->Elements[i];

		if (mc->Count > 0)
		{
			Object^ obj = mc[0];

			if (obj->GetType() == Member::typeid)
			{
				Member^ member = (Member^)obj;

				if (member->Link)
				{
					ContentObject^ o = mgr->FindEntry(
						member->Link->To, member->Value);
				
					if (o != nullptr && !Find(o->Name))
					{
						// New CTC.

						CTCBlock^ ctcBlock = (CTCBlock^)
							definitions->Find(o->Name);

						if (ctcBlock && Find(o->Name) == nullptr)
						{
							Add(ctcBlock->Duplicate());
						}

						ctcBlock = nullptr;
					}

					o = nullptr;
				}
			}
		}

		mc = nullptr;
	}

	control = nullptr;
	ctclist = nullptr;
}

Element^ Thing::Factory()
{
	return gcnew Thing(Name);
}

/// Does not copy the interface object.
void Thing::CopyTo(Element^ element)
{
	TNGDefinitionType::CopyTo(element);

	Thing^ thing = (Thing^)element;
	
	thing->m_UID = UIDManager::Generate();
	thing->m_Player = m_Player;
	thing->m_DefType = m_DefType;
	thing->m_BINDefType = m_BINDefType;
	thing->m_Section = m_Section;
	thing->m_Modified = m_Modified;

	if (thing->m_Section)
		thing->m_Section->AddThing(thing);
}

CTCBlock^ Thing::ApplyCTC(TNGDefinitions^ definitions,
						  String^ name)
{
	if (!definitions && Section)
		definitions = Section->TNGFile->Definitions;

	CTCBlock^ block = (CTCBlock^)Find(name);

	if (block)
		return block;

	// Create a new one... this wasn't in the XML definition.
	// Now check the XML for this specific CTC...
    
    block = (CTCBlock^)definitions->Find(name);

    if (block)
    {
        block = (CTCBlock^)block->Duplicate();
    }
    else
    {
        // Okay, not even there.
 	    block = gcnew CTCBlock();
	    block->Name = name;
	}
    
	Add(block);
	
	return block;
}

void Thing::ApplyVariable(TNGDefinitions^ definitions,
						  String^ argument, String^ value)
{
	if (argument == "UID")
	{
		UIDManager::Add(value);
		m_UID = value;
		return;
	}
	else if (argument == "Player")
	{	
		m_Player = int::Parse(value);
		return;
	}

	Element^ element = Find(argument);

	if (element)
	{
		if (element->GetType() == ElementArray::typeid)
		{
			// For CreateTC.

			Element^ e = ((FableMod::TNG::ElementArray^)element)->Add();

			if (e->GetType() == Variable::typeid)
			{		
				((FableMod::TNG::Variable^)e)->SetRawValue(value);
			}	

			return;
		}
		else if (element->GetType() == Variable::typeid)
		{	
			((FableMod::TNG::Variable^)element)->SetRawValue(value);
			return;
		}
	}
	
	
	Variable^ v = gcnew Variable();
	
	v->Name = argument;
	v->SetRawValue(value);
		
	Add(v);	
}

void Thing::ApplyCTCVariable(TNGDefinitions^ definitions,
							 CTCBlock^ ctc, 
							 String^ argument, 
							 String^ value)
{
	FableMod::TNG::Variable^ v = 
		(FableMod::TNG::Variable^)ctc->Find(argument);

	if (v != nullptr)
	{
		v->SetRawValue(value);
	}
	else
	{
		Variable^ v = gcnew Variable();
	
		v->Name = argument;
		v->SetRawValue(value);
		
		ctc->Add(v);
	}
}

Collection<CTCBlock^>^ Thing::CTCs::get()
{
	Collection<CTCBlock^>^ ctcc = gcnew Collection<CTCBlock^>();
	
	for (int i = 0; i < ElementCount; ++i)
	{
		if (Elements[i]->GetType() == CTCBlock::typeid)
			ctcc->Add((CTCBlock^)Elements[i]);
	}

	return ctcc;
}

void Thing::DefinitionType::set(String^ value)
{	
	if (value->StartsWith("\"") && value->EndsWith("\""))
		value = value->Substring(1, value->Length-2);

	if (value != m_DefType)
	{
		Modified = true;
		m_DefType = value;
	}
}

void Thing::UID::set(String^ value)
{
	if (value != m_UID)
	{
		Modified = true;
		m_UID = value;
	}
}

void Thing::Player::set(int value)
{
	if (value != m_Player)
	{
		Modified = true;
		m_Player = value;
	}
}

bool Thing::Modified::get()
{
	if (m_Modified)
		return true;

	return ComplexBlock::Modified;
}

void Thing::Modified::set(bool value)
{
	m_Modified = value;

	ComplexBlock::Modified = value;
}

}
}