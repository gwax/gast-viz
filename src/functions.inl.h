/*
 *	GAST-Viz
 *	version 1.0.0 (04 January 2004)
 *
 *	functions.inl.h
 *	INLINE HEADER
 *		various small utility functions
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

#ifndef _functions_inl_h_
#define _functions_inl_h_

//because max and min aren't always defined (which is really aggravating)
#define mymax(a,b)  (((a) > (b)) ? (a) : (b))
#define mymin(a,b)  (((a) < (b)) ? (a) : (b))

inline double signf(double number)
{
	if(number>=0.0)
		return 1.0;
	else
		return -1.0;
}

#endif
