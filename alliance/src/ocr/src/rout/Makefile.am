## Process this file with automake to produce Makefile.in

INCLUDES = -I$(top_srcdir)/src/include

AM_CFLAGS = @ALLIANCE_CFLAGS@
AM_CXXFLAGS = @ALLIANCE_CFLAGS@

bin_PROGRAMS = ocr

ocr_LDADD = @ALLIANCE_LIBS@ \
	   $(top_builddir)/src/seg/libocrPath.a \
	   $(top_builddir)/src/util/libocrUtil.a \
	  -lMlu -lMlo -lMal -lMcl -lMel -lMhl \
	  -lMgl -lMsl -lMvg -lMpu -lMph -lMap \
	  -lMcp -lMmg -lMut -lRcn

ocr_SOURCES = display.c findNPointsPath.c getOption.c\
		ocrGlobalRouting.c ocrMst.c ocrRouter.c\
		ocrWeightedTree.c ocrAstar.cpp
