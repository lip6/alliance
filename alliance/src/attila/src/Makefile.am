AM_CFLAGS           = @ALLIANCE_CFLAGS@ 

bin_SCRIPTS         = attila
CLEANFILES          = attila

attila : ${srcdir}/attila.sh
	${SED} 's,="__ALLIANCE_INSTALL_DIR__"$$,="$(prefix)",' $< > $@

EXTRA_DIST = attila.sh

install-exec-hook:
	chmod g+w $(DESTDIR)$(bindir)/attila
