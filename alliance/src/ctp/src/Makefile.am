AM_CFLAGS           = @ALLIANCE_CFLAGS@
lib_LTLIBRARIES       = libCtp.la
include_HEADERS     = ctp.h
libCtp_la_SOURCES    = ctp_y.y ctp_l.l ctp.h  \
ctp_bedef.h  ctp_bspec.c  ctp_byacc.h  ctp_parse.c  ctp_util.h \
ctp_blex.h   ctp_bspec.h  ctp_util.c   ctp_utype.h

CLEANFILES          = ctp_y.c ctp_y.h ctp_l.c


ctp_y.c ctp_y.h : $(srcdir)/ctp_y.y
	$(YACC) -d $(YFLAGS) $(srcdir)/ctp_y.y && sed -e "s/yy/ctp_y_/g" -e "s/YY/CTL_Y_/g" y.tab.c > ctp_y.c && sed -e "s/yy/ctp_y_/g" -e "s/YY/CTL_Y_/g" y.tab.h > ctp_y.h
ctp_l.c : $(srcdir)/ctp_l.l ctp_y.h
	$(LEX) -t $(srcdir)/ctp_l.l | sed -e "s/yy/ctp_y_/g" -e "s/YY/CTL_Y_/g" > ctp_l.c

EXTRA_PROGRAMS = ctptest

ctptest_LDADD = @ALLIANCE_LIBS@ \
                $(top_builddir)/src/libCtp.la \
                -lCtl -lVex -lAut -lMut

ctptest_SOURCES = main.c


LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck


