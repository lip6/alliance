

pkghtmldir = $(prefix)/doc/html/@PACKAGE@
pkghtml_DATA = \
               ./nero.html \
               ./ref_nero.html \
               ./man_nero.html

EXTRA_DIST = $(pkghtml_DATA)

install-data-hook:
	chmod -R g+w $(DESTDIR)$(prefix)/doc/html/@PACKAGE@
