/*
 *	GAST-Viz
 *	version 1.0.0 (10 January 2004)
 *
 *	template.cpp
 *	SOURCE
 *		template file for making atomic simulation applications
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

#include <stdio.h>
#include "glow.h"
#include "glowQuickPalette.h"
#include "glowViewTransform.h"

GLOW_NAMESPACE_USING

#include "assemblage.h"
#include "atoms.h"
#include "fps.h"
#include "interactionlibrary.h"
#include "interactions.h"
#include "random.h"
#include "vectors.h"
#include "functions.inl.h"

//here we set up our main window's class
class AssemblageWindow :
	public GlowWindow,				//it is a glow window
	public GlowPushButtonReceiver,	//that receives push button,
	public GlowRadioButtonReceiver,	//radio button
	public GlowIdleReceiver			//and idle events
{
public:
	AssemblageWindow();
	~AssemblageWindow();
	
protected:	//inherited Glow functions that need to be filled in
	virtual void OnReshape(int width, int height);

	virtual void OnMessage(const GlowRadioButtonMessage& message);
	virtual void OnMessage(const GlowPushButtonMessage& message);
	virtual void OnMessage(const GlowIdleMessage& message);
	
private:
	//contained object data structures
	fpscounter* myfps;
	AssemblageSubwindow* mysubwindow;
	Assemblage* myassemblage;

	//interaction for our assemblage
	Interaction2B* mylj6_12interact;

	//pointers to all of the user interface components
	GlowQuickPaletteSubwindow* myfpspanel;
	GlowLabelWidget* _fpslabel;

	GlowQuickPaletteWindow* _controlWindow;

	GlowPushButtonWidget* _initButton;

	GlowQuickRadioGroupWidget* _movealgoRadioGroup;
	GlowRadioButtonWidget* _moveStopRadio;
	GlowRadioButtonWidget* _moveSimpleRadio;
	GlowRadioButtonWidget* _moveDampedRadio;
	GlowRadioButtonWidget* _moveVelocityRadio;
	GlowRadioButtonWidget* _moveBeemanRadio;

	GlowPushButtonWidget* _quitPushButton;
};

//main window constructor
AssemblageWindow::AssemblageWindow() :
GlowWindow("Template", GlowWindow::autoPosition, GlowWindow::autoPosition,320, 320, Glow::rgbaBuffer, Glow::noEvents)	//obligatory superclass constructor call
{
	myassemblage = new Assemblage(Vector3D(-1,-1,-1), Vector3D(1,1,1));	//create an assemblage
	myassemblage->showboundaries(true);	//we want to see our boundaries
	myassemblage->setdt(0.001);	//damped, velocity verlet, beeman are stable; simple is unstable

	mysubwindow = new AssemblageSubwindow(this, 320, 30, myassemblage);	//create the subwindow to hold our assemblage (vertical size irrelevant as it will be resized in a moment)
	mysubwindow->spinnable(true);	//spinnable is good for 3D visualization
	mysubwindow->transparency(false);	//transparency off because I don't need it (enabled by default)

	myfps = new fpscounter(13);	//create a new fps counter

	myfpspanel = new GlowQuickPaletteSubwindow(this, 0, 0, 320, 30, GlowQuickPalette::horizontal, GlowQuickPalette::alignCenter, 10, 0, 0);	//creates a fps panel subwindow
	_fpslabel = myfpspanel->AddLabel("fps - Not Yet Initialized             ");	//adds a label for the fps count to the fps panel
	myfpspanel->Pack();	//packs the fps panel (see Glow documentation)

	mylj6_12interact = new Interaction2B(lj6_12::forcefunction);	//create a Lennard-Jones interaction
	myassemblage->addInteraction(mylj6_12interact);					//and add it to our assemblage

	Reshape(320, 320+myfpspanel->Height());	//call the reshape function which will call our resize function and make everything the right shape

	//here we construct the user interface using the methods provided by Glow
	GlowQuickPanelWidget* panel1;

	_controlWindow = new GlowQuickPaletteWindow("Controls");
		_initButton = _controlWindow->AddPushButton("Initialize", this);

		panel1 = _controlWindow->AddPanel(GlowQuickPanelWidget::etchedStyle, "Move Algorithm", GlowQuickPalette::vertical, GlowQuickPalette::alignLeft);
			_movealgoRadioGroup = panel1->AddRadioGroup(GlowQuickPalette::vertical, 3, this);
			_moveStopRadio = _movealgoRadioGroup->AddRadioButton("Stopped");
			_moveSimpleRadio = _movealgoRadioGroup->AddRadioButton("Simple");
			_moveDampedRadio = _movealgoRadioGroup->AddRadioButton("Damped");
			_moveVelocityRadio = _movealgoRadioGroup->AddRadioButton("Velocity Verlet");
			_moveBeemanRadio = _movealgoRadioGroup->AddRadioButton("Beeman");

		_quitPushButton = _controlWindow->AddPushButton("Quit", this);

	_controlWindow->Pack();
	//done contructing user interface

	Glow::RegisterIdle(mysubwindow);	//the subwindow needs to get idle calls
	Glow::RegisterIdle(this);			//and so does the main window
}

AssemblageWindow::~AssemblageWindow()
{
	//get rid of the stuff we created
	delete myfps;
	delete myassemblage;
	delete mysubwindow;
	delete myfpspanel;
	delete mylj6_12interact;
	delete _controlWindow;
	
	//many other objects exist but will be deleted recursively by the deletion of these objects
}

void AssemblageWindow::OnReshape(int width, int height)
{
	if (width < 80 || height < 80+myfpspanel->Height())	//if the window is smaller than this minimum size
	{
		Reshape(mymax(width, 80), mymax(height, 80+myfpspanel->Height()));	//reshape it to this minimum
	}
	else	//otherwise we can properly reshape things
	{
		if(width != myfpspanel->Width())	//if the fps panel has changed width
			myfpspanel->Reshape(width, myfpspanel->Height());	//we need to tell it to reshape itself
		myfpspanel->Move(0, 0);	//and then move it to the main window's top left corner

		int height2 = height - myfpspanel->Height();	//height available to subwindow
		int subwindsize = mymin(width, height2);	//find the subwindow's maximum available dimension
		mysubwindow->Move((width-subwindsize)/2, myfpspanel->Height()+(height2-subwindsize)/2);	//move the subwindow to the center of the free space
		mysubwindow->Reshape(subwindsize, subwindsize);	//resize the subwindow to fit
	}

	::glViewport(0, 0, width, height);	//we need to call glViewPort because of a Glow limitation, see Glow documentation for more
}

void AssemblageWindow::OnMessage(const GlowIdleMessage& message)	//on receipt of an idle message
{
	myfps->addframe();	//tell fps counter that one frame has passed
	if(myfps->mssincelastget() >= 500)	//if it's been long enough to justify updating the display
	{
		char* buffer = new char[30];	//allocate a buffer for holding text
		
		sprintf(buffer, "fps - %09.4f", myfps->getfps());	//write the fps count to buffer
		_fpslabel->SetText(buffer);	//update label

		delete buffer;	//delete our temporary buffer
	}
}

void AssemblageWindow::OnMessage(const GlowRadioButtonMessage& message)	//on receipt of a radio button message
{
	if(message.groupWidget == _movealgoRadioGroup)	//was it for the move algorithm radio group
	{
		if(message.buttonWidget == _moveStopRadio)		//was it for the stop move algorithm
			mysubwindow->setMoveType(AssemblageSubwindow::moveStopped);	//then tell the subwindow to go into stop mode

		if(message.buttonWidget == _moveSimpleRadio)	//was it for the simple move algorithm
			mysubwindow->setMoveType(AssemblageSubwindow::moveSimple);	//then tell the subwindow to go into simple mode
		
		if(message.buttonWidget == _moveDampedRadio)	//was it for the damped move algorithm
			mysubwindow->setMoveType(AssemblageSubwindow::moveDamped);	//then tell the subwindow to go into damped mode
		
		if(message.buttonWidget == _moveVelocityRadio)	//was it for the velocity verlet move algorithm
			mysubwindow->setMoveType(AssemblageSubwindow::moveVelocityVerlet);	//then tell the subwindow to go into velocity verlet mode

		if(message.buttonWidget == _moveBeemanRadio)	//was it for the beeman move algorithm
			mysubwindow->setMoveType(AssemblageSubwindow::moveBeeman);	//then tell the subwindow to go into beeman mode
	}
}

void AssemblageWindow::OnMessage(const GlowPushButtonMessage& message)	//on receipt of a push button message
{
	if(message.widget == _quitPushButton)	//if it was the quit push button
	{
		exit(0);	//then quit
	}

	if(message.widget == _initButton)	//create the initial atoms
	{
		if(myassemblage->numAtoms() >0 )	//if any atoms exist
		{
			fprintf(stdout, "Clearing old Atoms\n");
			int clearednum = myassemblage->removeAllAtoms();	//get rid of all the old atoms
			fprintf(stdout, "%d Atoms successfully removed\n", clearednum);	//output the number cleard
		}

		//the following just creates six atoms arranged on the faces of a 1.0x1.0x1.0 cube centered at 0.0, 0.0, 0.0
		Atom* temp;
		
		temp = new Atom(0, Vector3D(0.5, 0.0, 0.0), Vector3D(0.0), 0.2, 1.0, 0.0, (double)Random::randfloat(), (double)Random::randfloat(), (double)Random::randfloat());
		myassemblage->addAtom(temp);

		temp = new Atom(0, Vector3D(0.0, 0.5, 0.0), Vector3D(0.0), 0.2, 1.0, 0.0, (double)Random::randfloat(), (double)Random::randfloat(), (double)Random::randfloat());
		myassemblage->addAtom(temp);

		temp = new Atom(0, Vector3D(0.0, 0.0, 0.5), Vector3D(0.0), 0.2, 1.0, 0.0, (double)Random::randfloat(), (double)Random::randfloat(), (double)Random::randfloat());
		myassemblage->addAtom(temp);

		temp = new Atom(0, Vector3D(-0.5, 0.0, 0.0), Vector3D(0.0), 0.2, 1.0, 0.0, (double)Random::randfloat(), (double)Random::randfloat(), (double)Random::randfloat());
		myassemblage->addAtom(temp);

		temp = new Atom(0, Vector3D(0.0, -0.5, 0.0), Vector3D(0.0), 0.2, 1.0, 0.0, (double)Random::randfloat(), (double)Random::randfloat(), (double)Random::randfloat());
		myassemblage->addAtom(temp);

		temp = new Atom(0, Vector3D(0.0, 0.0, -0.5), Vector3D(0.0), 0.2, 1.0, 0.0, (double)Random::randfloat(), (double)Random::randfloat(), (double)Random::randfloat());
		myassemblage->addAtom(temp);

		fprintf(stdout, "Created %d Atoms\n", myassemblage->numAtoms());

		//do some energy calculations
		myassemblage->calculateneighbors();
		myassemblage->interactall();
		for(int i=0; i<myassemblage->numAtoms(); i++)
		{
			myassemblage->getAtom(i)->setforcetonew();	//clear the force buffer
			myassemblage->getAtom(i)->setforcetonew();
		}

		myassemblage->energycolorall(-4.0, 0.0);	//set energy coloring on

		mysubwindow->Refresh();	//request a refresh
	}
}









int main(int argc, char** argv)	//int main()
{
	fprintf(stdout, "template - a GAST-Viz application\n");		//print out various bits of header information
	fprintf(stdout, "Copyright (c)2003-2004 George Waksman. All rights reserved.\n\n");
	fprintf(stdout, "GAST-Viz Copyright (c)2003-2004 George Waksman. All rights reserved.\n");
	fprintf(stdout, "    GAST-Viz comes with ABSOLUTELY NO WARRANTY.\n");
	fprintf(stdout, "    This is free software, and you are welcome to redistribute it\n");
	fprintf(stdout, "    under certain conditions; see the included GPL license for details.\n\n\n");

	fprintf(stdout, "Starting Applicaton\n");

	Glow::Init(argc, argv);	//initialize Glow
	fprintf(stdout, "Glow Initialization complete\n");

	Random::timeseed();		//seed the random number generator
	fprintf(stdout, "Random seeded with %d\n", Random::getseed());	//output the value used to seed random

	new AssemblageWindow();	//create the main window
	fprintf(stdout, "Initial Components Constructed\n");

	fprintf(stdout, "Entering Glow Main Loop\n\n\n");
	Glow::MainLoop();	//start the Glow main loop

	return 0;	//we never get here
}
