#ifdef WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#else
#include <pthread.h>
#endif

#include "Mutex.h"

//---------------------------------------------------------------------------

// Select atomic addition operation implementation.
#if defined(WIN32) && defined(_MSC_VER)
	#define ATOMICADD_INTELWIN32
	#pragma warning(disable: 4035)	// 'no return value'
#elif defined(WIN32)
	#define ATOMICADD_WIN32
#elif defined(_POSIX_THREADS)
	#define ATOMICADD_PTHREADS
	static pthread_mutex_t s_refCountMutex = PTHREAD_MUTEX_INITIALIZER;
#else
	#error Current platform does not have atomic addition implemented.
#endif

//---------------------------------------------------------------------------

namespace FableMod
{
namespace Gfx
{
		
class Mutex::MutexImpl
{
public:

#ifdef WIN32
	CRITICAL_SECTION	m_Obj;
#else
	pthread_mutex_t		m_Obj;
#endif

	MutexImpl()
	{
	#ifdef WIN32
		InitializeCriticalSection(&m_Obj);
	#else
		pthread_mutex_init(&m_Obj, 0);
	#endif
	}

	~MutexImpl()
	{
	#ifdef WIN32
		DeleteCriticalSection(&m_Obj);
	#else
		pthread_mutex_destroy(&m_Obj);
	#endif
	}

	void Lock()
	{
	#ifdef WIN32
		EnterCriticalSection(&m_Obj);
	#else
		pthread_mutex_lock(&m_Obj);
	#endif
	}

	void Unlock()
	{
	#ifdef WIN32
		LeaveCriticalSection(&m_Obj);
	#else
		pthread_mutex_unlock(&m_Obj);
	#endif
	}
};

//-----------------------------------------------------------------------------

Mutex::Mutex() :
m_pThis(new MutexImpl)
{
}

//---------------------------------------------------------------------------

Mutex::~Mutex()
{
	Release();
}

//---------------------------------------------------------------------------

void Mutex::Release()
{
	if (m_pThis)
	{
		delete m_pThis;
		m_pThis = 0;
	}
}

//---------------------------------------------------------------------------

void Mutex::Lock()
{
	m_pThis->Lock();
}

//---------------------------------------------------------------------------

void Mutex::Unlock()
{
	m_pThis->Unlock();
}

//---------------------------------------------------------------------------

}
}
