/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU General Public |
| License as  published by the Free Software Foundation;      |
| either version 2 of the License, or (at your option) any    |
| later version.                                              |
|                                                             |
| Alliance VLSI  CAD System  is distributed  in the hope that |
| it  will be useful, but WITHOUT  ANY WARRANTY;              |
| without even the  implied warranty of MERCHANTABILITY or    |
| FITNESS FOR A PARTICULAR PURPOSE. See the GNU General       |
| Public License for more details.                            |
|                                                             |
| You should have received a copy  of the GNU General Public  |
| License along with the GNU C Library; see the file COPYING. |
| If not, write to the Free Software Foundation, Inc.,        |
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   XFSM                            |
|                                                             |
| File    :                   Grid.c                          |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   04.12.96                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <Xm/Xm.h>
 
# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "XSB.h"
# include "XFS.h"
# include "XMX.h"

# include "XMX_grid.h"

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                           Unit Grid                         |
|                                                             |
\------------------------------------------------------------*/

  float      XfsmUnitGridStep;

  long       XfsmUnitGridX;
  long       XfsmUnitGridY;
  long       XfsmUnitGridDx;
  long       XfsmUnitGridDy;

  long       XfsmPixelGridX;
  long       XfsmPixelGridY;

/*------------------------------------------------------------\
|                                                             |
|                        User Unit Grid                       |
|                                                             |
\------------------------------------------------------------*/

  long       XfsmUnitUserGridDx;
  long       XfsmUnitUserGridDy;
  char       XfsmUnitUserGrid;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  XfsmInitializeUnitGrid                    |
|                                                             |
\------------------------------------------------------------*/

void XfsmInitializeUnitGrid()

