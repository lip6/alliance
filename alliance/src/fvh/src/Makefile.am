lib_LIBRARIES       = libFvh.a
libFvh_a_SOURCES    = fbl_bcomp.y fbl_bcomp.l \
fbh_add.c  fbh_rmv.c	fbl_blex.h   fbl_drive.h  fvh.h       fvhfbh2fsm.c \
fbh_del.c  fbh_typ.c	fbl_bspec.c  fbl_utdef.h  fvhdrive.c  fvhfbh2fsm.h \
fbh_env.c  fbh_uti.c	fbl_bspec.h  fbl_util.c   fvhdrive.h  fvhparse.c \
fbh_fre.c  fbh_view.c	fbl_byacc.h  fbl_util.h   fvherror.c  fvhparse.h \
fbh_rin.c  fbl_bedef.h	fbl_drive.c  fbl_utype.h  fvherror.h

CLEANFILES          = fbl_bcompyac.c fbl_bcompyac.h fbl_bcomplex.c

fbl_bcompyac.c fbl_bcompyac.h : $(srcdir)/fbl_bcomp.y
	$(YACC) -d $(YFLAGS) $(srcdir)/fbl_bcomp.y && sed -e "s/yy/fbl_y_/g" -e "s/YY/FBL_Y_/g" y.tab.c > fbl_bcompyac.c && sed -e "s/yy/fbl_y_/g" -e "s/YY/FBL_Y_/g" y.tab.h > fbl_bcompyac.h
fbl_bcomplex.c : $(srcdir)/fbl_bcomp.l fbl_bcompyac.h
	$(LEX) -t $(srcdir)/fbl_bcomp.l | sed -e "s/yy/fbl_y_/g" -e "s/YY/FBL_Y_/g" > fbl_bcomplex.c
