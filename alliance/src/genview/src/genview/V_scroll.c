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
| File    :                  V_scroll.c                       |
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

# include "v_menu.h"
# include "v_view.h"
# include "v_extern.h"
# include "i_lines.h"
# include "m_mes_id.h"

# include "C_utils.h"
# include "V_drv.h"
# include "V_scroll.h"

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

int V_scrollH(lambda)

  long lambda;
{
  int message;

  if (lambda > 0)
    message = V_scrollRL((long)(lambda));
  else if (lambda < 0)
    message = V_scrollLR((long)(-lambda));
  V_drawInsRef();
  V_drawAB();
  V_saveWindow();

  return message;
}

int V_scrollV(lambda)

  long lambda;
{
  int message;

  if (lambda > 0)
    message = V_scrollLH((long)(lambda));
  else if (lambda < 0)
    message = V_scrollHL((long)(-lambda));
  V_drawInsRef();
  V_drawAB();
  V_saveWindow();
  return message;
}

/* The following functions actually performs the scrolling */
/* horizontal scroll  right -> left */

int V_scrollRL(nPixels)

  int nPixels;
{
  GC gc;
  long nLambda;

  /* moving the area the way it should */
  if (!(nLambda = (long)(nPixels / scale)))
      return M_id = M_SCROLL;
  nPixels = nLambda * scale;
  gc = V_gcCao[GC_FGND];
  XCopyArea(display, V_window, V_window, gc, 0, YMENU,
      dxWin - nPixels, dyWin, nPixels, YMENU);
  /* clearing the leftover after moving */
  XSetForeground(display, gc, V_colors[COLOR_BGND]);
  XFillRectangle(display, V_window, gc, 0, YMENU, nPixels, dyWin);
  XFillRectangle(display, V_PixSave, gc, 0, YMENU, nPixels, dyWin);
  XSetForeground(display, gc, V_colors[COLOR_FGND]);

  scroll(WEST, nLambda);
  
  if ( V_lock == 1)
  {
    if ( Peek( xscreen - 4 , yscreen - 4,
               nLambda + 8, dyscreen + 8 ) == 1 ) 
    {
      V_Peek_it( OFF, 0, YMENU, nPixels, dyWin );
      Unpeek();
    }
  }

  if (!C_atomicValue || !V_lock)
  {
    Put_it(V_PixSave, xscreen - 4, yscreen - 4, nLambda + 8, dyscreen + 8);
  }

  XCopyArea(display, V_PixSave, V_window, gc,
          0, YMENU, nPixels, dyWin, 0, YMENU);

  return M_NONE;
}

/* horizontal V_scroll left -> right */
int V_scrollLR(nPixels)

  int nPixels;
{
  GC gc;
  long nLambda;

  /* moving the area the way it should */
  if (!(nLambda =(long)(nPixels / scale)))
    return M_id = M_SCROLL;
  nPixels = nLambda * scale;
  gc = V_gcCao[GC_FGND];
  XCopyArea(display, V_window, V_window, gc, nPixels, YMENU,
      dxWin - nPixels, dyWin, 0, YMENU);
  XSetForeground(display, gc, V_colors[COLOR_BGND]);
  XFillRectangle(display, V_window, gc, dxWin - nPixels, YMENU,
      nPixels, dyWin);
  /* clearing part of the pixmap */
  XFillRectangle(display, V_PixSave, gc, dxWin - nPixels, YMENU,
      nPixels, dyWin);
  XSetForeground(display, gc, V_colors[COLOR_FGND]);

  scroll(EAST,(long)(nPixels / scale));
  if (V_lock == 1)
  {
    if (Peek(xscreen + dxscreen -(long)(nPixels / scale) - 4, yscreen - 4,
      (long)(nPixels / scale) + 8, dyscreen + 8) == 1) 
    {
      V_Peek_it(OFF, dxWin - nPixels - 2, YMENU, nPixels + 4, dyWin);
      Unpeek();
    }
  }

  if (!C_atomicValue || !V_lock)
  {
    Put_it(V_PixSave, xscreen + dxscreen -(long)(nPixels / scale) - 4,
          yscreen - 4,(long)(nPixels / scale)+8, dyscreen+8);
  }
  XCopyArea(display, V_PixSave, V_window, gc, dxWin - nPixels, YMENU,
      nPixels, dyWin, dxWin - nPixels, YMENU);
  return M_NONE;
}

/* vertical V_scroll bottom to top */
int V_scrollLH(nPixels)

  int nPixels;
{
  GC gc;
  long nLambda;

  /* moving the area the way it should */
  if (!(nLambda =(long)(nPixels / scale)))
      return M_id = M_SCROLL;
  nPixels = nLambda * scale;
  gc = V_gcCao[GC_FGND];
  XCopyArea(display, V_window, V_window, gc, 0, nPixels + YMENU,
      dxWin, dyWin - nPixels, 0, YMENU);
  scroll(SOUTH,(long)(nPixels / scale));
  XSetForeground(display, gc, V_colors[COLOR_BGND]);
  XFillRectangle(display, V_window, gc, 0, dyWin + YMENU - nPixels,
      dxWin, nPixels);
  /* clearing part of the pixmap */
  XFillRectangle(display, V_PixSave, gc, 0, dyWin + YMENU - nPixels,
      dxWin, nPixels);
  XSetForeground(display, gc, V_colors[COLOR_FGND]);
  if (V_lock == 1)
  {
    if (Peek(xscreen - 4, yscreen - 4, dxscreen + 8,
        (long)(nPixels / scale) +8) == 1) 
    {
      V_Peek_it(OFF, 0, dyWin + YMENU - nPixels - 2, dxWin, nPixels + 4);
      Unpeek();
    }
  }
  if (!C_atomicValue || !V_lock)
  {
    Put_it(V_PixSave, xscreen - 4, yscreen - 4, dxscreen + 8,
          (long)(nPixels / scale) + 8);
  }
  XCopyArea(display, V_PixSave, V_window, gc, 0, dyWin + YMENU - nPixels,
          dxWin, nPixels + 1, 0, dyWin + YMENU - nPixels);
  return M_NONE;
}

/* vertical V_scroll top to bottom */
int V_scrollHL(nPixels)

  int nPixels;
{
  GC gc;
  long nLambda;

  /* moving the area the way it should */
  if (!(nLambda =(long)(nPixels / scale)))
    return M_id = M_SCROLL;
  nPixels = nLambda * scale;
  gc = V_gcCao[GC_FGND];
  XCopyArea(display, V_window, V_window, gc, 0, YMENU,
      dxWin, dyWin - nPixels, 0, nPixels + YMENU);
  scroll(NORTH,(long)(nPixels / scale));
  XSetForeground(display, gc, V_colors[COLOR_BGND]);
  XFillRectangle(display, V_window, gc, 0, YMENU,
      dxWin, nPixels);
  /* clearing part of the pixmap */
  XFillRectangle(display, V_PixSave, gc, 0, YMENU,
      dxWin, nPixels);
  XSetForeground(display, gc, V_colors[COLOR_FGND]);
  if (V_lock == 1)
  {
    if (Peek(xscreen - 4, yscreen + dyscreen - (long)(nPixels / scale) - 4,
      dxscreen + 8,(long)(nPixels / scale) + 8) == 1) 
    {
      V_Peek_it(OFF, 0, YMENU, dxWin, nPixels);
      Unpeek();
    }
  }
  if (!C_atomicValue || !V_lock)
  {
    Put_it(V_PixSave, xscreen - 4,
          yscreen + dyscreen - (long)(nPixels / scale) - 4,
          dxscreen + 8,(long)(nPixels / scale) + 8);
  }
  XCopyArea(display, V_PixSave, V_window, gc, 0, YMENU, dxWin, nPixels,
          0, YMENU);
  return M_NONE;
}
