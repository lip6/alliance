AM_CFLAGS           = @ALLIANCE_CFLAGS@
lib_LTLIBRARIES       = libRtd.la
include_HEADERS     = rtd.h
libRtd_la_SOURCES    = \
rtd.h     rtd_drive.h  rtd_error.h  rtd_get.h    rtd_parse.h \
rtd_drive.c  rtd_error.c  rtd_get.c    rtd_parse.c


LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck


