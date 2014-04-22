AM_CFLAGS           = -I$(top_srcdir)/mbk/src -I$(top_srcdir)/aut/src -I$(top_srcdir)/abl/src -I$(top_srcdir)/bdd/src
lib_LTLIBRARIES     = libBtr.la
include_HEADERS     = btr.h
libBtr_la_SOURCES   = btr.h	  btrenv.c	btrerror.h  btrfunc.c	 btrresize.h  \
                      btralloc.c  btrenv.h	btrfree.c   btrfunc.h	 btrtrans.c   \
                      btralloc.h  btrerror.c	btrfree.h   btrresize.c  btrtrans.h
libBtr_la_LDFLAGS   = -version-info @BTR_DLL_VERSION@
libBtr_la_LIBADD    = -lBdd
