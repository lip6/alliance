AM_CFLAGS            = -I$(top_srcdir)/mbk/src -I$(top_srcdir)/aut/src
lib_LTLIBRARIES      = libAbl.la
include_HEADERS      = abl.h
libAbl_la_SOURCES    = \
abldel.h       ablflat.h      abloptim.h     ablunflat.h \
abladd.c       abldev.c       ablfree.c      ablpolar.c     ablvhdl.c \
abladd.h       abldev.h       ablfree.h      ablpolar.h     ablvhdl.h \
ablcreate.c    abldup.c       ablget.c       ablsimp.c      ablview.c \
ablcreate.h    abldup.h       ablget.h       ablsimp.h      ablview.h \
ablctlnorm.c   ablenv.c       ablis.c        ablstr.c \
ablctlnorm.h   ablenv.h       ablis.h        ablstr.h \
ablctlsimp.c   ablerror.c     ablmap.c       ablsubst.c \
ablctlsimp.h   ablerror.h     ablmap.h       ablsubst.h \
abldel.c       ablflat.c      abloptim.c     ablunflat.c

libAbl_la_LDFLAGS = -version-info @ABL_DLL_VERSION@
