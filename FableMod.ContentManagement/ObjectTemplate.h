#pragma once

#include "ContentManager.h"
#include "DefinitionType.h"

using namespace System;
using namespace System::Xml;
using namespace System::Collections::Generic;

using namespace FableMod::BIN;

namespace FableMod
{
namespace ContentManagement
{

ref class DefTypeTemplate;

public ref class BaseTemplate
{
public:

	virtual ~BaseTemplate() {}

	property String^ Name
	{
		String^ get() { return m_Name; }
		void set(String^ value) { m_Name = value; }
	}

	property DefTypeTemplate^ LinkTo
	{
		DefTypeTemplate^ get() { return m_Link; }
		void set(DefTypeTemplate^ value) { m_Link = value; }
	}

	property bool Named
	{
		bool get() { return m_Named; }
		void set(bool value) { m_Named = value; }
	}

	property bool Ref
	{
		bool get() { return m_Ref; }
		void set(bool value) { m_Ref = value; }
	}

	property int ID
	{
		int get() { return m_Id; }
		void set(int value) { m_Id = value; }
	}

internal:

private:

	DefTypeTemplate^ m_Link;
	String^ m_Name;
	bool m_Named;
	int m_Id;
	bool m_Ref;
};

public ref class DefTypeTemplate : BaseTemplate
{
public:

	virtual ~DefTypeTemplate() {}

	property int OriginalID
	{
		int get() { return m_OriginalId; }
		void set(int value) { m_OriginalId = value; }
	}

	property String^ Type
	{
		String^ get() { return m_Type; }
		void set(String^ value) { m_Type = value; }
	}

internal:

private:

	String^ m_Type;
	int m_OriginalId;
};

public ref class AssetTemplate : BaseTemplate
{
public:

	virtual ~AssetTemplate() {}

	property UInt32 ControlID
	{
		UInt32 get() { return m_Control; }
		void set(UInt32 value) { m_Control = value; }
	}

	property int Element
	{
		int get() { return m_Element; }
		void set(int value) { m_Element = value; }
	}

	property ContentType Type
	{
		ContentType get() { return m_Type; }
		void set(ContentType value) { m_Type = value; }
	}

	property String^ Prefix
	{
		String^ get();
	}

internal:
	
private:

	UInt32 m_Control;
	int m_Element;
	ContentType m_Type;
};

public ref class ObjectTemplate
{
public:

	ObjectTemplate();

	virtual ~ObjectTemplate() {}

	void Build(BINEntry^ entry);

	property int ItemCount
	{
		int get() { return m_Items->Count; }
	}

	property BaseTemplate^ Items[int]
	{
		BaseTemplate^ get(int index) { return m_Items[index]; }
	}

private:

	void AddControlMembers(
		DefTypeTemplate^ link,
		String^ controlName, 
		UInt32 control,
		MemberCollection^ c, 
		int* pId);
	
	void Build(
		BINEntry^ entry,
		DefTypeTemplate^ link,
		int* pId);

	List<BaseTemplate^>^ m_Items;
};

}
}