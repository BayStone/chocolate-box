#pragma once

#include <windows.h>
#include <stdio.h>
#include "GfxConf.h"
#include "SmartPointer.h"

namespace FableMod
{
namespace Gfx
{

/// Graphics engine base class.
class GFX_API RootObject
{
	GFX_DECLARE_ROOT_SMARTPOINTER;

public:

	RootObject();

	virtual ~RootObject();

	virtual void Dispose();

	void SetName(LPCTSTR pszName);

	LPCTSTR GetName() const;
	
	static unsigned int GetNumObjects();

private:
	
	TCHAR* m_pszName;

	static unsigned int sm_uiNumObjects;
};

}
}