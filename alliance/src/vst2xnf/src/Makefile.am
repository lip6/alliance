## Process this file with automake to produce Makefile.in

AM_CFLAGS        = -g \
-I$(top_srcdir)/mbk/src

bin_PROGRAMS     = vst2xnf

vst2xnf_LDADD    = \
-L$(top_builddir)/mbk/src \
-L$(top_builddir)/mbkal/src \
-L$(top_builddir)/mbkap/src \
-L$(top_builddir)/mbkedif/src \
-L$(top_builddir)/mbkhilo/src \
-L$(top_builddir)/mbkmg/src \
-L$(top_builddir)/mbkspice/src \
-L$(top_builddir)/mbkvhdlg/src \
-L$(top_builddir)/mbkvrlog/src \
-L$(top_builddir)/mbkvti/src \
-lMpu -lMlu  \
                   -lMcl -lMcp  \
                   -lMal -lMap  \
                   -lMsl        \
                   -lMel -lMgl  \
                   -lMhl        \
                   -lMvg        \
                   -lMmg        \
                   -lMlo        \
                   -lMph -lMut  \
                   -lRcn


vst2xnf_SOURCES  = vst2xnf.c

