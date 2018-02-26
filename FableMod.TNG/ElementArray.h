#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Collections::ObjectModel;

#include "ComplexBlock.h"

namespace FableMod
{
namespace TNG
{

public ref class ElementArray : public ComplexBlock
{
public:

	ElementArray();

	virtual ~ElementArray();

	Element^ Add();

	Element^ MatchAdd(String^ name);

	virtual void CopyTo(Element^ element) override;

	virtual void Load(TNGDefinitions^ definitions, XmlNode^ node) override;

protected:

	virtual Element^ Factory() override;

	Element^ m_Base;
};

}
}
