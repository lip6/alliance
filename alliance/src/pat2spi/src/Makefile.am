## Process this file with automake to produce Makefile.in

bin_PROGRAMS = pat2spi

AM_CFLAGS = @ALLIANCE_CFLAGS@ -I$(top_srcdir)/aut/src \
-I$(top_srcdir)/mbk/src \
-I$(top_srcdir)/pat/src \
-I$(top_srcdir)/ppt/src

pat2spi_LDADD = @ALLIANCE_LIBS@ \
-L$(top_srcdir)/aut/src \
-L$(top_srcdir)/mbk/src \
-L$(top_srcdir)/pat/src \
-L$(top_srcdir)/phl/src \
-L$(top_srcdir)/ppt/src \
          -lPpt -lPhl -lPat -lAut -lMut


pat2spi_SOURCES = pat2spi.c   pat2spi.h
