## Process this file with automake to produce Makefile.in

bin_PROGRAMS = l2p

AM_CFLAGS    = @ALLIANCE_CFLAGS@ -I$(top_srcdir)/mbk/src \
               -I$(top_srcdir)/rds/src

l2p_LDADD    = @ALLIANCE_LIBS@ \
               -L$(top_builddir)/mbk/src \
               -L$(top_builddir)/rds/src \
               -lRds -lMlu -lMpu -lMlo -lMph -lMut

l2p_SOURCES  = drive_ps.c  rps_inc.h   tmp_man.c \
               dict_color.ps  l2p.c	   tmp_dict.c
