#pragma once

#include <cassert>
#include "GfxConf.h"
#include "Exception.h"

namespace FableMod
{
namespace Gfx
{

/// Dynamically resizable vector. Meant to be used with pointers.
template <class T>
class GFX_API Vector
{
public:

	typedef bool (*COMPARISON_CALLBACK)(T& rObject1,T& rObject2);

	Vector(int size = 1, int grow = 4) :
	m_pItems(0)
	{
		Create(size,grow);
	}

	~Vector()
	{
		Dispose();
	}

	void Vector::Create(int size, int grow)
	{
		Dispose();

		if (size < 0)
			size = 1;

		m_iSize = size;
		m_iGrow = grow;
		m_iUsed = 0;
		
		m_pItems = new T[m_iSize];
	}

	void Vector::Dispose()
	{
		if (m_pItems)
		{
			delete[] m_pItems;
			m_pItems = 0;
		}

		m_iUsed = 0;
		m_iSize = 0;
		m_iGrow = 0;
	}

	void Add(const T& item)
	{
		if (m_iUsed == m_iSize)
		{
			if (m_iGrow == 0)
			{
				throw Exception(
					_T("FABLEMOD_GFX_EXCEPTION_VECTOR_NO_GROW"));
			}

			int iNewSize = m_iSize+m_iGrow;

			T* pItems = new T[iNewSize];

			for (int i = 0; i < m_iUsed; ++i)
				pItems[i] = m_pItems[i];

			delete[] m_pItems;

			m_pItems = pItems;

			m_iSize = iNewSize;
		}

		m_pItems[m_iUsed++] = item;
	}

	void Remove(int index)
	{
		assert(index >= 0 && index < m_iUsed && "Invalid index");

		for (int i = index; i < m_iUsed-1; ++i)
		{
			m_pItems[i] = m_pItems[i+1];
		}	

		--m_iUsed;
	}

	int IndexOf(const T& rObject)
	{
		for (int i = 0; i < m_iUsed; ++i)
		{
			if (m_pItems[i] == rObject)
			{
				return i;
			}
		}	

		return -1;
	}

	void Clear()
	{
		m_iUsed = 0;
	}

	void Sort(COMPARISON_CALLBACK pComparison)
	{
		if (m_iUsed > 1)
			QuickSort(0,m_iUsed-1,pComparison);
	}

	int GetCount() const
	{
		return m_iUsed;
	}

	T& operator [](int index)
	{
		assert(index < m_iUsed && "Invalid index");
		return m_pItems[index];
	}

	const T& operator [](int index) const
	{
		assert(index < m_iUsed && "Invalid index");
		return m_pItems[index];
	}

	const T& At(int index) const
	{
		assert(index < m_iUsed && "Invalid index");
		return m_pItems[index];
	}

	T& At(int index)
	{
		assert(index < m_iUsed && "Invalid index");
		return m_pItems[index];
	}

	const T* GetData() const
	{
		return m_pItems;
	}

private:

	void QuickSort(int iFirst,int iLast,COMPARISON_CALLBACK pComparison)
	{
		if (iFirst >= iLast)
		{
			return;
		}

		// Partition.
		int iMiddle = (iLast+iFirst) >> 1;

		// Swap first and middle item.
		T Temp = m_pItems[iFirst];
		m_pItems[iFirst] = m_pItems[iMiddle];
		m_pItems[iMiddle] = Temp;

		T Tst = m_pItems[iFirst];

		iMiddle = iFirst;

		for (int i = iFirst+1; i <= iLast; ++i)
		{
			if ((*pComparison)(m_pItems[i],Tst))
			{
				++iMiddle;

				// Swap.
				Temp = m_pItems[i];
				m_pItems[i] = m_pItems[iMiddle];
				m_pItems[iMiddle] = Temp;
			}
		}

		Temp = m_pItems[iFirst];
		m_pItems[iFirst] = m_pItems[iMiddle];
		m_pItems[iMiddle] = Temp;

		QuickSort(iFirst,iMiddle-1,pComparison);
		QuickSort(iMiddle+1,iLast,pComparison);
	}

	T* m_pItems;
	int m_iSize;
	int m_iUsed;
	int m_iGrow;
};

}
}