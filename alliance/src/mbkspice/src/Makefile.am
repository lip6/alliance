AM_CFLAGS = -I$(top_srcdir)/mbk/src
lib_LTLIBRARIES       = libMsl.la
include_HEADERS     = msl.h
libMsl_la_SOURCES    = spi_drive.c spi_drive.h spi_global.c spi_global.h \
                      spi_hash.c spi_hash.h spi_int.c spi_int.h spi_msg.c \
                      spi_msg.h spi_parse.c spi_parse.h \
		      msl.h
libMsl_la_LDFLAGS = -version-info @MSL_DLL_VERSION@
