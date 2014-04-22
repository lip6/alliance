## Process this file with automake to produce Makefile.in

AM_CFLAGS    = -g -I$(top_srcdir)/mbk/src

bin_PROGRAMS = m2e

m2e_LDADD    = -L$(top_builddir)/mbk/src \
               -lMpu -lMlu -lMlo -lMph -lMut -lRcn -lm

m2e_SOURCES  = m2e.c m2e.h

