#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Collections::ObjectModel;
using namespace System::Collections::Generic;

#include "TNGDefinitionType.h"

namespace FableMod
{
namespace TNG
{

public ref class TNGDefinitions : public TNGDefinitionType
{
public:
	
	TNGDefinitions();

	virtual ~TNGDefinitions();

	void Load(String^ fileName);

protected:

	virtual bool LoadElement(
		TNGDefinitions^ definitions, XmlNode^ node) override;
};

}
}
