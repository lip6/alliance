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
| File    :                   V_put.c                         |
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
# include <signal.h>
# include MUT_H
# include MPH_H
# include RDS_H
# include RWI_H
# include RUT_H
# include RPR_H
# include RFM_H
# include "G_global.h"
# include "C_utils.h"
# include "V_put.h"
# include "v_view.h"
# include "v_menu.h"
# include "v_extern.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  int GenviewInterrupt = 0;
  int GenviewCheckInt  = 0;

  static strview *GenviewHeadView = (strview *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Alloc Functions                       |
|                                                             |
\------------------------------------------------------------*/

strview *GenviewAllocView()
{
  return((strview *)rdsalloc(sizeof(strview), 1));
}

/*------------------------------------------------------------\
|                                                             |
|                        Free Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       GenviewFreeView                       |
|                                                             |
\------------------------------------------------------------*/

void GenviewFreeView( FreeView )

   strview *FreeView;
{
  rdsfree((char *)FreeView, sizeof(strview));
}

/*------------------------------------------------------------\
|                                                             |
|                    GenviewCheckInterrupt                    |
|                                                             |
\------------------------------------------------------------*/

int GenviewCheckInterrupt()
{
  XEvent       Event;
  KeySym       Key;
  char         Text[ 2 ];

  if ( GenviewCheckInt )
  {
    if ( XCheckTypedEvent( display, KeyPress, &Event ) )
    {
      XLookupString( &Event.xkey, Text, 1, &Key, 0 );
 
      if ( Text[ 0 ] == '\003' )
      {
        GenviewInterrupt = 1;

        XBell( display, 0 );
      }
    }
  }

  GenviewCheckInt = 0;

  return( GenviewInterrupt );
}

/*------------------------------------------------------------\
|                                                             |
|                  GenviewInterruptDisplay                    |
|                                                             |
\------------------------------------------------------------*/

void GenviewInterruptDisplay()
{
  GenviewCheckInt = 1;

  signal( SIGALRM, GenviewInterruptDisplay );
  alarm( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                    GenviewFlushEventDisplay                 |
|                                                             |
\------------------------------------------------------------*/

void GenviewFlushEventDisplay()
{
  XEvent Event;

  while ( XCheckTypedEvent( display, KeyPress, &Event ) );
}

/*------------------------------------------------------------\
|                                                             |
|                    GenviewAddViewLater                      |
|                                                             |
\------------------------------------------------------------*/

void GenviewAddViewLater( Rectangle )

   rdsrec_list *Rectangle;
{
  strview *View;

  View = GenviewAllocView();

  View->RECTANGLE = Rectangle;
  View->NEXT      = GenviewHeadView;
  GenviewHeadView = View;
}

/*------------------------------------------------------------\
|                                                             |
|                       GenviewDelView                        |
|                                                             |
\------------------------------------------------------------*/

void GenviewDelView()
{
  strview    *DelView;
  strview    *View;

  View            = GenviewHeadView;
  GenviewHeadView = (strview *)NULL;

  while( View != (strview *)NULL )
  {
    DelView  = View;
    View     = View->NEXT;

    GenviewFreeView( DelView );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          Put IT                             |
|                                                             |
\------------------------------------------------------------*/

void Put_it(drawable, xwin, ywin, dxwin, dywin)

   Drawable drawable;
   long xwin, ywin, dxwin, dywin;
{
  GenviewInterrupt = 0;
  GenviewCheckInt  = 0;

  signal( SIGALRM, GenviewInterruptDisplay );
  alarm( 1 );

  switch (C_Config_List.C_VIEW) 
  {
    case PATTERN:
      Put_it_pattern(drawable, xwin, ywin, dxwin, dywin);
      break;
    case OUTLINE:
      Put_it_wire(drawable, xwin, ywin, dxwin, dywin);
      break;
    case FILL:
      Put_it_fill(drawable, xwin, ywin, dxwin, dywin);
  }

  V_drawInsRef();
  V_drawAB();

  alarm( 0 );
  GenviewFlushEventDisplay();
}

void Put_it_pattern(drawable, xwin, ywin, dxwin, dywin)

  Drawable drawable;
  long xwin, ywin, dxwin, dywin;
{
  long x, y, dx, dy;
  GC gc;
  char          layer;
  rdsrec_list  *pt;
  rdsins_list  *ptins;
  char          scanins;
  int           flag;
  strview      *ScanStr;

  flag = ( scale > 1.0 );

  for (layer = 0; layer < RDS_MAX_LAYER; layer++) 
  {
    if ( GenviewCheckInterrupt() ) break;

    if ( C_Config_List.C_VISIBLE[ layer ] == 0) continue;

    pt = Visu_db->LAYERTAB[ layer ];
    gc = V_gcCao[ layer ];

    if ( C_Config_List.C_VISIBLE[ OBJ_INSTANCE ] == 1 )
    {
      ptins = Visu_db->INSTANCE;
    }
    else
    {
      ptins = (rdsins_list *)NULL;
    }

    scanins = 0;

    while ( ( pt != (rdsrec_list *)NULL    ) || 
            ( ptins != (rdsins_list *)NULL ) )
    {
      if ( GenviewCheckInterrupt() ) break;

      if ( pt == (rdsrec_list *)NULL )
      {
        if ( ptins == (rdsins_list *)NULL ) break;

        if ( scanins == 0 )
        {
          scanins = 1;
          pt = ptins->LAYERTAB[ layer ];
          continue;
        }
        else
        {
          ptins = ptins->NEXT;

          if ( ptins != (rdsins_list *)NULL )
          {
            pt = ptins->LAYERTAB[ layer ];
          }
          continue;
        }
      }

      x  = pt->X;
      dx = pt->DX;
      dy = pt->DY;
      y  = pt->Y;

      if ( Clip_to_window( &x, &y, &dx, &dy, xwin, ywin, dxwin, dywin ) ) 
      {
        dx = (long)dx * scale;
        dy = (long)dy * scale;
        x  = (long)((x - xscreen)* scale);
        y  = YMENU - 1 + (long)((yscreen - y + dyscreen)* scale);

        if (dx || dy) 
        {
          if ( ( layer != RDS_ABOX ) &&
               ( dx    >  2        ) &&
               ( dy    >  2        ) )
          {
            XSetFillStyle( display, gc, FillStippled );

            XFillRectangle( display, drawable, gc, (int)x,
                            (int)y, (int)(dx + 1), (int)(dy + 1));
          }

          XSetFillStyle( display, gc, FillSolid );

          XDrawRectangle( display, drawable, gc, (int)x,
                          (int)y, (int)dx, (int)dy );

          SetGenviewDrawn( pt );

          if ( ( flag                     ) &&
               ( pt->NAME != (char *)NULL ) )
          {
            GenviewAddViewLater( pt );
          }
        }
      }
      else 
      {
        ClearGenviewDrawn( pt );
      }

      pt = pt->NEXT;
    }
  }

  for ( ScanStr  = GenviewHeadView;
        ScanStr != (strview *)NULL;
        ScanStr  = ScanStr->NEXT )
  {
    Put_str( ScanStr->RECTANGLE, drawable );
  }

  GenviewDelView();
}

void Put_it_wire(drawable, xwin, ywin, dxwin, dywin)

  Drawable drawable;
  long xwin, ywin, dxwin, dywin;
{
  long x, y, dx, dy;
  GC gc;
  char         layer;
  rdsrec_list *pt;
  rdsins_list *ptins;
  char         flag;
  char         scanins;
  strview      *ScanStr;

  flag = ( scale > 1.0 );

  for (layer = 0; layer < RDS_MAX_LAYER; layer++) 
  {
    if ( C_Config_List.C_VISIBLE[ layer ] == 0) continue;

    pt = Visu_db->LAYERTAB[layer];
    gc = V_gcCao[layer];

    XSetFillStyle(display, gc, FillSolid);
     
    if ( C_Config_List.C_VISIBLE[ OBJ_INSTANCE ] == 1 )
    {
      ptins = Visu_db->INSTANCE;
    }
    else
    {
      ptins = (rdsins_list *)NULL;
    }

    scanins = 0;

    while ( ( pt != (rdsrec_list *)NULL    ) ||
            ( ptins != (rdsins_list *)NULL ) )
    {
      if ( GenviewCheckInterrupt() ) break;

      if ( pt == (rdsrec_list *)NULL )
      {
        if ( ptins == (rdsins_list *)NULL ) break;

        if ( scanins == 0 )
        {
          scanins = 1;
          pt = ptins->LAYERTAB[ layer ];
          continue;
        }
        else
        {
          ptins = ptins->NEXT;

          if ( ptins != (rdsins_list *)NULL )
          {
            pt = ptins->LAYERTAB[ layer ];
          }
          continue;
        }
      }

      x  = pt->X;
      y  = pt->Y;
      dx = pt->DX;
      dy = pt->DY;

      if ( Clip_to_window( &x, &y, &dx, &dy, xwin, ywin, dxwin, dywin ) )
      {
        dx = (long)dx * scale;
        dy = (long)dy * scale;
        x  = (long)((x - xscreen)* scale);
        y  = YMENU - 1 + (long)((yscreen - y + dyscreen)* scale);

        if (dx || dy)
        {
          XDrawRectangle(display, drawable, gc, (int)x,
                    (int)y, (int)dx, (int)dy);

          SetGenviewDrawn( pt );

          if ( ( flag                     ) &&
               ( pt->NAME != (char *)NULL ) )
          {
            GenviewAddViewLater( pt );
          }

        }
      } 
      else 
      {
        ClearGenviewDrawn( pt );
      } 

      pt = pt->NEXT;
    }
  }

  for ( ScanStr  = GenviewHeadView;
        ScanStr != (strview *)NULL;
        ScanStr  = ScanStr->NEXT )
  {
    Put_str( ScanStr->RECTANGLE, drawable );
  }

STOP:

  GenviewDelView();
}

void Put_it_fill(drawable, xwin, ywin, dxwin, dywin)

  Drawable drawable;
  long xwin, ywin, dxwin, dywin;
{
  long x, y, dx, dy;
  GC gc;
  char         layer;
  rdsrec_list *pt;
  rdsins_list *ptins;
  char         flag;
  char         scanins;
  strview     *ScanStr;

  flag = ( scale > 1.0 );

  for (layer = 0; layer < RDS_MAX_LAYER; layer++) 
  {
    if ( C_Config_List.C_VISIBLE[ layer ] == 0) continue;

    pt = Visu_db->LAYERTAB[ layer ];
    gc = V_gcCao[ layer ];

    XSetFillStyle(display, gc, FillSolid);

    if ( C_Config_List.C_VISIBLE[ OBJ_INSTANCE ] == 1 )
    {
      ptins = Visu_db->INSTANCE;
    }
    else
    {
      ptins = (rdsins_list *)NULL;
    }

    scanins = 0;

    while ( ( pt != (rdsrec_list *)NULL    ) ||
            ( ptins != (rdsins_list *)NULL ) )
    {
      if ( GenviewCheckInterrupt() ) break;

      if ( pt == (rdsrec_list *)NULL )
      {
        if ( ptins == (rdsins_list *)NULL ) break;

        if ( scanins == 0 )
        {
          scanins = 1;
          pt = ptins->LAYERTAB[ layer ];
          continue;
        }
        else
        {
          ptins = ptins->NEXT;

          if ( ptins != (rdsins_list *)NULL )
          {
            pt = ptins->LAYERTAB[ layer ];
          }
          continue;
        }
      }

      x  = pt->X;
      dx = pt->DX;
      dy = pt->DY;
      y  = pt->Y;

      if (Clip_to_window(&x, &y, &dx, &dy, xwin, ywin, dxwin, dywin)) 
      {
        dx = (long)dx *scale;
        dy = (long)dy *scale;
        x  = (long)((x - xscreen)* scale);
        y  = YMENU - 1 + (long)((yscreen - y + dyscreen)* scale);

        if (dx || dy) 
        {
          if ( layer != RDS_ABOX )
          {
            XFillRectangle(display, drawable, gc, (int)x,
                          (int)y, (int)(dx + 1), (int)(dy + 1));
          }
          else
          {
            XDrawRectangle(display, drawable, gc, (int)x,
                      (int)y, (int)dx, (int)dy);
          }

          SetGenviewDrawn( pt );

          if ( ( flag                     ) &&
               ( pt->NAME != (char *)NULL ) )
          {
            GenviewAddViewLater( pt );
          }
        }
      }
      else 
      {
        ClearGenviewDrawn( pt );
      }

      pt = pt->NEXT;
    }
  }

  for ( ScanStr  = GenviewHeadView;
        ScanStr != (strview *)NULL;
        ScanStr  = ScanStr->NEXT )
  {
    Put_str( ScanStr->RECTANGLE, drawable );
  }

STOP:

  GenviewDelView();
}

void V_DrawBox( rectBox )

  rdsrec_list *rectBox;
{
  GC gc;
  long x, y, dx, dy;

  gc = V_gcCao[ GetRdsLayer( rectBox ) ];

  x  = rectBox->X;
  dx = rectBox->DX;
  dy = rectBox->DY;
  y  = rectBox->Y;

  if (!dx || !dy) return;

  if ( Clip_to_window( &x, &y, &dx, &dy,
                       xscreen, yscreen, dxscreen, dyscreen ) ) 
  {
    dx = (long)( dx *scale );
    dy = (long)( dy *scale );
    x  = (long)(( x - xscreen)* scale);
    y  = YMENU - 1 + (long)((yscreen - y + dyscreen)* scale);

    if ( IsRdsInstance( rectBox ) ) 
    {
      XDrawRectangle(display, V_window, gc, x, y, dx, dy);
  
      if (scale > 1) Put_str( rectBox, V_window );
    }
    else
    {
      if (C_Config_List.C_VIEW == FILL) 
      {
        XSetFillStyle(display, gc, FillSolid);

        XFillRectangle( display, V_window, gc, x, y,
                        (int)(dx + 1), (int)(dy + 1));
      }
      else 
      if (C_Config_List.C_VIEW == PATTERN) 
      {
        XSetFillStyle( display, gc, FillStippled);
        XFillRectangle( display, V_window, gc, x, y, 
                      (int)(dx + 1), (int)(dy + 1));

        XSetFillStyle(display, gc, FillSolid );
        XDrawRectangle(display, V_window, gc, x, y, dx, dy);
      }
      else
      if (C_Config_List.C_VIEW == OUTLINE) 
      {
        XSetFillStyle(display, gc, FillSolid);
        XDrawRectangle(display, V_window, gc, x, y, dx, dy);
      }
  
      if (scale > 3) Put_str(rectBox, V_window);
    }
  }

  V_drawInsRef();

  XFlush(display);
}
