## Process this file with automake to produce Makefile.in

AM_CFLAGS    = @ALLIANCE_CFLAGS@ \
-I$(top_srcdir)/abe/src \
-I$(top_srcdir)/abl/src \
-I$(top_srcdir)/abv/src \
-I$(top_srcdir)/aut/src \
-I$(top_srcdir)/bdd/src \
-I$(top_srcdir)/mbk/src

bin_PROGRAMS = flatbeh

flatbeh_LDADD = @ALLIANCE_LIBS@ \
-L$(top_builddir)/abv/src/.libs \
-L$(top_builddir)/abe/src/.libs \
-L$(top_builddir)/abt/src/.libs \
-L$(top_builddir)/mbk/src/.libs \
-L$(top_builddir)/bdd/src/.libs \
-L$(top_builddir)/abl/src/.libs \
-L$(top_builddir)/aut/src/.libs \
          -lAbv -lAbe -lAbt -lMlu -lRcn -lMlo -lBdd -lAbl -lAut -lMut
	   
flatbeh_SOURCES = abstract.c abstract.h utils.c utils.h main.c

