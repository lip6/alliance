## Process this file with automake to produce Makefile.in

INCLUDES = -I$(srcdir)/../include -I$(top_srcdir)/mbk/src

AM_CFLAGS = @ALLIANCE_CFLAGS@
AM_CXXFLAGS = @ALLIANCE_CFLAGS@

bin_PROGRAMS = ocr

ocr_LDADD = @ALLIANCE_LIBS@ \
	   ../seg/libocrPath.a \
	   ../util/libocrUtil.a \
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
	  -lMlu -lMlo -lMal -lMcl -lMel -lMhl \
	  -lMgl -lMsl -lMvg -lMpu -lMph -lMap \
	  -lMcp -lMmg -lMut -lRcn

ocr_SOURCES = display.c findNPointsPath.c getOption.c\
		ocrGlobalRouting.c ocrMst.c ocrRouter.c\
		ocrWeightedTree.c ocrAstar.cpp
