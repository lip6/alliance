# $Id: Makefile.am,v 1.6 2002/09/23 15:17:06 pnt Exp $

AM_CFLAGS = @ALLIANCE_CFLAGS@ 

bin_PROGRAMS = x2y

x2y_LDADD = @ALLIANCE_LIBS@ \
          ${top_builddir}/../mbk/src/libMpu.la ${top_builddir}/../mbk/src/libMlu.la \
	  ${top_builddir}/../mbkvti/src/libMcl.la ${top_builddir}/../mbkvti/src/libMcp.la \
	  ${top_builddir}/../mbkal/src/libMal.la \
          ${top_builddir}/../mbkap/src/libMap.la \
	  ${top_builddir}/../mbkspice/src/libMsl.la \
	  ${top_builddir}/../mbkedif/src/libMel.la \
          ${top_builddir}/../mbkvrlog/src/libMgl.la \
	  ${top_builddir}/../mbkhilo/src/libMhl.la \
	  ${top_builddir}/../mbkvhdl/src/libMvl.la \
	  ${top_builddir}/../mbkmg/src/libMmg.la \
	  ${top_builddir}/../mbk/src/libMlo.la \
	  ${top_builddir}/../mbk/src/libMph.la \
          ${top_builddir}/../mbk/src/libMut.la \
	  ${top_builddir}/../mbk/src/libRcn.la 
	  
x2y_SOURCES = x2y.c
