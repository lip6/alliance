AM_CFLAGS           = @ALLIANCE_CFLAGS@

lib_LTLIBRARIES       = libRcf.la
libRcf_la_SOURCES    = cif_drive.c cif_error.c cif_parse.c rcf.h cif.h  cif_drive.h cif_error.h cif_parse.h


LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck


