/*
 *	GAST-Viz
 *	version 1.0.0 (04 January 2004)
 *
 *	vectors.h
 *	HEADER
 *		vector class to do math for me
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

#ifndef _vectors_h_
#define _vectors_h_

#include "glow.h"

GLOW_NAMESPACE_USING

#include <math.h>
#include "functions.inl.h"

/*===============================================
	class Vector3D

	Vector3Ds are exactly what they sound like.
	I didn't want to bother dealing with a
	vector class made by someone else, so I
	made my own. Also, overloading is pretty
	sweet.
===============================================*/
class Vector3D
{
public:
	//constructors
	Vector3D();	//creates a new vector (all elements are zero)
	Vector3D(double xyz);	//creates a new vector (all elements are xyz)
	Vector3D(double x, double y, double z);	//creates a new vector with specified elements
	Vector3D(const Vector3D& other);	//creates a new vector that is the same as the other one
	
	//destructor
	~Vector3D();

	//get the various elements of the vector
	double getX() const;
	double getY() const;
	double getZ() const;

	//change the various elements of the vector
	void setX(double x);
	void setY(double y);
	void setZ(double z);
	void setVal(double x, double y, double z);

	//operators (they work using all the standard vector arithmetic)
	Vector3D& operator=(const Vector3D& second);

	Vector3D operator-() const;
	Vector3D operator+(const Vector3D& second) const;
	Vector3D operator-(const Vector3D& second) const;
	Vector3D& operator+=(const Vector3D& second);
	Vector3D& operator-=(const Vector3D& second);
	Vector3D operator*(double scaler) const;
	friend Vector3D operator*(double scaler, const Vector3D& second);
	Vector3D& operator*=(double scaler);
	Vector3D operator/(double scaler) const;
	Vector3D& operator/=(double scaler);

	Vector3D normalvector() const; //return the direction vector of a given vector
	Vector3D& normalize(); //set this vectors magnitude to 1.0

	double dot(const Vector3D& second) const;	//dot product with another vector
	Vector3D cross(const Vector3D& second) const;	//cross product with another vector

	//this next one is a real doozy, it checks if the vector is in a region defined by an origin and three basis vectors
	bool inParallelipiped(const Vector3D& o, const Vector3D& v1, const Vector3D& v2, const Vector3D& v3) const;	//good god, don't use it unless you have to

	//these return the magnitude or magnitude squared
	double magnitudesq() const;	//use this and sqrt on your own if you want to save a little processing time
	double magnitude() const;	//or you can use this if you don't care

	friend Vector3D maxvect(const Vector3D& first, const Vector3D& second); //vector made of the maximum components for each vector
	friend Vector3D minvect(const Vector3D& first, const Vector3D& second);	//ditto minimum
	friend Vector3D avgvect(const Vector3D& first, const Vector3D& second);	//ditto average
	friend Vector3D absvect(const Vector3D& vect);	//all components positive
		
private:
	double value[3];
};

#include "vectors.inl.h"

#endif
