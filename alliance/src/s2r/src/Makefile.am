## Process this file with automake to produce Makefile.in

bin_PROGRAMS = s2r 

AM_CFLAGS = @ALLIANCE_CFLAGS@ \
-I$(top_srcdir)/rds/src \
-I$(top_srcdir)/genview/src/gcc-1.42 \
-I$(top_srcdir)/mbk/src

s2r_LDADD = @ALLIANCE_LIBS@ \
-L$(top_srcdir)/mbk/src \
-L$(top_srcdir)/mbkap/src \
-L$(top_srcdir)/mbkmg/src \
-L$(top_srcdir)/mbkvti/src \
-L$(top_srcdir)/rds/src \
-L$(top_srcdir)/rdscif/src \
-L$(top_srcdir)/rdsgds/src \
          -lMpu -lMcp -lMap -lMmg \
          -lRtl -lRgs -lRcf -lRfm -lRpr -lRwi -lRut -lRds\
	  -lMph -lMut

s2r_SOURCES = generic.h  hash.h  maxima.h  merge.h\
              postrat.h rdsacces.h  statistics.c hash.c\
              main.c  merge.c postrat.c rdsacces.c\
              statistics.h
