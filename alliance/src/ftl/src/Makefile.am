AM_CFLAGS           =
lib_LTLIBRARIES       = libFtl.la
include_HEADERS     = ftl.h
libFtl_la_SOURCES    = \
ftl.h  ftlacces.c  ftlacces.h  ftlerror.c  ftlerror.h

libFtl_la_LDFLAGS = -version-info @FTL_DLL_VERSION@

EXTRA_PROGRAMS = fsmtest


fsmtest_LDADD = -L${top_buildir}/src -lFtl -lFks -lFvh -lFsm \
	        -lBdd -lAbl -lAut -lMut
fsmtest_SOURCES = main.c
