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

#include "timer.h"


CTimer::CTimer()
{
	Init();
}

void CTimer::Init()
{
	#ifdef _WIN32
		QueryPerformanceFrequency(&m_TicksPerSecond);
		QueryPerformanceCounter(&m_AbsoluteTime);
	#else
		gettimeofday(&m_AbsoluteTime,NULL);
	#endif
}

double CTimer::Time() const
{
	double ElapsedTime;

	#ifdef _WIN32
		LARGE_INTEGER NewTime;
		QueryPerformanceCounter(&NewTime);
		ElapsedTime = (double(NewTime.QuadPart) - double(m_AbsoluteTime.QuadPart)) / double(m_TicksPerSecond.QuadPart);
	#else
		timeval NewTime;
		gettimeofday(&NewTime,NULL);
		ElapsedTime = (NewTime.tv_usec - m_AbsoluteTime.tv_usec) / 1000000. + NewTime.tv_sec - m_AbsoluteTime.tv_sec;
	#endif

	return ElapsedTime;
}
