# $Id: Makefile.am,v 1.4 2002/10/01 21:09:51 pnt Exp $

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
