AM_CFLAGS           = @ALLIANCE_CFLAGS@ 
lib_LTLIBRARIES       = libPgn.la
include_HEADERS     = genpat.h 
libPgn_la_SOURCES    = AFFECT.c ARRAY.c CONV.c DECLAR.c DEF_GEN.c GETCPAT.c \
		INIT.c LABEL.c  SAVE.c SAV_GEN.c SETTUNIT.c \
		libpat_l.c libpat_l.h

bin_SCRIPTS         = genpat
CLEANFILES          = genpat

genpat : ${srcdir}/genpat.sh
	${SED} 's,__ALLIANCE_INSTALL_DIR__,$(ALLIANCE_INSTALL_DIR),' $< > $@
	chmod a+x $@

EXTRA_DIST = genpat.sh


LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck


