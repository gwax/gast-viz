/*
 *	GAST-Viz
 *	version 1.0.0 (04 January 2004)
 *
 *	fps.cpp
 *	SOURCE
 *		fps counters, wahoo!
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

#include "fps.h"
#include "glow.h"

GLOW_NAMESPACE_USING

fpscounter::fpscounter()
{
	for(int i=0; i<FPS_BUFFER; i++)	//just fill the whole buffer
	{
		frametime[i] = Glow::GetMilliseconds();	//with the current time
	}

	lastget = 0;	//this means that mssincelastget will return a very long time until you get the fps for the first time (it's not significant but I prefer it this way)

	currentframe = 0;	//this sets up the rotating buffer

	avgframes = DEFAULT_AVG_PERIOD;	//and we use the default average period
}

fpscounter::fpscounter(unsigned int avgperiod)
{	//see above
	for(int i=0; i<FPS_BUFFER; i++)
	{
		frametime[i] = Glow::GetMilliseconds();
	}

	lastget = 0;

	currentframe = 0;

	avgframes = avgperiod%FPS_BUFFER;	//this is what I mean by not guaranteeing anything
}

unsigned int fpscounter::setavgperiod(unsigned int newAvgperiod)
{
	avgframes = newAvgperiod%FPS_BUFFER;	//this is what I mean by not guaranteeing anything
	return avgframes;	//return the value in case anyone wants to check
}

unsigned int fpscounter::mssincelastget() const
{
	return (Glow::GetMilliseconds() - lastget);
}

float fpscounter::getfps()
{
	float deltaframes = (float)avgframes;	//convert frames being averaged to float for accuracy's sake
	float deltatime = (float)(frametime[currentframe] - frametime[(FPS_BUFFER+currentframe-avgframes)%FPS_BUFFER]);	//deltatime = currenttime - time at the frame that is the averaging period ago
	lastget = frametime[currentframe];	//the last time we did get was now
	return (1000.0f*deltaframes/deltatime);	//frames per second = 1000 * frames passed / milliseconds passed
}

void fpscounter::addframe()
{
	currentframe++;	//the current frame is one farther in the list
	currentframe %= FPS_BUFFER;	//wrap around if we hit the end
	frametime[currentframe] = Glow::GetMilliseconds();	//record the time of the current frame
}
