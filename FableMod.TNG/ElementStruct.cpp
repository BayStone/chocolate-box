#include "Stdafx.h"
#include "ElementStruct.h"
#include "TNGDefinitions.h"

namespace FableMod
{
namespace TNG
{

ElementStruct::ElementStruct()
{
}

ElementStruct::~ElementStruct()
{
}

Element^ ElementStruct::Factory()
{
	return gcnew ElementStruct();
}

}
}