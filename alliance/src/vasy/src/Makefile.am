## Process this file with automake to produce Makefile.in

bin_PROGRAMS = vasy

AM_CFLAGS = @ALLIANCE_CFLAGS@ \
-I$(top_srcdir)/abl/src \
-I$(top_srcdir)/aut/src \
-I$(top_srcdir)/bdd/src \
-I$(top_srcdir)/mbk/src \
-I$(top_srcdir)/rtd/src \
-I$(top_srcdir)/rtn/src \
-I$(top_srcdir)/vex/src \
-I$(top_srcdir)/vbh/src \
-I$(top_srcdir)/vpn/src

vasy_LDADD = @ALLIANCE_LIBS@ \
-L$(top_builddir)/abl/src/.libs \
-L$(top_builddir)/aut/src/.libs \
-L$(top_builddir)/bdd/src/.libs \
-L$(top_builddir)/mbk/src/.libs \
-L$(top_builddir)/rtd/src/.libs \
-L$(top_builddir)/rtn/src/.libs \
-L$(top_builddir)/vbh/src/.libs \
-L$(top_builddir)/vex/src/.libs \
-L$(top_builddir)/vpn/src/.libs \
          -lRtd -lRtn -lVpn -lVbh \
	  -lVex -lBdd -lAbl -lAut -lMut

vasy_SOURCES = \
vasy_analys.c     vasy_drvvlog.h    vasy_mulwait.c    vasy_redwait.h \
vasy_analys.h     vasy_elabo.c      vasy_mulwait.h    vasy_shared.c \
vasy_array.c      vasy_elabo.h      vasy_onewait.c    vasy_shared.h \
vasy_array.h      vasy_error.c      vasy_onewait.h    vasy_simprtl.c \
vasy_dynfunc.c vasy_dynfunc.h \
vasy_debug.c      vasy_error.h      vasy_parse.c      vasy_simprtl.h \
vasy_debug.h      vasy_func.c       vasy_parse.h      vasy_simul.c \
vasy_drvalc.c     vasy_func.h       vasy_preanal.c    vasy_simul.h \
vasy_drvalc.h     vasy_generate.c   vasy_preanal.h    vasy_support.c \
vasy_drvrtl.c     vasy_generate.h   vasy_redact.c     vasy_support.h \
vasy_drvrtl.h     vasy_generic.c    vasy_redact.h     vasy_synth.c \
vasy_drvsyn.c     vasy_generic.h    vasy_redinst.c    vasy_synth.h \
vasy_drvsyn.h     vasy_loop.c       vasy_redinst.h    vasy_vexbdd.c \
vasy_drvvex.c     vasy_loop.h       vasy_reduce.c     vasy_vexbdd.h \
vasy_drvvex.h     vasy_main.c       vasy_reduce.h \
vasy_drvvlog.c    vasy_main.h       vasy_redwait.c
