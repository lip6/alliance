AM_CFLAGS           = -I$(top_srcdir)/mbk/src -I$(top_srcdir)/aut/src -I$(top_srcdir)/vex/src \
-I$(top_srcdir)/vpn/src
lib_LTLIBRARIES       = libVpd.la
include_HEADERS     = vpd.h
libVpd_la_SOURCES    = \
vpd.h	     vpd_drive.h  vpd_error.h  vpd_parse.h \
vpd_drive.c  vpd_error.c  vpd_parse.c
libVpd_la_LDFLAGS = -version-info @VPD_DLL_VERSION@
