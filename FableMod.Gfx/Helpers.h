#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <stdio.h>
#include "Exception.h"
#include "LogFile.h"

#define GFX_SAFE_ARRAYDELETE(_x) { if (_x) { delete[] (_x); (_x) = 0; } }

#define GFX_SAFE_DELETE(_x) \
if (_x) \
{ \
	FM_DBG_LOG2(_T("(%s:%d): "), _T(__FILE__), __LINE__); \
	FM_DBG_LOG1(_T("delete: \"%s\"\n"), _T(#_x)); \
	delete (_x); \
	(_x) = 0; \
} \

#define GFX_SAFE_RELEASE(_x) \
{ \
	if (_x) \
	{ \
		FM_DBG_LOG2(_T("(%s:%d): "), _T(__FILE__), __LINE__); \
		ULONG ulRefs = (_x)->Release(); \
		FM_DBG_LOG2(_T("Release: \"%s\" (refs = %u)\n"), _T(#_x), ulRefs); \
		(_x) = 0; \
	} \
}
			
#define GFX_TRYD3D(_x) \
{ \
	if (FAILED(_x)) \
	{ \
		FM_LOG3(_T("(%s:%d): DXERR: \"%s\"\n"), \
			_T(__FILE__), __LINE__, _T(#_x)); \
		throw Exception(_T("GFX_D3D_EXCEPTION")); \
	} \
}