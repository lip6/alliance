## Process this file with automake to produce Makefile.in

INCLUDES = -I$(top_srcdir)/src/include

AM_CFLAGS = @ALLIANCE_CFLAGS@
AM_CXXFLAGS = @ALLIANCE_CFLAGS@

bin_PROGRAMS = ocr

ocr_LDADD = @ALLIANCE_LIBS@ \
	   $(top_builddir)/src/seg/libocrPath.a \
	   $(top_builddir)/src/util/libocrUtil.a \
	  ${top_builddir}/../mbk/src/libMlu.la ${top_builddir}/../mbk/src/libMlo.la ${top_builddir}/../mbkal/src/libMal.la ${top_builddir}/../mbkvti/src/libMcl.la ${top_builddir}/../mbkedif/src/libMel.la ${top_builddir}/../mbkhilo/src/libMhl.la \
	  ${top_builddir}/../mbkvrlog/src/libMgl.la ${top_builddir}/../mbkspice/src/libMsl.la ${top_builddir}/../mbkvhdlg/src/libMvg.la ${top_builddir}/../mbk/src/libMpu.la ${top_builddir}/../mbk/src/libMph.la ${top_builddir}/../mbkap/src/libMap.la \
	  ${top_builddir}/../mbkvti/src/libMcp.la ${top_builddir}/../mbkmg/src/libMmg.la ${top_builddir}/../mbk/src/libMut.la ${top_builddir}/../mbk/src/libRcn.la

ocr_SOURCES = display.c findNPointsPath.c getOption.c\
		ocrGlobalRouting.c ocrMst.c ocrRouter.c\
		ocrWeightedTree.c ocrAstar.cpp
