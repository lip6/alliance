## Process this file with automake to produce Makefile.in

bin_PROGRAMS = k2f

AM_CFLAGS = @ALLIANCE_CFLAGS@ -I$(top_srcdir)/aut/src \
-I$(top_srcdir)/abl/src \
-I$(top_srcdir)/bdd/src \
-I$(top_srcdir)/fsm/src \
-I$(top_srcdir)/ftl/src \
-I$(top_srcdir)/mbk/src

k2f_LDADD = @ALLIANCE_LIBS@ \
-L$(top_srcdir)/abl/src \
-L$(top_srcdir)/aut/src \
-L$(top_srcdir)/bdd/src \
-L$(top_srcdir)/fks/src \
-L$(top_srcdir)/fsm/src \
-L$(top_srcdir)/ftl/src \
-L$(top_srcdir)/fvh/src \
-L$(top_srcdir)/mbk/src \
          -lFtl -lFks -lFvh -lFsm \
	  -lBdd -lAbl -lAut -lMut


k2f_SOURCES = \
k2f_error.c   k2f_error.h   k2f_main.c    k2f_main.h
