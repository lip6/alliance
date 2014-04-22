AM_CFLAGS           = -I$(top_srcdir)/mbk/src \
                      -I$(top_srcdir)/aut/src \
                      -I$(top_srcdir)/vex/src \
                      -I$(top_srcdir)/rtn/src
lib_LTLIBRARIES     = libRtd.la
include_HEADERS     = rtd.h
libRtd_la_SOURCES   = rtd.h     rtd_drive.h  rtd_error.h  rtd_get.h    rtd_parse.h \
                      rtd_drive.c  rtd_error.c  rtd_get.c    rtd_parse.c
libRtd_la_LDFLAGS   = -version-info @RTD_DLL_VERSION@
libRtd_la_LIBADD    = -lRtn -lVex -lMut
