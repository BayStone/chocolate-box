#include "Stdafx.h"
#include <tchar.h>
#include <cassert>
#include "RootObject.h"

namespace FableMod
{
namespace Gfx
{

unsigned int RootObject::sm_uiNumObjects = 0;

RootObject::RootObject() :
m_uiReferences(0)
{
	++sm_uiNumObjects;

	m_pszName = new TCHAR[1];
	m_pszName[0] = _T('\0');
}

RootObject::~RootObject()
{
	--sm_uiNumObjects;

	Dispose();

	delete[] m_pszName;
}

void RootObject::Dispose()
{
}

/// Set object name.
/// @param pszName [in] New name, cannot be NULL.
void RootObject::SetName(LPCTSTR pszName)
{
	assert(pszName != 0);

	delete[] m_pszName;

	m_pszName = new TCHAR[_tcslen(pszName)+1];
	
	_tcscpy_s(m_pszName, _tcslen(pszName)+1, pszName);
}

LPCTSTR RootObject::GetName() const
{
	return m_pszName;
}

unsigned int RootObject::GetNumObjects()
{
	return sm_uiNumObjects;
}

}
}