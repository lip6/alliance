# -*- Mode: Makefile -*-
#
####---------------------------------------------------------###
# description	: Alliance include file for Makefiles
# architecture	: Linux bip.lip6.fr 2.4.9-12_recher_smp_1 #1 SMP Wed Nov 14 14:49:08 MET 2001 i686 unknown
# date		: Wed Feb 27 11:25:31 CET 2002
# file		: Linux.mk
#

# The variables $ALLIANCE_* are set by
# alc_env.[c]sh script or libraries.mk

PROGRAM_SUFFIX=

GNU_LIB          = /usr/lib
GNU_INCLUDE      = .

X11_LIB          = .  -L/usr/X11R6/lib   -lSM -lICE
X11_INCLUDE      = .  -I/usr/X11R6/include

MOTIF_LIB        = -L/usr/X11R6/lib -lXm
MOTIF_INCLUDE    = -I/usr/X11R6/include

XPM_LIB          = -lXpm
XPM_INCLUDE      = 

SHELL		= /bin/sh
CSH		= /bin/csh
CP		= /asim/bazar/bin/cp
CAT		= /asim/bazar/bin/cat
MV		= /asim/bazar/bin/mv
RM		= /asim/bazar/bin/rm
MKDIR		= /asim/bazar/bin/mkdir
FIND		= /usr/bin/find
SED		= /bin/sed
AWK		= /bin/gawk
TR		= /asim/bazar/bin/tr
TOUCH		= /asim/bazar/bin/touch
STRIP		= /usr/bin/strip
RANLIB		= /usr/bin/ranlib

MAKE             = /usr/bin/gmake
MAKEFLAGS        = 

CC               = /usr/bin/gcc
CFLAGS           = -O2 -Wall -DLinux  -DAUTO_HAS_XPM -DAUTO_HAS_VFORK -DAUTO_HAS_SA_RESTART -DAUTO_HAS_DRAND48 -DAUTO_HAS_VALUES_H
CPPFLAGS         = 

YACC             = /usr/bin/bison
YACCFLAGS        = -y 

LEX              = /usr/bin/flex
LEXFLAGS         = 

AR               = /usr/bin/ar
ARFLAGS          = rv

# EOF
