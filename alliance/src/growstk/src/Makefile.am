## Process this file with automake to produce Makefile.in

bin_PROGRAMS = growstk 

AM_CFLAGS = @ALLIANCE_CFLAGS@

growstk_LDADD = @ALLIANCE_LIBS@ \
          -lMpu -lMcp -lMap -lMmg \
          -lRtl -lRgs -lRcf -lRfm -lRpr -lRwi -lRut -lRds\
	  -lMph -lMut

growstk_SOURCES = hash.h hash.c growstk.c 
