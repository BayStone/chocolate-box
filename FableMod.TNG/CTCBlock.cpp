#include "Stdafx.h"
#include "CTCBlock.h"
#include "Variable.h"
#include "TNGDefinitions.h"

namespace FableMod
{
namespace TNG
{

CTCBlock::CTCBlock()
{
}

CTCBlock::~CTCBlock()
{
}

void CTCBlock::Save(TextWriter^ writer)
{
	if (IsDefault() && !SaveDefault)
		return;

	writer->WriteLine("Start{0};", Name);

	for (int i = 0; i < ElementCount; ++i)
	{
		Elements[i]->Save(writer);
	}

	writer->WriteLine("End{0};", Name);
}

Element^ CTCBlock::Factory()
{
	return gcnew CTCBlock();
}

}
}