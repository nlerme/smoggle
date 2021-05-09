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

#ifndef __PATH_H__
#define __PATH_H__

#include <cassert>
#include "types.h"
#include "coords.h"
#include "grid.h"


class CPath
{
     public :
	//----- CoordsList -----
	typedef CGrid::TCoordsList    TCoordsList;
	typedef CGrid::TCoordsListCIt TCoordsListCIt;

     private :
	TCoordsList m_Nodes;
	uint m_Dim;

     public :
	CPath( const uint dim = 0 );
	~CPath();
	void AddNode( const CCoords & p );
	float MeanCurvature() const;
	float MaxCurvature() const;
	const uint & Dim() const;
	const CCoords & Node( const uint k ) const;
	const uint Size() const;

	friend std::ostream & operator << ( std::ostream & out, const CPath & other )
	{
		out << "[";

		for( uint k=0; k<other.Size(); k++ )
			out << other.Node(k) << ((k<other.Size()-1) ? " " : "");

		out << "]";

		return out;
	}
};

#endif //__PATH_H__
