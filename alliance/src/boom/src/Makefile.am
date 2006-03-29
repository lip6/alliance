## Process this file with automake to produce Makefile.in

bin_PROGRAMS = boom

AM_CFLAGS = -I$(top_srcdir)/abe/src \
-I$(top_srcdir)/abl/src \
-I$(top_srcdir)/abt/src \
-I$(top_srcdir)/abv/src \
-I$(top_srcdir)/aut/src \
-I$(top_srcdir)/bdd/src \
-I$(top_srcdir)/mbk/src \
@ALLIANCE_CFLAGS@

boom_LDADD = @ALLIANCE_LIBS@ \
-L$(top_srcdir)/abv/src \
-L$(top_srcdir)/abe/src \
-L$(top_srcdir)/abt/src \
-L$(top_srcdir)/bdd/src \
-L$(top_srcdir)/abl/src \
-L$(top_srcdir)/aut/src \
-L$(top_srcdir)/mbk/src \
          -lAbv -lAbe -lAbt -lBdd -lAbl -lAut -lMut

boom_SOURCES = \
boom_abl.c        boom_cost.h       boom_main.c       boom_procrast.h \
boom_abl.h        boom_dc.c         boom_main.h       boom_random.c \
boom_anneal.c     boom_dc.h         boom_one.c        boom_random.h \
boom_anneal.h     boom_debug.c      boom_one.h        boom_shared.c \
boom_aux.c        boom_debug.h      boom_optim.c      boom_shared.h \
boom_aux.h        boom_drive.c      boom_optim.h      boom_simple.c \
boom_auxinit.c    boom_drive.h      boom_order.c      boom_simple.h \
boom_auxinit.h    boom_error.c      boom_order.h      boom_simul.c \
boom_bdd.c        boom_error.h      boom_param.c      boom_simul.h \
boom_bdd.h        boom_gradient.c   boom_param.h      boom_top.c \
boom_burgun.c     boom_gradient.h   boom_parse.c      boom_top.h \
boom_burgun.h     boom_hash.c       boom_parse.h      boom_window.c \
boom_check.c      boom_hash.h       boom_post.c       boom_window.h \
boom_check.h      boom_post.h \
boom_cost.c       boom_procrast.c
