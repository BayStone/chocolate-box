#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Collections::ObjectModel;

#include "VariableBlock.h"

namespace FableMod
{
namespace TNG
{

ref class ElementArray;

/// Block that can contain variables and other arrays.
public ref class ComplexBlock abstract : public VariableBlock 
{
public:

	ComplexBlock();

	virtual ~ComplexBlock();

	virtual Element^ Find(String^ name) override;

	property Collection<ElementArray^>^ Arrays
	{
		Collection<ElementArray^>^ get();
	}

protected:

	virtual bool LoadElement(
		TNGDefinitions^ definitions, XmlNode^ node) override;
};

}
}
