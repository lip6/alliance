AM_CFLAGS           = @ALLIANCE_CFLAGS@ 
lib_LTLIBRARIES       = libAut.la
include_HEADERS     = aut.h
libAut_la_SOURCES    = autenv.h autfree.h     authash2.h    autsort.h \
autalloc.c    auterror.c    autgraph.c    autname.c     auttest.c \
autalloc.h    auterror.h    autgraph.h    autname.h     auttest.h \
autdebug.c    autexit.c     authash.c     autresize.c   \
autdebug.h    autexit.h     authash.h     autresize.h   \
autenv.c      autfree.c     authash2.c    autsort.c


LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck


