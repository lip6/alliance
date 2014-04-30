# $Id: Makefile.am,v 1.5 2005/01/19 15:13:50 hcl Exp $

AM_CFLAGS = @ALLIANCE_CFLAGS@ -I$(top_srcdir)/mbk/src

bin_PROGRAMS = flatph

flatph_LDADD = @ALLIANCE_LIBS@ \
-L$(top_builddir)/mbk/src/.libs \
          -lMpu -lMlu \
	  -lMlo \
	  -lMph -lMut \
	  -lRcn 
	   
flatph_SOURCES = flatph.c
