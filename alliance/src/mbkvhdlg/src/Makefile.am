YFLAGS           = -d
AM_CFLAGS        = @ALLIANCE_CFLAGS@
lib_LTLIBRARIES    = libMvg.la

libMvg_la_SOURCES = mvl_drive.c mvl_parse.c mvl_util.c mvl_sspec.c \
                   mvl_scomp_y.y mvl_scomp_l.l \
                   mvl_drive.h mvl_parse.h mvl_scomp_y.h mvl_slex.h \
                   mvl_sspec.h mvl_stdef.h mvl_stype.h mvl_syacc.h \
                   mvl_utdef.h mvl_util.h mvl_utype.h \
                   vel_chain.c  vel_sort.c  vel_vect.c vel_drive.c  vel_o.c vel_util.c  vel_velo.h \
                   gen_generic.c  gen_generic.h  gen_print.c

CLEANFILES       = mvl_scomp_y.c mvl_scomp_y.h mvl_scomp_l.c

mvl_scomp_y.c mvl_scomp_y.h : $(srcdir)/mvl_scomp_y.y
	$(YACC) $(YFLAGS) $(srcdir)/mvl_scomp_y.y && sed -e "s/yy/mvl_y_/g" -e "s/YY/MVL_Y_/g" y.tab.c > mvl_scomp_y.c && sed -e "s/yy/mvl_y_/g" -e "s/YY/MVL_Y_/g" y.tab.h > mvl_scomp_y.h
mvl_scomp_l.c : $(srcdir)/mvl_scomp_l.l mvl_scomp_y.h
	$(LEX) -t $(srcdir)/mvl_scomp_l.l | sed -e "s/yy/mvl_y_/g" -e "s/YY/MVL_Y_/g" > mvl_scomp_l.c

bin_PROGRAMS     = x2vy
x2vy_SOURCES     = x2vy_main.c
x2vy_LDADD       = @ALLIANCE_LIBS@ \
                   -lMlu \
                   -lMcl \
                   -lMel \
                   -lMhl \
                   -lMal \
                   -lMsl \
                   -lMgl \
                   $(top_builddir)/src/libMvg.la \
                   -lMlo \
                   -lMut \
                   -lRcn



LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck


