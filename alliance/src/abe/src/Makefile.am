AM_CFLAGS           = @ALLIANCE_CFLAGS@
lib_LTLIBRARIES       = libAbe.la
include_HEADERS     = abe.h
libAbe_la_SOURCES    = \
abe.h	   beh_del.c   beh_error.c  beh_getgenva.c  beh_rmv.c	   beh_view.c \
beh_add.c  beh_dict.c  beh_fre.c    beh_message.c   beh_toolbug.c


LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck


