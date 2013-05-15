/*
 *	GAST-Viz
 *	version 1.0.0 (04 January 2004)
 *
 *	fps.h
 *	HEADER
 *		class and functions to keep track
 *	of applicaion performance
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

#ifndef _fps_h_
#define _fps_h_

#define FPS_BUFFER 256
#define DEFAULT_AVG_PERIOD 5

class fpscounter
{
public:
	fpscounter();						//create a new fps counter with the default averaging period
	fpscounter(unsigned int avgperiod);			//create a new fps counter with a specified averaging period, if the specified period is larger than the allowed buffer it will work, but I guarantee no accuracy

	unsigned int setavgperiod(unsigned int newAvgperiod);	//change the averaging period; again, if it's longer than the buffer, I guarantee nothing
	unsigned int mssincelastget() const;	//how long has it been since I last asked for a fps count (useful if you don't want to get the fps more than once every so often)
	float getfps();						//what's the current frames per second count
	void addframe();					//call this when you finish a frame so the counter knows what's going on

private:
	unsigned int lastget;
	unsigned int currentframe;
	unsigned int avgframes;
	unsigned int frametime[FPS_BUFFER];
};

#endif
