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
-L$(top_srcdir)/abt/src \
-L$(top_srcdir)/abv/src \
-L$(top_srcdir)/abe/src \
-L$(top_srcdir)/ftl/src \
-L$(top_srcdir)/fks/src \
-L$(top_srcdir)/fvh/src \
-L$(top_srcdir)/fsm/src \
-L$(top_srcdir)/btr/src \
-L$(top_srcdir)/bdd/src \
-L$(top_srcdir)/abl/src \
-L$(top_srcdir)/aut/src \
-L$(top_srcdir)/mbk/src \
          -lAbt -lAbv -lAbe -lFtl -lFks -lFvh -lFsm \
	  -lBtr -lBdd -lAbl -lAut -lMut

b2f_SOURCES = \
b2f_beh2fsm.c   b2f_error.c     b2f_main.c \
b2f_beh2fsm.h   b2f_error.h     b2f_main.h
