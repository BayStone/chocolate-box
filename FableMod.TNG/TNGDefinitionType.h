#pragma once

using namespace System;
using namespace System::IO;

#include "ComplexBlock.h"

namespace FableMod
{
namespace TNG
{

public ref class TNGDefinitionType : public ComplexBlock 
{
public:

	TNGDefinitionType();

	virtual ~TNGDefinitionType();

	virtual void CopyTo(Element^ element) override;

	virtual void Load(TNGDefinitions^ definitions, XmlNode^ node) override;

	property String^ GraphicOverride
	{ 
		String^ get() { return m_Graphic; } 
	}

protected:

	virtual Element^ Factory() override;

	virtual bool LoadElement(
		TNGDefinitions^ definitions, XmlNode^ node) override;

	String^ m_Graphic;
};

}
}
