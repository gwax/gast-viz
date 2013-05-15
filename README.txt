
The Generalized Atomistic Simulation Toolkit for Visualization
GAST-Viz

version 1.0.1 (11 March 2004)
Copyright (c)2003-2004, George Waksman <waksman@mit.edu>. All rights reserved.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


=================================================
0 - Contents
=================================================
0     -  Contents

1     -  General Information
1.1   -  What is GAST-Viz
1.2   -  Copyright/Licensing Information
1.3   -  Acknowledgements

2     -  Release Information
2.1   -  Release Notes
2.2   -  Known Issues

3     -  Usage Information
3.1   -  Platform Compatibility
3.2   -  Install Information
3.3   -  Included Sample Applications
3.3.1 -  demo
3.3.2 -  stack2d
3.3.3 -  stack3d
3.3.4 -  template

4     -  Writing Your Own Applications
4.1   -  The GLOW - GAST-Viz Connection
4.2   -  Compiling Your Own Applications
4.3   -  template.cpp: A Walkthrough
4.3.1 -  Headers
4.3.2 -  AssemblageWindow class Components
4.3.3 -  AssemblageWindow constructor
4.3.4 -  AssemblageWindow Destructor
4.3.5 -  AssemblageWindow OnReshape
4.3.6 -  AssemblageWindow OnMessage(GlowIdleMessage)
4.3.7 -  AssemblageWindow OnMessage(GlowRadioButtonMessage)
4.3.8 -  AssemblageWindow OnMessage(GlowPushButtonMessage)
4.3.9 -  main()
4.4   -  Reference and Quirks
4.4.1 -  class Vector3D
4.4.2 -  The Assemblage class
4.4.3 -  The AssemblageSubwindow class
4.4.4 -  Atom and its superclasses
4.4.5 -  class Surface, Plane and RectangleSurface
4.4.6 -  struct interact and the Interaction classes
4.4.7 -  interactionlibrary and the lj6_12 static class
4.4.8 -  class myfpscounter, class Random and functions.inl.h


=================================================
1 - General Information
=================================================
1.1 - What is GAST-Viz
-------------------------------------------------
	The Generalized Atomistic Simulation Toolkit for Visualization
(GAST-Viz) is very much what its name suggests, a toolkit that will
allow for the creation of atomistic simulation using discrete
element modeling methods. In order to allow for maximum
accessability, the code uses GLOW (a freely available UI library)
and is built on GLUT (a multiplatform system to allow for higher
level access to OpenGL); further details on GLOW and GLUT are in
section 3.2 - Install Information.
	GAST-Viz is not designed to be used for heavy duty research
simulation as it is single-threaded, shortcuts have been taken to
optimize for small systems and there is a large amount of overhead
introduced by the graphics system. That having been said, GAST-Viz
is designed to be viewable and modifiable in real-time using OpenGL
as a graphics engine. The real-time nature of GAST-Viz makes it
ideal for demonstrating various concepts of materials science and
other fields or as an introduction to atomistic simulation.
	Beyond that, the sample applications are just fun to play
with, most notably stack2d and stack3d.

1.2 - Copyright Information
-------------------------------------------------
	As should be pretty clear from the copyright notices that
exist on all of the files, I, George Waksman, am the copyright
holder for all of the code and associated documentation involved
with GAST-Viz. As of version 1.0.1, all coding was done by myself.
	That having been said, I have decided to license GAST-Viz
using the GNU General Public License (GPL) version 2 or later,
which should be included with GAST-Viz itself and should be
easily available on the World Wide Web. This pretty much means
that you are free to do nearly whatever you want with GAST-Viz as
long as any product that includes it is also available as source;
for specifics, read through the GPL itself. Beyond that, I would
love to hear from anyone that uses GAST-Viz or develops anything
based on GAST-Viz.
	Also, this software is provided WITHOUT ANY WARRANTY to
the full extent possible; see GPL for more details.

