AM_CFLAGS           = @ALLIANCE_CFLAGS@ 
AM_YFLAGS           =
AM_LFLAGS           =
lib_LTLIBRARIES       = libMgn.la
include_HEADERS     = mgn.h mbkgen.h genlib.h dgn.h
libMgn_la_SOURCES    = genlib.c \
                      debugon.h \
                      debugoff.h \
                      GNU_Defs.h \
                      util_Defs.h \
                      util_Sys.c \
                      util_Misc.c \
                      util_Genlib.c \
                      util_Const.c \
                      dpgen_Defs.h \
                      dpgen_Regular.c \
                      dpgen_RF1.c \
                      dpgen_RF2.c \
                      dpgen_RAM.c \
                      dpgen_ROM.c \
                      dpgen_ROM_code.y \
                      dpgen_Nul.c \
                      dpgen_Macro.c \
                      dpgen_Adder.c \
                      dpgen_uROM.c \
                      dpgen_Shifter.c

bin_SCRIPTS         = genlib
CLEANFILES          = genlib y.output dpgen_ROM_code.c dpgen_ROM_code.h

genlib : ${srcdir}/genlib.sh
	${SED} 's,__ALLIANCE_INSTALL_DIR__,$(prefix),' $< > $@
	chmod a+x $@

EXTRA_DIST = genlib.sh dpgen_ROM_code.h


LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck


