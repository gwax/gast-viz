/*
 *	GAST-Viz
 *	version 1.0.0 (04 January 2004)
 *
 *	assemblage.cpp
 *	SOURCE
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

#include <GL/glut.h>
#include <algorithm>
#include "assemblage.h"
#include "random.h"
#include "functions.inl.h"

/*===============================================
	some constants
===============================================*/
const int RESETVIEW_ITEM = 0;	//these are all used by the right click menus in the assemblage subwindow
const int QUIT_ITEM = 1;
const int FOUR_TWO_ITEM = 10;
const int SIX_THREE_ITEM = 11;
const int EIGHT_FOUR_ITEM = 12;
const int ELEVEN_SEVEN_ITEM = 13;
const int SIXTEEN_TEN_ITEM = 14;
const int THIRTYTWO_SIXTEEN_ITEM = 15;
const int SIXTYFOUR_THIRTYTWO_ITEM = 16;


/*===============================================
	class Assemblage
===============================================*/
double Assemblage::dt = 0.01;	//static variables need init values
int Assemblage::EQsteps = 25;
int Assemblage::numneighborcalcsteps = 15;

/*-----------------------------------------------
	Constructors
-----------------------------------------------*/
Assemblage::Assemblage()
{
	minimum = new Vector3D;	//set default values
	maximum = new Vector3D;
	dynamicsize = true;	//use dynamic size calculation
	drawboundaries = false;	//don't draw boundaries

	currentT = 0.0;
	optimalT = 0.0;	//we don't want to force any temperature by default
	atEQ = true;	//an empty assemblage is always at equilibrium
	atEQstep = 0;
	maxTchange = 0.1;

	jiggledist = 0.0;	//by default we don't jiggle anywhere
	jigglemask[0] = true;	//but all directions are allowed
	jigglemask[1] = true;
	jigglemask[2] = true;
	
	cutoffsq = 10000.0;	//and our cutoff is essentially infinite
	atneighborcalcstep = 0;	//and we haven't done the whole neighbor calc thing

	//allocate everything
	atomlist = new Atom*[DEFAULT_ATOM_ALLOC_NUMBER];
	currentallocated = DEFAULT_ATOM_ALLOC_NUMBER;
	numatoms = 0;
	
	surflist = new Surface*[DEFAULT_ALLOC_NUMBER];
	numsurfs = 0;

	onebodyinteracts = new Interaction1B*[DEFAULT_ALLOC_NUMBER];
	onebodies = 0;

	twobodyinteracts = new Interaction2B*[DEFAULT_ALLOC_NUMBER];
	twobodies = 0;

	threebodyinteracts = new Interaction3B*[DEFAULT_ALLOC_NUMBER];
	threebodies = 0;

	oneandwallinteracts = new InteractionS*[DEFAULT_ALLOC_NUMBER];
	oneandwalls = 0;
}

Assemblage::Assemblage(const Vector3D& mini, const Vector3D& maxi)
{
	if(mini.getX() > maxi.getX() || mini.getY() > maxi.getY() || mini.getZ() > maxi.getZ())	//if the given extremal vectors are bad
	{
		minimum = new Vector3D;	//use defaults
		maximum = new Vector3D;
		dynamicsize = true;		//and set dynamic sizing true
	}
	else
	{
		minimum = new Vector3D(mini);	//use specified extremal values
		maximum = new Vector3D(maxi);
		dynamicsize = false;	//don't dynamically resize
	}
	drawboundaries = true;	//draw the boundaries

	currentT = 0.0;
	optimalT = 0.0;	//we don't want to force any temperature by default
	atEQ = true;	//an empty assemblage is always at equilibrium
	atEQstep = 0;
	maxTchange = 0.1;

	jiggledist = 0.0;	//by default we don't jiggle anywhere
	jigglemask[0] = true;	//but all directions are allowed
	jigglemask[1] = true;
	jigglemask[2] = true;
	
	cutoffsq = 10000.0;	//and our cutoff is essentially infinite
	atneighborcalcstep = 0;	//and we haven't done the whole neighbor calc thing

	//and now we allocate everything
	atomlist = new Atom*[DEFAULT_ATOM_ALLOC_NUMBER];
	currentallocated = DEFAULT_ATOM_ALLOC_NUMBER;
	numatoms = 0;
	
	surflist = new Surface*[DEFAULT_ALLOC_NUMBER];
	numsurfs = 0;

	onebodyinteracts = new Interaction1B*[DEFAULT_ALLOC_NUMBER];
	onebodies = 0;

	twobodyinteracts = new Interaction2B*[DEFAULT_ALLOC_NUMBER];
	twobodies = 0;

	threebodyinteracts = new Interaction3B*[DEFAULT_ALLOC_NUMBER];
	threebodies = 0;

	oneandwallinteracts = new InteractionS*[DEFAULT_ALLOC_NUMBER];
	oneandwalls = 0;
}


/*-----------------------------------------------
	Destructor
-----------------------------------------------*/
Assemblage::~Assemblage()
{
	removeAllAtoms();	//get rid of all atoms
	delete minimum;		//delete everything
	delete maximum;
	delete [] atomlist;
	delete [] onebodyinteracts;
	delete [] twobodyinteracts;
	delete [] threebodyinteracts;
	delete [] oneandwallinteracts;
	delete [] surflist;
}


/*-----------------------------------------------
	Assemblage methods
-----------------------------------------------*/
int Assemblage::addAtom(Atom* newAtom)
{
	if(newAtom->getIdx() != -1)	/*if the index isn't -1, then the atom is already in an assemblage*/
		return -1;

	atEQ = false;	//adding an atom means that we're not at equilibrium anymore
	atEQstep = 0;

	if(numatoms == currentallocated)	//do we need to allocate more memory
	{	//then do so
		Atom** temp;
		Atom** oldatomlist;
		int i;
		temp = new Atom*[currentallocated+DEFAULT_ATOM_ALLOC_NUMBER];
		for(i=0; i<currentallocated; i++)
		{
			temp[i] = atomlist[i];
		}
		oldatomlist = atomlist;
		atomlist = temp;
		delete oldatomlist;
		currentallocated += DEFAULT_ATOM_ALLOC_NUMBER;
	}

	atomlist[numatoms] = newAtom;	//add the atom to the list
	newAtom->indexnum = numatoms;	//set the atom's index number
	newAtom->Init(this);	//initialize the atom into the Glow hierarchy with the assemblage as it's parent
	return ++numatoms;	//we've added another atom, note this and return the currently existing number of atoms
}

int Assemblage::removeAnAtom(Atom* oldAtom)
{
	int i = oldAtom->indexnum;
	if(atomlist[i] != oldAtom)	/*the atom is in a different assemblage*/
		return -1;	//that's weird so we'll

	atEQ = false;	//removing an atom means we're not at equilibrium
	atEQstep = 0;

	oldAtom->indexnum = -1;	//get rid of it's index number
	oldAtom->Close();	//get rid of the atom

	numatoms--;	//we have fewer atoms now

	if(i != numatoms)	/*compact the atomlist down so we have no holes*/
	{
		if(numatoms>0)	//if there are no more atoms it's going to be pretty compact already
		{
			atomlist[i] = atomlist[numatoms];	//move the last atom to this space
			atomlist[i]->indexnum = i;	//set it's index number appropriately
			atomlist[numatoms] = NULL;	//get rid of the pointer in the old position
		}
		return i;	//must mean something
	}
	return (i-1);	//I don't remember what this is supposed to mean as a return value
}

int Assemblage::removeAtom(Atom* oldAtom)
{
	int temp = removeAnAtom(oldAtom);	//remove an atom
	calculateneighbors();	//recalculate neighbors
	return temp;
}

int Assemblage::removeAllAtoms()
{
	int temp = numatoms;
	for(int i=numatoms-1; i>=0; i--)	//go through one by one
	{
		removeAnAtom(getAtom(i));	//and get rid of all the atoms
	}
	numatoms = 0; //no more atoms

	delete [] atomlist;	//delete the atomlist
	atomlist = new Atom*[DEFAULT_ATOM_ALLOC_NUMBER];	//and reallocate it nice and small like
	currentallocated = DEFAULT_ATOM_ALLOC_NUMBER;

	calculateneighbors();	//this is probably pretty pointless considering there are no atoms to have neighbors, but I put it in for principles sake

	return temp;	//how many atoms did we remove
}

int Assemblage::addSurface(Surface* newSurface)
{
	atEQ = false;	//adding a new surface means no more equilibrium
	atEQstep = 0;

	if(numsurfs%DEFAULT_ALLOC_NUMBER == 0 && numsurfs != 0)	//if we need to allocate more space
	{	//allocate more space
		Surface** temp;
		int i;
		temp = new Surface*[numsurfs+DEFAULT_ALLOC_NUMBER];
		for(i=0; i<numsurfs; i++)
		{
			temp[i] = surflist[i];
		}
		delete surflist;
		surflist = temp;
	}

	surflist[numsurfs] = newSurface;	//add the surface
	newSurface->Init(this);	//initialize the surface into the glow hierarchy
	return ++numsurfs;
}

void Assemblage::setSize(const Vector3D& mini, const Vector3D& maxi)
{
	delete minimum;
	delete maximum;

	if(mini.getX() > maxi.getX() || mini.getY() > maxi.getY() || mini.getZ() > maxi.getZ())	//check that the values are reasonable
	{
		minimum = new Vector3D;	//if not, set defaults
		maximum = new Vector3D;
		dynamicsize = true;	//dynamic sizing on
	}
	else
	{
		minimum = new Vector3D(mini);	//use the new extremal vectors
		maximum = new Vector3D(maxi);
		dynamicsize = false;	//dynamic sizing off
	}
}

double Assemblage::setdt(double newdt)
{
	if(newdt <= 0.0)	//dt must be positive
		return dt;	//if not, don't change
	else
	{
		dt = newdt;	//if it's positive they must know what they're doing
		return dt;
	}
}

int Assemblage::setEQsteps(int newEQsteps)
{
	if(newEQsteps <= 0)	//it has to take at least on time step to get to equilibrium
		return EQsteps;	//if not, don't change
	else
	{
		EQsteps = newEQsteps;	//if it's more than one it should be good enough
		return EQsteps;
	}
}

double Assemblage::setjiggle(double newjiggle)
{
	if(newjiggle < 0.0)	//can't jiggle negative-like
		return jiggledist;
	else
	{
		jiggledist = newjiggle;	//set the new jiggle distance
		return jiggledist;
	}
}

void Assemblage::setjigglemask(bool x, bool y, bool z)
{
	jigglemask[0] = x;	//set the new jiggle mask
	jigglemask[1] = y;
	jigglemask[2] = z;
}

void Assemblage::stoppedall()
{	//dummy movement type, remember
}

void Assemblage::dampedall()
{
	int i;
	double totalKE = 0.0;
	for(i=0; i<numatoms; i++)	//for each
	{
		atomlist[i]->dampedforce(dt);	//move the atom
		totalKE += 0.5*atomlist[i]->getMass()*atomlist[i]->getVelocity().magnitudesq();	//increment our KE counter
	}
	currentT = totalKE/numatoms;	//temperature is more a measure of average KE

	if(!atEQ)	//were we already at EQ
	{
		if(atEQstep < EQsteps)	//not hit EQ?
			atEQstep++;	//tick off another step
		else
		{
			atEQ = true;	//we hit EQ
			atEQstep = 0;
		}
	}

	if(optimalT > 0.0 && currentT > 0.0 && atEQ)	//we only do temperature adjustments if we're adjusting to > 0, at > 0 and at EQ
	{
		double velscalefactor = mymax(1.0-maxTchange, mymin(1.0+maxTchange, sqrt(optimalT/currentT)));	//how much do we scale by
		for(i=0; i<numatoms; i++)	//for each
		{
			atomlist[i]->scaleVelocity(velscalefactor);	//scale the velocity
		}
	}

	if(jiggledist > 0.0)	//are we jiggling
	{
		for(i=0; i<numatoms; i++)	//for each
		{
			atomlist[i]->translate(Random::randdirvect(jigglemask[0], jigglemask[1], jigglemask[2])*jiggledist*dt);	//jiggle
		}
	}
}

void Assemblage::simpleall()
{
	int i;
	double totalKE = 0.0;
	for(i=0; i<numatoms; i++)	//for each
	{
		atomlist[i]->simpleforce(dt);	//move the atom
		totalKE += 0.5*atomlist[i]->getMass()*atomlist[i]->getVelocity().magnitudesq();	//increment our KE counter
	}
	currentT = totalKE/numatoms;	//temperature is more a measure of average KE

	if(!atEQ)	//were we already at EQ
	{
		if(atEQstep < EQsteps)	//not hit EQ?
			atEQstep++;	//tick off another step
		else
		{
			atEQ = true;	//we hit EQ
			atEQstep = 0;
		}
	}

	if(optimalT > 0.0 && currentT > 0.0 && atEQ)	//we only do temperature adjustments if we're adjusting to > 0, at > 0 and at EQ
	{
		double velscalefactor = mymax(1.0-maxTchange, mymin(1.0+maxTchange, sqrt(optimalT/currentT)));	//how much do we scale by
		for(i=0; i<numatoms; i++)
		{
			atomlist[i]->scaleVelocity(velscalefactor);	//scale the velocity

		}
	}

	if(jiggledist > 0.0)	//are we jiggling
	{
		for(i=0; i<numatoms; i++)	//for each
		{
			atomlist[i]->translate(Random::randdirvect(jigglemask[0], jigglemask[1], jigglemask[2])*jiggledist*dt);	//jiggle
		}
	}
}

