lib_LIBRARIES       = libAbv.a
libAbv_a_SOURCES    = bvl_bcomp.y bvl_bcomp.l \
abv.h	     bvl_bspec.c  bvl_drive.c  bvl_parse.h  bvl_util.h \
bvl_bedef.h  bvl_bspec.h  bvl_drive.h  bvl_utdef.h  bvl_utype.h \
bvl_blex.h   bvl_byacc.h  bvl_parse.c  bvl_util.c
CLEANFILES          = bvl_bcompyac.c bvl_bcompyac.h bvl_bcomplex.c

bvl_bcompyac.c bvl_bcompyac.h : $(srcdir)/bvl_bcomp.y
	$(YACC) -d $(YFLAGS) $(srcdir)/bvl_bcomp.y && sed -e "s/yy/bvl_y_/g" -e "s/YY/BVL_Y_/g" y.tab.c > bvl_bcompyac.c && sed -e "s/yy/bvl_y_/g" -e "s/YY/BVL_Y_/g" y.tab.h > bvl_bcompyac.h
bvl_bcomplex.c : $(srcdir)/bvl_bcomp.l bvl_bcompyac.h
	$(LEX) -t $(srcdir)/bvl_bcomp.l | sed -e "s/yy/bvl_y_/g" -e "s/YY/BVL_Y_/g" > bvl_bcomplex.c
