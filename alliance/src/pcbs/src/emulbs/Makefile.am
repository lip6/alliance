## Process this file with automake to produce Makefile.in

YACC = @YACC@ -d
DEFS = @DEFS@ -DEMULBS_NBR_EMULBS=4 -DEMULBS_BS_SIZE=31 -DEMULBS_VERSION=\"@EMULBS_VERSION@\" -DPROGRAM_NAME=\"emulbs\"

AM_CFLAGS = @ALLIANCE_CFLAGS@ \
-I$(top_srcdir)/beh/src \
-I$(top_srcdir)/log/src \
-I$(top_srcdir)/mbk/src \
-I$(top_srcdir)/pat/src \
-I$(top_srcdir)/phl/src \
-I$(top_srcdir)/ppt/src

bin_PROGRAMS = emulbs

CLEANFILES          =  emul_y.c emul_y.h emul_l.c y.tab.h y.tab.c

INCLUDES = -I$(srcdir)/../emulbs 

emulbs_LDADD = @ALLIANCE_LIBS@ \
-L$(top_srcdir)/asimut/src \
-L$(top_srcdir)/beh/src \
-L$(top_srcdir)/bhl/src \
-L$(top_srcdir)/genlib/src \
-L$(top_srcdir)/log/src \
-L$(top_srcdir)/mbk/src \
-L$(top_srcdir)/mbkal/src \
-L$(top_srcdir)/mbkap/src \
-L$(top_srcdir)/mbkedif/src \
-L$(top_srcdir)/mbkhilo/src \
-L$(top_srcdir)/mbkmg/src \
-L$(top_srcdir)/mbkspice/src \
-L$(top_srcdir)/mbkvhdl/src \
-L$(top_srcdir)/mbkvhdlg/src \
-L$(top_srcdir)/mbkvrlog/src \
-L$(top_srcdir)/mbkvti/src \
-L$(top_srcdir)/pat/src \
-L$(top_srcdir)/phl/src \
-L$(top_srcdir)/ppt/src \
     -lMpu \
     -lMal \
     -lMap \
     -lMcl \
     -lMcp \
     -lMel \
     -lMgl \
     -lMsl \
     -lMhl \
     -lMvg \
     -lMvl \
     -lMgn \
     -lMmg \
     -lMph \
     -lRcn \
     -lMpu \
     -lMlu \
     -lMlo \
     -lBhl \
     -lBeh \
     -lPpt \
     -lPhl \
     -lPat \
     -lLog \
     -lCst \
     -lMut


emulbs_SOURCES = emul31.h       global.h       infos.h        util.c \
    emul_l.l       hardware_io.c  traduction.c   util.h \
bstools.c      emul_y.y       hardware_io.h  traduction.h   verif.c \
bstools.h      emulbs.c       infos.c        type_infos.h   verif.h 


emul_y.c emul_y.h : $(srcdir)/emul_y.y
	$(YACC) $(YFLAGS) $(srcdir)/emul_y.y && sed -e "s/yy/emul_/g" -e "s/YY/EMUL_/g" y.tab.c > emul_y.c && sed -e "s/yy/emul_/g" -e "s/YY/EMUL_/g" y.tab.h > emul_y.h

emul_l.c : $(srcdir)/emul_l.l emul_y.h
	$(LEX) -t $(srcdir)/emul_l.l | sed -e "s/yy/emul_/g" -e "s/YY/EMUL_/g" > emul_l.c

