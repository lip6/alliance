## Process this file with automake to produce Makefile.in

bin_PROGRAMS = b2f

AM_CFLAGS = @ALLIANCE_CFLAGS@ 

b2f_LDADD = @ALLIANCE_LIBS@ \
          ${top_builddir}/../abt/src/libAbt.la ${top_builddir}/../abv/src/libAbv.la ${top_builddir}/../abe/src/libAbe.la ${top_builddir}/../ftl/src/libFtl.la ${top_builddir}/../fks/src/libFks.la ${top_builddir}/../fvh/src/libFvh.la ${top_builddir}/../fsm/src/libFsm.la \
	  ${top_builddir}/../btr/src/libBtr.la ${top_builddir}/../bdd/src/libBdd.la ${top_builddir}/../abl/src/libAbl.la ${top_builddir}/../aut/src/libAut.la ${top_builddir}/../mbk/src/libMut.la

b2f_SOURCES = \
b2f_beh2fsm.c   b2f_error.c     b2f_main.c \
b2f_beh2fsm.h   b2f_error.h     b2f_main.h
