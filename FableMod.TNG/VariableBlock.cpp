#include "Stdafx.h"
#include "VariableBlock.h"
#include "Variable.h"
#include "TNGDefinitions.h"

namespace FableMod
{
namespace TNG
{

VariableBlock::VariableBlock()
{
}

VariableBlock::~VariableBlock()
{
}

void VariableBlock::Save(TextWriter^ writer)
{
	for (int i = 0; i < ElementCount; ++i)
	{
		Elements[i]->Save(writer);
	}
}

Collection<Variable^>^ VariableBlock::Variables::get()
{
	Collection<Variable^>^ vc = gcnew Collection<Variable^>();
	
	for (int i = 0; i < ElementCount; ++i)
	{
		if (Elements[i]->GetType() == Variable::typeid)
			vc->Add((Variable^)Elements[i]);
	}

	return vc;
}

Variable^ VariableBlock::Variables::get(String^ name)
{
	for (int i = 0; i < ElementCount; ++i)
	{
		if (Elements[i]->GetType() == Variable::typeid &&
			Elements[i]->Name == name)
		{
			return (Variable^)Elements[i];
		}
	}

	return nullptr;
}


bool VariableBlock::LoadElement(TNGDefinitions^ definitions, 
								XmlNode^ node)
{
	if (Block::LoadElement(definitions, node))
		return true;

	if (node->Name == "variable")
	{
		// Overwrite or create the variable.

		Variable^ v = (Variable^)Find(
			node->Attributes["name"]->InnerText);
		
		if (v == nullptr)
		{
			v = gcnew Variable();
			v->Load(definitions, node);
			Add(v);
		}
		else
		{
			// Already have this... due to inheritance.
			v->Load(definitions, node);
		}

		return true;
	}
	else if (node->Name == "varref")
	{
		Variable^ v = (Variable^)definitions->Find(
			node->Attributes["name"]->InnerText);
		
		if (v != nullptr)
		{
			Add(v->Duplicate());
		}

		return true;
	}

	return false;
}

}
}