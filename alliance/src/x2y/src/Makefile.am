# $Id: Makefile.am,v 1.2 2002/04/29 15:36:08 xtof Exp $

AM_CFLAGS = @ALLIANCE_CFLAGS@ 

bin_PROGRAMS = x2y

x2y_LDADD = @ALLIANCE_LIBS@ \
          -lMpu -lMlu \
	  -lMcl -lMcp \
	  -lMal -lMap \
	  -lMsl \
	  -lMel -lMgl \
	  -lMhl \
	  -lMvl \
	  -lMmg \
	  -lMlo \
	  -lMph -lMut \
	  -lRcn 
	  
x2y_SOURCES = x2y.c
