## Process this file with automake to produce Makefile.in

bin_PROGRAMS = alcbanner

AM_CFLAGS = @ALLIANCE_CFLAGS@ -I$(top_srcdir)/mbk/src

alcbanner_LDADD = @ALLIANCE_LIBS@ \
          -L$(top_srcdir)/mbk/src -lMut

alcbanner_SOURCES = alcbanner.c
