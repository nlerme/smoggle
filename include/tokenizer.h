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

#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#include <functional>
#include <string>
#include <algorithm>
#include <vector>


class CSpace : public std::unary_function<char, bool>
{
     public :
	bool operator () ( char c ) const;
};

class CColon : public std::unary_function<char, bool>
{
     public :
	bool operator () ( char c ) const;
};

class CNewLine : public std::unary_function<char, bool>
{
     public :
	bool operator () ( char c ) const;
};

class CSeparator : public std::unary_function<char, bool>
{
     private :
	std::string m_String;

     public :
	CSeparator( const std::string & str );
	bool operator () ( char c ) const;
};

template<typename Predicate> class CTokenizer
{
     public :
	static std::vector<std::string> Tokenize( const std::string & str, const Predicate & p=Predicate() )
	{
		std::vector<std::string> tokens;
		std::string::const_iterator it(str.begin());
		std::string::const_iterator it_token_end(str.begin());


		while( it != str.end() )
		{
			while( p(*it) )
				it++;

			it_token_end = find_if(it, str.end(), p);

			if( it < it_token_end )
				tokens.push_back(std::string(it, it_token_end));

			it = it_token_end;
		}

		return tokens;
	}
};

typedef CTokenizer<CSpace> CSpaceTokenizer;
typedef CTokenizer<CNewLine> CNewLineTokenizer;
typedef CTokenizer<CColon> CColonTokenizer;

#endif //__TOKENIZER_H__
