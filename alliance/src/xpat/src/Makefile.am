## Process this file with automake to produce Makefile.in

bin_PROGRAMS = xpat
CFLAGS = @CFLAGS@ @X_CFLAGS@ -DXPAT_VERSION=\"@XPAT_VERSION@\" \
         -DXPAT_TOOL_NAME=\"xpat\" \
	 -DXPAT_DEFAULT_PARAM_NAME=\"${ALLIANCE_TOP}/etc/xpat.par\" \
	 -DXMS_FILE_NAME=\".xpat.cfg\"

xpat_LDADD = @LIBS@ @X_LIBS@ @LINK_MOTIF@ \
	  -lPpt -lPhl -lPat -lAut -lMut

INCLUDES = @INCLUDES@ @INCLUDE_MOTIF@

xpat_SOURCES = \
LIP6bw.h XME_dialog.c XME_dialog.h XME_edit.c XME_edit.h \
XME.h XME_menu.c XME_menu.h XME_message.c XME_message.h XME_panel.c \
XME_panel.h XME_search.c XME_search.h XME_select.c XME_select.h XMF_dialog.c \
XMF_dialog.h XMF_file.c XMF_file.h XMF.h XMF_menu.c XMF_menu.h \
XMF_panel.c XMF_panel.h XMH.h XMH_help.c XMH_help.h XMH_menu.c \
XMH_menu.h XMH_panel.c XMH_panel.h XMS_dialog.c XMS_dialog.h XMS.h \
XMS_menu.c XMS_menu.h XMS_panel.c XMS_panel.h XMS_setup.c XMS_setup.h \
XMT_dialog.c XMT_dialog.h XMT.h XMT_menu.c XMT_menu.h XMT_message.c \
XMT_message.h XMT_panel.c XMT_panel.h XMT_tools.c XMT_tools.h XMV_dialog.c \
XMV_dialog.h XMV.h XMV_map.c XMV_map.h XMV_menu.c XMV_menu.h XMV_message.c \
XMV_message.h XMV_panel.c XMV_panel.h XMV_view.c XMV_view.h XMX_color.c \
XMX_color.h XMX_cursor.c XMX_cursor.h XMX_dialog.c XMX_dialog.h XMX_event.c \
XMX_event.h XMX_graphic.c XMX_graphic.h XMX_grid.c XMX_grid.h XMX.h XMX_icon.c \
XMX_icon.h XMX_menu.c XMX_menu.h XMX_message.c XMX_message.h XMX_motif.c \
XMX_motif.h XMX_panel.c XMX_panel.h XMX_view.c XMX_view.h xpat.c \
xpat.h XPT_bound.c XPT_bound.h XPT_error.c XPT_error.h XPT.h \
XPT_object.c XPT_object.h XPT_pat.c XPT_pat.h XPT_place.c XPT_place.h \
XPT_select.c XPT_select.h XSB_error.c XSB_error.h XSB.h XSB_parse.c \
XSB_parse.h XSB_share.c XSB_share.h XTB_cursor.c XTB_cursor.h XTB_dialog.c \
XTB_dialog.h XTB.h XTB_icon.c XTB_icon.h XTB_menu.c XTB_menu.h \
XTB_panel.c XTB_panel.h XTB_pixmap.c XTB_pixmap.h
