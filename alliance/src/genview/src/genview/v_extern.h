/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
 * 
 * This progam is  free software; you can redistribute it  and/or modify it
 * under the  terms of the GNU  General Public License as  published by the
 * Free Software Foundation;  either version 2 of the License,  or (at your
 * option) any later version.
 * 
 * Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

extern  Display *display;
extern  Window V_window;
extern  Window   I_window;
extern  Window   C_window;
extern  Window   T_window;
extern  Window   E_window;
extern  Window   M_window;
extern  XRectangle V_I_pos;
extern  XEvent V_event;
extern  Colormap V_cmap;
extern  int V_depth,V_class;
extern  int V_width,V_height;
extern  Visual *V_vis;
extern  int V_screen;
extern  int V_colors[];
extern  char *V_colorName[];
extern  Pixmap V_PixSave;
extern  GC V_gcCao[];
extern  Font V_vr[];
extern  Font V_menuFont;
extern  char *V_fontName[] ;
extern  Pixmap V_icon[];
extern  GC V_gcMenu;
extern  int   V_xWin,V_yWin,dxWin,dyWin;
extern  int   V_xZoom,V_yZoom,V_dxZoom,V_dyZoom;
extern  long dxChip,dyChip;
extern  float scale;
extern  float V_oldscale;
extern  float V_factor;
extern  int V_fact;
extern  int V_pixDep;
extern  int M_id;

extern GC C_gcIcon,C_gcIconInv,C_gcBgnd, C_gcTextPop;
extern GC C_gcIconText,C_gcIconTextInv,C_gcText;

#define C_NB_OBJECT      (RDS_MAX_LAYER + 5)
#define C_MAX_NB_OBJECT  (RDS_ALL_LAYER + 5)

struct C_Config 
{                                       /* 1:select, 0:unselect */
  char C_VISIBLE[ C_MAX_NB_OBJECT ];    /* code visibily in define order */
  int  C_SCROLL;                        /* 0 < SCROLL < 100  */
  int  C_VIEW;                          /* 0:fill;1:pattern;2:outline */
  int  C_PEEK;                          /* 0:fill;1:pattern;2:outline */
};

extern struct C_Config C_Config_List;
typedef struct {
  short x, y;
  unsigned short width, height;
  short t; /* useful in drawing text in rectangle */
} YRectangle;
extern  YRectangle *V_icon_pos;
