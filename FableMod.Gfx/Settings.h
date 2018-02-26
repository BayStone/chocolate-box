#pragma once

#include <windows.h>
#include <tchar.h>
#include "GfxConf.h"

namespace FableMod
{
namespace Gfx
{

class GFX_API Settings
{
public:

	~Settings();

	static Settings* GetInstance();

	FLOAT GetFloat(LPCTSTR pszSection, LPCTSTR pszKey, FLOAT fDefault);

	int GetInt(LPCTSTR pszSection, LPCTSTR pszKey, int iDefault);

protected:

	Settings();

	void CheckPath();

	TCHAR* m_pszPath;
};

static Settings* GetSettings() { return Settings::GetInstance(); }

}
}
