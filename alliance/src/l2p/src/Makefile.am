## Process this file with automake to produce Makefile.in

bin_PROGRAMS = l2p

AM_CFLAGS = @ALLIANCE_CFLAGS@ 

l2p_LDADD = @ALLIANCE_LIBS@ \
 ${top_builddir}/../mbk/src/libMlu.la ${top_builddir}/../mbk/src/libMpu.la ${top_builddir}/../mbkap/src/libMap.la ${top_builddir}/../mbkmg/src/libMmg.la ${top_builddir}/../mbkvti/src/libMcp.la ${top_builddir}/../mbkvti/src/libMcl.la ${top_builddir}/../mbkedif/src/libMel.la \
 ${top_builddir}/../mbkhilo/src/libMhl.la ${top_builddir}/../mbkvhdlg/src/libMvg.la ${top_builddir}/../mbk/src/libMlo.la ${top_builddir}/../mbk/src/libMph.la ${top_builddir}/../mbk/src/libMut.la ${top_builddir}/../mbkal/src/libMal.la ${top_builddir}/../mbk/src/libRcn.la \
 ${top_builddir}/../mbkspice/src/libMsl.la ${top_builddir}/../mbkvrlog/src/libMgl.la \
 ${top_builddir}/../rds/src/libRtl.la ${top_builddir}/../rdsgds/src/libRgs.la ${top_builddir}/../rdscif/src/libRcf.la ${top_builddir}/../rds/src/libRfm.la ${top_builddir}/../rds/src/libRpr.la ${top_builddir}/../rds/src/libRwi.la ${top_builddir}/../rds/src/libRut.la ${top_builddir}/../rds/src/libRds.la

l2p_SOURCES = \
drive_ps.c  rps_inc.h   tmp_man.c \
dict_color.ps  l2p.c	   tmp_dict.c
