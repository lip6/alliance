AM_CFLAGS           = @ALLIANCE_CFLAGS@ 
lib_LIBRARIES       = libMgn.a
include_HEADERS     = mgn.h mbkgen.h genlib.h dgn.h
libMgn_a_SOURCES    = genlib.c \
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
                      dpgen_Nul.c \
                      dpgen_Macro.c \
                      dpgen_Adder.c \
                      dpgen_uROM.c \
                      dpgen_Shifter.c

bin_SCRIPTS         = genlib
CLEANFILES          = genlib

genlib : ${srcdir}/genlib.sh
	${SED} 's,__ALLIANCE_INSTALL_DIR__,$(ALLIANCE_INSTALL_DIR),' $< > $@
	chmod a+x $@

EXTRA_DIST = genlib.sh
