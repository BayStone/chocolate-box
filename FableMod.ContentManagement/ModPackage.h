#pragma once

using namespace System;
using namespace System::Collections::ObjectModel;

#include "Member.h"
#include "ArrayMember.h"
#include "DefinitionDB.h"
#include "ContentManager.h"

using namespace FableMod::BIG;
using namespace FableMod::BIN;
using namespace FableMod::CLRCore;

namespace FableMod
{
namespace ContentManagement
{
    
public ref class ModPackage
{
public:

    ModPackage();
    
    ~ModPackage();

    void Load(String^ fileName, ProgressInterface^ progress);
    
	void Merge(ModPackage^ package);
    
	void Apply();

    void Save(String^ fileName, ProgressInterface^ progress);
    
    void New();

    bool Add(ContentType contentType, Object^ object);
    
	bool Remove(ContentType entryType, String^ entryName);
   
	bool IsValid(ContentType contentType, Object^ object);

	property String^ Description
	{
		String^ get();
		void set(String^ value);
	}

	property Collection<ContentObject^>^ Items
	{
		Collection<ContentObject^>^ get();
	}

private:
    
    void ApplyBank(ContentType entryType);
	void ApplyBINBank(
		ContentType entryType, Collection<BINEntry^>^ entries);
	void AddAssetEntry(ContentType entryType, AssetEntry^ entry);
	void ItemsAddBank(Collection<ContentObject^>^ c, 
		ContentType contentType);
	String^ GetBankName(ContentType contentType);
    String^ GetLinkBankName(ContentType contentType);
    String^ GetSettings(String^ key);
	BIGBank^ GetBank(ContentType contentType);
	BIGBank^ GetLinkBank(ContentType contentType);
	void SetSettings(String^ key, String^ val);
	void AddAssetLinks(ContentType entryType, AssetEntry^ entry);
	void RemoveLinks(ContentType entryType, String^ entryName);
	void CreateLinks(
		NameValueCollection^ links, String^ path, Object^ member);
	void ApplyLinks(
		NameValueCollection^ links, String^ path, Object^ member);
	void ApplyAssetLinks(ContentType contentType);
	void ApplyBINLinks(
		ContentType contentType, Collection<BINEntry^>^ entries);
	void MergeBank(ModPackage^ package, ContentType entryType, bool bLink);
	bool RemoveFromBank(ContentType entryType, String^ entryName);
#ifdef _DEBUG
	void PrintLinks(NameValueCollection^ links);
#endif

	BIGFile^ m_Big;
};

}
}