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

#ifndef __LANGUAGE_CONFIG_H__
#define __LANGUAGE_CONFIG_H__

#include <vector>
#include "types.h"


class CLanguageConfig
{
     public :
	//------ Letters ------
	typedef std::vector<TWordType>       TLettersList;
	typedef TLettersList::const_iterator TLettersListCIt;
	typedef TLettersList::iterator       TLettersListIt;
	//------ Weights ------
	typedef std::vector<uint>            TWeightsList;
	typedef TWeightsList::const_iterator TWeightsListCIt;
	typedef TWeightsList::iterator       TWeightsListIt;

     private :
	TLettersList m_Letters;
	TWeightsList m_Weights;
	std::string m_Language;
	std::string m_DictionaryFilename;

     public :
	CLanguageConfig();
	~CLanguageConfig();
	const TLettersList & Letters() const;
	const TWeightsList & Weights() const;
	const std::string & DictionaryFilename() const;
	void DictionaryFilename( const std::string & dict_filename );
	const std::string & Language() const;
	void Language( const std::string & language );
	void AddLetter( const TWordType & letter, const uint weight );
	const bool IsConsistent() const;
};

#endif //__LANGUAGE_CONFIG_H__
