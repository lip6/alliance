AM_CFLAGS = @ALLIANCE_CFLAGS@
lib_LTLIBRARIES       = libMph.la libMpu.la libMlo.la libMlu.la libMut.la libRcn.la
include_HEADERS     = mlu.h mlo.h mpu.h mph.h mut.h rcn.h
libMph_la_SOURCES    = mbk_ph.c mbk_ph.h
libMpu_la_SOURCES    = mbk_ph_util.c mbk_ph_util.h
libMlo_la_SOURCES    = mbk_lo.c mbk_lo.h
libMlu_la_SOURCES    = mbk_lo_util.c mbk_lo_util.h
libMut_la_SOURCES    = mbk_sys.c mbk_sys.h mbk_util.c mbk_util.h
libRcn_la_SOURCES    = rcn_lo.c  rcn_lo.h  rcn_lo_util.c


LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck


