AM_CFLAGS = @ALLIANCE_CFLAGS@
lib_LTLIBRARIES       = libMmg.la
libMmg_la_SOURCES    = mbk2mg.c mg2mbk_y.y mg2mbk_l.l mmg.h
CLEANFILES          = mg2mbk_y.c mg2mbk_y.h mg2mbk_l.c

mg2mbk_y.c mg2mbk_y.h : $(srcdir)/mg2mbk_y.y
	$(YACC) -d $(YFLAGS) $(srcdir)/mg2mbk_y.y && sed -e "s/yy/mgn/g" -e "s/YY/MGN/g" y.tab.c > mg2mbk_y.c && sed -e "s/yy/mgn/g" -e "s/YY/MGN/g" y.tab.h > mg2mbk_y.h
mg2mbk_l.c : $(srcdir)/mg2mbk_l.l mg2mbk_y.h
	$(LEX) -t $(srcdir)/mg2mbk_l.l | sed -e "s/yy/mgn/g" -e "s/YY/MGN/g" > mg2mbk_l.c


LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck


