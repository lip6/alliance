## Process this file with automake to produce Makefile.in

bin_PROGRAMS = k2f

AM_CFLAGS = @ALLIANCE_CFLAGS@ 

k2f_LDADD = @ALLIANCE_LIBS@ \
          ${top_builddir}/../ftl/src/libFtl.la ${top_builddir}/../fks/src/libFks.la ${top_builddir}/../fvh/src/libFvh.la ${top_builddir}/../fsm/src/libFsm.la \
	  ${top_builddir}/../bdd/src/libBdd.la ${top_builddir}/../abl/src/libAbl.la ${top_builddir}/../aut/src/libAut.la ${top_builddir}/../mbk/src/libMut.la


k2f_SOURCES = \
k2f_error.c   k2f_error.h   k2f_main.c    k2f_main.h
