/*
 *	GAST-Viz
 *	version 1.0.0 (04 January 2004)
 *
 *	surfaces.h
 *	HEADER
 *		Classes for walls and surfaces that
 *	interact with atoms
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

#ifndef _surfaces_h_
#define _surfaces_h_

#include "glow.h"

GLOW_NAMESPACE_USING

#include "vectors.h"

/*===============================================
	class Surface

	A Surface is an abstract class from which
	I derive Walls and may derive other such
	things
===============================================*/
class Surface :
	public GlowComponent
{
public:
	inline Surface() {visible = true;};
	inline Surface(const Surface& other):GlowComponent((GlowComponent)other) {visible = other.visible;};
	virtual ~Surface() {};

	inline bool isvisible() const {return visible;};		//should the surface be drawn?
	inline bool isvisible(bool newVisibility) {visible = newVisibility; return visible;};	//change whether it should be drawn

	virtual Vector3D pathtopoint(const Vector3D& point) const = 0;	//returns the displacement vector from the surface to the point

protected:
	bool visible;
};

/*===============================================
	class Plane

	Planes are big stationary, flat things that
	can interact with Atoms (they are infinitely
	large). The most significant bit is that
	they don't need to use the 
	Vector::inParallelipiped method which can
	be a real system hog (I think).
===============================================*/
class Plane :
	public Surface
{
public:
	Plane(const Vector3D& point, const Vector3D& direction1, const Vector3D& direction2);	//construct a plane that contains point and is parallel to direction1 and direction2
	Plane(const Vector3D& point, const Vector3D& normal);	//construct a plane that contains point and is perpendicular to normal
	inline virtual ~Plane() {};
	inline virtual Vector3D pathtopoint(const Vector3D& point) const {return normal*(point.dot(normal)-origindotnormal);};	//returns the shortest displacement vector from the surface to the point

protected:
	virtual void OnEndPaint();	//draw the surface if it is visible

private:
	Vector3D origin;
	Vector3D v1;
	Vector3D v2;

	Vector3D normal;		//these don't vary during runtime so we can use
	double origindotnormal;	//memory and save processor by setting them now
};

/*===============================================
	class RectangleSurface

	A RectangleSurface is a finite rectangular
	surface. Calculations are reasonably fast
	because the calculations are greatly
	simplified when dealing with perfectly
	rectangular surfaces. A little more
	system intensive than a Plane, but not by
	very much.
===============================================*/

class RectangleSurface :
	public Surface
{
public:
	RectangleSurface(const Vector3D& origin, const Vector3D& side1, const Vector3D& side2);	//origin is one corner of the rectangle, side1 and side2 are the side vectors of the rectangle (if they are not perpendicular, they will be adjusted)
	inline virtual ~RectangleSurface() {};
	virtual Vector3D pathtopoint(const Vector3D& point) const;	//returns the shortest displacement vector from the surface to the point

protected:
	virtual void OnEndPaint();

private:
	Vector3D O;
	Vector3D A;
	Vector3D ApO;
	Vector3D B;
	Vector3D BpO;
	Vector3D C;
	Vector3D CpO;
	Vector3D normal;
};

#endif
