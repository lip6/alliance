## Process this file with automake to produce Makefile.in

bin_PROGRAMS = growstk 

AM_CFLAGS = @ALLIANCE_CFLAGS@ -I$(top_srcdir)/mbk/src \
-I$(top_srcdir)/genview/src/gcc-1.42 \
-I$(top_srcdir)/rds/src

growstk_LDADD = @ALLIANCE_LIBS@ \
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

growstk_SOURCES = hash.h hash.c growstk.c 
