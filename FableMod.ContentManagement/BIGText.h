#pragma once

using namespace System;
using namespace FableMod::BIG;

namespace FableMod
{
namespace ContentManagement
{

public ref class BIGText
{
public:
	
	BIGText();
	
	BIGText(AssetEntry^ ent);

	~BIGText();

	void ApplyToEntry(AssetEntry^ entry);
	
	property String^ Content
	{
		String^ get();
		void set(String^);
	}
	
	property String^ SoundFile
	{
		String^ get();
		void set(String^);
	}
	
	property String^ Speaker
	{
		String^ get();
		void set(String^);
	}
	
	property String^ Identifier
	{
		String^ get();
		void set(String^);
	}
	
	property array<String^>^ Modifiers
	{
		array<String^>^ get();
		void set(array<String^>^);
	}
	
private:
	wchar_t*			m_Content;
	char*				m_SoundFile;
	char*				m_Speaker;
	char*				m_Identifier;
	char**				m_Modifiers;
	unsigned __int32	m_ModCount;
};

}
}