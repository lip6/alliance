# $Id: Makefile.am,v 1.8 2005/01/19 15:13:57 hcl Exp $

AM_CFLAGS = @ALLIANCE_CFLAGS@ \
-I$(top_srcdir)/mbk/src

bin_PROGRAMS = x2y

x2y_LDADD = @ALLIANCE_LIBS@ \
-L$(top_srcdir)/mbk/src \
-L$(top_srcdir)/mbkal/src \
-L$(top_srcdir)/mbkap/src \
-L$(top_srcdir)/mbkedif/src \
-L$(top_srcdir)/mbkhilo/src \
-L$(top_srcdir)/mbkmg/src \
-L$(top_srcdir)/mbkspice/src \
-L$(top_srcdir)/mbkvhdl/src \
-L$(top_srcdir)/mbkvrlog/src \
-L$(top_srcdir)/mbkvti/src \
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
