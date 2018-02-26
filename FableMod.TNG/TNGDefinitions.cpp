#include "Stdafx.h"
#include "TNGDefinitions.h"
#include "CTCBlock.h"
#include "Variable.h"
#include "TNGDefinitionType.h"

namespace FableMod
{
namespace TNG
{

TNGDefinitions::TNGDefinitions()
{
}

TNGDefinitions::~TNGDefinitions()
{
}

void TNGDefinitions::Load(String^ fileName)
{
	Clear();
	
	XmlDocument^ doc = gcnew XmlDocument();

	doc->Load(fileName);

	XmlNode^ root = doc["tngdefinitions"];

	XmlNode^ child = root->FirstChild;

	while (child != nullptr)
	{
		LoadElement(this, child);
		child = child->NextSibling;
	}
}

bool TNGDefinitions::LoadElement(TNGDefinitions^ definitions, 
								 XmlNode^ node)
{
	if (TNGDefinitionType::LoadElement(definitions, node))
		return true;

	if (node->Name == "deftype")
	{
		TNGDefinitionType^ def = gcnew TNGDefinitionType();

		def->Load(definitions, node);

		Add(def);

		return true;
	}

	return false;
}

}
}