AM_CFLAGS           = -I$(top_srcdir)/mbk/src -I$(top_srcdir)/aut/src -I$(top_srcdir)/vex/src \
-I$(top_srcdir)/vpn/src -I$(top_srcdir)/vbh/src -I$(top_srcdir)/vbl/src
lib_LTLIBRARIES       = libVvh.la
include_HEADERS     = vvh.h
libVvh_la_SOURCES    = \
vvh.h	     vvh_drive.h  vvh_error.h  vvh_parse.h    vvh_vbh2vpn.h \
vvh_drive.c  vvh_error.c  vvh_parse.c  vvh_vbh2vpn.c
libVvh_la_LDFLAGS = -version-info @VVH_DLL_VERSION@
