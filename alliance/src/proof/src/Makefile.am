## Process this file with automake to produce Makefile.in

bin_PROGRAMS = proof

AM_CFLAGS = @ALLIANCE_CFLAGS@ 

proof_LDADD = @ALLIANCE_LIBS@ \
          ${top_builddir}/../bvl/src/libBvl.la ${top_builddir}/../bhl/src/libBhl.la ${top_builddir}/../beh/src/libBeh.la ${top_builddir}/../log/src/libLog.la ${top_builddir}/../mbk/src/libMut.la ${top_builddir}/../asimut/src/libCst.la

proof_SOURCES = \
proof_compile.c  proof_main.c  proof_util.c \
proof_compile.h proof_util.h
