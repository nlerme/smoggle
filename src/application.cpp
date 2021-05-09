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
#include "iboggle_factory.h"
#include "application.h"


CApplication::CApplication(){}

CApplication::~CApplication(){}

void CApplication::PrintResults( const uint nb_columns, const uint column_size )
{
	uint k(0);


	std::cout << "\n---- Words found (" << m_WordsFound.size() << ") ----";

	for( TWordsListCIt it(m_WordsFound.begin()); it!=m_WordsFound.end(); it++ )
	{
		if( (k++%nb_columns) == 0 )
			std::cout << '\n';

		std::cout << (*it).c_str() << std::string(column_size-(*it).size(), ' ').c_str();

		m_WordsLengths[(*it).size()]++;
	}

	std::cout << "\n\n";

	for( TWordsLengthsMapCIt it(m_WordsLengths.begin()); it!=m_WordsLengths.end(); it++ )
		std::cout << "* " << it->second << " words of " << it->first << "-length\n";

	std::cout << "\n* Total score   : " << m_Tree.Score() << '\n';
	std::cout << "* Mean curvature: " << m_Tree.MeanCurvature() << '\n';
	std::cout << "--------------------------\n";
}

void CApplication::Banner() const
{
	std::cout << "########################################################\n"
	     << "#                                                      #\n"
	     << "# Smoggle solver                                       #\n"
	     << "#                                                      #\n"
	     << "########################################################\n\n";
}

void CApplication::Run( int argc, char **argv )
{
	smart_ptr<IConfigFileIO> config_io(NULL);
	smart_ptr<IGridFileIO> grid_io(NULL);
	smart_ptr<IIBoggle> iboggle(NULL);
	std::string solving_mode_str;
	CTimer timer;


	//--- We set default values of parameters -----------
	m_Language      = "british";
	m_GridDim       = 2;
	m_GridSize      = 4;
	m_MinWordLength = 3;
	m_MaxWordLength = 1000;
	m_SolvingMode   = "none";

	//--- We get parameters -----------------------------
	argstream as(argc, argv);
	as >> parameter('i',            "input",  m_InputGridFilename,                                   "input grid filename", false)
	   >> parameter('o',           "output", m_OutputGridFilename,                                  "output grid filename", false)
	   >> parameter('l',         "language",           m_Language,                                              "language", false)
	   >> parameter('d',         "grid-dim",            m_GridDim,                          "grid dimension (2 <= d <= 4)", false)
	   >> parameter('n',        "grid-size",           m_GridSize,                                      "grid size (>= 3)", false)
	   >> parameter('m',  "min-word-length",      m_MinWordLength,                        "minimum length of words (>= 3)", false)
	   >> parameter('M',  "max-word-length",      m_MaxWordLength,                        "maximum length of words (>= 3)", false)
	   >> parameter('s',     "solving-mode",        m_SolvingMode, "solving mode (none, direct or inverse:heuristic_name)", false)
	   >>    option('p', "periodic-borders",    m_PeriodicBorders,                                      "periodic borders")
	   >> help();
	as.defaultErrorHandling();

	//--- We check parameters ---------------------------
	if( m_InputGridFilename.empty() && m_Language.empty() ) throw std::logic_error("you must either give an input grid filename or choose to generate one");
	if( !IsBetween(int(m_GridDim), 2, 4) )                  throw std::logic_error("grid dimensionality must be 2, 3 or 4");
	if( m_GridSize < 3 )                                    throw std::logic_error("grid size must be >= 3");
	if( m_MinWordLength < 3 )                               throw std::logic_error("minimum word length is >= 3");
	if( m_MaxWordLength < 3 )                               throw std::logic_error("maximum word length is >= 3");
	if( m_MinWordLength > m_MaxWordLength )                 throw std::logic_error("minimum word length must be <= maximum word length");

	//--- Program core ----------------------------------
	timer.Init();
	Banner();

	// We set activate the periodic borders if asked
	m_Tree.PeriodicBorders(m_PeriodicBorders);

	// Then, we either load a grid or randomly generate it
	if( !(config_io = CConfigFileIOFactory::Create(CFile(CONFIG_FILENAME).Extension())) )
		throw std::logic_error("unsupported file format for the config file");

	if( !m_InputGridFilename.empty() )
	{
		if( !(grid_io = CGridFileIOFactory::Create(CFile(m_InputGridFilename).Extension())) )
			throw std::logic_error("unsupported file format for the input grid file");

		if( !grid_io->Load(m_InputGridFilename, m_Grid, m_Language) )
			throw std::logic_error("unable to load the grid");

		if( !config_io->Load(CONFIG_FILENAME, m_Language, m_LanguageConfig) )
			throw std::logic_error("unable to load the config file");

		if( m_LanguageConfig.DictionaryFilename().empty() )
			throw std::logic_error("dictionary file not found");
	}
	else
	{
		m_Grid = CGrid(m_GridDim, m_GridSize);

		if( !config_io->Load(CONFIG_FILENAME, m_Language, m_LanguageConfig) )
			throw std::logic_error("unable to load the config file");

		if( m_LanguageConfig.DictionaryFilename().empty() )
			throw std::logic_error("dictionary file not found");

		m_Grid.Generate(m_LanguageConfig);
	}

	// We check if the language config is consistent and build the lexicographic tree
	if( !m_LanguageConfig.IsConsistent() )
		throw std::logic_error("unable to correctly parse the config file");

	if( !m_Tree.Load(m_LanguageConfig.DictionaryFilename(), m_MinWordLength, m_MaxWordLength) )
		throw std::logic_error("unable to build the lexicographic tree");

	// We print the grid
	m_Grid.Print();

	// We solve the grid in direct or inverse mode
	if( AreStringsEqual(m_SolvingMode, "none") )
		;
	else if( AreStringsEqual(m_SolvingMode, "direct") )
	{
		m_WordsFound = m_Tree.Solve(m_Grid);
		PrintResults();
	}
	else
	{
		std::vector<std::string> tokens = CColonTokenizer::Tokenize(m_SolvingMode);

		if( tokens.size() != 2 || !AreStringsEqual(tokens[0], "inverse") )
			throw std::logic_error("bad format of solving mode option");

		if( !(iboggle = IIBoggleFactory::Create(tokens[1])) )
			throw std::logic_error("unsupported heuristic method");

		iboggle->Compute(m_Grid, m_Tree);
		std::cout << "\n[ Final solution ]\n";
		m_Grid.Print();

		m_WordsFound = m_Tree.Solve(m_Grid);
		PrintResults();
	}

	// We save the output grid if asked
	if( !m_OutputGridFilename.empty() )
	{
		if( !(grid_io = CGridFileIOFactory::Create(CFile(m_OutputGridFilename).Extension())) )
			throw std::logic_error("unsupported file format for the output grid file");

		if( !grid_io->Save(m_OutputGridFilename, m_Grid, m_Language) )
			throw std::logic_error("unable to save the grid");
	}

	std::cout << "\n[ Total time -> " << timer.Time() << " secs ]\n";
}
