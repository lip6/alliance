AM_CFLAGS           =
lib_LTLIBRARIES       = libVbl.la
include_HEADERS     = vbl.h
libVbl_la_SOURCES    = vbl_bcomp_y.y vbl_bcomp_l.l \
vbl_bedef.h  vbl_bspec.c  vbl_byacc.h  vbl_util.c  vbl_utype.h \
vbl.h	vbl_blex.h   vbl_bspec.h  vbl_parse.c  vbl_util.h
libVbl_la_LDFLAGS = -version-info @VBL_DLL_VERSION@

CLEANFILES          = vbl_bcomp_y.c vbl_bcomp_y.h vbl_bcomp_l.c

vbl_bcomp_y.c vbl_bcomp_y.h : $(srcdir)/vbl_bcomp_y.y
	$(YACC) -d $(YFLAGS) $(srcdir)/vbl_bcomp_y.y && sed -e "s/yy/vbl_y_/g" -e "s/YY/VBL_Y_/g" y.tab.c > vbl_bcomp_y.c && sed -e "s/yy/vbl_y_/g" -e "s/YY/VBL_Y_/g" y.tab.h > vbl_bcomp_y.h
vbl_bcomp_l.c : $(srcdir)/vbl_bcomp_l.l vbl_bcomp_y.h
	$(LEX) -t $(srcdir)/vbl_bcomp_l.l | sed -e "s/yy/vbl_y_/g" -e "s/YY/VBL_Y_/g" > vbl_bcomp_l.c
