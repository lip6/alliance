## Process this file with automake to produce Makefile.in

bin_PROGRAMS = s2r 

AM_CFLAGS = @ALLIANCE_CFLAGS@

s2r_LDADD = @ALLIANCE_LIBS@ \
          ${top_builddir}/../mbk/src/libMpu.la ${top_builddir}/../mbkvti/src/libMcp.la ${top_builddir}/../mbkap/src/libMap.la ${top_builddir}/../mbkmg/src/libMmg.la \
          ${top_builddir}/../rds/src/libRtl.la ${top_builddir}/../rdsgds/src/libRgs.la ${top_builddir}/../rdscif/src/libRcf.la ${top_builddir}/../rds/src/libRfm.la ${top_builddir}/../rds/src/libRpr.la ${top_builddir}/../rds/src/libRwi.la ${top_builddir}/../rds/src/libRut.la ${top_builddir}/../rds/src/libRds.la\
	  ${top_builddir}/../mbk/src/libMph.la ${top_builddir}/../mbk/src/libMut.la

s2r_SOURCES = generic.h  hash.h  maxima.h  merge.h\
              postrat.h rdsacces.h  statistics.c hash.c\
              main.c  merge.c postrat.c rdsacces.c\
              statistics.h
