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

#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "types.h"
#include "lexicographic_tree.h"
#include "language_config.h"
#include "grid.h"
#include "timer.h"
#include "argstream.h"
#include "file.h"
#include "config_file_io_factory.h"
#include "grid_file_io_factory.h"

#define CONFIG_FILENAME std::string("data/config.xml")


class CApplication
{
     private :
	//------ Words ------
	typedef CLexicographicTree::TWordsList    TWordsList;
	typedef CLexicographicTree::TWordsListCIt TWordsListCIt;
	//------ Words length ------
	typedef std::map<uint,uint>              TWordsLengthsMap;
	typedef TWordsLengthsMap::const_iterator TWordsLengthsMapCIt;

	std::string m_Language, m_InputGridFilename, m_OutputGridFilename;
	uint m_GridDim, m_GridSize, m_MinWordLength, m_MaxWordLength;
	CLexicographicTree m_Tree;
	CGrid m_Grid;
	CLanguageConfig m_LanguageConfig;
	TWordsLengthsMap m_WordsLengths;
	TWordsList m_WordsFound;
	bool m_PeriodicBorders;
	std::string m_SolvingMode;

     public :
	CApplication();
	~CApplication();
	void PrintResults( const uint nb_columns = 10, const uint column_size = 20 );
	void Banner() const;
	void Run( int argc, char **argv );
};

#endif //__APPLICATION_H__
