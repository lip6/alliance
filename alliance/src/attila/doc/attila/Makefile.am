

pkghtmldir = $(DESTDIR)$(prefix)/doc/html/@PACKAGE@
pkghtml_DATA = \
               ./attila.html \
               ./ref_attila.html \
               ./man_attila.html

EXTRA_DIST = $(pkghtml_DATA)

install-data-hook:
	chmod -R g+w $(DESTDIR)$(prefix)/doc/html/@PACKAGE@
