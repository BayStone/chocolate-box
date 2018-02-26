#pragma once

using namespace System;
using namespace FableMod::BIG;

namespace FableMod
{
namespace ContentManagement
{
	
public ref class BIGTextGroup
{
public:
	
	BIGTextGroup();
		
	BIGTextGroup(AssetEntry^ ent);
		
	~BIGTextGroup();
	
	void ApplyToEntry(AssetEntry^ entry);
	
	property array<UInt32>^ Entries
	{
		array<UInt32>^ get();
		void set(array<UInt32>^);
	}
	
private:
	unsigned __int32*	m_Entries;
	unsigned __int32	m_EntryCount;
};

}
}