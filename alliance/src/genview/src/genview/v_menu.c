/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
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

# include MUT_H
# include MPH_H
# include RDS_H
# include RWI_H
# include RUT_H
# include RPR_H
# include RFM_H

# include "G_global.h"
# include "v_view.h"
# include "v_menu.h"
# include "v_extern.h"


static char scroll[] = 
{
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0xf9, 0xff,
   0xff, 0x0f, 0xff, 0xff, 0xff, 0xf9, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff,
   0xf9, 0xff, 0xff, 0x0f, 0xff, 0xcf, 0xff, 0xf9, 0xff, 0xff, 0x0f, 0xff,
   0x87, 0xff, 0xf9, 0xff, 0xfe, 0x0f, 0xff, 0x03, 0xff, 0xf9, 0xff, 0xfc,
   0x0f, 0xff, 0x01, 0xfe, 0xf9, 0xff, 0xf8, 0x0f, 0xff, 0x00, 0xfc, 0xf9,
   0xff, 0xf0, 0x0f, 0x7f, 0x00, 0xf8, 0xf9, 0xff, 0xe0, 0x0f, 0x3f, 0x00,
   0xf0, 0x39, 0x00, 0xc0, 0x0f, 0x1f, 0x00, 0xe0, 0x39, 0x00, 0x80, 0x0f,
   0xff, 0x03, 0xff, 0x39, 0x00, 0x00, 0x0f, 0xff, 0x03, 0xff, 0x39, 0x00,
   0x00, 0x0f, 0xff, 0x03, 0xff, 0x39, 0x00, 0x80, 0x0f, 0xff, 0x03, 0xff,
   0x39, 0x00, 0xc0, 0x0f, 0xff, 0x03, 0xff, 0xf9, 0xff, 0xe0, 0x0f, 0xff,
   0x03, 0xff, 0xf9, 0xff, 0xf0, 0x0f, 0xff, 0x03, 0xff, 0xf9, 0xff, 0xf8,
   0x0f, 0xff, 0x03, 0xff, 0xf9, 0xff, 0xfc, 0x0f, 0xff, 0x03, 0xff, 0xf9,
   0xff, 0xfe, 0x0f, 0xff, 0x03, 0xff, 0xf9, 0xff, 0xff, 0x0f, 0xff, 0xff,
   0xff, 0xf9, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0xf9, 0xff, 0xff, 0x0f,
   0xff, 0xff, 0xff, 0xf9, 0xff, 0xff, 0x0f, 0x01, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x08, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xff, 0xff, 0xff,
   0xf9, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0xf9, 0xff, 0xff, 0x0f, 0xff,
   0xff, 0xff, 0xf9, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0xf9, 0x0f, 0xfc,
   0x0f, 0xff, 0xf7, 0xff, 0xf9, 0x0f, 0xfc, 0x0f, 0xff, 0xf3, 0xff, 0xf9,
   0x0f, 0xfc, 0x0f, 0xff, 0xf1, 0xff, 0xf9, 0x0f, 0xfc, 0x0f, 0xff, 0xf0,
   0xff, 0xf9, 0x0f, 0xfc, 0x0f, 0x7f, 0xf0, 0xff, 0xf9, 0x0f, 0xfc, 0x0f,
   0x3f, 0x00, 0xc0, 0xf9, 0x0f, 0xfc, 0x0f, 0x1f, 0x00, 0xc0, 0xf9, 0x0f,
   0xfc, 0x0f, 0x0f, 0x00, 0xc0, 0xf9, 0x0f, 0xfc, 0x0f, 0x0f, 0x00, 0xc0,
   0xf9, 0x0f, 0xfc, 0x0f, 0x1f, 0x00, 0xc0, 0x79, 0x00, 0x80, 0x0f, 0x3f,
   0x00, 0xc0, 0xf9, 0x00, 0xc0, 0x0f, 0x7f, 0xf0, 0xff, 0xf9, 0x01, 0xe0,
   0x0f, 0xff, 0xf0, 0xff, 0xf9, 0x03, 0xf0, 0x0f, 0xff, 0xf1, 0xff, 0xf9,
   0x07, 0xf8, 0x0f, 0xff, 0xf3, 0xff, 0xf9, 0x0f, 0xfc, 0x0f, 0xff, 0xf7,
   0xff, 0xf9, 0x1f, 0xfe, 0x0f, 0xff, 0xff, 0xff, 0xf9, 0x3f, 0xff, 0x0f,
   0xff, 0xff, 0xff, 0xf9, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0xf9, 0xff,
   0xff, 0x0f, 0xff, 0xff, 0xff, 0xf9, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0x0f
};

static char scl[] = 
{
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0x7f,
   0xff, 0x3f, 0xff, 0x3f, 0xff, 0x1f, 0xff, 0x1f, 0xff, 0x0f, 0xff, 0x0f,
   0xff, 0x0f, 0xff, 0x07, 0xff, 0x07, 0xff, 0x03, 0xff, 0x03, 0xff, 0x03,
   0xff, 0x01, 0xff, 0x01, 0xff, 0x00, 0xff, 0x00, 0x7f, 0x00, 0x7f, 0x00,
   0x7f, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00,
   0x0f, 0x00, 0x0f, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x03, 0x00,
   0x03, 0x00, 0xe1, 0x1f, 0xf9, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static Pixmap icon[2];
static char *V_menu[] = {
    "IN",
    "OUT",
    "FIT",
    "ZOOM",
    "MOOZ",
    "PEEK",
    "PAN",
    "REFRESH",
    "INSPECT",
    "SETUP",
    "LOAD",
    "MAP",
    "LOCK",
    "X:",
    "Y:",
    "DX:",
    "DY:"
};

static int dx;


void
  V_setMenu()
{
static int first;

  if (first == 0) { /* creating the bitmaps associated with the icons */
    icon[0] = XCreatePixmapFromBitmapData(display, V_window, scl,
        B_SCALE_DX, B_SCALE_DY,
        V_colors[COLOR_ICON], V_colors[COLOR_M_BGND], V_depth);
  
    icon[1] = XCreatePixmapFromBitmapData(display, V_window, scroll,
        B_SCROLL_DX, B_SCROLL_DY,
        V_colors[COLOR_ICON], V_colors[COLOR_M_BGND], V_depth);
  
    V_icon_pos = (YRectangle *)mbkalloc((I_DXDY + 1) * sizeof(YRectangle));
    first++;
  }

  V_icon_pos[I_MOUSE].x = I_MOUSE_X;
  V_icon_pos[I_MOUSE].y = I_MOUSE_Y;
  V_icon_pos[I_MOUSE].width = I_MOUSE_DX;
  V_icon_pos[I_MOUSE].height = I_MOUSE_DY;
  V_icon_pos[I_MOUSE].t = I_MOUSE_T;

  V_icon_pos[I_UNDOMOUSE].x = I_UNDOMOUSE_X;
  V_icon_pos[I_UNDOMOUSE].y = I_UNDOMOUSE_Y;
  V_icon_pos[I_UNDOMOUSE].width = I_UNDOMOUSE_DX;
  V_icon_pos[I_UNDOMOUSE].height = I_UNDOMOUSE_DY;
  V_icon_pos[I_UNDOMOUSE].t = I_UNDOMOUSE_T;

  V_icon_pos[I_PLUS].x = I_PLUS_X;
  V_icon_pos[I_PLUS].y = I_PLUS_Y;
  V_icon_pos[I_PLUS].width = I_PLUS_DX;
  V_icon_pos[I_PLUS].height = I_PLUS_DY;
  V_icon_pos[I_PLUS].t = I_PLUS_T;

  V_icon_pos[I_MINUS].x = I_MINUS_X;
  V_icon_pos[I_MINUS].y = I_MINUS_Y;
  V_icon_pos[I_MINUS].width = I_MINUS_DX;
  V_icon_pos[I_MINUS].height = I_MINUS_DY;
  V_icon_pos[I_MINUS].t = I_MINUS_T;

  V_icon_pos[I_LOCK].x = I_LOCK_X;
  V_icon_pos[I_LOCK].y = I_LOCK_Y;
  V_icon_pos[I_LOCK].width = I_LOCK_DX;
  V_icon_pos[I_LOCK].height = I_LOCK_DY;
  V_icon_pos[I_LOCK].t = I_LOCK_T;

  V_icon_pos[I_FIT].x = I_FIT_X;
  V_icon_pos[I_FIT].y = I_FIT_Y;
  V_icon_pos[I_FIT].width = I_FIT_DX;
  V_icon_pos[I_FIT].height = I_FIT_DY;
  V_icon_pos[I_FIT].t = I_FIT_T;

  V_icon_pos[I_PEEK].x = I_PEEK_X;
  V_icon_pos[I_PEEK].y = I_PEEK_Y;
  V_icon_pos[I_PEEK].width = I_PEEK_DX;
  V_icon_pos[I_PEEK].height = I_PEEK_DY;
  V_icon_pos[I_PEEK].t = I_PEEK_T;

  V_icon_pos[I_UNPEEK].x = I_UNPEEK_X;
  V_icon_pos[I_UNPEEK].y = I_UNPEEK_Y;
  V_icon_pos[I_UNPEEK].width = I_UNPEEK_DX;
  V_icon_pos[I_UNPEEK].height = I_UNPEEK_DY;
  V_icon_pos[I_UNPEEK].t = I_UNPEEK_T;

  V_icon_pos[I_INSPECT].x = I_INSPECT_X;
  V_icon_pos[I_INSPECT].y = I_INSPECT_Y;
  V_icon_pos[I_INSPECT].width = I_INSPECT_DX;
  V_icon_pos[I_INSPECT].height = I_INSPECT_DY;
  V_icon_pos[I_INSPECT].t = I_INSPECT_T;

  V_icon_pos[I_LEFT].x = I_LEFT_X;
  V_icon_pos[I_LEFT].y = I_LEFT_Y;
  V_icon_pos[I_LEFT].width = I_LEFT_DX;
  V_icon_pos[I_LEFT].height = I_LEFT_DY;
  V_icon_pos[I_LEFT].t = I_LEFT_T;

  V_icon_pos[I_UP].x = I_UP_X;
  V_icon_pos[I_UP].y = I_UP_Y;
  V_icon_pos[I_UP].width = I_UP_DX;
  V_icon_pos[I_UP].height = I_UP_DY;
  V_icon_pos[I_UP].t = I_UP_T;

  V_icon_pos[I_RIGHT].x = I_RIGHT_X;
  V_icon_pos[I_RIGHT].y = I_RIGHT_Y;
  V_icon_pos[I_RIGHT].width = I_RIGHT_DX;
  V_icon_pos[I_RIGHT].height = I_RIGHT_DY;
  V_icon_pos[I_RIGHT].t = I_RIGHT_T;

  V_icon_pos[I_DOWN].x = I_DOWN_X;
  V_icon_pos[I_DOWN].y = I_DOWN_Y;
  V_icon_pos[I_DOWN].width = I_DOWN_DX;
  V_icon_pos[I_DOWN].height = I_DOWN_DY;
  V_icon_pos[I_DOWN].t = I_DOWN_T;

  V_icon_pos[I_PAN].x = I_PAN_X;
  V_icon_pos[I_PAN].y = I_PAN_Y;
  V_icon_pos[I_PAN].width = I_PAN_DX;
  V_icon_pos[I_PAN].height = I_PAN_DY;
  V_icon_pos[I_PAN].t = I_PAN_T;

  V_icon_pos[I_LOAD].x = I_LOAD_X;
  V_icon_pos[I_LOAD].y = I_LOAD_Y;
  V_icon_pos[I_LOAD].width = I_LOAD_DX;
  V_icon_pos[I_LOAD].height = I_LOAD_DY;
  V_icon_pos[I_LOAD].t = I_LOAD_T;

  V_icon_pos[I_FACT].x = I_FACT_X;
  V_icon_pos[I_FACT].y = I_FACT_Y;
  V_icon_pos[I_FACT].width = I_FACT_DX;
  V_icon_pos[I_FACT].height = I_FACT_DY;
  V_icon_pos[I_FACT].t = I_FACT_T;

  V_icon_pos[I_SETUP].x = I_SETUP_X;
  V_icon_pos[I_SETUP].y = I_SETUP_Y;
  V_icon_pos[I_SETUP].width = I_SETUP_DX;
  V_icon_pos[I_SETUP].height = I_SETUP_DY;
  V_icon_pos[I_SETUP].t = I_SETUP_T;

  V_icon_pos[I_MAP].x = I_MAP_X;
  V_icon_pos[I_MAP].y = I_MAP_Y;
  V_icon_pos[I_MAP].width = I_MAP_DX;
  V_icon_pos[I_MAP].height = I_MAP_DY;
  V_icon_pos[I_MAP].t = I_MAP_T;

  V_icon_pos[I_XY].x = I_XY_X;
  V_icon_pos[I_XY].y = I_XY_Y;
  V_icon_pos[I_XY].width = I_XY_DX;
  V_icon_pos[I_XY].height = I_XY_DY;
  V_icon_pos[I_XY].t = I_XY_T;

  V_icon_pos[I_DXDY].x = I_DXDY_X;
  V_icon_pos[I_DXDY].y = I_DXDY_Y;
  V_icon_pos[I_DXDY].width = I_DXDY_DX;
  V_icon_pos[I_DXDY].height = I_DXDY_DY;
  V_icon_pos[I_DXDY].t = I_DXDY_T;
}

void
  V_drawMenu()
{
void V_drawIcon();
void V_drawXY();
void V_drawSC();
void V_drawVol();
register int i;

  /*  centering the menu each time it appears */
  dx = (int)((dxWin/2) - (I_MAP_X + I_MAP_DX)/2);
  for (i = 0; i <= I_DXDY; i++)
    V_icon_pos[i].t += V_icon_pos[i].x += dx;
  /* displaying the icons in the menu bar */
  XFillRectangle(display, V_window, C_gcBgnd, 0, 0, dxWin, YMENU - 1);
  for (i = 0; i <= I_LOCK; i++)
    V_drawIcon(i, OFF);
  V_drawXY(OFF);
  XCopyArea(display, icon[0], V_window, C_gcIconText, 0, 0,
          B_SCALE_DX, B_SCALE_DY, B_SCALE_X + dx, B_SCALE_Y);
  V_drawVol(OFF);
  XCopyArea(display, icon[1], V_window, C_gcIconText, 0, 0,
          B_SCROLL_DX, B_SCROLL_DY, B_SCROLL_X + dx, B_SCROLL_Y);
  V_drawSC(OFF);
}

int
  V_clickedIcon(xClick, yClick)
int xClick, yClick;
{
register int i;

  for (i = 0; i <= I_XY; i++)
    if (((xClick > V_icon_pos[i].x) &&
        (xClick < V_icon_pos[i].x + V_icon_pos[i].width)) &&
        ((yClick > V_icon_pos[i].y) &&
        (yClick < V_icon_pos[i].y + V_icon_pos[i].height)))
      return (i);
  if (yClick < YMENU)
    return NOP;
  return INSCREEN;
}


void
  V_drawIcon(icon, state)
int icon, state;
{
  XFillRectangle(display,V_window, C_gcIcon, (int)V_icon_pos[icon].x,
            (int)V_icon_pos[icon].y, 
            (unsigned int)(V_icon_pos[icon].width + 1),
            (unsigned int)(V_icon_pos[icon].height + 1));
  V_surround(V_window, V_icon_pos[icon].x, V_icon_pos[icon].y,
          V_icon_pos[icon].width, V_icon_pos[icon].height, state);
  XDrawImageString(display, V_window, C_gcIconText,
              (int)(V_icon_pos[icon].t - state),
              (int)(V_icon_pos[icon].y + 12 - state),
              V_menu[icon], strlen (V_menu[icon]));
}

void
  V_drawXY(state)
int state;
{
  XFillRectangle(display, V_window, C_gcIcon, V_icon_pos[I_XY].x,
              V_icon_pos[I_XY].y, V_icon_pos[I_XY].width + 1,
              V_icon_pos[I_XY].height + 1);
  V_surround(V_window, V_icon_pos[I_XY].x, V_icon_pos[I_XY].y,
          V_icon_pos[I_XY].width, V_icon_pos[I_XY].height, state);
  XDrawImageString(display, V_window, C_gcIconText, V_icon_pos[I_XY].x + 2,
              V_icon_pos[I_XY].y + 12, V_menu[13], strlen (V_menu[13]));
  XDrawImageString(display, V_window, C_gcIconText, V_icon_pos[I_XY].x + 2,
      V_icon_pos[I_XY].y + 18 + 12, V_menu[14], strlen (V_menu[14]));
  XFillRectangle(display, V_window, C_gcIcon, V_icon_pos[I_DXDY].x,
              V_icon_pos[I_DXDY].y, V_icon_pos[I_DXDY].width + 1,
              V_icon_pos[I_DXDY].height + 1);
  V_surround(V_window, V_icon_pos[I_DXDY].x, V_icon_pos[I_DXDY].y,
          V_icon_pos[I_DXDY].width, V_icon_pos[I_DXDY].height, OFF);
  XDrawImageString(display, V_window, C_gcIconText, V_icon_pos[I_DXDY].x + 2,
              V_icon_pos[I_DXDY].y + 12,
              V_menu[15], strlen (V_menu[15]));
  XDrawImageString(display, V_window, C_gcIconText, V_icon_pos[I_DXDY].x + 2,
              V_icon_pos[I_DXDY].y + 18 + 12,
              V_menu[16], strlen (V_menu[16]));
}

void
  V_drawSC(state)
int state;
{
  V_surround(V_window, B_SCROLL_X + dx, B_SCROLL_Y, B_SCROLL_DX,
          B_SCROLL_DY, state);
}

void
  V_drawVol(state)
int state;
{
  V_surround(V_window, B_SCALE_X + dx-28, B_SCALE_Y, B_SCALE_DX+34,
          B_SCALE_DY, state);
}

/* to be used by all other icon drawer */
void
  V_surround(window, x, y, dx, dy, state)
Window window;
short x, y;
unsigned short dx, dy;
int state;
{
extern GC C_gcV1, C_gcV2;
GC *g1, *g2;

  if (state == OFF) {
    g1 = &C_gcV1;
    g2 = &C_gcV2;
  } else {
    g1 = &C_gcV2;
    g2 = &C_gcV1;
  }
  XDrawLine(display, window, *g1, (int)x, (int)y, (int)(x + dx), (int)y);
  XDrawLine(display, window, *g1, (int)(x + 1), (int)(y + 1), (int)(x + dx),
          (int)(y + 1));
  XDrawLine(display, window, *g1, (int)x, (int)y, (int)x, (int)(y + dy));
  XDrawLine(display, window, *g1, (int)(x + 1), (int)(y + 1), (int)(x + 1),
          (int)(y + dy));
  XDrawLine(display, window, *g2, (int)(x + dx), (int)(y + 1), (int)(x + dx),
          (int)(y + dy));
  XDrawLine(display, window, *g2, (int)(x + dx - 1), (int)(y + 2),
          (int)(x + dx - 1), (int)(y + dy));
  XDrawLine(display, window, *g2, (int)(x + 1), (int)(y + dy), (int)(x + dx),
          (int)y + dy);
  XDrawLine(display, window, *g2, (int)(x + 2), (int)(y + dy - 1),
          (int)(x + dx), (int)(y + dy - 1));
}

/* highlite of the bitmaped icons */
void
  V_coverIcon(i)
int i;
{
  XSetFunction(display, C_gcIconText, GXxor);
  XSetForeground(display, C_gcIconText, V_colors[COLOR_RED]);
  XFillRectangle(display, V_window, C_gcIconText, (int)V_icon_pos[i].x,
              (int)V_icon_pos[i].y, (unsigned int)V_icon_pos[i].width,
              (unsigned int)V_icon_pos[i].height);
  XSetForeground(display, C_gcIconText, V_colors[COLOR_ICON_TEXT]);
  XSetFunction(display, C_gcIconText, GXcopy);
}

void
  V_changeName(s)
char *s;
{
char str[25];
  sprintf(str, "View : %s", s);
  XStoreName(display, V_window, str);
}
