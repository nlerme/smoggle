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
#include "utils.h"
#include "types.h"
#include "coords.h"
#include "tokenizer.h"
#include "grid_file_io_xml.h"


bool CGridFileIOXML::_ParseGrid( xmlNodePtr node, CGrid & grid, std::string & language )
{
	std::string letters, lang;
	std::vector<TWordType> vec_letters;
	uint size(0), dim(0);
	bool res(false);


	while( node != NULL )
	{
		if( !xmlStrcmp(node->name, BAD_CAST "language") )
		{
			if( node->xmlChildrenNode != NULL )
				lang = TWordType((char*)node->xmlChildrenNode->content);
		}
		else if( !xmlStrcmp(node->name, BAD_CAST "size") )
		{
			if( node->xmlChildrenNode != NULL )
				FromString(TWordType((char*)node->xmlChildrenNode->content), size);
		}
		else if( !xmlStrcmp(node->name, BAD_CAST "dimensionality") )
		{
			if( node->xmlChildrenNode != NULL )
				FromString(TWordType((char*)node->xmlChildrenNode->content), dim);
		}
		else if( !xmlStrcmp(node->name, BAD_CAST "letters") )
		{
			if( node->xmlChildrenNode != NULL )
				letters = TWordType((char*)node->xmlChildrenNode->content);
		}

		node = node->next;
	}

	if( !lang.empty() && size>0 && dim>0 && !letters.empty() )
	{
		vec_letters = CSpaceTokenizer::Tokenize(letters);

		if( vec_letters.size() == uint(CCoords(dim, size).Product()) )
		{
			grid = CGrid(dim, size);

			for( uint k=0; k<vec_letters.size(); k++ )
				grid(k) = vec_letters[k];

			res = true;
		}

		language = lang;
	}

	return res;
}

CGridFileIOXML::CGridFileIOXML(){}

CGridFileIOXML::~CGridFileIOXML(){}

bool CGridFileIOXML::Load( const std::string & grid_filename, CGrid & grid, std::string & language )
{
	xmlDocPtr doc(NULL);
	xmlNodePtr node(NULL);
	bool res(false);


	if( (doc = xmlParseFile(grid_filename.c_str())) )
	{
		if( (node = xmlDocGetRootElement(doc)) && !xmlStrcmp(node->name, BAD_CAST "grid") )
		{
			node = node->xmlChildrenNode;
			res  = _ParseGrid(node, grid, language);
			//xmlFreeNode(node);
		}

		xmlFreeDoc(doc);
	}

	xmlCleanupParser();
	xmlMemoryDump();

	return res;
}

bool CGridFileIOXML::Save( const std::string & grid_filename, CGrid & grid, std::string & language )
{
	TWordType letters("");
	xmlDocPtr doc(NULL);
	xmlNodePtr node(NULL);
	bool res(false);


	if( (doc = xmlNewDoc(BAD_CAST "1.0")) )
	{
		if( (node = xmlNewNode(NULL, BAD_CAST "grid")) )
		{
			xmlDocSetRootElement(doc, node);
			xmlNewChild(node, NULL, BAD_CAST "language", BAD_CAST language.c_str());
			xmlNewChild(node, NULL, BAD_CAST "size", BAD_CAST ToString(grid.Size()).c_str());
			xmlNewChild(node, NULL, BAD_CAST "dimensionality", BAD_CAST ToString(grid.Dim()).c_str());

			for( uint k=0; k<grid.TotalSize(); k++ )
				letters += (k == (grid.TotalSize()-1) ? grid(k) : grid(k) + " ");

			xmlNewChild(node, NULL, BAD_CAST "letters", BAD_CAST letters.c_str());

			if( xmlSaveFormatFileEnc(grid_filename.c_str(), doc, "ISO-8859-1", 1) )
				res = true;
		}

		xmlFreeDoc(doc);
	}

	xmlCleanupParser();
	xmlMemoryDump();

	return res;
}
