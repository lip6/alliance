## Process this file with automake to produce Makefile.in

bin_PROGRAMS = fsp

AM_CFLAGS = @ALLIANCE_CFLAGS@ \
-I$(top_srcdir)/abl/src \
-I$(top_srcdir)/aut/src \
-I$(top_srcdir)/bdd/src \
-I$(top_srcdir)/fsm/src \
-I$(top_srcdir)/ftl/src \
-I$(top_srcdir)/mbk/src

fsp_LDADD = @ALLIANCE_LIBS@ \
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


fsp_SOURCES = \
fsp_comp.c    fsp_debug.c   fsp_main.c    fsp_proof.c \
fsp_comp.h    fsp_debug.h   fsp_main.h    fsp_proof.h
