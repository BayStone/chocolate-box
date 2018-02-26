#include "StdAfx.h"
#include "ModPackage.h"
#include "AssetLinker.h"

using namespace System::Xml::Serialization;
using namespace System::Runtime::InteropServices;
using namespace System::IO;
using namespace System::Collections::Specialized;
using namespace System::Runtime::Serialization::Formatters::Binary;
using namespace System::Runtime::Serialization;

static unsigned int FMP_VERSION = 0x01CB;

namespace FableMod
{
namespace ContentManagement
{

ModPackage::ModPackage()
{
	// Create the output big file.
	m_Big = gcnew BIGFile(FMP_VERSION);

	New();
}

ModPackage::~ModPackage()
{
    m_Big->Destroy();
}


void ModPackage::Load(String^ fileName, ProgressInterface^ progress)
{
    m_Big = gcnew BIGFile();
	
    if (progress)
        progress->Begin(4);
    
    m_Big->Load(fileName, progress);
    
    if (m_Big->ContentType != 0x01FE &&
		m_Big->ContentType != FMP_VERSION)
	{
        throw gcnew System::Exception("Invalid mod package type.");
	}

	// Now, check the version and update it accordingly...

	if (m_Big->ContentType == 0x01FE)
	{
		m_Big->ContentType = FMP_VERSION;

		m_Big->Banks["graphicsLinkMetaData"]->Name = "AssetLinks";
		m_Big->Banks["GameBINLinkMetaData"]->Name = "ObjectLinks";
		m_Big->Banks["maintextures"]->Name = "MainTextures";
		m_Big->Banks["graphics"]->Name = "Graphics";
		m_Big->Banks["GameBINEntries"]->Name = "Objects";
		m_Big->Banks["text"]->Name = "Text";
		m_Big->Banks["frontendtextures"]->Name = "FrontEndTextures";
		m_Big->Banks["guitextures"]->Name = "GUITextures";
		m_Big->Banks["ScriptBINEntries"]->Name = "Scripts";
	}

    if (progress)
        progress->End();
}

void ModPackage::Merge(ModPackage^ package)
{
	if (package->Description != "")
		Description += "\r\n"+package->Description;

	MergeBank(package, ContentType::MainTextures, false);
    MergeBank(package, ContentType::GUITextures, false);
	MergeBank(package, ContentType::FrontEndTextures, false);
	MergeBank(package, ContentType::Graphics, false);
    MergeBank(package, ContentType::Text, false);
    MergeBank(package, ContentType::Objects, false);
	MergeBank(package, ContentType::Scripts, false);

    MergeBank(package, ContentType::Graphics, true);
    MergeBank(package, ContentType::Objects, true);
}

void ModPackage::MergeBank(ModPackage^ package, 
						   ContentType entryType,
						   bool bLink)
{
	BIGBank^ bank1 =
		bLink ? package->GetLinkBank(entryType) : package->GetBank(entryType);
	BIGBank^ bank0 = 
		bLink ? GetLinkBank(entryType) : GetBank(entryType);

	Console::WriteLine("ModPackage::MergeBank({0},{1})", 
		entryType.ToString(), bLink);

    for (int i = 0; i < bank1->EntryCount; ++i)
    {
		AssetEntry^ entry = bank1->Entries[i];

		Console::WriteLine(" [{0}] {1}", i, entry->DevSymbolName);

		AssetEntry^ newEntry = 
			bank0->FindEntryBySymbolName(entry->DevSymbolName);

		if (newEntry)
		{
			newEntry->Copy(entry);
		}
		else
		{
			newEntry = gcnew AssetEntry(entry, bank0);
			bank0->AddEntry(newEntry);
		}
    }
}

void ModPackage::ApplyBank(ContentType entryType)
{
    BIGBank^ bank = GetBank(entryType);
	
    if (bank != nullptr)
    {
        for (int i = 0; i < bank->EntryCount; ++i)
        {
			Console::WriteLine("ModPackage::ApplyEntry({0})", 
				bank->Entries[i]->DevSymbolName);

            ContentManager::Instance->UpdateEntry(
				entryType, bank->Entries[i]);
        }
    }
}

void ModPackage::ApplyBINBank(ContentType entryType, 
							  Collection<BINEntry^>^ entries)
{
	ContentManager^ mgr = ContentManager::Instance;

	BIGBank^ bank = GetBank(entryType);
    
	BINFile^ bin = nullptr;

	// Need to select the proper bin file.
	if (entryType == ContentType::Objects)
		bin = mgr->Objects;
	else if (entryType == ContentType::Scripts)
		bin = mgr->Scripts;

	Console::WriteLine("ModPackage::ApplyBINBank({0})...", bank->Name);

	for (int i = 0; i < bank->EntryCount; ++i)
    {
		AssetEntry^ assetEntry = bank->Entries[i];

		Console::WriteLine(" BIN [{0}] {1}", i, assetEntry->DevSymbolName);

		String^ def = gcnew String(assetEntry->GetSubHeader());

		BINEntry^ entry;
		
		if (assetEntry->DevSymbolName != "")
			entry = bin->GetEntryByName(assetEntry->DevSymbolName);
		else
			entry = bin->Entries[assetEntry->ID];
		
		if (entry)
			entry->SetData(assetEntry->GetData(), assetEntry->Length);
		else
		{
			entry = bin->AddEntry(
				assetEntry->DevSymbolName, 
				def, 
				assetEntry->GetData(), 
				assetEntry->Length);

			entry->Modified = true;
		}

		if (entries)
			entries->Add(entry);

		entry = nullptr;
	}
}

void ModPackage::ApplyBINLinks(ContentType contentType, 
							   Collection<BINEntry^>^ entries)
{
	if (entries->Count == 0)
		return;

	BIGBank^ bank = GetBank(contentType);
	BIGBank^ linkBank = GetLinkBank(contentType);

	Console::WriteLine("ModPackage::ApplyBINLinks({0},{1})...",
		bank->Name,
		linkBank->Name);

	System::Diagnostics::Debug::Assert(
		linkBank->EntryCount == entries->Count);

	for (int i = 0; i < bank->EntryCount; ++i)
    {
		Console::WriteLine(" BINLink [{0}] {1}",
			i,
			bank->Entries[i]->DevSymbolName);

		AssetEntry^ linkEntry = 
			linkBank->FindEntryBySymbolName(bank->Entries[i]->DevSymbolName);
		
		if (linkEntry)
		{
			MemoryStream^ stream = gcnew MemoryStream(linkEntry->Data);

			IFormatter^ formatter = gcnew BinaryFormatter();

			NameValueCollection^ links = 
				(NameValueCollection^)formatter->Deserialize(stream);
			
			//for each (String^ key in links->AllKeys)
			//	Console::WriteLine("  Link[{0}] - {1}", key, links[key]);

			DefinitionType^ def = 
				ContentManager::Instance->Definitions->GetDefinition(
					entries[i]->Definition);
			
			if (def && links)
			{
				def->ReadIn(entries[i]);

				for (int j = 0; j < def->Controls->Count; ++j)
				{
					ApplyLinks(
						links, 
						def->Controls[j]->ID.ToString(), 
						def->Controls[j]->Members);
				}

				if (def->HasCDefListing)
				{
					ApplyLinks(links, "",  def->CDefs); 
				}

				def->Write(entries[i]);
			}

			stream->Close();
			stream = nullptr;
			formatter = nullptr;
		}
	}
}

void ModPackage::ApplyAssetLinks(ContentType contentType)
{
	ContentManager^ mgr = ContentManager::Instance;

	BIGBank^ bank = m_Big->Banks[GetBankName(ContentType::Graphics)];
    BIGBank^ linkBank = m_Big->Banks[GetBankName(ContentType::Graphics)];
    
	Console::WriteLine("ModPackage::ApplyAssetLinks({0},{1})...",
		bank->Name,
		linkBank->Name);

	for (int i = 0; i < bank->EntryCount; ++i)
    {
		AssetEntry^ linkEntry = 
			linkBank->FindEntryBySymbolName(bank->Entries[i]->DevSymbolName);
		
		Console::WriteLine(" AssetLink [{0}] {1}",
			i,
			bank->Entries[i]->DevSymbolName);

		if (linkEntry)
		{
			AssetLinker^ linker = mgr->CreateAssetLinker(
				ContentType::Graphics, bank->Entries[i]);

			if (linker)
			{
				MemoryStream^ stream = gcnew MemoryStream(linkEntry->Data);

				IFormatter^ formatter = gcnew BinaryFormatter();

				NameValueCollection^ links = 
					(NameValueCollection^)formatter->Deserialize(stream);

				linker->ApplyLinks(links);

				Console::WriteLine("  Links applied.");
			}
		}
	}
}

void ModPackage::Apply()
{
	ContentManager^ mgr = ContentManager::Instance;

	// Apply all BIG objects.
	ApplyBank(ContentType::MainTextures);
    ApplyBank(ContentType::GUITextures);
	ApplyBank(ContentType::FrontEndTextures);
	ApplyBank(ContentType::Graphics);
    ApplyBank(ContentType::Text);
    
	Collection<BINEntry^>^ objects = gcnew Collection<BINEntry^>();
	Collection<BINEntry^>^ scripts = gcnew Collection<BINEntry^>();
		
	// Apply all the bin objects.
	ApplyBINBank(ContentType::Objects, objects);
	ApplyBINBank(ContentType::Scripts, scripts);

	// Okay, now we need to link the objects together.

	ApplyAssetLinks(ContentType::Graphics);
	
	ApplyBINLinks(ContentType::Objects, objects);
	ApplyBINLinks(ContentType::Scripts, scripts);

	objects = nullptr;
	scripts = nullptr;
}


void ModPackage::ApplyLinks(NameValueCollection^ links, 
							String^ path, 
							Object^ member)
{
	if (member->GetType() == Member::typeid)
	{
		Member^ mem = (Member^)member;
		
		if (mem->Link)
		{
			String^ value = (String^)links[path+"."+mem->Name];
			
			if (value == nullptr)
			{
				Console::WriteLine("Link {0} is null.", path+"."+mem->Name);
				return;
			}

			Console::WriteLine("  Link {0} to {1}.", 
				path+"."+mem->Name, value);

			ContentManager^ mgr = ContentManager::Instance;

			try
			{
				Object^ val = nullptr;

				switch (mem->Link->To)
				{
				case LinkDestination::GameBINEntryID:
					{
						BINEntry^ entry = 
							mgr->Objects->GetEntryByName(value);
						
						if (entry)
							val = entry->ID;
					}
					break;
				case LinkDestination::FrontEndBINEntryID:
					break;
				case LinkDestination::ScriptBINEntryID:
					break;
				case LinkDestination::NamesBINEnum:
					{
						NamesBINEntry^ entry = 
							mgr->Names->GetEntryByName(value);
						
						if (entry)
							val = entry->Enum;
					}
					break;
				case LinkDestination::NamesBINOffset:
					{
						NamesBINEntry^ entry = 
							mgr->Names->GetEntryByName(value);
						if (entry)
							val = entry->Offset;
					}
					break;
				case LinkDestination::ModelID:
					{
						AssetEntry^ entry = 
							mgr->GraphicsBank->FindEntryBySymbolName(value);
						if (entry)
							val = (int)entry->ID;
					}
					break;
				case LinkDestination::AnimationID:
					break;
				case LinkDestination::MainTextureID:
					{
						AssetEntry^ entry = 
							mgr->MainTextureBank->FindEntryBySymbolName(
								value);
						
						if (entry)
							val = (int)entry->ID;
					}
					break;
				case LinkDestination::GUITextureID:
					{
						AssetEntry^ entry = 
							mgr->GUITextureBank->FindEntryBySymbolName(
								value);
						
						if (entry)
							val = (int)entry->ID;
					}
					break;
				case LinkDestination::FrontEndTextureID:
					{
						AssetEntry^ entry = 
							mgr->FrontEndTextureBank->FindEntryBySymbolName(
								value);
						
						if (entry)
							val = (int)entry->ID;
					}
					break;
				case LinkDestination::EffectID:
					break;
				case LinkDestination::TextID:
					{
						AssetEntry^ entry = 
							mgr->TextBank->FindEntryBySymbolName(value);
						if (entry)
							val = (int)entry->ID;
					}
					break;
				case LinkDestination::TextGroupID:
					break;
				}

				if (val == nullptr)
				{
					Console::WriteLine("No value.");
				}
				else
				{
					mem->Value = val;
				}
			}
			catch (Exception^ ex)
			{
				//Console::WriteLine(ex->ToString());
			}
		}
	}
	else if (member->GetType() == ArrayMember::typeid)
	{
		ArrayMember^ arr = (ArrayMember^)member;
		path += "." + arr->Name;
		for (int i = 0; i < arr->Elements->Count; ++i)
		{
			ApplyLinks(
				links, 
				path+"."+i.ToString(), 
				arr->Elements[i]);
		}
	}
	else if (member->GetType() == MemberCollection::typeid)
	{
		MemberCollection^ mc = (MemberCollection^)member;
		
		for (int i = 0; i < mc->Count; ++i)
		{
			ApplyLinks(
				links, 
				path, 
				mc[i]);
		}
	}
}

Collection<ContentObject^>^ ModPackage::Items::get()
{
	Collection<ContentObject^>^ c = gcnew Collection<ContentObject^>();

	// Add all objects.
	ItemsAddBank(c, ContentType::MainTextures);
	ItemsAddBank(c, ContentType::GUITextures);
	ItemsAddBank(c, ContentType::FrontEndTextures);
	ItemsAddBank(c, ContentType::Graphics);
	ItemsAddBank(c, ContentType::Objects);
	ItemsAddBank(c, ContentType::Scripts);
	ItemsAddBank(c, ContentType::Text);
	
	return c;
}

void ModPackage::ItemsAddBank(Collection<ContentObject^>^ c,
							  ContentType contentType)
{
	BIGBank^ bank = GetBank(contentType);

	for (int i = 0; i < bank->EntryCount; ++i)
	{
		AssetEntry^ entry = bank->Entries[i];

		c->Add(gcnew ContentObject(
			entry->DevSymbolName, entry, contentType));
	}
}

void ModPackage::Save(String^ fileName, ProgressInterface^ progress)
{
	m_Big->Save(fileName, progress);
}

void ModPackage::New()
{
	m_Big->Destroy();

    int iID = 0;

	// For user stuff, desc, etc.
	m_Big->AddBank(gcnew BIGBank("Settings", iID++, 1, m_Big));

	// For the links in objects.
	m_Big->AddBank(gcnew BIGBank(
		GetLinkBankName(ContentType::BIG), iID++, 1, m_Big));
	m_Big->AddBank(gcnew BIGBank(
		GetLinkBankName(ContentType::BIN), iID++, 1, m_Big));
	
	m_Big->AddBank(gcnew BIGBank(
		GetBankName(ContentType::Text), iID++, 1, m_Big));
	m_Big->AddBank(gcnew BIGBank(
		GetBankName(ContentType::MainTextures), iID++, 1, m_Big));
	m_Big->AddBank(gcnew BIGBank(
		GetBankName(ContentType::Objects), iID++, 1, m_Big));
	m_Big->AddBank(gcnew BIGBank(
		GetBankName(ContentType::Graphics), iID++, 1, m_Big));
	m_Big->AddBank(gcnew BIGBank(
		GetBankName(ContentType::Scripts), iID++, 1, m_Big));
	m_Big->AddBank(gcnew BIGBank(
		GetBankName(ContentType::GUITextures), iID++, 1, m_Big));
	m_Big->AddBank(gcnew BIGBank(
		GetBankName(ContentType::FrontEndTextures), iID++, 1, m_Big));
}

void ModPackage::AddAssetEntry(ContentType entryType,
							   AssetEntry^ entry)
{
	Console::WriteLine("ModPackage::AddAssetEntry({0})", 
		entry->DevSymbolName);
    
	BIGBank^ bank = GetBank(entryType);
        
	AssetEntry^ newEntry = 
		bank->FindEntryBySymbolName(entry->DevSymbolName);

	if (newEntry)
	{
		newEntry->Copy(entry);
		RemoveLinks(entryType, entry->DevSymbolName);
	}
	else
	{
		newEntry = gcnew AssetEntry(entry, bank);
		bank->AddEntry(newEntry);
	}

	AddAssetLinks(entryType, entry);
}

void ModPackage::AddAssetLinks(ContentType entryType, 
							   AssetEntry^ entry)
{

	AssetLinker^ linker = 
		ContentManager::Instance->CreateAssetLinker(entryType, entry);

	if (linker)
	{
		// This object has a linker.

		NameValueCollection^ links = linker->CreateLinks();

		if (!links)
			return;

	#ifdef _DEBUG
		Console::WriteLine("Asset links for {0}...", entry->DevSymbolName);
		PrintLinks(links);
	#endif

		MemoryStream^ stream = gcnew MemoryStream();

		IFormatter^ formatter = gcnew BinaryFormatter();
		formatter->Serialize(stream, links);

		array<System::Byte>^ data = stream->GetBuffer();
		pin_ptr<System::Byte> p = &data[0];
		
		BIGBank^ linkBank = m_Big->Banks["AssetLinks"];

		AssetEntry^ linkEntry = 
			linkBank->FindEntryBySymbolName(entry->DevSymbolName);

		if (!linkEntry)
		{
			linkEntry = gcnew AssetEntry(
				entry->DevSymbolName, 
				linkBank->GetNewID(), 
				0, 
				linkBank);
			
			linkBank->AddEntry(linkEntry);
		}

		linkEntry->SetData((char*)p, stream->Length);
		
		stream->Close();
		stream = nullptr;
		formatter = nullptr;
		links = nullptr;
	}
	else
	{
		Console::WriteLine("No Asset linker for {0}", 
			entry->DevSymbolName);
	}
}

void ModPackage::RemoveLinks(ContentType entryType, String^ entryName)
{
	BIGBank^ linkBank = m_Big->Banks[GetLinkBankName(entryType)];

	AssetEntry^ linkEntry = linkBank->FindEntryBySymbolName(entryName);

	if (linkEntry)
		linkBank->RemoveEntry(linkEntry);
}

void ModPackage::CreateLinks(NameValueCollection^ links, 
							 String^ path, 
							 Object^ member)
{
	if (member->GetType() == Member::typeid)
	{
		Member^ mem = (Member^)member;

		if (mem->Link)
		{
			try
			{
				ContentObject^ o = ContentManager::Instance->FindEntry(
					mem->Link->To, mem->Value);

				if (o)
				{
					BINEntry^ entry = (BINEntry^)o->Object;

					if (entry->Name != "")
						links->Add(path+"."+mem->Name, entry->Name);
				}
			}
			catch (Exception^)
			{
				// Whatever.
			}
		}
	}
	else if (member->GetType() == ArrayMember::typeid)
	{
		ArrayMember^ arr = (ArrayMember^)member;
		
		path += "."+arr->Name;
		
		for (int i = 0 ; i < arr->Elements->Count; ++i)
		{
			CreateLinks(
				links, 
				path+"."+i.ToString(), 
				arr->Elements[i]);
		}
	}
	else if (member->GetType() == MemberCollection::typeid)
	{
		MemberCollection^ mc = (MemberCollection^)member;
		
		for (int i = 0; i < mc->Count; ++i)
			CreateLinks(links, path, mc[i]);
	}
}

#ifdef _DEBUG
void ModPackage::PrintLinks(NameValueCollection^ links)
{
	for each (String^ key in links->AllKeys)
		Console::WriteLine("{0}: {1}", key, links[key]);
}
#endif

bool ModPackage::Add(ContentType contentType, Object^ object)
{
	if ((unsigned int)(contentType & ContentType::BIG) != 0)
    {
		AddAssetEntry(contentType, (AssetEntry^)object);
    	return true;
    }
	else if ((unsigned int)(contentType & ContentType::BIN) != 0)
    {
		BINEntry^ entry = (BINEntry^)object;

		if (entry->Name == "")
			return false;

		BIGBank^ bank = GetBank(contentType);
        
        AssetEntry^ newEntry = gcnew AssetEntry(
			entry->Name, 
			entry->ID,
			0, 
			bank);
		
		bank->AddEntry(newEntry);

		newEntry->Data = entry->Data;

		IntPtr pdef = Marshal::StringToHGlobalAnsi(entry->Definition);
		
		newEntry->SetSubHeader(
			(char*)pdef.ToPointer(), entry->Definition->Length+1);
		
		Marshal::FreeHGlobal(pdef);

		NameValueCollection^ links = gcnew NameValueCollection();
		
		DefinitionType^ def = 
			ContentManager::Instance->Definitions->GetDefinition(
				entry->Definition);
		
		if (def)
		{
			def->ReadIn(entry);
			
			for (int i = 0; i < def->Controls->Count; ++i)
			{
				CreateLinks(
					links, 
					def->Controls[i]->ID.ToString(), 
					def->Controls[i]->Members);
			}

			if (def->HasCDefListing)
				CreateLinks(links, "",  def->CDefs); 
		}
	
	#ifdef _DEBUG
		Console::WriteLine("Links for {0}", entry->Name);
		PrintLinks(links);
	#endif

		MemoryStream^ stream = gcnew MemoryStream();

		IFormatter^ formatter = gcnew BinaryFormatter();
		formatter->Serialize(stream, links);

		array<System::Byte>^ data = stream->GetBuffer();
		pin_ptr<System::Byte> p = &data[0];
		
		BIGBank^ linkBank = GetLinkBank(contentType);

		newEntry = gcnew AssetEntry(
			entry->Name, 
			linkBank->GetNewID(), 
			0, 
			linkBank);

		linkBank->AddEntry(newEntry);

		newEntry->SetData((char*)p, stream->Length);

		stream->Close();
		stream = nullptr;
		formatter = nullptr;
		links = nullptr;
	
		return true;
    }
	
	return false;
}

bool ModPackage::Remove(ContentType entryType, String^ entryName)
{
	return RemoveFromBank(entryType, entryName);
}

bool ModPackage::RemoveFromBank(ContentType entryType, String^ entryName)
{
	BIGBank^ bank = GetBank(entryType);
		
	AssetEntry^ removeEntry = bank->FindEntryBySymbolName(entryName);
    
	if (removeEntry)
	{
		RemoveLinks(entryType, entryName);
		return bank->RemoveEntry(removeEntry);
	}

	return false;
}

bool ModPackage::IsValid(ContentType contentType, Object^ object)
{
	BIGBank^ bank = GetBank(contentType);
		
	if ((unsigned int)(contentType & ContentType::BIG) != 0)
    {
		AssetEntry^ entry = (AssetEntry^)object;
		return bank->FindEntryBySymbolName(entry->DevSymbolName) == nullptr;
    }
	else if ((unsigned int)(contentType & ContentType::BIN) != 0)
	{
		BINEntry^ entry = (BINEntry^)object;
    
		if (entry->Name == "")
			return false;

		return bank->FindEntryBySymbolName(entry->Name) == nullptr;
	}
	
	return false;
}

String^ ModPackage::Description::get()
{
	return GetSettings("Description");
}

void ModPackage::Description::set(String^ value)
{
	SetSettings("Description", value);
}

String^ ModPackage::GetSettings(String^ key)
{
	BIGBank^ bank = m_Big->Banks["Settings"];
	
	if (!bank)
		return gcnew String("");

	AssetEntry^ entry = bank->FindEntryBySymbolName(key);
	
	if (!entry)
		return gcnew String("");

	return gcnew String((wchar_t*)entry->GetData());
}

void ModPackage::SetSettings(String^ key, String^ val)
{
	int iLen = val->Length;

	System::IntPtr ptr = Marshal::StringToHGlobalUni(val);

	BIGBank^ bank = m_Big->Banks["Settings"];

	if (!bank)
	{
		bank = gcnew BIGBank("Settings", m_Big->BankCount, 1, m_Big);
		m_Big->AddBank(bank);
	}

	AssetEntry^ entry = bank->FindEntryBySymbolName(key);

	if (!entry)
	{
		entry = gcnew AssetEntry("Description", 0, 0, bank);
		bank->AddEntry(entry);
	}

	entry->SetData((char*)ptr.ToPointer(), (iLen+1)*2);

	Marshal::FreeHGlobal(ptr);
}

BIGBank^ ModPackage::GetBank(ContentType contentType)
{
	return m_Big->Banks[GetBankName(contentType)];
}

BIGBank^ ModPackage::GetLinkBank(ContentType contentType)
{
	return m_Big->Banks[GetLinkBankName(contentType)];
}

String^ ModPackage::GetBankName(ContentType contentType)
{
	switch (contentType)
	{
	case ContentType::MainTextures:
		return gcnew String("MainTextures");
	case ContentType::Graphics:
		return gcnew String("Graphics");
	case ContentType::Text:
		return gcnew String("Text");
	case ContentType::Objects:
		return gcnew String("Objects");
	case ContentType::Scripts:
		return gcnew String("Scripts");
	case ContentType::GUITextures:
		return gcnew String("GUITextures");
	case ContentType::FrontEndTextures:
		return gcnew String("FrontEndTextures");
	}

	System::Diagnostics::Debug::Assert(false);
	return nullptr;
}

String^ ModPackage::GetLinkBankName(ContentType contentType)
{
	switch (contentType)
	{
	case ContentType::BIG:
	case ContentType::Graphics:
	case ContentType::Text:
	case ContentType::MainTextures:
	case ContentType::GUITextures:
	case ContentType::FrontEndTextures:
		return gcnew String("AssetLinks");
	
	case ContentType::BIN:
	case ContentType::Objects:
	case ContentType::Scripts:
		return gcnew String("ObjectLinks");
	}

	System::Diagnostics::Debug::Assert(false);
	return nullptr;
}

}
}