lib_LIBRARIES       = libVbl.a
libVbl_a_SOURCES    = vbl_bcomp_y.y vbl_bcomp_l.l \
vbl_bedef.h  vbl_bspec.c  vbl_byacc.h  vbl_util.c  vbl_utype.h \
vbl.h	vbl_blex.h   vbl_bspec.h  vbl_parse.c  vbl_util.h

CLEANFILES          = vbl_bcompyac.c vbl_bcompyac.h vbl_bcomplex.c

vbl_bcompyac.c vbl_bcompyac.h : $(srcdir)/vbl_bcomp_y.y
	$(YACC) -d $(YFLAGS) $(srcdir)/vbl_bcomp_y.y && sed -e "s/yy/vbl_y_/g" -e "s/YY/VBL_Y_/g" y.tab.c > vbl_bcompyac.c && sed -e "s/yy/vbl_y_/g" -e "s/YY/VBL_Y_/g" y.tab.h > vbl_bcompyac.h
vbl_bcomplex.c : $(srcdir)/vbl_bcomp_l.l vbl_bcompyac.h
	$(LEX) -t $(srcdir)/vbl_bcomp_l.l | sed -e "s/yy/vbl_y_/g" -e "s/YY/VBL_Y_/g" > vbl_bcomplex.c
