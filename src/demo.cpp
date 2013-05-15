/*
 *	GAST-Viz
 *	version 1.0.0 (10 January 2004)
 *
 *	demo.cpp
 *	SOURCE
 *		main program file for demo
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

//here we set up our main window's class
class AssemblageWindow :
	public GlowWindow,				//it is a glow window
	public GlowCheckBoxReceiver,	//that receives check box,
	public GlowPushButtonReceiver,	//push button,
	public GlowRadioButtonReceiver,	//radio button
	public GlowIdleReceiver			//and idle events
{
public:
	AssemblageWindow();
	~AssemblageWindow();
	
protected:	//inherited Glow functions that need to be filled in
	virtual void OnReshape(int width, int height);

	virtual void OnMessage(const GlowCheckBoxMessage& message);
	virtual void OnMessage(const GlowRadioButtonMessage& message);
	virtual void OnMessage(const GlowPushButtonMessage& message);
	virtual void OnMessage(const GlowIdleMessage& message);
	
private:
	void createwalls();	//this is a utility function that creates the assemblage's walls

	//contained object data structures
	fpscounter* myfps;
	AssemblageSubwindow* mysubwindow;
	Assemblage* myassemblage;

	//interactions for our assemblage
	Interaction2B* mylj6_12interact;
	Interaction2B* mycoulombicinteract;
	InteractionS* myhardwall;
	Interaction1B* mygravity;

	//default values to be given to Atoms
	bool visibility1;
	bool visibility2;
	double defaultColor1[3];
	double defaultColor2[3];
	double defaultMass1;
	double defaultMass2;
	double defaultRad1;
	double defaultRad2;
	double defaultQ1;
	double defaultQ2;

	//pointers to all of the user interface components
	GlowQuickPaletteSubwindow* myfpspanel;
	GlowLabelWidget* _fpslabel;

	GlowQuickPaletteWindow* _setupWindow;
	GlowQuickPaletteWindow* _interactWindow;

	GlowLabeledTextFieldWidget* _numberType1Text;
	GlowLabeledTextFieldWidget* _numberType2Text;
	GlowPushButtonWidget* _createButton;
	GlowPushButtonWidget* _clearButton;

	GlowQuickRadioGroupWidget* _color1RadioGroup;
	GlowRadioButtonWidget* _color1redRadio;
	GlowRadioButtonWidget* _color1randRadio;
	GlowRadioButtonWidget* _color1invisRadio;
	GlowQuickRadioGroupWidget* _color2RadioGroup;
	GlowRadioButtonWidget* _color2greenRadio;
	GlowRadioButtonWidget* _color2randRadio;
	GlowRadioButtonWidget* _color2invisRadio;

	GlowLabeledTextFieldWidget* _timestepText;
	GlowPushButtonWidget* _setcalcButton;

	GlowQuickRadioGroupWidget* _movealgoRadioGroup;
	GlowRadioButtonWidget* _moveStopRadio;
	GlowRadioButtonWidget* _moveSimpleRadio;
	GlowRadioButtonWidget* _moveDampedRadio;
	GlowRadioButtonWidget* _moveVelocityRadio;
	GlowRadioButtonWidget* _moveBeemanRadio;

	GlowPushButtonWidget* _quitPushButton;

	GlowPushButtonWidget* _setinterButton;

	GlowLabeledTextFieldWidget* _mass1Text;
	GlowLabeledTextFieldWidget* _mass2Text;

	GlowCheckBoxWidget* _gravityenabledCheckBox;
	GlowLabeledTextFieldWidget* _gravxText;
	GlowLabeledTextFieldWidget* _gravyText;
	GlowLabeledTextFieldWidget* _gravzText;

	GlowCheckBoxWidget* _lj6_12enabledCheckBox;
	GlowLabeledTextFieldWidget* _epsilon11Text;
	GlowLabeledTextFieldWidget* _radius1Text;
	GlowLabeledTextFieldWidget* _epsilon22Text;
	GlowLabeledTextFieldWidget* _radius2Text;
	GlowLabeledTextFieldWidget* _epsilon12Text;
	GlowCheckBoxWidget* _coulombicenabledCheckBox;
	GlowLabeledTextFieldWidget* _q1Text;
	GlowLabeledTextFieldWidget* _q2Text;
};

//main window constructor
AssemblageWindow::AssemblageWindow() :
GlowWindow("Feature Demo", GlowWindow::autoPosition, GlowWindow::autoPosition,320, 320, Glow::rgbaBuffer, Glow::noEvents)	//obligatory superclass constructor call
{
	myfps = new fpscounter(13);	//create a new fps counter

	myassemblage = new Assemblage(Vector3D(-1,-1,-1), Vector3D(1,1,1));	//create an assemblage
	myassemblage->showboundaries(true);	//we want to see our boundaries

	mysubwindow = new AssemblageSubwindow(this, 320, 30, myassemblage);	//create the subwindow to hold our assemblage (vertical size irrelevant as it will be resized in a moment)
	mysubwindow->spinnable(true);	//spinnable is good for 3D visualization
	mysubwindow->transparency(false);	//transparency off because I don't need it (enabled by default)

	myfpspanel = new GlowQuickPaletteSubwindow(this, 0, 0, 320, 30, GlowQuickPalette::horizontal, GlowQuickPalette::alignCenter, 10, 0, 0);	//creates a fps panel subwindow
	_fpslabel = myfpspanel->AddLabel("fps - Not Yet Initialized             ");	//adds a label for the fps count to the fps panel
	myfpspanel->Pack();	//packs the fps panel (see Glow documentation)

	Reshape(320, 320+myfpspanel->Height());	//call the reshape function which will call our resize function and make everything the right shape

	lj6_12::setdefaultepsilon(1.0);	//set the default epsilon value for Lennard-Jones interactions

	createwalls();

	mylj6_12interact = new Interaction2B(lj6_12::forcefunction);	//create a Lennard-Jones interaction
	mycoulombicinteract = new Interaction2B(coulombic);				//create a Coulombic interaction
	myhardwall = new InteractionS(hardwall);	//create a hard wall interaction
	mygravity = new Interaction1B(gravity);		//create a gravity interaction

	myassemblage->addInteraction(mylj6_12interact);		//add all our interactions to our assemblage
	myassemblage->addInteraction(mycoulombicinteract);
	myassemblage->addInteraction(myhardwall);
	myassemblage->addInteraction(mygravity);

	//here we construct the user interface using the methods provided by Glow
	GlowQuickPanelWidget* panel1;
	GlowQuickPanelWidget* panel2;
	GlowQuickPanelWidget* panel3;
	char buffer[20];

	_setupWindow = new GlowQuickPaletteWindow("Setup");
		panel1 = _setupWindow->AddPanel(GlowQuickPanelWidget::etchedStyle, "Initialization");
		
			panel2 = panel1->AddPanel(GlowQuickPanelWidget::etchedStyle, "Type 1");
			_numberType1Text = panel2->AddTextField(60, "5", "Number");

			panel2 = panel1->AddPanel(GlowQuickPanelWidget::etchedStyle, "Type 2");
			_numberType2Text = panel2->AddTextField(60, "8", "Number");

			panel2 = panel1->AddArrangingPanel(GlowQuickPanelWidget::horizontal, GlowQuickPanelWidget::alignCenter);
			_createButton = panel2->AddPushButton("Create", this);
			_clearButton = panel2->AddPushButton("Clear", this);

		panel1 = _setupWindow->AddPanel(GlowQuickPanelWidget::etchedStyle, "Display", GlowQuickPanelWidget::vertical, GlowQuickPanelWidget::alignLeft);
			panel2 = panel1->AddArrangingPanel(GlowQuickPalette::horizontal, GlowQuickPalette::alignLeft);
				panel3 = panel2->AddPanel(GlowQuickPanelWidget::etchedStyle, "Type 1");
				_color1RadioGroup = panel3->AddRadioGroup(GlowQuickPalette::vertical, 3, this);
				visibility1 = true;
				defaultColor1[0] = 1.0; defaultColor1[1] = 0.0; defaultColor1[2] = 0.0;
				_color1redRadio = _color1RadioGroup->AddRadioButton("Red");
				_color1randRadio = _color1RadioGroup->AddRadioButton("Random");
				_color1invisRadio = _color1RadioGroup->AddRadioButton("Invisible");

				panel3 = panel2->AddPanel(GlowQuickPanelWidget::etchedStyle, "Type 2");
				_color2RadioGroup = panel3->AddRadioGroup(GlowQuickPalette::vertical, 3, this);
				visibility2 = true;
				defaultColor2[0] = 0.0; defaultColor2[1] = 1.0; defaultColor2[2] = 0.0;
				_color2greenRadio = _color2RadioGroup->AddRadioButton("Green");
				_color2randRadio = _color2RadioGroup->AddRadioButton("Random");
				_color2invisRadio = _color2RadioGroup->AddRadioButton("Invisible");

		panel1 = _setupWindow->AddPanel(GlowQuickPanelWidget::etchedStyle, "Calculations", GlowQuickPalette::vertical, GlowQuickPalette::alignLeft);

			Assemblage::setdt(0.001);
			_timestepText = panel1->AddTextField(100, "0.001", "Time Step (dt)");

			_setcalcButton = panel1->AddPushButton("Set", this);

			panel2 = panel1->AddPanel(GlowQuickPanelWidget::etchedStyle, "Move Algorithm", GlowQuickPalette::vertical, GlowQuickPalette::alignLeft);
			_movealgoRadioGroup = panel2->AddRadioGroup(GlowQuickPalette::vertical, 3, this);
			_moveStopRadio = _movealgoRadioGroup->AddRadioButton("Stopped");
			_moveSimpleRadio = _movealgoRadioGroup->AddRadioButton("Simple");
			_moveDampedRadio = _movealgoRadioGroup->AddRadioButton("Damped");
			_moveVelocityRadio = _movealgoRadioGroup->AddRadioButton("Velocity Verlet");
			_moveBeemanRadio = _movealgoRadioGroup->AddRadioButton("Beeman");

		panel1 = _setupWindow->AddArrangingPanel(GlowQuickPalette::horizontal, GlowQuickPalette::alignLeft);

			_quitPushButton = panel1->AddPushButton("Quit", this);
	_setupWindow->Pack();

	_interactWindow = new GlowQuickPaletteWindow("Interactions");
		_setinterButton = _interactWindow->AddPushButton("Set", this);
		
		panel1 = _interactWindow->AddPanel(GlowQuickPanelWidget::etchedStyle, "Mass");
			defaultMass1 = 1.0;
			_mass1Text = panel1->AddTextField(100, "1.0", "Mass 1");
			defaultMass2 = 1.0;
			_mass2Text = panel1->AddTextField(100, "1.0", "Mass 2");

		panel1 = _interactWindow->AddPanel(GlowQuickPanelWidget::etchedStyle, "Monatomic");
			panel2 = panel1->AddPanel(GlowQuickPanelWidget::etchedStyle, "Gravity");
			_gravityenabledCheckBox = panel2->AddCheckBox("Enabled", GlowCheckBoxWidget::on, this);
			panel2->AddLabel("Direction");
			gravity_down = Vector3D(0.0, -1.0, 0.0);
			gravity_down.normalize();
				panel3 = panel2->AddArrangingPanel(GlowQuickPalette::horizontal);
				sprintf(buffer, "%.2f", gravity_down.getX());
				_gravxText = panel3->AddTextField(40, buffer, "x");
				sprintf(buffer, "%.2f", gravity_down.getY());
				_gravyText = panel3->AddTextField(40, buffer, "y");
				sprintf(buffer, "%.2f", gravity_down.getZ());
				_gravzText = panel3->AddTextField(40, buffer, "z");

		panel1 = _interactWindow->AddPanel(GlowQuickPanelWidget::etchedStyle, "Interatomic");
			panel2 = panel1->AddPanel(GlowQuickPanelWidget::etchedStyle, "Leonard-Jones");
			_lj6_12enabledCheckBox = panel2->AddCheckBox("Enabled", GlowCheckBoxWidget::on, this);
			lj6_12::setepsilon(1, 1, 1.0);
			_epsilon11Text = panel2->AddTextField(100, "1.0", "epsilon 1-1");
			defaultRad1 = 0.2;
			_radius1Text = panel2->AddTextField(100, "0.2", "Radius 1");
			lj6_12::setepsilon(2, 2, 1.0);
			_epsilon22Text = panel2->AddTextField(100, "1.0", "epsilon 2-2");
			defaultRad2 = 0.1;
			_radius2Text = panel2->AddTextField(100, "0.1", "Radius 2");
			lj6_12::setepsilon(1, 2, 1.0);
			_epsilon12Text = panel2->AddTextField(100, "1.0", "epsilon 1-2");

			panel2 = panel1->AddPanel(GlowQuickPanelWidget::etchedStyle, "Coulombic");
			_coulombicenabledCheckBox = panel2->AddCheckBox("Enabled", GlowCheckBoxWidget::off, this);
			mycoulombicinteract->enabled(false);
			defaultQ1 = 1.0;
			_q1Text = panel2->AddTextField(100, "1.0", "q1");
			defaultQ2 = -1.0;
			_q2Text = panel2->AddTextField(100, "-1.0", "q2");

	_interactWindow->Pack();
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
	delete mylj6_12interact;
	delete mycoulombicinteract;
	delete myhardwall;
	delete mygravity;
	delete _setupWindow;
	delete _interactWindow;
	
	//many other objects exist but will be deleted recursively by the deletion of these objects
}

void AssemblageWindow::createwalls()
{
	//do not worry that these surfaces are not explicitly deleted anywhere
	//GLOW will delete them when the assemblage is deleted

	myassemblage->addSurface(new Plane(Vector3D(-1,-1,-1), Vector3D(1,0,0)));	//these each make a new plane and add it to the assemblage
	myassemblage->addSurface(new Plane(Vector3D(-1,-1,-1), Vector3D(0,1,0)));
	myassemblage->addSurface(new Plane(Vector3D(-1,-1,-1), Vector3D(0,0,1)));
	myassemblage->addSurface(new Plane(Vector3D(1,1,1), Vector3D(-1,0,0)));
	myassemblage->addSurface(new Plane(Vector3D(1,1,1), Vector3D(0,-1,0)));
	myassemblage->addSurface(new Plane(Vector3D(1,1,1), Vector3D(0,0,-1)));

	myassemblage->addSurface(new RectangleSurface(Vector3D(-.5,0,-.5), Vector3D(1,0,0), Vector3D(0,0,1)));	//this creates a new rectangle surface and adds it to the assemblage
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

void AssemblageWindow::OnMessage(const GlowCheckBoxMessage& message)	//on receipt of a checkbox message
{
	if(message.widget == _gravityenabledCheckBox)	//if it pertains to gravity
	{
		if(message.state == GlowCheckBoxWidget::on)	//should gravity be on
			mygravity->enabled(true);	//then make it on
		else
			mygravity->enabled(false);	//or make it off
	}

	if(message.widget == _lj6_12enabledCheckBox)	//should lj6_12 be on
	{
		if(message.state == GlowCheckBoxWidget::on)
		{
			mylj6_12interact->enabled(true);	//the Coulombic and Lennard Jones potentials are mutually exclusive
			_coulombicenabledCheckBox->SetState(GlowCheckBoxWidget::off);
			mycoulombicinteract->enabled(false);
		}
		else
			mylj6_12interact->enabled(false);
	}

	if(message.widget == _coulombicenabledCheckBox)	//should coulombic be on
	{
		if(message.state == GlowCheckBoxWidget::on)
		{
			mycoulombicinteract->enabled(true);	//the Coulombic and Lennard Jones potentials are mutually exclusive
			_lj6_12enabledCheckBox->SetState(GlowCheckBoxWidget::off);
			mylj6_12interact->enabled(false);
		}
		else
			mycoulombicinteract->enabled(false);
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

	if(message.groupWidget == _color1RadioGroup)	//change atom type 1 coloring
	{
		int i;
		Atom* temp;
		if(message.buttonWidget == _color1redRadio)	//go to red mode
		{
			if(message.oldButtonWidget == _color1redRadio)	//if we're already in red mode
				return;										//return
			if(message.oldButtonWidget == _color1invisRadio)	//if we're in invisible mode
				visibility1 = true;								//go into visible mode
			defaultColor1[0] = 1.0;	//set the color to red
			defaultColor1[1] = 0.0;
			defaultColor1[2] = 0.0;
			mysubwindow->Refresh();	//tell the subwindow to refresh
		}

		if(message.buttonWidget == _color1randRadio)	//go into random mode
		{
			if(message.oldButtonWidget == _color1randRadio)	//if we're already in random mode
				return;										//return
			if(message.oldButtonWidget == _color1invisRadio)	//if we're in invisible mode
				visibility1 = true;								//go into visible mode
			defaultColor1[0] = (double)Random::randfloat();	//set the color to random values
			defaultColor1[1] = (double)Random::randfloat();
			defaultColor1[2] = (double)Random::randfloat();
			mysubwindow->Refresh();	//tell the subwindow to refresh
		}

		if(message.buttonWidget == _color1invisRadio)	//go into invisible mode
		{
			if(message.oldButtonWidget == _color1invisRadio)	//if we're already in invisible mode
				return;											//return
			visibility1 = false;	//go into invisible mode
			mysubwindow->Refresh();	//tell the subwindow to refresh
		}

		for(i = 0; i<myassemblage->numAtoms(); ++i)	//for every atom
		{
			temp = myassemblage->getAtom(i);	//select the atom
			if(temp->getType() == 1)	//if it's type 1
			{
				temp->visibility(visibility1);	//set it's visibility
				temp->setColor(defaultColor1[0], defaultColor1[1], defaultColor1[2]);	//and color accordingly
			}
		}
	}

	if(message.groupWidget == _color2RadioGroup)	//change atom type 2 coloring
	{
		int i;
		Atom* temp;
		if(message.buttonWidget == _color2greenRadio)	//go to green mode
		{
			if(message.oldButtonWidget == _color2greenRadio)	//if we're already in green mode
				return;											//return
			if(message.oldButtonWidget == _color2invisRadio)	//if we're in invisible mode
				visibility2 = true;								//go into visible mode
			defaultColor2[0] = 0.0;	//set the color to green
			defaultColor2[1] = 1.0;
			defaultColor2[2] = 0.0;
			mysubwindow->Refresh();	//tell the subwindow to refresh
		}

		if(message.buttonWidget == _color2randRadio)	//go into random mode
		{
			if(message.oldButtonWidget == _color2randRadio)	//if we're already in random mode
				return;										//return
			if(message.oldButtonWidget == _color2invisRadio)	//if we're in invisible mode
				visibility2 = true;								//go into visible mode
			defaultColor2[0] = (double)Random::randfloat();	//set the color to random values
			defaultColor2[1] = (double)Random::randfloat();
			defaultColor2[2] = (double)Random::randfloat();
			mysubwindow->Refresh();	//tell the subwindow to refresh
		}

		if(message.buttonWidget == _color2invisRadio)	//go into invisible mode
		{
			if(message.oldButtonWidget == _color2invisRadio)	//if we're already in invisible mode
				return;											//return
			visibility2 = false;	//go into invisible mode
			mysubwindow->Refresh();	//tell the subwindow to refresh
		}

		for(i = 0; i<myassemblage->numAtoms(); ++i)	//for every atom
		{
			temp = myassemblage->getAtom(i);	//select the atom
			if(temp->getType() == 2)	//if it's type 2
			{
				temp->visibility(visibility2);	//set it's visibility
				temp->setColor(defaultColor2[0], defaultColor2[1], defaultColor2[2]);	//and color accordingly
			}
		}
	}
}

void AssemblageWindow::OnMessage(const GlowPushButtonMessage& message)	//on receipt of a push button message
{
	if(message.widget == _quitPushButton)	//if it was the quit push button
	{
		exit(0);	//then quit
	}

	if(message.widget == _createButton)	//create atoms and add them to the assemblage
	{
		int i;
		int numType1;
		int numType2;
		Atom* temp;

		numType1 = mymax(0,atoi(_numberType1Text->GetTextChars()));	//number of atoms of type 1 to create from user interface
		numType2 = mymax(0,atoi(_numberType2Text->GetTextChars()));	//number of atoms of type 2 to create from user interface

		char* buffer = new char[20];		//just make sure
		sprintf(buffer, "%d", numType1);	//that the ui
		_numberType1Text->SetText(buffer);	//agrees with our
		sprintf(buffer, "%d", numType2);	//internals, in case
		_numberType2Text->SetText(buffer);	//the input was bad
		delete buffer;

		for(i=0; i<numType1; ++i)	//once for each new type 1
		{
			temp = new Atom(1, Random::randvect(myassemblage->getMin()+Vector3D(defaultRad1), myassemblage->getMax()-Vector3D(defaultRad1)), Vector3D(0.0), defaultRad1, defaultMass1, defaultQ1, defaultColor1[0], defaultColor1[1], defaultColor1[2]);	//create an atom with the type 1 defaults
			temp->visibility(visibility1);	//set it's visibility to the type 1 default
			myassemblage->addAtom(temp);	//add it to the assemblage
		}
		
		for(i=0; i<numType2; ++i)	//once for each new type 2
		{
			temp = new Atom(2, Random::randvect(myassemblage->getMin()+Vector3D(defaultRad2), myassemblage->getMax()-Vector3D(defaultRad2)), Vector3D(0.0), defaultRad2, defaultMass2, defaultQ2, defaultColor2[0], defaultColor2[1], defaultColor2[2]);	//create an atom with the type 2 defaults
			temp->visibility(visibility2);	//set it's visibility to the type 2 default
			myassemblage->addAtom(temp);	//add it to the assemblage
		}

		fprintf(stdout, "Created %d Atoms of Type 1 and %d Atoms of Type 2\n", numType1, numType2);	//output to console the numbers created
		fprintf(stdout, "\t%d Atoms now exist\n", myassemblage->numAtoms());	//out put to console the total number of existing atoms

		mysubwindow->Refresh();	//request a refresh
	}

	if(message.widget == _clearButton)	//rmove all atoms
	{
		int numdeleted = myassemblage->removeAllAtoms();	//keep track of how many atoms are deleted
		fprintf(stdout, "%d Atoms destroyed\n", numdeleted);	//output to console how many atoms are deleted
		mysubwindow->Refresh();	//request a refresh
	}

	if(message.widget == _setcalcButton)	//set the calculation options
	{
		double temp = atof(_timestepText->GetTextChars());	//new value for dt

		if(temp <= 0.0)	//if the new dt value is less than 0
		{
			new GlowMessageWindow("Illegal Value!", GlowWindow::autoPosition, GlowWindow::autoPosition, "You have input an illegal value!", "OK", NULL);	//yell at the user
		}
		else	//otherwise
		{
			Assemblage::setdt(temp);	//change the value
		}

		char* buffer = new char[20];		//update the user interface to match the internals
		sprintf(buffer, "%f", Assemblage::getdt());
		_timestepText->SetText(buffer);
		delete buffer;
	}

	if(message.widget == _setinterButton)	//set interaction conditions
	{
		double mass1 = atof(_mass1Text->GetTextChars());	//get new mass values
		double mass2 = atof(_mass2Text->GetTextChars());

		Vector3D gravdir = Vector3D(atof(_gravxText->GetTextChars()), atof(_gravyText->GetTextChars()), atof(_gravzText->GetTextChars()));	//get new gravity direction

		double epsilon11 = atof(_epsilon11Text->GetTextChars());	//new values for the Lennard-Jones potential
		double epsilon22 = atof(_epsilon22Text->GetTextChars());
		double epsilon12 = atof(_epsilon12Text->GetTextChars());

		double rad1 = atof(_radius1Text->GetTextChars());	//new radii
		double rad2 = atof(_radius2Text->GetTextChars());

		double q1 = atof(_q1Text->GetTextChars());	//new charge values
		double q2 = atof(_q2Text->GetTextChars());

		if(mass1 <= 0.0 || mass2 <= 0.0 ||	//if we have negative mass values
			rad1 <= 0.0 || rad2 <= 0.0 ||	//or negative radii
			epsilon11 < 0.0 || epsilon22 < 0.0 || epsilon12 < 0.0 ||	//or negative epsilon values
			gravdir.magnitudesq() == 0.0)	//or a zero gravity direction
		{
			new GlowMessageWindow("Illegal Value!", GlowWindow::autoPosition, GlowWindow::autoPosition, "You have input an illegal value!", "OK", NULL);	//so yell at the user
		}
		else	//or
		{
			defaultMass1 = mass1;	//change the mass values
			defaultMass2 = mass2;

			lj6_12::setepsilon(1, 1, epsilon11);	//change the epsilon values
			lj6_12::setepsilon(2, 2, epsilon22);
			lj6_12::setepsilon(1, 2, epsilon12);

			defaultRad1 = rad1;	//change the radii
			defaultRad2 = rad2;

			gravity_down = gravdir.normalvector();	//change the gravity direction (to the normalized value of the new one
		}

		defaultQ1 = q1;	//change the charge
		defaultQ2 = q2;


		char* buffer = new char[20];	//update the user interface to match the internals
		
		sprintf(buffer, "%f", defaultMass1);
		_mass1Text->SetText(buffer);
		sprintf(buffer, "%f", defaultMass2);
		_mass2Text->SetText(buffer);

		sprintf(buffer, "%.2f", gravity_down.getX());
		_gravxText->SetText(buffer);
		sprintf(buffer, "%.2f", gravity_down.getY());
		_gravyText->SetText(buffer);
		sprintf(buffer, "%.2f", gravity_down.getZ());
		_gravzText->SetText(buffer);

		sprintf(buffer, "%f", lj6_12::getepsilon(1, 1));
		_epsilon11Text->SetText(buffer);
		sprintf(buffer, "%f", defaultRad1);
		_radius1Text->SetText(buffer);
		sprintf(buffer, "%f", lj6_12::getepsilon(2, 2));
		_epsilon22Text->SetText(buffer);
		sprintf(buffer, "%f", defaultRad2);
		_radius2Text->SetText(buffer);
		sprintf(buffer, "%f", lj6_12::getepsilon(1, 2));
		_epsilon12Text->SetText(buffer);

		sprintf(buffer, "%f", defaultQ1);
		_q1Text->SetText(buffer);
		sprintf(buffer, "%f", defaultQ2);
		_q2Text->SetText(buffer);

		delete buffer;

		Atom* tempAtom;

		for(int i=0; i<myassemblage->numAtoms(); ++i)	//for each atom in the assemblage
		{
			tempAtom = myassemblage->getAtom(i);	//select the atom
			switch(tempAtom->getType())		//figure out its type
			{
			case 1:		//if type 1
				tempAtom->setMass(defaultMass1);	//give it the new type 1 values
				tempAtom->setRadius(defaultRad1);
				tempAtom->setCharge(defaultQ1);
				break;
			case 2:		//if type 2
				tempAtom->setMass(defaultMass2);	//give it the new type 2 values
				tempAtom->setRadius(defaultRad2);
				tempAtom->setCharge(defaultQ2);
				break;
			}
		}

		mysubwindow->Refresh();	//request a refresh
	}
}









int main(int argc, char** argv)	//int main()
{
	fprintf(stdout, "demo - a GAST-Viz application\n");		//print out various bits of header information
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
