## Process this file with automake to produce Makefile.in

YACC = @YACC@ -d

INCLUDES = -I$(srcdir)/../common

AM_CXXFLAGS = @ALLIANCE_CFLAGS@ -I$(top_srcdir)/mbk/src -std=c++0x
AM_CFLAGS = @ALLIANCE_CFLAGS@ -I$(top_srcdir)/mbk/src

bin_PROGRAMS = ocp

noinst_LIBRARIES = libOcp.a

libOcp_a_SOURCES = PElem.cpp PIns.cpp PNet.cpp \
		   PCon.cpp

ocp_LDADD = @ALLIANCE_LIBS@ \
	  ../common/libPCommon.a \
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
          -lMpu -lMlu \
	  -lMcl -lMcp \
	  -lMal -lMap \
	  -lMsl \
	  -lMel -lMgl \
	  -lMhl \
	  -lMvg \
	  -lMmg \
	  -lMlo \
	  -lMph -lMut \
	  -lRcn 
	   
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
