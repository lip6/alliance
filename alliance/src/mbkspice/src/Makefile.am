AM_CFLAGS = @ALLIANCE_CFLAGS@
lib_LTLIBRARIES       = libMsl.la
include_HEADERS     = msl.h
libMsl_la_SOURCES    = spi_drive.c spi_drive.h spi_global.c spi_global.h \
                      spi_hash.c spi_hash.h spi_int.c spi_int.h spi_msg.c \
                      spi_msg.h spi_parse.c spi_parse.h \
		      msl.h


LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck


