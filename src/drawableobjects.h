/*
 *	GAST-Viz
 *	version 1.0.0 (04 January 2004)
 *
 *	drawableobjects.h
 *	HEADER
 *		GLOW componenet base classes to be used for deriving
 *		classes that are visible objects
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

#ifndef _drawableobjects_h_
#define _drawableobjects_h_

#include "glow.h"

GLOW_NAMESPACE_USING

#include "vectors.h"

/*===============================================
	class SphereComponent

	The SphereComponent class serves a very
	simple purpose. It exists to create derived
	classes that can draw themselves as spheres.
	It is derived from GlowComponent so that it
	can be drawn under the Glow hierarchy.
===============================================*/
class SphereComponent :
	public GlowComponent
{
public:
	//constructors
	SphereComponent(const SphereComponent& other);	//makes a new sphere just like other
	SphereComponent();	//makes a new sphere with some default color, size and position information
	SphereComponent(const Vector3D& position);	//makes a new sphere with specified postion
	SphereComponent(const Vector3D& position, double radius);	//makes a new sphere with specified postion and size
	SphereComponent(const Vector3D& position, double radius, double r, double g, double b);	//makes a new sphere with specified position, size and color
	//please note that spheres are not initialized into the Glow hierarchy by default and this must be done elsewhere
	//Assemblages will initialize atoms (sphere superclass) when they are "added"

private:
	bool quadricInit();
public:

	//destructor
	~SphereComponent();

	//don't let things break by copying pointers
	SphereComponent& operator=(const SphereComponent& other);

	//access information
	inline Vector3D& getPosition() const {return *position;};
	inline double getRadius() const {return radius;};

	//move or resize the sphere
	void translate(const Vector3D& displacement);	//shift position by displacement
	void setPosition(const Vector3D& newPos);		//move position to newPos
	void setRadius(double newRadius);

	//change the color of the sphere
	void setAmbient(double r, double g, double b);
	void setDiffuse(double r, double g, double b);
	void setSpecular(double r, double g, double b);
	void setColor(double r, double g, double b);
	void setAlpha(double a);

	//these static functions determine what detail level all sphere components are drawn with, spheres are drawn with a call to glutSolidSphere(radius, slices, stacks), use their documentation if you need more
	inline static int sphereSlices() {return slices;};	//if you need to know the current
	inline static int sphereStacks() {return stacks;};	//detail level, use these
	inline static int sphereSlices(int newSlices) {DLready = false; ::glDeleteLists(DLid, 1); return (slices = newSlices);};	//to change slices...
	inline static int sphereStacks(int newStacks) {DLready = false; ::glDeleteLists(DLid, 1); return (stacks = newStacks);};	//to change stacks...
	inline static void sphereDetail(int newSlices, int newStacks) {slices = newSlices; stacks = newStacks; DLready = false; ::glDeleteLists(DLid, 1);};	//to change slices and stacks...

	//visibility
	inline bool visibility() const {return visible;};	//is this sphere to be drawn
	inline bool visibility(bool newVis) {return (visible = newVis);};	//do you want this sphere to be drawn

protected:
	virtual void OnEndPaint();	//this draws the sphere
	Vector3D* position;	//it's easier to derive from if position is protected and not private (ie. it saves me unnecessary work)

private:
	double radius;	//yup, radius

	float ambient[4];	//color information
	float diffuse[4];
	float specular[4];

	static int slices;	//detail level information
	static int stacks;
	static GLUquadricObj *SphereQuadric;
	static bool quadricInitDone;
	static GLuint DLid;	//this is a display list id number, we use display lists because spheres are slow to draw
	static bool DLready;	//this tells us if the display list is ready or if we need to compile it

	bool visible;	//visibility
};

#endif
