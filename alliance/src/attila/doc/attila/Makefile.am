

pkghtmldir = $(prefix)/doc/html/@PACKAGE@
pkghtml_DATA = \
               ./attila.html \
               ./ref_attila.html \
               ./man_attila.html

EXTRA_DIST = $(pkghtml_DATA)

install-data-hook:
	find $(DESTDIR)$(prefix)/doc/html/@PACKAGE@ -type f | xargs chmod g+w
