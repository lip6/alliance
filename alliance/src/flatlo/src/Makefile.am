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
-L$(top_builddir)/mbkal/src \
-L$(top_builddir)/mbkedif/src \
-L$(top_builddir)/mbkhilo/src \
-L$(top_builddir)/mbkspice/src \
-L$(top_builddir)/mbkvhdlg/src \
-L$(top_builddir)/mbkvrlog/src \
-L$(top_builddir)/mbkvti/src \
		     -lAbv \
	             -lAbe \
	             -lAbt \
	             -lMlu \
	             -lMcl \
	             -lMal \
	             -lMsl \
	             -lMhl \
	             -lMel \
	             -lMvg \
	             -lMgl \
	             -lRcn \
	             -lMlo \
	             -lBdd \
	             -lAbl \
	             -lAut \
	             -lMut

flatlo_SOURCES = flatlo.c
