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
# include "t_menu.h"
# include "v_menu.h"

extern Display *display;
extern Window T_window;
extern XEvent V_event;

int T_event ()
{
int x, y;
static int witchIcon;

  switch (V_event.type) {
    case Expose:
      T_setMenu();
      T_drawMenu();
      T_refresh();
      break;
    case ButtonPress:
      x = V_event.xbutton.x;
      y = V_event.xbutton.y;
      witchIcon = T_clickedIcon (x, y);
      switch (witchIcon) {
        case TR_PGUP: 
          T_drawIcon(TR_PGUP, ON); 
          break;
        case TR_PGDN: 
          T_drawIcon(TR_PGDN, ON); 
          break;
        case TR_LNUP: 
          T_drawIcon(TR_LNUP, ON); 
          break;
        case TR_LNDN: 
          T_drawIcon(TR_LNDN, ON); 
          break;
        case TR_EXIT: 
          T_drawIcon(TR_EXIT, ON); 
          break;
      }
      break;
    case ButtonRelease:
      switch (witchIcon) {
        case TR_PGUP: 
          T_pageUp (); 
          T_drawIcon(TR_PGUP, OFF); 
          break;
        case TR_PGDN: 
          T_pageDn (); 
          T_drawIcon(TR_PGDN, OFF); 
          break;
        case TR_LNUP: 
          T_lineUp (); 
          T_drawIcon(TR_LNUP, OFF); 
          break;
        case TR_LNDN: 
          T_lineDn (); 
          T_drawIcon(TR_LNDN, OFF); 
          break;
        case TR_EXIT: 
          T_initText(); 
          T_drawIcon(TR_EXIT, OFF); 
          break;
      }
  }
  return NO_WINDOW_ACTION;
}
