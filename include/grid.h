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

#ifndef __GRID_H__
#define __GRID_H__

#include <vector>
#include <cassert>
#include <iostream>
#include "types.h"
#include "utils.h"
#include "coords.h"
#include "language_config.h"

using std::cout;
using std::endl;
using std::flush;


class CGrid
{
     public :
	//------- Grid -------
	typedef std::vector<TWordType>    TGridData;
	typedef TGridData::iterator       TGridDataIt;
	typedef TGridData::const_iterator TGridDataCIt;
	//------ Coords ------
	typedef std::vector<CCoords>        TCoordsList;
	typedef TCoordsList::const_iterator TCoordsListCIt;
	typedef TCoordsList::iterator       TCoordsListIt;
	//------ Letters ------
	typedef CLanguageConfig::TLettersList    TLettersList;
	typedef CLanguageConfig::TLettersListCIt TLettersListCIt;
	typedef CLanguageConfig::TLettersListIt  TLettersListIt;
	//------ Weights ------
	typedef CLanguageConfig::TWeightsList    TWeightsList;
	typedef CLanguageConfig::TWeightsListCIt TWeightsListCIt;
	typedef CLanguageConfig::TWeightsListIt  TWeightsListIt;

     protected :
	uint m_Dim;
	CCoords m_Sizes;
	TGridData m_GridData;

	void _Init()
	{
		std::fill(m_GridData.begin(), m_GridData.end(), TWordType());
	}

     public :
	CGrid() : m_Dim(0), m_Sizes(0, 0){}

	CGrid( const uint dim, const uint size ) : m_Dim(dim), m_Sizes(dim, size)
	{
		assert(dim > 0 && size > 0);
		Resize(CCoords(dim, size));
	}

	~CGrid(){}

	CCoordsIterator Begin() const
	{
		return CCoordsIterator(CCoords(m_Dim, 0), m_Sizes-CCoords(m_Dim, 1));
	}

	CCoordsIterator End() const
	{
		return CCoordsIterator(m_Dim);
	}

	const CCoords RandomPoint() const
	{
		CCoords p(m_Dim);

		for( uint k=0; k<p.Dim(); k++ )
			p[k] = Random<CCoords::Type>(0, m_Sizes[k]);

		return p;
	}

	CGrid SubGrid( const CCoords & offset, const CCoords & sizes ) const
	{
		assert(offset.Dim() == sizes.Dim() && offset.Dim() == m_Dim);
		CGrid grid(m_Dim, sizes[0]);

		for( CCoordsIterator p=grid.Begin(); p!=grid.End(); ++p )
			grid(*p) = operator () (*p + offset);

		return grid;
	}

	TCoordsList NegativeHalfNeighbors( const CCoords & c, const uint neighborhood_radius ) const
	{
		assert(c.Dim() == m_Dim && neighborhood_radius > 0);

		TCoordsList result;
		CCoords neighbor(m_Dim);
		CCoordsIterator it(CCoords(m_Dim, 0), CCoords(m_Dim, 2*neighborhood_radius+1)-1), end(m_Dim);

		while( it != end )
		{
			neighbor = (*it - neighborhood_radius + c);

			if( neighbor != c && neighbor >= 0 && neighbor < m_Sizes && (*it - neighborhood_radius) <= 0 )
				result.push_back(neighbor);

			it++;
		}

		return result;
	}

	TCoordsList PositiveHalfNeighbors( const CCoords & c, const uint neighborhood_radius ) const
	{
		assert(c.Dim() == m_Dim && neighborhood_radius > 0);

		TCoordsList result;
		CCoords neighbor(m_Dim);
		CCoordsIterator it(CCoords(m_Dim, 0), CCoords(m_Dim, 2*neighborhood_radius+1)-1), end(m_Dim);

		while( it != end )
		{
			neighbor = (*it - neighborhood_radius + c);

			if( neighbor != c && neighbor >= 0 && neighbor < m_Sizes && (*it - neighborhood_radius) >= 0 )
				result.push_back(neighbor);

			it++;
		}

		return result;
	}

	TCoordsList Neighbors( const CCoords & c, const uint neighborhood_radius ) const
	{
		assert(c.Dim() == m_Dim && neighborhood_radius > 0);

		TCoordsList result;
		CCoords neighbor(m_Dim);
		CCoordsIterator it(CCoords(m_Dim, 0), CCoords(m_Dim, 2*neighborhood_radius+1)-1), end(m_Dim);

		while( it != end )
		{
			neighbor = (*it - neighborhood_radius + c);

			if( neighbor != c && neighbor >= 0 && neighbor < m_Sizes )
				result.push_back(neighbor);

			it++;
		}

		return result;
	}

