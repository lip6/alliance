AM_CFLAGS           = @ALLIANCE_CFLAGS@ 
lib_LTLIBRARIES       = libBtr.la
include_HEADERS     = btr.h
libBtr_la_SOURCES    = \
btr.h	    btrenv.c	btrerror.h  btrfunc.c	 btrresize.h  \
btralloc.c  btrenv.h	btrfree.c   btrfunc.h	 btrtrans.c \
btralloc.h  btrerror.c	btrfree.h   btrresize.c  btrtrans.h


LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck


