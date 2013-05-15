/*
 *	GAST-Viz
 *	version 1.0.0 (04 January 2004)
 *
 *	random.inl.h
 *	INLINE HEADER
 *		inlines are good for speed
 *
 *	Copyright (c)2003-2004, George Waksman <waksman@mit.edu>. All rights reserverd.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _random_inl_h_
#define _random_inl_h_

/*===============================================
	class Random
===============================================*/

/*-----------------------------------------------
	Random methods
-----------------------------------------------*/
inline int Random::randint()
{
	return rand();
}

inline long Random::randlong()	//longs require two calls to the rand() function and should be used sparingly; jsut cast if you can; making two subsequent calls also lowers randomness a little
{
	return ((long)rand()*(long)(MAX_INT) + (long)rand());
}

inline int Random::randint(int lower, int upper)
{
	return (lower + randint()%(upper-lower+1));		//using % lowers the randomness but it's faster than the alternative so I'll live
}

inline long Random::randlong(long lower, long upper)
{
	return (lower + randlong()%(upper-lower+1));	//using % lowers the randomness but it's faster than the alternative so I'll live
}

inline float Random::randfloat()
{
	return ((float)randint()/(float)MAX_INT);
}

inline double Random::randdouble()
{
	return ((double)randlong()/(double)MAX_LONG);
}

inline float Random::randfloat(float lower, float upper)
{
	return (lower + randfloat()*(upper-lower));
}

inline double Random::randdouble(double lower, double upper)
{
	return (lower + randdouble()*(upper-lower));
}

inline float Random::randhalfintervalf()
{
	return (randfloat()-0.5f);
}

inline double Random::randhalfintervald()
{
	return (randdouble()-0.5);
}

#endif
