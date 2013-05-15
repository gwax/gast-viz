/*
 *	GAST-Viz
 *	version 1.0.0 (04 January 2004)
 *
 *	drawableobjects.cpp
 *	SOURCE
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

#include <GL/glut.h>
#include "drawableobjects.h"

/*===============================================
	class SphereComponent
===============================================*/
int SphereComponent::slices = 11;	//static variables need init values
int SphereComponent::stacks = 7;
GLUquadricObj *(SphereComponent::SphereQuadric) = NULL;
bool SphereComponent::quadricInitDone = false;
GLuint SphereComponent::DLid = 0;
bool SphereComponent::DLready = false;

/*-----------------------------------------------
	Constructors
-----------------------------------------------*/
SphereComponent::SphereComponent(const SphereComponent& other) :
GlowComponent((GlowComponent)other)
{	//just copies all the attributes of other
	position = new Vector3D(*other.position);	//don't want to have pointer problems
	radius = other.radius;

	ambient[0] = other.ambient[0];
	ambient[1] = other.ambient[1];
	ambient[2] = other.ambient[2];
	ambient[3] = other.ambient[3];

	diffuse[0] = other.diffuse[0];
	diffuse[1] = other.diffuse[1];
	diffuse[2] = other.diffuse[2];
	diffuse[3] = other.diffuse[3];

	specular[0] = other.specular[0];
	specular[1] = other.specular[1];
	specular[2] = other.specular[2];
	specular[3] = other.specular[3];

	visible = other.visible;

	if(!quadricInitDone)
		quadricInitDone = quadricInit();
}

SphereComponent::SphereComponent()
{	//creates a default sphere
	position = new Vector3D;	//use default vector for position
	radius = 1.0;	//default to radius of 1

	ambient[0] = 1.0;	//white
	ambient[1] = 1.0;
	ambient[2] = 1.0;
	ambient[3] = 1.0;

	diffuse[0] = 0.5;
	diffuse[1] = 0.5;
	diffuse[2] = 0.5;
	diffuse[3] = 1.0;

	specular[0] = 0.0;
	specular[1] = 0.0;
	specular[2] = 0.0;
	specular[3] = 1.0;

	visible = true;

	if(!quadricInitDone)
		quadricInitDone = quadricInit();
}

SphereComponent::SphereComponent(const Vector3D& position)
{	//default everything but position
	this->position = new Vector3D(position);
	radius = 1.0;

	ambient[0] = 1.0;
	ambient[1] = 1.0;
	ambient[2] = 1.0;
	ambient[3] = 1.0;

	diffuse[0] = 0.5;
	diffuse[1] = 0.5;
	diffuse[2] = 0.5;
	diffuse[3] = 1.0;

	specular[0] = 0.0;
	specular[1] = 0.0;
	specular[2] = 0.0;
	specular[3] = 1.0;

	visible = true;

	if(!quadricInitDone)
		quadricInitDone = quadricInit();
}

SphereComponent::SphereComponent(const Vector3D& position, double radius)
{	//you get the point
	this->position = new Vector3D(position);
	this->radius = radius;

	ambient[0] = 1.0;
	ambient[1] = 1.0;
	ambient[2] = 1.0;
	ambient[3] = 1.0;

	diffuse[0] = 0.5;
	diffuse[1] = 0.5;
	diffuse[2] = 0.5;
	diffuse[3] = 1.0;

	specular[0] = 0.0;
	specular[1] = 0.0;
	specular[2] = 0.0;
	specular[3] = 1.0;

	visible = true;

	if(!quadricInitDone)
		quadricInitDone = quadricInit();
}

