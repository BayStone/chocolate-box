#pragma once

using namespace System;
using namespace System::IO;

#include "Element.h"

namespace FableMod
{
namespace TNG
{

public enum class VariableType : unsigned int
{
	Unknown = 0,
	String,
	QuoteString,
	Float,
	Integer,
	Boolean,
	UID,
	GameEnum
};

public ref class Variable : public Element
{
public:

	Variable();

	virtual ~Variable();

	virtual void Load(TNGDefinitions^ definitions, XmlNode^ node) override;

	virtual void Save(TextWriter^ writer) override;

	virtual void CopyTo(Element^ element) override;

	property VariableType Type 
	{ 
		VariableType get() { return m_Type; }
	}

    property String^ Restriction
	{ 
		String^ get() { return m_Restriction; }
		void set(String^ value) { m_Restriction = value; }
	}
    
	property Object^ Value 
	{ 
		Object^ get();
		void set(Object^ value);
	}
	
	property String^ StringValue 
	{ 
		String^ get();
	}

	property bool Modified
	{
		virtual bool get() override;
		virtual void set(bool value) override;
	}

	virtual void ToDefault() override;

	virtual bool IsDefault() override;

	property bool HasDefault
	{
		virtual bool get() override;
	}
	
internal:
	
	void SetRawValue(Object^ value);

protected:

	static System::Globalization::CultureInfo^ m_StaticCulture = nullptr;
		
	virtual Element^ Factory() override;

	VariableType m_Type;
    String^ m_Restriction;
	Object^ m_Value;
	Object^ m_Default;
	bool m_Modified;
};

}
}
