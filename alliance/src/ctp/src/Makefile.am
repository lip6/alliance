AM_CFLAGS           = -I$(top_srcdir)/mbk/src -I$(top_srcdir)/aut/src -I$(top_srcdir)/vex/src -I$(top_srcdir)/ctl/src
lib_LTLIBRARIES     = libCtp.la
include_HEADERS     = ctp.h
libCtp_la_SOURCES   = ctp_y.y ctp_l.l ctp.h  \
                      ctp_bedef.h  ctp_bspec.c  ctp_byacc.h  ctp_parse.c  ctp_util.h \
                      ctp_blex.h   ctp_bspec.h  ctp_util.c   ctp_utype.h
libCtp_la_LDFLAGS   = -version-info @CTP_DLL_VERSION@ -L../../ctl/src -L../../vex/src -L../../aut/src -L../../mbk/src
libCtp_la_LIBADD    = -lCtl -lVex -lAut -lMut

CLEANFILES          = ctp_y.c ctp_y.h ctp_l.c


ctp_y.c ctp_y.h : $(srcdir)/ctp_y.y
	$(YACC) -d $(YFLAGS) $(srcdir)/ctp_y.y && \
	$(SED) -e "s/yy/ctp_y_/g" -e "s/YY/CTL_Y_/g" y.tab.c > ctp_y.c && \
	$(SED) -e "s/yy/ctp_y_/g" -e "s/YY/CTL_Y_/g" y.tab.h > ctp_y.h
ctp_l.c : $(srcdir)/ctp_l.l ctp_y.h
	$(LEX) -t $(srcdir)/ctp_l.l | $(SED) -e "s/yy/ctp_y_/g" -e "s/YY/CTL_Y_/g" > ctp_l.c

EXTRA_PROGRAMS  = ctptest

ctptest_LDADD   = -L. -lCtp -lCtl -lVex -lAut -lMut

ctptest_SOURCES = main.c
