## Process this file with automake to produce Makefile.in
bin_PROGRAMS = lvx

AM_CFLAGS = @ALLIANCE_CFLAGS@ -I$(top_srcdir)/mbk/src

lvx_LDADD = @ALLIANCE_LIBS@ \
-L$(top_srcdir)/mbk/src \
-L$(top_srcdir)/mbkal/src \
-L$(top_srcdir)/mbkedif/src \
-L$(top_srcdir)/mbkhilo/src \
-L$(top_srcdir)/mbkspice/src \
-L$(top_srcdir)/mbkvhdlg/src \
-L$(top_srcdir)/mbkvrlog/src \
-L$(top_srcdir)/mbkvti/src \
          -lMlu \
	  -lMcl \
	  -lMal \
	  -lMsl \
	  -lMel -lMgl \
	  -lMhl \
	  -lMvg \
	  -lMlo \
	  -lMut \
	  -lRcn 
	   
lvx_SOURCES = lvx.c
