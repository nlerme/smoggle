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

#include "language_config.h"


CLanguageConfig::CLanguageConfig(){}

CLanguageConfig::~CLanguageConfig(){}

const CLanguageConfig::TLettersList & CLanguageConfig::Letters() const
{
	return m_Letters;
}

const CLanguageConfig::TWeightsList & CLanguageConfig::Weights() const
{
	return m_Weights;
}

const std::string & CLanguageConfig::DictionaryFilename() const
{
	return m_DictionaryFilename;
}

void CLanguageConfig::DictionaryFilename( const std::string & dict_filename )
{
	m_DictionaryFilename = dict_filename;
}

const std::string & CLanguageConfig::Language() const
{
	return m_Language;
}

void CLanguageConfig::Language( const std::string & language )
{
	m_Language = language;
}

void CLanguageConfig::AddLetter( const TWordType & letter, const uint weight )
{
	m_Letters.push_back(letter);
	m_Weights.push_back(weight);
}

const bool CLanguageConfig::IsConsistent() const
{
	uint nb_letters(m_Letters.size()), nb_weights(m_Weights.size());
	return (nb_letters > 0 && nb_weights > 0 && nb_letters == nb_weights);
}
