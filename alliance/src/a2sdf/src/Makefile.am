## Process this file with automake to produce Makefile.in

AM_CXXFLAGS = @ALLIANCE_CFLAGS@ @POPT_CFLAGS@

bin_PROGRAMS = a2sdf

a2sdf_SOURCES = IOPath.cpp IOPath.h \
		TimingCell.cpp TimingCell.h \
		A2Sdf.cpp A2Sdf.h \
		a2sdf.cpp

a2sdf_LDADD = @ALLIANCE_LIBS@ @POPT_LIBS@ \
		-lAbv -lAbe -lAbt -lMlu \
		-lMcl -lMal -lMsl -lMhl \
		-lMel -lMvl -lMgl -lRcn \
		-lMlo -lBdd -lAbl -lAut -lMut
