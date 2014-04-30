## Process this file with automake to produce Makefile.in

bin_PROGRAMS = k2f

AM_CFLAGS = @ALLIANCE_CFLAGS@ -I$(top_srcdir)/aut/src \
-I$(top_srcdir)/abl/src \
-I$(top_srcdir)/bdd/src \
-I$(top_srcdir)/fsm/src \
-I$(top_srcdir)/ftl/src \
-I$(top_srcdir)/mbk/src

k2f_LDADD = @ALLIANCE_LIBS@ \
-L$(top_builddir)/abl/src/.libs \
-L$(top_builddir)/aut/src/.libs \
-L$(top_builddir)/bdd/src/.libs \
-L$(top_builddir)/fks/src/.libs \
-L$(top_builddir)/fsm/src/.libs \
-L$(top_builddir)/ftl/src/.libs \
-L$(top_builddir)/fvh/src/.libs \
-L$(top_builddir)/mbk/src/.libs \
          -lFtl -lFks -lFvh -lFsm \
	  -lBdd -lAbl -lAut -lMut


k2f_SOURCES = \
k2f_error.c   k2f_error.h   k2f_main.c    k2f_main.h
