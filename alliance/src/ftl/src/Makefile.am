AM_CFLAGS           = @ALLIANCE_CFLAGS@ 
lib_LTLIBRARIES       = libFtl.la
include_HEADERS     = ftl.h
libFtl_la_SOURCES    = \
ftl.h  ftlacces.c  ftlacces.h  ftlerror.c  ftlerror.h

EXTRA_PROGRAMS = fsmtest


fsmtest_LDADD = @ALLIANCE_LIBS@ ${top_builddir}/../ftl/src/libFtl.la ${top_builddir}/../fks/src/libFks.la ${top_builddir}/../fvh/src/libFvh.la ${top_builddir}/../fsm/src/libFsm.la \
	        ${top_builddir}/../bdd/src/libBdd.la ${top_builddir}/../abl/src/libAbl.la ${top_builddir}/../aut/src/libAut.la ${top_builddir}/../mbk/src/libMut.la
fsmtest_SOURCES = main.c


LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck


