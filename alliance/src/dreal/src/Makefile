# /*------------------------------------------------------------\
# |                                                             |
# | This file is part of the Alliance CAD System Copyright      |
# | (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
# |                                                             |
# | Home page      : http://www-asim.lip6.fr/alliance/          |
# | E-mail support : mailto:alliance-support@asim.lip6.fr       |
# |                                                             |
# | This progam is  free software; you can redistribute it      |
# | and/or modify it under the  terms of the GNU General Public |
# | License as  published by the Free Software Foundation;      |
# | either version 2 of the License, or (at your option) any    |
# | later version.                                              |
# |                                                             |
# | Alliance VLSI  CAD System  is distributed  in the hope that |
# | it  will be useful, but WITHOUT  ANY WARRANTY;              |
# | without even the  implied warranty of MERCHANTABILITY or    |
# | FITNESS FOR A PARTICULAR PURPOSE. See the GNU General       |
# | Public License for more details.                            |
# |                                                             |
# | You should have received a copy  of the GNU General Public  |
# | License along with the GNU C Library; see the file COPYING. |
# | If not, write to the Free Software Foundation, Inc.,        |
# | 675 Mass Ave, Cambridge, MA 02139, USA.                     |
# |                                                             |
# \------------------------------------------------------------*/

# /*------------------------------------------------------------\
# |                                                             |
# | Tool   :                     DREAL                          |
# |                                                             |
# | File   :                    Makefile                        |
# |                                                             |
# | Author :                 Jacomme Ludovic                    |
# |                                                             |
# | Date   :                    20.02.96                        |
# |                                                             |
# \------------------------------------------------------------*/

include $(ALLIANCE_TOP)/etc/$(ALLIANCE_OS).mk
include $(ALLIANCE_TOP)/etc/libraries.mk
include local.mk

# /*------------------------------------------------------------\
# |                                                             |
# |                             Variables                       |
# |                                                             |
# \------------------------------------------------------------*/

LOCAL_CFLAGS = $(CFLAGS) -D$(ALLIANCE_OS)

# /*------------------------------------------------------------\
# |                                                             |
# |                            Binary                           |
# |                                                             |
# \------------------------------------------------------------*/

LOCAL_DREAL_BIN = dreal

# /*------------------------------------------------------------\
# |                                                             |
# |                             Include                         |
# |                                                             |
# \------------------------------------------------------------*/

LOCAL_INCLUDE = $(MOTIF_INCLUDE) $(XPM_INCLUDE) -I$(X11_INCLUDE) \
                -I$(ALLIANCE_INCLUDE)

# /*------------------------------------------------------------\
# |                                                             |
# |                             Library                         |
# |                                                             |
# \------------------------------------------------------------*/

LOCAL_MOTIF_LIB = -L$(MOTIF_LIB) -lXm

LOCAL_X11_LIB   = -L$(X11_LIB) -lXt -lX11 $(XPM_LIB)

LOCAL_MBK_LIB = -L$(ALLIANCE_LIB) \
                $(MPU_L) \
                $(MAP_L) \
                $(MMG_L) \
                $(MCP_L) \
                $(MPH_L) \
                $(MUT_L)

LOCAL_RDS_LIB = $(RDS_LIBRARY) \
                -L$(ALLIANCE_LIB) \
                $(RTL_L) \
                $(RGS_L) \
                $(RCF_L) \
                $(RUT_L) \
                $(RFM_L) \
                $(RPR_L) \
                $(RDS_L)

LOCAL_LIB = $(LOCAL_MOTIF_LIB) $(LOCAL_X11_LIB) $(LOCAL_RDS_LIB) \
            $(LOCAL_MBK_LIB)

# /*------------------------------------------------------------\
# |                                                             |
# |                             Define                          |
# |                                                             |
# \------------------------------------------------------------*/

LOCAL_X11_DEFINE   = -D_NO_PROTO

LOCAL_MBK_DEFINE   = -DMUT_H='"$(MUT_H)"' \
                     -DMPH_H='"$(MPH_H)"'

LOCAL_RDS_DEFINE   = $(RDS_INCLUDE) \
                     -DRDS_H='"$(RDS_H)"' \
                     -DRPR_H='"$(RPR_H)"' \
                     -DRFM_H='"$(RFM_H)"' \
                     -DRWI_H='"$(RWI_H)"' \
                     -DRUT_H='"$(RUT_H)"' \
                     -DRTL_H='"$(RTL_H)"'

LOCAL_DREAL_DEFINE = -DGTB_H='"GTB.h"' -DGSB_H='"GSB.h"' \
                     -DGMF_H='"GMF.h"' -DGMV_H='"GMV.h"' -DGMC_H='"GMC.h"' \
                     -DGMT_H='"GMT.h"' -DGMS_H='"GMS.h"' -DGMH_H='"GMH.h"' \
                     -DGMX_H='"GMX.h"' -DGRD_H='"GRD.h"' -DGME_H='"GME.h"' \
                     -DALLIANCE_TOP='"$(ALLIANCE_TOP)"' \
                     -DDREAL_DEFAULT_TECHNO_NAME='"etc/cmos_7.$(LOCAL_DREAL_BIN)"' \
                     -DGMS_FILE_NAME='".$(LOCAL_DREAL_BIN).cfg"' \
                     -DDREAL_WINDOW_DX='"DREAL_WINDOW_DX"' \
                     -DDREAL_WINDOW_DY='"DREAL_WINDOW_DY"' \
                     -DDREAL_WINDOW_XMIN='"DREAL_WINDOW_XMIN"' \
                     -DDREAL_WINDOW_YMIN='"DREAL_WINDOW_YMIN"' \
                     -DDREAL_WINDOW_SIDE='"DREAL_WINDOW_SIDE"' \
                     -DALLIANCE_VERSION=$(ALLIANCE_VERSION) \
                     -DDREAL_TOOL_NAME='"$(LOCAL_DREAL_BIN)"' \
                     -DDREAL_VERSION='"1.14"'

LOCAL_DEFINE = $(LOCAL_X11_DEFINE) $(LOCAL_RDS_DEFINE) \
               $(LOCAL_MBK_DEFINE) $(LOCAL_DREAL_DEFINE)

LOCAL_HEADER = $(LOCAL_INCLUDE) $(LOCAL_DEFINE)

# /*------------------------------------------------------------\
# |                                                             |
# |                             Object                          |
# |                                                             |
# \------------------------------------------------------------*/

LOCAL_DREAL_OBJ  = dreal.o 
LOCAL_GSB_OBJ    = GSB_share.o GSB_parse.o GSB_error.o
LOCAL_GRD_OBJ    = GRD_rds.o GRD_window.o GRD_error.o GRD_undo.o GRD_select.o \
                   GRD_search.o
LOCAL_GTB_OBJ    = GTB_icon.o GTB_panel.o GTB_menu.o GTB_pixmap.o    \
                   GTB_cursor.o GTB_dialog.o
LOCAL_GMX_OBJ    = GMX_motif.o GMX_color.o GMX_graphic.o GMX_panel.o  \
                   GMX_menu.o GMX_message.o GMX_grid.o GMX_event.o    \
                   GMX_cursor.o GMX_dialog.o GMX_view.o \
                   GMX_icon.o
LOCAL_GMF_OBJ    = GMF_menu.o GMF_panel.o GMF_dialog.o GMF_file.o
LOCAL_GME_OBJ    = GME_edit.o GME_panel.o GME_dialog.o GME_menu.o \
                   GME_message.o GME_modify.o GME_search.o GME_select.o
LOCAL_GMC_OBJ    = GMC_create.o GMC_menu.o GMC_panel.o GMC_dialog.o \
                   GMC_message.o
LOCAL_GMV_OBJ    = GMV_view.o GMV_panel.o GMV_menu.o GMV_dialog.o \
                   GMV_message.o GMV_map.o
LOCAL_GMT_OBJ    = GMT_tools.o GMT_menu.o GMT_panel.o GMT_dialog.o \
                   GMT_message.o
LOCAL_GMS_OBJ    = GMS_setup.o GMS_menu.o GMS_dialog.o GMS_panel.o 
LOCAL_GMH_OBJ    = GMH_help.o GMH_menu.o GMH_panel.o 

LOCAL_OBJ        = $(LOCAL_GSB_OBJ) $(LOCAL_GRD_OBJ) $(LOCAL_GTB_OBJ) $(LOCAL_GMX_OBJ) \
                   $(LOCAL_GMF_OBJ) $(LOCAL_GMV_OBJ) $(LOCAL_GMT_OBJ) $(LOCAL_GMS_OBJ) \
                   $(LOCAL_GMC_OBJ) $(LOCAL_GME_OBJ) $(LOCAL_GMH_OBJ) $(LOCAL_DREAL_OBJ)

# /*------------------------------------------------------------\
# |                                                             |
# |                             Dreal                           |
# |                                                             |
# \------------------------------------------------------------*/

distrib : $(LOCAL_TARGET_BIN)/$(LOCAL_DREAL_BIN)

$(LOCAL_TARGET_BIN)/$(LOCAL_DREAL_BIN) : $(LOCAL_OBJ)
	$(CC) $(LOCAL_CFLAGS) -o $(LOCAL_TARGET_BIN)/$(LOCAL_DREAL_BIN) \
              $(LOCAL_OBJ) $(LOCAL_LIB) -lm
	$(STRIP) $(LOCAL_TARGET_BIN)/$(LOCAL_DREAL_BIN)


dreal.o : dreal.c dreal.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  dreal.c

# /*------------------------------------------------------------\
# |                                                             |
# |                         Dreal Share Box                     |
# |                                                             |
# \------------------------------------------------------------*/

GSB_parse.o  : GSB.h GSB_parse.c GSB_parse.h GSB_error.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GSB_parse.c

GSB_error.o  : GSB.h GSB_error.c GSB_error.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GSB_error.c

GSB_share.o  : GSB.h GSB_share.c GSB_share.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GSB_share.c

# /*------------------------------------------------------------\
# |                                                             |
# |                          Dreal Rds                          |
# |                                                             |
# \------------------------------------------------------------*/

GRD_rds.o    : GSB.h GRD.h GRD_rds.c GRD_rds.h GRD_window.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GRD_rds.c

GRD_undo.o   : GSB.h GRD.h GRD_undo.c GRD_undo.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GRD_undo.c

GRD_search.o : GSB.h GRD.h GRD_search.c GRD_search.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GRD_search.c

GRD_select.o : GSB.h GRD.h GRD_select.c GRD_select.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GRD_select.c

GRD_window.o : GSB.h GRD.h GRD_window.c GRD_window.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GRD_window.c

GRD_error.o  : GSB.h GRD.h GRD_error.c GRD_error.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GRD_error.c


# /*------------------------------------------------------------\
# |                                                             |
# |                         Dreal Tools Box                     |
# |                                                             |
# \------------------------------------------------------------*/

GTB_cursor.o  : GTB.h GTB_cursor.c GTB_cursor.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GTB_cursor.c

GTB_dialog.o  : GTB.h GTB_dialog.c GTB_dialog.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GTB_dialog.c

GTB_icon.o    : GTB.h GTB_icon.c GTB_icon.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GTB_icon.c

GTB_menu.o    : GTB.h GTB_menu.c GTB_menu.h 
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GTB_menu.c

GTB_panel.o   : GTB.h GTB_panel.c GTB_panel.h GTB_dialog.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GTB_panel.c

GTB_pixmap.o  : GTB.h GTB_pixmap.c GTB_pixmap.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GTB_pixmap.c

# /*------------------------------------------------------------\
# |                                                             |
# |                         Dreal Motif X11                     |
# |                                                             |
# \------------------------------------------------------------*/

GMX_icon.o    : GSB.h GMX.h GMX_icon.c GMX_icon.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMX_icon.c

GMX_color.o   : GSB.h GMX.h GMX_color.c GMX_color.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMX_color.c

GMX_cursor.o  : GSB.h GMX.h GMX_cursor.c GMX_cursor.h GMX_view.h GMX_grid.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMX_cursor.c

GMX_dialog.o  : GSB.h GMX.h GMX_dialog.c GMX.h GMX_dialog.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMX_dialog.c

GMX_graphic.o : GSB.h GMX.h GMX_graphic.c GMX_graphic.h GMX_view.h GMX_grid.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMX_graphic.c

GMX_grid.o    : GSB.h GMX.h GMX_grid.c GMX_grid.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMX_grid.c

GMX_menu.o    : GSB.h GTB.h GMX.h GMF.h GMV.h GMS.h GMT.h GMH.h \
                GMX_menu.c GMX_menu.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMX_menu.c

GMX_message.o : GSB.h GTB.h GMX.h GMX_message.c GMX_message.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMX_message.c

