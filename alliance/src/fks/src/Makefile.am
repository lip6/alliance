AM_CFLAGS           = @ALLIANCE_CFLAGS@ 
lib_LTLIBRARIES       = libFks.la
include_HEADERS     = fks.h
libFks_la_SOURCES    = \
fks.h  fksdrive.c  fksdrive.h  fkserror.c  fkserror.h  fksparse.c  fksparse.h


LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck


