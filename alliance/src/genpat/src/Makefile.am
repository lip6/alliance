AM_CFLAGS           = @ALLIANCE_CFLAGS@ 
lib_LIBRARIES       = libPgn.a
include_HEADERS     = genpat.h 
libPgn_a_SOURCES    = AFFECT.c ARRAY.c CONV.c DECLAR.c DEF_GEN.c GETCPAT.c \
		INIT.c LABEL.c  SAVE.c SAV_GEN.c SETTUNIT.c \
		genpat.1 libpat_l.c libpat_l.h

bin_SCRIPTS         = genpat
CLEANFILES          = genpat

genpat : ${srcdir}/genpat.sh
	${SED} 's,__ALLIANCE_INSTALL_DIR__,$(ALLIANCE_INSTALL_DIR),' $< > $@
	chmod a+x $@

EXTRA_DIST = genpat.sh
