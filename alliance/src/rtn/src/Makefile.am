AM_CFLAGS           = @ALLIANCE_CFLAGS@
lib_LTLIBRARIES       = libRtn.la
include_HEADERS     = rtn.h
libRtn_la_SOURCES    = \
rtnadd.h    rtndel.c	  rtnerror.h  rtnget.c	   rtnsearch.h \
rtn.h	  rtnalloc.c  rtndel.h	  rtnfree.c   rtnget.h	   rtnview.c \
rtnadd.c  rtnalloc.h  rtnerror.c  rtnfree.h   rtnsearch.c  rtnview.h


LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck


