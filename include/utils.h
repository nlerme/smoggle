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
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __UTILS_H__
#define __UTILS_H__

#include <ctime>
#include <string>
#include <cstdlib>
#include <limits>
#include <cassert>
#include <algorithm>
#include <sstream>
#include "types.h"


//----------------------------------------- String conversions -----------------------------------------
template<typename Type> std::string ToString( const Type & value )
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

template<typename Type> bool FromString( const std::string & str, Type & dest )
{
	std::istringstream iss(str);
	return bool(iss >> dest) != 0;
}

//------------------------------------------- Math functions -------------------------------------------
template<typename Type> Type Min( const Type a, const Type b )
{
	return (a < b ? a : b);
}

template<typename Type> Type Max( const Type a, const Type b )
{
	return (a > b ? a : b);
}

template<typename Type> inline bool IsNumeric(){ return false; }
template<> inline bool IsNumeric<short>(){ return true; }
template<> inline bool IsNumeric<ushort>(){ return true; }
template<> inline bool IsNumeric<int>(){ return true; }
template<> inline bool IsNumeric<uint>(){ return true; }
template<> inline bool IsNumeric<float>(){ return true; }
template<> inline bool IsNumeric<double>(){ return true; }

//------------------------------------------------ Other -----------------------------------------------
template<typename Type> void Swap( Type & a, Type & b )
{
	Type c(a);
	a = b;
	b = c;
}

template<typename T,typename U,typename V> bool IsBetween( const T & value, const U & lb, const V & ub )
{
	assert(IsNumeric<T>() && IsNumeric<U>() && IsNumeric<V>());
	return (value >= lb && value <= ub);
}

// Generate a pseudo random number in [min, max[
template<typename Type> Type Random( const Type min, const Type max )
{
	static bool init(false);

	if( !init )
	{
		srand(time(NULL));
		init = true;
	}

	return Type((float(std::rand()) / RAND_MAX * (max - min)) + min);
}

// Generate a pseudo random binary number
inline int Random()
{
	static bool init(false);

	if( !init )
	{
		srand(time(NULL));
		init = true;
	}

	return (rand() % 2);
}

inline int LastCommonPosition( const std::string & str1, const std::string & str2 )
{
	uint k(0);
	std::string::size_type Length(Min(str1.size(), str2.size()));

	while( k < Length && str1[k] == str2[k] )
		k++;

	return (k-1);
}

// Returns true if the two strings are equal without regards to the case
inline bool AreStringsEqual( const std::string & str1, const std::string & str2 )
{
	std::string s1(str1), s2(str2);
	std::transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
	std::transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
	return (s1 == s2);
}

#endif //__UTILS_H__
