/*
 *	GAST-Viz
 *	version 1.0.0 (04 January 2004)
 *
 *	assemblage.h
 *	HEADER
 *		GLOW component that serves to contain
 *	other objects. Loops through the objects
 *	that it contains and performs operations on
 *	them.
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

#ifndef _assemblage_h_
#define _assemblage_h_

#include "glow.h"
#include "glowViewTransform.h"

GLOW_NAMESPACE_USING

#include "vectors.h"
#include "atoms.h"
#include "interactions.h"
#include "surfaces.h"

/*===============================================
	class Assemblage

	The Assemblage class can be though of as
	a nice big fishtank full of atoms, or
	whatever else you end up filling it with.
===============================================*/
#define DEFAULT_ATOM_ALLOC_NUMBER 50	//size of chunks used to keep track of the number of atom's in the assemblage
#define DEFAULT_ALLOC_NUMBER 10			//size of chunks used to keep track of anything else

class Assemblage :
	public GlowComponent
{
public:
	Assemblage();	//create a default assemblage
	Assemblage(const Vector3D& mini, const Vector3D& maxi);	//create a default assemblage with extremal vectors to indicate size

	~Assemblage();	//destroy the assemblage and all things it contains

	void setSize(const Vector3D& mini, const Vector3D& maxi);	//resize the assemblage using extremal vectors
	inline Vector3D& getMin() {return *minimum;};	//what is the extreme minimum of the assemblage
	inline Vector3D& getMax() {return *maximum;};	//what is the extreme maximum of the assemblage
	inline bool showboundaries() const {return drawboundaries;};	//are the boundaries being drawn
	inline bool showboundaries(bool newState) {return (drawboundaries = newState);};	//should the boundaries be drawn

	int addAtom(Atom* newAtom);	//add an atom to the assemblage and initialize the atom's GlowComponent with the assemblage as its parent
	int removeAtom(Atom* oldAtom);	//calls the internal function for removal of the specific atom and then recalculates neighbors
	int removeAllAtoms();	//removes all atoms
	inline int numAtoms() const {return numatoms;};	//how many atoms currently in the assemblage
	inline Atom* getAtom(int index) {return atomlist[index];};	//access an atom by its index number, usually best in loops
private:
	int removeAnAtom(Atom* oldAtom);	//here's my internal atom removal function
public:

	int addSurface(Surface* newSurface);	//add surfaces to your assemblage (currently not removable)

	//add new interactions to those used in calculations
	int addInteraction(Interaction1B* newonebody);
	int addInteraction(Interaction2B* newtwobody);
	int addInteraction(Interaction3B* newthreebody);
	int addInteraction(InteractionS* newoneandwall);

	static inline double getdt() {return dt;};	//all assemblages use the same value for dt, this tells you what that value is
	static double setdt(double newdt);	// this changes the dt value

	static inline int getEQsteps() {return EQsteps;};	//all assemblages wait the same length of time before claiming to be at equilibrium, this is that number of steps
	static int setEQsteps(int newEQsteps);	//change the length of time waited before claiming equilibrium
	inline bool equilibrated() const {return atEQ;};	//has the requisite time elapsed to be at equilibrium

	inline double getjiggle() const {return jiggledist;};	//how far are we jiggling
	double setjiggle(double newjiggle);		//how far should we be jiggling
	void setjigglemask(bool x, bool y, bool z);	//which directions should we jiggle in

	//move all atoms using the given move algorithm
	void stoppedall();
	void simpleall();
	void dampedall();
	void velocity_verletall();
	void beemanall();

	void energycolorall(double emin, double emax);	//set all atoms to color based on potential energy
	void nativecolorall();	//set all atoms to use their default color

	inline double cutoff() const {return sqrt(cutoffsq);};	//what is the neighbor cutoff
	double cutoff(double newcutoff);	//what should the neighbor cutoff be
	void interactall();	//perform all interactions between all neighbor pairs
	void calculateneighbors();	//figure out which atoms are neighbors
	static inline int neighborcalcsteps() {return numneighborcalcsteps;};	//all assemblages recalculate neighbors at the same interval, this is that interval
	static int neighborcalcsteps(int newneighborcalcsteps);	//change how frequently neighbors are recalculated

	/*please note that the temperature adjustments aren't 100% accurate for time step algorithms
	that only partially integrate velocity (Velocity Verlet, Beeman) but should be close enough*/
	inline double OptimalTemp() const {return optimalT;};	//in case you want to know what the current optimal temp is
	inline double OptimalTemp(double newtemp) {optimalT = newtemp; return optimalT;};	//this is the temp we will adjust to reach (changing up to 1(+-)maxTchange per time step), if optimalT <= 0.0 we don't perform adjustments
	inline double getCurrentTemp() const {return currentT;};	//what is the current system temperature
	inline double maxTempchange() const {return maxTchange;};	//we only allow
	inline double maxTempchange(double fraction) {maxTchange = fraction; return maxTchange;};

protected:
	virtual bool OnBeginPaint();	//drawing stuff
	virtual void OnEndPaint();
	
private:
	static double dt;
	Vector3D* minimum;	/*bottom left back*/
	Vector3D* maximum;	/*top right front*/
	bool dynamicsize;
	bool drawboundaries;

	double currentT;
	double optimalT;
	double maxTchange;
	bool atEQ;
	int atEQstep;
	static int EQsteps;

	double jiggledist;
	bool jigglemask[3];

	Atom** atomlist;
	int numatoms;
	int currentallocated;

	double cutoffsq;
	int atneighborcalcstep;
	static int numneighborcalcsteps;

	Surface** surflist;
	int numsurfs;

	Interaction1B** onebodyinteracts;
	int onebodies;
	Interaction2B** twobodyinteracts;
	int twobodies;
	Interaction3B** threebodyinteracts;
	int threebodies;
	InteractionS** oneandwallinteracts;
	int oneandwalls;
};


