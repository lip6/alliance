
# ###---------------------------------------------------------###
# file		: Linux_aout.mk  				#
# date		: Jun 19 1997					#
# description	: Alliance include file for Makefiles		#
# ###---------------------------------------------------------###

# ###---------------------------------------------------------###
# architecture	: 386 PC (Linux 0.99.14)			#
# ###---------------------------------------------------------###

ALLIANCE_VERSION = '"3.2b"'
ALLIANCE_BIN     = $(TOP)/bin
ALLIANCE_LIB     = $(TOP)/lib
ALLIANCE_INCLUDE = $(TOP)/include
ALLIANCE_CELLS   = $(TOP)/cells
ALLIANCE_ETC     = $(TOP)/etc

# distrib path
#TARGET_BIN       = $(TOP)/bin
#TARGET_LIB       = $(TOP)/lib
#TARGET_INCLUDE   = $(TOP)/include

# develloppers path
TARGET_BIN       = $(HOME)/labo/$(MACHINE)/bin
TARGET_LIB       = $(HOME)/labo/$(MACHINE)/lib
TARGET_INCLUDE   = $(HOME)/labo/$(MACHINE)/include

X11_BIN          = /usr/bin/X11
X11_LIB          = /usr/lib/X11
X11_INCLUDE      = /usr/include/X11

MOTIF_BIN        = /usr/bin/X11
MOTIF_LIB        = /usr/lib/X11
MOTIF_INCLUDE    = /usr/include/X11

SHELL            = /bin/sh
CSH              = /bin/csh
CP               = /bin/cp
MV               = /bin/mv
RM               = /bin/rm
MKDIR            = /bin/mkdir
FIND             = /usr/bin/find
SED              = /usr/bin/sed
AWK              = /usr/bin/awk
TR               = /usr/bin/tr
TOUCH            = /bin/touch
STRIP            = /usr/bin/strip
RANLIB           = /usr/bin/ranlib

MAKE             = /usr/bin/make
MAKEFLAGS        =

CC               = /usr/bin/gcc
CFLAGS           = -static
CPPFLAGS         = 
#CFLAGS           = -static -DALLIANCE_DISTRIB
#CPPFLAGS         = -DALLIANCE_DISTRIB

YACC             = /usr/bin/bison
YACCFLAGS        = -y

LEX              = /usr/bin/flex
LEXFLAGS         =

AR               = /usr/bin/ar
ARFLAGS          = -rv

LINT             = 
LINTFLAGS        =
