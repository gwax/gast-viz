/*
 *	GAST-Viz
 *	version 1.0.0 (04 January 2004)
 *
 *	vectors.cpp
 *	SOURCE
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

#include "vectors.h"

/*===============================================
	class Vector3D
===============================================*/

/*-----------------------------------------------
	Methods
-----------------------------------------------*/
/*This will be a real system hog if you're doing it for every atom, so try not to*/
bool Vector3D::inParallelipiped(const Vector3D& o, const Vector3D& v1, const Vector3D& v2, const Vector3D& v3) const
{
	/*there's a lot of linear algebra here,
	but basically what we're doing is solving

	this - o = a*v1 + b*v2 + c*v3 for a, b and c

	then if a, b and c are between 0 and 1, it's in the region*/

	//determinant of [v1, v2, v3]
	double determinant = v1.value[0]*(v2.value[1]*v3.value[2]-v3.value[1]*v2.value[2])
						+v2.value[0]*(v3.value[1]*v1.value[2]-v1.value[1]*v3.value[2])
						+v3.value[0]*(v1.value[1]*v2.value[2]-v2.value[1]*v1.value[2]);

	if(determinant == 0.0)
		return false;	//I think that this is an ambiguous case, but either way this is just a div by zero catch

	Vector3D t(*this-o);

	//following is the result of ([v1,v2,v3]^-1).(this-o)
	double a = (t.value[0]*(v2.value[1]*v3.value[2]-v3.value[1]*v2.value[2])
				+t.value[1]*(v2.value[2]*v3.value[0]-v3.value[2]*v2.value[0])
				+t.value[2]*(v2.value[0]*v3.value[1]-v3.value[0]*v2.value[1])
				)/determinant;

	double b = (t.value[0]*(v1.value[2]*v3.value[1]-v3.value[2]*v1.value[1])
				+t.value[1]*(v1.value[0]*v3.value[2]-v3.value[0]*v1.value[2])
				+t.value[2]*(v1.value[1]*v3.value[0]-v3.value[1]*v1.value[0])
				)/determinant;

	double c = (t.value[0]*(v1.value[1]*v2.value[2]-v2.value[1]*v1.value[2])
				+t.value[1]*(v1.value[2]*v2.value[0]-v2.value[2]*v1.value[0])
				+t.value[2]*(v1.value[0]*v2.value[1]-v2.value[0]*v1.value[1])
				)/determinant;
	//good god, just don't ask, it's a whole lot of linear algebra and I've more than triple checked it

	//check if it's out of the bounds
	if(a < 0.0  || a > 1.0 || b < 0.0  || b > 1.0 || c < 0.0  || c > 1.0)
		return false;

	//otherwise it's in the bounding parallelipiped
	return true;	//joy
}
