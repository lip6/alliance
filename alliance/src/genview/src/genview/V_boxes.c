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
| File    :                   V_boxes.c                       |
|                                                             |
| Authors :                Frederic Petrot                    |
|                    Modified by Ludovic Jacomme              |
|                                                             |
| Date    :                   24.04.94                        |
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
# include "V_boxes.h"
# include "C_utils.h"

# include "v_view.h"
# include "v_menu.h"
# include "v_extern.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  static phfig_list *fig;
  static long xold, yold;
  static long dxold, dyold;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

/* highlight the new reference instance */

void V_drawInsRef()
{
  long x, y, dx, dy;
  rdsrec_list *aboxrec;

  if ( ( C_atomicValue == 1    ) || 
       ( xvisufig      == NULL ) ||
       ( Ptinsref      == NULL ) )
  {
    dxold = 0;
    dyold = 0;
    return;
  }

  /* unhighlight the previous one */

  if ( fig == xvisufig && dxold != 0 && dyold != 0 && Ptinsref != NULL ) 
  {
    x = xold, y = yold, dx = dxold, dy = dyold;

    if ( Clip_to_window( &x, &y, &dx, &dy, 
                         xscreen, yscreen, dxscreen, dyscreen ) ) 
    {
      dx = (long)dx * scale;
      dy = (long)dy * scale;
      x  = (long)((x - xscreen)* scale);
      y  = YMENU - 1 + (long)((yscreen - y + dyscreen)* scale);

      if (dx != 0 || dy != 0)
      {
        XDrawRectangle( display, V_window, V_gcCao[ RDS_ABOX ], x, y, dx, dy );
      }
    }
  }

  /* retrieves the new values for overwrite purposes */

  aboxrec = Ptinsref->LAYERTAB[ RDS_ABOX ];

  fig = xvisufig;
  x   = xold = aboxrec->X;
  y   = yold = aboxrec->Y;
  dx  = dxold = aboxrec->DX;
  dy  = dyold = aboxrec->DY;

  if ( Clip_to_window( &x, &y, &dx, &dy, 
                       xscreen, yscreen, dxscreen, dyscreen ) )
  {
    dx = (long)dx * scale;
    dy = (long)dy * scale;
    x  = (long)((x - xscreen)* scale);
    y  = YMENU - 1 + (long)((yscreen - y + dyscreen)* scale);

    if (dx == 0 && dy == 0) return;

    XDrawRectangle( display, V_window, V_gcCao[ GC_INSREF ], x, y, dx, dy );

    if ( ( scale > 0.8                                  ) && 
         ( C_Config_List.C_VISIBLE[ OBJ_INSTANCE ] == 1 ) )

    {
      Put_str( aboxrec, V_window );
    }
  }
}

/* puts the abutment box around the figure */

void V_drawAB()
{
  long x, y, dx, dy;
  long xt, yt, dxt, dyt; /* lambdas */
  unsigned char flag;

  if ( ( C_atomicValue == 1                       ) || 
       ( run_mode == RUN                          ) ||
       ( C_Config_List.C_VISIBLE[ RDS_ABOX ] == 0 ) )
  {
    return;
  }

  xt  = xabfig;
  dxt = xab2fig - xabfig;
  dyt = yab2fig - yabfig;
  yt  = yabfig;

  Clip_to_window(&xt, &yt, &dxt, &dyt, xscreen, yscreen, dxscreen, dyscreen);

  x  = (long)((xt - xscreen)* scale);
  dx = (long)dxt * scale;
  dy = (long)dyt * scale;
  y  = YMENU - 1 + (long)((yscreen - yt + dyscreen)* scale);

  if (dx < 3 || dy < 3) return;

  XDrawRectangle( display, V_window, V_gcCao[ RDS_ABOX ], x, y, dx, dy );
}
