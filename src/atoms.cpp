/*
 *	GAST-Viz
 *	version 1.0.0 (04 January 2004)
 *
 *	atoms.cpp
 *	SOURCE
 *		Atom classes derived from Particle
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

#include "atoms.h"

/*===============================================
	class Atom
===============================================*/


/*-----------------------------------------------
	Constructors
-----------------------------------------------*/
Atom::Atom(int type, const Vector3D& position, const Vector3D& velocity, double radius, double mass, double charge, double red, double green, double blue) :
Particle(position, radius, red, green, blue, velocity)
{
	this->type = type;
	this->mass = mass;
	this->charge = charge;
	defaultcolor[0] = red;
	defaultcolor[1] = green;
	defaultcolor[2] = blue;
	defaultcolor[3] = 1.0;

	force = Vector3D(0.0);
	newforce = Vector3D(0.0);
	potential = 0.0;
	newpotential = 0.0;

	energydraw = false;	//we default to drawing the normal way
	drawemin = 0.0;	//these are default values and shouldn't ever be used
	drawemax = 0.1;

	indexnum = -1;	//the assemblage wants atoms to have an index of -1 before they've been added

	neighbors = 0;	//we know of no neighbors at birth
}


/*-----------------------------------------------
	Destructor
-----------------------------------------------*/
Atom::~Atom()
{	//I've got nothing worth doing here
}


/*-----------------------------------------------
	Atom methods
-----------------------------------------------*/
void Atom::setType(int newType)
{
	type = newType;	//yeah that's what it does
}

void Atom::setMass(double newMass)
{
	mass = newMass;	//yeah that's what it does
}

void Atom::setCharge(double newCharge)
{
	charge = newCharge;	//yeah that's what it does
}

void Atom::setforcetonew()
{
	force = newforce;	//set the current force to the force buffer
	newforce.setVal(0.0, 0.0, 0.0);	//clear the force buffer
}


void Atom::setpotentialtonew()
{
	potential = newpotential;	//set the current potential
	newpotential = 0.0;	//clear the potential buffer

	if(energydraw)	//if we're using the energy to determine the color
	{
		if(potential < drawemin)		//if it's less than the minimum
			setColor(0.0, 0.0, 1.0);		//draw blue
		else if(potential > drawemax)	//if it's more than the maximum
			setColor(1.0, 0.0, 0.0);		//draw red
		else							//otherwise
		{									//draw a gradient in between
			double scale = (potential - drawemin)/(drawemax - drawemin);
			setColor(scale, 0.0, 1.0-scale);
		}
	}
}

void Atom::setEnergyColor(double emin, double emax)
{
	if(emax < emin)	//if emax and emin are wrong then this won't work right
		return;		//so just break out and fail

	energydraw = true;	//now we're drawing with energy coloring so we want to remember that
	
	drawemax = emax;	//change the internal ranges for energy coloring to those specified
	drawemin = emin;

	if(potential < drawemin)		//if it's less than the minimum
		setColor(0.0, 0.0, 1.0);		//draw blue
	else if(potential > drawemax)	//if it's more than the maximum
		setColor(1.0, 0.0, 0.0);		//draw red
	else							//otherwise
	{									//draw a gradient in between
		double scale = (potential - drawemin)/(drawemax - drawemin);
		setColor(scale, 0.0, 1.0-scale);
	}
}

void Atom::setDefaultColor()
{
	energydraw = false;	//no more energy coloring
	setColor(defaultcolor[0], defaultcolor[1], defaultcolor[2]);	//go back to default color
}

void Atom::changeDefaultColor(double r, double g, double b)
{	//different default color
	defaultcolor[0] = r;
	defaultcolor[1] = g;
	defaultcolor[2] = b;
}

void Atom::setIdx(int newindex)
{
	indexnum = newindex;	//yup, changes the index (I wouldn't change this on your own, it could break assemblage)
}

int Atom::addneighbor(Atom* newneighbor)	//this function could do with optimization
{
	if(this == newneighbor)	//if they try to add itself
		return neighbors;	//don't let it

	if(newneighbor->indexnum < indexnum)		//since neighboring is symmetrical we only want one to know about the other
		return newneighbor->addneighbor(this);	//so add this as a neighbor to the other

	if(neighbors == MAXNEIGHBORS)	//we've filled the array
		return MAXNEIGHBORS;		//don't add another

	for(int i=0; i<neighbors; i++)	//I should make this better
		if(neighborlist[i] == newneighbor)	//check if the neighbor is already in the list
			return neighbors;	//if it's already there, don't add it again

	neighborlist[neighbors] = newneighbor;	//add the new neighbor at the end of the list

	return ++neighbors;	//we've added one more neighbor so note that and return the number of neighbors we have
}

void Atom::clearneighbors()
{
	neighbors = 0;	//this just says to ignore everything and go back to the beginning
}
