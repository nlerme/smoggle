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
#include "iboggle_genetic_algorithm.h"


CIBoggleGeneticAlgorithm::CIBoggleGeneticAlgorithm(){}

CIBoggleGeneticAlgorithm::~CIBoggleGeneticAlgorithm(){}

void CIBoggleGeneticAlgorithm::Compute( CGrid & grid, CLexicographicTree & tree )
{
	uint population_size(200), nb_generations(50);
	std::vector<CGrid> population(population_size);
	//std::vector<float> scores(population_size);
	//float scores_sum(0.0f);
	uint best_solution, max_score, score;


	std::cout << "\n[ Resolution ]\n";

	// Initialization
	for( uint i=0; i<population.size(); i++ )
	{
		population[i] = CGrid(grid);
		population[i].Shuffle();
	}

	// We iterate during multiple generations
	for( uint i=0; i<nb_generations; i++ )
	{
		// Selection
		/*scores_sum = 0.0f;

		for( uint j=0; j<population_size; j++ )
		{
			tree.Solve(population[j]);
			scores[j]   = tree.Score();
			scores_sum += scores[j];
		}

		for( uint j=0; j<population_size; j++ )
			scores[j] /= scores_sum;

		std::sort(scores.begin(), scores.end(), std::greater<float>());

		for( uint j=1; j<population_size; j++ )
			scores[j] = scores[j] + scores[j-1];

		for( uint j=0; j<population_size; j++ )
			std::cout << "scores[" << j << "] = " << scores[j] << '\n';*/

		best_solution = 0;
		max_score     = 0;

		for( uint j=0; j<population_size; j++ )
		{
			tree.Solve(population[j]);
			score = tree.Score();

			if( score > max_score )
			{
				max_score = score;
				best_solution = j;
			}
		}

		std::cout << "  + generation " << i << " | score = " << max_score << '\n';

		// Reproduction
		for( uint j=0; j<population_size; j++ )
		{
			if( j != best_solution )
			{
				for( CCoordsIterator p(population[j].Begin()); p!=population[j].End(); p++ )
				{
					// Crossover
					if( Random() == 1 )
					{
						if( population[j](*p) != population[best_solution](*p) )
						{
							for( CCoordsIterator q(population[j].Begin()); q!=population[j].End(); q++ )
							{
								if( population[j](*q) == population[best_solution](*p) )
								{
									population[j](*q) = population[j](*p);
									break;
								}
							}

							population[j](*p) = population[best_solution](*p);
						}
					}

					// Mutation
					if( Random(0.0f, 1.0f) < 0.1f )
					{
						CCoords q;

						do
						{
							q = population[j].RandomPoint();
						}
						while( *p == q );

						population[j].Swap(*p, q);
					}
				}
			}
		}
	}

	grid = population[best_solution];
}
