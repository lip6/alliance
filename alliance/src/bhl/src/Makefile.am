AM_CFLAGS           = -I$(top_srcdir)/mbk/src -I$(top_srcdir)/log/src -I$(top_srcdir)/beh/src
lib_LTLIBRARIES       = libBhl.la
include_HEADERS     = bhl.h
libBhl_la_SOURCES    = \
beh_chkbefig.c	 beh_dly2sta.c	 beh_makbdd.c	 beh_makvarlist.c \
beh_clrdepend.c  beh_freabl.c	 beh_makderiv.c  beh_namelist.c \
beh_debug.c	 beh_gettu.c	 beh_makgex.c	 beh_sta2dly.c \
beh_debug.h	 beh_indexbdd.c  beh_maknode.c	 beh_unamlist.c \
beh_depend.c	 beh_makquad.c	 bhl.h

libBhl_la_LDFLAGS = -version-info @BHL_DLL_VERSION@
