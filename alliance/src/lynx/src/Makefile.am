## Process this file with automake to produce Makefile.in

bin_PROGRAMS = cougar flatrds

AM_CFLAGS = @ALLIANCE_CFLAGS@ 

cougar_LDADD = @ALLIANCE_LIBS@ \
 ${top_builddir}/../mbk/src/libMlu.la ${top_builddir}/../mbk/src/libMpu.la ${top_builddir}/../mbkap/src/libMap.la ${top_builddir}/../mbkmg/src/libMmg.la ${top_builddir}/../mbkvti/src/libMcp.la ${top_builddir}/../mbkvti/src/libMcl.la ${top_builddir}/../mbkedif/src/libMel.la \
 ${top_builddir}/../mbkhilo/src/libMhl.la ${top_builddir}/../mbkvhdl/src/libMvl.la ${top_builddir}/../mbk/src/libMlo.la ${top_builddir}/../mbk/src/libMph.la ${top_builddir}/../mbk/src/libMut.la ${top_builddir}/../mbkal/src/libMal.la ${top_builddir}/../mbk/src/libRcn.la \
 ${top_builddir}/../mbkspice/src/libMsl.la ${top_builddir}/../mbkvrlog/src/libMgl.la \
 ${top_builddir}/../rds/src/libRtl.la ${top_builddir}/../rdsgds/src/libRgs.la ${top_builddir}/../rdscif/src/libRcf.la ${top_builddir}/../rds/src/libRfm.la ${top_builddir}/../rds/src/libRpr.la ${top_builddir}/../rds/src/libRwi.la ${top_builddir}/../rds/src/libRut.la ${top_builddir}/../rds/src/libRds.la

flatrds_LDADD = @ALLIANCE_LIBS@ \
 ${top_builddir}/../mbk/src/libMlu.la ${top_builddir}/../mbk/src/libMpu.la ${top_builddir}/../mbkap/src/libMap.la ${top_builddir}/../mbkmg/src/libMmg.la ${top_builddir}/../mbkvti/src/libMcp.la ${top_builddir}/../mbkvti/src/libMcl.la ${top_builddir}/../mbkedif/src/libMel.la \
 ${top_builddir}/../mbkhilo/src/libMhl.la ${top_builddir}/../mbkvhdl/src/libMvl.la ${top_builddir}/../mbk/src/libMlo.la ${top_builddir}/../mbk/src/libMph.la ${top_builddir}/../mbk/src/libMut.la ${top_builddir}/../mbkal/src/libMal.la ${top_builddir}/../mbk/src/libRcn.la \
 ${top_builddir}/../mbkspice/src/libMsl.la ${top_builddir}/../mbkvrlog/src/libMgl.la \
 ${top_builddir}/../rds/src/libRtl.la ${top_builddir}/../rdsgds/src/libRgs.la ${top_builddir}/../rdscif/src/libRcf.la ${top_builddir}/../rds/src/libRfm.la ${top_builddir}/../rds/src/libRpr.la ${top_builddir}/../rds/src/libRwi.la ${top_builddir}/../rds/src/libRut.la ${top_builddir}/../rds/src/libRds.la

flatrds_SOURCES = flatrds.c

cougar_SOURCES = \
addlynxrcn.c addlynxrcn.h box.c box.h buildrcn.c \
buildrcn.h bulk.c bulk.h chklynxrcn.c chklynxrcn.h \
cutelbow.c cutelbow.h diffusion.c diffusion.h error.c \
error.h extract.c extract.h hole.c \
hole.h inter.c inter.h interval.c interval.h \
lynx.c lynx.h lynxrcn.h mbkrds.c mbkrds.h netlist.c \
netlist.h parse.c parse.h pattern.c pattern.h via.h \
Windows95.c Windows95.h znc.c znc.h