{
  autbegin();

  XfsmUnitGridX      = XFSM_DEFAULT_GRID_X;
  XfsmUnitGridY      = XFSM_DEFAULT_GRID_Y;
  XfsmUnitGridDx     = XFSM_DEFAULT_GRID_DX;
  XfsmUnitGridDy     = XFSM_DEFAULT_GRID_DY;
  XfsmUnitUserGridDx = 4;
  XfsmUnitUserGridDy = 4;
  XfsmUnitUserGrid   = XFSM_FALSE;

  XfsmComputeUnitGrid();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                   XfsmComputeUnitGrid                      |
|                                                             |
\------------------------------------------------------------*/

void XfsmComputeUnitGrid()

{
  float StepX;
  float StepY;

  autbegin();

  StepX = (float)(XfsmGraphicDx) / (float)(XfsmUnitGridDx);
  StepY = (float)(XfsmGraphicDy) / (float)(XfsmUnitGridDy);

  if ( StepX < StepY )
  {
    XfsmUnitGridStep = StepX;
    XfsmUnitGridDy   = 1 + ( XfsmGraphicDy / StepX );
  }
  else
  {
    XfsmUnitGridStep = StepY;
    XfsmUnitGridDx   = 1 + ( XfsmGraphicDx / StepY );
  }

  XfsmPixelGridX = (float)(XfsmUnitGridX) * XfsmUnitGridStep;
  XfsmPixelGridY = (float)(XfsmUnitGridY) * XfsmUnitGridStep;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XfsmResizeUnitGrid                      |
|                                                             |
\------------------------------------------------------------*/

void XfsmResizeUnitGrid()

{
  autbegin();

  XfsmUnitGridDx = 1 + ( XfsmGraphicDx / XfsmUnitGridStep );
  XfsmUnitGridDy = 1 + ( XfsmGraphicDy / XfsmUnitGridStep );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XfsmDisplayUnitGrid                  |
|                                                             |
\------------------------------------------------------------*/

char XfsmDisplayUnitGrid( GraphicX1, GraphicY1, GraphicX2, GraphicY2 )

   Dimension GraphicX1;
   Dimension GraphicY1;
   Dimension GraphicX2;
   Dimension GraphicY2;
{
  long      X;
  long      Y;
  long      MaxGridX;
  long      MaxGridY;
  long      PixelX;
  long      PixelY;
  long      Xmin;
  long      Ymin;
  char      UserGrid;
  char      UnitGrid;
  float     Check;

  autbegin();

  if ( XfsmUnitGridStep < XFSM_LOWER_GRID_STEP ) 
  {
    UnitGrid = 0;
  }
  else 
  {
    UnitGrid = 1;
  }

  if ( XfsmUnitUserGrid == 1  )
  {
    if ( XfsmUnitUserGridDx > XfsmUnitUserGridDy )
    {
      Check = ( XFSM_LOWER_GRID_STEP * 2 / XfsmUnitUserGridDy );
    }
    else
    {
      Check = ( XFSM_LOWER_GRID_STEP * 2 / XfsmUnitUserGridDy );
    }

    if ( XfsmUnitGridStep < Check ) UserGrid = 0;
    else                               UserGrid = 1;
  } 
  else
  {
    UserGrid = 0;
  }

  if ( ( UserGrid   == 0 ) &&
       ( UnitGrid == 0 ) )
  {
    autend();
    return( XFSM_FALSE );
  }

  MaxGridX = XfsmUnitGridX + XfsmUnitGridDx;
  MaxGridY = XfsmUnitGridY + XfsmUnitGridDy;
 
  if ( UnitGrid )
  {
    for ( X = XfsmUnitGridX; X < MaxGridX; X = X + 1 )
    {
      PixelX = ((float)(X) * XfsmUnitGridStep);
      PixelX = PixelX - XfsmPixelGridX;

      if ( ( PixelX <= GraphicX2 ) &&
           ( PixelX >= GraphicX1 ) )

      for ( Y = XfsmUnitGridY; Y < MaxGridY; Y = Y + 1 )
      {
        PixelY = ((float)(Y) * XfsmUnitGridStep);
        PixelY = PixelY - XfsmPixelGridY;
        PixelY = XfsmGraphicDy - PixelY;

        if ( ( PixelY <= GraphicY2 ) &&
             ( PixelY >= GraphicY1 ) )
        {
          XDrawPoint( XfsmGraphicDisplay,
                      XtWindow( XfsmGraphicWindow ),
                      XfsmGridGC,
                      PixelX, PixelY );
        }
      }
    }
  }

  if ( UserGrid )
  {
    Xmin  = ( XfsmUnitGridX / XfsmUnitUserGridDx ) * XfsmUnitUserGridDx;
    Ymin  = ( XfsmUnitGridY / XfsmUnitUserGridDy ) * XfsmUnitUserGridDy;

    for ( X = Xmin; X < MaxGridX ; X = X + XfsmUnitUserGridDx )
    {
      PixelX = ((float)(X) * XfsmUnitGridStep);
      PixelX = PixelX - XfsmPixelGridX;

      if ( ( PixelX <= GraphicX2 ) &&
           ( PixelX >= GraphicX1 ) )

      for ( Y = Ymin; Y < MaxGridY; Y = Y + XfsmUnitUserGridDy )
      {
        PixelY = ((float)(Y) * XfsmUnitGridStep);
        PixelY = PixelY - XfsmPixelGridY;
        PixelY = XfsmGraphicDy - PixelY;

        if ( ( PixelY <= GraphicY2 ) &&
             ( PixelY >= GraphicY1 ) )
        {
          XDrawLine( XfsmGraphicDisplay,
                     XtWindow( XfsmGraphicWindow ),
                     XfsmGridGC,
                     PixelX - 2, PixelY,
                     PixelX + 2, PixelY );
                        
          XDrawLine( XfsmGraphicDisplay,
                     XtWindow( XfsmGraphicWindow ),
                     XfsmGridGC,
                     PixelX, PixelY + 2,
                     PixelX, PixelY - 2 );
        }
      }
    }
  }

  autend();
  return( XFSM_TRUE );
}
