## Process this file with automake to produce Makefile.in
bin_PROGRAMS = lvx

AM_CFLAGS = @ALLIANCE_CFLAGS@

lvx_LDADD = @ALLIANCE_LIBS@ \
          ${top_builddir}/../mbk/src/libMlu.la \
	  ${top_builddir}/../mbkvti/src/libMcl.la \
	  ${top_builddir}/../mbkal/src/libMal.la \
	  ${top_builddir}/../mbkspice/src/libMsl.la \
	  ${top_builddir}/../mbkedif/src/libMel.la ${top_builddir}/../mbkvrlog/src/libMgl.la \
	  ${top_builddir}/../mbkhilo/src/libMhl.la \
	  ${top_builddir}/../mbkvhdlg/src/libMvg.la \
	  ${top_builddir}/../mbk/src/libMlo.la \
	  ${top_builddir}/../mbk/src/libMut.la \
	  ${top_builddir}/../mbk/src/libRcn.la 
	   
lvx_SOURCES = lvx.c
