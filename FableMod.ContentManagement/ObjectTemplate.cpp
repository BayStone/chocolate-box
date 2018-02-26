#include "stdafx.h"
#include "ObjectTemplate.h"
#include "DefinitionDB.h"

using namespace System::Globalization;
using namespace FableMod::BIN;

namespace FableMod
{
namespace ContentManagement
{

//----------------------------------------------------------------------------

String^ AssetTemplate::Prefix::get()
{
	switch (Type)
	{
	case ContentType::Graphics:
		return gcnew String("MESH");
	case ContentType::MainTextures:
		return gcnew String("TEXTURE");
	case ContentType::Text:
		return gcnew String("TEXT");
	}
		
	return gcnew String("PREFIX");
}

//----------------------------------------------------------------------------

ObjectTemplate::ObjectTemplate() :
m_Items(gcnew List<BaseTemplate^>())
{
}

void ObjectTemplate::AddControlMembers(DefTypeTemplate^ link,
									   String^ controlName, 
									   UInt32 control,
									   MemberCollection^ c, 
									   int* pId)
{
	for (int i = 0; i < c->Count; ++i)
	{
		if (c[i]->GetType() == Member::typeid)
		{
			Member^ member = (Member^)c[i];

			if (member->Link)
			{
				ContentType contentType = ContentType::Unknown;

				if (member->Link->To == LinkDestination::ModelID)
					contentType = ContentType::Graphics;
				else if (member->Link->To == LinkDestination::MainTextureID)
					contentType = ContentType::MainTextures;
				else if (member->Link->To == LinkDestination::TextID)
					contentType = ContentType::Text;

				if (contentType == ContentType::Unknown)
					continue;

				AssetTemplate^ item = gcnew AssetTemplate();
				
				item->Name = link->Name+"."+member->Name;
				item->ControlID = control;
				item->Element = i;
				item->ID = ++(*pId);
				item->LinkTo = link;
				item->Named = true;
				item->Type = contentType;
				item->Ref = (contentType == ContentType::MainTextures);
				
				m_Items->Add(item);
			}
		}
	}
}

void ObjectTemplate::Build(BINEntry^ entry,
						   DefTypeTemplate^ link, 
						   int* pId)
{
	DefinitionType^ defType = 
		ContentManager::Instance->Definitions->GetDefinition(
		entry->Definition);

	if (!defType)
		return;

	defType->ReadIn(entry);

	ArrayMember^ cdefs = defType->CDefs;

	DefTypeTemplate^ obj = gcnew DefTypeTemplate();

	obj->Name = entry->Definition;
	obj->Type = entry->Definition;
	obj->ID = ++(*pId);
	obj->OriginalID = entry->ID;
	obj->LinkTo = link;
	obj->Named = !link;
	
	m_Items->Add(obj);

	if (cdefs)
	{
		for (int i = 0; i < cdefs->Elements->Count; ++i)
		{
			Member^ member = (Member^)cdefs->Elements[i][0];

			ContentObject^ o = ContentManager::Instance->FindEntry(
				member->Link->To, member->Value);

			if (o != nullptr)
			{
				Member^ member2 = (Member^)cdefs->Elements[i][1];

				ContentObject^ linkObj = ContentManager::Instance->FindEntry(
					member2->Link->To, member2->Value);

				BINEntry^ entry = dynamic_cast<BINEntry^>(linkObj->Object);

				if (entry)
					Build(entry, obj, pId);  
			}
		}
	}
	
	for (int i = 0; i < defType->Controls->Count; ++i)
	{
		Control^ c = defType->Controls[i];
		AddControlMembers(obj, c->Name, c->ID, c->Members, pId);
	}

	obj->Ref = true;

	for (int i = 0; i < m_Items->Count; ++i)
	{
		BaseTemplate^ item = m_Items[i];

		if (item->LinkTo == obj && !item->Ref)
		{
			obj->Ref = false;
			break;
		}
	}
}

void ObjectTemplate::Build(BINEntry^ entry)
{
	m_Items->Clear();

	int id = 0;

	Build(entry, nullptr, &id);
}

}
}