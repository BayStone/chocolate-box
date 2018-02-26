#pragma once

#include <windows.h>
#include <tchar.h>
#include "GfxConf.h"

namespace FableMod
{
namespace Gfx
{

class GFX_API Exception
{
public:

	Exception();

	virtual ~Exception();

	Exception(LPCTSTR pszMessage);

	const TCHAR* GetMsg() const;

protected:

	TCHAR* m_pszMessage;
};

}
}