/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
| Tool    :                   XGRA                            |
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
# include "XSB.h"
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

  float      XgraUnitGridStep;

  long       XgraUnitGridX;
  long       XgraUnitGridY;
  long       XgraUnitGridDx;
  long       XgraUnitGridDy;

  long       XgraPixelGridX;
  long       XgraPixelGridY;

/*------------------------------------------------------------\
|                                                             |
|                        User Unit Grid                       |
|                                                             |
\------------------------------------------------------------*/

  long       XgraUnitUserGridDx;
  long       XgraUnitUserGridDy;
  char       XgraUnitUserGrid;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  XgraInitializeUnitGrid                    |
|                                                             |
\------------------------------------------------------------*/

void XgraInitializeUnitGrid()

{
  autbegin();

  XgraUnitGridX      = XGRA_DEFAULT_GRID_X;
  XgraUnitGridY      = XGRA_DEFAULT_GRID_Y;
  XgraUnitGridDx     = XGRA_DEFAULT_GRID_DX;
  XgraUnitGridDy     = XGRA_DEFAULT_GRID_DY;
  XgraUnitUserGridDx = 4;
  XgraUnitUserGridDy = 4;
  XgraUnitUserGrid   = XGRA_FALSE;

  XgraComputeUnitGrid();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                   XgraComputeUnitGrid                      |
|                                                             |
\------------------------------------------------------------*/

void XgraComputeUnitGrid()

{
  float StepX;
  float StepY;

  autbegin();

  StepX = (float)(XgraGraphicDx) / (float)(XgraUnitGridDx);
  StepY = (float)(XgraGraphicDy) / (float)(XgraUnitGridDy);

  if ( StepX < StepY )
  {
    XgraUnitGridStep = StepX;
    XgraUnitGridDy   = 1 + ( XgraGraphicDy / StepX );
  }
  else
  {
    XgraUnitGridStep = StepY;
    XgraUnitGridDx   = 1 + ( XgraGraphicDx / StepY );
  }

  XgraPixelGridX = (float)(XgraUnitGridX) * XgraUnitGridStep;
  XgraPixelGridY = (float)(XgraUnitGridY) * XgraUnitGridStep;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XgraResizeUnitGrid                      |
|                                                             |
\------------------------------------------------------------*/

void XgraResizeUnitGrid()

{
  autbegin();

  XgraUnitGridDx = 1 + ( XgraGraphicDx / XgraUnitGridStep );
  XgraUnitGridDy = 1 + ( XgraGraphicDy / XgraUnitGridStep );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XgraDisplayUnitGrid                  |
|                                                             |
\------------------------------------------------------------*/

char XgraDisplayUnitGrid( GraphicX1, GraphicY1, GraphicX2, GraphicY2 )

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

  if ( XgraUnitGridStep < XGRA_LOWER_GRID_STEP ) 
  {
    UnitGrid = 0;
  }
  else 
  {
    UnitGrid = 1;
  }

  if ( XgraUnitUserGrid == 1  )
  {
    Check = ( XGRA_LOWER_GRID_STEP * 2 / XgraUnitUserGridDy );
#if 0
    if ( XgraUnitUserGridDx > XgraUnitUserGridDy )
    {
      Check = ( XGRA_LOWER_GRID_STEP * 2 / XgraUnitUserGridDy );
    }
    else
    {
      Check = ( XGRA_LOWER_GRID_STEP * 2 / XgraUnitUserGridDy );
    }
#endif

    if ( XgraUnitGridStep < Check ) UserGrid = 0;
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
    return( XGRA_FALSE );
  }

  MaxGridX = XgraUnitGridX + XgraUnitGridDx;
  MaxGridY = XgraUnitGridY + XgraUnitGridDy;
 
  if ( UnitGrid )
  {
    for ( X = XgraUnitGridX; X < MaxGridX; X = X + 1 )
    {
      PixelX = ((float)(X) * XgraUnitGridStep);
      PixelX = PixelX - XgraPixelGridX;

      if ( ( PixelX <= GraphicX2 ) &&
           ( PixelX >= GraphicX1 ) )

      for ( Y = XgraUnitGridY; Y < MaxGridY; Y = Y + 1 )
      {
        PixelY = ((float)(Y) * XgraUnitGridStep);
        PixelY = PixelY - XgraPixelGridY;
        PixelY = XgraGraphicDy - PixelY;

        if ( ( PixelY <= GraphicY2 ) &&
             ( PixelY >= GraphicY1 ) )
        {
          XDrawPoint( XgraGraphicDisplay,
                      XtWindow( XgraGraphicWindow ),
                      XgraGridGC,
                      PixelX, PixelY );
        }
      }
    }
  }

  if ( UserGrid )
  {
    Xmin  = ( XgraUnitGridX / XgraUnitUserGridDx ) * XgraUnitUserGridDx;
    Ymin  = ( XgraUnitGridY / XgraUnitUserGridDy ) * XgraUnitUserGridDy;

    for ( X = Xmin; X < MaxGridX ; X = X + XgraUnitUserGridDx )
    {
      PixelX = ((float)(X) * XgraUnitGridStep);
      PixelX = PixelX - XgraPixelGridX;

      if ( ( PixelX <= GraphicX2 ) &&
           ( PixelX >= GraphicX1 ) )

      for ( Y = Ymin; Y < MaxGridY; Y = Y + XgraUnitUserGridDy )
      {
        PixelY = ((float)(Y) * XgraUnitGridStep);
        PixelY = PixelY - XgraPixelGridY;
        PixelY = XgraGraphicDy - PixelY;

        if ( ( PixelY <= GraphicY2 ) &&
             ( PixelY >= GraphicY1 ) )
        {
          XDrawLine( XgraGraphicDisplay,
                     XtWindow( XgraGraphicWindow ),
                     XgraGridGC,
                     PixelX - 2, PixelY,
                     PixelX + 2, PixelY );
                        
          XDrawLine( XgraGraphicDisplay,
                     XtWindow( XgraGraphicWindow ),
                     XgraGridGC,
                     PixelX, PixelY + 2,
                     PixelX, PixelY - 2 );
        }
      }
    }
  }

  autend();
  return( XGRA_TRUE );
}
