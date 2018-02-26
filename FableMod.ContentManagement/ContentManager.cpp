#include "Stdafx.h"
#include "ContentManager.h"

namespace FableMod
{
namespace ContentManagement
{

ContentManager::ContentManager() :
m_ObjectDefTypes(gcnew Dictionary<String^, DefinitionType^>())
{
}

ContentManager::~ContentManager()
{
	m_ObjectDefTypes = nullptr;
}

void ContentManager::ResetBIG()
{
	m_GraphicsBank = nullptr;
	m_TextBank = nullptr;
	m_Textures = nullptr;
	m_FrontEndTextureBank = nullptr;
}

bool ContentManager::AutoLoadGraphics()
{
	if (!m_GraphicsBank)
		m_GraphicsBank = LoadGraphics();

	return m_GraphicsBank != nullptr;
}

bool ContentManager::AutoLoadTextures()
{
	if (!m_Textures)
		m_Textures = LoadTextures();

	return m_Textures != nullptr;
}

bool ContentManager::AutoLoadFrontEndTextures()
{
	if (!m_FrontEndTextureBank)
		m_FrontEndTextureBank = LoadFrontEndTextures();

	return m_FrontEndTextureBank != nullptr;
}

bool ContentManager::AutoLoadNames()
{
	if (!m_Names)
		m_Names = LoadNames();

	return m_Names != nullptr;
}

bool ContentManager::AutoLoadObjects()
{
	if (!m_Objects)
		m_Objects = LoadObjects();

	return m_Objects != nullptr;
}

bool ContentManager::AutoLoadScripts()
{
	if (!m_Scripts)
		m_Scripts = LoadScripts();

	return m_Scripts != nullptr;
}

bool ContentManager::AutoLoadText()
{
	if (!m_TextBank)
		m_TextBank = LoadText();

	return m_TextBank != nullptr;
}

ContentObject^ ContentManager::FindEntry(LinkDestination dst, 
										 Object^ o)
{
	if (dst == LinkDestination::NamesBINOffset)
    {
        if (!AutoLoadNames())
            return nullptr;

        NamesBINEntry^ entry = m_Names->GetEntryByOffset(
			UInt32::Parse(o->ToString()));

        if (entry)
            return gcnew ContentObject(
                entry->Name, 
                entry,
				ContentType::Names);
    }
    else if (dst == LinkDestination::NamesBINEnum)
    {
        if (!AutoLoadNames())
            return nullptr;
        
		NamesBINEntry^ entry = m_Names->GetEntryByEnum(
			UInt32::Parse(o->ToString()));

        if (entry)
            return gcnew ContentObject(
                entry->Name, 
                entry,
				ContentType::Names);
    }
    else if (dst == LinkDestination::GameBINEntryID)
    {
        if (AutoLoadObjects())
		{
			int id = int::Parse(o->ToString());

			if (id >= 0)
			{
				BINEntry^ entry = m_Objects->Entries[id];

				if (entry)
				{
					return gcnew ContentObject(
						entry->Name, 
						entry,
						ContentType::Objects);
				}
			}
		}
    }
	else if (dst == LinkDestination::ScriptBINEntryID)
    {
        if (AutoLoadScripts())
		{
			int id = int::Parse(o->ToString());

			if (id >= 0)
			{
				BINEntry^ entry = m_Scripts->Entries[id];

				if (entry)
				{
					return gcnew ContentObject(
						entry->Name, 
						entry,
						ContentType::Scripts);
				}
			}
		}
    }
    else if (dst == LinkDestination::GameBINEntryName)
    {
        if (AutoLoadObjects())
		{
			BINEntry^ entry = m_Objects->GetEntryByName(
				o->ToString());

			if (entry)
				return gcnew ContentObject(
					entry->Name, 
					entry,
					ContentType::Objects);
		}
    }
    else if (dst == LinkDestination::MainTextureID)
    {
        if (AutoLoadTextures())
		{
			AssetEntry^ entry = nullptr;

			try
			{
				entry = MainTextureBank->FindEntryByID(
					UInt32::Parse(o->ToString()));
			}
			catch (Exception^)
			{
			}

			if (entry)
			{
				return gcnew ContentObject(
					entry->DevSymbolName, 
					entry, 
					ContentType::MainTextures);
	        }
		}
    }
	else if (dst == LinkDestination::FrontEndTextureID)
    {
        if (AutoLoadFrontEndTextures())
		{
			AssetEntry^ entry = nullptr;

			try
			{
				entry = m_FrontEndTextureBank->FindEntryByID(
					UInt32::Parse(o->ToString()));
			}
			catch (Exception^)
			{
			}

			if (entry)
			{
				return gcnew ContentObject(
					entry->DevSymbolName, 
					entry, 
					ContentType::FrontEndTextures);
	        }
		}
    }
    else if (dst == LinkDestination::MainTextureName)
    {
        if (AutoLoadTextures())
		{
			AssetEntry^ entry = MainTextureBank->FindEntryBySymbolName(
				o->ToString());

			if (entry)
			{
				return gcnew ContentObject(
					entry->DevSymbolName,
					entry,
					ContentType::MainTextures);
			}
		}
    }
    else if (dst == LinkDestination::ModelID)
    {
        if (AutoLoadGraphics())
        {
            AssetEntry^ entry = m_GraphicsBank->FindEntryByID(
				UInt32::Parse(o->ToString()));

            if (entry)
            {
                return gcnew ContentObject(
                    entry->DevSymbolName, 
                    entry, 
					ContentType::Graphics);
            }
        }
    }
    else if (dst == LinkDestination::ModelName)
    {
        if (AutoLoadGraphics())
        {
            AssetEntry^ entry = m_GraphicsBank->FindEntryBySymbolName(
				o->ToString());

            if (entry)
            {
                return gcnew ContentObject(
                    entry->DevSymbolName, 
                    entry, 
					ContentType::Graphics);
            }
        }
    }
	else if (dst == LinkDestination::TextID || 
			 dst == LinkDestination::TextGroupID)
    {
        if (AutoLoadText())
        {
            AssetEntry^ entry = m_TextBank->FindEntryByID(
				UInt32::Parse(o->ToString()));

            if (entry)
            {
                return gcnew ContentObject(
                    entry->DevSymbolName, 
                    entry, 
					ContentType::Text);
            }
        }
    }

    return nullptr;

}

AssetEntry^ ContentManager::UpdateBIGEntry(BIGBank^ bank, 
										   AssetEntry^ newEntry)
{
    AssetEntry^ entry = 
        bank->FindEntryBySymbolName(newEntry->DevSymbolName);

    if (entry != nullptr)
    {
        entry->Copy(newEntry);
    }
    else
    {
        entry = gcnew AssetEntry(newEntry, bank);
        
		// Fix 26/03/2009.
        entry->ID = bank->GetNewID();

		bank->AddEntry(entry);
    }
    
    return entry;
}

AssetEntry^ ContentManager::UpdateEntry(ContentType contentType, 
										AssetEntry^ entry)
{
	if (contentType == ContentType::Names)
    {
    }
    else if (contentType == ContentType::MainTextures)
    {
        if (AutoLoadTextures())
        {
            return UpdateBIGEntry(MainTextureBank, entry);
        }
    }
	else if (contentType == ContentType::GUITextures)
    {
        if (AutoLoadTextures())
        {
            return UpdateBIGEntry(GUITextureBank, entry);
        }
    }
	else if (contentType == ContentType::FrontEndTextures)
    {
        if (AutoLoadFrontEndTextures())
        {
            return UpdateBIGEntry(FrontEndTextureBank, entry);
        }
    }
	else if (contentType == ContentType::Text)
    {
        if (AutoLoadText())
        {
            return UpdateBIGEntry(TextBank, entry);
        }
    }
	else if (contentType == ContentType::Graphics)
    {
        if (AutoLoadGraphics())
        {
            return UpdateBIGEntry(GraphicsBank, entry);
        }
    }

    return nullptr;
}

void ContentManager::ShowEntry(LinkDestination destination, 
							   Object^ object,
							   bool dialog)
{
    ContentObject^ obj = FindEntry(destination, object);

    if (obj != nullptr)
        ShowEntry(obj->Object, dialog);
}

DefinitionType^ ContentManager::FindObjectDefinitionType(String^ objectName)
{
	ContentObject^ obj = FindEntry(
		LinkDestination::GameBINEntryName, objectName);

    if (obj != nullptr)
	{
		BINEntry^ entry = (BINEntry^)obj->Object;

		DefinitionType^ defType = nullptr;

		if (m_ObjectDefTypes->TryGetValue(objectName, defType))
			return defType;
		
		defType = Definitions->GetDefinition(entry->Definition);

		if (defType)
		{
			defType->ReadIn(entry);
			m_ObjectDefTypes[objectName] = defType;
		}

		entry = nullptr;
		obj = nullptr;

		return defType;
    }

	return nullptr;
}

BINFile^ ContentManager::Objects::get()
{
	AutoLoadObjects();
	return m_Objects; 
}

AssetLinker^ ContentManager::CreateAssetLinker(ContentType contentType, 
											   AssetEntry^ entry)
{
	return nullptr;
}

}
}