# $Id: Makefile.am,v 1.8 2005/01/19 15:13:57 hcl Exp $

AM_CFLAGS = @ALLIANCE_CFLAGS@ \
-I$(top_srcdir)/mbk/src

bin_PROGRAMS = x2y

x2y_LDADD = @ALLIANCE_LIBS@ \
            -L$(top_srcdir)/mbk/src \
            -lMpu -lMlu -lMlo -lMph -lMut -lRcn 
	  
x2y_SOURCES = x2y.c
