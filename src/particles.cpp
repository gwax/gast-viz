/*
 *	GAST-Viz
 *	version 1.0.0 (04 January 2004)
 *
 *	particless.cpp
 *	SOURCE
 *		atom classes derived from SphereComponent
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

#include "particles.h"

/*===============================================
	class Particle
===============================================*/


/*-----------------------------------------------
	Constructors
-----------------------------------------------*/
//most attributes are passed on to the base class, only velocity is explicitly set
Particle::Particle(const Particle& other) :
SphereComponent((SphereComponent)other)
{
	velocity = new Vector3D(*other.velocity);
	oldacceleration = new Vector3D(*other.oldacceleration);
}

Particle::Particle()  :
SphereComponent()
{
	velocity = new Vector3D;
	oldacceleration = new Vector3D;
}

Particle::Particle(const Vector3D& position) :
SphereComponent(position)
{
	velocity = new Vector3D;
	oldacceleration = new Vector3D;
}

Particle::Particle(const Vector3D& position, const Vector3D& velocity) :
SphereComponent(position)
{
	this->velocity = new Vector3D(velocity);
	oldacceleration = new Vector3D;
}

Particle::Particle(const Vector3D& position, double radius) :
SphereComponent(position, radius)
{
	velocity = new Vector3D;
	oldacceleration = new Vector3D;
}

Particle::Particle(const Vector3D& position, double radius, const Vector3D& velocity) :
SphereComponent(position, radius)
{
	this->velocity = new Vector3D(velocity);
	oldacceleration = new Vector3D;
}

Particle::Particle(const Vector3D& position, double radius, double r, double g, double b) :
SphereComponent(position, radius, r, g, b)
{
	velocity = new Vector3D;
	oldacceleration = new Vector3D;
}

Particle::Particle(const Vector3D& position, double radius, double r, double g, double b, const Vector3D& velocity) :
SphereComponent(position, radius, r, g, b)
{
	this->velocity = new Vector3D(velocity);
	oldacceleration = new Vector3D;
}


/*-----------------------------------------------
	Destructor
-----------------------------------------------*/
Particle::~Particle()
{
	delete velocity;	//free up our memory when we're done
	delete oldacceleration;
}


/*-----------------------------------------------
	Particle methods
-----------------------------------------------*/
Particle& Particle::operator=(const Particle &other)
{
	SphereComponent::operator=((SphereComponent)other);	//call base class copy constructor
	*velocity = *other.velocity;	//copy values not pointers
	*oldacceleration = *other.oldacceleration;

	return *this;
}

void Particle::impulse(const Vector3D& acceleration)
{
	*velocity += acceleration;	//directly accelerate the particle
}

void Particle::setVelocity(const Vector3D& newVel)
{
	*velocity = newVel;	//change the velocity
}

void Particle::stopped(const Vector3D& acceleration, double dt)
{
	return;	//this is a dummy move algorithm, remember
}

void Particle::simple(const Vector3D& acceleration, double dt)
{
	*position += (*velocity + acceleration*(0.5*dt))*dt;	//simple integration of velocity and acceleration
	*velocity += acceleration*dt;	//the integrate acceleration
}

void Particle::damped(const Vector3D& acceleration, double dt)
{
	*position += (*velocity + acceleration*dt)*(0.5*dt);	//similar to simple except with a damping term that is equal to the velocity
	*velocity += (acceleration-*velocity)*dt;	//again, simple integration - velocity damping
}

void Particle::velocity_verlet(const Vector3D& acceleration, double dt)
{
	Vector3D adt = acceleration*(0.5*dt);

	*velocity += adt;					//this works out
	*position += *velocity*dt+adt*dt;	//to be equivalent
	*velocity += adt;					//to velocity verlet
}

void Particle::beeman(const Vector3D& acceleration, double dt)
{
	*velocity += acceleration*(dt/3.0);												//I don't want to explain it,
	*position += *velocity*dt + (acceleration*4.0 - *oldacceleration)*(dt*dt/6.0);	//but this is the Beeman
	*velocity += (acceleration*5.0 - *oldacceleration)*(dt/6.0);					//algorithm (it's nice and stable)

	*oldacceleration = acceleration;	//we need to remember this so that we can get a_(n-2)
}
