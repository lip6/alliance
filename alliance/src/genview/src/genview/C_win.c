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
| File    :                   C_win.c                         |
|                                                             |
| Authors :                Frederic Petrot                    |
|                    Modified by Ludovic Jacomme              |
|                                                             |
| Date    :                   30.04.94                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Include File                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include MUT_H
# include MPH_H
# include RDS_H
# include RWI_H
# include RUT_H
# include RPR_H
# include RFM_H
# include "G_global.h"

# include "v_view.h"
# include "c_menu.h"
# include "v_extern.h"

# include "V_drv.h"
# include "C_win.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  static char *title = "Set up";

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

void C_open()
{
XSizeHints hint;

  hint.x = 80;
  hint.y = 80;
  hint.width = C_W_WIDTH;
  hint.height = C_W_HEIGHT;
  hint.min_width = C_W_WIDTH;
  hint.min_height = C_W_HEIGHT;
  hint.max_width = C_W_WIDTH;
  hint.max_height = C_W_HEIGHT;
  hint.flags = PSize | PPosition | PMinSize | PMaxSize;
  C_window = XCreateSimpleWindow(display, DefaultRootWindow (display),
                  hint.x, hint.y, hint.width, hint.height,
                  5, V_colors[COLOR_FGND], V_colors[COLOR_BGND]);
  XSelectInput(display, C_window, KeyPressMask | ButtonReleaseMask
            | ButtonPressMask | ExposureMask);
  XSetStandardProperties(display, C_window, title,
                  title, None, NULL, 0, &hint);
  XMapRaised(display, C_window);
  *C_NewScrollValue = '\0';
  C_IconFlag = C_NO_ICON;
  C_ReadConf();
  C_setMenu();
}

int C_event()
{
KeySym key;
static int witchIcon;
static int x, y;
char buf;
XWindowAttributes WAttr;

  switch (V_event.type) {
    case Expose:
      C_setMenu();
      C_drawMenu();
      break;
    case ButtonPress:

      XGetWindowAttributes( display, C_window, &WAttr );

      if ( WAttr.map_state == IsViewable )
      {
        XSetInputFocus( display, C_window, RevertToNone, CurrentTime);
      }

      witchIcon =
          C_clickedIcon(x = V_event.xbutton.x, y = V_event.xbutton.y);
      if (witchIcon != C_NO_ICON)
        C_drawIcon(witchIcon, ON);
      break;
    case ButtonRelease:
      if (C_actionMouse(witchIcon, x, y) == C_EX) {
        XDestroyWindow (display, C_window);
        return witchIcon == C_CA ? CLOSE_WINDOW | REFRESH_AREA 
                          : CLOSE_WINDOW | REFRESH_ALL;
      }
      break;
    case KeyPress:
      XLookupString (&V_event.xkey, &buf, 1, &key, 0);
      if (C_IconFlag == C_SC)
        C_ActionKey (key);
  }
  return NO_WINDOW_ACTION;
}

int C_actionMouse (icon, x, y)
int icon;
{
register int i, j;

  switch(icon) {
    case C_SC:
      if (C_IconFlag == C_NO_ICON) {
        C_IconFlag = C_SC;
        C_askScroll();
        C_drawIcon(C_SC, ON);
        return 0;
      }
      if (C_IconFlag == C_SC) {
        C_IconFlag = C_NO_ICON;
        *C_NewScrollValue = '\0';
        C_newScroll();
        C_drawIcon(C_SC, OFF);
        return 0;
      }
    case C_SR:
      C_WriteConf();
      V_refresh();
      C_drawIcon(C_SR, OFF);
      break;
    case C_RR:
      C_ReadConf();
      C_drawIcon(C_RR, OFF);
      break;
    case C_EX:
      C_WriteConf();
      C_drawIcon(C_EX, OFF);
      return C_EX;
    case C_CA:
      C_drawIcon(C_CA, OFF);
      return C_EX;
    case C_SD:
      C_WriteConf();
      C_WriteDisk();
      V_refresh();
      C_drawIcon(C_SD, OFF);
      break;
    case C_RD:
      C_ReadDisk();
      C_ReadConf();
      C_drawIcon(C_RD, OFF);
      break;
    case C_GEOM:
      C_atomic();
      C_drawIcon(C_GEOM, OFF);
      break;
    case C_VI:
      C_View();
      C_drawIcon(C_VI, OFF);
      break;
    case C_PE:
      C_Peek();
      C_drawIcon(C_PE, OFF);
      break;
    case C_AV:
      C_AllVisible ();
      C_drawIcon(C_AV, OFF);
      break;
    case C_AI:
      C_AllInvisible ();
      C_drawIcon(C_AI, OFF);
      break;
    default:
      for (i = 0, j = C_Y_Switch; i < C_NB_OBJECT; i++, j += C_DY_Switch)
        if ((x > C_Xrect) && (x < C_Xrect + C_DXrect) 
            && (y > j) && (y < j + C_DYrect)) 
      {
        C_ObjectVisibility[i] ^= 1;
        SwitchRefresh (i);
        break;
      }
  }
  return 0;
}
