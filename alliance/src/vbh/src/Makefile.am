AM_CFLAGS           = -I$(top_srcdir)/mbk/src \
                      -I$(top_srcdir)/aut/src \
                      -I$(top_srcdir)/vex/src \
                      -I$(top_srcdir)/vpn/src
lib_LTLIBRARIES     = libVbh.la
include_HEADERS     = vbh.h vbl.h vvh.h vtl.h vpd.h
libVbh_la_SOURCES   = vbh.h  vbh_add.c  vbh_crt.c  vbh_dup.c	vbh_fre.c  vbh_simp.c  vbh_view.c \
                      vbl_bcomp_y.y vbl_bcomp_l.l \
                      vbl_bedef.h  vbl_bspec.c  vbl_byacc.h  vbl_util.c  vbl_utype.h \
                      vbl.h	vbl_blex.h   vbl_bspec.h  vbl_parse.c  vbl_util.h \
                      vvh.h vvh_drive.h  vvh_error.h  vvh_parse.h    vvh_vbh2vpn.h \
                      vvh_drive.c  vvh_error.c  vvh_parse.c  vvh_vbh2vpn.c \
                      vtl.h  vtlacces.c	vtlacces.h  vtlerror.c	vtlerror.h \
                      vpd.h          vpd_drive.h  vpd_error.h  vpd_parse.h \
                      vpd_drive.c  vpd_error.c  vpd_parse.c
libVbh_la_LIBADD    = -lVpn -lVex -lAut -lMut

CLEANFILES          = vbl_bcomp_y.c vbl_bcomp_y.h vbl_bcomp_l.c

vbl_bcomp_y.c vbl_bcomp_y.h : $(srcdir)/vbl_bcomp_y.y
	$(YACC) -d $(YFLAGS) $(srcdir)/vbl_bcomp_y.y && sed -e "s/yy/vbl_y_/g" -e "s/YY/VBL_Y_/g" y.tab.c > vbl_bcomp_y.c && sed -e "s/yy/vbl_y_/g" -e "s/YY/VBL_Y_/g" y.tab.h > vbl_bcomp_y.h
vbl_bcomp_l.c : $(srcdir)/vbl_bcomp_l.l vbl_bcomp_y.h
	$(LEX) -t $(srcdir)/vbl_bcomp_l.l | sed -e "s/yy/vbl_y_/g" -e "s/YY/VBL_Y_/g" > vbl_bcomp_l.c
