## Process this file with automake to produce Makefile.in

bin_PROGRAMS = moka

AM_CFLAGS = -Wall \
-I$(top_srcdir)/abe/src \
-I$(top_srcdir)/abl/src \
-I$(top_srcdir)/abv/src \
-I$(top_srcdir)/aut/src \
-I$(top_srcdir)/bdd/src \
-I$(top_srcdir)/btr/src \
-I$(top_srcdir)/ctl/src \
-I$(top_srcdir)/ctp/src \
-I$(top_srcdir)/fsm/src \
-I$(top_srcdir)/ftl/src \
-I$(top_srcdir)/mbk/src \
-I$(top_srcdir)/fvh/src \
-I$(top_srcdir)/vex/src \
@ALLIANCE_CFLAGS@  

moka_LDADD = @ALLIANCE_LIBS@ \
-L$(top_srcdir)/abe/src \
-L$(top_srcdir)/abl/src \
-L$(top_srcdir)/abt/src \
-L$(top_srcdir)/abv/src \
-L$(top_srcdir)/aut/src \
-L$(top_srcdir)/bdd/src \
-L$(top_srcdir)/btr/src \
-L$(top_srcdir)/ctl/src \
-L$(top_srcdir)/ctp/src \
-L$(top_srcdir)/fks/src \
-L$(top_srcdir)/fsm/src \
-L$(top_srcdir)/ftl/src \
-L$(top_srcdir)/fvh/src \
-L$(top_srcdir)/mbk/src \
-L$(top_srcdir)/vex/src \
          -lFtl -lFks -lFvh -lFsm \
	  -lCtp -lCtl -lVex -lAbt -lAbv -lAbe -lBtr -lBdd -lAbl -lAut -lMut


moka_SOURCES = \
mocha_check.h  mocha_debug.h  mocha_main.h    mocha_syf.h \
mocha_beh.c    mocha_ctl.c    mocha_fsm.c    mocha_shared.c \
mocha_beh.h    mocha_ctl.h    mocha_fsm.h    mocha_shared.h \
mocha_check.c  mocha_debug.c  mocha_main.c   mocha_syf.c \
mocha_bdd.c mocha_bdd.h
