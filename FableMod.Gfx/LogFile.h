#pragma once

#include <stdio.h>
#include <windows.h>
#include "GfxConf.h"

namespace FableMod
{

class GFX_API LogFile
{
public:

	virtual ~LogFile();

	void Open(LPCTSTR pszFileName);

	void Close();

	void Write(LPCTSTR pszFormat, ...);

	static LogFile* GetInstance();

private:

	LogFile();

	FILE* m_pFile;
	TCHAR* m_pszBuffer;
};

}

#ifndef FM_NO_LOG

#define FM_LOG3(_text,_a,_b,_c) \
	FableMod::LogFile::GetInstance()->Write((_text),(_a),(_b),(_c))

#define FM_LOG_OPEN(_filename) \
	FableMod::LogFile::GetInstance()->Open(_filename)

#else

#define FM_LOG3(_text,_a,_b,_c)
#define FM_LOG_OPEN(_filename)

#endif

#define FM_LOG2(_text,_a,_b) \
FM_LOG3(_text,_a,_b,0)

#define FM_LOG1(_text,_a) \
FM_LOG3(_text,_a,0,0)

#define FM_LOG0(_text) \
FM_LOG3(_text,0,0,0)

#ifdef _DEBUG

#define FM_DBG_LOG3(_text,_a,_b,_c) FM_LOG3(_text,_a,_b,_c)

#else

#define FM_DBG_LOG3(_text,_a,_b,_c) 

#endif

#define FM_DBG_LOG2(_text,_a,_b) \
FM_DBG_LOG3(_text,_a,_b,0)

#define FM_DBG_LOG1(_text,_a) \
FM_DBG_LOG3(_text,_a,0,0)

#define FM_DBG_LOG0(_text) \
FM_DBG_LOG3(_text,0,0,0)