lib_LIBRARIES       = libFvh.a
include_HEADERS     = fvh.h
libFvh_a_SOURCES    = fbl_bcomp_y.y fbl_bcomp_l.l \
fbh_add.c fbh_del.c fbh_env.c fbh_fre.c fbh_rin.c fbh_rmv.c \
fbh_typ.c fbh_uti.c fbh_view.c \
fbl_bedef.h fbl_blex.h fbl_bspec.c fbl_bspec.h fbl_byacc.h \
fbl_drive.c fbl_drive.h fbl_utdef.h fbl_util.c fbl_util.h \
fbl_utype.h fvhdrive.c fvhdrive.h fvherror.c fvherror.h \
fvhfbh2fsm.c fvhfbh2fsm.h fvh.h fvhparse.c fvhparse.h

CLEANFILES          = fbl_bcomp_y.c fbl_bcomp_y.h fbl_bcomp_l.c

fbl_bcomp_y.c fbl_bcomp_y.h : $(srcdir)/fbl_bcomp_y.y
	$(YACC) -d $(YFLAGS) $(srcdir)/fbl_bcomp_y.y && sed -e "s/yy/fbl_y_/g" -e "s/YY/FBL_Y_/g" y.tab.c > fbl_bcomp_y.c && sed -e "s/yy/fbl_y_/g" -e "s/YY/FBL_Y_/g" y.tab.h > fbl_bcomp_y.h
fbl_bcomp_l.c : $(srcdir)/fbl_bcomp_l.l fbl_bcomp_y.h
	$(LEX) -t $(srcdir)/fbl_bcomp_l.l | sed -e "s/yy/fbl_y_/g" -e "s/YY/FBL_Y_/g" > fbl_bcomp_l.c
