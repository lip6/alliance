AM_CFLAGS           = -I$(top_srcdir)/mbk/src -I$(top_srcdir)/aut/src -I$(top_srcdir)/vex/src
lib_LTLIBRARIES       = libVpn.la
include_HEADERS     = vpn.h
libVpn_la_SOURCES    = \
vpnalloc.c  vpnenv.h	  vpnget.c     vpnsimp.h \
vpnalloc.h  vpnerror.c  vpnget.h     vpnunion.c \
vpn.h	  vpndel.c    vpnerror.h  vpnsearch.c  vpnunion.h \
vpnadd.c  vpndel.h    vpnfree.c   vpnsearch.h  vpnview.c \
vpnadd.h  vpnenv.c    vpnfree.h   vpnsimp.c    vpnview.h
libVpn_la_LDFLAGS = -version-info @VPN_DLL_VERSION@
