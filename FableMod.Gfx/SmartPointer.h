#pragma once

namespace FableMod
{
namespace Gfx
{

template <class T>
class SmartPointer
{
public:
    SmartPointer(T* pObject = 0);
    SmartPointer(const SmartPointer& rPointer);
    
	virtual ~SmartPointer();

    operator T*() const;
    T& operator *() const;
    T* operator ->() const;

    SmartPointer& operator =(const SmartPointer& rReference);
    SmartPointer& operator =(T* pObject);

    bool operator ==(T* pObject) const;
    bool operator !=(T* pObject) const;
    bool operator ==(const SmartPointer& rReference) const;
    bool operator !=(const SmartPointer& rReference) const;

protected:
    
	T* m_pObject;
};

#include "SmartPointer.inl"
#include "SmartPointer.mcr"

}
}

