## Process this file with automake to produce Makefile.in

bin_PROGRAMS = xfsm
AM_CFLAGS = @ALLIANCE_CFLAGS@ @X_CFLAGS@ \
	 -DXFSM_DEFAULT_PARAM_NAME=\"${ALLIANCE_TOP}/etc/xfsm.par\" \
	 -DXMS_FILE_NAME=\".xfsm.cfg\"

xfsm_LDADD = @ALLIANCE_LIBS@ @X_LIBS@ @LINK_MOTIF@ \
          -lFtl -lFks -lFvh -lFsm \
	  -lBdd -lAbl -lAut -lMut

INCLUDES = @INCLUDE_MOTIF@


xfsm_SOURCES = \
LIP6bw.h        XME_search.c    XMS_panel.c     XMV_panel.h     XMX_panel.h \
XFS.h           XME_search.h    XMS_panel.h     XMV_view.c      XMX_view.c \
XFS_bdd.c       XME_select.c    XMS_setup.c     XMV_view.h      XMX_view.h \
XFS_bdd.h       XME_select.h    XMS_setup.h     XMX.h           XSB.h \
XFS_bound.c     XMF.h           XMT.h           XMX_color.c     XSB_error.c \
XFS_bound.h     XMF_dialog.c    XMT_dialog.c    XMX_color.h     XSB_error.h \
XFS_error.c     XMF_dialog.h    XMT_dialog.h    XMX_cursor.c    XSB_parse.c \
XFS_error.h     XMF_file.c      XMT_menu.c      XMX_cursor.h    XSB_parse.h \
XFS_fsm.c       XMF_file.h      XMT_menu.h      XMX_dialog.c    XSB_share.c \
XFS_fsm.h       XMF_menu.c      XMT_message.c   XMX_dialog.h    XSB_share.h \
XFS_object.c    XMF_menu.h      XMT_message.h   XMX_event.c     XTB.h \
XFS_object.h    XMF_panel.c     XMT_panel.c     XMX_event.h     XTB_cursor.c \
XFS_select.c    XMF_panel.h     XMT_panel.h     XMX_graphic.c   XTB_cursor.h \
XFS_select.h    XMH.h           XMT_tools.c     XMX_graphic.h   XTB_dialog.c \
XME.h           XMH_help.c      XMT_tools.h     XMX_grid.c      XTB_dialog.h \
XME_dialog.c    XMH_help.h      XMV.h           XMX_grid.h      XTB_icon.c \
XME_dialog.h    XMH_menu.c      XMV_dialog.c    XMX_icon.c      XTB_icon.h \
XME_edit.c      XMH_menu.h      XMV_dialog.h    XMX_icon.h      XTB_menu.c \
XME_edit.h      XMH_panel.c     XMV_map.c       XMX_menu.c      XTB_menu.h \
XME_menu.c      XMH_panel.h     XMV_map.h       XMX_menu.h      XTB_panel.c \
XME_menu.h      XMS.h           XMV_menu.c      XMX_message.c   XTB_panel.h \
XME_message.c   XMS_dialog.c    XMV_menu.h      XMX_message.h   XTB_pixmap.c \
XME_message.h   XMS_dialog.h    XMV_message.c   XMX_motif.c     XTB_pixmap.h \
XME_panel.c     XMS_menu.c      XMV_message.h   XMX_motif.h     xfsm.c \
XME_panel.h     XMS_menu.h      XMV_panel.c     XMX_panel.c     xfsm.h
