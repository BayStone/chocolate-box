#include "Stdafx.h"
#include "TNGDefinitionType.h"
#include "Variable.h"
#include "TNGDefinitions.h"
#include "CTCBlock.h"

namespace FableMod
{
namespace TNG
{

TNGDefinitionType::TNGDefinitionType()
{
}

TNGDefinitionType::~TNGDefinitionType()
{
#ifdef _DEBUG
	//Console::WriteLine("TNGDefinitionType({0})::Destructor", Name);
#endif
}

Element^ TNGDefinitionType::Factory()
{
	return gcnew TNGDefinitionType();
}

void TNGDefinitionType::CopyTo(Element^ element)
{
	ComplexBlock::CopyTo(element);

	((TNGDefinitionType^)element)->m_Graphic = m_Graphic;
}

void TNGDefinitionType::Load(TNGDefinitions^ definitions, XmlNode^ node)
{
	XmlAttribute^ graphicAttr = node->Attributes["graphic"];
	
	ComplexBlock::Load(definitions, node);

	if (graphicAttr != nullptr)
		m_Graphic = graphicAttr->InnerText;
}

bool TNGDefinitionType::LoadElement(TNGDefinitions^ definitions, 
									XmlNode^ node)
{
	if (ComplexBlock::LoadElement(definitions, node))
		return true;

	if (node->Name == "ctc")
	{
		CTCBlock^ ctc = (CTCBlock^)Find(
			node->Attributes["name"]->InnerText);
		
		if (ctc == nullptr)
		{
			ctc = gcnew CTCBlock();
			ctc->Load(definitions, node);
			Add(ctc);
		}
		else
		{
			ctc->Load(definitions, node);
		}
	
		return true;
	}
	else if (node->Name == "ctcref")
	{
		CTCBlock^ ctc = (CTCBlock^)definitions->Find(
			node->Attributes["name"]->InnerText);
		
		if (ctc != nullptr)
		{
			Add(ctc->Duplicate());
		}

		return true;
	}
	
	return false;
}


}
}