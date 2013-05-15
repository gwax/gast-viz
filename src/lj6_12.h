/*
 *	GAST-Viz
 *	version 1.0.0 (04 January 2004)
 *
 *	lj6_12.h
 *	HEADER
 *		a static class that handles
 *	everything related to an lj6_12
 *	potential and contains an exportable
 *	two body force function
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

#ifndef _lj6_12_h_
#define _lj6_12_h_

#include "interactions.h"
#include "atoms.h"

/*	this class had to be implemented as a static class
	because Interaction2B needs a pointer to a
	function and member functions have different
	pointer types, but static member functions and
	normal functions have the same pointer types
	*/

const double twototheonesixth = pow(2.0,1.0/6.0);

class lj6_12
{
public:
	static struct interact forcefunction(Atom* one, Atom* two);		//this is the forcefunction that your Interaction2B should point to
	static double setdefaultepsilon(double newDefaultepsilon);			//change the default epsilon value for unknown type pairs; return new default epsilon, or on error, return -1.0
	inline static double getdefaultepsilon() {return defaultepsilon;};	//tells you what the current default epsilon value is
	static double setepsilon(int type1, int type2, double newepsilon);	//set or change the epsilon value for interactions between type1 and type2 atoms; return new epsilon, on error return -1.0
	static double getepsilon(int type1, int type2);					//tells you the epsilon value for interactions between type1 and type2 atoms

private:
	static double defaultepsilon;
	static double** epsilons;
	static int dim;
};

#endif
