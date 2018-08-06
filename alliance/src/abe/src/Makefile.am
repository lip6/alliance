AM_CFLAGS           = -I$(top_srcdir)/mbk/src -I$(top_srcdir)/aut/src -I$(top_srcdir)/abl/src
lib_LTLIBRARIES     = libAbe.la
include_HEADERS     = abe.h
libAbe_la_SOURCES   = abe.h	   beh_del.c   beh_error.c  beh_getgenva.c  beh_rmv.c	   beh_view.c \
                      beh_add.c  beh_dict.c  beh_fre.c    beh_message.c   beh_toolbug.c
libAbe_la_LDFLAGS   = -version-info @ABE_DLL_VERSION@ -L../../abl/src -L../../aut/src -L../../mbk/src
libAbe_la_LIBADD    = -lAbl -lAut -lMut
