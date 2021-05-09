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

#include <limits>
#include "iboggle_hill_climbing.h"


CIBoggleHillClimbing::CIBoggleHillClimbing(){}

CIBoggleHillClimbing::~CIBoggleHillClimbing(){}

void CIBoggleHillClimbing::Compute( CGrid & grid, CLexicographicTree & tree )
{
	int old_score(-1), new_score(0), max_score, current_score;
	int iteration(0);
	CSwap best_swap;


	std::cout << "\n[ Resolution ]\n";

	while( new_score > old_score )
	{
		std::cout << "  + iteration " << iteration << " | score=" << new_score << '\n';

		max_score = new_score;

		for( CCoordsIterator p=grid.Begin(); p!=grid.End(); p++ )
		{
			CGrid::TCoordsList neighbors = grid.NegativeHalfNeighbors(*p, 1);

			for( CGrid::TCoordsListIt q(neighbors.begin()); q!=neighbors.end(); q++ )
			{
				grid.Swap(*p, *q);
				tree.Solve(grid);
				current_score = tree.Score();

				if( current_score > max_score )
				{
					max_score = current_score;
					best_swap = CSwap(*p, *q);
				}

				grid.Swap(*p, *q);
			}
		}

		if( max_score > new_score )
			grid.Swap(best_swap.c1, best_swap.c2);

		old_score = new_score;
		new_score = max_score;

		iteration++;
	}
}
