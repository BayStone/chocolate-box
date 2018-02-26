#pragma once

using namespace System;
using namespace System::Collections::Generic;

#include "Member.h"
#include "DefinitionDB.h"
#include "AssetLinker.h"

using namespace FableMod::BIG;

namespace FableMod
{
namespace ContentManagement
{

public enum class ContentType : unsigned int
{
    Unknown = 0,
	Names = 1,
    BIG = 2,
	BIN = 4,
    Graphics = BIG | (1 << 10),
    Text = BIG | (1 << 11),
    MainTextures = BIG | (1 << 12),
	GUITextures = BIG | (1 << 13),
	FrontEndTextures = BIG | (1 << 14),
	Objects = BIN | (1 << 20),
	Scripts = BIN | (1 << 21),
};

/// <summary>
/// Not all game assets have the same base type, so
/// ContentObject acts as a simple interface to hide this issue.
/// </summary>
public ref class ContentObject
{
public:

	ContentObject(String^ name, Object^ object) :
	m_Name(name),
	m_Object(object),
	m_Type(ContentType::Unknown)
	{
	}

    ContentObject(String^ name, Object^ object, ContentType contentType) :
	m_Name(name),
	m_Object(object),
    m_Type(contentType)
	{
	}

	~ContentObject() 
	{
		m_Name = nullptr;
		m_Object = nullptr;
	}

	property String^ Name 
	{ 
		String^ get() { return m_Name; } 
	}

	property System::Object^ Object 
	{ 
		System::Object^ get() { return m_Object; }
	}

    property ContentType Type
	{ 
		ContentType get() { return m_Type; }
	}
    
protected:

	String^ m_Name;
	System::Object^ m_Object;
    ContentType m_Type;
};

/// <summary>
/// Handle various tasks related to content. Links
/// between objects and what not.
/// </summary>
public ref class ContentManager abstract
{
public:

	virtual ~ContentManager();

	virtual ContentObject^ FindEntry(
		LinkDestination dst, Object^ object);

	virtual void ShowEntry(Object^ object, bool dialog) = 0;
	
	virtual void ShowEntry(
		LinkDestination destination, Object^ object, bool dialog);
	
	virtual Object^ SelectEntry(
		Link^ link, Object^ current) = 0;
	
    virtual AssetEntry^ UpdateEntry(
		ContentType contentType, AssetEntry^ entry);

	virtual AssetLinker^ CreateAssetLinker(
		ContentType contentType, AssetEntry^ entry);

	DefinitionType^ FindObjectDefinitionType(String^ objectName);

	property DefinitionDB^ Definitions { virtual DefinitionDB^ get() = 0; }
	
	property String^ DataDirectory { virtual String^ get() = 0; }
	
	static property ContentManager^ Instance 
	{ 
		ContentManager^ get() { return sm_Instance; }
	}

	property BIGFile^ Textures
	{ 
		BIGFile^ get() 
		{ 
			AutoLoadTextures(); 
			return m_Textures; 
		}
	}

	property BIGBank^ MainTextureBank
	{ 
		BIGBank^ get() 
		{ 
			AutoLoadTextures(); 
			return GetMainTextureBank(m_Textures); 
		}
	}

	property BIGBank^ GUITextureBank
	{ 
		BIGBank^ get() 
		{ 
			AutoLoadTextures(); 
			return GetGUITextureBank(m_Textures); 
		}
	}

	property BINFile^ Scripts
	{ 
		BINFile^ get() 
		{ 
			AutoLoadScripts(); 
			return m_Scripts;
		}
	}

	property BIGBank^ GraphicsBank
	{ 
		BIGBank^ get() { AutoLoadGraphics(); return m_GraphicsBank; }
	}

	property BIGBank^ TextBank
	{ 
		BIGBank^ get() { AutoLoadText(); return m_TextBank; }
	}

	property BIGBank^ FrontEndTextureBank
	{ 
		BIGBank^ get() 
		{ 
			AutoLoadFrontEndTextures(); 
			return m_FrontEndTextureBank; 
		}
	}

	property BINFile^ Objects { BINFile^ get(); }

	property NamesBINFile^ Names
	{ 
		NamesBINFile^ get() { return m_Names; }
	}

protected:

	static void SetInstance(ContentManager^ manager)
	{
		sm_Instance = manager;
	}

	void ResetBIG();

	ContentManager();

	virtual BIGBank^ LoadGraphics() = 0;
	virtual BIGFile^ LoadTextures() = 0;
	virtual NamesBINFile^ LoadNames() = 0;
	virtual BINFile^ LoadObjects() = 0;
	virtual BINFile^ LoadScripts() = 0;
	virtual BIGBank^ LoadText() = 0;
	virtual BIGBank^ LoadFrontEndTextures() = 0;

	virtual BIGBank^ GetMainTextureBank(BIGFile^ textures) = 0;
	virtual BIGBank^ GetGUITextureBank(BIGFile^ textures) = 0;

	bool AutoLoadGraphics();
	bool AutoLoadTextures();
	bool AutoLoadNames();
	bool AutoLoadObjects();
	bool AutoLoadText();
	bool AutoLoadScripts();
	bool AutoLoadFrontEndTextures();

private:

	AssetEntry^ UpdateBIGEntry(BIGBank^ bank, AssetEntry^ newEntry);

	static ContentManager^ sm_Instance;
			
	Dictionary<String^, DefinitionType^>^ m_ObjectDefTypes;
	
	BINFile^ m_Objects;
	BINFile^ m_Scripts;
	NamesBINFile^ m_Names;
	BIGBank^ m_GraphicsBank;
	BIGFile^ m_Textures;
	BIGBank^ m_TextBank;
	BIGBank^ m_FrontEndTextureBank;
};

}
}