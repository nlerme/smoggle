/*
    Smoggle - A multidimensional and multi-language boggle solver
    Copyright (C) 2009 - Nicolas Lermé <nicolas.lerme@free.fr>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __COORDS_H__
#define __COORDS_H__

#include <numeric>
#include <functional>
#include <algorithm>
#include <cassert>
#include <cmath>
#include "utils.h"
#include "types.h"


class CCoords
{
     public :
	typedef int Type;
	typedef Type* Iterator;
	typedef const Type* ConstIterator;

     protected :
	uint m_Dim;
	Type *m_Data;

     public :
	//---------- Creation, destruction & copy operators ----------
	CCoords( const uint dim = 1 ) : m_Dim(0), m_Data(NULL)
	{
		Resize(dim);
		Fill(Type());
	}

	CCoords( const Type x, const Type y ) : m_Dim(0), m_Data(NULL)
	{
		Resize(2);
		m_Data[0] = x;
		m_Data[1] = y;
	}

	CCoords( const Type x, const Type y, const Type z ) : m_Dim(0), m_Data(NULL)
	{
		Resize(3);
		m_Data[0] = x;
		m_Data[1] = y;
		m_Data[2] = z;
	}

	CCoords( const Type* data, const uint dim ) : m_Dim(0), m_Data(NULL)
	{
		Resize(dim);

		for( uint k=0; k<m_Dim; k++ )
			m_Data[k] = data[k];
	}

	CCoords( const uint dim, const Type value ) : m_Dim(0), m_Data(NULL)
	{
		Resize(dim);
		Fill(value);
	}

	~CCoords()
	{
		if( m_Data != NULL )
			delete [] m_Data, m_Data=NULL;
	}

	CCoords( const CCoords & other ) : m_Dim(0), m_Data(NULL)
	{
		Resize(other.Dim());

		for( uint k=0; k<m_Dim; k++ )
			m_Data[k] = other.m_Data[k];
	}

	CCoords & operator = ( const CCoords & other )
	{
		Resize(other.Dim());

		for( uint k=0; k<m_Dim; k++ )
			m_Data[k] = other[k];

		return *this;
	}

	//-------------------- Access operators --------------------
	Type operator [] ( const uint k ) const
	{
		assert(k < m_Dim);
		return m_Data[k];
	}

	Type & operator [] ( const uint k )
	{
		assert(k < m_Dim);
		return m_Data[k];
	}

	Type* Data()
	{
		return m_Data;
	}

	const Type* Data() const
	{
		return m_Data;
	}

	Iterator Begin()
	{
		return m_Data;
	}

	ConstIterator Begin() const
	{
		return m_Data;
	}

	Iterator End()
	{
		return m_Data+m_Dim;
	}

	ConstIterator End() const
	{
		return m_Data+m_Dim;
	}

	//-------------------- Boolean operators --------------------
	const bool operator == ( const CCoords & other ) const
	{
		assert(m_Dim == other.Dim());
		return std::equal(Begin(), End(), other.Begin(), std::equal_to<Type>());
	}

	const bool operator != ( const CCoords & other ) const
	{
		return !(*this == other);
	}

	const bool operator < ( const CCoords & other ) const
	{
		assert(m_Dim == other.Dim());
		return std::equal(Begin(), End(), other.Begin(), std::less<Type>());
	}

	const bool operator >= ( const CCoords & other ) const
	{
		assert(m_Dim == other.Dim());
		return std::equal(Begin(), End(), other.Begin(), std::greater_equal<Type>());
	}

	const bool operator > ( const CCoords & other ) const
	{
		assert(m_Dim == other.Dim());
		return std::equal(Begin(), End(), other.Begin(), std::greater<Type>());
	}

	const bool operator <= ( const CCoords & other ) const
	{
		assert(m_Dim == other.Dim());
		return std::equal(Begin(), End(), other.Begin(), std::less_equal<Type>());
	}

	const bool operator == ( const Type value ) const
	{
		CCoords tmp(m_Dim, value);
		return std::equal(Begin(), End(), tmp.Begin(), std::equal_to<Type>());
	}

	const bool operator != ( const Type value ) const
	{
		CCoords tmp(m_Dim, value);
		return std::equal(Begin(), End(), tmp.Begin(), std::not_equal_to<Type>());
	}

	const bool operator < ( const Type value ) const
	{
		CCoords tmp(m_Dim, value);
		return std::equal(Begin(), End(), tmp.Begin(), std::less<Type>());
	}

	const bool operator >= ( const Type value ) const
	{
		CCoords tmp(m_Dim, value);
		return std::equal(Begin(), End(), tmp.Begin(), std::greater_equal<Type>());
	}

	const bool operator > ( const Type value ) const
	{
		CCoords tmp(m_Dim, value);
		return std::equal(Begin(), End(), tmp.Begin(), std::greater<Type>());
	}

	const bool operator <= ( const Type value ) const
	{
		CCoords tmp(m_Dim, value);
		return std::equal(Begin(), End(), tmp.Begin(), std::less_equal<Type>());
	}

	//-------------------- Arithmetic operators --------------------
	CCoords operator + ( const CCoords & other ) const
	{
		assert(m_Dim == other.Dim());
		CCoords tmp(m_Dim);
		std::transform(Begin(), End(), other.Begin(), tmp.Begin(), std::plus<Type>());
		return tmp;
	}

	CCoords & operator += ( const CCoords & other )
	{
		assert(m_Dim == other.Dim());
		std::transform(Begin(), End(), other.Begin(), Begin(), std::plus<Type>());
		return *this;
	}

	CCoords operator - ( const CCoords & other ) const
	{
		assert(m_Dim == other.Dim());
		CCoords tmp(m_Dim);
		std::transform(Begin(), End(), other.Begin(), tmp.Begin(), std::minus<Type>());
		return tmp;
	}

	CCoords & operator -= ( const CCoords & other )
	{
		assert(m_Dim == other.Dim());
		std::transform(Begin(), End(), other.Begin(), Begin(), std::minus<Type>());
		return *this;
	}

	CCoords operator + ( const Type k ) const
	{
		CCoords tmp(m_Dim);
		std::transform(Begin(), End(), tmp.Begin(), std::bind2nd(std::plus<Type>(), k));
		return tmp;
	}

	CCoords operator += ( const Type k )
	{
		std::transform(Begin(), End(), Begin(), std::bind2nd(std::plus<Type>(), k));
		return *this;
	}

	CCoords operator - ( const Type k ) const
	{
		CCoords tmp(m_Dim);
		std::transform(Begin(), End(), tmp.Begin(), std::bind2nd(std::minus<Type>(), k));
		return tmp;
	}

	CCoords operator -= ( const Type k )
	{
		std::transform(Begin(), End(), Begin(), std::bind2nd(std::minus<Type>(), k));
		return *this;
	}

	CCoords operator - () const
	{
		CCoords tmp(m_Dim);
		std::transform(Begin(), End(), tmp.Begin(), std::negate<Type>());
		return tmp;
	}

	//-------------------- I/O operations --------------------
	friend std::ostream & operator << ( std::ostream & out, const CCoords & other )
	{
		out << "[";

		for( uint k=0; k<other.Dim(); k++ )
			out << other[k] << ((k<other.Dim()-1) ? " " : "");

		out << "]";

		return out;
	}

	//-------------------- other --------------------
	const uint Dim() const
	{
		return m_Dim;
	}

	void Fill( const Type value )
	{
		std::fill(Begin(), End(), value);
	}

	Type Product() const
	{
		return std::accumulate(Begin(), End(), 1, std::multiplies<Type>());
	}

	void Resize( const uint dim )
	{
		if( m_Dim != dim )
		{
			if( m_Data != NULL )
				delete [] m_Data;

			m_Data = new Type[dim];
			m_Dim = dim;
		}
	}

	Type Magnitude() const
	{
		return std::inner_product(Begin(), End(), Begin(), 0);
	}

	float Norm() const
	{
		return sqrtf(Magnitude());
	}

	static float Curvature( const CCoords & prev, const CCoords & p, const CCoords & next )
	{
		CCoords u(p-prev), v(p-next);
		return float((u+v).Magnitude()) / (u.Magnitude() * v.Magnitude());
	}
};

class CCoordsIterator : public std::iterator<std::input_iterator_tag, CCoords>
{
     protected :
	CCoords m_A, m_B, m_Pos;
	bool m_Stop;

     public :
	CCoordsIterator( const CCoords & a, const CCoords & b ) : m_A(a), m_B(b), m_Pos(a), m_Stop(false)
	{
		assert(a.Dim() == b.Dim());
	}

	CCoordsIterator( const uint dim ) : m_A(dim), m_B(dim), m_Pos(dim), m_Stop(true){}

	CCoordsIterator & operator = ( const CCoordsIterator & other )
	{
		m_A    = other.m_A;
		m_B    = other.m_B;
		m_Pos  = other.m_Pos;
		m_Stop = other.m_Stop;
		return *this;
	}

	bool operator == ( const CCoordsIterator & other )
	{
		return (m_Stop ? other.m_Stop : !other.m_Stop && m_Pos == other.m_Pos);
	}

	bool operator != ( const CCoordsIterator & other )
	{
		return (m_Stop ? !other.m_Stop : other.m_Stop || m_Pos != other.m_Pos);
	}

	CCoordsIterator & operator ++ ()
	{
		for( uint k=0; k<m_A.Dim(); k++ )
		{
			if( m_Pos[k] != m_B[k] )
			{
				m_Pos[k]++;
				return *this;
			}

			m_Pos[k] = m_A[k];
		}

		m_Stop = true;

		return *this;
	}

	CCoordsIterator operator ++ ( int k )
	{
		CCoordsIterator tmp(*this);
		++(*this);
		return tmp;
	}

	CCoords operator * () const
	{
		return m_Pos;
	}

	const CCoords* operator -> () const
	{
		return &m_Pos;
	}

	//-------------------- I/O operations --------------------
	friend std::ostream & operator << ( std::ostream & out, const CCoordsIterator & other )
	{
		out << "[ A=" << other.m_A << ", B=" << other.m_B << ", Pos=" << other.m_Pos << " ]";
		return out;
	}
};

#endif //__COORDS_H__
