AM_CFLAGS           =
lib_LTLIBRARIES       = libFsm.la
include_HEADERS     = fsm.h
libFsm_la_SOURCES    = \
fsm.h	    fsmalloc.h	fsmdel.h    fsmfree.h	 fsmsearch.h  fsmview.h \
fsmadd.c    fsmbdd.c	fsmerror.c  fsmorder.c	 fsmsimp.c \
fsmadd.h    fsmbdd.h	fsmerror.h  fsmorder.h	 fsmsimp.h \
fsmalloc.c  fsmdel.c	fsmfree.c   fsmsearch.c  fsmview.c

libFsm_la_LDFLAGS = -version-info @FSM_DLL_VERSION@
