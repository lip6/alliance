AM_CFLAGS           = @ALLIANCE_CFLAGS@ 
lib_LIBRARIES       = libFtl.a
include_HEADERS     = ftl.h
libFtl_a_SOURCES    = \
ftl.h  ftlacces.c  ftlacces.h  ftlerror.c  ftlerror.h

EXTRA_PROGRAMS = fsmtest


fsmtest_LDADD = @ALLIANCE_LIBS@ -lFtl -lFks -lFvh -lFsm \
	        -lBdd -lAbl -lAut -lMut
fsmtest_SOURCES = main.c
