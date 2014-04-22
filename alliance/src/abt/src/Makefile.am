AM_CFLAGS           = -I$(top_srcdir)/mbk/src \
                      -I$(top_srcdir)/aut/src \
                      -I$(top_srcdir)/abl/src \
                      -I$(top_srcdir)/bdd/src \
                      -I$(top_srcdir)/abe/src
lib_LTLIBRARIES     = libAbt.la
include_HEADERS     = abt.h
libAbt_la_SOURCES   = abt.h	      bhl_depend.c  bhl_freabl.c  bhl_makgex.c \
                      bhl_delaux.c  bhl_error.c   bhl_makbdd.c  bhl_orderbdd.c \
                      bhl_delaux.h  bhl_error.h   bhl_makbdd.h  bhl_orderbdd.h
libAbt_la_LDFLAGS   = -version-info @ABT_DLL_VERSION@
libAbt_la_LIBADD    = -lBdd -lAbe -lAbl -lAut -lMut
