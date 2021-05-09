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

#include "tokenizer.h"


//---------------------------------------- CNewLine class ----------------------------------------
bool CNewLine::operator () ( char c ) const
{
	return (c == '\n');
}

//----------------------------------------- CSpace class -----------------------------------------
bool CSpace::operator () ( char c ) const
{
	return (c == ' ');
}

//----------------------------------------- CSpace class -----------------------------------------
bool CColon::operator () ( char c ) const
{
	return (c == ':');
}

//--------------------------------------- CSeparator class ---------------------------------------
CSeparator::CSeparator( const std::string & str ) : m_String(str){}

bool CSeparator::operator () ( char c ) const
{
	return (m_String.find(c) != std::string::npos);
}
