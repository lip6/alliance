AM_CFLAGS           = -I$(top_srcdir)/mbk/src -I$(top_srcdir)/aut/src \
                      -I$(top_srcdir)/abl/src -I$(top_srcdir)/bdd/src \
                      -I$(top_srcdir)/abe/src
lib_LTLIBRARIES     = libScl.la
include_HEADERS     = scl.h
libScl_la_SOURCES   = scgmain.h	schenv.c    schget.h   scpadd.h    scpfree.c \
                      scg.h	    schadd.c	schenv.h    schview.c  scpalloc.c  scpfree.h \
                      scgbefig.c  schadd.h	scherror.c  schview.h  scpalloc.h  scpswap.h \
                      scgbefig.h  schalloc.c	scherror.h  scpbuild.c \
                      scglofig.c  schalloc.h	schfree.c   scl.h   scpbuild.h \
                      scglofig.h  schdel.c	schfree.h   scp.h      scpdel.c \
                      scgmain.c   schdel.h	schget.c    scpadd.c   scpdel.h
libScl_la_LDFLAGS   = -version-info @SCL_DLL_VERSION@ -L../../abl/src -L../../aut/src -L../../mbk/src
libScl_la_LIBADD    = -lAbl -lAut -lMut
