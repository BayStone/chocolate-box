#pragma once

#include "GfxConf.h"

namespace FableMod
{
namespace Gfx
{
	
/// For multi-threading.	
class GFX_API Mutex
{
public:

	Mutex();

	~Mutex();

	void Release();

	// Acquires exclusive access to the mutex. 
	void Lock();

	// Releases exclusive access to the mutex.
	void Unlock();

private:

	Mutex(const Mutex&);
	
	Mutex& operator=(const Mutex&);

	class MutexImpl;
	
	MutexImpl* m_pThis;
};

}
}
