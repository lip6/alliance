## Process this file with automake to produce Makefile.in

bin_PROGRAMS = syf

AM_CFLAGS = @ALLIANCE_CFLAGS@

syf_LDADD = @ALLIANCE_LIBS@ \
          ${top_builddir}/../ftl/src/libFtl.la ${top_builddir}/../fks/src/libFks.la ${top_builddir}/../fvh/src/libFvh.la ${top_builddir}/../fsm/src/libFsm.la \
	  ${top_builddir}/../bdd/src/libBdd.la ${top_builddir}/../abl/src/libAbl.la ${top_builddir}/../aut/src/libAut.la ${top_builddir}/../mbk/src/libMut.la


syf_SOURCES = \
fbh_parse.c     syf_comp.h      syf_error.c     syf_main.h      syf_synth.c \
fbh_parse.h     syf_cost.c      syf_error.h     syf_must.c      syf_synth.h \
kis_parse.c     syf_cost.h      syf_fsm.c       syf_must.h      syf_tools.c \
kis_parse.h     syf_dc.c        syf_fsm.h       syf_onehot.c    syf_tools.h \
syf_asp.c       syf_dc.h        syf_fsm2fbh.c   syf_onehot.h    syf_user.c \
syf_asp.h       syf_encod.c     syf_fsm2fbh.h   syf_random.c    syf_user.h \
syf_bdd.c       syf_encod.h     syf_jedi.c      syf_random.h    syf_verify.c \
syf_bdd.h       syf_env.c       syf_jedi.h      syf_simp.c      syf_verify.h \
syf_comp.c      syf_env.h       syf_main.c      syf_simp.h
