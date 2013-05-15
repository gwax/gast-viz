/*
 *	GAST-Viz
 *	version 1.0.0 (04 January 2004)
 *
 *	lj6_12.cpp
 *	SOURCE
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

#include "lj6_12.h"
#include "vectors.h"
#include "functions.inl.h"

//static variables need init values
double lj6_12::defaultepsilon = 1.0;
double** lj6_12::epsilons = NULL;
int lj6_12::dim = -1;

//here's the interaction it's pretty straight forward
struct interact lj6_12::forcefunction(Atom* one, Atom* two)
{
	double emin = defaultepsilon;
	struct interact result;
	double rmin = (one->getRadius() + two->getRadius())/twototheonesixth; //I don't know why I need the 2^(1/6) correction, but I do

	if(one->getType() >= 0 && two->getType() >= 0 && one->getType() <= dim && two->getType() <= dim)	//saves a function call
		emin = epsilons[one->getType()][two->getType()];

	/*	here's the potential and force that we'll return
	V = 4*emin( (rmin/r)^6 - (rmin/r)^12)

	F = (24*emin/r)( (rmin/r)^6 - 2*(rmin/r)^12 )
	*/

	Vector3D Rvect(two->getPosition() - one->getPosition());
	double rsq = Rvect.magnitudesq();

	result.potentials = new double[2];
	result.forces = new Vector3D[2];

	double rmor2 = rmin*rmin/rsq;
	double rmor6 = rmor2*rmor2*rmor2;
	double rmor12 = rmor6*rmor6;

	result.potentials[0] = -4.0*emin*(rmor6-rmor12);
	result.potentials[1] = result.potentials[0];

	result.forces[0] = Rvect*(24*(emin/rsq)*(rmor6-2.0*rmor12));
	result.forces[1] = -result.forces[0];

	return result;
}

double lj6_12::getepsilon(int type1, int type2)
{
	if(type1 < 0 || type2 < 0 || type1 > dim || type2 > dim)
		return defaultepsilon;	//if the types are outside the range, return default epsilon

	return epsilons[type1][type2];	//otherwise return requested epsilon
}

double lj6_12::setdefaultepsilon(double newDefaultepsilon)
{
	if(newDefaultepsilon < 0.0)
		return -1.0;

	defaultepsilon = newDefaultepsilon;
	return defaultepsilon;
}

double lj6_12::setepsilon(int type1, int type2, double newepsilon)
{
	if(type1 < 0 || type2 < 0 || newepsilon < 0.0)	//on error return -1.0
		return -1.0;

	if(type1 > dim || type2> dim)	//if epsilons[][] is not large enough, make it bigger
	{
		int i, j;
		int newdim = mymax(dim, mymax(type1, type2));
		double** newepsilons;
		newepsilons = new double*[newdim+1];
		for(i=0; i<=newdim; i++)
		{
			newepsilons[i] = new double[newdim+1];
		}
		for(i=0; i<=dim; i++)
		{
			for(j=0; j<=dim; j++)
			{
				newepsilons[i][j] = epsilons[i][j];
			}
		}

		for(i=dim+1; i<=newdim; i++)
		{
			for(j=0; j<=newdim; j++)
			{
				newepsilons[i][j] = defaultepsilon;
				newepsilons[j][i] = defaultepsilon;
			}
		}

		double** oldepsilons = epsilons;
		epsilons = newepsilons;
		dim = newdim;
		delete [] oldepsilons;
	}

	epsilons[type1][type2] = newepsilon;	//set the new epsilon value
	epsilons[type2][type1] = newepsilon;	//in both directions
	return newepsilon;
}
