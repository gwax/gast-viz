/*
 *	GAST-Viz
 *	version 1.0.0 (04 January 2004)
 *
 *	atoms.h
 *	HEADER
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

#ifndef _atoms_h_
#define _atoms_h_

#include "particles.h"
#include "vectors.h"

/*===============================================
	class Atom

	Atoms are derived from Particles
===============================================*/
#define MAXNEIGHBORS 64

class Atom :
	public Particle
{
	friend class Assemblage;	//it's very much better for me this way

public:
	//I'm not giving you many constructor options because I want you to create them my way
	Atom(int type, const Vector3D& position, const Vector3D& velocity, double radius, double mass, double charge, double red, double green, double blue);
	
	~Atom();	//destructor

	//these are access functions for an Atom's various bits and pieces
	inline int getType() const {return type;};
	inline double getMass() const {return mass;};
	inline double getCharge() const {return charge;};
	inline double getPotential() const {return potential;};
	inline Vector3D& getForce() {return force;};

	void setType(int newType);	//sets type to new
	void setMass(double newMass);	//sets mass to new
	void setCharge(double newCharge);	//sets charge to new

	//forces and potentials are implemented using a double buffering system
	inline void addtoforce(const Vector3D& impulse) {newforce += impulse;};	//a force buffer is incremented bit by bit
	inline void addtopotential(double energy) {newpotential += energy;};	//a potential buffer is incremented bit by byt
	void setforcetonew();			//set the force to the buffer and clear the buffer
	void setpotentialtonew();	//set the potential to the buffer and clear the buffer
	
	//these all call the appropriate particle move algorithm with the current force/mass and a given value of dt
	inline void stoppedforce(double dt) {stopped(force/mass, dt);};
	inline void simpleforce(double dt) {simple(force/mass, dt);};
	inline void dampedforce(double dt) {damped(force/mass, dt);};
	inline void velocity_verletforce(double dt) {velocity_verlet(force/mass, dt);};
	inline void beemanforce(double dt) {beeman(force/mass, dt);};

	void setEnergyColor(double emin, double emax);	//set the atom's color based on it's energy and bounding energy values
	void setDefaultColor();	//go back to the atom's default color
	void changeDefaultColor(double r, double g, double b);	//change the atom's default color

	inline int getIdx() const {return indexnum;};	//I don't know if this is useful by itself
	void setIdx(int newindex);	//the index bit is mainly used by the assemblage for bookkeeping

	int addneighbor(Atom* newneighbor);	//add a new atom as a neighbor
	void clearneighbors();	//clear the neighbor list 

protected:
	int neighbors;
	Atom* neighborlist[MAXNEIGHBORS];

private:
	int type;
	Vector3D force;
	Vector3D newforce;
	double potential;
	double newpotential;
	double mass;
	double charge;
	double defaultcolor[4];
	int indexnum;

	double drawemax;
	double drawemin;
	bool energydraw;
};

#endif