/*===============================================
	class AssemblageSubwindow

	The AssemblageSubwindow class will create
	a Glow Subwindow that contains an Assemblage
	and give necessary functions to interact
	with the assemblage.
===============================================*/
class AssemblageSubwindow :
	public GlowSubwindow,
	public GlowMenuReceiver,
	public GlowIdleReceiver
{
public:
	enum MoveType {moveStopped, moveSimple, moveDamped, moveVelocityVerlet, moveBeeman};

	AssemblageSubwindow(GlowComponent* parent, int size, int ypos, Assemblage* assem, bool stereo=false);	//constructor takes a parent, subwindow size, y-position and pointer to an assemblage; stereo=true should enable stereographic display for systems that are capable of it but is an untested feature due to my hardware limitations
	inline MoveType getMoveType() {return movetype;};	//what move algorithm is being used by the contained assemblage
	inline void setMoveType(MoveType newMoveType) {movetype = newMoveType;};	//what move algorithm should be used by the contained assemblage
	inline bool spinnable() {return canspin;};	//can the assemblage be spun using the using the mouse
	inline bool spinnable(bool newstate) {canspin = newstate; return canspin;};	//should the assemblage be able to be spun using the mouse
	inline bool transparency() {return allowalpha;};	//is alpha blending enabled
	inline bool transparency(bool newstate) {allowalpha = newstate; return allowalpha;};	//should alpha blending be enabled

protected:	//these are all Glow hooks and such
	virtual bool OnBeginPaint();
	virtual void OnEndPaint();
	virtual void OnMouseDown(Glow::MouseButton button, int x, int y, Glow::Modifiers modifiers);
	virtual void OnMouseUp(Glow::MouseButton button, int x, int y, Glow::Modifiers modifiers);
	virtual void OnMouseDrag(int x, int y);
	virtual void OnMessage(const GlowMenuMessage& message);
	virtual void OnMessage(const GlowIdleMessage& message);

private:
	GlowViewManipulator* _manip;	//this is used for the rotations
	Assemblage* thingstuff;			//this is the assemblage
	MoveType movetype;	

	bool canspin;
	bool allowalpha;
	bool stereographic;
	bool left;
};

#endif
