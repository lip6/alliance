AM_CFLAGS         = @ALLIANCE_CFLAGS@ \
                    -I$(top_srcdir)/mbk/src \
                    -DMAXLINE=500 -DMAXCOL=1000 -O4

lib_LTLIBRARIES   = libApr.la
include_HEADERS   = apr.h

libApr_la_SOURCES = gscr_greedy.h  gscr_StaticUtil.h  scp_inits.h  scp_modif.h  scp_time.h  \
                    gscr_AllowFree.c  gscr_GreedyRout.c  gscr_SymbChanRout.c  scp_main.c  \
                    scp_placer.c   scp_types.h  gscr_AllowFree.h  gscr_GreedyRout.h  \
                    gscr_SymbChanRout.h  scp_main.h  scp_placer.h  sx2sc.c  gscr_DataBase.h  \
                    scp_channel.c  scp_mbk2scp.c  scp_scp2mbk.c  gscr_DynamicUtil.c  \
                    gscr_main.h  scp_channel.h  scp_mbk2scp.h  scp_scp2mbk.h  gscr_DynamicUtil.h  \
                    gscr_StaticUtil.c  scp_inits.c  scp_modif.c  scp_time.c

libApr_la_LDFLAGS = -version-info @APR_DLL_VERSION@ -L../../mbk/src

libApr_la_LIBADD  = -lMut -lMph -lMlo

