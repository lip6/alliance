AM_CFLAGS           = -I$(top_srcdir)/mbk/src -I$(top_srcdir)/log/src -I$(top_srcdir)/beh/src -I$(top_srcdir)/bhl/src
lib_LTLIBRARIES     = libBvl.la
include_HEADERS     = bvl.h
libBvl_la_SOURCES   = bvl_bcomp_y.y bvl_bcomp_l.l  bvl_bcomp_y.h \
                      bvl_bedef.h   bvl_drive.c    bvl.h         bvl_util.c \
                      bvl_byacc.h   bvl_globals.c  bvl_parse.c
libBvl_la_LDFLAGS   = -version-info @BVL_DLL_VERSION@ -L../../bhl/src -L../../beh/src -L../../mbk/src
libBvl_la_LIBADD    = -lBhl -lBeh -lMut

CLEANFILES          = bvl_bcomp_y.c bvl_bcomp_y.h bvl_bcomp_l.c

bvl_bcomp_y.c bvl_bcomp_y.h : $(srcdir)/bvl_bcomp_y.y
	$(YACC) -d $(YFLAGS) $(srcdir)/bvl_bcomp_y.y && $(SED) -e "s/yy/bvl_y_/g" -e "s/YY/BVL_Y_/g" y.tab.c > bvl_bcomp_y.c && $(SED) -e "s/yy/bvl_y_/g" -e "s/YY/BVL_Y_/g" y.tab.h > bvl_bcomp_y.h
bvl_bcomp_l.c : $(srcdir)/bvl_bcomp_l.l bvl_bcomp_y.h
	$(LEX) -t $(srcdir)/bvl_bcomp_l.l | $(SED) -e "s/yy/bvl_y_/g" -e "s/YY/BVL_Y_/g" > bvl_bcomp_l.c
