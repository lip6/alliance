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

/* B stands for BITMAP */

#define B_SCALE 6
#define B_SCALE_X I_PLUS_X+I_PLUS_DX+30
#define B_SCALE_Y 2
#define B_SCALE_DX 16
#define B_SCALE_DY 52

#define B_SCROLL 5
#define B_SCROLL_X I_PAN_X+I_PAN_DX+2
#define B_SCROLL_Y 2
#define B_SCROLL_DX 52
#define B_SCROLL_DY 52

/* I stands for ICON */

#define I_MOUSE 0
#define I_MOUSE_X 6
#define I_MOUSE_Y 2
#define I_MOUSE_DX 40
#define I_MOUSE_DY 16
#define I_MOUSE_T 14

#define I_UNDOMOUSE 1
#define I_UNDOMOUSE_X 6
#define I_UNDOMOUSE_Y I_MOUSE_Y+I_MOUSE_DY+2
#define I_UNDOMOUSE_DX 40 
#define I_UNDOMOUSE_DY 16
#define I_UNDOMOUSE_T 11

#define I_FIT 2
#define I_FIT_X 6
#define I_FIT_Y I_UNDOMOUSE_Y+I_UNDOMOUSE_DY+2
#define I_FIT_DX 40
#define I_FIT_DY 16
#define I_FIT_T 11

#define I_PLUS 3
#define I_PLUS_X I_MOUSE_X+I_MOUSE_DX+2
#define I_PLUS_Y 2
#define I_PLUS_DX 40
#define I_PLUS_DY 16
#define I_PLUS_T 8

#define I_MINUS 4
#define I_MINUS_X I_PLUS_X
#define I_MINUS_Y I_FIT_Y
#define I_MINUS_DX 40
#define I_MINUS_DY 16
#define I_MINUS_T 8

#define I_PEEK 5
#define I_PEEK_X B_SCALE_X+B_SCALE_DX+8
#define I_PEEK_Y 2
#define I_PEEK_DX 40
#define I_PEEK_DY 16
#define I_PEEK_T 8

#define I_PAN 6
#define I_PAN_X I_PEEK_X+I_PEEK_DX+2
#define I_PAN_Y 2
#define I_PAN_DX 30
#define I_PAN_DY 16
#define I_PAN_T 6

#define I_UNPEEK 7
#define I_UNPEEK_X I_PEEK_X
#define I_UNPEEK_Y I_PEEK_Y+I_PEEK_DY+2
#define I_UNPEEK_DX 72
#define I_UNPEEK_DY 16
#define I_UNPEEK_T 15

#define I_INSPECT 8
#define I_INSPECT_X I_PEEK_X
#define I_INSPECT_Y I_UNPEEK_Y+I_UNPEEK_DY+2
#define I_INSPECT_DX 72
#define I_INSPECT_DY 16
#define I_INSPECT_T 15

#define I_SETUP 9
#define I_SETUP_X B_SCROLL_X+B_SCROLL_DX+2
#define I_SETUP_Y 2
#define I_SETUP_DX 58
#define I_SETUP_DY 16
#define I_SETUP_T 14

#define I_LOAD 10
#define I_LOAD_X I_SETUP_X+I_SETUP_DX+2
#define I_LOAD_Y 2
#define I_LOAD_DX 48
#define I_LOAD_DY 16
#define I_LOAD_T 12

#define I_MAP 11
#define I_MAP_X I_LOAD_X+I_LOAD_DX+2
#define I_MAP_Y 2
#define I_MAP_DX 42
#define I_MAP_DY 16
#define I_MAP_T 12

#define I_LOCK 12
#define I_LOCK_X I_PLUS_X
#define I_LOCK_Y I_UNDOMOUSE_Y
#define I_LOCK_DX 40
#define I_LOCK_DY 16
#define I_LOCK_T 8 

#define I_FACT 17
#define I_FACT_MIN 2
#define I_FACT_MAX 38
#define I_FACT_X I_PLUS_X+I_PLUS_DX+6
#define I_FACT_Y 20
#define I_FACT_DX 17
#define I_FACT_DY 17
#define I_FACT_T 0

#define I_XY 18
#define I_XY_X I_SETUP_X
#define I_XY_Y I_SETUP_Y+I_SETUP_DY+2
#define I_XY_DX 80
#define I_XY_DY 34
#define I_XY_T 0
 
#define I_DXDY 19
#define I_DXDY_X I_XY_X+I_XY_DX+2
#define I_DXDY_Y I_XY_Y
#define I_DXDY_DX 70
#define I_DXDY_DY 34
#define I_DXDY_T 0
 
#define I_X_X I_SETUP_X + 2
#define I_X_Y I_SETUP_Y+I_SETUP_DY+2

#define I_Y_X I_SETUP_X + 2
#define I_Y_Y I_X_Y+18

#define I_UP 13
#define I_UP_X B_SCROLL_X+1
#define I_UP_Y 3
#define I_UP_DX 24
#define I_UP_DY 24
#define I_RIGHT 14
#define I_RIGHT_X I_UP_X+I_UP_DX+2
#define I_RIGHT_Y 3
#define I_RIGHT_DX 24
#define I_RIGHT_DY 24
#define I_DOWN 15
#define I_DOWN_X I_RIGHT_X+I_RIGHT_DX-I_DOWN_DX
#define I_DOWN_Y I_RIGHT_Y+I_RIGHT_DY+1
#define I_DOWN_DX 24
#define I_DOWN_DY 24
#define I_LEFT 16
#define I_LEFT_X I_UP_X
#define I_LEFT_Y I_UP_Y+I_UP_DY+2
#define I_LEFT_DX 24
#define I_LEFT_DY 24
#define I_UP_T 0
#define I_RIGHT_T 0
#define I_DOWN_T 0
#define I_LEFT_T 0

#define YMENU 58
#define INSCREEN -1
#define NOP 255
