AM_CFLAGS           = @ALLIANCE_CFLAGS@ 
lib_LTLIBRARIES       = libFtl.la
include_HEADERS     = ftl.h
libFtl_la_SOURCES    = \
ftl.h  ftlacces.c  ftlacces.h  ftlerror.c  ftlerror.h

EXTRA_PROGRAMS = fsmtest


fsmtest_LDADD = @ALLIANCE_LIBS@ -lFtl -lFks -lFvh -lFsm \
	        -lBdd -lAbl -lAut -lMut
fsmtest_SOURCES = main.c


LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck


