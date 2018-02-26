#include "Stdafx.h"
#include "Variable.h"

namespace FableMod
{
namespace TNG
{

Variable::Variable() :
m_Type(VariableType::Unknown),
m_Value(gcnew String("")),
m_Modified(false),
m_Default(nullptr)
{
	if (m_StaticCulture == nullptr)
	{
		m_StaticCulture = gcnew System::Globalization::CultureInfo("en-US");
		m_StaticCulture->NumberFormat->NumberDecimalDigits = 6;
		m_StaticCulture->NumberFormat->NumberGroupSeparator = "";
	}
}

Variable::~Variable()
{
	m_Value = nullptr;
	m_Default = nullptr;
}

void Variable::Save(TextWriter^ writer)
{
	if (IsDefault() && !SaveDefault)
		return;

	String^ s = StringValue;

	if (Type == VariableType::QuoteString ||
		Type == VariableType::GameEnum)
	{
		s = "\""+s+"\"";
	}

	writer->WriteLine("{0} {1};", Name, s);
}

Element^ Variable::Factory()
{
	return gcnew Variable();
}

void Variable::CopyTo(Element^ element)
{
	Element::CopyTo(element);

	Variable^ v = (Variable^)element;
	
	v->m_Type = m_Type;
	v->m_Value = m_Value;
	v->m_Default = m_Default;
    v->m_Restriction = m_Restriction;
}

void Variable::Load(TNGDefinitions^ definitions, XmlNode^ node)
{
	Element::Load(definitions, node);

	XmlAttribute^ typeAttr = node->Attributes["type"];

	if (typeAttr != nullptr)
	{
		m_Type = (VariableType)Enum::Parse(
			VariableType::typeid, typeAttr->InnerText, true);
	}

    XmlAttribute^ restAttr = node->Attributes["restriction"];

    if (restAttr != nullptr)
    {
        m_Restriction = restAttr->InnerText;
    }
    
	SetRawValue(node->Attributes["value"]->InnerText);
	m_Default = m_Value;
}

Object^ Variable::Value::get()
{
	return m_Value;
}

String^ Variable::StringValue::get()
{
	if (Type == VariableType::Float)
	{
		// We want the floats in a very strict format.
		// No exponents, and no commas as decimal points.
		// Meters, miles, pounds, dollars and euros, FUCK THAT!
		return ((float)m_Value).ToString("N5", m_StaticCulture);
	}
	else if (Type == VariableType::Boolean)
	{
		return m_Value->ToString()->ToUpper();
	}
	
	return m_Value->ToString();
}

void Variable::SetRawValue(Object^ value)
{
	if (Type == VariableType::Unknown)
	{
		// Not set, yet.

		String^ tmp = value->ToString();

		if (tmp->StartsWith("\"") && tmp->EndsWith("\""))
		{
			m_Type = VariableType::QuoteString; 
		}
		else if (tmp == "TRUE" || tmp == "FALSE")
		{
			m_Type = VariableType::Boolean;
		}
		else
		{
			m_Type = VariableType::String;
		}

		tmp = nullptr;
	}

	if (Type == VariableType::Float)
	{
		if (value->GetType() == String::typeid)
		{
			m_Value = float::Parse((String^)value, m_StaticCulture);
		}
		else
		{
			m_Value = value;
		}
	}
	else if (Type == VariableType::Integer)
		m_Value = int::Parse(value->ToString());
	else if (Type == VariableType::UID)
		m_Value = value->ToString();
	else if (Type == VariableType::Boolean)
		m_Value = bool::Parse(value->ToString());
	else if (Type == VariableType::QuoteString ||
			 Type == VariableType::GameEnum ||
			 Type == VariableType::String)
	{
		String^ val = value->ToString();

		// We don't need the quotes for the values.
		if (val->StartsWith("\"") && val->EndsWith("\""))
		{
			val = val->Substring(1, val->Length-2);
		}

		m_Value = val;

		val = nullptr;
	}
}

void Variable::Value::set(Object^ value)
{
	if (!m_Value->Equals(value))
	{
		SetRawValue(value);
		Modified = true;
		OnChanged(this);
	}
}

bool Variable::Modified::get()
{
	return m_Modified;
}

void Variable::Modified::set(bool value)
{
	m_Modified = value;
}

bool Variable::IsDefault()
{
	return m_Default != nullptr && m_Default->Equals(m_Value);
}

void Variable::ToDefault()
{
	if (m_Default != nullptr)
		Value = m_Default;
}

bool Variable::HasDefault::get()
{
	return m_Default != nullptr;
}

}
}