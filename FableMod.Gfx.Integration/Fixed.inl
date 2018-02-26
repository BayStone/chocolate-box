template <class T, int S, int C>
inline CTFixed<T,S,C>::CTFixed()
{
}

template <class T, int S, int C>
inline CTFixed<T,S,C>::CTFixed(T tValue, bool bRaw = false)
{
	if (bRaw)
		m_tValue = tValue;
	else
		*this = tValue;
}

template <class T, int S, int C>
inline CTFixed<T,S,C>::CTFixed(float fValue)
{
	*this = fValue;
}

template <class T, int S, int C>
inline CTFixed<T,S,C>::CTFixed(int iValue)
{
	*this = iValue;
}

template <class T, int S, int C>
inline CTFixed<T,S,C>::CTFixed(unsigned int uiValue)
{
	*this = uiValue;
}


template <class T, int S, int C>
inline CTFixed<T,S,C>::CTFixed(const CTFixed& Fixed) :
m_tValue(Fixed.m_tValue)
{
}

//----------------------------------------------------------------------------

template <class T, int S, int C>
T& CTFixed<T,S,C>::Raw()
{
	return m_tValue;
}

template <class T, int S, int C>
const T& CTFixed<T,S,C>::Raw() const
{
	return m_tValue;
}

template <class T, int S, int C>
inline float CTFixed<T,S,C>::Float() const
{
	return ((float)m_tValue)/(float)C;
}

//----------------------------------------------------------------------------

template <class T, int S, int C>
bool CTFixed<T,S,C>::Equals(float fValue, float fError) const
{
	return fabs(Float(), fValue) < fEpsilon;
}

//----------------------------------------------------------------------------

template <class T, int S, int C>
inline CTFixed<T,S,C>& CTFixed<T,S,C>::operator =(const CTFixed& Fixed)
{
	m_tValue = Fixed.m_tValue;
	return *this;
}

template <class T, int S, int C>
inline CTFixed<T,S,C>& CTFixed<T,S,C>::operator =(T tValue)
{
	m_tValue = tValue << S;
	return *this;
}

template <class T, int S, int C>
inline CTFixed<T,S,C>& CTFixed<T,S,C>::operator =(int iValue)
{
	m_tValue = (T)(iValue << S);
	return *this;
}

template <class T, int S, int C>
inline CTFixed<T,S,C>& CTFixed<T,S,C>::operator =(unsigned int uiValue)
{
	m_tValue = (T)(uiValue << S);
	return *this;
}

template <class T, int S, int C>
inline CTFixed<T,S,C>& CTFixed<T,S,C>::operator =(float fValue)
{
	m_tValue = (T)floorf(fValue*C);
	return *this;
}

//----------------------------------------------------------------------------

template <class T, int S, int C>
inline CTFixed<T,S,C>& CTFixed<T,S,C>::operator +=(const CTFixed& Fixed)
{
	m_tValue += Fixed.m_tValue;
	return *this;
}

template <class T, int S, int C>
inline CTFixed<T,S,C>& CTFixed<T,S,C>::operator -=(const CTFixed& Fixed)
{
	m_tValue -= Fixed.m_tValue;
	return *this;
}

template <class T, int S, int C>
inline CTFixed<T,S,C>& CTFixed<T,S,C>::operator *=(const CTFixed& Fixed)
{
	m_tValue = (m_tValue*Fixed.m_tValue) >> S;
	return *this;
}

template <class T, int S, int C>
inline CTFixed<T,S,C>& CTFixed<T,S,C>::operator /=(const CTFixed& Fixed)
{
	m_tValue = (m_tValue << S)/Fixed.m_tValue;
	return *this;
}

//----------------------------------------------------------------------------

template <class T, int S, int C>
inline CTFixed<T,S,C> CTFixed<T,S,C>::operator +(const CTFixed& Fixed) const
{
	CTFixed Tmp;
	Tmp.m_tValue = m_tValue+Fixed.m_tValue;
	return Tmp;
}

template <class T, int S, int C>
inline CTFixed<T,S,C> CTFixed<T,S,C>::operator -(const CTFixed& Fixed) const
{
	CTFixed Tmp;
	Tmp.m_tValue = m_tValue-Fixed.m_tValue;
	return Tmp;
}

template <class T, int S, int C>
inline CTFixed<T,S,C> CTFixed<T,S,C>::operator *(const CTFixed& Fixed) const
{
	CTFixed Tmp;
	Tmp.m_tValue = (m_tValue*Fixed.m_tValue) >> S;
	return Tmp;
}

template <class T, int S, int C>
inline CTFixed<T,S,C> CTFixed<T,S,C>::operator /(const CTFixed& Fixed) const
{
	CTFixed Tmp;
	Tmp.m_tValue = (m_tValue << S)/Fixed.m_tValue;
	return Tmp;
}

//----------------------------------------------------------------------------

template <class T, int S, int C>
inline bool CTFixed<T,S,C>::operator ==(const CTFixed& Fixed) const
{
	return m_tValue == Fixed.m_tValue;
}

template <class T, int S, int C>
inline bool CTFixed<T,S,C>::operator !=(const CTFixed& Fixed) const
{
	return !(*this == Fixed);
}

//----------------------------------------------------------------------------

template <class T, int S, int C>
inline CTFixed<T,S,C>& CTFixed<T,S,C>::operator ++()
{
	m_tValue += 1 << S;
	return *this;
}

template <class T, int S, int C>
inline CTFixed<T,S,C> CTFixed<T,S,C>::operator ++(int)
{
	CTFixed Tmp(*this);
	m_tValue += 1 << S;
	return Tmp;
}

//----------------------------------------------------------------------------

template <class T, int S, int C>
inline CTFixed<T,S,C>& CTFixed<T,S,C>::operator --()
{
	m_tValue -= 1 << S;
	return *this;
}

template <class T, int S, int C>
inline CTFixed<T,S,C> CTFixed<T,S,C>::operator --(int)
{
	CTFixed Tmp(*this);
	m_tValue -= 1 << S;
	return Tmp;
}

//----------------------------------------------------------------------------

template <class T, int S, int C>
inline CTFixed<T,S,C>::operator T() const
{
	return m_tValue >> S;
}

template <class T, int S, int C>
inline CTFixed<T,S,C>::operator int() const
{
	return (int)(m_tValue >> S);
}

template <class T, int S, int C>
inline CTFixed<T,S,C>::operator float() const
{
	return Float();
}

//----------------------------------------------------------------------------
