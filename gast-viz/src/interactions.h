/*
 *	GAST-Viz
 *	version 1.0.0 (04 January 2004)
 *
 *	interactions.h
 *	HEADER
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

#ifndef _interactions_h_
#define _interactions_h_

#include "atoms.h"
#include "surfaces.h"

//all force functions need to return an interact
struct interact
{
	Vector3D* forces;	//a 1D array of vectors representing the force to be exerted on each atom
	double* potentials;	//a 1D array of doubles representing the potentials contribution to each atom
	//if forces or potentials is a NULL pointer, that value will be interpreted as a zero (this can be useful)
};

/*===============================================
	class Interaction1B, Interaction2B,
		Interaction3D, InteractionS

	The interaction classes serve as containers
	for one body, two body, three body and
	one body/wall interaction equations.
===============================================*/
class Interaction1B
{
public:
	Interaction1B(struct interact (*onebody)(Atom*));	//constructor takes a pointer to a force function
	~Interaction1B();

	inline bool enabled() const {return isenabled;};	//query enabled status
	inline bool enabled(bool enabled) {isenabled = enabled; return isenabled;};	//set enabled status
	
	void interact(Atom* first);	//this will actually perform the interaction if it is enabled
	
private:
	bool isenabled;
	struct interact results;
	struct interact (*forcefunct)(Atom*);
};

class Interaction2B
{
public:
	Interaction2B(struct interact (*twobody)(Atom*, Atom*));
	~Interaction2B();

	inline bool enabled() const {return isenabled;};
	inline bool enabled(bool enabled) {isenabled = enabled; return isenabled;};

	void interact(Atom* first, Atom* second);
	
private:
	bool isenabled;
	struct interact results;
	struct interact (*forcefunct)(Atom*, Atom*);
};

class Interaction3B
{
public:
	Interaction3B(struct interact (*threebody)(Atom*, Atom*, Atom*));
	~Interaction3B();

	inline bool enabled() const {return isenabled;};
	inline bool enabled(bool enabled) {isenabled = enabled; return isenabled;};

	void interact(Atom* first, Atom* second, Atom* third);
	
private:
	bool isenabled;
	struct interact results;
	struct interact (*forcefunct)(Atom*, Atom*, Atom*);
};

class InteractionS
{
public:
	InteractionS(struct interact (*oneandwall)(Atom*, Surface*));
	~InteractionS();

	inline bool enabled() const {return isenabled;};
	inline bool enabled(bool enabled) {isenabled = enabled; return isenabled;};

	void interact(Atom* first, Surface* wall);
	
private:
	bool isenabled;
	struct interact results;
	struct interact (*forcefunct)(Atom*, Surface*);
};

#endif
