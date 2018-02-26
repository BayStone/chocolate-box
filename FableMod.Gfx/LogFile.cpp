#include "Stdafx.h"
#include <tchar.h>
#include "LogFile.h"

namespace FableMod
{

LogFile::LogFile() :
m_pFile(0),
m_pszBuffer(0)
{
}

LogFile::~LogFile()
{
	Close();
	
	delete[] m_pszBuffer;
}

void LogFile::Open(LPCTSTR pszFileName)
{
	Close();
	m_pFile = _tfopen(pszFileName, _T("wt"));
	Write(_T("Log file %s opened.\n"), pszFileName);
}

void LogFile::Close()
{
	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = 0;
	}
}

void LogFile::Write(const LPCTSTR pszFormat, ...)
{
	if (!m_pFile)
		return;
	
	if (!m_pszBuffer)
		m_pszBuffer = new TCHAR[2048];
	
	va_list	args;

	// Create the string.
	va_start(args,pszFormat);
	_vstprintf(m_pszBuffer,pszFormat,args);
	va_end(args);
	
	_ftprintf(m_pFile, m_pszBuffer);
	fflush(m_pFile);
}

LogFile* LogFile::GetInstance()
{
	static LogFile s_Instance;
	return &s_Instance;
}

}