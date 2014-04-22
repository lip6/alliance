## Process this file with automake to produce Makefile.in
bin_PROGRAMS = lvx

AM_CFLAGS = @ALLIANCE_CFLAGS@ -I$(top_srcdir)/mbk/src

lvx_LDADD = @ALLIANCE_LIBS@ \
            -L$(top_srcdir)/mbk/src \
            -lMlu -lMlo -lMut -lRcn 
	   
lvx_SOURCES = lvx.c
