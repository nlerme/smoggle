/*
    Smoggle - A multidimensional and multi-language boggle solver
    Copyright (C) 2009 - Nicolas Lerm� <nicolas.lerme@free.fr>

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

#ifndef __TIMER_H__
#define __TIMER_H__

#include <cstdlib>
#ifndef _WIN32
#	include <sys/time.h>
#else
#	include <windows.h>
#endif


class CTimer
{
     private :
	#ifndef _WIN32
		timeval m_AbsoluteTime;
	#else
		LARGE_INTEGER m_AbsoluteTime, m_TicksPerSecond;
	#endif

     public :
	CTimer();
	void Init();
	double Time() const;
};

#endif //__TIMER_H__