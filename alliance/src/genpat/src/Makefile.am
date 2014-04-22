AM_CFLAGS         = -I$(top_srcdir)/mbk/src \
                    -I$(top_srcdir)/pat/src 

lib_LTLIBRARIES   = libPgn.la
include_HEADERS   = genpat.h 
libPgn_la_SOURCES = AFFECT.c ARRAY.c CONV.c DECLAR.c DEF_GEN.c GETCPAT.c \
                    INIT.c LABEL.c  SAVE.c SAV_GEN.c SETTUNIT.c \
                    libpat_l.c libpat_l.h

libPgn_la_LDFLAGS = -version-info @PGN_DLL_VERSION@
libPgn_la_LIBADD  = -lPat -lMut

bin_SCRIPTS       = genpat
CLEANFILES        = genpat

genpat : ${srcdir}/genpat.sh
	${SED} 's,__ALLIANCE_INSTALL_DIR__,$(ALLIANCE_INSTALL_DIR),' $< > $@
	chmod a+x $@

EXTRA_DIST = genpat.sh
