
#===============================================================================
# stack2d
#===============================================================================

PROG=stack2d

SRCDIR=../src
GLOWDIR=../glow/glow_src

MODULES=assemblage atoms drawableobjects fps interactionlibrary interactions lj6_12 particles random stack2d surfaces vectors

GLOWMODULES=glow glowAux glowCheckBoxWidget glowDebug glowLabelWidget glowMenuButtonWidget glowPanelWidget glowPushButtonWidget glowQuickPalette glowRadioButtonWidget glowScrollBarWidget glowSenderReceiver glowSeparatorWidget glowSliderWidget glowTextData glowTextFieldWidget glowTextFieldWindow glowUtilities glowVectorAlgebra glowViewTransform glowWidget glowMessageWindow

COMPILE=gcc
COMPILE_IRIX=CC
SHELL=/bin/sh

LIBS=-lglut -lpthread -lGL -lGLU -lXi -lXmu -lX11 -lm -lstdc++
LIBS_IRIX=-lCio -lglut -lX11 -lXmu -lXext -lGL -lGLU -lm

DEPFLAGS=-MM
DEPFLAGS_IRIX=-M

CFLAGS=-Wall \
	-DGLOW_PLATFORM_LINUX \
	-DGLOW_PLATFORM_LITTLEENDIAN
NODEBUGCFLAGS=-O2 -DNODEBUG
LDFLAGS=-L/usr/X11R6/lib

CFLAGS_IRIX=-fullwarn -no_auto_include \
	-LANG:std \
	-woff 1209,3201,1424,1375 \
	-OPT:Olimit=0 \
	-I../../Compat-SGI \
	-DGLOW_COMPAT_CLIBNOSTDNAMESPACE \
	-DGLOW_PLATFORM_IRIX \
	-DGLOW_PLATFORM_BIGENDIAN
LDFLAGS_IRIX=


include $(GLOWDIR)/glow_base.Makefile