GMX_motif.o   : GSB.h GTB.h GMX.h GMX_motif.c GMX_motif.h GMX_graphic.h \
                GMX_event.h GMX_message.h GMX_menu.h GMX_color.h        \
                GMX_panel.h GMX_dialog.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMX_motif.c

GMX_event.o   : GSB.h GTB.h GRD.h GMX.h GMV.h GMX_event.c GMX_grid.h \
                GMX_event.h GMX_cursor.h GMX_event.h \
                GMX_message.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMX_event.c

GMX_panel.o   : GSB.h GTB.h GMF.h GMV.h GMT.h GMS.h GMH.h GMX.h \
                GMX_panel.c GMX_panel.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMX_panel.c

GMX_view.o    : GSB.h GRD.h GMX_view.c GMX_view.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMX_view.c

# /*------------------------------------------------------------\
# |                                                             |
# |                         Dreal Menu File                     |
# |                                                             |
# \------------------------------------------------------------*/

GMF_panel.o  : GSB.h GRD.h GTB.h GMX.h GMF.h GMF_panel.c GMF_panel.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMF_panel.c

GMF_menu.o   : GSB.h GRD.h GTB.h GMX.h GMF.h GMF_menu.c GMF_menu.h \
               GMF_dialog.h GMF_file.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMF_menu.c

GMF_dialog.o  : GSB.h GRD.h GTB.h GMX.h GMF.h GMF_dialog.c GMF_dialog.h \
                GMF_file.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMF_dialog.c

GMF_file.o  :  GSB.h GRD.h GTB.h GMX.h GMF.h GMF_file.c GMF_file.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMF_file.c

# /*------------------------------------------------------------\
# |                                                             |
# |                         Dreal Menu Edit                     |
# |                                                             |
# \------------------------------------------------------------*/

GME_search.o  : GSB.h GRD.h GMX.h GTB.h GME.h GME_search.c GME_search.h \
                GME_panel.h GME_edit.h GME_message.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GME_search.c

GME_modify.o  : GSB.h GRD.h GMX.h GTB.h GME.h GME_modify.c GME_modify.h \
                GME_panel.h GME_edit.h GME_message.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GME_modify.c

GME_edit.o    : GSB.h GRD.h GMX.h GTB.h GME.h GME_edit.c GME_edit.h \
                GME_message.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GME_edit.c

GME_select.o  : GSB.h GRD.h GMX.h GTB.h GME.h GME_select.c GME_select.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GME_select.c

GME_panel.o   : GSB.h GRD.h GMX.h GTB.h GME.h GME_panel.c GME_panel.h \
                GME_message.h GME_dialog.h GME_modify.h GME_search.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GME_panel.c

GME_menu.o    : GSB.h GRD.h GMX.h GTB.h GME.h GME_menu.c GME_menu.h \
                GME_edit.h GME_modify.h GME_message.h 
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GME_menu.c

GME_dialog.o  : GSB.h GRD.h GMX.h GTB.h GME.h GME_dialog.c GME_dialog.h \
                GME_modify.h GME_search.h GME_edit.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GME_dialog.c

GME_message.o : GSB.h GRD.h GMX.h GTB.h GME.h GME_message.c GME_message.h \
                GME_modify.h 
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GME_message.c

# /*------------------------------------------------------------\
# |                                                             |
# |                         Dreal Menu Create                   |
# |                                                             |
# \------------------------------------------------------------*/

GMC_menu.o    : GSB.h GRD.h GTB.h GMX.h GMC.h GMC_menu.c GMC_menu.h \
                GMC_create.h GMC_message.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMC_menu.c

GMC_panel.o   : GSB.h GRD.h GTB.h GMX.h GMC.h GMC_panel.c GMC_panel.h \
                GMC_dialog.h GMC_create.h GMC_message.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMC_panel.c

GMC_dialog.o  : GSB.h GRD.h GTB.h GMX.h GMC.h GMC_dialog.c GMC_dialog.h \
                GMC_create.h GMC_message.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMC_dialog.c

GMC_create.o  : GSB.h GRD.h GTB.h GMX.h GMC.h GMC_create.c GMC_create.h \
                GMC_dialog.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMC_create.c

GMC_message.o : GSB.h GRD.h GTB.h GMX.h GMC.h GMC_message.c GMC_message.h \
                GMC_create.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMC_message.c

