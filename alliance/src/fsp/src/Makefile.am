## Process this file with automake to produce Makefile.in

bin_PROGRAMS = fsp

AM_CFLAGS = @ALLIANCE_CFLAGS@ 

fsp_LDADD = @ALLIANCE_LIBS@ \
          ${top_builddir}/../ftl/src/libFtl.la ${top_builddir}/../fks/src/libFks.la ${top_builddir}/../fvh/src/libFvh.la ${top_builddir}/../fsm/src/libFsm.la \
	  ${top_builddir}/../bdd/src/libBdd.la ${top_builddir}/../abl/src/libAbl.la ${top_builddir}/../aut/src/libAut.la ${top_builddir}/../mbk/src/libMut.la


fsp_SOURCES = \
fsp_comp.c    fsp_debug.c   fsp_main.c    fsp_proof.c \
fsp_comp.h    fsp_debug.h   fsp_main.h    fsp_proof.h
