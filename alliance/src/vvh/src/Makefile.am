AM_CFLAGS           = @ALLIANCE_CFLAGS@
lib_LTLIBRARIES       = libVvh.la
include_HEADERS     = vvh.h
libVvh_la_SOURCES    = \
vvh.h	     vvh_drive.h  vvh_error.h  vvh_parse.h    vvh_vbh2vpn.h \
vvh_drive.c  vvh_error.c  vvh_parse.c  vvh_vbh2vpn.c


LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck


