## Process this file with automake to produce Makefile.in

bin_PROGRAMS    = pat2spi

AM_CFLAGS       = @ALLIANCE_CFLAGS@ -I$(top_srcdir)/aut/src \
                  -I$(top_srcdir)/mbk/src \
                  -I$(top_srcdir)/pat/src

pat2spi_LDADD   = @ALLIANCE_LIBS@ \
                  -L$(top_builddir)/aut/src/.libs \
                  -L$(top_builddir)/mbk/src/.libs \
                  -L$(top_builddir)/pat/src/.libs \
                  -lPat -lAut -lMut

pat2spi_SOURCES = pat2spi.c   pat2spi.h
