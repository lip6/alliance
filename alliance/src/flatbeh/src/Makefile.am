## Process this file with automake to produce Makefile.in

AM_CFLAGS    = @ALLIANCE_CFLAGS@ 

bin_PROGRAMS = flatbeh

flatbeh_LDADD = @ALLIANCE_LIBS@ \
          ${top_builddir}/../abv/src/libAbv.la ${top_builddir}/../abe/src/libAbe.la ${top_builddir}/../abt/src/libAbt.la ${top_builddir}/../mbk/src/libMlu.la ${top_builddir}/../mbkvti/src/libMcl.la ${top_builddir}/../mbkal/src/libMal.la ${top_builddir}/../mbkspice/src/libMsl.la ${top_builddir}/../mbkhilo/src/libMhl.la ${top_builddir}/../mbkedif/src/libMel.la ${top_builddir}/../mbkvhdl/src/libMvl.la ${top_builddir}/../mbkvrlog/src/libMgl.la ${top_builddir}/../mbk/src/libRcn.la ${top_builddir}/../mbk/src/libMlo.la ${top_builddir}/../bdd/src/libBdd.la ${top_builddir}/../abl/src/libAbl.la ${top_builddir}/../aut/src/libAut.la ${top_builddir}/../mbk/src/libMut.la
	   
flatbeh_SOURCES = abstract.c abstract.h utils.c utils.h main.c

