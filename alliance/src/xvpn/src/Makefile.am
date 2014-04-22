## Process this file with automake to produce Makefile.in

bin_PROGRAMS = xvpn
AM_CFLAGS    = @ALLIANCE_CFLAGS@ @X_CFLAGS@ \
               -DXVPN_DEFAULT_PARAM_NAME=\"${ALLIANCE_TOP}/etc/xvpn.par\" \
               -DXMS_FILE_NAME=\".xvpn.cfg\" \
               -I$(top_srcdir)/aut/src \
               -I$(top_srcdir)/vex/src \
               -I$(top_srcdir)/vpn/src \
               -I$(top_srcdir)/vtl/src \
               -I$(top_srcdir)/abl/src \
               -I$(top_srcdir)/mbk/src

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

# Xp library; sometimes needed for Motif >= 2.1.  (-lXp)
LIBXP = @XP_LIBS@

# X toolkit library.  (-lXt)
LIBXT = @XT_LIBS@

# X library.  (-lX11 -lSM -lICE -lnsl -lsocket)
LIBX11 = @X_PRE_LIBS@ @X11_LIBS@ @X_EXTRA_LIBS@

# All libraries shown above
ALL_X_LIBS = $(X_LDFLAGS) $(LIBXM) $(LIBXT) \
             $(LIBXP) $(LIBXEXT) $(LIBX11)



xvpn_LDADD = @ALLIANCE_LIBS@ $(ALL_X_LIBS) \
             -L$(top_srcdir)/abl/src \
             -L$(top_srcdir)/aut/src \
             -L$(top_srcdir)/mbk/src \
             -L$(top_srcdir)/vbh/src \
             -L$(top_srcdir)/vex/src \
             -L$(top_srcdir)/vpn/src \
             -lVpn -lVbh -lVex -lAbl -lAut -lMut

xvpn_SOURCES = \
LIP6bw.h XME_dialog.c XME_dialog.h XME_edit.c XME_edit.h XME.h XME_menu.c \
XME_menu.h XME_message.c XME_message.h XME_panel.c XME_panel.h XME_search.c \
XME_search.h XME_select.c XME_select.h XMF_dialog.c XMF_dialog.h XMF_file.c \
XMF_file.h XMF.h XMF_menu.c XMF_menu.h XMF_panel.c XMF_panel.h XMH.h XMH_help.c \
XMH_help.h XMH_menu.c XMH_menu.h XMH_panel.c XMH_panel.h XMS_dialog.c \
XMS_dialog.h XMS.h XMS_menu.c XMS_menu.h XMS_panel.c XMS_panel.h XMS_setup.c \
XMS_setup.h XMT_dialog.c XMT_dialog.h XMT.h XMT_menu.c XMT_menu.h XMT_message.c \
XMT_message.h XMT_panel.c XMT_panel.h XMT_tools.c XMT_tools.h XMV_dialog.c \
XMV_dialog.h XMV.h XMV_map.c XMV_map.h XMV_menu.c XMV_menu.h XMV_message.c \
XMV_message.h XMV_panel.c XMV_panel.h XMV_view.c XMV_view.h XMX_color.c \
XMX_color.h XMX_cursor.c XMX_cursor.h XMX_dialog.c XMX_dialog.h XMX_event.c \
XMX_event.h XMX_graphic.c XMX_graphic.h XMX_grid.c XMX_grid.h XMX.h XMX_icon.c \
XMX_icon.h XMX_menu.c XMX_menu.h XMX_message.c XMX_message.h XMX_motif.c \
XMX_motif.h XMX_panel.c XMX_panel.h XMX_view.c XMX_view.h XSB_error.c \
XSB_error.h XSB.h XSB_parse.c XSB_parse.h XSB_share.c XSB_share.h XTB_cursor.c \
XTB_cursor.h XTB_dialog.c XTB_dialog.h XTB.h XTB_icon.c XTB_icon.h XTB_menu.c \
XTB_menu.h XTB_panel.c XTB_panel.h XTB_pixmap.c XTB_pixmap.h XVP_bound.c \
XVP_bound.h XVP_error.c XVP_error.h XVP.h xvpn.c xvpn.h XVP_object.c \
XVP_object.h XVP_select.c XVP_select.h
