## Process this file with automake to produce Makefile.in

YACC = @YACC@ -d
DEFS = @DEFS@ -DPCBS_PGA_SIZE=17 -DPCBS_NBR_EMULBS=4 -DPCBS_VERSION=\"@PCBS_VERSION@\" -DPROGRAM_NAME=\"pcbs\"


AM_CFLAGS = @ALLIANCE_CFLAGS@

bin_PROGRAMS = pcbs

CLEANFILES          =  emul_y.c emul_y.h pga_y.c pga_y.h pga_l.c emul_l.c y.tab.c y.tab.h

INCLUDES = -I$(top_builddir)/src/pcbs -I$(top_srcdir)/src/bvl \
-I$(top_srcdir)/src/pcbs

pcbs_LDADD = \
	   -L$(top_builddir)/src/bvl ${top_builddir}/../bvl/src/libBvl.la \
     @ALLIANCE_LIBS@ \
     ${top_builddir}/../mbkal/src/libMal.la \
     ${top_builddir}/../mbkap/src/libMap.la \
     ${top_builddir}/../mbkvti/src/libMcl.la \
     ${top_builddir}/../mbkvti/src/libMcp.la \
     ${top_builddir}/../mbkedif/src/libMel.la \
     ${top_builddir}/../mbkvrlog/src/libMgl.la \
     ${top_builddir}/../mbkspice/src/libMsl.la \
     ${top_builddir}/../mbkhilo/src/libMhl.la \
     ${top_builddir}/../mbkvhdlg/src/libMvg.la \
     ${top_builddir}/../mbkvhdl/src/libMvl.la \
     ${top_builddir}/../genlib/src/libMgn.la \
     ${top_builddir}/../mbkmg/src/libMmg.la \
     ${top_builddir}/../mbk/src/libMph.la \
     ${top_builddir}/../mbk/src/libRcn.la \
     ${top_builddir}/../mbk/src/libMpu.la \
     ${top_builddir}/../mbk/src/libMlu.la \
     ${top_builddir}/../mbk/src/libMlo.la \
     ${top_builddir}/../bhl/src/libBhl.la \
     ${top_builddir}/../beh/src/libBeh.la \
     ${top_builddir}/../ppt/src/libPpt.la \
     ${top_builddir}/../phl/src/libPhl.la \
     ${top_builddir}/../pat/src/libPat.la \
     ${top_builddir}/../log/src/libLog.la \
     ${top_builddir}/../asimut/src/libCst.la \
     ${top_builddir}/../mbk/src/libMut.la
	   
pcbs_SOURCES = emul_y.y pga_y.c emul_l.c pga_l.c \
               bstools.c     parseur.c       \
               traduction.c  verif.c \
               infos.c       pcbs.c        sig.c         util.c \
               bstools.h     global.h      pcbs.h        traduction.h  \
               verif.h emul31.h      infos.h         \
               type_infos.h parseur.h     sig.h   util.h


emul_y.c emul_y.h : $(srcdir)/emul_y.y
	$(YACC) $(YFLAGS) $(srcdir)/emul_y.y && sed -e "s/yy/emul_/g" -e "s/YY/EMUL_/g" y.tab.c > emul_y.c && sed -e "s/yy/emul_/g" -e "s/YY/EMUL_/g" y.tab.h > emul_y.h

pga_y.c pga_y.h : $(srcdir)/pga_y.y
	$(YACC) $(YFLAGS) $(srcdir)/pga_y.y && sed -e "s/yy/pga_/g" -e "s/YY/PGA_/g" y.tab.c > pga_y.c && sed -e "s/yy/pga_/g" -e "s/YY/PGA_/g" y.tab.h > pga_y.h

emul_l.c : $(srcdir)/lexeur_l.l emul_y.h
	$(LEX) -t $(srcdir)/lexeur_l.l | sed -e "s/yy/emul_/g" -e "s/YY/EMUL_/g" > emul_l.c

pga_l.c : $(srcdir)/lexeur_l.l pga_y.h
	$(LEX) -t $(srcdir)/lexeur_l.l | sed -e "s/yy/pga_/g" -e "s/YY/PGA_/g" > pga_l.c

