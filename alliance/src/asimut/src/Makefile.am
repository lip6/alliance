CFLAGS = @ALLIANCE_CFLAGS@

lib_LIBRARIES       = libCst.a libSch.a
include_HEADERS     = cst.h sch.h

libCst_a_SOURCES    =  \
cst_AddSetElt.c    cst_error.c	      cst_globals.c \
cst_CrtCompSet.c   cst_GetFirstElt.c  cst.h \
cst_CrtInterSet.c  cst_GetLastElt.c   cst_IsInSetElt.c \
cst_CrtUnionSet.c  cst_GetNextElt.c   cst_RemSetElt.c \
cst_DisjunctSet.c  cst_GetPrevElt.c   cst_SetCmp.c


libSch_a_SOURCES    =  \
sch_addshent.c	sch_CrtSch.c  sch_GetCTim.c  sch.h	     sch_rmventry.c \
sch_addshtra.c	sch_debug.c   sch_GetCTra.c  sch_hash.c      sch_SetCTim.c \
sch_addshwav.c	sch_debug.h   sch_GetNTim.c  sch_insentry.c \
sch_AddTra.c	sch_error.c   sch_globals.c  sch_inswave.c \
sch_bug.c	sch_Free.c    sch_GoNTim.c   sch_message.c

bin_PROGRAMS = asimut

asimut_LDADD = @ALLIANCE_LIBS@ \
-L.  -lMlu -lMal -lMcl -lMel -lMhl -lMgl -lMsl -lMvl -lMlo -lRcn \
     -lBvl -lBhl -lBeh -lPpt -lPhl -lPat -lSch -lLog -lMut

asimut_SOURCES = \
beh_delay.h	  c_fsyn_sr1k_56.c    vh_debug.c    vh_lspec.c	  vh_util.h \
beh_setdelay.c	  c_fsyn_sr4k_10.c    vh_debug.h    vh_lspec.h	  vh_xcomm.c \
c_fsyn_sr1k_10.c  c_hada_repondeur.c  vh_dtype.h    vh_ltype.h	  vh_xcomm.h \
c_fsyn_sr1k_1.c   c_sr1k_8a.c	      vh_globals.c  vh_simulad.c  vh_xspec.c \
c_fsyn_sr1k_24.c  c_sr1k_8b.c	      vh_globals.h  vh_simulad.h  vh_xspec.h \
c_fsyn_sr1k_4.c   c_sr8k_8a.c	      vh_init.c     vh_util.c