George Waksman
Massachusetts Institute of Technology, Building NW-61
290 Mass Ave.
Cambridge, MA 02139
USA
waksman@mit.edu

1.3 - Acknowledgements
-------------------------------------------------
	Credit is due to all those that supported me in the
development of this software package. The work was done under
the advising of Professor W. Craig Carter and the Labratory for
Computation Simulation of Microstructures, Microstructural
Evolution, and Materials Properties. Funding for the work was
provided by the Massachusetts Institute of Technology through
the Undergraduate Research Opportunities Program (UROP).


=================================================
2 - Release Information
=================================================
2.1 - Release Notes
-------------------------------------------------
version 1.0.1
-------------------------------------------------
It occured to me that I could include glow in my release because of
the way in which it is licensed and have done so, GAST-Viz should
now compile right after downloading

version 1.0.0
-------------------------------------------------
Initial release, everything complete and functional

2.2 - Known Issues
-------------------------------------------------
General Issues:
	GAST-Viz uses dimensionally insignificant units. A lot of
constants work and calculations will be needed to get quanitative
information out. I have already failed to do so more than once and
I expect it would require a rather thorough overhall of everything
in order to make this reasonably possible. In short, use GAST-Viz
qualitatively or do a lot of work on your own.
	The temperature methodolgy needs a little bit of work. It's
pretty good right now, but it doesn't work well if you have any
atoms that manage to escape and start flying off towards infinity.
	Surfaces can not be removed from an Assemblage or disabled;
workarounds can be written and this feature may be added to a
future version of GAST-Viz.
	The stereographic feature has not been tested and may not
work at all. I do not have the necessary hardware. If anyone else
manages to test this feature and find that it does or does not
work, I would love to hear about it. If it does not work and you
feel ambitious, feel free to fix it/make it work and please let
me know, I will credit you and integrate it into a future
version (if possible).
	The Random class uses modulo arithmetic in some places, this
can slightly decrease pseudorandomness.
	Atoms that are removed from an assemblage are not removed
from memory immediately, instead one must wait up to one full
cycle before the memory will be freed. This is because they are
waiting for Glow to release them of responsibilities before
getting deleted. This is a Glow issue and is neither important
nor will it be fixed.

Interaction Library Issues:
	The coulombic interaction does not work very and needs to be
fixed somehow. This may be as simple as figuring out a value for
Kcoulomb that works with the arbitrary unit system of GAST-Viz,
but probably also requires a rethinking of the way the repulsive
term is handled.
	The hardsphere and hardwall potentials are minimally
attractive at very short distances. This should not really be an
issue in most cases and can probably be ignored for all intents
and purposes.

Application Issues:
	Jiggle should not apply to the atoms at the ends of the
stacks in the applications stack2d and stack3d, but it does.


=================================================
3 - Usage Information
=================================================
3.1 - Platform Compatibility
-------------------------------------------------
	GAST-Viz should theoretically work on any platform that can run
GLOW. GLOW can theoretically run on any platform that can compile C++
code and can run GLUT. GLUT runs on essentially every platform with a
decent OpenGL implementation. So in theory, GAST-Viz should be able
to run on any machine with a decent OpenGL implementation and a C++
compiler.
	That having been said, GAST-Viz has been tested on Windows 2k/XP
using Microsoft Visual Studio .NET as a compiler on Linux using the GNU
C++ compiler and on MacOS X using the GNU C++ compiler as well. These
are the only platforms for which projects and makefiles exist, so if you
manage to run GAST-Viz on another platform please let me know and provide
me with the makefiles or projects that you used. I can probably provide
some assistance in making GAST-Viz work with earlier versions of Visual
Studio, but I am not very good with makefiles or *nix so I can't help
much there.

