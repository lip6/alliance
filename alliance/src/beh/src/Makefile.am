AM_CFLAGS           = -I$(top_srcdir)/mbk/src -I$(top_srcdir)/log/src
lib_LTLIBRARIES     = libBeh.la
include_HEADERS     = beh.h cst.h
libBeh_la_SOURCES   = beh_addbeaux.c	 beh_delbebus.c    beh_frebeaux.c  beh_message.c \
                      beh_addbebus.c	 beh_delbebux.c    beh_frebebus.c  beh_rmvbeaux.c \
                      beh_addbebux.c	 beh_delbeder.c    beh_frebebux.c  beh_rmvbebus.c \
                      beh_addbeder.c	 beh_delbefig.c    beh_frebeder.c  beh_rmvbebux.c \
                      beh_addbefig.c	 beh_delbegen.c    beh_frebefig.c  beh_rmvbeder.c \
                      beh_addbegen.c	 beh_delbemsg.c    beh_frebegen.c  beh_rmvbefig.c \
                      beh_addbemsg.c	 beh_delbeout.c    beh_frebemsg.c  beh_rmvbegen.c \
                      beh_addbeout.c	 beh_delbepor.c    beh_frebeout.c  beh_rmvbemsg.c \
                      beh_addbepor.c	 beh_delbequad.c   beh_frebepor.c  beh_rmvbeout.c \
                      beh_addbequad.c  beh_delbereg.c    beh_frebereg.c  beh_rmvbepor.c \
                      beh_addbereg.c	 beh_delberin.c    beh_freberin.c  beh_rmvbereg.c \
                      beh_addberin.c	 beh_delbiabl.c    beh_frebiabl.c  beh_rmvberin.c \
                      beh_addbiabl.c	 beh_delbinod.c    beh_frebinod.c  beh_rpldelayed.c \
                      beh_addbinod.c	 beh_dict.c	   beh_getgenva.c  beh_rplstable.c \
                      beh_apdbeaux.c	 beh_error.c	   beh_getvers.c   beh_toolbug.c \
                      beh_apdberin.c	 beh_expdelayed.c  beh_globals.c   beh_vhdlname.c \
                      beh_delbeaux.c	 beh_expstable.c   beh.h	   beh_vhvector.c \
                      cst_AddSetElt.c    cst_error.c       cst_globals.c \
                      cst_CrtCompSet.c   cst_GetFirstElt.c cst.h \
                      cst_CrtInterSet.c  cst_GetLastElt.c  cst_IsInSetElt.c \
                      cst_CrtUnionSet.c  cst_GetNextElt.c  cst_RemSetElt.c \
                      cst_DisjunctSet.c  cst_GetPrevElt.c  cst_SetCmp.c

libBeh_la_LDFLAGS   = -version-info @BEH_DLL_VERSION@ -L../../log/src -L../../mbk/src \
			-L../../abl/src -L../../aut/src -L../../mbk/src
libBeh_la_LIBADD    = -lLog -lAbl -lAut -lMut
