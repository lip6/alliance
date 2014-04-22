AM_CFLAGS           = -I$(top_srcdir)/mbk/src
lib_LTLIBRARIES     = libAut.la
include_HEADERS     = aut.h
libAut_la_SOURCES   =  autenv.h autfree.h     authash2.h    autsort.h \
autalloc.c    auterror.c    autgraph.c    autname.c     auttest.c \
autalloc.h    auterror.h    autgraph.h    autname.h     auttest.h \
autdebug.c    autexit.c     authash.c     autresize.c   \
autdebug.h    autexit.h     authash.h     autresize.h   \
autenv.c      autfree.c     authash2.c    autsort.c

libAut_la_LDFLAGS = -version-info @AUT_DLL_VERSION@
libAut_la_LIBADD  = -lMut
