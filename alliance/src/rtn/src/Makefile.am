AM_CFLAGS           = -I$(top_srcdir)/mbk/src -I$(top_srcdir)/aut/src -I$(top_srcdir)/vex/src
lib_LTLIBRARIES     = libRtn.la
include_HEADERS     = rtn.h
libRtn_la_SOURCES   = rtnadd.h    rtndel.c	  rtnerror.h  rtnget.c	   rtnsearch.h \
                      rtn.h	  rtnalloc.c  rtndel.h	  rtnfree.c   rtnget.h	   rtnview.c \
                      rtnadd.c  rtnalloc.h  rtnerror.c  rtnfree.h   rtnsearch.c  rtnview.h
libRtn_la_LDFLAGS   = -version-info @RTN_DLL_VERSION@ -L../../vex/src -L../../aut/src -L../../mbk/src
libRtn_la_LIBADD    = -lVex -lAut -lMut
