lib_LTLIBRARIES       = libVrd.la
include_HEADERS     = vrd.h
libVrd_la_SOURCES    = drucompi_l.l drucompi_y.y \
defdefin.c defdefin.h defexclu.c defexclu.h definclu.c definclu.h definter.c \
definter.h defresiz.c defresiz.h deftools.c deftools.h defunion.c defunion.h \
drucgral.c druchier.c druchier.h drucmin.c drucmin.h drucompi.c \
drucompi.h drucring.c drucring.h drucutil.c drucutil.h vmcaract.c vmcaract.h \
vmcasmld.c vmcasmld.h vmcerror.c vmcerror.h vmcmesur.c vmcmesur.h vmcrelat.c \
vmcrelat.h vmctools.c vmctools.h vmcunify.c vmcunify.h

CFLAGS = $(ALLIANCE_CFLAGS) -g -O0
libVrd_la_LDFLAGS = -version-info @VRD_DLL_VERSION@ -L../../rds/src -L../../mbk/src
libVrd_la_LIBADD  = -lRds -lMut -lMph

CLEANFILES          = drucompi_l.c drucompi_y.h drucompi_y.c

drucompi_y.c drucompi_y.h : $(srcdir)/drucompi_y.y
	$(YACC) -d $(YFLAGS) $(srcdir)/drucompi_y.y && sed -e "s/yy/vrd_y_/g" -e "s/YY/VRD_Y_/g" y.tab.c > drucompi_y.c && sed -e "s/yy/vrd_y_/g" -e "s/YY/VRD_Y_/g" y.tab.h > drucompi_y.h
drucompi_l.c : $(srcdir)/drucompi_l.l drucompi_y.h
	$(LEX) -t $(srcdir)/drucompi_l.l | sed -e "s/yy/vrd_y_/g" -e "s/YY/VRD_Y_/g" > drucompi_l.c


bin_PROGRAMS = druc

AM_CFLAGS = -I$(top_srcdir)/mbk/src -I$(top_srcdir)/rds/src

druc_LDADD = $(ALLIANCE_LIBS) -L. libVrd.la \
-L$(builddir)/../../rds/src -lRds \
-L$(builddir)/../../mbk/src -lMpu -lMut -lMph \
-L$(builddir)/../../genlib/src -lMgn

druc_SOURCES = \
drucbath.c drucbath.h