void Assemblage::velocity_verletall()
{
	int i;
	double totalKE = 0.0;
	for(i=0; i<numatoms; i++)	//for each
	{
		atomlist[i]->velocity_verletforce(dt);	//move the atom
		totalKE += 0.5*atomlist[i]->getMass()*atomlist[i]->getVelocity().magnitudesq();	//increment our KE counter
	}
	currentT = totalKE/numatoms;	//temperature is more a measure of average KE

	if(!atEQ)	//were we already at EQ
	{
		if(atEQstep < EQsteps)	//not hit EQ?
			atEQstep++;	//tick off another step
		else
		{
			atEQ = true;	//we hit EQ
			atEQstep = 0;
		}
	}

	if(optimalT > 0.0 && currentT > 0.0 && atEQ)	//we only do temperature adjustments if we're adjusting to > 0, at > 0 and at EQ
	{
		double velscalefactor = mymax(1.0-maxTchange, mymin(1.0+maxTchange, sqrt(optimalT/currentT)));	//how much do we scale by
		for(i=0; i<numatoms; i++)
		{
			atomlist[i]->scaleVelocity(velscalefactor);	//scale the velocity
		}
	}

	if(jiggledist > 0.0)	//are we jiggling
	{
		for(i=0; i<numatoms; i++)	//for each
		{
			atomlist[i]->translate(Random::randdirvect(jigglemask[0], jigglemask[1], jigglemask[2])*jiggledist*dt);	//jiggle
		}
	}
}

void Assemblage::beemanall()
{
	int i;
	double totalKE = 0.0;
	for(i=0; i<numatoms; i++)	//for each
	{
		atomlist[i]->velocity_verletforce(dt);	//move the atom
		totalKE += 0.5*atomlist[i]->getMass()*atomlist[i]->getVelocity().magnitudesq();	//increment our KE counter
	}
	currentT = totalKE/numatoms;	//temperature is more a measure of average KE

	if(!atEQ)	//were we already at EQ
	{
		if(atEQstep < EQsteps)	//not hit EQ?
			atEQstep++;	//tick off another step
		else
		{
			atEQ = true;	//we hit EQ
			atEQstep = 0;
		}
	}

	if(optimalT > 0.0 && currentT > 0.0 && atEQ)	//we only do temperature adjustments if we're adjusting to > 0, at > 0 and at EQ
	{
		double velscalefactor = mymax(1.0-maxTchange, mymin(1.0+maxTchange, sqrt(optimalT/currentT)));	//how much do we scale by
		for(i=0; i<numatoms; i++)
		{
			atomlist[i]->scaleVelocity(velscalefactor);	//scale the velocity
		}
	}

	if(jiggledist > 0.0)	//are we jiggling
	{
		for(i=0; i<numatoms; i++)	//for each
		{
			atomlist[i]->translate(Random::randdirvect(jigglemask[0], jigglemask[1], jigglemask[2])*jiggledist*dt);	//jiggle
		}
	}
}

void Assemblage::energycolorall(double emin, double emax)
{
	for(int i=0; i<numatoms; i++)	//this is easy, just call the equivalent function for all the individual atoms
	{
		atomlist[i]->setEnergyColor(emin, emax);
	}
}

void Assemblage::nativecolorall()
{
	for(int i=0; i<numatoms; i++)	//this is easy, just call the equivalent function for all the individual atoms
	{
		atomlist[i]->setDefaultColor();
	}
}

double Assemblage::cutoff(double newcutoff)
{
	cutoffsq = newcutoff*newcutoff;	//we store it internally as a square for convenience and speed
	return cutoff();
}

void Assemblage::interactall()
{
	if(atneighborcalcstep <= numneighborcalcsteps)	//haven't reached the time to recalculate neighbors
		atneighborcalcstep++;	//tick off another time step
	else	//otherwise
	{
		atneighborcalcstep = 0;	//reset counter
		calculateneighbors();	//calculate neighbors
	}

	//this may look a little convoluted but it's to avoid excess looping
	int i, j, k, l;
	if(onebodies > 0 || twobodies > 0 || threebodies > 0 || oneandwalls > 0)	//are there any registered interactions
	{
		for(i=0; i<numatoms; i++)	//for all
		{
			if(twobodies > 0 || threebodies > 0)	//are there any interactions registered for multiple atoms
			{
				for(j=0; j<atomlist[i]->neighbors; j++)	//go through the current atoms neighbors
				{
					if(threebodies > 0)	//are there any registered three body interactions
					{
						for(k=0; k<atomlist[i]->neighborlist[j]->neighbors; k++)	//go through all of the neighbor's neighbors
						{
							for(l=0; l<threebodies; l++)	//for each registered three body interaction
							{
								threebodyinteracts[l]->interact(atomlist[i], atomlist[i]->neighborlist[j], atomlist[i]->neighborlist[j]->neighborlist[k]);	//interact the current atom, its current neighbor and its current neighbor's current neighbor
							}
						}
					}
					for(l=0; l<twobodies; l++)	//for each registered two body interaction
					{
						twobodyinteracts[l]->interact(atomlist[i], atomlist[i]->neighborlist[j]);	//interact the current atom and its current neighbor
					}
				}
			}
			for(l=0; l<onebodies; l++)	//for each registered one body interaction
			{
				onebodyinteracts[l]->interact(atomlist[i]);	//interact the current atom
			}

			for(l=0; l<oneandwalls; l++)	//for each registered wall interaction
			{
				for(j=0; j<numsurfs; j++)	//for each surface
				{
					oneandwallinteracts[l]->interact(atomlist[i], surflist[j]);	//interact the current atom with the current surface
				}
			}

			atomlist[i]->setforcetonew();		//deal with atom force buffering
			atomlist[i]->setpotentialtonew();	//deal with atom potential buffering
		}
	}
}

void Assemblage::calculateneighbors()
{
	int i, j;
	for(i=0; i<numatoms; i++)	//for each atom
	{
		atomlist[i]->clearneighbors();	//clear the neighbor list
		for(j=i+1; j<numatoms; j++)	//for each atom higher in the index
			if((atomlist[i]->getPosition()-atomlist[j]->getPosition()).magnitudesq() <= cutoffsq)	//if the distance between the atoms is less than the cutoff
				atomlist[i]->addneighbor(atomlist[j]);	//they are neighbors
	}
}

int Assemblage::neighborcalcsteps(int newneighborcalcsteps)
{
	if(newneighborcalcsteps < 0)	//can't calculate neighbors more frequently than every time step
		return numneighborcalcsteps;	//so fail to change
	else
		return (numneighborcalcsteps = newneighborcalcsteps);	//otherwise change
}

int Assemblage::addInteraction(Interaction1B* newonebody)	//interactions may never be removed, but they may be disabled
{
	if(onebodies%DEFAULT_ALLOC_NUMBER == 0 && onebodies != 0)	//allocate more if we need to
	{
		Interaction1B** temp;
		int i;
		temp = new Interaction1B*[onebodies+DEFAULT_ALLOC_NUMBER];
		for(i=0; i<=onebodies; i++)
		{
			temp[i] = onebodyinteracts[i];
		}
		delete onebodyinteracts;
		onebodyinteracts = temp;
	}

	onebodyinteracts[onebodies] = newonebody;	//add the interaction
	return ++onebodies;
}

int Assemblage::addInteraction(Interaction2B* newtwobody)	//interactions may never be removed, but they may be disabled
{
	if(twobodies%DEFAULT_ALLOC_NUMBER == 0 && twobodies != 0)	//allocate more if we need to
	{
		Interaction2B** temp;
		int i;
		temp = new Interaction2B*[twobodies+DEFAULT_ALLOC_NUMBER];
		for(i=0; i<=twobodies; i++)
		{
			temp[i] = twobodyinteracts[i];
		}
		delete twobodyinteracts;
		twobodyinteracts = temp;
	}

	twobodyinteracts[twobodies] = newtwobody;	//add the interaction
	return ++twobodies;
}

int Assemblage::addInteraction(Interaction3B* newthreebody)	//interactions may never be removed, but they may be disabled
{
	if(threebodies%DEFAULT_ALLOC_NUMBER == 0 && threebodies != 0)	//allocate more if we need to
	{
		Interaction3B** temp;
		int i;
		temp = new Interaction3B*[threebodies+DEFAULT_ALLOC_NUMBER];
		for(i=0; i<=threebodies; i++)
		{
			temp[i] = threebodyinteracts[i];
		}
		delete threebodyinteracts;
		threebodyinteracts = temp;
	}

	threebodyinteracts[threebodies] = newthreebody;	//add the interaction
	return ++threebodies;
}

int Assemblage::addInteraction(InteractionS* newoneandwall)	//interactions may never be removed, but they may be disabled
{
	if(oneandwalls%DEFAULT_ALLOC_NUMBER == 0 && oneandwalls != 0)	//allocate more if we need to
	{
		InteractionS** temp;
		int i;
		temp = new InteractionS*[oneandwalls+DEFAULT_ALLOC_NUMBER];
		for(i=0; i<oneandwalls; i++)
		{
			temp[i] = oneandwallinteracts[i];
		}
		delete oneandwallinteracts;
		oneandwallinteracts = temp;
	}

	oneandwallinteracts[oneandwalls] = newoneandwall;	//add the interaction
	return ++oneandwalls;
}


/*-----------------------------------------------
	On*Paint() draws the assemblage
-----------------------------------------------*/
bool Assemblage::OnBeginPaint()
{
	if(dynamicsize == true)	//It's much better to fix the size (at least until I find a better way to do this)
	{
		for(int i=0; i<numatoms; i++)
		{
			*minimum = minvect(*minimum, atomlist[i]->getPosition()-Vector3D(atomlist[i]->getRadius()));
			*maximum = maxvect(*maximum, atomlist[i]->getPosition()+Vector3D(atomlist[i]->getRadius()));
		}
	}

	glPushMatrix();

	Vector3D centervect = avgvect(*maximum, *minimum);

	Vector3D scalevect = maxvect(*maximum-centervect, centervect-*minimum);

	double scalefactor = 1.5/mymax(scalevect.getX(), mymax(scalevect.getY(), scalevect.getZ()));

	::glScaled(scalefactor, scalefactor, scalefactor);	//stretch to take up space

	::glTranslated(-centervect.getX(), -centervect.getY(), -centervect.getZ());	//move so that we're centered

	GLfloat light0_ambient[] =  {0.6f, 0.8f, 0.8f, 1.0f};	//add a light
	GLfloat light0_diffuse[] =  {.9f, .9f, 1.0f, 1.0f};
	GLfloat light0_position[] = {10.8f, 12.9f, 2.0f, 1.0f};

	::glEnable(GL_LIGHT0);
	::glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	::glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	::glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	return true;	//done with preparing for children to be drawn
}

void Assemblage::OnEndPaint()
{
	static float diffuse[4] = {1.0, 1.0, 1.0, 1.0};	//material coloring information
	static float specular[4] = {1.0, 1.0, 1.0, 1.0};
	static float ambient[4] = {0.37645, 0.37645, 0.37645, 1.0};

	if(drawboundaries)	//if we're drawing the boundaries
	{
		::glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);	//set up the material
		::glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
		::glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);

		::glBegin(GL_LINE_STRIP);	//one side
			::glVertex3d(minimum->getX(), minimum->getY(), minimum->getZ());
			::glVertex3d(minimum->getX(), maximum->getY(), minimum->getZ());
			::glVertex3d(maximum->getX(), maximum->getY(), minimum->getZ());
			::glVertex3d(maximum->getX(), minimum->getY(), minimum->getZ());
			::glVertex3d(minimum->getX(), minimum->getY(), minimum->getZ());
		::glEnd();

		::glBegin(GL_LINE_STRIP);	//the other side
			::glVertex3d(minimum->getX(), minimum->getY(), maximum->getZ());
			::glVertex3d(minimum->getX(), maximum->getY(), maximum->getZ());
			::glVertex3d(maximum->getX(), maximum->getY(), maximum->getZ());
			::glVertex3d(maximum->getX(), minimum->getY(), maximum->getZ());
			::glVertex3d(minimum->getX(), minimum->getY(), maximum->getZ());
		::glEnd();

		::glBegin(GL_LINES);	//the missing edges
			::glVertex3d(minimum->getX(), minimum->getY(), minimum->getZ());
			::glVertex3d(minimum->getX(), minimum->getY(), maximum->getZ());

			::glVertex3d(minimum->getX(), maximum->getY(), minimum->getZ());
			::glVertex3d(minimum->getX(), maximum->getY(), maximum->getZ());

			::glVertex3d(maximum->getX(), maximum->getY(), minimum->getZ());
			::glVertex3d(maximum->getX(), maximum->getY(), maximum->getZ());

			::glVertex3d(maximum->getX(), minimum->getY(), minimum->getZ());
			::glVertex3d(maximum->getX(), minimum->getY(), maximum->getZ());
		::glEnd();
	}

	::glPopMatrix();
}


/*===============================================
	class AssemblageSubwindow
===============================================*/
/*-----------------------------------------------
	Constructors
-----------------------------------------------*/
AssemblageSubwindow::AssemblageSubwindow(GlowComponent* parent, int size, int ypos, Assemblage* assem, bool stereo)
{
	if(stereo)
		GlowSubwindow::Init(parent, 0, ypos, size, size, Glow::rgbaBuffer |
				Glow::depthBuffer | Glow::doubleBuffer | Glow::stereoBuffer,
				Glow::mouseEvents | Glow::dragEvents | Glow::menuEvents);
	else
		GlowSubwindow::Init(parent, 0, ypos, size, size, Glow::rgbaBuffer |
				Glow::depthBuffer | Glow::doubleBuffer, Glow::mouseEvents |
				Glow::dragEvents | Glow::menuEvents);

	SetAutoSwapBuffersEnabled(false);

	_manip = new GlowViewManipulator(this, GlowViewManipulatorParams::defaults);
	_manip->SetSpinnable(true);

	thingstuff = assem;
	thingstuff->Init(_manip);

	GlowMenu* menu = new GlowMenu;
	menu->AddEntry("Reset View", RESETVIEW_ITEM);
	menu->AddEntry("Quit", QUIT_ITEM);

	GlowMenu* detailMenu = new GlowMenu;
	detailMenu->AddEntry("4x2", FOUR_TWO_ITEM);
	detailMenu->AddEntry("6x3", SIX_THREE_ITEM);
	detailMenu->AddEntry("8x4", EIGHT_FOUR_ITEM);
	detailMenu->AddEntry("11x7 - Default", ELEVEN_SEVEN_ITEM);
	detailMenu->AddEntry("16x10", SIXTEEN_TEN_ITEM);
	detailMenu->AddEntry("32x16", THIRTYTWO_SIXTEEN_ITEM);
	detailMenu->AddEntry("64x32", SIXTYFOUR_THIRTYTWO_ITEM);

	menu->AddSubmenu("Sphere Detail", detailMenu);
	detailMenu->SetItemMark(3, "<>");

	SetMenu(Glow::rightButton, menu);

	menu->Notifier().Bind(this);
	detailMenu->Notifier().Bind(this);

	movetype = AssemblageSubwindow::moveStopped;

	canspin = true;
	allowalpha = true;
	stereographic = stereo;
	left = true;
}


/*-----------------------------------------------
	AssemblageSubwindow methods
-----------------------------------------------*/
void AssemblageSubwindow::OnMouseDown(Glow::MouseButton button, int x, int y, Glow::Modifiers modifiers)
{
	if(button == Glow::leftButton && canspin)
	{
		if (_manip->GetState() == GlowViewManipulator::idleState)
		{
			GLfloat xn, yn;
			NormalizeCoordinates(x, y, xn, yn);
			_manip->BeginRotationDrag(xn, yn);
		}
	}
}

void AssemblageSubwindow::OnMouseUp(Glow::MouseButton button, int x, int y, Glow::Modifiers modifiers)
{
	if(button == Glow::leftButton && canspin)
	{
		if (_manip->GetState() == GlowViewManipulator::rotatingState)
		{
			GLfloat xn, yn;
			NormalizeCoordinates(x, y, xn, yn);
			_manip->EndDrag(xn, yn);
		}
	}
}

void AssemblageSubwindow::OnMouseDrag(int x, int y)
{
	if (_manip->GetState() == GlowViewManipulator::rotatingState)
	{
		GLfloat xn, yn;
		NormalizeCoordinates(x, y, xn, yn);
		_manip->InDrag(xn, yn);
	}
}

