## Process this file with automake to produce Makefile.in

AM_CFLAGS    = -g -I$(top_srcdir)/mbk/src


bin_PROGRAMS = m2e

m2e_LDADD    = \
-L$(top_srcdir)/mbk/src \
-L$(top_srcdir)/mbkal/src \
-L$(top_srcdir)/mbkap/src \
-L$(top_srcdir)/mbkedif/src \
-L$(top_srcdir)/mbkhilo/src \
-L$(top_srcdir)/mbkmg/src \
-L$(top_srcdir)/mbkspice/src \
-L$(top_srcdir)/mbkvhdlg/src \
-L$(top_srcdir)/mbkvrlog/src \
-L$(top_srcdir)/mbkvti/src \
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

