/*
 *	GAST-Viz
 *	version 1.0.0 (04 January 2004)
 *
 *	vectors.inl.h
 *	INLINE HEADER
 *		inlines are good for speed
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

#ifndef _vectors_inl_h_
#define _vectors_inl_h_

/*===============================================
	class Vector3D
===============================================*/

/*-----------------------------------------------
	Constructors
-----------------------------------------------*/
inline Vector3D::Vector3D()
{
	value[0] = 0.0;
	value[1] = 0.0;
	value[2] = 0.0;
}

inline Vector3D::Vector3D(double xyz)
{
	value[0] = xyz;
	value[1] = xyz;
	value[2] = xyz;
}

inline Vector3D::Vector3D(double x, double y, double z)
{
	value[0] = x;
	value[1] = y;
	value[2] = z;
}

inline Vector3D::Vector3D(const Vector3D& other)
{
	value[0] = other.value[0];
	value[1] = other.value[1];
	value[2] = other.value[2];
}


/*-----------------------------------------------
	Destructor
-----------------------------------------------*/
inline Vector3D::~Vector3D()
{
}


/*-----------------------------------------------
	Inline Methods
-----------------------------------------------*/
inline double Vector3D::getX() const
{
	return value[0];
}

inline double Vector3D::getY() const
{
	return value[1];
}

inline double Vector3D::getZ() const
{
	return value[2];
}

inline void Vector3D::setX(double x)
{
	value[0] = x;
}

inline void Vector3D::setY(double y)
{
	value[1] = y;
}

inline void Vector3D::setZ(double z)
{
	value[2] = z;
}

inline void Vector3D::setVal(double x, double y, double z)
{
	value[0] = x;
	value[1] = y;
	value[2] = z;
}

inline Vector3D& Vector3D::operator=(const Vector3D& second)
{
	value[0] = second.value[0];
	value[1] = second.value[1];
	value[2] = second.value[2];
	return *this;
}

inline Vector3D Vector3D::operator-() const
{
	return Vector3D(-value[0], -value[1], -value[2]);
}

inline Vector3D Vector3D::operator+(const Vector3D& second) const
{
	return Vector3D(value[0]+second.value[0], value[1]+second.value[1], value[2]+second.value[2]);
}

inline Vector3D Vector3D::operator-(const Vector3D& second) const
{
	return Vector3D(value[0]-second.value[0], value[1]-second.value[1], value[2]-second.value[2]);
}

inline Vector3D& Vector3D::operator+=(const Vector3D& second)
{
	value[0] += second.value[0];
	value[1] += second.value[1];
	value[2] += second.value[2];
	return *this;
}

inline Vector3D& Vector3D::operator-=(const Vector3D& second)
{
	value[0] -= second.value[0];
	value[1] -= second.value[1];
	value[2] -= second.value[2];
	return *this;
}

inline Vector3D Vector3D::operator*(double scaler) const
{
	return Vector3D(value[0] * scaler, value[1] * scaler, value[2] * scaler);
}

inline Vector3D& Vector3D::operator*=(double scaler)
{
	value[0] *= scaler;
	value[1] *= scaler;
	value[2] *= scaler;
	return *this;
}

inline Vector3D Vector3D::operator/(double scaler) const
{
	if(scaler==0.0)
		scaler = 1E-50;

	return (*this*(1.0/scaler));
}

inline Vector3D& Vector3D::operator/=(double scaler)
{
	if(scaler==0.0)
		scaler = 1E-50;

	return (*this*=(1.0/scaler));
}

inline Vector3D Vector3D::normalvector() const
{
	double temp = magnitudesq();
	if(temp == 0.0)
		return(Vector3D(1.0, 0.0, 0.0));
	
	return (*this * pow(temp,-0.5));
}

inline Vector3D& Vector3D::normalize()
{
	double temp = magnitudesq();
	if(temp == 0.0)
		return(*this=Vector3D(1.0, 0.0, 0.0));
	
	return (*this *= pow(temp,-0.5));
}

inline double Vector3D::dot(const Vector3D& second) const
{
	return (value[0]*second.value[0]+value[1]*second.value[1]+value[2]*second.value[2]);
}

inline Vector3D Vector3D::cross(const Vector3D& second) const
{
	return Vector3D(
		value[1]*second.value[2]-value[2]*second.value[1],
		value[2]*second.value[0]-value[0]*second.value[2],
		value[0]*second.value[1]-value[1]*second.value[0]);
}

inline double Vector3D::magnitudesq() const
{
	return (value[0]*value[0]+value[1]*value[1]+value[2]*value[2]);
}

inline double Vector3D::magnitude() const
{
	return sqrt(magnitudesq());
}

/*-----------------------------------------------
	Inline Vector3D's friends
-----------------------------------------------*/
inline Vector3D operator*(double scaler, const Vector3D& second)
{
	return second*scaler;
}

inline Vector3D maxvect(const Vector3D& first, const Vector3D& second)
{
	return Vector3D(mymax(first.value[0],second.value[0]), mymax(first.value[1],second.value[1]), mymax(first.value[2],second.value[2]));
}

inline Vector3D minvect(const Vector3D& first, const Vector3D& second)
{
	return Vector3D(mymin(first.value[0],second.value[0]), mymin(first.value[1],second.value[1]), mymin(first.value[2],second.value[2]));
}

inline Vector3D avgvect(const Vector3D& first, const Vector3D& second)
{
	return Vector3D((first.value[0]+second.value[0])*0.5, (first.value[1]+second.value[1])*0.5, (first.value[2]+second.value[2])*0.5);
}

inline Vector3D absvect(const Vector3D& vect)
{
	return Vector3D(mymax(vect.value[0],-vect.value[0]), mymax(vect.value[1],-vect.value[1]), mymax(vect.value[2],-vect.value[2]));
}

#endif
