## Process this file with automake to produce Makefile.in

bin_PROGRAMS = moka

AM_CFLAGS = @ALLIANCE_CFLAGS@  -Wall

moka_LDADD = @ALLIANCE_LIBS@ \
          ${top_builddir}/../ftl/src/libFtl.la ${top_builddir}/../fks/src/libFks.la ${top_builddir}/../fvh/src/libFvh.la ${top_builddir}/../fsm/src/libFsm.la \
	  ${top_builddir}/../ctp/src/libCtp.la ${top_builddir}/../ctl/src/libCtl.la ${top_builddir}/../vex/src/libVex.la ${top_builddir}/../abt/src/libAbt.la ${top_builddir}/../abv/src/libAbv.la ${top_builddir}/../abe/src/libAbe.la ${top_builddir}/../btr/src/libBtr.la ${top_builddir}/../bdd/src/libBdd.la ${top_builddir}/../abl/src/libAbl.la ${top_builddir}/../aut/src/libAut.la ${top_builddir}/../mbk/src/libMut.la


moka_SOURCES = \
mocha_check.h  mocha_debug.h  mocha_main.h    mocha_syf.h \
mocha_beh.c    mocha_ctl.c    mocha_fsm.c    mocha_shared.c \
mocha_beh.h    mocha_ctl.h    mocha_fsm.h    mocha_shared.h \
mocha_check.c  mocha_debug.c  mocha_main.c   mocha_syf.c \
mocha_bdd.c mocha_bdd.h
