YFLAGS           = -d
AM_CFLAGS        = -I$(top_srcdir)/mbk/src
lib_LTLIBRARIES    = libMvl.la

libMvl_la_SOURCES = mvl_drive.c  mvl_parse.h    mvl_sspec.c  mvl_syacc.h  mvl_utype.h \
mvl_drive.h  mvl_scomp_l.l  mvl_sspec.h  mvl_utdef.h \
mvl.h	     mvl_scomp_y.y  mvl_stdef.h  mvl_util.c \
mvl_parse.c  mvl_slex.h     mvl_stype.h  mvl_util.h

libMvl_la_LDFLAGS = -version-info @MVL_DLL_VERSION@

CLEANFILES       = mvl_scomp_y.c mvl_scomp_y.h mvl_scomp_l.c

mvl_scomp_y.c mvl_scomp_y.h : $(srcdir)/mvl_scomp_y.y
	$(YACC) $(YFLAGS) $(srcdir)/mvl_scomp_y.y && sed -e "s/yy/mvl_y_/g" -e "s/YY/MVL_Y_/g" y.tab.c > mvl_scomp_y.c && sed -e "s/yy/mvl_y_/g" -e "s/YY/MVL_Y_/g" y.tab.h > mvl_scomp_y.h
mvl_scomp_l.c : $(srcdir)/mvl_scomp_l.l mvl_scomp_y.h
	$(LEX) -t $(srcdir)/mvl_scomp_l.l | sed -e "s/yy/mvl_y_/g" -e "s/YY/MVL_Y_/g" > mvl_scomp_l.c
