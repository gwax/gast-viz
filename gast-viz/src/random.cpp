/*
 *	GAST-Viz
 *	version 1.0.0 (04 January 2004)
 *
 *	random.cpp
 *	SOURCE
 *		Random functions, get it?
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

#include "random.h"
#include <time.h>

/*===============================================
	class Random
===============================================*/
unsigned int Random::seed = 0;	//static variables need init values

/*-----------------------------------------------
	Random methods
-----------------------------------------------*/
unsigned int Random::timeseed()
{
	seed = (unsigned)time(NULL);
	srand(seed);
	return seed;
}

unsigned int Random::newseed(unsigned int newSeed)
{
	seed = newSeed;
	srand(seed);
	return seed;
}

Vector3D Random::randvect(const Vector3D& minvect, const Vector3D& maxvect)
{	//it's pretty simple
return Vector3D((double)randfloat((float)minvect.getX(), (float)maxvect.getX()),
				(double)randfloat((float)minvect.getY(), (float)maxvect.getY()),
				(double)randfloat((float)minvect.getZ(), (float)maxvect.getZ()));
}

Vector3D Random::randdirvect()
{
	Vector3D temp((double)randhalfintervalf(), (double)randhalfintervalf(), (double)randhalfintervalf());	//choose directions
	return temp.normalvector();	//normalize and return
}

Vector3D Random::randdirvect(bool xcomp, bool ycomp, bool zcomp)
{	//the bools act as directional masks
	if(!(xcomp || ycomp || zcomp))
		return Vector3D(0.0);

	Vector3D temp((double)randhalfintervalf(), (double)randhalfintervalf(), (double)randhalfintervalf());

	if(!xcomp)
		temp.setX(0.0);

	if(!ycomp)
		temp.setY(0.0);

	if(!zcomp)
		temp.setZ(0.0);

	return temp.normalvector();
}
