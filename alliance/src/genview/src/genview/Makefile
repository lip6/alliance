#
# This file is part of the Alliance CAD System
# Copyright (C) Laboratoire LIP6 - Département ASIM
# Universite Pierre et Marie Curie
# 
# Home page          : http://www-asim.lip6.fr/alliance/
# E-mail             : mailto:alliance-users@asim.lip6.fr
# 
# This progam is  free software; you can redistribute it  and/or modify it
# under the  terms of the GNU  General Public License as  published by the
# Free Software Foundation;  either version 2 of the License,  or (at your
# option) any later version.
# 
# Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
# useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
# Public License for more details.
# 
# You should have received a copy  of the GNU General Public License along
# with the GNU C Library; see the  file COPYING. If not, write to the Free
# Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#

# /*------------------------------------------------------------\
# |                                                             |
# | Tool   :                   GenView                          |
# |                                                             |
# | File   :                   Makefile                         |
# |                                                             |
# | Author :                 Petrot Frederic                    |
# |                                                             |
# | Date   :                    26.09.94                        |
# |                                                             |
# \------------------------------------------------------------*/

include $(ALLIANCE_TOP)/etc/$(ALLIANCE_OS).mk
include $(ALLIANCE_TOP)/etc/libraries.mk
include local.mk

# /*------------------------------------------------------------\
# |                                                             |
# |                            Variables                        |
# |                                                             |
# \------------------------------------------------------------*/

LOCAL_CFLAGS = -g -D$(ALLIANCE_OS) -DGENVIEW_DEBUG

# /*------------------------------------------------------------\
# |                                                             |
# |                             Include                         |
# |                                                             |
# \------------------------------------------------------------*/

LOCAL_INCLUDE = -I$(X11_INCLUDE) -I$(ALLIANCE_INCLUDE)

# /*------------------------------------------------------------\
# |                                                             |
# |                             Library                         |
# |                                                             |
# \------------------------------------------------------------*/

LOCAL_X11_LIB = -L$(X11_LIB) -lX11
LOCAL_MBK_LIB = -L$(ALLIANCE_LIB) \
                $(MGN_L) \
                $(MLU_L) \
                $(MPU_L) \
                $(MCP_L) \
                $(MAP_L) \
                $(MMG_L) \
                $(MCL_L) \
                $(MGL_L) \
                $(MAL_L) \
                $(MVL_L) \
                $(MEL_L) \
                $(MSL_L) \
                $(MHL_L) \
                $(RCN_L) \
                $(MLO_L) \
                $(MPH_L) \
                $(MUT_L)

LOCAL_RDS_LIB = $(RDS_LIB_PATH) \
                $(RTL_L) \
                $(RGS_L) \
                $(RCF_L) \
                $(RFM_L) \
                $(RPR_L) \
                $(RUT_L) \
                $(RDS_L)

LOCAL_LIB = $(LOCAL_X11_LIB) $(LOCAL_RDS_LIB) $(LOCAL_MBK_LIB) -lm


# /*------------------------------------------------------------\
# |                                                             |
# |                             Define                          |
# |                                                             |
# \------------------------------------------------------------*/


LOCAL_MBK_DEFINE   = -DMUT_H="<$(MUT_H)>" \
                     -DMPU_H="<$(MPU_H)>" \
                     -DMPH_H="<$(MPH_H)>" \
                     -DMGN_H="<$(MGN_H)>" \
                     -DMLU_H="<$(MLU_H)>" \
                     -DMLO_H="<$(MLO_H)>"

LOCAL_RDS_DEFINE   = $(RDS_INC_PATH) \
                     -DRDS_H='"$(RDS_H)"' \
                     -DRPR_H='"$(RPR_H)"' \
                     -DRFM_H='"$(RFM_H)"' \
                     -DRWI_H='"$(RWI_H)"' \
                     -DRUT_H='"$(RUT_H)"' \
                     -DRTL_H='"$(RTL_H)"'

LOCAL_GENVIEW_DEFINE = -DALLIANCE_BIN='"$(ALLIANCE_BIN)"'\
                       -DALLIANCE_INCLUDE='"$(ALLIANCE_INCLUDE)"'\
                       -DALLIANCE_VERSION=$(ALLIANCE_VERSION)\
                       -DGENVIEW='"4.4"' \
                       -DGENVIEW_TECHNO_NAME='"GENVIEW_TECHNO_NAME"' \
                       -DGENVIEW_DEFAULT_TECHNO_NAME='"$(ALLIANCE_TOP)/etc/cmos_11.genview"'

LOCAL_DEFINE = $(LOCAL_MBK_DEFINE) $(LOCAL_RDS_DEFINE) \
               $(LOCAL_GENVIEW_DEFINE)

LOCAL_HEADER = $(LOCAL_INCLUDE) $(LOCAL_DEFINE)

# /*------------------------------------------------------------\
# |                                                             |
# |                             Object                          |
# |                                                             |
# \------------------------------------------------------------*/


# Graphic cache files

OBJG = G_parse.o G_cache.o G_user.o G_string.o G_move.o \
       G_peek.o G_inspect.o G_rpeek.o G_global.o 


# Graphic driver and `window manager'

OBJV = V_put.o V_str.o V_clip.o V_colors.o V_boxes.o V_drv.o\
       v_menu.o i_win.o V_scroll.o V_peek.o V_area.o V_clear.o \
       genview.o

# Inspect window routines

OBJI = i_rl.o i_menu.o

# Trace window routines

OBJT = t_win.o t_menu.o t_rl.o

# Edit window routines

OBJE = e_menu.o e_win.o e_vi_display.o e_vi_cmd.o

# Setup window routines

OBJS = C_utils.o C_win.o

# Assembler and interpreter files

OBJD = d_yac.o d_lex.o d_asm.o d_tabcodes.o d_exec.o d_interp.o \
       D_stdlib.o d_interp_fonc.o d_interp_sig.o d_dbx.o \
       d_dbx_trace.o d_term.o

# Message window routines

OBJM = m_menu.o M_win.o

LOCAL_OBJ = $(OBJP) $(OBJG) $(OBJV) $(OBJI) $(OBJT) $(OBJE) $(OBJS) $(OBJD) \
            $(OBJM)

.c.o:
	$(CC) -c $(LOCAL_CFLAGS) $(LOCAL_HEADER) $<

distrib : $(TARGET_BIN)/genview \
          $(TARGET_BIN)/genview_cpp \
          $(TARGET_BIN)/genview_gcc$(PROGRAM_SUFFIX)

$(TARGET_BIN)/genview_gcc$(PROGRAM_SUFFIX):
	cd gcc-1.42; \
	$(MAKE) ALLIANCE_TOP=$(ALLIANCE_TOP) ALLIANCE_OS=$(ALLIANCE_OS) CFLAGS=-D$(ALLIANCE_OS) CC=$(CC);
	cd gcc-1.42; \
	$(MV) genview_gcc$(PROGRAM_SUFFIX) genview_cc1$(PROGRAM_SUFFIX) $(TARGET_BIN)

$(TARGET_BIN)/genview : $(OBJP) $(OBJG) $(OBJV) \
                        $(OBJI) $(OBJT) $(OBJE) $(OBJS) $(OBJD) $(OBJM)
	$(CC) -o $(TARGET_BIN)/genview $(LOCAL_CFLAGS) $(OBJG) $(OBJT) \
          $(OBJE) $(OBJI) $(OBJV) $(OBJD) $(D_OBJ) $(OBJM) $(OBJS) $(LOCAL_LIB)
	# $(STRIP) $(TARGET_BIN)/genview

$(TARGET_BIN)/genview_cpp :
	echo '#! /bin/sh' > 1; \
        echo 'gcc $(LOCAL_MBK_DEFINE) -E $$1 $$2 $$3 | grep -v "^#" > $$4' >> 1; \
        echo 'exit $?' >> 1; \
        chmod a+x 1; \
        $(MV) 1 $@

grog_genview : grog_genview.c
	$(CC) -o grog_genview grog_genview.c $(LOCAL_CFLAGS) $(LOCAL_LIB)

G_global.o  : G_global.c G_global.h
G_parse.o   : G_parse.c G_parse.h G_global.h
G_cache.o   : G_cache.c G_cache.h G_global.h V_put.h v_view.h v_extern.h
G_user.o    : G_user.c G_user.h G_global.h
G_string.o  : G_string.c G_string.h G_global.h v_menu.h v_view.h v_extern.h
G_move.o    : G_move.c G_move.h G_global.h
G_peek.o    : G_peek.c G_peek.h G_global.h G_parse.h C_utils.h G_rpeek.h
G_inspect.o : G_inspect.c G_inspect.h G_global.h G_parse.h G_user.h
G_rpeek.o   : G_rpeek.c G_rpeek.h G_global.h

V_put.o     : V_put.c G_global.h V_put.h C_utils.h v_view.h v_menu.h v_extern.h
V_str.o     : V_str.c G_global.h G_string.h C_utils.h V_str.h v_view.h v_extern.h \
              v_menu.h
V_clip.o    : V_clip.c G_global.h V_clip.h
V_colors.o  : V_colors.c G_parse.h v_view.h v_extern.h V_colors.h V_drv.h
V_boxes.o   : V_boxes.c G_global.h V_boxes.h C_utils.h v_view.h v_menu.h v_extern.h
V_drv.o     : V_drv.c G_global.h v_view.h i_lines.h v_menu.h v_extern.h m_mes_id.h \
              V_drv.h G_peek.h
v_menu.o    : v_menu.c G_global.h v_view.h v_menu.h v_extern.h
i_win.o     : i_win.c G_global.h v_view.h i_menu.h v_menu.h v_extern.h
V_scroll.o  : V_scroll.c G_global.h v_menu.h v_view.h v_extern.h i_lines.h \
              m_mes_id.h C_utils.h V_drv.h V_scroll.h
V_peek.o    : V_peek.c G_global.h V_peek.h C_utils.h v_view.h v_menu.h v_extern.h
V_area.o    : V_area.c G_global.h v_view.h v_menu.h v_extern.h C_utils.h V_drv.h \
              V_area.h
