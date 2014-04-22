## Process this file with automake to produce Makefile.in

bin_PROGRAMS = dreal
AM_CFLAGS    = -I$(top_srcdir)/mbk/src \
               -I$(top_srcdir)/rds/src \
               @ALLIANCE_CFLAGS@ @X_CFLAGS@ \
               -DDREAL_DEFAULT_TECHNO_NAME=\"etc/cmos.dreal\" \
               -DGMS_FILE_NAME=\".dreal.cfg\" \
               -DDREAL_WINDOW_DX=\"DREAL_WINDOW_DX\" \
               -DDREAL_WINDOW_DY=\"DREAL_WINDOW_DY\" \
               -DDREAL_WINDOW_XMIN=\"DREAL_WINDOW_XMIN\" \
               -DDREAL_WINDOW_YMIN=\"DREAL_WINDOW_YMIN\" \
               -DDREAL_WINDOW_SIDE=\"DREAL_WINDOW_SIDE\" 


# -----------------------------------------------------------------------------
# X Libraries.
# -----------------------------------------------------------------------------

# Special flags for linking with X.  (-L/usr/X11R5/lib)
X_LDFLAGS = @X_LIBS@

# Motif library.  (-lXm)
LIBXM = @XM_LIBS@

# Use this alternative for building `semistatic' executables
# where Motif libraries are statically linked in.
# LIBXM = -Wl,-Bstatic @XM_LIBS@ -Wl,-Bdynamic

# X extension library; needed for Athena and Motif >= 2.0.  (-lXext)
LIBXEXT = @XEXT_LIBS@

# Xpm library; needed for my tool and sometimes for Motif >= 2.0.  (-lXpm)
LIBXPM = @XPM_LIBS@

# Xp library; sometimes needed for Motif >= 2.1.  (-lXp)
LIBXP = @XP_LIBS@

# X toolkit library.  (-lXt)
LIBXT = @XT_LIBS@

# X library.  (-lX11 -lSM -lICE -lnsl -lsocket)
LIBX11 = @X_PRE_LIBS@ @X11_LIBS@ @X_EXTRA_LIBS@

# All libraries shown above
ALL_X_LIBS = $(X_LDFLAGS) $(LIBXM) $(LIBXT) $(LIBXPM) \
             $(LIBXP) $(LIBXEXT) $(LIBX11)


dreal_LDADD = @ALLIANCE_LIBS@ $(ALL_X_LIBS) \
              -L$(top_builddir)/mbk/src \
              -L$(top_builddir)/rds/src \
              -lRds -lMpu -lMph -lMut 


dreal_SOURCES = \
dreal.c dreal.h GMC_create.c GMC_create.h GMC_dialog.c GMC_dialog.h GMC.h \
GMC_menu.c GMC_menu.h GMC_message.c GMC_message.h GMC_panel.c GMC_panel.h \
GME_dialog.c GME_dialog.h GME_edit.c GME_edit.h GME.h GME_menu.c GME_menu.h \
GME_message.c GME_message.h GME_modify.c GME_modify.h GME_panel.c GME_panel.h \
GME_search.c GME_search.h GME_select.c GME_select.h GMF_dialog.c GMF_dialog.h \
GMF_file.c GMF_file.h GMF.h GMF_menu.c GMF_menu.h GMF_panel.c GMF_panel.h GMH.h \
GMH_help.c GMH_help.h GMH_menu.c GMH_menu.h GMH_panel.c GMH_panel.h \
GMS_dialog.c GMS_dialog.h GMS.h GMS_menu.c GMS_menu.h GMS_panel.c GMS_panel.h \
GMS_setup.c GMS_setup.h GMT_dialog.c GMT_dialog.h GMT.h GMT_menu.c GMT_menu.h \
GMT_message.c GMT_message.h GMT_panel.c GMT_panel.h GMT_tools.c GMT_tools.h \
GMV_dialog.c GMV_dialog.h GMV.h GMV_map.c GMV_map.h GMV_menu.c GMV_menu.h \
GMV_message.c GMV_message.h GMV_panel.c GMV_panel.h GMV_view.c GMV_view.h \
GMX_color.c GMX_color.h GMX_cursor.c GMX_cursor.h GMX_dialog.c GMX_dialog.h \
GMX_event.c GMX_event.h GMX_graphic.c GMX_graphic.h GMX_grid.c GMX_grid.h GMX.h \
GMX_icon.c GMX_icon.h GMX_menu.c GMX_menu.h GMX_message.c GMX_message.h \
GMX_motif.c GMX_motif.h GMX_panel.c GMX_panel.h GMX_view.c GMX_view.h \
GRD_error.c GRD_error.h GRD.h GRD_rds.c GRD_rds.h GRD_search.c GRD_search.h \
GRD_select.c GRD_select.h GRD_undo.c GRD_undo.h GRD_window.c GRD_window.h \
GSB_error.c GSB_error.h GSB.h GSB_parse.c GSB_parse.h GSB_share.c GSB_share.h \
GTB_cursor.c GTB_cursor.h GTB_dialog.c GTB_dialog.h GTB.h GTB_icon.c GTB_icon.h \
GTB_menu.c GTB_menu.h GTB_panel.c GTB_panel.h GTB_pixmap.c GTB_pixmap.h \
LIP6bw.h
