## Process this file with automake to produce Makefile.in

bin_PROGRAMS = scapin

AM_CFLAGS = @ALLIANCE_CFLAGS@ \
         -DSCAPIN_DEFAULT_PARAM_NAME=\"etc/sxlib.scapin\"

scapin_LDADD = @ALLIANCE_LIBS@ \
${top_builddir}/../mbk/src/libMlu.la ${top_builddir}/../mbkvti/src/libMcl.la ${top_builddir}/../mbkedif/src/libMel.la ${top_builddir}/../mbkhilo/src/libMhl.la ${top_builddir}/../mbkvhdlg/src/libMvg.la ${top_builddir}/../mbk/src/libMlo.la ${top_builddir}/../mbk/src/libMut.la ${top_builddir}/../mbkal/src/libMal.la ${top_builddir}/../mbk/src/libRcn.la ${top_builddir}/../mbkspice/src/libMsl.la ${top_builddir}/../mbkvrlog/src/libMgl.la ${top_builddir}/../aut/src/libAut.la

scapin_SOURCES = \
scan_insert.c scan_insert.h scan_main.c scan_main.h \
scan_param.c scan_param.h scan_path.c scan_path.h
