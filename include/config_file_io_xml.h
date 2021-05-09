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

#ifndef __CONFIG_FILE_IO_XML_H__
#define __CONFIG_FILE_IO_XML_H__

#include <libxml/parser.h>
#include <libxml/xmlmemory.h>
#include <string>
#include "config_file_io.h"
#include "language_config.h"


class CConfigFileIOXML : public IConfigFileIO
{
     private :
	bool _ParseLanguage( xmlNodePtr node, CLanguageConfig & language_config );
	bool _ParseLetter( xmlNodePtr node, CLanguageConfig & language_config );

     public :
	CConfigFileIOXML();
	virtual ~CConfigFileIOXML();
	virtual bool Load( const std::string & config_filename, const std::string & language, CLanguageConfig & language_config );
};

#endif //__CONFIG_FILE_IO_XML_H__
