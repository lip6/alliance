AM_CFLAGS           = @ALLIANCE_CFLAGS@ 
lib_LTLIBRARIES     = libBvl.la
include_HEADERS     = bvl.h
libBvl_la_SOURCES    = bvl_bcomp_y.y bvl_bcomp_l.l \
bvl_bedef.h  bvl_drive.c    bvl.h	 bvl_util.c \
bvl_byacc.h  bvl_globals.c  bvl_parse.c

CLEANFILES          = bvl_bcomp_y.c bvl_bcomp_y.h bvl_bcomp_l.c

bvl_bcomp_y.c bvl_bcomp_y.h : $(srcdir)/bvl_bcomp_y.y
	$(YACC) -d $(YFLAGS) $(srcdir)/bvl_bcomp_y.y && sed -e "s/yy/bvl_y_/g" -e "s/YY/BVL_Y_/g" y.tab.c > bvl_bcomp_y.c && sed -e "s/yy/bvl_y_/g" -e "s/YY/BVL_Y_/g" y.tab.h > bvl_bcomp_y.h
bvl_bcomp_l.c : $(srcdir)/bvl_bcomp_l.l bvl_bcomp_y.h
	$(LEX) -t $(srcdir)/bvl_bcomp_l.l | sed -e "s/yy/bvl_y_/g" -e "s/YY/BVL_Y_/g" > bvl_bcomp_l.c

LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck

