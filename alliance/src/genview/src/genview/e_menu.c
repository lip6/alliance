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
# include <stdlib.h>
# include MUT_H
# include MPH_H
# include RDS_H
# include RWI_H
# include RUT_H
# include RPR_H
# include RFM_H

# include "G_global.h"

# include "v_view.h"
# include "v_extern.h"
# include "e_menu.h"

int E_IconFlag;
int E_wb_x;
static YRectangle *E_icon_pos;
int E_dx_win, E_dy_win;
static char *E_menu[] = {
  "SET/UNSET BP",
  "CLEAR ALL BPS",
  "SET TRACE",
  "UNSET TRACE",
  "RUN",
  "STEP",
  "LOAD",
  "SAVE",
  "EXIT",
  "COMPILE",
  "NEXT",
  "PARAMS",
  "SHELL",
  "START"
};



/* menu bar initialisation */
void E_setMenu()
{
static int first;

  if(first == 0) {
    E_icon_pos = (YRectangle *)mbkalloc(14 * sizeof (YRectangle));
    first ++;
  }
  E_icon_pos[E_S_BKPT].x = E_S_BKPT_X;
  E_icon_pos[E_S_BKPT].y = E_S_BKPT_Y;
  E_icon_pos[E_S_BKPT].width = E_S_BKPT_DX;
  E_icon_pos[E_S_BKPT].height = E_S_BKPT_DY;
  E_icon_pos[E_S_BKPT].t = E_S_BKPT_T;

  E_icon_pos[E_C_BKPT].x = E_C_BKPT_X;
  E_icon_pos[E_C_BKPT].y = E_C_BKPT_Y;
  E_icon_pos[E_C_BKPT].width = E_C_BKPT_DX;
  E_icon_pos[E_C_BKPT].height = E_C_BKPT_DY;
  E_icon_pos[E_C_BKPT].t = E_C_BKPT_T;

  E_icon_pos[E_SETTRACE].x = E_SETTRACE_X;
  E_icon_pos[E_SETTRACE].y = E_SETTRACE_Y;
  E_icon_pos[E_SETTRACE].width = E_SETTRACE_DX;
  E_icon_pos[E_SETTRACE].height = E_SETTRACE_DY;
  E_icon_pos[E_SETTRACE].t = E_SETTRACE_T;

  E_icon_pos[E_UNSTRACE].x = E_UNSTRACE_X;
  E_icon_pos[E_UNSTRACE].y = E_UNSTRACE_Y;
  E_icon_pos[E_UNSTRACE].width = E_UNSTRACE_DX;
  E_icon_pos[E_UNSTRACE].height = E_UNSTRACE_DY;
  E_icon_pos[E_UNSTRACE].t = E_UNSTRACE_T;

  E_icon_pos[E_RUN].x = E_RUN_X;
  E_icon_pos[E_RUN].y = E_RUN_Y;
  E_icon_pos[E_RUN].width = E_RUN_DX;
  E_icon_pos[E_RUN].height = E_RUN_DY;
  E_icon_pos[E_RUN].t = E_RUN_T;

  E_icon_pos[E_STEP].x = E_STEP_X;
  E_icon_pos[E_STEP].y = E_STEP_Y;
  E_icon_pos[E_STEP].width = E_STEP_DX;
  E_icon_pos[E_STEP].height = E_STEP_DY;
  E_icon_pos[E_STEP].t = E_STEP_T;

  E_icon_pos[E_LOAD].x = E_LOAD_X;
  E_icon_pos[E_LOAD].y = E_LOAD_Y;
  E_icon_pos[E_LOAD].width = E_LOAD_DX;
  E_icon_pos[E_LOAD].height = E_LOAD_DY;
  E_icon_pos[E_LOAD].t = E_LOAD_T;

  E_icon_pos[E_SAVE].x = E_SAVE_X;
  E_icon_pos[E_SAVE].y = E_SAVE_Y;
  E_icon_pos[E_SAVE].width = E_SAVE_DX;
  E_icon_pos[E_SAVE].height = E_SAVE_DY;
  E_icon_pos[E_SAVE].t = E_SAVE_T;

  E_icon_pos[E_EXIT].x = E_EXIT_X;
  E_icon_pos[E_EXIT].y = E_EXIT_Y;
  E_icon_pos[E_EXIT].width = E_EXIT_DX;
  E_icon_pos[E_EXIT].height = E_EXIT_DY;
  E_icon_pos[E_EXIT].t = E_EXIT_T;

  E_icon_pos[E_COMPILE].x = E_COMPILE_X;
  E_icon_pos[E_COMPILE].y = E_COMPILE_Y;
  E_icon_pos[E_COMPILE].width = E_COMPILE_DX;
  E_icon_pos[E_COMPILE].height = E_COMPILE_DY;
  E_icon_pos[E_COMPILE].t = E_COMPILE_T;

  E_icon_pos[E_NEXT].x = E_NEXT_X;
  E_icon_pos[E_NEXT].y = E_NEXT_Y;
  E_icon_pos[E_NEXT].width = E_NEXT_DX;
  E_icon_pos[E_NEXT].height = E_NEXT_DY;
  E_icon_pos[E_NEXT].t = E_NEXT_T;

  E_icon_pos[E_PARAM].x = E_PARAM_X;
  E_icon_pos[E_PARAM].y = E_PARAM_Y;
  E_icon_pos[E_PARAM].width = E_PARAM_DX;
  E_icon_pos[E_PARAM].height = E_PARAM_DY;
  E_icon_pos[E_PARAM].t = E_PARAM_T;

  E_icon_pos[E_SHELL].x = E_SHELL_X;
  E_icon_pos[E_SHELL].y = E_SHELL_Y;
  E_icon_pos[E_SHELL].width = E_SHELL_DX;
  E_icon_pos[E_SHELL].height = E_SHELL_DY;
  E_icon_pos[E_SHELL].t = E_SHELL_T;

  E_icon_pos[E_START].x = E_START_X;
  E_icon_pos[E_START].y = E_START_Y;
  E_icon_pos[E_START].width = E_START_DX;
  E_icon_pos[E_START].height = E_START_DY;
  E_icon_pos[E_START].t = E_START_T;
}

/*  drawing the menu */
void E_drawMenu ()
{
register int i;
Window w;
int fgm;

  XGetGeometry(display, E_window, &w, &fgm, &fgm, 
            &E_dx_win, &E_dy_win, &fgm, &fgm);
  fgm = (int)((E_dx_win/2) - (E_EXIT_X + E_EXIT_DX)/2);
  E_wb_x = fgm;
  for(i=0; i <= E_START; i++) {
    E_icon_pos[i].t += E_icon_pos[i].x += fgm;
    E_drawIcon(i, OFF);
  }
}

/*  draws an icon in normal or highlit mode */
E_drawIcon (icon, state)
int icon;
int state;
{
  XFillRectangle(display, E_window, C_gcIcon, (int)E_icon_pos[icon].x,
              (int)E_icon_pos[icon].y,
              (unsigned int)E_icon_pos[icon].width, 
              (unsigned int)E_icon_pos[icon].height);
  V_surround(E_window, E_icon_pos[icon].x, E_icon_pos[icon].y,
          E_icon_pos[icon].width, E_icon_pos[icon].height, state);
  XDrawImageString(display, E_window, C_gcIconText, 
              (int)(E_icon_pos[icon].t - state),
              (int)(E_icon_pos[icon].y + E_DY_TXT - state), 
              E_menu[icon], strlen(E_menu[icon]));
  XFlush(display); /* to see it as soon as possible */
}

int E_clickedIcon(xClick, yClick)
int xClick, yClick;
{
register int i;

  for (i = 0; i <= E_START; i++)
    if (((xClick > E_icon_pos[i].x) &&
        (xClick < E_icon_pos[i].x + E_icon_pos[i].width)) &&
        ((yClick > E_icon_pos[i].y) &&
        (yClick < E_icon_pos[i].y + E_icon_pos[i].height)))
      return i;
  return E_NO_ICON;
}
