AM_CFLAGS           = @ALLIANCE_CFLAGS@ -I$(top_srcdir)/mbk/src -I$(top_srcdir)/aut/src -I$(top_srcdir)/vex/src
lib_LTLIBRARIES     = libCtl.la
include_HEADERS     = ctl.h
libCtl_la_SOURCES   = ctladd.c    ctlalloc.h	ctlenv.c    ctlerror.h	ctl.h	     ctlview.c \
                      ctladd.h    ctldel.c	ctlenv.h    ctlfree.c	ctlsearch.c  ctlview.h \
                      ctlalloc.c  ctldel.h	ctlerror.c  ctlfree.h	ctlsearch.h
libCtl_la_LDFLAGS   = -version-info @CTL_DLL_VERSION@ -L../../mbk/src -L../../aut/src -L../../vex/src
libCtl_la_LIBADD    = -lVex -lAut -lMut
