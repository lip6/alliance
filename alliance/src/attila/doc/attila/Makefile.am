

pkghtmldir = $(prefix)/doc/html/@PACKAGE@
pkghtml_DATA = \
               ./attila.html \
               ./ref_attila.html \
               ./man_attila.html

EXTRA_DIST = $(pkghtml_DATA)

install-data-hook:
	find -type f $(DESTDIR)$(prefix)/doc/html/@PACKAGE@ | xargs chmod g+w
