AM_CFLAGS           = @ALLIANCE_CFLAGS@
lib_LTLIBRARIES       = libVpn.la
include_HEADERS     = vpn.h
libVpn_la_SOURCES    = \
vpnalloc.c  vpnenv.h	  vpnget.c     vpnsimp.h \
vpnalloc.h  vpnerror.c  vpnget.h     vpnunion.c \
vpn.h	  vpndel.c    vpnerror.h  vpnsearch.c  vpnunion.h \
vpnadd.c  vpndel.h    vpnfree.c   vpnsearch.h  vpnview.c \
vpnadd.h  vpnenv.c    vpnfree.h   vpnsimp.c    vpnview.h


LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck


