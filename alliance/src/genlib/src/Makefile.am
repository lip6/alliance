lib_LIBRARIES       = libMgn.a
include_HEADERS     = mgn.h mbkgen.h genlib.h
libMgn_a_SOURCES    = genlib.c
bin_SCRIPTS         = genlib
CLEANFILES          = genlib

genlib : ${srcdir}/genlib.sh
	${SED} 's,__ALLIANCE_INSTALL_DIR__,$(ALLIANCE_INSTALL_DIR),' $< > $@
	chmod a+x $@
