## Process this file with automake to produce Makefile.in

bin_PROGRAMS = growstk 

AM_CFLAGS = @ALLIANCE_CFLAGS@

growstk_LDADD = @ALLIANCE_LIBS@ \
          ${top_builddir}/../mbk/src/libMpu.la ${top_builddir}/../mbkvti/src/libMcp.la ${top_builddir}/../mbkap/src/libMap.la ${top_builddir}/../mbkmg/src/libMmg.la \
          ${top_builddir}/../rds/src/libRtl.la ${top_builddir}/../rdsgds/src/libRgs.la ${top_builddir}/../rdscif/src/libRcf.la ${top_builddir}/../rds/src/libRfm.la ${top_builddir}/../rds/src/libRpr.la ${top_builddir}/../rds/src/libRwi.la ${top_builddir}/../rds/src/libRut.la ${top_builddir}/../rds/src/libRds.la\
	  ${top_builddir}/../mbk/src/libMph.la ${top_builddir}/../mbk/src/libMut.la

growstk_SOURCES = hash.h hash.c growstk.c 
