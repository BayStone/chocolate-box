#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Collections::ObjectModel;

#include "ComplexBlock.h"

namespace FableMod
{
namespace TNG
{

public ref class ElementStruct : public ComplexBlock
{
public:

	ElementStruct();

	virtual ~ElementStruct();

protected:

	virtual Element^ Factory() override;
};

}
}
