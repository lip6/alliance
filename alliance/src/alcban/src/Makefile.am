## Process this file with automake to produce Makefile.in

bin_PROGRAMS = alcbanner

CFLAGS = @ALLIANCE_CFLAGS@ -DALLIANCE_VERSION=\"@ALLIANCE_VERSION@\"

alcbanner_LDADD = @ALLIANCE_LIBS@ \
          -lMut

alcbanner_SOURCES = alcbanner.c