	TCoordsList PeriodicNeighbors( const CCoords & c, const uint neighborhood_radius ) const
	{
		assert(c.Dim() == m_Dim && neighborhood_radius > 0);

		TCoordsList result;
		CCoords neighbor(m_Dim);
		CCoordsIterator it(CCoords(m_Dim, 0), CCoords(m_Dim, 2*neighborhood_radius+1)-CCoords(m_Dim, 1)), end(m_Dim);

		while( it != end )
		{
			neighbor = (*it - CCoords(m_Dim, neighborhood_radius) + c);

			if( neighbor != c )
			{
				for( uint k=0; k<m_Dim; k++ )
				{
					if( neighbor[k] < 0 )           neighbor[k] = -neighbor[k];
					if( neighbor[k] >= m_Sizes[k] ) neighbor[k] = (neighbor[k] % m_Sizes[k]);
				}

				result.push_back(neighbor);
			}

			it++;
		}

		return result;
	}

	void Resize( const CCoords & sizes )
	{
		assert(sizes > 0);

		m_Sizes    = sizes;
		m_GridData = TGridData(m_Sizes.Product());

		_Init();
	}

	int Offset( const CCoords & c ) const
	{
		assert(m_Dim == c.Dim());

		int o = c[m_Dim - 1];
		assert(o >= 0 && o < m_Sizes[m_Dim - 1]);

		for( int d=(m_Dim - 2); d>=0; d-- )
		{
			assert(c[d] >= 0 && c[d] < m_Sizes[d]);
			o = o * m_Sizes[d] + c[d];
		}

		return o;
	}

	TWordType operator () ( const uint k ) const
	{
		assert(k <= uint(Offset(m_Sizes-1)));
		return m_GridData[k];
	}

	TWordType & operator () ( const uint k )
	{
		assert(k <= uint(Offset(m_Sizes-1)));
		return m_GridData[k];
	}

	TWordType operator () ( const CCoords & c ) const
	{
		return m_GridData[Offset(c)];
	}

	TWordType & operator () ( const CCoords & c )
	{
		return m_GridData[Offset(c)];
	}

	void Print( const uint gap = 2 )
	{
		if( m_Dim == 2 )
		{
			register int x, y;
			register uint k;

			for( y=0; y<m_Sizes[1]; y++ )
			{
				for( x=0; x<m_Sizes[0]; x++ )
				{
					TWordType tmp = (*this)(CCoords(x, y));
					cout << tmp;

					for( k=0; k<(gap*3-tmp.size()+1); k++ )
						cout << " " << flush;
				}

				if( y != (m_Sizes[1]-1) )
				{
					cout << endl;

					for( k=0; k<gap; k++ )
						cout << endl;
				}
			}

			cout << endl;
		}
		else if( m_Dim == 3 )
		{
			register int x, y, z;
			register uint k;

			for( z=0; z<m_Sizes[2]; z++ )
			{
				for( y=0; y<m_Sizes[1]; y++ )
				{
					for( x=0; x<m_Sizes[0]; x++ )
					{
						TWordType tmp = (*this)(CCoords(x, y, z));
						cout << tmp;

						for( k=0; k<(gap*2-tmp.size()+1); k++ )
							cout << " " << flush;
					}

					if( y != (m_Sizes[1]-1) )
					{
						for( k=0; k<gap; k++ )
							cout << endl << flush;
					}
					else
						cout << endl << flush;
				}

				if( z != (m_Sizes[2]-1) )
					cout << std::string(m_Sizes[0]*4, '*') << endl << flush;
			}
		}
	}

	void Generate( const CLanguageConfig & language_config )
	{
		TGridDataIt it_grid_data(m_GridData.begin());
		TLettersList letters(language_config.Letters());
		TWeightsList weights(language_config.Weights());
		TWeightsListCIt it_weights(weights.begin());
		register uint cumul(0), k(1), letters_size(letters.size());
		TWeightsList cumul_weights(letters_size+1);


		cumul_weights[0] = 0;

		while( it_weights < weights.end() )
		{
			cumul += *it_weights;
			cumul_weights[k++] = cumul;
			it_weights++;
		}

		while( it_grid_data < m_GridData.end() )
		{
			uint n(Random<int>(0, cumul_weights[letters_size]));
			uint lb(0), ub(letters_size-1);

			while( lb+1 < ub )
			{
				uint middle = (lb + ub) / 2;

				if( n <= cumul_weights[middle] ) ub = middle;
				if( n >= cumul_weights[middle] ) lb = middle;
			}

			*it_grid_data = letters[lb];
			it_grid_data++;
		}
	}

	const uint TotalSize() const
	{
		return m_Sizes.Product();
	}

	const uint Size() const
	{
		return m_Sizes[0];
	}

	const uint Dim() const
	{
		return m_Dim;
	}

	void Swap( const CCoords & c1, const CCoords & c2 )
	{
		TWordType w = (*this)(c1);
		(*this)(c1) = (*this)(c2);
		(*this)(c2) = w;
	}

	void Shuffle()
	{
		std::random_shuffle(m_GridData.begin(), m_GridData.end());
	}
};

#endif //__GRID_H__
