# $Id: Makefile.am,v 1.5 2002/05/06 13:31:05 czo Exp $

AM_CFLAGS = @ALLIANCE_CFLAGS@ 

bin_PROGRAMS = x2y

x2y_LDADD = @ALLIANCE_LIBS@ \
          -lMpu -lMlu \
	  -lMcl -lMcp \
	  -lMal \
          -lMap \
	  -lMsl \
	  -lMel \
          -lMgl \
	  -lMhl \
	  -lMvl \
	  -lMmg \
	  -lMlo \
	  -lMph \
          -lMut \
	  -lRcn 
	  
x2y_SOURCES = x2y.c