void AssemblageSubwindow::OnMessage(const GlowMenuMessage& message)
{
	switch(message.code)
	{
	case RESETVIEW_ITEM:
		_manip->BeginRotationDrag(0.0, 0.0);
		_manip->EndDrag(0.0, 0.0);
		Quatf* identityquatf;
		identityquatf = new Quatf();
		_manip->SetRotation(*identityquatf);
		delete identityquatf;
		break;
	case QUIT_ITEM:
		exit(0);
		break;
	case FOUR_TWO_ITEM:
		SphereComponent::sphereDetail(4, 2);
		message.menu->UnmarkAllItems();
		message.menu->SetItemMark(0, "<>");
		Refresh();
		break;
	case SIX_THREE_ITEM:
		SphereComponent::sphereDetail(6, 3);
		message.menu->UnmarkAllItems();
		message.menu->SetItemMark(1, "<>");
		Refresh();
		break;
	case EIGHT_FOUR_ITEM:
		SphereComponent::sphereDetail(8, 4);
		message.menu->UnmarkAllItems();
		message.menu->SetItemMark(2, "<>");
		Refresh();
		break;
	case ELEVEN_SEVEN_ITEM:
		SphereComponent::sphereDetail(11, 7);
		message.menu->UnmarkAllItems();
		message.menu->SetItemMark(3, "<>");
		Refresh();
		break;
	case SIXTEEN_TEN_ITEM:
		SphereComponent::sphereDetail(16, 10);
		message.menu->UnmarkAllItems();
		message.menu->SetItemMark(4, "<>");
		Refresh();
		break;
	case THIRTYTWO_SIXTEEN_ITEM:
		SphereComponent::sphereDetail(32, 16);
		message.menu->UnmarkAllItems();
		message.menu->SetItemMark(5, "<>");
		Refresh();
		break;
	case SIXTYFOUR_THIRTYTWO_ITEM:
		SphereComponent::sphereDetail(64, 32);
		message.menu->UnmarkAllItems();
		message.menu->SetItemMark(6, "<>");
		Refresh();
		break;
	}
}

void AssemblageSubwindow::OnMessage(const GlowIdleMessage& message)
{
	switch(movetype)
	{
	case moveStopped:
		thingstuff->stoppedall();
		break;
	case moveSimple:
		thingstuff->interactall();
		thingstuff->simpleall();
		break;
	case moveDamped:
		thingstuff->interactall();
		thingstuff->dampedall();
		break;
	case moveVelocityVerlet:
		thingstuff->interactall();
		thingstuff->velocity_verletall();
		break;
	case moveBeeman:
		thingstuff->interactall();
		thingstuff->beemanall();
		break;
	}
	
	Refresh();
}


/*-----------------------------------------------
	AssemblageSubwindow OnFooPaint() blah blah
-----------------------------------------------*/
bool AssemblageSubwindow::OnBeginPaint()
{
	if(stereographic)
	{
		if(left)
		{
			::glDrawBuffer(GL_BACK_LEFT);
		}
		else
		{
			::glDrawBuffer(GL_BACK_RIGHT);
		}
	}

	::glClearColor(0.0, 0.0, 0.0, 0.0);	//set clear color
	::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//clear stuff

	::glPushMatrix();

	::glMatrixMode(GL_MODELVIEW);	//set up the matrices
	::glLoadIdentity();
	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();

	if(stereographic)
	{
		if(left)
		{
			::glTranslatef(-0.5, 0.0, 0.0);
		}
		else
		{
			::glTranslatef(0.5, 0.0, 0.0);
		}
	}

	::glFrustum(-0.5, 0.5, -0.5, 0.5, 1.0, 8.0);
	::glMatrixMode(GL_MODELVIEW);
	
	::glEnable(GL_LIGHTING);	//lighting governs colors and such
	::glShadeModel(GL_SMOOTH);	//smooth shading makes it look decent
	::glEnable(GL_LINE_SMOOTH);

	::glEnable(GL_DEPTH_TEST);	//depth testing is pretty important
	::glDepthFunc(GL_LESS);
	
	if(allowalpha)	//if alpha is allowed we turn blending on
	{
		::glEnable(GL_BLEND);
		::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	
	if(canspin)	//the zoom needs to be a little different if we're looking at a block of atoms or a plane of atoms
		::glTranslatef(0.0, 0.0, -5.0);
	else
		::glTranslatef(0.0, 0.0, -4.0);
	
	return true;
}


// End-redraw method

void AssemblageSubwindow::OnEndPaint()
{
	::glPopMatrix();

	::glFlush();	//to prevent things from looking ugly around the subwindow

	if(stereographic)
	{
		if(left)
		{
			left = false;
			this->Refresh();
		}
		else
		{
			left = true;
			Glow::SwapBuffers();
		}
	}
	else
	{
		Glow::SwapBuffers();	//we need to do this explicitly because of the way Glow deals with subwindows
	}
}
