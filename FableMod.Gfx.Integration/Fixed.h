#ifndef INCLUDED_FIXED_H
#define INCLUDED_FIXED_H

#include <cmath>

#define FIXED_OPERATOR(_op, _type) \
inline CTFixed operator _op(_type Value) const \
{ \
	return *this _op CTFixed(Value); \
}

#define FIXED_OPERATOR_MATH(_type) \
FIXED_OPERATOR(+, _type) \
FIXED_OPERATOR(-, _type) \
FIXED_OPERATOR(/, _type) \
FIXED_OPERATOR(*, _type)

template <class T = long, int S = 16, int C = 1 << S>
class CTFixed
{
public:

	CTFixed();
	CTFixed(const CTFixed& Fixed);
	explicit CTFixed(T tValue, bool bRaw = false);
	explicit CTFixed(float fValue);
	explicit CTFixed(int iValue);
	explicit CTFixed(unsigned int uiValue);
	
	bool Equals(float fValue, float fError = 0.0001f) const;

	T& Raw();
	const T& Raw() const;

	float Float() const;

	CTFixed& operator =(const CTFixed& Fixed);
	CTFixed& operator =(T tValue);
	CTFixed& operator =(int iValue);
	CTFixed& operator =(unsigned int uiValue);
	CTFixed& operator =(float fValue);

	CTFixed& operator +=(const CTFixed& Fixed);
	CTFixed& operator -=(const CTFixed& Fixed);
	CTFixed& operator *=(const CTFixed& Fixed);
	CTFixed& operator /=(const CTFixed& Fixed);

	CTFixed operator +(const CTFixed& Fixed) const;
	CTFixed operator -(const CTFixed& Fixed) const;
	CTFixed operator *(const CTFixed& Fixed) const;
	CTFixed operator /(const CTFixed& Fixed) const;

	FIXED_OPERATOR_MATH(float)
	FIXED_OPERATOR_MATH(int)
	FIXED_OPERATOR_MATH(unsigned int)
	FIXED_OPERATOR_MATH(T)

	bool operator ==(const CTFixed& Fixed) const;
	bool operator !=(const CTFixed& Fixed) const;

	CTFixed& operator ++();
	CTFixed operator ++(int);

	CTFixed& operator --();
	CTFixed operator --(int);

	operator T() const;
	operator int() const;
	operator float() const;

private:

	T m_tValue;
};

#include "Fixed.inl"

#endif