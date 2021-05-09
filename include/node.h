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

#ifndef __NODE_H__
#define __NODE_H__

#include <vector>
#include "types.h"


template<typename Type> class CNode
{
     public :
	typedef CNode<Type>                         TNode;
	typedef typename std::vector<TNode*>        TNodesList;
	typedef typename TNodesList::const_iterator TNodesListCIt;
	typedef typename TNodesList::iterator       TNodesListIt;

     protected :
	Type m_Value;
	bool m_IsTerminal;
	TNodesList m_Children;

     public :
	CNode( const Type & value = Type(), 
	       const bool is_terminal = false, 
	       const TNodesList & children = TNodesList() ) : m_Value(value), 
							      m_IsTerminal(is_terminal), 
							      m_Children(children){}

	~CNode()
	{
		TNodesListIt It(Begin());

		while( It < End() )
		{
			delete *It;
			*It = NULL;
			It  = m_Children.erase(It);
		}
	}

	const Type & Value() const
	{
		return m_Value;
	}

	void Value( const Type & value )
	{
		m_Value = value;
	}

	const bool & IsTerminal() const
	{
		return m_IsTerminal;
	}

	void IsTerminal( const bool is_terminal )
	{
		m_IsTerminal = is_terminal;
	}

	const TNodesList & Children() const
	{
		return m_Children;
	}

	void Children( const TNodesList & children )
	{
		m_Children = children;
	}

	const uint NbOfChildren() const
	{
		return m_Children.size();
	}

	const bool IsLeaf() const
	{
		return (NbOfChildren() == 0);
	}

	TNode* AddChild( TNode *node )
	{
		m_Children.push_back(node);
		return this;
	}

	TNodesListIt Begin()
	{
		return m_Children.begin();
	}

	TNodesListCIt Begin() const
	{
		return m_Children.begin();
	}

	TNodesListIt End()
	{
		return m_Children.end();
	}

	TNodesListCIt End() const
	{
		return m_Children.end();
	}

	TNodesListCIt ChildrenValue( const Type & value ) const
	{
		TNodesListCIt it;


		for( it=Begin(); it<End(); it++ )
			if( (*it)->Value() == value )
				return it;

		return End();
	}
};

//------------- Some aliases -------------
typedef CNode<int>         TNodeInt;
typedef CNode<uint>        TNodeUInt;
typedef CNode<float>       TNodeFloat;
typedef CNode<double>      TNodeDouble;
typedef CNode<char>        TNodeChar;
typedef CNode<std::string> TNodeString;
//----------------------------------------

#endif //__NODE_H__
