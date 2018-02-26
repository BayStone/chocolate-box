
template <class T>
inline SmartPointer<T>::SmartPointer(T* pObject) :
m_pObject(pObject)
{
	if (m_pObject)
	{
		m_pObject->IncReferences();
	}
}

//---------------------------------------------------------------------------

template <class T>
inline SmartPointer<T>::SmartPointer(const SmartPointer& rPointer) :
m_pObject(rPointer.m_pObject)
{
	if (m_pObject)
	{
		m_pObject->IncReferences();
	}
}

//---------------------------------------------------------------------------

template <class T>
inline SmartPointer<T>::~SmartPointer()
{
    if (m_pObject && m_pObject->DecReferences() == 0)
	{
		delete m_pObject;
		m_pObject = 0;
	}
}

//---------------------------------------------------------------------------

template <class T>
inline SmartPointer<T>::operator T*() const
{
    return m_pObject;
}

//---------------------------------------------------------------------------

template <class T>
inline T& SmartPointer<T>::operator *() const
{
    return *m_pObject;
}

//---------------------------------------------------------------------------

template <class T>
inline T* SmartPointer<T>::operator ->() const
{
    return m_pObject;
}

//---------------------------------------------------------------------------

template <class T>
inline SmartPointer<T>& 
SmartPointer<T>::operator =(const SmartPointer& rPointer)
{
	*this = rPointer.m_pObject;
    return *this;
}

//---------------------------------------------------------------------------

template <class T>
inline SmartPointer<T>& SmartPointer<T>::operator =(T* pObject)
{
	if (m_pObject != pObject)
	{
		if (m_pObject && m_pObject->DecReferences() == 0)
		{
			delete m_pObject;
		}
		
		m_pObject = pObject;
		
		if (m_pObject)
		{
			m_pObject->IncReferences();
		}
	}

    return *this;
}

//---------------------------------------------------------------------------

template <class T>
inline bool SmartPointer<T>::operator ==(T* pObject) const
{
    return m_pObject == pObject;
}

//---------------------------------------------------------------------------

template <class T>
inline bool SmartPointer<T>::operator !=(T* pObject) const
{
    return m_pObject != pObject;
}

//---------------------------------------------------------------------------

template <class T>
inline bool 
SmartPointer<T>::operator ==(const SmartPointer& rPointer) const
{
    return m_pObject == rPointer.m_pObject;
}

//---------------------------------------------------------------------------

template <class T>
inline bool 
SmartPointer<T>::operator !=(const SmartPointer& rPointer) const
{
    return m_pObject != rPointer.m_pObject;
}

//---------------------------------------------------------------------------
