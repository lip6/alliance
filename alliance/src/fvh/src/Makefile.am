AM_CFLAGS           = -I$(top_srcdir)/mbk/src \
                      -I$(top_srcdir)/aut/src \
                      -I$(top_srcdir)/abl/src \
                      -I$(top_srcdir)/bdd/src \
                      -I$(top_srcdir)/fsm/src
lib_LTLIBRARIES     = libFvh.la
include_HEADERS     = fvh.h
libFvh_la_SOURCES   = fbl_bcomp_y.y fbl_bcomp_l.l \
                      fbh_add.c fbh_del.c fbh_env.c fbh_fre.c fbh_rin.c fbh_rmv.c \
                      fbh_typ.c fbh_uti.c fbh_view.c \
                      fbl_bedef.h fbl_blex.h fbl_bspec.c fbl_bspec.h fbl_byacc.h \
                      fbl_drive.c fbl_drive.h fbl_utdef.h fbl_util.c fbl_util.h \
                      fbl_utype.h fvhdrive.c fvhdrive.h fvherror.c fvherror.h \
                      fvhfbh2fsm.c fvhfbh2fsm.h fvh.h fvhparse.c fvhparse.h

libFvh_la_LDFLAGS   = -version-info @FVH_DLL_VERSION@ -L../../fsm/src -L../../abl/src \
					  -L../../bdd/src -L../../aut/src -L../../mbk/src
libFvh_la_LIBADD    = -lFsm -lAbl -lBdd -lAut -lMut

CLEANFILES          = fbl_bcomp_y.c fbl_bcomp_y.h fbl_bcomp_l.c

fbl_bcomp_y.c fbl_bcomp_y.h : $(srcdir)/fbl_bcomp_y.y
	$(YACC) -d $(YFLAGS) $(srcdir)/fbl_bcomp_y.y && \
	$(SED) -e "s/yy/fbl_y_/g" -e "s/YY/FBL_Y_/g" y.tab.c > fbl_bcomp_y.c && \
	$(SED) -e "s/yy/fbl_y_/g" -e "s/YY/FBL_Y_/g" y.tab.h > fbl_bcomp_y.h
fbl_bcomp_l.c : $(srcdir)/fbl_bcomp_l.l fbl_bcomp_y.h
	$(LEX) -t $(srcdir)/fbl_bcomp_l.l | $(SED) -e "s/yy/fbl_y_/g" -e "s/YY/FBL_Y_/g" > fbl_bcomp_l.c
