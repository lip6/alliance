AM_CFLAGS           = @ALLIANCE_CFLAGS@ 
lib_LTLIBRARIES       = libLog.la
include_HEADERS     = log.h
libLog_la_SOURCES    = \
log_bdd0.c  log.h	   log_thashbdd.c  log_thashloc.c \
log_bdd1.c  log_prefbib.c  log_thash.c


LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck


