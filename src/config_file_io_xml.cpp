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

#include <string.h>
#include "language_config.h"
#include "utils.h"
#include "types.h"
#include "config_file_io_xml.h"


bool CConfigFileIOXML::_ParseLanguage( xmlNodePtr node, CLanguageConfig & language_config )
{
	xmlChar *language(xmlGetProp(node, BAD_CAST "value"));
	xmlChar *dictionary(xmlGetProp(node, BAD_CAST "dictionary"));
	bool res(false);


	if( xmlStrlen(language) != 0 && xmlStrlen(dictionary) != 0 && xmlStrEqual(language, (xmlChar*)language_config.Language().c_str()) )
	{
		language_config.DictionaryFilename(std::string((char*)dictionary));
		node = node->xmlChildrenNode;

		while( node != NULL )
		{
			if( !xmlStrcmp(node->name, BAD_CAST "letter") )
				res = _ParseLetter(node, language_config);

			node = node->next;
		}
	}

	xmlFree(language);
	xmlFree(dictionary);

	return res;
}

bool CConfigFileIOXML::_ParseLetter( xmlNodePtr node, CLanguageConfig & language_config )
{
	xmlChar *letter(xmlGetProp(node, BAD_CAST "value"));
	xmlChar *string_weight(xmlGetProp(node, BAD_CAST "weight"));
	bool res(false);


	if( xmlStrlen(letter) != 0 && xmlStrlen(string_weight) != 0 )
	{
		uint int_weight;
		FromString(TWordType((char*)string_weight), int_weight);
		language_config.AddLetter(TWordType((char*)letter), int_weight);
		res = true;
	}

	xmlFree(letter);
	xmlFree(string_weight);

	return res;
}

CConfigFileIOXML::CConfigFileIOXML(){}

CConfigFileIOXML::~CConfigFileIOXML(){}

bool CConfigFileIOXML::Load( const std::string & config_filename, const std::string & language, CLanguageConfig & language_config )
{
	xmlDocPtr doc(NULL);
	xmlNodePtr node(NULL);
	bool res(false);


	language_config.Language(language);

	if( (doc = xmlParseFile(config_filename.c_str())) )
	{
		if( (node = xmlDocGetRootElement(doc)) && !xmlStrcmp(node->name, BAD_CAST "config") )
		{
			node = node->xmlChildrenNode;

			while( node != NULL )
			{
				if( !xmlStrcmp(node->name, BAD_CAST "language") && !res )
					res = _ParseLanguage(node, language_config);

				node = node->next;
			}

			xmlFreeNode(node);
		}

		xmlFreeDoc(doc);
	}

	xmlCleanupParser();
	xmlMemoryDump();

	return res;
}
