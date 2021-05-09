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

#include <stdexcept>
#include "utils.h"
#include "lexicographic_tree.h"


CLexicographicTree::CLexicographicTree() : TTreeChar(new TNodeChar(0)), m_PeriodicBorders(false){}

CLexicographicTree::~CLexicographicTree(){}

void CLexicographicTree::PeriodicBorders( const bool periodic_borders )
{
	m_PeriodicBorders = periodic_borders;
}

const bool & CLexicographicTree::PeriodicBorders() const
{
	return m_PeriodicBorders;
}

void CLexicographicTree::_InsertNode( TNodeChar *node, const TWordType & word, const uint position )
{
	m_LastNodes.push_back(node);

	if( position >= word.size() )
		return;

	TNodesListCIt it;
	int diff_position(word.size()-position);

	if( (it = node->ChildrenValue(word[position])) == node->End() )
	{
		node->AddChild(new TNodeChar(word[position], (diff_position == 1)));
		_InsertNode(*(node->End()-1), word, position+1);
	}
	else
	{
		if( diff_position == 1 )
		{
			// Important: the next instruction is conditional. Indeed, suppose we have marked w1="american". Next, we want to mark 
			// w2="america". We only mark terminal nodes because we could overwrite old nodes.
			(*it)->IsTerminal(true);
		}

		_InsertNode(*it, word, position+1);
	}
}

void CLexicographicTree::_SaveWords( std::ofstream & fp, const TNodeChar *node, const TWordType & word ) const
{
	if( node->IsTerminal() )
		fp << word << endl;

	for( TNodesListCIt it=node->Begin(); it<node->End(); it++ )
		_SaveWords(fp, *it, word+(*it)->Value());
}

const uint CLexicographicTree::_GetNumberOfWords( TNodeChar *node ) const
{
	uint sum(0);


	if( node->IsTerminal() )
		sum++;

	for( TNodesListCIt it=node->Begin(); it<node->End(); it++ )
		sum += _GetNumberOfWords(*it);

	return sum;
}

TNodeChar* CLexicographicTree::_ChildrenWord( TNodeChar* node, const TWordType & word, const uint position ) const
{
	if( position >= word.size() )
		return node;

	TNodesListCIt it;

	if( (it = node->ChildrenValue(word[position])) != node->End() )
		return _ChildrenWord(*it, word, position+1);

	return NULL;
}

const bool CLexicographicTree::Load( const std::string & dict_filename, const uint min_word_length, const uint max_word_length )
{
	std::ifstream fp(dict_filename.c_str());
	TWordType word;
	int position;
	uint word_size;
	TNode *node(NULL);


	if( min_word_length < 1 )
		throw std::logic_error("Minimum words length is 1");

	if( min_word_length > max_word_length )
		throw std::logic_error("Minimum word length must be less or equal than maximum word length");

	if( !fp )
	{
		fp.close();
		return false;
	}

	m_MinWordLength = min_word_length;
	m_MaxWordLength = max_word_length;

	// We push the root node
	m_LastNodes.push_back(m_Root);

	while( std::getline(fp, word) )
	{
		word_size = word.size();

		if( word_size > 0 && word_size >= min_word_length && word_size <= max_word_length )
		{
			//------------------------------------------------------------------------------------------------------------
			// Strategy: first, we insert a word w1 and we store the path in a list during its insertion. Next, we want 
			// to insert w2. We compute the last common position (pos) from the left, between w1 and w2. Then, if 
			// (pos != |w2|-1), we retrieve the corresponding node in the list and we launch insertion from it. Otherwise, 
			// w2[pos] represents the last character of w2 and we just have to update the state of this node.
			//------------------------------------------------------------------------------------------------------------
			position = LastCommonPosition(word, m_LastWord);

			if( position == int(word.size()-1) )
				m_LastNodes[position+1]->IsTerminal(true);
			else
			{
				node = m_LastNodes[position+1];
				m_LastNodes.erase(m_LastNodes.begin()+position+1, m_LastNodes.end());
				_InsertNode(node, word, position+1);
			}

			m_LastWord = word;
		}
	}

	fp.close();

	return true;
}

const uint CLexicographicTree::GetNumberOfWords() const
{
	return _GetNumberOfWords(m_Root);
}

void CLexicographicTree::SaveWords( const std::string & dict_filename ) const
{
	std::ofstream fp(dict_filename.c_str());
	_SaveWords(fp, m_Root, "");
}
