## Process this file with automake to produce Makefile.in

YACC = @YACC@ -d

INCLUDES = -I$(top_srcdir)/src/common -I$(top_srcdir)/src/placer

CXXFLAGS = @ALLIANCE_CFLAGS@
CFLAGS = @ALLIANCE_CFLAGS@

bin_PROGRAMS = ocp

noinst_LIBRARIES = libOcp.a

libOcp_a_SOURCES = PElem.cpp PIns.cpp PNet.cpp \
		   PCon.cpp

ocp_LDADD = @ALLIANCE_LIBS@ \
	   $(top_builddir)/src/common/libPCommon.a \
          -lMpu -lMlu \
	  -lMcl -lMcp \
	  -lMal -lMap \
	  -lMsl \
	  -lMel -lMgl \
	  -lMhl \
	  -lMvl \
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

CLEANFILES = iocgram.c iocgram.h
