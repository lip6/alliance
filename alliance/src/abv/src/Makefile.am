AM_CFLAGS           = -I$(top_srcdir)/mbk/src \
                      -I$(top_srcdir)/aut/src \
                      -I$(top_srcdir)/abl/src \
                      -I$(top_srcdir)/bdd/src \
                      -I$(top_srcdir)/abe/src
lib_LTLIBRARIES     = libAbv.la
include_HEADERS     = abv.h
libAbv_la_SOURCES   = bvl_bcomp_y.y bvl_bcomp_l.l \
                      abv.h	     bvl_bspec.c  bvl_drive.c  bvl_parse.h  bvl_util.h \
                      bvl_bedef.h  bvl_bspec.h  bvl_drive.h  bvl_utdef.h  bvl_utype.h \
                      bvl_blex.h   bvl_byacc.h  bvl_parse.c  bvl_util.c
libAbv_la_LIBADD    = -lAbe -lBdd -lMut

CLEANFILES          = bvl_bcomp_y.c bvl_bcomp_y.h bvl_bcomp_l.c


bvl_bcomp_y.c bvl_bcomp_y.h : $(srcdir)/bvl_bcomp_y.y
	$(YACC) -d $(YFLAGS) $(srcdir)/bvl_bcomp_y.y && $(SED) -e "s/yy/bvl_y_/g" -e "s/YY/BVL_Y_/g" y.tab.c > bvl_bcomp_y.c && $(SED) -e "s/yy/bvl_y_/g" -e "s/YY/BVL_Y_/g" y.tab.h > bvl_bcomp_y.h

bvl_bcomp_l.c : $(srcdir)/bvl_bcomp_l.l bvl_bcomp_y.h
	$(LEX) -t $(srcdir)/bvl_bcomp_l.l | $(SED) -e "s/yy/bvl_y_/g" -e "s/YY/BVL_Y_/g" > bvl_bcomp_l.c

libAbv_la_LDFLAGS = -version-info @ABV_DLL_VERSION@ -L../../abe/src -L../../bdd/src -L../../mbk/src
