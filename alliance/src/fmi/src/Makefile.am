## Process this file with automake to produce Makefile.in

bin_PROGRAMS = fmi

AM_CFLAGS = @ALLIANCE_CFLAGS@ 

fmi_LDADD = @ALLIANCE_LIBS@ \
          ${top_builddir}/../ftl/src/libFtl.la ${top_builddir}/../fks/src/libFks.la ${top_builddir}/../fvh/src/libFvh.la ${top_builddir}/../fsm/src/libFsm.la \
	  ${top_builddir}/../bdd/src/libBdd.la ${top_builddir}/../abl/src/libAbl.la ${top_builddir}/../aut/src/libAut.la ${top_builddir}/../mbk/src/libMut.la

fmi_SOURCES = \
fmi_bdd.c     fmi_main.c    fmi_optim.c   fmi_parse.c   \
fmi_bdd.h     fmi_main.h    fmi_optim.h   fmi_parse.h
