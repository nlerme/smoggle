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

#include "path.h"


CPath::CPath( const uint dim ) : m_Dim(dim){}

CPath::~CPath(){}

void CPath::AddNode( const CCoords & p )
{
	assert(p.Dim() == m_Dim);
	m_Nodes.push_back(p);
}

float CPath::MeanCurvature() const
{
	float mean_curv(0.0f);


	// We check if we have at least three nodes in the path
	assert(m_Nodes.size()>=3);

	// We compute the curvature
	for( TCoordsListCIt it(m_Nodes.begin()+1); it<(m_Nodes.end()-1); it++ )
		mean_curv += CCoords::Curvature(*(it-1), *it, *(it+1));

	return (mean_curv / m_Nodes.size());
}

float CPath::MaxCurvature() const
{
	float max_curv(0.0f);


	// We check if we have at least three nodes in the path
	assert(m_Nodes.size()>=3);

	// We compute the curvature
	for( TCoordsListCIt it(m_Nodes.begin()+1); it<(m_Nodes.end()-1); it++ )
		max_curv = Max(max_curv, CCoords::Curvature(*(it-1), *it, *(it+1)));

	return max_curv;
}

const uint & CPath::Dim() const
{
	return m_Dim;
}

const CCoords & CPath::Node( const uint k ) const
{
	assert(k >= 0 && k < m_Nodes.size());
	return m_Nodes[k];
}

const uint CPath::Size() const
{
	return m_Nodes.size();
}
