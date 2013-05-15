/*
 *	GAST-Viz
 *	version 1.0.0 (10 January 2004)
 *
 *	stack2d.cpp
 *	SOURCE
 *		main program file for stack2d
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
#include "glowMessageWindow.h"
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


//these are used for Atom type values because names are better than numbers
#define EDGEATOM 3
#define OTHERATOM 4


//some custom interaction functions are employed here and to avoid prototyping they have been put before the AssemblageWindow

//variables to hold our strain rates
double strainYrate;
double strainXrate;

//the next two functions handle the strain rate bits
struct interact strainy(Atom* one)	//one body interaction
{
	if(one->getType() == EDGEATOM)	//if it's one of the Atoms at the edge
		one->translate(Vector3D(0,signf(one->getPosition().getY())*strainYrate*Assemblage::getdt(),0)); //then move it a little farther away along the y-direction

	//we're not actually applying a force to any Atoms so we return NULL interact results to be ignored
	struct interact result;
	result.forces = NULL;
	result.potentials = NULL;
	return result;
}

struct interact strainx(Atom* one)
{
	if(one->getType() == EDGEATOM)
		one->translate(Vector3D(signf(one->getPosition().getY())*strainXrate*Assemblage::getdt(),0,0));

	struct interact result;
	result.forces = NULL;
	result.potentials = NULL;
	return result;
}

//this encapsulates the standard lj6_12 force function and filters it so that the Atoms at the edge don't feel any force
struct interact filteredlj6_12(Atom* one, Atom* two)
{
	struct interact result = lj6_12::forcefunction(one, two);	//get the force from lj6_12
	
	if(result.forces == NULL)	//if we got a NULL force, just return what we got
		return result;

	if(one->getType() == EDGEATOM)			//if any given atom
		result.forces[0] = Vector3D(0.0);	//happens to be an
	if(two->getType() == EDGEATOM)			//EDGEATOM, then set it's
		result.forces[1] = Vector3D(0.0);	//force to a zero vector
	return result;	//leaving the potentials alone, return the (possibly) modified result
}


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

	//interactions for our assemblage
	Interaction2B* myfilteredlj6_12interact;
	Interaction1B* mystrainy;
	Interaction1B* mystrainx;

	//default values to be given to Atoms
	double defaultColor[3];
	double defaultMass;
	double defaultRad;

	//pointers to all of the user interface components
	GlowQuickPaletteSubwindow* mystatuspanel;
	GlowLabelWidget* _templabel;
	GlowLabelWidget* _fpslabel;

	GlowQuickPaletteWindow* _controlsWindow;

	GlowPushButtonWidget* _initPushButton;
	GlowPushButtonWidget* _createvacancyPushButton;

	GlowLabeledTextFieldWidget* _epsilonText;
	GlowPushButtonWidget* _setinteractPushButton;

	GlowLabeledTextFieldWidget* _strainYText;
	GlowLabeledTextFieldWidget* _strainXText;
	GlowLabeledTextFieldWidget* _dtText;
	GlowLabeledTextFieldWidget* _tempText;
	GlowLabeledTextFieldWidget* _jiggleText;
	GlowPushButtonWidget* _setsystemPushButton;

	GlowQuickRadioGroupWidget* _movealgoRadioGroup;
	GlowRadioButtonWidget* _moveStopRadio;
	GlowRadioButtonWidget* _moveSimpleRadio;
	GlowRadioButtonWidget* _moveDampedRadio;
	GlowRadioButtonWidget* _moveVelocityRadio;
	GlowRadioButtonWidget* _moveBeemanRadio;

	GlowQuickRadioGroupWidget* _drawRadioGroup;
	GlowRadioButtonWidget* _drawnativeRadio;
	GlowRadioButtonWidget* _drawenergyRadio;
	GlowQuickPanelWidget* _energyPanel;
	GlowLabeledTextFieldWidget* _emaxText;
	GlowLabeledTextFieldWidget* _eminText;
	GlowPushButtonWidget* _esetPushButton;

	GlowPushButtonWidget* _quitPushButton;
};

//main window constructor
AssemblageWindow::AssemblageWindow() :
GlowWindow("Stack 2D", GlowWindow::autoPosition, GlowWindow::autoPosition, 320, 320, Glow::rgbaBuffer, Glow::noEvents)	//obligatory superclass constructor call
{
	myfps = new fpscounter(13);	//create a new fps counter

	myassemblage = new Assemblage(Vector3D(-4,-12,-1), Vector3D(4,12,1));	//create an assemblage
	myassemblage->showboundaries(false);	//stack2d will appear 2D so we don't want to show the 3D boundary box
	myassemblage->setjigglemask(true, true, false);	//we also don't want jiggle to move Atoms out of the plane
	myassemblage->cutoff(2.5);	//this is a pretty reasonable cutoff value for this simulation

	mysubwindow = new AssemblageSubwindow(this, 320, 30, myassemblage);	//create the subwindow to hold our assemblage (vertical size irrelevant as it will be resized in a moment)
	mysubwindow->spinnable(false);	//again, 2D so we don't want it to be spinnable
	mysubwindow->transparency(false);	//transparency off because I don't need it (enabled by default)

	mystatuspanel = new GlowQuickPaletteSubwindow(this, 0, 0, 320, 30, GlowQuickPalette::horizontal, GlowQuickPalette::alignCenter, 10, 0, 0);	//creates a status panel subwindow
	_templabel = mystatuspanel->AddLabel("Temp = 00.000000");	//adds a label for the temperature to the status panel
	_fpslabel = mystatuspanel->AddLabel("fps - 0000.0000");	//adds a label for the fps count to the status panel
	mystatuspanel->Pack();	//packs the status panel (see Glow documentation)

	Reshape(320, 320+mystatuspanel->Height());	//call the reshape function which will call our resize function and make everything the right shape

	lj6_12::setdefaultepsilon(1.0);	//set the default epsilon value for Lennard-Jones interactions

	myfilteredlj6_12interact = new Interaction2B(filteredlj6_12);	//create a filtered Lennard-Jones interaction
	myassemblage->addInteraction(myfilteredlj6_12interact);			//and add it to our assemblage

	mystrainy = new Interaction1B(strainy);		//create the strain interactions
	mystrainx = new Interaction1B(strainx);
	myassemblage->addInteraction(mystrainy);	//and add them to our assemblage
	myassemblage->addInteraction(mystrainx);

	//here we construct the user interface using the methods provided by Glow
	GlowQuickPanelWidget* panel1;

	_controlsWindow = new GlowQuickPaletteWindow("Controls");
		panel1 = _controlsWindow->AddArrangingPanel(GlowQuickPalette::horizontal, GlowQuickPalette::alignCenter);
			_initPushButton = panel1->AddPushButton("Init", this);
			_createvacancyPushButton = panel1->AddPushButton("Create Vacancy", this);

		panel1 = _controlsWindow->AddPanel(GlowQuickPanelWidget::etchedStyle, "Interactions", GlowQuickPalette::horizontal);
			lj6_12::setdefaultepsilon(1.0);
			_epsilonText = panel1->AddTextField(100, "1.0", "Epsilon");
			_setinteractPushButton = panel1->AddPushButton("Set", this);

		panel1 = _controlsWindow->AddPanel(GlowQuickPanelWidget::etchedStyle, "System");
			strainYrate = 0.0;
			_strainYText = panel1->AddTextField(100, "0.0", "Strain Rate (y-axis)");
			strainXrate = 0.0;
			_strainXText = panel1->AddTextField(100, "0.0", "Strain Rate (x-axis)");
			myassemblage->setdt(0.01);
			_dtText = panel1->AddTextField(100, "0.01", "Time Step (dt)");
			myassemblage->setEQsteps(25);
			myassemblage->OptimalTemp(0.02);
			_tempText = panel1->AddTextField(100, "0.02", "Desired Temp");
			myassemblage->setjiggle(0.0);
			_jiggleText = panel1->AddTextField(100, "0.0", "Jiggle");
			_setsystemPushButton = panel1->AddPushButton("Set", this);

		panel1 = _controlsWindow->AddPanel(GlowQuickPanelWidget::etchedStyle, "Move Algorithm");
			_movealgoRadioGroup = panel1->AddRadioGroup(GlowQuickPalette::vertical, 3, this);
			_moveStopRadio = _movealgoRadioGroup->AddRadioButton("Stopped");
			_moveSimpleRadio = _movealgoRadioGroup->AddRadioButton("Simple");
			_moveDampedRadio = _movealgoRadioGroup->AddRadioButton("Damped");
			_moveVelocityRadio = _movealgoRadioGroup->AddRadioButton("Velocity Verlet");
			_moveBeemanRadio = _movealgoRadioGroup->AddRadioButton("Beeman");

		panel1 = _controlsWindow->AddPanel(GlowQuickPanelWidget::etchedStyle, "Draw Style");
			_drawRadioGroup = panel1->AddRadioGroup(GlowQuickPalette::horizontal, 3, this);
			_drawnativeRadio = _drawRadioGroup->AddRadioButton("Native");
			_drawenergyRadio = _drawRadioGroup->AddRadioButton("Energy");
			_energyPanel = panel1->AddPanel(GlowQuickPanelWidget::etchedStyle, "Energy");
			_emaxText = _energyPanel->AddTextField(100, "-5.0", "E max");
			_eminText = _energyPanel->AddTextField(100, "-7.0", "E min");
			_esetPushButton = _energyPanel->AddPushButton("Set", this);
			_energyPanel->Deactivate();

		_quitPushButton = _controlsWindow->AddPushButton("Quit", this);

	_controlsWindow->Pack();
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
	delete mystatuspanel;
	delete myfilteredlj6_12interact;
	delete mystrainy;
	delete mystrainx;
	delete _controlsWindow;

	//many other objects exist but will be deleted recursively by the deletion of these objects
}

void AssemblageWindow::OnReshape(int width, int height)
{
	if (width < 80 || height < 80+mystatuspanel->Height())	//if the window is smaller than this minimum size
	{
		Reshape(mymax(width, 80), mymax(height, 80+mystatuspanel->Height()));	//reshape it to this minimum
	}
	else	//otherwise we can properly reshape things
	{
		if(width != mystatuspanel->Width())	//if the status panel has changed width
			mystatuspanel->Reshape(width, mystatuspanel->Height());	//we need to tell it to reshape itself
		mystatuspanel->Move(0, 0);	//and then move it to the main window's top left corner

		int height2 = height - mystatuspanel->Height();	//height available to subwindow
		int subwindsize = mymin(width, height2);	//find the subwindow's maximum available dimension
		mysubwindow->Move((width-subwindsize)/2, mystatuspanel->Height()+(height2-subwindsize)/2);	//move the subwindow to the center of the free space
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

		sprintf(buffer, "Temp = %09.6f", myassemblage->getCurrentTemp());	//write the temperatue to buffer
		_templabel->SetText(buffer);	//update label
		
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

	if(message.groupWidget == _drawRadioGroup)	//should we be changing the drawwing method
	{
		if(message.buttonWidget == _drawnativeRadio)	//should we go into native color mode
		{
			myassemblage->nativecolorall();	//tell the assemblage to go into native color mode
			_energyPanel->Deactivate();	//deactivate the energy panel in the user interface
			mysubwindow->Refresh();	//request a refresh
		}

		if(message.buttonWidget == _drawenergyRadio)	//should we go into energy color mode
		{
			double emin, emax;	//temp variables
			_energyPanel->Activate();	//activate the energy panel
			emin = atof(_eminText->GetTextChars());	//get values for the energy panel
			emax = atof(_emaxText->GetTextChars());

			if(emin > emax)	//if emin is greater than emax
			{
				double temp;	//reverse their values
				temp = emin;
				emin = emax;
				emax = temp;
			}

			if(emin == emax)	//if their values are the same
			{
				emax += 0.1;	//increase emax a little
			}

			char* buffer = new char[20];	//set the user interface values to match our internal values
			sprintf(buffer, "%f", emin);
			_eminText->SetText(buffer);
			sprintf(buffer, "%f", emax);
			_emaxText->SetText(buffer);
			delete buffer;

			myassemblage->energycolorall(emin, emax);	//tell the asseblage to go into energy color mode using our energy values
			mysubwindow->Refresh();	//request a refresh
		}
	}
}

void AssemblageWindow::OnMessage(const GlowPushButtonMessage& message)	//on receipt of a push button message
{
	if(message.widget == _quitPushButton)	//if it was the quit push button
	{
		exit(0);	//then quit
	}

	if(message.widget == _initPushButton)
	{
		const double sqrt3 = sqrt(3.0);

		if(myassemblage->numAtoms() >0 )
		{
			fprintf(stdout, "Clearing old Atoms\n");
			int clearednum = myassemblage->removeAllAtoms();
			fprintf(stdout, "%d Atoms successfully removed\n", clearednum);
		}

		defaultColor[0] = (double)Random::randfloat();
		defaultColor[1] = (double)Random::randfloat();
		defaultColor[2] = (double)Random::randfloat();

		int i, j, newtype;
		Atom* temp;
		for(i=-10; i<=10; i++)
		{
			if(i == 10 || i == -10)
				newtype = EDGEATOM;
			else
				newtype = OTHERATOM;

			if(abs(i)%2 == 0)
			{
				for(j=0; j<8; j++)
				{
					temp = new Atom(newtype, Vector3D(-3.5+(double)j, 0.5*sqrt3*(double)i, 0.0), Vector3D(0.0), 0.5, 1.0, 0.0, defaultColor[0], defaultColor[1], defaultColor[2]);
					myassemblage->addAtom(temp);
				}
			}
			else
			{
				for(j=0; j<7; j++)
				{
					temp = new Atom(newtype, Vector3D(-3+(double)j, 0.5*sqrt3*(double)i, 0.0), Vector3D(0.0), 0.5, 1.0, 0.0, defaultColor[0], defaultColor[1], defaultColor[2]);
					myassemblage->addAtom(temp);
				}
			}
		}

		fprintf(stdout, "Created %d Atoms\n", myassemblage->numAtoms());

		myassemblage->calculateneighbors();
		myassemblage->interactall();
		for(int i=0; i<myassemblage->numAtoms(); i++)
		{
			myassemblage->getAtom(i)->setforcetonew();
			myassemblage->getAtom(i)->setforcetonew();
		}

		if(_drawenergyRadio->GetState())
		{
			double emin, emax;
			emin = atof(_eminText->GetTextChars());
			emax = atof(_emaxText->GetTextChars());

			if(emin > emax)
			{
				double temp;
				temp = emin;
				emin = emax;
				emax = temp;
			}

			if(emin == emax)
			{
				emax += 0.1;
			}

			char* buffer = new char[20];
			sprintf(buffer, "%f", emin);
			_eminText->SetText(buffer);
			sprintf(buffer, "%f", emax);
			_emaxText->SetText(buffer);
			delete buffer;

			myassemblage->energycolorall(emin, emax);
		}

		mysubwindow->Refresh();
	}

	if(message.widget == _createvacancyPushButton)	//create a random vacancy
	{
		if(myassemblage->numAtoms() > 0)	//if there are any atoms to remove
		{
			myassemblage->removeAtom(myassemblage->getAtom(Random::randint(0, myassemblage->numAtoms()-1)));	//randomly remove an atom

			fprintf(stdout, "1 Atom successfully removed\n");	//output that we've removed an atom

			myassemblage->interactall();	//recalculate interactions
			for(int i=0; i<myassemblage->numAtoms(); i++)
			{
				myassemblage->getAtom(i)->setforcetonew();	//clear force buffers
				myassemblage->getAtom(i)->setforcetonew();
			}
		}

		mysubwindow->Refresh();	//request a refresh
	}

	if(message.widget == _setinteractPushButton)	//set the interaction conditions
	{
		double epsilon = atof(_epsilonText->GetTextChars());	//get new epsilon value

		if(epsilon <= 0.0)	//if  new epsilon is too small
		{
			new GlowMessageWindow("Illegal Value!", GlowWindow::autoPosition, GlowWindow::autoPosition, "You have input an illegal value!", "OK", NULL);	//yell at the user
		}
		else
		{
			lj6_12::setdefaultepsilon(epsilon);	//otherwise set the value to the provided one
		}

		char* buffer = new char[20];
		
		sprintf(buffer, "%f", lj6_12::getdefaultepsilon());	//update the user interface to match internals
		_epsilonText->SetText(buffer);
		
		delete buffer;

		mysubwindow->Refresh();	//request a refresh
	}

	if(message.widget == _setsystemPushButton)	//set the system conditions
	{
		double newtimestep = atof(_dtText->GetTextChars());		//new timestep value
		double newstrainYrate = atof(_strainYText->GetTextChars());	//new value for y-strain rate
		double newstrainXrate = atof(_strainXText->GetTextChars());	//new value for x-strain rate
		double newtemp = mymax(0.0,atof(_tempText->GetTextChars()));	//new optimal temperature
		double newjiggle = mymax(0.0,atof(_jiggleText->GetTextChars()));	//new jiggle

		if(newtimestep <= 0.0)	//if we have a negative value for dt
		{
			new GlowMessageWindow("Illegal Value!", GlowWindow::autoPosition, GlowWindow::autoPosition, "You have input an illegal value!", "OK", NULL);	//yell at the user
		}
		else	//otherwise
		{
			myassemblage->setdt(newtimestep);	//internally set everything to the new values
			strainYrate = newstrainYrate;
			strainXrate = newstrainXrate;
			myassemblage->OptimalTemp(newtemp);
			myassemblage->setjiggle(newjiggle);
		}

		char* buffer = new char[20];
		
		sprintf(buffer, "%f", strainYrate);		//update the user interface to match the internals
		_strainYText->SetText(buffer);
		sprintf(buffer, "%f", strainXrate);
		_strainXText->SetText(buffer);
		sprintf(buffer, "%f", myassemblage->getdt());
		_dtText->SetText(buffer);
		sprintf(buffer, "%f", newtemp);
		_tempText->SetText(buffer);
		sprintf(buffer, "%f", newjiggle);
		_jiggleText->SetText(buffer);
		
		delete buffer;

		mysubwindow->Refresh(); //request a refresh
	}

	if(message.widget == _esetPushButton)	//set the energy coloring limits
	{
		double emin = atof(_eminText->GetTextChars());	//get emin and emax from the interface
		double emax = atof(_emaxText->GetTextChars());

		if(emin > emax)	//if emin is greater than emax
		{
			double temp;	//swap their values
			temp = emin;
			emin = emax;
			emax = temp;
		}

		if(emin == emax)	//if emin equals emax
		{
			emax += 0.1;	//make emax a little bigger
		}

		char* buffer = new char[20];	//update user interface to match internals
		sprintf(buffer, "%f", emin);
		_eminText->SetText(buffer);
		sprintf(buffer, "%f", emax);
		_emaxText->SetText(buffer);
		delete buffer;

		myassemblage->energycolorall(emin, emax);	//change energy coloring
		mysubwindow->Refresh();	//request a refresh
	}
}









int main(int argc, char** argv)	//int main
{
	fprintf(stdout, "stack2d - a GAST-Viz application\n");		//print out various bits of header information
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
