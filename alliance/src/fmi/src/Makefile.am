## Process this file with automake to produce Makefile.in

bin_PROGRAMS = fmi

AM_CFLAGS = @ALLIANCE_CFLAGS@ -I$(top_srcdir)/abl/src \
-I$(top_srcdir)/aut/src \
-I$(top_srcdir)/bdd/src \
-I$(top_srcdir)/fsm/src \
-I$(top_srcdir)/ftl/src \
-I$(top_srcdir)/mbk/src

fmi_LDADD = @ALLIANCE_LIBS@ \
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

fmi_SOURCES = \
fmi_bdd.c     fmi_main.c    fmi_optim.c   fmi_parse.c   \
fmi_bdd.h     fmi_main.h    fmi_optim.h   fmi_parse.h
