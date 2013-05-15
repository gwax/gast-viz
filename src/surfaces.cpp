/*
 *	GAST-Viz
 *	version 1.0.0 (04 January 2004)
 *
 *	surfaces.cpp
 *	SOURCE
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

#include "surfaces.h"


/*===============================================
	class Plane
===============================================*/
/*-----------------------------------------------
	Constructors
-----------------------------------------------*/
Plane::Plane(const Vector3D& point, const Vector3D& direction1, const Vector3D& direction2) :
Surface()
{	//this mostly prevents the creations of "weird" planes, it can be ignored if you don't do things like make planes where direction1 = +-direction2 and their magnitudes are > 0.0
	origin = point;
	v1 = direction1;
	if(direction1.cross(direction2).magnitudesq() == 0.0)
		if(direction1.dot(Vector3D(0.0, 0.0, 1.0)) > direction1.dot(Vector3D(0.0, 1.0, 0.0)))
			v2 = direction2.cross(Vector3D(0.0, 1.0, 0.0));
		else if(direction1.magnitudesq() > 0.0)
			v2 = direction2.cross(Vector3D(0.0, 0.0, 1.0));
		else
		{
			v1 = Vector3D(1.0, 0.0, 0.0);
			v2 = Vector3D(0.0, 1.0, 0.0);
		}
	else
		v2 = direction2;

	normal = v1.cross(v2).normalvector();
	origindotnormal = origin.dot(normal);
}

Plane::Plane(const Vector3D& point, const Vector3D& normal) :
Surface()
{	//again, this just prevents the creation of weird planes such as when normal = 0.0, it also creates arbitrary basis vectors that are parallel to normal
	origin = point;
	this->normal = normal;

	isvisible(false);

	if(normal.cross(Vector3D(0.0, 0.0, 1.0)).magnitudesq() == 0.0)
		v1 = normal.cross(Vector3D(1.0, 0.0, 0.0));
	else if(normal.magnitudesq() > 0.0)
		v1 = normal.cross(Vector3D(0.0, 0.0, 1.0));
	else
	{
		this->normal = Vector3D(1.0, 0.0, 0.0);
		v1 = Vector3D(0.0, 1.0, 0.0);
	}

	v2 = normal.cross(v1);

	v1.normalize();
	v2.normalize();
	this->normal.normalize();
	origindotnormal = origin.dot(this->normal);
}


/*-----------------------------------------------
	On*Paint() draws the specified part of
	the Plane
-----------------------------------------------*/
void Plane::OnEndPaint()
{
	if(!visible)	//if it's not visible, don't draw it
		return;

	::glPushMatrix();

	static float diffuse[4] = {1.0, 1.0, 1.0, 1.0};		//color stuff
	static float specular[4] = {1.0, 1.0, 1.0, 1.0};
	static float ambient[4] = {0.37645, 0.37645, 0.37645, 1.0};

	::glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);		//material stuff
	::glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	::glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);

	::glBegin(GL_LINE_STRIP);	//it's a square!
		::glVertex3d(origin.getX(), origin.getY(), origin.getZ());
		::glVertex3d(origin.getX()+v1.getX(), origin.getY()+v1.getY(), origin.getZ()+v1.getZ());
		::glVertex3d(origin.getX()+v1.getX()+v2.getX(), origin.getY()+v1.getY()+v2.getY(), origin.getZ()+v1.getZ()+v2.getZ());
		::glVertex3d(origin.getX()+v2.getX(), origin.getY()+v2.getY(), origin.getZ()+v2.getZ());
		::glVertex3d(origin.getX(), origin.getY(), origin.getZ());
	::glEnd();

	::glPopMatrix();
}


/*===============================================
	class RectangleSurface
===============================================*/
/*-----------------------------------------------
	Constructors
-----------------------------------------------*/
RectangleSurface::RectangleSurface(const Vector3D& origin, const Vector3D& side1, const Vector3D& side2) :
Surface()
{	//this makes sure that side1 and side2 are perpendicular and non-zero, then is creates the plane using internal coordinates
	O = origin;
	A = side1;
	B = side2;

	if(A.magnitudesq() == 0.0)	//can't have sides of zero length
		A = Vector3D(1.0, 0.0, 0.0);

	if(B.magnitudesq() == 0.0)	//can't have sides of zero length
		B = Vector3D(0.0, 1.0, 0.0);

	normal = B.cross(A);	//this defines the normal direction
	
	if(normal.magnitudesq() == 0.0)	//are A and B colinear
	{
		if(A.getX() == 0.0)	//force non-colinearity
			B = Vector3D(1.0, 0.0, 0.0);
		else
			B = Vector3D(0.0, 1.0, 0.0);

		normal = B.cross(A);	//recalculate the normal direction
	}

	normal.normalize();	//normalize the normal direction vector

	if(B.dot(A) != 0.0)	//are A and B orthogonal?
	{
		B = A.cross(normal) * (B.cross(A).magnitude() / -(A.magnitude()));	//make A and B orthogonal while maintaining the cross product
	}

	//these are utility vectors used to trade memory for processing time
	C = B + A;
	ApO = A + O;	//these are absolute vectors, instead of relative to O
	BpO = B + O;
	CpO = C + O;
}


/*-----------------------------------------------
	RectangleSurface methods
-----------------------------------------------*/
Vector3D RectangleSurface::pathtopoint(const Vector3D& point) const
{	//just trust me on this one, it does work and it's not worth understanding
	Vector3D path(point - O);

	double rdB = path.dot(B);
	double rdA = path.dot(A);
	double Bmagsq = B.magnitudesq();
	double Amagsq = A.magnitudesq();

	if(rdB > Bmagsq)
	{
		if(rdA > Amagsq)
			path = point - CpO;
		else if(rdA < 0.0)
			path = point - BpO;
		else
		{
			path = point - (BpO + A*rdA);
		}
	}
	else if(rdB < 0.0)
	{
		if(rdA > Amagsq)
			path = point - ApO;
		else if(rdA < 0.0)
			path = point - O;
		else
		{
			path = point - (O + A*rdA);
		}
	}
	else
	{
		if(rdA > Amagsq)
		{
			path = point - (ApO + B*rdB);
		}
		else if(rdA < 0.0)
		{
			path = point - (O + B*rdB);
		}
		else
		{
			path = normal*path.dot(normal);
		}
	}

	return path;
}


/*-----------------------------------------------
	RectangleSurface On*Paint()
-----------------------------------------------*/
void RectangleSurface::OnEndPaint()
{
	if(!visible)	//if it's not visible, don't draw it
		return;

	::glPushMatrix();

	static float diffuse[4] = {1.0, 1.0, 1.0, 1.0};		//color stuff
	static float specular[4] = {1.0, 1.0, 1.0, 1.0};
	static float ambient[4] = {0.37645, 0.37645, 0.37645, 1.0};

	::glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);		//material stuff
	::glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	::glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);

	::glBegin(GL_LINE_STRIP);	//it's a square!
		::glVertex3d(O.getX(), O.getY(), O.getZ());
		::glVertex3d(ApO.getX(), ApO.getY(), ApO.getZ());
		::glVertex3d(CpO.getX(), CpO.getY(), CpO.getZ());
		::glVertex3d(BpO.getX(), BpO.getY(), BpO.getZ());
		::glVertex3d(O.getX(), O.getY(), O.getZ());
	::glEnd();

	::glPopMatrix();
}
