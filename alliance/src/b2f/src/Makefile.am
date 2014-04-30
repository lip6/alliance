## Process this file with automake to produce Makefile.in

bin_PROGRAMS = b2f

AM_CFLAGS = @ALLIANCE_CFLAGS@ -I$(top_srcdir)/abe/src \
-I$(top_srcdir)/abl/src \
-I$(top_srcdir)/abt/src \
-I$(top_srcdir)/abv/src \
-I$(top_srcdir)/aut/src \
-I$(top_srcdir)/bdd/src \
-I$(top_srcdir)/btr/src \
-I$(top_srcdir)/fsm/src \
-I$(top_srcdir)/ftl/src \
-I$(top_srcdir)/mbk/src

b2f_LDADD = @ALLIANCE_LIBS@ \
-L$(top_builddir)/abt/src/.libs \
-L$(top_builddir)/abv/src/.libs \
-L$(top_builddir)/abe/src/.libs \
-L$(top_builddir)/ftl/src/.libs \
-L$(top_builddir)/fks/src/.libs \
-L$(top_builddir)/fvh/src/.libs \
-L$(top_builddir)/fsm/src/.libs \
-L$(top_builddir)/btr/src/.libs \
-L$(top_builddir)/bdd/src/.libs \
-L$(top_builddir)/abl/src/.libs \
-L$(top_builddir)/aut/src/.libs \
-L$(top_builddir)/mbk/src/.libs \
          -lAbt -lAbv -lAbe -lFtl -lFks -lFvh -lFsm \
	  -lBtr -lBdd -lAbl -lAut -lMut

b2f_SOURCES = \
b2f_beh2fsm.c   b2f_error.c     b2f_main.c \
b2f_beh2fsm.h   b2f_error.h     b2f_main.h
