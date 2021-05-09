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
#include "utils.h"
#include "iboggle_simulated_annealing.h"


CIBoggleSimulatedAnnealing::CIBoggleSimulatedAnnealing(){}

CIBoggleSimulatedAnnealing::~CIBoggleSimulatedAnnealing(){}

void CIBoggleSimulatedAnnealing::Compute( CGrid & grid, CLexicographicTree & tree )
{
	float temperature;
	int max_temperature(1000), current_score, new_score(0);


	std::cout << "\n[ Resolution]\n";

	for( int t=0; t<max_temperature; t++ )
	{
		CCoords p, q;

		do
		{
			p = grid.RandomPoint();
			q = grid.RandomPoint();
		}
		while( p == q );

		grid.Swap(p, q);
		tree.Solve(grid);
		current_score = tree.Score();
		temperature   = 10.0f / sqrtf(t);

		if( current_score > new_score )
		{
			// We keep the solution
			new_score = current_score;
			std::cout << "  + new score = " << new_score << '\n';
		}
		else
		{
			if( current_score < new_score && Random<float>(0.0f, 1.0f) < expf(-(new_score-current_score) / temperature) )
			{
				// We keep the solution
				new_score = current_score;
				std::cout << "  + new score = " << new_score << '\n';
			}
			else
			{
				// We reject the solution
				grid.Swap(p, q);
			}
		}
	}
}
