/*
 *	GAST-Viz
 *	version 1.0.0 (04 January 2004)
 *
 *	interactionslibrary.cpp
 *	SOURCE
 *		Functions? Woah, good!
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

#include <math.h>
#include "interactionlibrary.h"
#include "vectors.h"

struct interact coulombic(Atom* first, Atom* second)
{
	const double Kcoulomb = 1.0;

	struct interact result;				//initialize
	result.potentials = new double[2];	//allocate
	result.forces = new Vector3D[2];

	Vector3D R(second->getPosition() - first->getPosition());	//r(hat)
	double rsq = R.magnitudesq();	//r^2
	double r = sqrt(rsq);			//r
	
	double KqQ = Kcoulomb * first->getCharge() * second->getCharge();		//KqQ

	double rmin = (first->getRadius() + second->getRadius()); //rmin

	/*
	V = KqQ/r * (1 - 1/19 * (rmin/r)^18)
	F = -KqQ/r^2 * (1 - (rmin/r)^18
	*/

	result.potentials[0] = KqQ/r * (1 - pow(rmin/r,18.0)/19.0);
	result.potentials[1] = result.potentials[0];

	result.forces[0] = R*(-KqQ/(r*rsq) * (1 - pow(rmin/r,18.0)));
	result.forces[1] = -result.forces[0];

	return result;
}

//this is a hard wall algorithm that I devised using a simple power law
struct interact hardwall(Atom* myatom, Surface* mysurface)
{
	struct interact result;				//initialize
	result.potentials = new double[1];	//allocate
	result.forces = new Vector3D[1];

	double rad = myatom->getRadius();	//optimal distance
	Vector3D path(mysurface->pathtopoint(myatom->getPosition()));	//vector distance from surface to atom

	double rsq = path.magnitudesq();	//we're dealing with powers so we don't need to waste the time calculating the magnitude

	/*	the following implements this power law
	V(r) = (1/16) * (2*(rad/r)^36 - 3*(rad/r)^24)
	F(r) = -V'(r) = -(1/16) * (72*(rad/r)^37 - 72*(rad/r)^25)
	*/

	double rmor12 = pow(rad*rad/rsq,6.0);	//using powers that are multiples of 12 makes for only one call of pow()
	double rmor24 = rmor12*rmor12;			//then just a few multiplications
	double rmor36 = rmor12*rmor24;			//which is cheaper than a bunch of pow() calls

	result.potentials[0] = 0.0625*(2*rmor36-3*rmor24);		//yeah, this is the power law that I mentioned
	result.forces[0] = path*((4.5/rsq)*(rmor36-rmor24));	//and the negative is factored in to the path vector

	return result;
}

//hard sphere algorithm (operates as the above hardwall)
struct interact hardsphere(Atom* first, Atom* second)
{
	struct interact result;
	result.potentials = new double[2];
	result.forces = new Vector3D[2];

	double rad = first->getRadius() + second->getRadius();
	Vector3D path(first->getPosition() - second->getPosition());

	double rsq = path.magnitudesq();

	double rmor12 = pow(rad*rad/rsq,6.0);
	double rmor24 = rmor12*rmor12;
	double rmor36 = rmor12*rmor24;

	result.potentials[0] = 0.0625*(2*rmor36-3*rmor24);
	result.potentials[1] = result.potentials[0];
	result.forces[0] = path*((4.5/rsq)*(rmor36-rmor24));
	result.forces[1] = -result.forces[0];

	return result;
}

Vector3D gravity_down(0.0, -1.0, 0.0);	//we need a default value for down
struct interact gravity(Atom* myatom)
{
	struct interact result;
	result.potentials = NULL;	//we don't have a good reference point, so I'll go with potential = 0; if you want potential, write your own gravity
	result.forces = new Vector3D[1];

	result.forces[0] = gravity_down * (9.8 * myatom->getMass());	//F = mg in the direction of down

	return result;
}
