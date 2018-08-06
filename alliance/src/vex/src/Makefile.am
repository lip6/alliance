AM_CFLAGS           = -I$(top_srcdir)/mbk/src -I$(top_srcdir)/aut/src
lib_LTLIBRARIES     = libVex.la
include_HEADERS     = vex.h
libVex_la_SOURCES   = vexcreate.h  vexerror.c   vexfree.h   vexshift.c	vexunflat.h \
                      vex.h	     vexdel.c	  vexerror.h   vexget.c    vexshift.h	vexview.c \
                      vexadd.c     vexdel.h	  vexeval.c    vexget.h    vexsimp.c	vexview.h \
                      vexadd.h     vexdup.c	  vexeval.h    vexis.c	   vexsimp.h \
                      vexalloc.c   vexdup.h	  vexextend.c  vexis.h	   vexslice.c \
                      vexalloc.h   vexenv.c	  vexextend.h  vexoptim.c  vexslice.h \
                      vexcreate.c  vexenv.h	  vexfree.c    vexoptim.h  vexunflat.c
libVex_la_LDFLAGS   = -version-info @VEX_DLL_VERSION@ -L../../mbk/src -L../../aut/src
libVex_la_LIBADD    = -lAut -lMut
