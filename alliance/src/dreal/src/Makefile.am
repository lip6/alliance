## Process this file with automake to produce Makefile.in

bin_PROGRAMS = dreal
CFLAGS = @CFLAGS@ @X_CFLAGS@ -DDREAL_VERSION=\"@DREAL_VERSION@\" \
         -DDREAL_TOOL_NAME=\"dreal\" \
         -DALLIANCE_TOP=\"${ALLIANCE_TOP}\" \
         -DDREAL_DEFAULT_TECHNO_NAME=\"etc/cmos_7.dreal\" \
         -DGMS_FILE_NAME=\".dreal.cfg\" \
         -DDREAL_WINDOW_DX=\"DREAL_WINDOW_DX\" \
         -DDREAL_WINDOW_DY=\"DREAL_WINDOW_DY\" \
         -DDREAL_WINDOW_XMIN=\"DREAL_WINDOW_XMIN\" \
         -DDREAL_WINDOW_YMIN=\"DREAL_WINDOW_YMIN\" \
         -DDREAL_WINDOW_SIDE=\"DREAL_WINDOW_SIDE\" 

dreal_LDADD = @LIBS@ @X_LIBS@ @LINK_MOTIF@ \
-lRtl -lRgs -lRcf -lRut -lRfm -lRpr -lRds -lMpu -lMap -lMmg -lMcp -lMph -lMut


INCLUDES = @INCLUDE_MOTIF@

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
