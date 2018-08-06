AM_CFLAGS           = -I$(top_srcdir)/mbk/src \
                      -I$(top_srcdir)/aut/src \
                      -I$(top_srcdir)/abl/src \
                      -I$(top_srcdir)/bdd/src \
                      -I$(top_srcdir)/fsm/src
lib_LTLIBRARIES     = libFtl.la
include_HEADERS     = ftl.h
libFtl_la_SOURCES   = ftl.h  ftlacces.c  ftlacces.h  ftlerror.c  ftlerror.h
libFtl_la_LDFLAGS   = -version-info @FTL_DLL_VERSION@ -L../../fks/src -L../../fvh/src \
					  -L../../fsm/src -L../../bdd/src -L../../abl/src -L../../aut/src \
					  -L../../mbk/src
libFtl_la_LIBADD    = -lFks -lFvh -lFsm -lBdd -lAbl -lAut -lMut 

EXTRA_PROGRAMS = fsmtest


fsmtest_LDADD       = -L. -lFtl -lFks -lFvh -lFsm -lBdd -lAbl -lAut -lMut
fsmtest_SOURCES     = main.c
