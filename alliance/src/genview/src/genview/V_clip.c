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
| File    :                   V_clip.c                        |
|                                                             |
| Authors :                Frederic Petrot                    |
|                    Modified by Ludovic Jacomme              |
|                                                             |
| Date    :                   05.03.94                        |
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
# include "V_clip.h"

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

/* quite subtle :
   this must be a mutiple of the pixmap size for proper X11r4 displaying!
   if the beginning of a rectangle is not on a X11 ``grid step'',
   small garbage appears on the screen. */

/*
* returns 0 for no display
*/

int Clip_to_window( x, y, dx, dy, xscreen, yscreen, dxscreen, dyscreen)

    long *x, *y, *dx, *dy;
    long xscreen, yscreen, dxscreen, dyscreen;
{
  long x1, y1, x2, y2, xs1, ys1, xs2, ys2; /* cartesian coordinates */

  x1 = *x;
  y1 = *y;
  x2 = x1 + *dx;
  y2 = y1 + *dy;
  xs1 = xscreen;
  ys1 = yscreen;
  xs2 = xscreen + dxscreen;
  ys2 = yscreen + dyscreen;

  /* not points inside the window */
  if ((xs1 > x2) || (x1 > xs2) || (ys1 > y2) || (y1 > ys2))
    return 0;
  /* if it at least one point belongs to the window, let's troncate */
  if ((x1 >= xs1 && x1 <= xs2 && y1 >= ys1 && y1 <= ys2)
      || (x2 >= xs1 && x2 <= xs2 && y1 >= ys1 && y1 <= ys2)
      || (x1 >= xs1 && x1 <= xs2 && y2 >= ys1 && y2 <= ys2)
      || (x2 >= xs1 && x2 <= xs2 && y2 >= ys1 && y2 <= ys2)) {
    if (y1 < ys1)
      y1 = ys1 - V_CLIP_OVERSIZE;
    if (y2 > ys2)
      y2 = ys2 + V_CLIP_OVERSIZE;
    if (x1 < xs1)
      x1 = xs1 - V_CLIP_OVERSIZE;
    if (x2 > xs2)
      x2 = xs2 + V_CLIP_OVERSIZE;
    *x = x1;
    *dx = x2 - x1;
    *y = y2;
    *dy = y2 - y1;
    return 1; /* as soon as possible */
  }
  /* if it crosses the window, let's troncate */
  if (((x1 < xs1 && x2 > xs2)
        && ((y1 >= ys1 && y1 <= ys2) || (y2 >= ys1 && y2 <= ys2)))
      || ((y1 < ys1 && y2 > ys2)
        && ((x1 >= xs1 && x1 <= xs2) || (x2 >= xs1 && x2 <= xs2)))) {
    if (y1 < ys1)
      y1 = ys1 - V_CLIP_OVERSIZE;
    if (y2 > ys2)
      y2 = ys2 + V_CLIP_OVERSIZE;
    if (x1 < xs1)
      x1 = xs1 - V_CLIP_OVERSIZE;
    if (x2 > xs2)
      x2 = xs2 + V_CLIP_OVERSIZE;
    *x = x1;
    *dx = x2 - x1;
    *y = y2;
    *dy = y2 - y1;
    return 1;
  }
  /* if it surrounds, much less often, the window, let's troncate too */
  if (x1 < xs1 && x2 > xs2 && y1 < ys1 && y2 > ys2) {
    y1 = ys1 - V_CLIP_OVERSIZE;
    y2 = ys2 + V_CLIP_OVERSIZE;
    x1 = xs1 - V_CLIP_OVERSIZE;
    x2 = xs2 + V_CLIP_OVERSIZE;
    *x = x1;
    *dx = x2 - x1;
    *y = y2;
    *dy = y2 - y1;
    return 1;
  }
  return 0;
}
