/*
 *	GAST-Viz
 *	version 1.0.0 (04 January 2004)
 *
 *	interactionslibrary.h
 *	HEADER
 *		just a bunch of functions that can
 *	be used as Interactions
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

#ifndef _interactionslibrary_h_
#define _interactionslibrary_h_

#include "atoms.h"
#include "interactions.h"
#include "lj6_12.h"	//this gives us the more complicated lennard-jones 6_12 interaction

struct interact coulombic(Atom* first, Atom* second);	//short range repulsion, long range coulombic attraction

//these hard * functions are pretty good, with one slight flaw: there is a VERY small, VERY short range attractive force, if this doesn't work for you, write your own
struct interact hardwall(Atom* myatom, Surface* mysurface);		//this is a hard wall algorithm that I came up with; it is based on a 36_24 power law
struct interact hardsphere(Atom* first, Atom* second);			//this is a hard sphere potential that uses the same algorithm as the above hardwall

extern Vector3D gravity_down;
struct interact gravity(Atom* myatom);

#endif
