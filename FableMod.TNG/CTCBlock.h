#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Collections::ObjectModel;

#include "ComplexBlock.h"

namespace FableMod
{
namespace TNG
{

public ref class CTCBlock : public ComplexBlock 
{
public:

	CTCBlock();

	virtual ~CTCBlock();

	virtual void Save(TextWriter^ writer) override;

protected:

	virtual Element^ Factory() override;
};

}
}
