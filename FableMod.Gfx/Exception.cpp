#include "Stdafx.h"
#include "Exception.h"
#include <cassert>

namespace FableMod
{
namespace Gfx
{

static const unsigned int MAX_MESSAGE_SIZE = 256;

Exception::Exception()
{
	m_pszMessage = new TCHAR[1];
	m_pszMessage[0] = _T('\0');
}

Exception::~Exception()
{
	delete[] m_pszMessage;
}

Exception::Exception(LPCTSTR pszMessage)
{
	assert(pszMessage != 0);

	size_t len = _tcslen(pszMessage)+1;

	m_pszMessage = new TCHAR[len];
	
	_tcscpy_s(m_pszMessage, len, pszMessage);
}

const TCHAR* Exception::GetMsg() const
{
	return m_pszMessage;
}

}
}