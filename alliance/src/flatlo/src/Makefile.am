# $Id: Makefile.am,v 1.5 2005/01/19 15:13:50 hcl Exp $

AM_CFLAGS      = -I$(top_srcdir)/mbk/src

bin_PROGRAMS   = flatlo

flatlo_LDADD   = \
-L$(top_srcdir)/abe/src \
-L$(top_srcdir)/abl/src \
-L$(top_srcdir)/abt/src \
-L$(top_srcdir)/abv/src \
-L$(top_srcdir)/aut/src \
-L$(top_srcdir)/bdd/src \
-L$(top_srcdir)/mbk/src \
-L$(top_srcdir)/mbkal/src \
-L$(top_srcdir)/mbkedif/src \
-L$(top_srcdir)/mbkhilo/src \
-L$(top_srcdir)/mbkspice/src \
-L$(top_srcdir)/mbkvhdlg/src \
-L$(top_srcdir)/mbkvrlog/src \
-L$(top_srcdir)/mbkvti/src \
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
