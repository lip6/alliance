## Process this file with automake to produce Makefile.in

bin_PROGRAMS = s2r 

CFLAGS = @CFLAGS@ @ALLIANCE_CFLAGS@

s2r_LDADD = @ALLIANCE_LIBS@ \
          -lMpu -lMcp -lMap -lMmg \
          -lRtl -lRgs -lRcf -lRfm -lRpr -lRwi -lRut -lRds\
	  -lMph -lMut

s2r_SOURCES = generic.h  hash.h  maxima.h  merge.h\
              postrat.h rdsacces.h  statistics.c hash.c\
              main.c  merge.c postrat.c rdsacces.c\
              statistics.h
