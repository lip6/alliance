## Process this file with automake to produce Makefile.in

bin_PROGRAMS  = proof

AM_CFLAGS     = @ALLIANCE_CFLAGS@ -I$(top_srcdir)/beh/src \
                -I$(top_srcdir)/bvl/src \
                -I$(top_srcdir)/log/src \
                -I$(top_srcdir)/mbk/src

proof_LDADD   = @ALLIANCE_LIBS@ \
                -L$(top_builddir)/asimut/src/.libs \
                -L$(top_builddir)/abl/src/.libs \
                -L$(top_builddir)/aut/src/.libs \
                -L$(top_builddir)/beh/src/.libs \
                -L$(top_builddir)/bhl/src/.libs \
                -L$(top_builddir)/bvl/src/.libs \
                -L$(top_builddir)/log/src/.libs \
                -L$(top_builddir)/mbk/src/.libs \
                -lBvl -lBhl -lBeh -lLog -lMut

proof_SOURCES = proof_compile.c  proof_main.c  proof_util.c \
                proof_compile.h proof_util.h
