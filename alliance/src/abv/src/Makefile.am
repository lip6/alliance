AM_CFLAGS           = @ALLIANCE_CFLAGS@
lib_LIBRARIES       = libAbv.a
include_HEADERS     = abv.h
libAbv_a_SOURCES    = bvl_bcomp_y.y bvl_bcomp_l.l \
abv.h	     bvl_bspec.c  bvl_drive.c  bvl_parse.h  bvl_util.h \
bvl_bedef.h  bvl_bspec.h  bvl_drive.h  bvl_utdef.h  bvl_utype.h \
bvl_blex.h   bvl_byacc.h  bvl_parse.c  bvl_util.c
CLEANFILES          = bvl_bcomp_y.c bvl_bcomp_y.h bvl_bcomp_l.c

bvl_bcomp_y.c bvl_bcomp_y.h : $(srcdir)/bvl_bcomp_y.y
	$(YACC) -d $(YFLAGS) $(srcdir)/bvl_bcomp_y.y && sed -e "s/yy/bvl_y_/g" -e "s/YY/BVL_Y_/g" y.tab.c > bvl_bcomp_y.c && sed -e "s/yy/bvl_y_/g" -e "s/YY/BVL_Y_/g" y.tab.h > bvl_bcomp_y.h

bvl_bcomp_l.c : $(srcdir)/bvl_bcomp_l.l bvl_bcomp_y.h
	$(LEX) -t $(srcdir)/bvl_bcomp_l.l | sed -e "s/yy/bvl_y_/g" -e "s/YY/BVL_Y_/g" > bvl_bcomp_l.c
