# -*- Mode: Makefile -*-
#
####---------------------------------------------------------###
# description	: Alliance include file for Makefiles
# architecture	: Linux bechet 2.0.33 #2 Sun Mar 15 03:57:50 MET 1998 i586 unknown
# date		: Mon Sep 28 13:20:39 MET DST 1998
# file		: Linux_elf.mk
#

GNU_LIB          = /asim/gnu/lib
GNU_INCLUDE      = /asim/gnu/include


X11_LIB          = .  -L/usr/X11R6/lib   -lSM -lICE -lXintl
X11_INCLUDE      = .  -I/usr/X11R6/include

#You may need -lXintl
MOTIF_LIB        = /usr/X11R6/lib
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
