AM_CFLAGS           = -I$(top_srcdir)/mbk/src -I$(top_srcdir)/aut/src -I$(top_srcdir)/abl/src -I$(top_srcdir)/bdd/src \
-I$(top_srcdir)/fsm/src
lib_LTLIBRARIES       = libFks.la
include_HEADERS     = fks.h
libFks_la_SOURCES    = \
fks.h  fksdrive.c  fksdrive.h  fkserror.c  fkserror.h  fksparse.c  fksparse.h

libFks_la_LDFLAGS = -version-info @FKS_DLL_VERSION@
