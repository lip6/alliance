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

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#define GC_FGND               ( RDS_MAX_LAYER    )
#define GC_INSREF             ( RDS_MAX_LAYER + 1)
#define GENVIEW_ALL_GC        ( RDS_ALL_LAYER + 2)
#define GENVIEW_MAX_GC        ( RDS_MAX_LAYER + 2)

#define COLOR_FGND            0
#define COLOR_BGND            1
#define COLOR_M_BGND          2
#define COLOR_ICON            3
#define COLOR_ICON_TEXT       4
#define COLOR_ICON_INV        5
#define COLOR_ICON_TEXT_INV   6
#define COLOR_TEXT            7
#define COLOR_BUTTON1         8
#define COLOR_BUTTON2         9
#define COLOR_RED             10
#define COLOR_BLUE            11

#define GENVIEW_MAX_COLOR     12

/* Color Map Size */
#define CMAP_MAX_SIZE         (GENVIEW_MAX_COLOR + RDS_ALL_LAYER)
#define CMAP_SIZE             (GENVIEW_MAX_COLOR + RDS_MAX_LAYER)


/* fill style */
#define FILL                  0
#define PATTERN               1
#define OUTLINE               2

/* window behaviour codes */
#define NO_WINDOW_ACTION      0x00
#define OPEN_WINDOW_INSPECT   0x01
#define OPEN_WINDOW_SETUP     0x02
#define OPEN_WINDOW_MESSAGE   0x03
#define CLOSE_GENVIEW         0x04
#define CLOSE_WINDOW          0x05

/* what's to be done on a CLOSE_WINDOW action, in order to postpone an action */
#define NO_REFRESH            0x00
#define REFRESH_ALL           0x10
#define REFRESH_AREA          0x20

/* mask value used to test retuned values that are ored */
#define REFRESH_MASK          0xF0
#define WINDOW_MASK           0x0F

/* icon state */
#define OFF                   0
#define ON                    1
 
/* functions type */
extern void V_getColors();
extern void V_setGc();
extern void V_getFonts();
extern void V_drawZoomBox();
extern void V_drawFactIcon();
extern void V_coverIcon();
extern void V_setClip();
extern void V_refresh();
extern void V_drawBox();
extern void V_drawString();
extern void V_fitscreen();
extern void V_pan();
extern void V_drawCoords();
extern void V_drawRelCoords();
extern void V_setMenu();
extern void V_drawMenu();
extern void V_iconOn() ;
extern void V_iconOff() ;
extern int V_scrollH();
extern int V_scrollV();
extern int V_scrollRL();
extern int V_scrollLR() ;
extern int V_scrollLH();
extern int V_scrollHL();
extern int V_view_event();
extern int V_clickedIcon();
extern void V_drawPointer();
extern void put_str();
extern void I_setMenu();
extern void I_drawMenu();
extern void I_iconOn() ;
extern void I_iconOff() ;
extern void I_out();
extern void writeLine();
extern void i_pageUp();
extern void i_pageDn();
extern void i_lineUp();
extern void i_lineDn();
extern void clearscreen();
extern void I_open();
extern int I_event();
extern int I_clickedIcon();
extern void T_setMenu();
extern void T_drawMenu();
extern void T_iconOn() ;
extern void T_iconOff() ;
extern void T_out();
extern void T_writeLine();
extern void T_pageUp();
extern void T_pageDn();
extern void T_lineUp();
extern void T_lineDn();
extern void T_clearscreen();
extern void T_free();
extern int readLine();
extern int T_event();
extern int T_clickedIcon();
extern void C_InitGc();
extern void InitConf();
extern void E_setMenu();
extern void E_drawMenu();
extern void V_surround();
