# $Id: Makefile.am,v 1.6 2007/12/26 10:02:38 xtof Exp $

AM_CFLAGS      = -I$(top_srcdir)/mbk/src

bin_PROGRAMS   = flatlo

flatlo_LDADD   = \
-L$(top_builddir)/abe/src \
-L$(top_builddir)/abl/src \
-L$(top_builddir)/abt/src \
-L$(top_builddir)/abv/src \
-L$(top_builddir)/aut/src \
-L$(top_builddir)/bdd/src \
-L$(top_builddir)/mbk/src \
		     -lAbv \
	             -lAbe \
	             -lAbt \
	             -lMlu \
	             -lRcn \
	             -lMlo \
	             -lBdd \
	             -lAbl \
	             -lAut \
	             -lMut

flatlo_SOURCES = flatlo.c
