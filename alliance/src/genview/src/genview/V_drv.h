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

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   GenView                         |
|                                                             |
| File    :                   V_drv.h                         |
|                                                             |
| Authors :                Frederic Petrot                    |
|                    Modified by Ludovic Jacomme              |
|                                                             |
| Date    :                   05.02.94                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                             Constants                       |
|                                                             |
\------------------------------------------------------------*/

# ifndef  V_DRV_H  
# define  V_DRV_H  

/*------------------------------------------------------------\
|                                                             |
|                            Macros                           |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                             Variables                       |
|                                                             |
\------------------------------------------------------------*/


  extern Display *display;
  extern Window E_window;
  extern Window T_window;
  extern Window V_window;
  extern Window I_window;
  extern Window C_window;
  extern Window M_window;
  extern XRectangle V_I_pos;
  extern XEvent V_event;
  extern Colormap V_cmap;
  extern int V_depth;
  extern int V_class;
  extern int V_width;
  extern int V_height;
  extern Visual *V_vis;
  extern int V_screen;
  /* these variables are comming from the V_colors.c file */
  extern int    V_colors[ CMAP_MAX_SIZE ];
  extern char  *V_colorName[ GENVIEW_MAX_COLOR ];
  extern Pixmap V_PixSave;
  extern GC     V_gcCao[ GENVIEW_ALL_GC ];
  extern GC     V_gcBox;
  extern GC     C_gcV1;
  extern GC     C_gcV2;
  extern GC     C_gcV3;
  extern GC     C_gcV4;
  extern Font V_vr[6]; /* font for textual output */
  extern char *V_fontName[];
  extern Font V_menuFont;

/* as it is many times used, the V_windows size attributes
  are passed to global variables. */
  extern int V_xWin;
  extern int V_yWin;
  extern int dxWin;
  extern int dyWin;
  extern int V_xZoom;
  extern int V_yZoom;
  extern int V_dxZoom;
  extern int V_dyZoom;
  extern long dxChip;
  extern long dyChip;

/* Some constants for scaling purposes */
  extern int V_fact;
  extern int V_pixDep; /* scrolling is considered in terms of lambda */
  extern YRectangle *V_icon_pos; /* icon position for menu */
  extern int M_id; /* Message id */
  extern int V_lock;

/*------------------------------------------------------------\
|                                                             |
|                             Functions                       |
|                                                             |
\------------------------------------------------------------*/


  extern int  V_view_event();
  extern void V_drawZoomBox();
  extern void V_drawFactIcon();
  extern void V_setClip();
  extern void V_drawPointer();
  extern void V_fitscreen();
  extern void V_pan();
  extern void V_drawCoords();
  extern void V_drawRelCoords();

# endif
