lib_LTLIBRARIES       = libVrd.la
include_HEADERS     = vrd.h
libVrd_la_SOURCES    = drucompi_l.l drucompi_y.y \
defdefin.c defdefin.h defexclu.c defexclu.h definclu.c definclu.h definter.c \
definter.h defresiz.c defresiz.h deftools.c deftools.h defunion.c defunion.h \
drucgral.c druchier.c druchier.h drucmin.c drucmin.h drucompi.c \
drucompi.h drucring.c drucring.h drucutil.c drucutil.h vmcaract.c vmcaract.h \
vmcasmld.c vmcasmld.h vmcerror.c vmcerror.h vmcmesur.c vmcmesur.h vmcrelat.c \
vmcrelat.h vmctools.c vmctools.h vmcunify.c vmcunify.h

CLEANFILES          = drucompi_l.c drucompi_y.h drucompi_y.c

drucompi_y.c drucompi_y.h : $(srcdir)/drucompi_y.y
	$(YACC) -d $(YFLAGS) $(srcdir)/drucompi_y.y && sed -e "s/yy/vrd_y_/g" -e "s/YY/VRD_Y_/g" y.tab.c > drucompi_y.c && sed -e "s/yy/vrd_y_/g" -e "s/YY/VRD_Y_/g" y.tab.h > drucompi_y.h
drucompi_l.c : $(srcdir)/drucompi_l.l drucompi_y.h
	$(LEX) -t $(srcdir)/drucompi_l.l | sed -e "s/yy/vrd_y_/g" -e "s/YY/VRD_Y_/g" > drucompi_l.c


bin_PROGRAMS = druc

AM_CFLAGS = @ALLIANCE_CFLAGS@

druc_LDADD = @ALLIANCE_LIBS@ $(top_builddir)/src/libVrd.la \
${top_builddir}/../rds/src/libRtl.la ${top_builddir}/../rdsgds/src/libRgs.la ${top_builddir}/../rdscif/src/libRcf.la ${top_builddir}/../rds/src/libRfm.la ${top_builddir}/../rds/src/libRpr.la ${top_builddir}/../rds/src/libRwi.la ${top_builddir}/../rds/src/libRut.la ${top_builddir}/../rds/src/libRds.la ${top_builddir}/../mbk/src/libMpu.la ${top_builddir}/../mbkvti/src/libMcp.la ${top_builddir}/../mbkap/src/libMap.la ${top_builddir}/../mbkmg/src/libMmg.la ${top_builddir}/../mbk/src/libMph.la \
${top_builddir}/../mbk/src/libMut.la

druc_SOURCES = \
drucbath.c drucbath.h


LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck


