## Process this file with automake to produce Makefile.in

bin_PROGRAMS = proof

AM_CFLAGS = @ALLIANCE_CFLAGS@ -I$(top_srcdir)/beh/src \
-I$(top_srcdir)/bvl/src \
-I$(top_srcdir)/log/src \
-I$(top_srcdir)/mbk/src

proof_LDADD = @ALLIANCE_LIBS@ \
-L$(top_srcdir)/asimut/src \
-L$(top_srcdir)/beh/src \
-L$(top_srcdir)/bhl/src \
-L$(top_srcdir)/bvl/src \
-L$(top_srcdir)/log/src \
-L$(top_srcdir)/mbk/src \
          -lBvl -lBhl -lBeh -lLog -lMut -lCst

proof_SOURCES = \
proof_compile.c  proof_main.c  proof_util.c \
proof_compile.h proof_util.h
