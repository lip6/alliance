## Process this file with automake to produce Makefile.in

YACC = @YACC@ -d


AM_CFLAGS = @ALLIANCE_CFLAGS@


CLEANFILES          =  bvl_bcomp_y.c bvl_bcomp_y.h bvl_bcomp_l.c y.tab.h y.tab.c

noinst_LIBRARIES = libBvl.a
	   
libBvl_a_SOURCES = bvl999.h       bvl_blex.h     bvl_drive.c    bvl_utdef.h \
bvl_bcomp_l.l  bvl_bspec.c    bvl_drive.h    bvl_util.c \
bvl_bcomp_y.y  bvl_bspec.h    bvl_parse.c    bvl_util.h \
bvl_bedef.h    bvl_byacc.h    bvl_parse.h    bvl_utype.h 


bvl_bcomp_y.c bvl_bcomp_y.h : $(srcdir)/bvl_bcomp_y.y
	$(YACC) $(YFLAGS) $(srcdir)/bvl_bcomp_y.y && sed -e "s/yy/bvl_y_/g" -e "s/YY/BVL_Y_/g" y.tab.c > bvl_bcomp_y.c && sed -e "s/yy/bvl_y_/g" -e "s/YY/BVL_Y_/g" y.tab.h > bvl_bcomp_y.h

bvl_bcomp_l.c : $(srcdir)/bvl_bcomp_l.l bvl_bcomp_y.h
	$(LEX) -t $(srcdir)/bvl_bcomp_l.l | sed -e "s/yy/bvl_y_/g" -e "s/YY/BVL_Y_/g" > bvl_bcomp_l.c

