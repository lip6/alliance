## Process this file with automake to produce Makefile.in

bin_PROGRAMS = boom

CFLAGS = @CFLAGS@ -DBOOM_VERSION=\"@BOOM_VERSION@\"

boom_LDADD = @LIBS@ \
          -lAbv -lAbe -lAbt -lBdd -lAbl -lAut -lMut -lm

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
