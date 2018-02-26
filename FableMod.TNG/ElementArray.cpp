#include "Stdafx.h"
#include "ElementArray.h"
#include "TNGDefinitions.h"

namespace FableMod
{
namespace TNG
{

ElementArray::ElementArray()
{
}

ElementArray::~ElementArray()
{
	m_Base = nullptr;
}

Element^ ElementArray::Factory()
{
	return gcnew ElementArray();
}

Element^ ElementArray::Add()
{
	Element^ element = m_Base->Duplicate();
	
	element->Name = 
		element->Name->Replace("*", ElementCount.ToString());
	
	Add(element);

	Modified = true;

	return element;
}

Element^ ElementArray::MatchAdd(String^ name)
{
	int index1 = name->IndexOf("[");
	int index2 = name->IndexOf("]", index1+1);

	String^ testName1 =
		name->Substring(0,index1+1)+
		name->Substring(index2);
	
	int index3 = m_Base->Name->IndexOf("[");
	int index4 = m_Base->Name->IndexOf("]", index3+1);

	String^ testName2 =
		m_Base->Name->Substring(0,index3+1)+
		m_Base->Name->Substring(index4);

	if (testName1 == testName2)
	{
		int elementIndex = int::Parse(
			name->Substring(index1+1, (index2-index1)-1));
	
		while (elementIndex >= m_Elements->Count)
		{
			Add();
		}

		Element^ element = m_Elements[elementIndex];

		if (index2 < name->Length-1)
		{
			// Doesn't end with the []. So either more struct
			// members or arrays.
		}

		return element;
	}

	return nullptr;
}

void ElementArray::CopyTo(Element^ element)
{
	VariableBlock::CopyTo(element);
	((ElementArray^)element)->m_Base = m_Base->Duplicate();
}

void ElementArray::Load(TNGDefinitions^ definitions, XmlNode^ node)
{
	Element::Load(definitions, node);
	
	node = node->FirstChild;

	if (node->Name == "array")
	{
		// Overwrite or create the variable.

		ElementArray^ ar = (ElementArray^)Find(
			node->Attributes["name"]->InnerText);
		
		if (ar == nullptr)
		{
			ar = gcnew ElementArray();
			ar->Load(definitions, node);
		}
		else
		{
			// Already have this... due to inheritance.
			ar->Load(definitions, node);
		}

		m_Base = ar;
	}
	else if (node->Name == "arrayref")
	{
		m_Base = definitions->Find(
			node->Attributes["name"]->InnerText)->Duplicate();
	}
	else if (node->Name == "variable")
	{
		// Overwrite or create the variable.

		Variable^ v = (Variable^)Find(
			node->Attributes["name"]->InnerText);
		
		if (v == nullptr)
		{
			v = gcnew Variable();
			v->Load(definitions, node);
		}
		else
		{
			// Already have this... due to inheritance.
			v->Load(definitions, node);
		}

		m_Base = v;
	}
	else if (node->Name == "varref")
	{
		m_Base = definitions->Find(
			node->Attributes["name"]->InnerText)->Duplicate();
	}

	if (!m_Base)
	{
		throw gcnew System::Exception(
			"FableMod::TNG: Array base member not defined");
	}
}

}
}