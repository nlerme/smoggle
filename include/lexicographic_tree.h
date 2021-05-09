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

#ifndef __LEXICOGRAPHIC_TREE_H__
#define __LEXICOGRAPHIC_TREE_H__

#include <iostream>
#include <fstream>
#include <omp.h>
#include <set>
#include "grid.h"
#include "types.h"
#include "tree.h"
#include "path.h"


class CLexicographicTree : public TTreeChar
{
     public :
	//------ Nodes ------
	typedef TTreeChar::TNodesList    TNodesList;
	typedef TTreeChar::TNodesListCIt TNodesListCIt;
	typedef TTreeChar::TNodesListIt  TNodesListIt;
	//------ Words ------
	typedef std::set<TWordType>         TWordsList;
	typedef TWordsList::const_iterator  TWordsListCIt;
	typedef TWordsList::iterator        TWordsListIt;
	//----- Coords ------
	typedef CGrid::TCoordsList          TCoordsList;
	typedef TCoordsList::const_iterator TCoordsListCIt;
	typedef TCoordsList::iterator       TCoordsListIt;
	//------ Paths ------
	typedef std::vector<CPath>         TPathsList;
	typedef TPathsList::const_iterator TPathsListCIt;
	typedef TPathsList::iterator       TPathsListIt;

     protected :
	TNodesList m_LastNodes;
	TWordType m_LastWord;
	uint m_MinWordLength, m_MaxWordLength;
	int m_Score;
	float m_MeanCurvature;
	bool m_PeriodicBorders;
	TPathsList m_Paths;

	void _InsertNode( TNodeChar *node, const TWordType & word, const uint position );
	void _SaveWords( std::ofstream & fp, const TNodeChar *node, const TWordType & word ) const;
	const uint _GetNumberOfWords( TNodeChar *node ) const;
	TNodeChar* _ChildrenWord( TNodeChar* node, const TWordType & word, const uint position ) const;

	void _Solve( const CGrid & grid, 
		     const CCoords & p, 
		     TNodeChar * node, 
		     const TWordType & word, 
		     const CPath & path, 
		     TCoordsList visited_nodes, 
		     TWordsList & words, 
		     TPathsList & paths ) const
	{
		TCoordsList neighbors;
		TCoordsListCIt it_coords;
		TNodeChar* children(NULL);
		TWordType new_word;
		uint new_word_length;
		CPath new_path(path);


		// We continue only if the current node has a correct children in the grid
		if( (children = _ChildrenWord(node, grid(p), 0)) != NULL )
		{
			new_word        = word + grid(p);
			new_word_length = new_word.size();
			new_path.AddNode(p);

			// If the current node is terminal, we have found a word. Then we add it to the current list if it has a correct length
			if( children->IsTerminal() && new_word_length >= m_MinWordLength && new_word_length <= m_MaxWordLength )
			{
				paths.push_back(new_path);
				words.insert(new_word);
			}

			// We get the neighbors of p
			neighbors = (m_PeriodicBorders ? grid.PeriodicNeighbors(p, 1) : grid.Neighbors(p, 1));

			for( it_coords=neighbors.begin(); it_coords!=neighbors.end(); it_coords++ )
			{
				// If the neighbor is not marked ...
				if( std::find(visited_nodes.begin(), visited_nodes.end(), *it_coords) == visited_nodes.end() )
				{
					// We add the neighbor
					visited_nodes.push_back(p);

					// We launch again the process from this neighbor
					_Solve(grid, *it_coords, children, new_word, new_path, visited_nodes, words, paths);
				}
			}
		}
	}

     public :
	CLexicographicTree();
	virtual ~CLexicographicTree();
	const bool Load( const std::string & dict_filename, const uint min_word_length, const uint max_word_length );
	const uint GetNumberOfWords() const;
	void SaveWords( const std::string & dict_filename ) const;
	void PeriodicBorders( const bool periodic_borders );
	const bool & PeriodicBorders() const;

	TWordsList Solve( const CGrid & grid )
	{
		TWordsList final_words;
		TPathsList final_paths;
		TWordsListCIt it_words;
		TPathsListCIt it_paths;
		TCoordsList visited_nodes;


		// We reinit the score and the mean curvature
		m_Score         = 0;
		m_MeanCurvature = 0.0f;

		// We assign the number of threads as the number of processors
		omp_set_num_threads(omp_get_num_procs());

		// We add the first node position
		visited_nodes.push_back(CCoords(grid.Dim(), 0));

		// We browse the grid
		//#pragma omp parallel private(it_words,it_paths)
		for( CCoordsIterator it_coords(grid.Begin()); it_coords!=grid.End(); it_coords++ )
		{
			// We launch the process for each point and gather final words and paths
			TWordsList tmp_words;
			TPathsList tmp_paths;

			_Solve(grid, *it_coords, m_Root, "", CPath(grid.Dim()), visited_nodes, tmp_words, tmp_paths);

			for( it_words=tmp_words.begin(); it_words!=tmp_words.end(); it_words++ )
			{
				//#pragma omp critical
				{
					final_words.insert(*it_words);
				}
			}

			for( it_paths=tmp_paths.begin(); it_paths!=tmp_paths.end(); it_paths++ )
			{
				//#pragma omp critical
				{
					final_paths.push_back(*it_paths);
				}
			}
		}

		// We compute the score
		for( it_words=final_words.begin(); it_words!=final_words.end(); it_words++ )
			m_Score += (*it_words).size();

		// We compute the mean curvature
		for( it_paths=final_paths.begin(); it_paths!=final_paths.end(); it_paths++ )
			m_MeanCurvature += (*it_paths).MeanCurvature();

		m_MeanCurvature /= final_paths.size();

		return final_words;
	}

	const int & Score() const
	{
		return m_Score;
	}

	const float & MeanCurvature() const
	{
		return m_MeanCurvature;
	}
};

#endif //__LEXICOGRAPHIC_TREE_H__
