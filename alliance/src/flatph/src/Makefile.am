# $Id: Makefile.am,v 1.2 2002/04/29 15:28:11 xtof Exp $

AM_CFLAGS = @ALLIANCE_CFLAGS@ 

bin_PROGRAMS = flatph

flatph_LDADD = @ALLIANCE_LIBS@ \
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
	   
flatph_SOURCES = flatph.c
