## Process this file with automake to produce Makefile.in

YACC = @YACC@ -d

INCLUDES = -I$(top_srcdir)/src/common -I$(top_srcdir)/src/placer

AM_CXXFLAGS = @ALLIANCE_CFLAGS@
AM_CFLAGS = @ALLIANCE_CFLAGS@

bin_PROGRAMS = ocp

noinst_LIBRARIES = libOcp.a

libOcp_a_SOURCES = PElem.cpp PIns.cpp PNet.cpp \
		   PCon.cpp

ocp_LDADD = @ALLIANCE_LIBS@ \
	   $(top_builddir)/src/common/libPCommon.a \
          ${top_builddir}/../mbk/src/libMpu.la ${top_builddir}/../mbk/src/libMlu.la \
	  ${top_builddir}/../mbkvti/src/libMcl.la ${top_builddir}/../mbkvti/src/libMcp.la \
	  ${top_builddir}/../mbkal/src/libMal.la ${top_builddir}/../mbkap/src/libMap.la \
	  ${top_builddir}/../mbkspice/src/libMsl.la \
	  ${top_builddir}/../mbkedif/src/libMel.la ${top_builddir}/../mbkvrlog/src/libMgl.la \
	  ${top_builddir}/../mbkhilo/src/libMhl.la \
	  ${top_builddir}/../mbkvhdlg/src/libMvg.la \
	  ${top_builddir}/../mbkmg/src/libMmg.la \
	  ${top_builddir}/../mbk/src/libMlo.la \
	  ${top_builddir}/../mbk/src/libMph.la ${top_builddir}/../mbk/src/libMut.la \
	  ${top_builddir}/../mbk/src/libRcn.la 
	   
ocp_SOURCES = Ocp.cpp PBin.cpp PCon.cpp \
	      PIns.cpp PMove.cpp PNet.cpp PONet.cpp \
	      PFixedIns.cpp \
	      PDetToPlaceIns.cpp \
	      PPlacement.cpp \
	      PPlacementGlobal.cpp \
	      PSubRow.cpp PRow.cpp \
	      PDetSubRow.cpp PElem.cpp\
	      PToPlaceIns.cpp \
	      PPlacementFinal.cpp \
	      PDetPlacement.cpp \
	      iocgram.y iocscan.l \
	      iocheader.h \
	      PBin.h PCon.h PElem.h PIns.h \
	      PMove.h PNet.h PONet.h \
	      PFixedIns.h PPlacement.h \
	      PSubRow.h PRow.h PToPlaceIns.h \
	      PDetSubRow.h PDetToPlaceIns.h \
	      PDetPlacement.h

EXTRA_DIST = iocgram.h

CLEANFILES = iocgram.c iocgram.h
