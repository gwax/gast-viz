/*
 *	GAST-Viz
 *	version 1.0.0 (04 January 2004)
 *
 *	interactions.cpp
 *	SOURCE
 *		the various interaction classes
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

#include "interactions.h"

/*===============================================
	class Interaction{1B,2B,3B,S}
===============================================*/


/*-----------------------------------------------
	Constructors
-----------------------------------------------*/
Interaction1B::Interaction1B(struct interact (*onebody)(Atom*))
{
	forcefunct = onebody;	//set our local pointer to the given function
	isenabled = true;		//defaults to enabled
}

Interaction2B::Interaction2B(struct interact (*twobody)(Atom*, Atom*))
{
	forcefunct = twobody;
	isenabled = true;
}

Interaction3B::Interaction3B(struct interact (*threebody)(Atom*, Atom*, Atom*))
{
	forcefunct = threebody;
	isenabled = true;
}

InteractionS::InteractionS(struct interact (*oneandwall)(Atom*, Surface*))
{
	forcefunct = oneandwall;
	isenabled = true;
}


/*-----------------------------------------------
	Destructors
-----------------------------------------------*/
Interaction1B::~Interaction1B()
{
	delete [] results.forces;		//it can't hurt if we delete a NULL pointer and it can if
	delete [] results.potentials;	//we forget so we'll do it once more for safety's sake
}

Interaction2B::~Interaction2B()
{
	delete [] results.forces;
	delete [] results.potentials;
}

Interaction3B::~Interaction3B()
{
	delete [] results.forces;
	delete [] results.potentials;
}

InteractionS::~InteractionS()
{
	delete [] results.forces;
	delete [] results.potentials;
}


/*-----------------------------------------------
	Interaction{1B,2B,3B,S} methods
-----------------------------------------------*/
void Interaction1B::interact(Atom* first)
{
	if(!isenabled)	//if diabled, we don't care
		return;		//just return

	results = forcefunct(first);	//run the calculations using our force function

	if(results.forces != NULL)		//if we didn't get a NULL pointer force
		first->addtoforce(results.forces[0]);			//contribute to the atom's force

	if(results.potentials != NULL)	//if we didn't get a NULL pointer potential
		first->addtopotential(results.potentials[0]);	//contribute to the atom's force

	delete [] results.forces;		//kill our old pointers
	delete [] results.potentials;
}

void Interaction2B::interact(Atom* first, Atom* second)
{
	if(!isenabled)	//if diabled, we don't care
		return;		//just return

	results = forcefunct(first, second);	//run the calculations using our force function

	if(results.forces != NULL)		//if we didn't get a NULL pointer force
	{
		first->addtoforce(results.forces[0]);			//contribute to the atom's force
		second->addtoforce(results.forces[1]);
	}

	if(results.potentials != NULL)	//if we didn't get a NULL pointer potential
	{
		first->addtopotential(results.potentials[0]);	//contribute to the atom's force
		second->addtopotential(results.potentials[1]);
	}

	delete [] results.forces;		//kill our old pointers
	delete [] results.potentials;
}

void Interaction3B::interact(Atom* first, Atom* second, Atom* third)
{
	if(!isenabled)	//if diabled, we don't care
		return;		//just return

	results = forcefunct(first, second, third);	//run the calculations using our force function

	if(results.forces != NULL)		//if we didn't get a NULL pointer force
	{
		first->addtoforce(results.forces[0]);			//contribute to the atom's force
		second->addtoforce(results.forces[1]);
		third->addtoforce(results.forces[2]);
	}

	if(results.potentials != NULL)	//if we didn't get a NULL pointer potential
	{
		first->addtopotential(results.potentials[0]);	//contribute to the atom's force
		second->addtopotential(results.potentials[1]);
		third->addtopotential(results.potentials[2]);
	}

	delete [] results.forces;		//kill our old pointers
	delete [] results.potentials;
}

void InteractionS::interact(Atom* first, Surface* wall)
{
	if(!isenabled)	//if diabled, we don't care
		return;		//just return

	results = forcefunct(first, wall);	//run the calculations using our force function

	if(results.forces != NULL)		//if we didn't get a NULL pointer force
		first->addtoforce(results.forces[0]);			//contribute to the atom's force

	if(results.potentials != NULL)	//if we didn't get a NULL pointer potential
		first->addtopotential(results.potentials[0]);	//contribute to the atom's force

	delete [] results.forces;		//kill our old pointers
	delete [] results.potentials;
}
