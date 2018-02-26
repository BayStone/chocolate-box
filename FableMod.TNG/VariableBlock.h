#pragma once

using namespace System;
using namespace System::IO;

#include "Block.h"
#include "Variable.h"

namespace FableMod
{
namespace TNG
{

public ref class VariableBlock abstract : public Block 
{
public:

	VariableBlock();

	virtual ~VariableBlock();

	virtual void Save(TextWriter^ writer) override;

	property Collection<Variable^>^ Variables
	{
		Collection<Variable^>^ get();
	}

	property Variable^ Variables[String^]
	{
		Variable^ get(String^ name);
	}

protected:

	virtual bool LoadElement(
		TNGDefinitions^ definitions, XmlNode^ node) override;

};

}
}
