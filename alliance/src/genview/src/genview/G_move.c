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
| File    :                   G_move.c                        |
|                                                             |
| Authors :                Frederic Petrot                    |
|                    Modified by Ludovic Jacomme              |
|                                                             |
| Date    :                   05.02.94                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Include File                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <math.h>
# include MUT_H
# include MPH_H
# include RDS_H
# include RWI_H
# include RUT_H
# include RPR_H
# include RFM_H
# include "G_global.h"
# include "G_move.h"

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

  /* fit : makes the whole circuit fit in the screen. */

void fit()
{
    float scale1, scale2, scale3, scale4;
    long  dx, dy;
    char  buffer[200];

    /* Returns when no circuit is loaded */

    if (xvisufig == (phfig_list *)NULL) 
    {
      scale    = 1;
      dxscreen = dxWin / scale;
      dyscreen = dyWin / scale;
      xscreen  = xvisu + (dxvisu - dxscreen) / 2;
      yscreen  = yvisu + (dyvisu - dyscreen) / 2;

      return;
    }

    /* the origin of the screen becomes the origin of the bounding box */
    /* Two values of scale are calculated to take the min and fit correctly */

    scale1 = ((float)dxWin / dxvisu);
    scale2 = ((float)dyWin / dyvisu);
    scale3 = ((float)dxWin / (xab2fig - xabfig));
    scale4 = ((float)dyWin / (yab2fig - yabfig));

    /* The two values of scale are casted to avoid side effects on drawing */

    if (scale1 > 1) scale1 = (float)((long)scale1);
    if (scale2 > 1) scale2 = (float)((long)scale2);
    if (scale3 > 1) scale3 = (float)((long)scale3);
    if (scale4 > 1) scale4 = (float)((long)scale4);

    /* The lowest value is choosen */

    scale = G_MIN(scale1, G_MIN(scale2, G_MIN(scale3, scale4)));

    /* New window dimension */

    dxscreen = dxWin / scale;
    dyscreen = dyWin / scale;

    if (scale == scale1 || scale == scale2) 
    {
      xscreen = xvisu + (dxvisu - dxscreen) / 2;
      yscreen = yvisu + (dyvisu - dyscreen) / 2;
    }
    else 
    {
      xscreen = xabfig + (xab2fig - xabfig - dxscreen) / 2;
      yscreen = yabfig + (yab2fig - yabfig - dyscreen) / 2;
    }
}


/* pan : move the window to another point. */

void pan(deltax, deltay)

  long deltax, deltay;
{
  xscreen += deltax;
  yscreen += deltay;
}

/* scroll : move the window in case of scrolling operation. */

void scroll(direction, move_size)

  char direction;
  long move_size;
{
  switch (direction) {
    case NORTH:
      yscreen += move_size;
      break;
    case EAST:
      xscreen += move_size;
      break;
    case SOUTH:
      yscreen -= move_size;
      break;
    case WEST:
      xscreen -= move_size;
      break;
  }
}

/* zoom : decrease the visualised part of the circuit. */

int zoom(fact)

  long fact;
{
  /* Backup values for a mooz event */
  zoomTrack.x = xscreen;
  zoomTrack.y = yscreen;
  zoomTrack.dx = dxscreen;
  zoomTrack.dy = dyscreen;
  zoomTrack.scale = scale;

  /* Calculate the new scale value */
  dxscreen -= fact * dxscreen / 20;
  scale = (float)dxWin / dxscreen;

  /* Cast to long avoids side effects for drawing */
  if (scale > 1)
    scale = (float)((long)scale);

  /* If no variation of scale (due to the cast) then it's increased */
  if (scale == zoomTrack.scale)
    scale++;

  /* If scale is too high then recovers old value and exit */
  if (scale > MAX_ZOOM) {
    scale = zoomTrack.scale;
    dxscreen = zoomTrack.dx;
    return 0;
  }

  /* Calculate the new window dimension */
  xscreen += zoomTrack.dx / 2;
  yscreen += dyscreen / 2;
  dyscreen = dyWin / scale;
  dxscreen = dxWin / scale;
  xscreen -= dxscreen / 2;
  yscreen -= dyscreen / 2;

  return 1;
}

/* loom : increase the visualised part of the circuit.
          externally called mooz now, because it's funnier! */
int loom(fact)

  long fact;
{
  /* Backup values for an out event */
  zoomTrack.x = xscreen;
  zoomTrack.y = yscreen;
  zoomTrack.dx = dxscreen;
  zoomTrack.dy = dyscreen;
  zoomTrack.scale = scale;

  /* Calculate the new scale value */
  dxscreen += fact * dxscreen / 20;
  scale = (float)dxWin / dxscreen;

  /* Cast to long avoids side effects */
  if (scale > 1)
    scale = (float)((long)scale);

  /* If scale is too low then recovers old value and exit */
  if (scale < MIN_ZOOM) {
    scale = zoomTrack.scale;
    dxscreen = zoomTrack.dx;
    return 0;
  }

  /* Calculates the new window dimension */
  xscreen += zoomTrack.dx / 2;
  yscreen += dyscreen / 2;
  dyscreen = dyWin / scale;
  dxscreen = dxWin / scale;
  xscreen -= dxscreen / 2;
  yscreen -= dyscreen / 2;

  return 1;
}

/* resize : calculates the new dimension of window when resize event. */

void resize()
{
  /* New window dimension */
  dxscreen = dxWin / scale;
  dyscreen = dyWin / scale;
}

/* zoom_mouse : zoom upon a part of circuit delimited with the mouse. */

int zoom_mouse(x, y, dx, dy)

  long x, y, dx, dy;
{
float old_scale, scale2;

  /* Backup values for a loom_mouse event */
  zoomTrack.x = xscreen;
  zoomTrack.y = yscreen;
  zoomTrack.dx = dxscreen;
  zoomTrack.dy = dyscreen;
  zoomTrack.scale = scale;

  old_scale = scale;

  /* Two values of scale are calculated to take the min and fit correctly */
  scale = (float)dxWin / dx;
  scale2 = (float)dyWin / dy;

  /* The two values of scale are casted to avoid side effects on drawing */
  if (scale > 1)
    scale = (float)((long)scale);
  if (scale2 > 1)
    scale2 = (float)((long)scale2);

  /* The lowest value is choosen */
  if (scale > scale2)
    scale = scale2;

  /* exit if scale too high */
  if (scale > MAX_ZOOM) {
      scale = old_scale;
      return 0;
  }
  /* new window dimension */
  dxscreen = (long)(dxWin / scale);
  dyscreen = (long)(dyWin / scale);
  xscreen += x;
  yscreen += y;
  xy_move(xscreen + dxscreen / 2, yscreen + dyscreen / 2);
  return 1;
}

/* loom_mouse : restores old values of the globals variables. */
void loom_mouse()
{
  xscreen = zoomTrack.x;
  yscreen = zoomTrack.y;
  dxscreen = zoomTrack.dx;
  dyscreen = zoomTrack.dy;
  scale = zoomTrack.scale;
}

/* xy_move : makes the origine of the screen move to a new value. */
void xy_move(x, y)
  long x, y;
{
  xscreen = x - dxscreen / 2;
  yscreen = y - dyscreen / 2;
}