SphereComponent::SphereComponent(const Vector3D& position, double radius, double r, double g, double b)
{	//good enough, eh?
	this->position = new Vector3D(position);
	this->radius = radius;

	ambient[0] = 1.0*r;
	ambient[1] = 1.0*g;
	ambient[2] = 1.0*b;
	ambient[3] = 1.0;

	diffuse[0] = 0.5*r;
	diffuse[1] = 0.5*g;
	diffuse[2] = 0.5*b;
	diffuse[3] = 1.0;

	specular[0] = 0.0;
	specular[1] = 0.0;
	specular[2] = 0.0;
	specular[3] = 1.0;

	visible = true;

	if(!quadricInitDone)
		quadricInitDone = quadricInit();
}

bool SphereComponent::quadricInit()
{
	SphereQuadric = ::gluNewQuadric();
	::gluQuadricNormals(SphereQuadric, GLU_SMOOTH);
	::gluQuadricTexture(SphereQuadric, GLU_FALSE);

	return true;
}


/*-----------------------------------------------
	Destructor
-----------------------------------------------*/
SphereComponent::~SphereComponent()
{
	delete position;	//we need to delete this so we don't waste memory
}


/*-----------------------------------------------
	Functions to modify the sphere
-----------------------------------------------*/
SphereComponent& SphereComponent::operator=(const SphereComponent& other)
{
	GlowComponent::operator=((GlowComponent)other);
	
	*position = *other.position;	//don't want pointer problems
	radius = other.radius;

	ambient[0] = other.ambient[0];
	ambient[1] = other.ambient[1];
	ambient[2] = other.ambient[2];
	ambient[3] = other.ambient[3];

	diffuse[0] = other.diffuse[0];
	diffuse[1] = other.diffuse[1];
	diffuse[2] = other.diffuse[2];
	diffuse[3] = other.diffuse[3];

	specular[0] = other.specular[0];
	specular[1] = other.specular[1];
	specular[2] = other.specular[2];
	specular[3] = other.specular[3];

	visible = other.visible;

	return *this;
}

void SphereComponent::translate(const Vector3D& displacement)
{
	*position += displacement;	//wahoo for Vector3D operators
}

void SphereComponent::setPosition(const Vector3D& newPos)
{
	*position = newPos;
}

void SphereComponent::setRadius(double newRadius)
{
	radius = newRadius;
}

void SphereComponent::setAmbient(double r, double g, double b)
{
	ambient[0] = (float)r;
	ambient[1] = (float)g;
	ambient[2] = (float)b;
}

void SphereComponent::setDiffuse(double r, double g, double b)
{
	diffuse[0] = (float)r;
	diffuse[1] = (float)g;
	diffuse[2] = (float)b;
}

void SphereComponent::setSpecular(double r, double g, double b)
{
	specular[0] = (float)r;
	specular[1] = (float)g;
	specular[2] = (float)b;
}

void SphereComponent::setColor(double r, double g, double b)
{
	ambient[0] = (float)r;
	ambient[1] = (float)g;
	ambient[2] = (float)b;

	diffuse[0] = (float)(0.5*r);
	diffuse[1] = (float)(0.5*g);
	diffuse[2] = (float)(0.5*b);

	specular[0] = 0.0;
	specular[1] = 0.0;
	specular[2] = 0.0;
}

void SphereComponent::setAlpha(double a)
{
	ambient[3] = (float)a;
	diffuse[3] = (float)a;
	specular[3] = (float)a;
}


/*-----------------------------------------------
	OnEndPaint() draws the sphere
-----------------------------------------------*/
void SphereComponent::OnEndPaint()
{
	if(visible)
	{
		::glPushMatrix();

		if(!DLready)
		{
			DLid = ::glGenLists(1);
			::glNewList(DLid, GL_COMPILE);
				::gluSphere(SphereQuadric, 1.0, slices, stacks);
			::glEndList();
			DLready = true;
		}

		::glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);	//set material
		::glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
		::glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);

		::glTranslated(position->getX(), position->getY(), position->getZ());	//move to position

		::glScaled(radius, radius, radius);

		::glCallList(DLid);

		::glPopMatrix();
	}
}