3.2 - Install Information
-------------------------------------------------
Installation is a simple process as it's mainly a matter of getting
your system ready to compile GAST-Viz and then doing so. If you have
a working C++ compiler then you are most of the way there. It is
likely that I will make some precompiled binaries available for some
of the more popular systems (Win32 very likely, Debian packages
possible, etc. we'll see).

If you have gotten you hands on some precompiled binaries the only
thing that you will need to do is obtain and install a copy of the
GLUT binaries.

If you are planning to build from source then you will need to get
a working copy of GLUT installed with the libraries and headers
necessary to link to GLUT. All of this is available either as
binaries or by building GLUT yourself. I will leave this to you as
it is mostly platform dependent. (Note: as of the writing of this
document there is a slight problem with GLUT and .NET; the solution
is easily found by typing "glut visual studio .net" into Google)

GLUT can be obtained from http://www.opengl.org/resources/libraries/glut.html
and a number of other places on the Internet.

All that is left now is to compile and run the sample applications.
To compile the applications under Visual Studio .NET simply open
the included solution file and build. To compile the applications
under Linux, enter the subdirectory of the specific application that
you would like to build and then "make". OSX links a little bit
differently from Linux and I have made makefiles to work around this
issue (Makefile.OSX). For information on compiling your own
applications, see section 4.2 - Compiling Your Own Applications.

3.3 - Included Sample Applications
-------------------------------------------------
Detail levels can be manipulated from the right click menu of the
main application window of all included sample applications. Also
some of the sample applications (demo, stack3d, template) can be
rotated by dragging with the left mouse button in the main
application window.

3.3.1 - demo
-------------------------------------------------
This application serves mostly to showcase all of the different
features of GAST-Viz. The application itself is very feature rich
and fairly self explanatory so I will not go into great depth as
to the functionality of it except insofar as to suggest that the
Coulombic option be avoided for reasons mentioned in the section
2.2 - Known Issues. Feel free to play all you want, I'm just not
going to document it.

3.3.2 - stack2d
-------------------------------------------------
The premise of this application is very simple: you have a two
dimensional close packed stack of metal atoms that interact
and you can control various parameters of the stack.

The controls are fairly self explanatory, and the supplied
values are pretty good for most things. Strain Rate (y-axis)
is normal and (x-axis) is shear, I reccomend values between
0.1 and -0.1 or even 0.01 and -0.01. Strain Rate works by
manually moving the atoms at the extremes of the stack and
preventing them from being affected by the forces of the
other atoms in the stack. I reccomend leaving Jiggle at 0
unless you really want to play with it. Velocity Verlet is
often the best move algorithm, although Beeman may be better
in some ways; Damped can be nice for some effects and Simple
is really best for showing how bad Simple is.

3.3.3 - stack3d
-------------------------------------------------
This application is the logical extension of stack2d into three
dimensions. The notable differences are that now the stack is
a face centered cubic arrangement of atoms and you can choose
to initialize the stack in the <100>, <110> or <111>
orientations and that you can also shear strain along the
z-axis.

stack3d is a lot of fun to play with if your computer can
maintain a decent frame rate. Some things that I have
discovered in playing with it are that the non-real metal melts
at a temperature of around 0.5 or 0.6 and boils somewhere in
the range of 1.0 and 2.0 temperatures. Lots of fun can be had
melting and freezing this block of metal as well as doing all
sorts of other things to it.

3.3.4 - template
-------------------------------------------------
This is about the simplest complete application that I could come
up with. It exists to simplify explaining how to write your own
applications.


=================================================
4 - Writing Your Own Applications
=================================================
4.1 - The GLOW - GAST-Viz Connection
-------------------------------------------------
Since GAST-Viz is dependent on GLOW for all of its user interface
and drawing hierarchy, one would do well to look through the GLOW
documentation before trying to do anything with GAST-Viz or you
will have a hard time making your program very interactive or 
following much of what is going on with the drawing of things.

4.2 - Compiling Your Own Applications
-------------------------------------------------
I will detail how to compiler your own applications under Linux
and Visual Studio .NET, for other platforms you are on your own.

Linux:
Compiling your own applications under Linux is quite easy. Put
the source for your application in the src directory of GAST-Viz
and then steal a Makefile from demo, stack2d, stack3d or template.
You will want to put your Makefile in a new subdirectory of
GAST-Viz or you will have to modify it more thoroughly. Open your
Makefile and make the following changes:

line	change
3		change the comment to your program name
6		change the output name to your program name
12-23	remove whichever .cpp file you do not need and add your own
		(ex. remove template and add myprog)

Visual Studio .NET:
You will need to create a new project, it is easiest if you add this
project to the provided GAST-Viz solution. You should put your source
files in the GAST-Viz\src directory. Add all of the needed files in
the src directory to your project (probably everything except
demo.cpp, stack2d.cpp, stack3d.cpp and template.cpp) You will then
need to make a number of modifications to your project. First set your
project to depend on the glow project (Project->Project Dependencies).
Now you will need to change a number of the properties of your
project (all of which can be changed from Project->Properties):

C/C++
	General
		Additional Include Directories
			$(SolutionDir)\glow\glow_src
	Preprocessor
		Preprocessor Definitions
			GLOW_COMPAT_NOTEMPLATESPECIALIZATIONS
			GLOW_COMPAT_BADFORSCOPING
			GLOW_COMPAT_CLIBNOSTDNAMESPACE
	Language
		Enable Run-Time Type Info
			Yes (/GR)
Linker
	Input
		Additional Dependencies
			opengl32.lib glut32.lib glu32.lib
	Advanced
		Import Library
			$(SolutionDir)\glow\$(OutDir)\glow.lib


4.3 - template.cpp: A Walkthrough
-------------------------------------------------
	The application template (template.cpp) is provided as a very
simple, but complete, application built on the GAST-Viz framework.
In this section, I will go through all the different parts of
template.cpp so as to help explain the core components of a
GAST-Viz application. For those starting out with GAST-Viz that
would like to write their own applications I reccommend following
along as I go through template.cpp and then taking a short look at
some of the other, more complex, included applications.
	A reference section is included in section 4.4 detailing most
everything else that you will need to know about GAST-Viz.

4.3.1 - Headers
-------------------------------------------------
A number of header files are included in template, and they can
be broken up into two sections. First are the GLOW and system
header files, here you will want to put headers for the various
GLOW modules that you are using and anything else that might be
necessary. Following these headers is the command
GLOW_NAMESPACE_USING which is a macro defined by GLOW to switch to
the proper namespace. Following the switch to the GLOW namespace
are the various GAST-Viz headers; these must be included after
GLOW_NAMESPACE_USING as many of them require the GLOW namespace to
operate.

4.3.2 - AssemblageWindow class Components
-------------------------------------------------
	The AssemblageWindow inherits from a number of GLOW components;
these are so as to handle the drawing and user interface of the
window itself.
	The only public functions that we need to provide are the
constructor and the destructor as these are the only external
operations we will ever perform on the AssemblageWindow. Following
are a number of protected functions inheritted from the various
GLOW components; these handle user interface interaction. After
the protected functions are references to all the private data that
the AssemblageWindow needs to keep track of; this includes
references to user interface components and all of the GAST-Viz
components contained by the AssemblageWindow. For information on
the user interface, see the GLOW documentation.

4.3.3 - AssemblageWindow constructor
-------------------------------------------------
	The AssemblageWindow constructor makes the important things and
sets up the user interface. The first thing that we need to do is
to call the superclass GlowWindow constructor with the appropriate
options; this will actually create our window for us. Now that all
precursor bits of the constructor are complete we can get into the
constructor itself.
	First we create our Assemblage and set a few options to non-
default values; this is important as the Assemblage class does most
of the real work (see reference for more details). Next we must
create an AssemblageSubwindow and bind it to the AssemblageWindow;
this is a subwindow entity that is used to handle orientation,
drawing and some other details of the Assemblage class; this is
absolutely necessary even if you do not intend to have other
subwindows. Following this we create an fps counter so that we have
one to use, and then we make a little status panel in our window so
that we have somewhere to display the fps and anything else we may
want (the stack applications use this to display temperature as well
as fps). Since we want our Atoms to be able to interact we create a
two body ineraction (using a Lennard-Jones force function) and then
add it to the Assemblage.
	Now that our GAST-Viz items have been created we can build a
user interface. To create the user interface, I have used a 
GlowQuickPaletteWindow as it is the easiest way to make a function
user interface; you may use whatever GLOW or other facilities you
may choose.
	Finally, we must register some idle receivers so that GLOW know
to tell things to operate once every cycle. The AssemblageWindow
itself must receive idles so that it can do fps calculations and be
redrawn. The AssemblageSubwindow must be registered so that it can
be redrawn and tell the Assemblage to perform calculations; an
Assemblage will not do things on its own and needs an
AssemblageSubwindow to direct it.

4.3.4 - AssemblageWindow Destructor
-------------------------------------------------
The AssemblageWindow destructor serves to delete and deallocate all
of the objects and such that were created in the AssemblageWindow
constructor. Please note that you should really only worry about
top level GLOW objects as other GLOW objects will be deleted by
GLOW itself when it deletes the parent object.

4.3.5 - AssemblageWindow OnReshape
-------------------------------------------------
The OnReshape function should be fairly easy to follow as its
operation is fairly simple. This function is called by GLOW whenever
the size of the AssemblageWindow is changed and it resizes and moves
the subcomponents of AssemblageWindow while enforcing a size minimum
of 80 pixels x 80 pixels on the main subwindow.

4.3.6 - AssemblageWindow OnMessage(GlowIdleMessage)
-------------------------------------------------
The AssemblageWindow is registered as an idle receiver and as such
will receive idle messages at least once per operating cycle (this
is because all operations and redrawing elsewhere is done use the
GLOW idle system as well). In template, this is used to add another
frame to the fps counter and if a specified amount of time (0.25
seconds) has passed, the displayed fps count is updated. This uses
the fpscounter class to handle fps calculation details.

4.3.7 - AssemblageWindow OnMessage(GlowRadioButtonMessage)
-------------------------------------------------
This function handles radio button messages that are passed in by
GLOW. There is on radio group in template and it handles movement
algorithm selection. This is done using nested if statements to
determine which radio group was changed and which radio button was
selected. After the correct button has been identified, the
AssemblageSubwindow mysubwindow is told to change the movement
algorithm that it is using to the newly selected one.

4.3.8 - AssemblageWindow OnMessage(GlowPushButtonMessage)
-------------------------------------------------
This function handles push button messages from GLOW. The template
application has two push button controls: Quit and Initialize;
both will be described.

The Quit button calls exit(0), which is a perfectly acceptable way
to end a GLOW (or GAST-Viz) application.

The Initialize button does a number of things. First, the Assemblage
myassemblage is checked to see if it contains any Atoms, if it does,
it is asked to remove any Atoms that it contains. Secondly a number
of Atoms are created and added to myassemblage (see the reference
for the specifics of the Atom constructor). It is not necessary to
keep track of any of these Atoms as the Assemblage will do that for
us. One can make the Atom creation method as simple or complex  as
one likes and one does not have to clear all old Atoms beforehand.
For further examples of Atom creation, look at the other included
sample applications. After the Atoms have been created we spit out
some information to the console, this is just to be as informative
as possible. After this, we force the Assemblage to do one set of
calculations, while not strictly necessary, behavior may be odd or
delayed briefly if this is not included; also energy based coloring
will be inaccurate until one full set of calculations has been
performed. To perform one set of calculations, first tell the
Assemblage to calculate neighbors, this prevents any delay in
operation. Secondly, tell the Assemblage to have everything
interact, this calculates the future forces and potentials of each
Atom. Following this, we go Atom by Atom and call the setforcetonew
function twice; the first call gives each Atom the force and
potential information that was just calculated and the second call
clears the Atom's internal buffers (details in the Atom reference).
Subsequently, we tell the Assemblage that when it draws the Atoms it
should set their color based on what their potential energy is; a
range of -4.0 to 0.0 works well in the template application but
will not work well elsewhere (I usually just play around with the
values until I find ones that work well). And, finally, we tell the
AssemblageSubwindow that it should be refreshed when it gets a
chance.

4.3.9 - main()
-------------------------------------------------
The main function is where everything is put into motion. As a
result of GLOW handling most things, main is fairly thin, but no
less important.

The fprintf statements exist to push information to the console;
in this case it is mostly useless information (save the copyright
and license bit), but you can use the console for whatever sort of
output you please.

The important bits are as follow. First we initialize GLOW with all
of the parameters that have been passed to our application; GLOW
requires us to do this. Secondly, the Random class is seeded; this
need not be done with the timeseed and may simply use an integer if
you would like Random operation to be consistent and
quasideterministic. We then create our AssemblageWindow so that it
will do its thing when we enter the GLOW loop. Then we enter the
GLOW main loop, which will do everything else for us. The return 0
exists to make our compilers happy, but will never be reached in
execution.

4.4 - Reference and Quirks
-------------------------------------------------
	Following is a reference to the important classes of GAST-Viz.
If a class is not included here in the reference then you do not
need to worry about it or understand its operation to use
GAST-Viz; some classes are just used internally by other
components.
	I have made a concerted effort to thoroughly comment things,
especially the header files for all of the various bits of
GAST-Viz and as such you should not hesitate to look at the various
header files associated with classes.

4.4.1 - class Vector3D
-------------------------------------------------
	Vector3D is my personal three dimensional vector class. It was
written so that I could have a vector class with all the
functionality that I might need as well as the ability to optimize
for my specific needs. Everything should be very intuitive for
anyone that knows about vectors.
	The division and normalization functions do perform checks to
prevent the occurance of divided by zero errors, but I make no
guarantees that the results will make sense if a divide by zero
occurs.

4.4.2 - The Assemblage class
-------------------------------------------------
	Assemblage is a great big container for things; it holds them,
keeps track of them and handles their various interactions. Much
of the power of GAST-Viz is contained in the Assemblage class.
There are two forms of the Assemblage constructor; a generic one
and one that lets you set its size. If you set the size of an
Assemblage, it will always scale to that size when drawing;
otherwise it will try to guess how big things should be drawn.
Subsequently there are a number of functions to allow for the
changing of the size and drawing style. 
	Following are a number of functions for the addition, removal
and accessing of Atoms to, from and within the Assemblage; an
Atom is not initialized until it is added to an Assemblage and is
deinitialized and deleted if it is removed. There are also
functions for the addition of surfaces (generalized, see surfaces
below for details); surfaces may not be removed. Functions are
then provided to allow for the addition of interactions to an
Assemblage. Interactions may be disabled but not removed; all
enabled interactions will be performed everytime calculations are
done.
	Time stepping parameters are handled through the getdt and
setdt functions which allow you to set the value of dt for
use in numerical integration or find out what it is currently
set to. If you have an application with multiple Assemblages,
this will change the value for all Assemblages. dt defaults to
a value of 0.01.
	Next are a few functions pertaining to whether or not the
Assemblage has reached equilibrium and how fast it is assumed to
do so. This is fairly arbitrary and mainly serves to tell you how
long it has been since the system was last changed.
	Following are the jiggle methods; these are used to tell the
system how much to jiggle Atoms every time step and in which
directions. jigglemask defaults to true in all three directions
and jiggle defaults to 0.0. For the most part, I reccommend
against the use of jiggle as it introduces randomness into the
system in a mostly meaningless manner.
	Following are the nuerical integration algorithms. By calling
one of these, the whole system can be advanced one time step with
all of the forces as calculated so far. You will usually not need
to deal with these functions as they will be called in the idle
loop during AssemblageSubwindow operation.
	The next two functions let you set the way Atoms are colored
when they are drawn. Atoms can either be drawn in their native
color or based on their potential energy. If to be drawn by their
potential energy, you must specify bounds and the Atoms will be
drawn in a red-blue color gradient between those two energy
values.
	Cutoff options are provided in order to greatly speed up
calculatations. This is handled through the use of Atom neighbor
lists. Neighbor lists are recalculated periodically and the
interval may be changed; default is once every 15 time steps. All
Atoms within the cutoff distance of another are considered to be
neighbors. An Atom will interact with all of its neighbors and no
other Atoms.
	Then there are some temperature related functions, these are
used to read and control the average kinetic energy of the system.
When dealing with temperature, remember that GAST-Viz does not
use dimensionally significant units.
	The rest of the details may be ignored and considered to be
internals.

4.4.3 - The AssemblageSubwindow class
-------------------------------------------------
	As has been mentioned, the AssemblageSubwindow is a container
for Assemblages and handles some drawing and bookkeeping related
issues.
	The constructor is simple with the exception of the stereo
option, which defaults to false. The stereo option allows the
system to be drawn in stereographic projection if you have
hardware that supports it.
	The get and set MoveType functions allow you to tell the
AssemblageSubwindow which move algorithms it should have its
Assemblage use every time it is passed an idle message;
moveStopped is the default and is a dummy algorithm that causes
this step to be ignored.
	The spinnable option tells an AssemblageSubwindow whether it
is to allow the user to rotate the view using the left click and
drag interface.
	The transparency option tells the AssemblageSubwindow whether
or not it is supposed to enable alpha blending. Alpha blending can
be a very nice effect and some developers might want to include it,
but it is very system intensive and is disabled by default.

4.4.4 - Atom and its superclasses
-------------------------------------------------
	Atoms derive from Particles derive from SphereComponents
derive from GlowComponents. An Atom can do everything that any of
its superclasses can do. As far as constructors are concerned, you
can probably ignore everything except for Atom. In fact, since
the majority of operation is handled elsewhere, it is a pretty
safe bet to say that you would be safe by looking at the Atom
constructor, reading the fourth paragraph of this section and then
ignoring the rest of it.
	A SphereComponent is simply a sphere that has a position and
some graphical information and knows how to draw itself. You can
get and set the position and radius of a SphereComponent as well
as translate it by a vector. You may also change the detail level
of all SphereComponents (this is also done through the right click
menu of AssemblageSubwindow). There are a number of functions that
allow you to change the color of a SphereComponent and other bits
of the way it is drawn. You may also set a SphereComponent to skip
its drawing (visibility(false)).
	A Particle is a SphereComponent that also has velocity and
aome routines associated therewith. You can get, set, scale or
impulse the velocity of a Particle. Additionally, the heart of the
various movement algorithms exist in the Particle class. These take
an acceleration and a time step and perform numerical integration.
The time step functions should be called by other classes and are
best ignored in this context.
	An Atom is a Particle with a bunch of physical information
included. The contructor is fairly strict and fairly self
explanatory being physical, with two exceptions: the color values
and the type. The color values allow one to se the default color of
the Atom both as the initial SphereComponent color and as the color
that will be returned to if energycoloring is turned off. The type
of an Atom is not something that is used internally, but may be
used by a programmer to easily distinguish between different sets
of Atoms in operation (for possible uses, see demo, stack2d and
stack3d).
	Mass and charge should be very self-explanatory.
	The treatment of force and potential in the Atom class needs
some small amount of explanation as it is not done in a normal
manner. Both are doublebuffered and you can not directly access
either of them. The buffer may be accessed, but only for
incremental changes; you can add to the force or potential buffer
but now set them arbitrarily. After the buffers have been
sufficiently incremented, you can set the actual value to the
value in the buffer; this clears the buffers.
	You will note that the move algorithm functions are present
here as well, but all they do is call the Particle functions with
a provided value for dt and acceleration caluclated as force/mass.

4.4.5 - class Surface, Plane and RectangleSurface
-------------------------------------------------
	The pure virtual Surface class and its derivative exist toallow
you to have your Atoms interact with larger, non-round things.
Currently two types of surface exist: Planes and RectangleSurfaces.
Planes are infinite, two-dimensional flat objects and
RectangleSurfaces are finite, flate, two-dimentional objects with
four sides and four right angle corners.
	Surface is a pure virtual class that exists to be derived from.
This system allows for surfaces to be as generalized as possible. A
Surface has very few important functions, namel two aside from the
constructor and destructor. The first of these lets you query and
control the visibility of a surface and the other is the really
important one. This second function pathtopoint gives the shortest
path from the surface to a given vector position.
	Planes are created in one of two ways, either as a point and
two directions that lie in the plane or as a point and a direction
that is normal to the Plane. Trying to feed strange values to the
constructors will give strange results, reasonable values will give
reasonable results.
	RectangleSurface takes as contructor arguments an origin and
two vectors for its sides. The vectors for its sides should be
orthogonal, if they are not, things will work, but the results will
be strange. RectangleSurface is more system intensive than Plane,
but not by a whole lot and its calculations are the result of some
rather elegant linear algebra.

4.4.6 - struct interact and the Interaction classes
-------------------------------------------------
	Interactions are container and utility classes that are used to
contain functions for calculating the interactions between various
things. Interaction1B, Interaction2B and Interaction3B handle
(inter)atomic potentials for one body, two body and three body
interactions and InteractionS handles the interactions between Atoms
and surfaces. The constructor takes a pointer to an appropriate
force function as an argument.
	The interact structure (struct interact) is important as it
is the return value for all functions that can be used for
interactions. There are a number of odd quirks to struct interact.
The first such quirk is that it contains only pointers, these must
be allocated by the force function but should not be deleted; the
Interaction* classes will internally deallocate the pointers of
a struct interact. The second quirk of struct interact is that if
a force function give NULL pointers to struct interact that given
value will be ignored; NULL pointers are equivalent to zeros or
zero vectors.
	Everything else with relation to these items can be ignored as
it is handled internally by other classes.

4.4.7 - interactionlibrary and the lj6_12 static class
-------------------------------------------------
	A few force functions are provided so as to allow for the
construction of the sample programs and to provide examples for
developers that would like to make their own.
	The interactionlibrary files contain some sample one and two
body interactions, they are well commented. Some more advanced
interaction functions are present in the stack2d and stack3d
applications.
	The lj6_12 class provides a generalized Lennard-Jones potential
function for use in simulation. The lj6_12 class contains only
static components because a static class function is the same as
a general function but not a non-static class function is not. If
lj6_12 were not static we would not be able to use one of its
memeber functions as a force function in an Interaction2B. The
lj6_12 class serves as an example of the possible uses of an Atoms
type information. You can provide the lj6_12 class with epsilon
values for the interaction between any two Atoms based on their
type (this is symmetric), but if the lj6_12 class has not been told
about two specific types of Atoms it will just use its default value
for epsilon. The default value of epsilon can be changed as well and
defaults to 1.0.

4.4.8 - class fpscounter, class Random and functions.inl.h
-------------------------------------------------
	The fpscounter class is a simple class that keeps track of
various timing information. Its usage is simple and can be inferred
from the sample applications and comments.
	The class Random is another static class that provides all the
various randomness functionality that you might need. The comments
in the header file should explain anything that's not already
self-explanatory.
	The functions in functions.inl.h are simple ones that exist
because they are needed. Currently all that exists is a min and a
max function because not all C/C++ environments provide them and a
simple function that takes a double and returns it's direction
(sign).
