## Process this file with automake to produce Makefile.in

YACC = @YACC@ -d
DEFS = @DEFS@ -DEMULBS_NBR_EMULBS=4 -DEMULBS_VERSION=\"@EMULBS_VERSION@\" -DPROGRAM_NAME=\"emulbs\"

AM_CFLAGS = @ALLIANCE_CFLAGS@

bin_PROGRAMS = emulbs

CLEANFILES          =  emul_y.c emul_y.h emul_l.c y.tab.h y.tab.c

INCLUDES = -I$(top_builddir)/src/emulbs -I$(top_srcdir)/src/emulbs 

emulbs_LDADD = @ALLIANCE_LIBS@ \
     -lMpu \
     -lMlu \
	  -lMlo \
     -lBhl \
	  -lBeh \
     -lPpt \
	  -lPhl \
	  -lPat \
     -lLog \
	  -lMut 

emulbs_SOURCES = emul31.h       global.h       infos.h        util.c \
    emul_l.l       hardware_io.c  traduction.c   util.h \
bstools.c      emul_y.y       hardware_io.h  traduction.h   verif.c \
bstools.h      emulbs.c       infos.c        type_infos.h   verif.h 


emul_y.c emul_y.h : $(srcdir)/emul_y.y
	$(YACC) $(YFLAGS) $(srcdir)/emul_y.y && sed -e "s/yy/emul_/g" -e "s/YY/EMUL_/g" y.tab.c > emul_y.c && sed -e "s/yy/emul_/g" -e "s/YY/EMUL_/g" y.tab.h > emul_y.h

emul_l.c : $(srcdir)/emul_l.l emul_y.h
	$(LEX) -t $(srcdir)/emul_l.l | sed -e "s/yy/emul_/g" -e "s/YY/EMUL_/g" > emul_l.c

