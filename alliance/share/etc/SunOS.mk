# -*- Mode: Makefile -*-
#
####---------------------------------------------------------###
# description	: Alliance include file for Makefiles
# architecture	: SunOS heavy 4.1.3_U1 1 sun4m unknown
# date		: Tue Jun 30 00:58:53 MET DST 1998
# file		: SunOS.mk
#

GNU_LIB          = /asim/gnu/lib
GNU_INCLUDE      = /asim/gnu/include


X11_LIB          = .  -L/usr/X11R6.3/lib   -lSM -lICE
X11_INCLUDE      = .  -I/usr/X11R6.3/include

#You may need -lXintl
MOTIF_LIB        = /usr/lib/X11
MOTIF_INCLUDE    = /usr/include/X11

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
