AM_CFLAGS           = -DTECHNOLOGY=\"etc/prol.elp\" 

lib_LTLIBRARIES       = libElp.la
include_HEADERS     = elp.h
libElp_la_SOURCES    = elp_y.y elp_l.l \
elp.c  elperror.c  elp.h

libElp_la_LDFLAGS = -version-info @ELP_DLL_VERSION@

CLEANFILES          = elp_y.c elp_y.h elp_l.c

elp_y.c elp_y.h : $(srcdir)/elp_y.y
	$(YACC) -d $(YFLAGS) $(srcdir)/elp_y.y && sed -e "s/yy/elpyy/g" -e "s/YY/elpYY/g" y.tab.c > elp_y.c && sed -e "s/yy/elpyy/g" -e "s/YY/elpYY/g" y.tab.h > elp_y.h

elp_l.c : $(srcdir)/elp_l.l elp_y.h
	$(LEX) -t $(srcdir)/elp_l.l | sed -e "s/yy/elpyy/g" -e "s/YY/elpYY/g" > elp_l.c
