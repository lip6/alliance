AM_CFLAGS           = @ALLIANCE_CFLAGS@
lib_LTLIBRARIES       = libAbt.la
include_HEADERS     = abt.h
libAbt_la_SOURCES    = \
abt.h	      bhl_depend.c  bhl_freabl.c  bhl_makgex.c \
bhl_delaux.c  bhl_error.c   bhl_makbdd.c  bhl_orderbdd.c \
bhl_delaux.h  bhl_error.h   bhl_makbdd.h  bhl_orderbdd.h


LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck


