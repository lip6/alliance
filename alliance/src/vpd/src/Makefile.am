AM_CFLAGS           = @ALLIANCE_CFLAGS@
lib_LTLIBRARIES       = libVpd.la
include_HEADERS     = vpd.h
libVpd_la_SOURCES    = \
vpd.h	     vpd_drive.h  vpd_error.h  vpd_parse.h \
vpd_drive.c  vpd_error.c  vpd_parse.c


LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck


