## Process this file with automake to produce Makefile.in
bin_PROGRAMS = lvx

AM_CFLAGS = @ALLIANCE_CFLAGS@

lvx_LDADD = @ALLIANCE_LIBS@ \
          -lMlu \
	  -lMcl \
	  -lMal \
	  -lMsl \
	  -lMel -lMgl \
	  -lMhl \
	  -lMvl \
	  -lMlo \
	  -lMut \
	  -lRcn 
	   
lvx_SOURCES = lvx.c
