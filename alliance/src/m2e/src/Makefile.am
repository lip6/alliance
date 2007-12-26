## Process this file with automake to produce Makefile.in

AM_CFLAGS    = -g -I$(top_srcdir)/mbk/src


bin_PROGRAMS = m2e

m2e_LDADD    = \
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
               -lRcn -lm


m2e_SOURCES  = m2e.c m2e.h

