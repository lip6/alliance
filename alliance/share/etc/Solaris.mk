# -*- Mode: Makefile -*-
#
####---------------------------------------------------------###
# description	: Alliance include file for Makefiles
# architecture	: SunOS opera 5.5.1 Generic sun4u sparc
# date		: Fri Sep 25 00:53:59 MET DST 1998
# file		: Solaris.mk
#

GNU_LIB          = /asim/gnu/lib
GNU_INCLUDE      = /asim/gnu/include


X11_LIB          = .  -L/labo/X11R6/lib -R/labo/X11R6/lib -lsocket  -lnsl  -lSM -lICE
X11_INCLUDE      = .  -I/labo/X11R6/include

#You may need -lXintl
MOTIF_LIB        = /usr/dt/lib -R /usr/dt/lib
MOTIF_INCLUDE    = /usr/dt/include

SHELL		= /asim/gnu/bin/sh
CSH		= /bin/csh
CP		= /asim/gnu/bin/cp
MV		= /asim/gnu/bin/mv
RM		= /asim/gnu/bin/rm
MKDIR		= /asim/gnu/bin/mkdir
FIND		= /asim/gnu/bin/find
SED		= /asim/gnu/bin/sed
AWK		= /asim/gnu/bin/gawk
TR		= /asim/gnu/bin/tr
TOUCH		= /asim/gnu/bin/touch
STRIP		= /asim/gnu/bin/strip
RANLIB		= /asim/gnu/bin/ranlib

MAKE             = /asim/gnu/bin/gmake
MAKEFLAGS        = 

CC               = /asim/gnu/bin/gcc
CFLAGS           = -O4 -Wall
CPPFLAGS         = 

#CFLAGS           = -DALLIANCE_DISTRIB
#CPPFLAGS         = -DALLIANCE_DISTRIB

YACC             = /asim/gnu/bin/bison
YACCFLAGS        = -y 

LEX              = /asim/gnu/bin/flex
LEXFLAGS         = 

AR               = /asim/gnu/bin/ar
ARFLAGS          = rv

# EOF
