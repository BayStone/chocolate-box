#include "Stdafx.h"
#include "Settings.h"
#include "Manager.h"
#include <tchar.h>

namespace FableMod
{
namespace Gfx
{

Settings::Settings() :
m_pszPath(0)
{
}

Settings::~Settings()
{
	if (m_pszPath)
		delete[] m_pszPath;
}

void Settings::CheckPath()
{
	if (!m_pszPath)
	{
		m_pszPath = new TCHAR[MAX_PATH];

		_stprintf_s(m_pszPath, MAX_PATH, 
			_T("%sFableMod.Gfx.INI"), Manager::GetDirectory());
	}
}

FLOAT Settings::GetFloat(LPCTSTR pszSection, LPCTSTR pszKey, FLOAT fDefault)
{
	CheckPath();

	TCHAR szKey[128];
	TCHAR szDef[128];

	_stprintf_s(szDef,128,_T("%f"),fDefault);

	GetPrivateProfileString(
		pszSection, pszKey, szDef, szKey, 128, m_pszPath);

	return (FLOAT)_tstof(szKey);
}

int Settings::GetInt(LPCTSTR pszSection, LPCTSTR pszKey, int iDefault)
{
	CheckPath();

	TCHAR szKey[32];
	TCHAR szDef[32];

	_stprintf_s(szDef,32,_T("%d"),iDefault);

	GetPrivateProfileString(
		pszSection, pszKey, szDef, szKey, 32, m_pszPath);

	return (int)_tstol(szKey);
}

Settings* Settings::GetInstance()
{
	static Settings s_Settings;
	return &s_Settings;
}

}
}