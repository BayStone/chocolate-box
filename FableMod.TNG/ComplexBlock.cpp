#include "Stdafx.h"
#include "ComplexBlock.h"
#include "ElementArray.h"
#include "ElementStruct.h"
#include "TNGDefinitions.h"

namespace FableMod
{
namespace TNG
{

ComplexBlock::ComplexBlock()
{
}

ComplexBlock::~ComplexBlock()
{
}

Element^ ComplexBlock::Find(String^ name)
{
	int indexStruct = name->IndexOf(".");
	int indexArrayL = name->IndexOf("[");
	int indexArrayR = (indexArrayL >= 0) ? name->IndexOf("]") : -1;
	bool bArray = indexArrayL >= 0 && indexArrayR >= 0;

	if (indexStruct >= 0)
	{
		if (!bArray || 
			(indexStruct < indexArrayL && indexStruct < indexArrayR))
		{
			String^ structName = name->Substring(0, indexStruct);
			String^ structVar = name->Substring(indexStruct+1);

			for (int i = 0; i < ElementCount; ++i)
			{
				if (Elements[i]->GetType() == ElementStruct::typeid &&
					Elements[i]->Name == structName)
				{
					Element^ element = 
						((ElementStruct^)Elements[i])->Find(structVar);
					
					if (element != nullptr)
						return element;
				}
			}
		
			return nullptr;
		}
	}

	if (bArray)
	{
		for (int i = 0; i < ElementCount; ++i)
		{
			if (Elements[i]->GetType() == ElementArray::typeid)
			{
				Element^ element = 
					((ElementArray^)Elements[i])->MatchAdd(name);
				
				if (element != nullptr)
					return element;
			}
		}
	
		return nullptr;
	}

	return VariableBlock::Find(name);
}

Collection<ElementArray^>^ ComplexBlock::Arrays::get()
{
	Collection<ElementArray^>^ ar = gcnew Collection<ElementArray^>();
	
	for (int i = 0; i < ElementCount; ++i)
	{
		if (Elements[i]->GetType() == ElementArray::typeid)
			ar->Add((ElementArray^)Elements[i]);
	}

	return ar;
}

bool ComplexBlock::LoadElement(TNGDefinitions^ definitions, 
							 XmlNode^ node)
{
	if (VariableBlock::LoadElement(definitions, node))
		return true;

	if (node->Name == "array")
	{
		// Overwrite or create the variable.

		ElementArray^ ar = (ElementArray^)Find(
			node->Attributes["name"]->InnerText);
		
		if (ar == nullptr)
		{
			ar = gcnew ElementArray();
			ar->Load(definitions, node);
			Add(ar);
		}
		else
		{
			// Already have this... due to inheritance.
			ar->Load(definitions, node);
		}

		return true;
	}
	else if (node->Name == "arrayref")
	{
		ElementArray^ ar = (ElementArray^)definitions->Find(
			node->Attributes["name"]->InnerText);
		
		if (ar != nullptr)
		{
			Add(ar->Duplicate());
		}

		return true;
	}
	else if (node->Name == "struct")
	{
		// Overwrite or create the variable.

		ElementStruct^ ar = (ElementStruct^)Find(
			node->Attributes["name"]->InnerText);
		
		if (ar == nullptr)
		{
			ar = gcnew ElementStruct();
			ar->Load(definitions, node);
			Add(ar);
		}
		else
		{
			// Already have this... due to inheritance.
			ar->Load(definitions, node);
		}

		return true;
	}
	else if (node->Name == "structref")
	{
		ElementStruct^ ar = (ElementStruct^)definitions->Find(
			node->Attributes["name"]->InnerText);
		
		if (ar != nullptr)
		{
			Add(ar->Duplicate());
		}

		return true;
	}
	
	return false;
}

}
}