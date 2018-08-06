AM_CFLAGS           = -I$(top_srcdir)/mbk/src
lib_LTLIBRARIES     = libLog.la
include_HEADERS     = log.h
libLog_la_SOURCES   = log_bdd0.c  log.h	log_thashbdd.c  log_thashloc.c \
                      log_bdd1.c  log_prefbib.c  log_thash.c
libLog_la_LDFLAGS   = -version-info @LOG_DLL_VERSION@ -L../../mbk/src
libLog_la_LIBADD    = -lMut
