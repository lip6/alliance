AM_CFLAGS           = @ALLIANCE_CFLAGS@ 
lib_LTLIBRARIES       = libPat.la
include_HEADERS     = pat.h
libPat_la_SOURCES    = \
pat_addpacom.c pat_addpaevt.c pat_addpagrp.c pat_addpaini.c \
pat_addpains.c pat_addpaiol.c pat_addpapat.c pat_addpaseq.c \
pat_crtpaiol.c pat_error.c pat_frepacom.c pat_frepaevt.c \
pat_frepaini.c pat_frepaiol.c pat_frepapat.c pat_globals.c \
pat.h pat_message.c pat_warning.c


LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck


