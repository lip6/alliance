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
| File    :                   V_peek.c                        |
|                                                             |
| Authors :                Frederic Petrot                    |
|                    Modified by Ludovic Jacomme              |
|                                                             |
| Date    :                   08.05.94                        |
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
# include "V_put.h"
# include "V_peek.h"
# include "C_utils.h"

# include "v_view.h"
# include "v_menu.h"
# include "v_extern.h"

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


static void DrawObjectPattern( ptdesc, drawable)

  rdsrec_list *ptdesc;
  Drawable drawable;
{
  GC gc;
  long x, y, dx, dy;
  char layer;

  layer = GetRdsLayer( ptdesc );
  gc    = V_gcCao[ layer ];

  while ( ptdesc ) 
  {
    if ( GenviewCheckInterrupt() ) break;

    dx = (long)ptdesc->DX * scale;
    dy = (long)ptdesc->DY * scale;
    x  = (long)((ptdesc->X - xscreen)* scale);
    y  = YMENU - 1 + (long)((yscreen - 
         ( ptdesc->Y + ptdesc->DY ) + dyscreen)* scale);

    if (dx || dy)
    {
      if ( layer != RDS_ABOX )
      {
        XSetFillStyle(display, gc, FillStippled);
        XFillRectangle(display, drawable, gc, (int)x,
                        (int)y, (int)(dx + 1), (int)(dy + 1));
      }
      XSetFillStyle(display, gc, FillSolid);
      XDrawRectangle(display, drawable, gc, (int)x,
                    (int)y, (int)dx, (int)dy);
    }

    ptdesc = ptdesc->NEXT;
  }
}

static void DrawObjectWire( ptdesc, drawable )

  rdsrec_list *ptdesc;
  Drawable drawable;
{
  GC gc;
  long x, y, dx, dy;
  char layer;

  layer = GetRdsLayer( ptdesc );
  gc    = V_gcCao[ layer ];

  while ( ptdesc ) 
  {
    if ( GenviewCheckInterrupt() ) break;

    dx = (long)ptdesc->DX * scale;
    dy = (long)ptdesc->DY * scale;
    x  = (long)((ptdesc->X - xscreen)* scale);
    y  = YMENU - 1 + (long)((yscreen - 
         ( ptdesc->Y + ptdesc->DY ) + dyscreen)* scale);

    if (dx || dy)
    {
      XSetFillStyle(display, gc, FillSolid);
      XDrawRectangle(display, drawable, gc, (int)x, (int)y,
                (int)dx, (int)dy);
    }
    ptdesc = ptdesc->NEXT;
  }
}

static void DrawObjectFill( ptdesc, drawable )

  rdsrec_list *ptdesc;
  Drawable drawable;
{
  GC gc;
  long x, y, dx, dy;
  char layer;

  layer = GetRdsLayer( ptdesc );
  gc    = V_gcCao[ layer ];

  while ( ptdesc ) 
  {
    if ( GenviewCheckInterrupt() ) break;

    dx = (long)ptdesc->DX * scale;
    dy = (long)ptdesc->DY * scale;
    x  = (long)((ptdesc->X - xscreen)* scale);
    y  = YMENU - 1 + (long)((yscreen - 
         ( ptdesc->Y + ptdesc->DY ) + dyscreen)* scale);

    if (dx || dy)
    {
      XSetFillStyle(display, gc, FillSolid);
      XFillRectangle(display, drawable, gc, (int) x, (int) y,
                (int) dx + 1, (int) dy + 1);
    }
    ptdesc = ptdesc->NEXT;
  }
}

static void DrawObject( ptdesc, drawable )

  rdsrec_list *ptdesc;
  Drawable drawable;
{
  signal( SIGALRM, GenviewInterruptDisplay );
  alarm( 1 );

  if ( ptdesc != (rdsrec_list *)NULL )
  {
    switch (C_Config_List.C_PEEK)
    {
      case FILL:
        DrawObjectFill( ptdesc, drawable );
        break;
      case OUTLINE:
        DrawObjectWire( ptdesc, drawable );
        break;
      default:
        DrawObjectPattern( ptdesc, drawable );
    }
  }

  alarm( 0 );
  GenviewFlushEventDisplay();
}

void V_Peek_it(state, xp, yp, dxp, dyp)

  int state;
  int xp, yp, dxp, dyp;
{
  unsigned char layer;

  XSetForeground (display, V_gcCao[GC_FGND], V_colors[COLOR_BGND]);
  XFillRectangle (display, V_PixSave, V_gcCao[GC_FGND],
      (int)(xp - 1), (int)(yp - 1), (int)(dxp + 2), (int)(dyp + 2));

  for (layer = 0; layer < RDS_MAX_LAYER; layer++) 
  {
    if (C_Config_List.C_VISIBLE[ layer ] == 0) continue;

    DrawObject( Peek_db->LAYERTAB[ layer ], V_PixSave );
  }

  if (state == ON)
  {
    XCopyArea(display, V_PixSave, V_window, V_gcCao[GC_FGND],
            xp, yp, dxp, dyp, xp, yp);
  }

  XSetForeground(display, V_gcCao[GC_FGND], V_colors[COLOR_FGND]);
}

