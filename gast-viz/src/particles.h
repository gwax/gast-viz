/*
 *	GAST-Viz
 *	version 1.0.0 (04 January 2004)
 *
 *	particles.h
 *	HEADER
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

#ifndef _particles_h_
#define _particles_h_

#include "drawableobjects.h"
#include "vectors.h"

/*===============================================
	class Particle

	Particles are derived from SphereComponent
	and as such will draw themselves as spheres
	but they have the added feature that they
	can move and be accelerated.
===============================================*/
class Particle :
	public SphereComponent
{
public:
	//the constructors are pretty self-explanatory
	Particle(const Particle& other);
	Particle();
	Particle(const Vector3D& position);
	Particle(const Vector3D& position, const Vector3D& velocity);
	Particle(const Vector3D& position, double radius);
	Particle(const Vector3D& position, double radius, const Vector3D& velocity);
	Particle(const Vector3D& position, double radius, double r, double g, double b);
	Particle(const Vector3D& position, double radius, double r, double g, double b, const Vector3D& velocity);

	//the destructor
	~Particle();

	Particle& operator=(const Particle& other);	//set the attributes to the same as other

	inline Vector3D& getVelocity() const {return *velocity;};	//find out what the velocity is

	void impulse(const Vector3D& acceleration);	//apply an impulse to the velocity
	void setVelocity(const Vector3D& newVel);	//change the velocity
	inline void scaleVelocity(double scalefactor) {*velocity *= scalefactor;};	//scales the velocity by a scalefactor (useful for temperature adjustment)

	//these are the various movement algorithms, given an acceleration and timestep they will move the particle
	void stopped(const Vector3D& acceleration, double dt);	//dummy algorithm that does nothing
	void simple(const Vector3D& acceleration, double dt);	//simple integration algorithm
	void damped(const Vector3D& acceleration, double dt);	//similar to simple algorithm but introduces viscous damping
	void velocity_verlet(const Vector3D& acceleration, double dt);	//uses the Velocity Verlet algorithm to integrate (more stable than simple)
	void beeman(const Vector3D& acceleration, double dt);	//uses the Beeman algorithm (similar stability to Velocity Verlet, better energy conservation, not reversable)

private:
	Vector3D* oldacceleration;
	Vector3D* velocity;
};

#endif
