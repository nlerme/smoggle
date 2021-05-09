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

#ifndef __TREE_H__
#define __TREE_H__

#include <iostream>
#include <vector>
#include <string>
#include "node.h"

using std::cout;
using std::endl;


template<typename Type> class CTree
{
     public :
	typedef typename CNode<Type>::TNode         TNode;
	typedef typename CNode<Type>::TNodesList    TNodesList;
	typedef typename CNode<Type>::TNodesListCIt TNodesListCIt;
	typedef typename CNode<Type>::TNodesListIt  TNodesListIt;


     protected :
	TNode *m_Root;

	const uint _Size( TNode *Node ) const
	{
		if( !Node )
			return 0;

		uint Result(0);
		TNodesListCIt It(Node->Begin());

		while( It < Node->End() )
		{
			Result += _Size(*It);
			It++;
		}

		return (Result + 1);
	}

	const uint _Height( TNode *Node ) const
	{
		if( !Node )
			return 0;

		uint Result(0);
		TNodesListCIt It(Node->Begin());

		while( It < Node->End() )
		{
			Result = Max(Result, _Height(*It));
			It++;
		}

		return (Result + 1);
	}

	void _Print( TNode *Node, const uint Depth ) const
	{
		if( !Node )
			return;

		cout << std::string(Depth*3, ' ') << "+ " << Node->Value();

		if( Node->IsTerminal() )
			cout << "[*]";

		cout << endl;

		TNodesListCIt It(Node->Begin());

		while( It < Node->End() )
		{
			_Print(*It, Depth+1);
			It++;
		}
	}


     public :
	CTree( TNode *Root ) : m_Root(Root){}

	~CTree()
	{
		// Recursive destruction of the n-tree
		delete m_Root;
		m_Root = NULL;
	}

	const uint Size() const
	{
		return _Size(m_Root);
	}

	const uint Height() const
	{
		return _Height(m_Root);
	}

	void Print() const
	{
		_Print(m_Root, 0);
	}

	TNode* Root() const
	{
		return m_Root;
	}

	void Root( TNode *Root ) const
	{
		m_Root = Root;
	}
};

//------------- Some aliases -------------
typedef CTree<int>         TTreeInt;
typedef CTree<uint>        TTreeUInt;
typedef CTree<float>       TTreeFloat;
typedef CTree<double>      TTreeDouble;
typedef CTree<char>        TTreeChar;
typedef CTree<std::string> TTreeString;
//----------------------------------------

#endif //__TREE_H__
