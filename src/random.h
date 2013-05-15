/*
 *	GAST-Viz
 *	version 1.0.0 (04 January 2004)
 *
 *	random.h
 *	HEADER
 *		Functions that return pseudorandomness
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

/*
please note that bounds are not checked for
ranges, so if you give unreasonable values,
don't expect reasonable results; that said,
you will get results, just not necessarily
as expected

also, for performance reasons, modulo
functions are used in bounded integer
functions; this decreases randomness
slightly, so, if you mind, I recommend
constructing your own bounded functions
based on the unbounded ones provided here
*/

#ifndef _random_h_
#define _random_h_

#include "vectors.h"
#include <stdlib.h>

//Random function implemented as a static class to make my life easy
class Random
{
public:
	const static int MAX_INT = RAND_MAX;	//maximum size of a random integer
	const static long MAX_LONG = (long)MAX_INT*((long)MAX_INT+1);	//maximum size of a random long
	
	static unsigned int timeseed();	//set the random seed based on the current time; return seed
	static unsigned int newseed(unsigned int newSeed);	//set the seed arbitrarily; return seed

	inline static unsigned int getseed() {return seed;};	//return current seed

	static int randint();	//return int between 0 and Random::MAX_INT
	static long randlong();	//return long between 0 and Random::MAX_LONG
	static int randint(int lower, int upper);	//return an int between lower and upper
	static long randlong(long lower, long upper);	//return a long between lower and upper

	static float randfloat();	//return a float between 0 and 1
	static double randdouble();	//return a double between 0 and 1 (unless you really need randomness, use (double)randfloat(), it's less intensive
	static float randfloat(float lower, float upper);	//return a float between lower and upper
	static double randdouble(double lower, double upper);	//return a double between lower and upper (ditto the earlier warning on doubles)
	static float randhalfintervalf();	//return a float between -0.5 and 0.5
	static double randhalfintervald();	//return a double between -0.5 and 0.5 (ditto above warnings)

	static Vector3D randvect(const Vector3D& minvect, const Vector3D& maxvect);	//return a vector with components between the components of minvect and maxvect
	
	static Vector3D randdirvect();	//return a random direction vector
	static Vector3D randdirvect(bool xcomp, bool ycomp, bool zcomp);	//return a direction vector with non-zero components possible only in directions where the associated 'comp' bool is true

private:
	static unsigned int seed;
};

#include "random.inl.h"

#endif