V_clear.o   : V_clear.c G_global.h v_menu.h v_view.h v_extern.h V_clear.h
genview.o   : genview.c G_global.h G_move.h v_view.h v_menu.h v_extern.h e_edit.h \
              e_menu.h m_mes_id.h V_drv.h genview.h c_menu.h

ctrlc.o     : ctrlc.c

i_rl.o      : i_rl.c G_global.h v_view.h i_lines.h c_menu.h v_extern.h
i_menu.o    : i_menu.c v_view.h i_menu.h v_menu.h c_menu.h v_extern.h
t_win.o     : t_win.c G_global.h v_view.h t_menu.h v_menu.h
t_menu.o    : t_menu.c G_global.h v_view.h t_menu.h v_menu.h c_menu.h v_extern.h
t_rl.o      : t_rl.c G_global.h v_view.h c_menu.h v_extern.h
e_menu.o    : e_menu.c G_global.h v_view.h v_extern.h e_menu.h
e_win.o     : e_win.c G_global.h d_asm.h v_view.h v_extern.h e_menu.h m_mes_id.h

e_vi_display.o : e_vi_display.c G_global.h v_view.h c_menu.h v_extern.h e_edit.h
e_vi_cmd.o     : v_view.h c_menu.h v_extern.h e_edit.h

C_utils.o   : C_utils.c G_global.h G_parse.h v_view.h c_menu.h v_extern.h C_utils.h
C_win.o     : C_win.c G_global.h v_view.h c_menu.h v_extern.h V_drv.h C_win.h

d_lex.c : d_asm.lex y.tab.h
	$(LEX) d_asm.lex
	$(SED) -e "s/yy/asm/g" -e "s/YY/ASM/g" lex.yy.c > d_lex.c

d_lex.o : d_lex.c
	$(CC) -c $(LOCAL_CFLAGS) $(LOCAL_HEADER) d_lex.c

y.tab.h d_yac.c : d_asm.yac d_dbx.h
	$(YACC) $(YACCFLAGS) -d d_asm.yac
	$(SED) -e "s/yy/asm/g" -e "s/YY/ASM/g" y.tab.c > d_yac.c
	$(SED) -e "s/yy/asm/g" -e "s/YY/ASM/g" y.tab.h > x.tab.h
	$(MV) x.tab.h y.tab.h

d_dbx.o         : d_dbx.c d_dbx.h
	$(CC) -c $(LOCAL_CFLAGS) $(LOCAL_HEADER) $< -DD_DBXDEBUG
d_dbx.c : d_dbx.yac d_dbx.h
	$(YACC) $(YACCFLAGS) d_dbx.yac
	$(SED) -e "s/yy/d_dbx/g" -e "s/YY/D_DBX/g" y.tab.c > d_dbx.c

d_yac.o : d_yac.c
	$(CC) -c $(LOCAL_CFLAGS) $(LOCAL_HEADER) d_yac.c

d_asm.o : d_asm.c d_asm.h d_dbx.h
	$(CC) -c $(LOCAL_CFLAGS) $(LOCAL_HEADER) -DFLEX_SCANNER d_asm.c

d_tabcodes.o    : d_tabcodes.c d_asm.h d_codes.h
d_exec.o        : d_exec.c d_fonc.h d_asm.h d_interp_lib.h d_dbx.h
d_interp.o      : d_interp.c d_asm.h d_codes.h d_fonc.h
D_stdlib.o      : D_stdlib.c G_global.h G_user.h G_cache.h d_interp_lib.h d_fonc.h \
                  d_asm.h D_stdlib.h
d_interp_fonc.o : d_interp_fonc.c G_global.h d_fonc.h d_codes.h d_asm.h
d_interp_sig.o  : d_interp_sig.c d_asm.h
d_dbx_trace.o   : d_dbx_trace.c d_dbx.h
d_term.o        : d_term.c v_view.h v_extern.h d_asm.h


m_menu.o    : m_menu.c G_global.h v_view.h m_menu.h m_mes_id.h v_menu.h v_extern.h
M_win.o     : M_win.c G_global.h G_cache.h v_view.h v_extern.h m_menu.h v_menu.h \
              i_lines.h m_mes_id.h e_edit.h V_drv.h M_win.h
	$(CC) -DWELCOME='"GenView : Public Release 4.2"' \
              -c $(LOCAL_CFLAGS) $(LOCAL_HEADER) M_win.c

clean :
	$(RM) -f $(LOCAL_OBJ) d_lex.c d_yac.c y.tab.c y.tab.h lex.yy.c
	cd gcc-1.42 ; \
	$(MAKE) ALLIANCE_TOP=$(ALLIANCE_TOP) ALLIANCE_OS=$(ALLIANCE_OS) clean


realclean : clean
	$(RM) -f $(TARGET_BIN)/genview $(TARGET_BIN)/genview_cpp $(TARGET_BIN)/genview_gcc $(TARGET_BIN)/genview_cc1
	cd gcc-1.42 ; \
	$(MAKE) ALLIANCE_TOP=$(ALLIANCE_TOP) ALLIANCE_OS=$(ALLIANCE_OS) realclean