# /*------------------------------------------------------------\
# |                                                             |
# |                         Dreal Menu View                     |
# |                                                             |
# \------------------------------------------------------------*/

GMV_panel.o   : GSB.h GTB.h GMX.h GMV.h GMV_panel.c GMV_panel.h \
                GMV_dialog.h GMV_view.h GMV_message.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMV_panel.c

GMV_menu.o    : GSB.h GTB.h GMX.h GMV.h GMV_menu.c GMV_menu.h\
                GMV_dialog.h GMV_view.h GMV_map.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMV_menu.c

GMV_dialog.o  : GSB.h GTB.h GMX.h GMV.h GMV_dialog.c GMV_dialog.h \
                GMV_view.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMV_dialog.c

GMV_map.o     : GSB.h GTB.h GMX.h GMV.h GMV_map.c GMV_map.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMV_map.c

GMV_message.o : GSB.h GTB.h GMX.h GMV.h GMV_message.c GMV_message.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMV_message.c

GMV_view.o    : GSB.h GTB.h GMX.h GMV.h GMV_view.c GMV_view.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMV_view.c

# /*------------------------------------------------------------\
# |                                                             |
# |                         Dreal Menu Tools                    |
# |                                                             |
# \------------------------------------------------------------*/

GMT_tools.o   : GTB.h GSB.h GMX.h GRD.h GMT.h GMT_tools.c GMT_tools.h \
                GMT_message.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMT_tools.c

GMT_menu.o    : GTB.h GSB.h GMX.h GRD.h GMT.h  GMT_menu.c GMT_menu.h \
                GMT_message.h GMT_tools.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMT_menu.c

GMT_panel.o   : GTB.h GSB.h GMX.h GRD.h GMT.h  GMT_panel.c GMT_panel.h \
                GMT_message.h GMT_tools.h GMT_dialog.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMT_panel.c

GMT_dialog.o  : GTB.h GSB.h GMX.h GRD.h GMT.h  GMT_dialog.c GMT_dialog.h \
                GMT_tools.h 
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMT_dialog.c

GMT_message.o : GTB.h GSB.h GMX.h GRD.h GMT.h  GMT_message.c GMT_message.h \
                GMT_tools.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMT_message.c

# /*------------------------------------------------------------\
# |                                                             |
# |                         Dreal Menu Setup                    |
# |                                                             |
# \------------------------------------------------------------*/

GMS_menu.o   : GTB.h GSB.h GMS.h GMS_menu.c GMS_menu.h GMS_setup.h 
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMS_menu.c

GMS_dialog.o : GTB.h GSB.h GMS.h GMS_dialog.c GMS_dialog.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMS_dialog.c

GMS_panel.o  : GTB.h GSB.h GMS.h GMS_panel.c GMS_panel.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMS_panel.c

GMS_setup.o  : GTB.h GSB.h GMX.h GMF.h GMV.h GMT.h GMS.h GMH.h \
               GMS_setup.c GMS_setup.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMS_setup.c

# /*------------------------------------------------------------\
# |                                                             |
# |                         Dreal Menu Help                     |
# |                                                             |
# \------------------------------------------------------------*/

GMH_menu.o   : GTB.h GSB.h GMH.h GMH_menu.c GMH_menu.h 
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMH_menu.c

GMH_panel.o : GTB.h GSB.h GMH.h GMH_panel.c GMH_panel.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMH_panel.c

GMH_help.o : GTB.h GSB.h GMX.h GMH.h GMH_help.c GMH_help.h
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_HEADER) -c  GMH_help.c

# /*------------------------------------------------------------\
# |                                                             |
# |                             Clean                           |
# |                                                             |
# \------------------------------------------------------------*/

clean :
	$(RM) -f $(LOCAL_OBJ)

# /*------------------------------------------------------------\
# |                                                             |
# |                         RealClean                           |
# |                                                             |
# \------------------------------------------------------------*/

realclean : clean
	$(RM) -f $(LOCAL_TARGET_BIN)/$(LOCAL_DREAL_BIN)

# /*------------------------------------------------------------\
# |                                                             |
# |                          Clean Object                       |
# |                                                             |
# \------------------------------------------------------------*/

cleanobj :
	$(RM) -f $(LOCAL_OBJ)
