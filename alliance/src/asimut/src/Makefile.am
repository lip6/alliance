AM_CFLAGS = -I$(top_srcdir)/mbk/src \
            -I$(top_srcdir)/log/src \
            -I$(top_srcdir)/beh/src \
            -I$(top_srcdir)/pat/src \
            -I$(top_srcdir)/bhl/src \
            -I$(top_srcdir)/bvl/src \
            -I$(top_srcdir)/ppt/src \
            -I$(top_srcdir)/mlu/src

lib_LTLIBRARIES   = libSch.la
include_HEADERS   = sch.h

libSch_la_SOURCES = sch_addshent.c	sch_CrtSch.c  sch_GetCTim.c  sch.h	     sch_rmventry.c \
                    sch_addshtra.c	sch_debug.c   sch_GetCTra.c  sch_hash.c      sch_SetCTim.c \
                    sch_addshwav.c	sch_debug.h   sch_GetNTim.c  sch_insentry.c \
                    sch_AddTra.c	sch_error.c   sch_globals.c  sch_inswave.c \
                    sch_bug.c	sch_Free.c    sch_GoNTim.c   sch_message.c

libSch_la_LDFLAGS = -version-info @SCH_DLL_VERSION@ -L../../mbk/src
libSch_la_LIBADD  = -lMut

bin_PROGRAMS      = asimut

asimut_LDADD      = -L. libSch.la -lBvl -lBhl \
                    -L$(top_builddir)/mbk/src \
                    -L$(top_builddir)/bvl/src \
                    -L$(top_builddir)/bhl/src \
                    -L$(top_builddir)/beh/src \
                    -L$(top_builddir)/pat/src \
                    -L$(top_builddir)/log/src \
                    -L$(top_builddir)/abl/src \
                    -L$(top_builddir)/aut/src \
                    -lAut -lAbl -lMlu -lRcn -lBvl -lBhl -lBeh -lPat -lLog \
                    -lMut -lMlo


asimut_SOURCES    = beh_delay.h	  c_fsyn_sr1k_56.c    vh_debug.c    vh_lspec.c	  vh_util.h  vh_init.h \
                    beh_setdelay.c	  c_fsyn_sr4k_10.c    vh_debug.h    vh_lspec.h	  vh_xcomm.c \
                    c_fsyn_sr1k_10.c  c_hada_repondeur.c  vh_dtype.h    vh_ltype.h	  vh_xcomm.h \
                    c_fsyn_sr1k_1.c   c_sr1k_8a.c	      vh_globals.c  vh_simulad.c  vh_xspec.c \
                    c_fsyn_sr1k_24.c  c_sr1k_8b.c	      vh_globals.h  vh_simulad.h  vh_xspec.h \
                    c_fsyn_sr1k_4.c   c_sr8k_8a.c	      vh_init.c     vh_util.c
