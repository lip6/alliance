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

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   GenView                         |
|                                                             |
| File    :                   V_area.c                        |
|                                                             |
| Authors :                Frederic Petrot                    |
|                    Modified by Ludovic Jacomme              |
|                                                             |
| Date    :                   12/05/94                        |
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
# include "v_menu.h"
# include "v_extern.h"

# include "C_utils.h"
# include "V_drv.h"
# include "V_area.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/


void V_refresh()
{
  GC gc = V_gcCao[GC_FGND];
  char mode = run_mode;

  run_mode = STEP;

  /* clearing the window and it's associated pixmap */

  V_changeCursor(0);
  XFlush(display);
  XClearArea(display, V_window, 0, YMENU, dxWin, dyWin, False);
  XSetFillStyle(display, gc, FillSolid);
  XSetForeground(display, gc, V_colors[COLOR_BGND]);
  XFillRectangle(display, V_PixSave, gc, 0, YMENU, dxWin, dyWin);
  XSetForeground(display, gc, V_colors[COLOR_FGND]);
  if (V_lock == 1)
  {
    if ( Peek( xscreen - 4 , yscreen - 4,
               dxscreen + 8, dyscreen + 8 ) == 1 ) 
    {
      V_Peek_it( ON, 0, YMENU, dxWin, dyWin );
      Unpeek();
    }
  }

  if ( !C_atomicValue || !V_lock )
  {
    Put_it(V_window, xscreen, yscreen, dxscreen, dyscreen);
  }

  V_changeCursor(1);
  run_mode = mode;
}

void V_refreshArea( x, y, dx, dy )

  int x, y, dx, dy;
{
  GC gc = V_gcCao[GC_FGND];
  char mode = run_mode;

  V_changeCursor(0);
  XFlush(display); /* do not wait for the buffer to be filled */
  XSetFillStyle(display, gc, FillSolid);
  XSetForeground(display, gc, V_colors[COLOR_BGND] );
  XFillRectangle(display, V_PixSave, gc, 0, 0, dxWin, dyWin + YMENU);
  XSetForeground(display, gc, V_colors[COLOR_FGND]);
  x -= GENVIEW_SOME_MORE_STUFF;
  y -= GENVIEW_SOME_MORE_STUFF;
  dx += 2 * GENVIEW_SOME_MORE_STUFF;
  dy += 2 * GENVIEW_SOME_MORE_STUFF;

  if (V_lock == 1)
  {
    if ( Peek(xscreen +(long)(x / scale) - 1,
        yscreen + dyscreen +(long)(YMENU / scale) -
        (long)((y + dy) / scale) - 1,
        (long)(dx / scale) + 1,(long)(dy / scale) + 1) == 1) 
    {
      V_Peek_it(OFF, x, y, dx, dy); /* do not transfer yet */
      Unpeek();
    }
  }

  if (!C_atomicValue)
  {
    Put_it(V_PixSave, xscreen +(long)(x / scale) - 1,
        yscreen + dyscreen + (long)(YMENU / scale) 
        - (long)((y + dy) / scale) - 1,
        (long)(dx / scale) + 1,(long)(dy / scale) + 1);
  }

  x  += GENVIEW_SOME_MORE_STUFF;
  y  += GENVIEW_SOME_MORE_STUFF;
  dx -= 2 * GENVIEW_SOME_MORE_STUFF;
  dy -= 2 * GENVIEW_SOME_MORE_STUFF;

  XCopyArea(display, V_PixSave, V_window, gc, x, y, dx, dy, x, y);

  V_drawAB();
  V_drawPointer(0, 0);
  V_changeCursor(1);

  run_mode = mode;
}

void V_saveWindow()
{
  GC gc = V_gcCao[GC_FGND];

  XCopyArea(display, V_window, V_PixSave, gc, 0, 0,
          dxWin, dyWin + YMENU, 0, 0);
}

void V_restoreWindow()
{
  GC gc = V_gcCao[GC_FGND];

  XCopyArea(display, V_PixSave, V_window, gc, 0, 0,
          dxWin, dyWin + YMENU, 0, 0);
}
