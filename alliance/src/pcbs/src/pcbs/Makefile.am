## Process this file with automake to produce Makefile.in

YACC = @YACC@ -d
DEFS = @DEFS@ -DPCBS_PGA_SIZE=17 -DPCBS_NBR_EMULBS=4 -DPCBS_VERSION=\"@PCBS_VERSION@\" -DPROGRAM_NAME=\"pcbs\"


AM_CFLAGS = @ALLIANCE_CFLAGS@

bin_PROGRAMS = pcbs

CLEANFILES          =  emul_y.c emul_y.h pga_y.c pga_y.h pga_l.c emul_l.c y.tab.c y.tab.h

INCLUDES = -I$(top_builddir)/src/pcbs -I$(top_srcdir)/src/bvl \
-I$(top_srcdir)/src/pcbs

pcbs_LDADD = \
	   -L$(top_builddir)/src/bvl -lBvl \
     @ALLIANCE_LIBS@ \
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

