## Process this file with automake to produce Makefile.in

YACC = @YACC@ -d
DEFS = @DEFS@ -DEMULBS_NBR_EMULBS=4 -DEMULBS_VERSION=\"@EMULBS_VERSION@\" -DPROGRAM_NAME=\"emulbs\"

AM_CFLAGS = @ALLIANCE_CFLAGS@

bin_PROGRAMS = emulbs

CLEANFILES          =  emul_y.c emul_y.h emul_l.c y.tab.h y.tab.c

INCLUDES = -I$(top_builddir)/src/emulbs -I$(top_srcdir)/src/emulbs 

emulbs_LDADD = @ALLIANCE_LIBS@ \
     ${top_builddir}/../mbk/src/libMpu.la \
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


emulbs_SOURCES = emul31.h       global.h       infos.h        util.c \
    emul_l.l       hardware_io.c  traduction.c   util.h \
bstools.c      emul_y.y       hardware_io.h  traduction.h   verif.c \
bstools.h      emulbs.c       infos.c        type_infos.h   verif.h 


emul_y.c emul_y.h : $(srcdir)/emul_y.y
	$(YACC) $(YFLAGS) $(srcdir)/emul_y.y && sed -e "s/yy/emul_/g" -e "s/YY/EMUL_/g" y.tab.c > emul_y.c && sed -e "s/yy/emul_/g" -e "s/YY/EMUL_/g" y.tab.h > emul_y.h

emul_l.c : $(srcdir)/emul_l.l emul_y.h
	$(LEX) -t $(srcdir)/emul_l.l | sed -e "s/yy/emul_/g" -e "s/YY/EMUL_/g" > emul_l.c

