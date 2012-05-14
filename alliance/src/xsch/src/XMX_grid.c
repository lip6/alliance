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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   XSCH                            |
|                                                             |
| File    :                   Grid.c                          |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   01.06.96                        |
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
# include "mlo.h"
# include "XSB.h"
# include "scl.h"
# include "XSC.h"
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

  float      XschUnitGridStep;

  long       XschUnitGridX;
  long       XschUnitGridY;
  long       XschUnitGridDx;
  long       XschUnitGridDy;

  long       XschPixelGridX;
  long       XschPixelGridY;

/*------------------------------------------------------------\
|                                                             |
|                        User Unit Grid                       |
|                                                             |
\------------------------------------------------------------*/

  long       XschUnitUserGridDx;
  long       XschUnitUserGridDy;
  int        XschUnitUserGrid;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  XschInitializeUnitGrid                    |
|                                                             |
\------------------------------------------------------------*/

void XschInitializeUnitGrid()

{
  autbegin();

  XschUnitGridX      = XSCH_DEFAULT_GRID_X;
  XschUnitGridY      = XSCH_DEFAULT_GRID_Y;
  XschUnitGridDx     = XSCH_DEFAULT_GRID_DX;
  XschUnitGridDy     = XSCH_DEFAULT_GRID_DY;
  XschUnitUserGridDx = 4;
  XschUnitUserGridDy = 4;
  XschUnitUserGrid   = XSCH_FALSE;

  XschComputeUnitGrid();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                   XschComputeUnitGrid                      |
|                                                             |
\------------------------------------------------------------*/

void XschComputeUnitGrid()

{
  float StepX;
  float StepY;

  autbegin();

  StepX = (float)(XschGraphicDx) / (float)(XschUnitGridDx);
  StepY = (float)(XschGraphicDy) / (float)(XschUnitGridDy);

  if ( StepX < StepY )
  {
    XschUnitGridStep = StepX;
    XschUnitGridDy   = 1 + ( XschGraphicDy / StepX );
  }
  else
  {
    XschUnitGridStep = StepY;
    XschUnitGridDx   = 1 + ( XschGraphicDx / StepY );
  }

  XschPixelGridX = (float)(XschUnitGridX) * XschUnitGridStep;
  XschPixelGridY = (float)(XschUnitGridY) * XschUnitGridStep;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XschResizeUnitGrid                      |
|                                                             |
\------------------------------------------------------------*/

void XschResizeUnitGrid()

{
  autbegin();

  XschUnitGridDx = 1 + ( XschGraphicDx / XschUnitGridStep );
  XschUnitGridDy = 1 + ( XschGraphicDy / XschUnitGridStep );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XschDisplayUnitGrid                  |
|                                                             |
\------------------------------------------------------------*/

int  XschDisplayUnitGrid( GraphicX1, GraphicY1, GraphicX2, GraphicY2 )

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
  int       UserGrid;
  int       UnitGrid;
  float     Check;

  autbegin();

  if ( XschUnitGridStep < XSCH_LOWER_GRID_STEP ) 
  {
    UnitGrid = 0;
  }
  else 
  {
    UnitGrid = 1;
  }

  if ( XschUnitUserGrid == 1  )
  {
    if ( XschUnitUserGridDx > XschUnitUserGridDy )
    {
      Check = ( XSCH_LOWER_GRID_STEP * 2 / XschUnitUserGridDy );
    }
    else
    {
      Check = ( XSCH_LOWER_GRID_STEP * 2 / XschUnitUserGridDy );
    }

    if ( XschUnitGridStep < Check ) UserGrid = 0;
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
    return( XSCH_FALSE );
  }

  MaxGridX = XschUnitGridX + XschUnitGridDx;
  MaxGridY = XschUnitGridY + XschUnitGridDy;
 
  if ( UnitGrid )
  {
    for ( X = XschUnitGridX; X < MaxGridX; X = X + 1 )
    {
      PixelX = ((float)(X) * XschUnitGridStep);
      PixelX = PixelX - XschPixelGridX;

      if ( ( PixelX <= GraphicX2 ) &&
           ( PixelX >= GraphicX1 ) )

      for ( Y = XschUnitGridY; Y < MaxGridY; Y = Y + 1 )
      {
        PixelY = ((float)(Y) * XschUnitGridStep);
        PixelY = PixelY - XschPixelGridY;
        PixelY = XschGraphicDy - PixelY;

        if ( ( PixelY <= GraphicY2 ) &&
             ( PixelY >= GraphicY1 ) )
        {
          XDrawPoint( XschGraphicDisplay,
                      XtWindow( XschGraphicWindow ),
                      XschGridGC,
                      PixelX, PixelY );
        }
      }
    }
  }

  if ( UserGrid )
  {
    Xmin  = ( XschUnitGridX / XschUnitUserGridDx ) * XschUnitUserGridDx;
    Ymin  = ( XschUnitGridY / XschUnitUserGridDy ) * XschUnitUserGridDy;

    for ( X = Xmin; X < MaxGridX ; X = X + XschUnitUserGridDx )
    {
      PixelX = ((float)(X) * XschUnitGridStep);
      PixelX = PixelX - XschPixelGridX;

      if ( ( PixelX <= GraphicX2 ) &&
           ( PixelX >= GraphicX1 ) )

      for ( Y = Ymin; Y < MaxGridY; Y = Y + XschUnitUserGridDy )
      {
        PixelY = ((float)(Y) * XschUnitGridStep);
        PixelY = PixelY - XschPixelGridY;
        PixelY = XschGraphicDy - PixelY;

        if ( ( PixelY <= GraphicY2 ) &&
             ( PixelY >= GraphicY1 ) )
        {
          XDrawLine( XschGraphicDisplay,
                     XtWindow( XschGraphicWindow ),
                     XschGridGC,
                     PixelX - 2, PixelY,
                     PixelX + 2, PixelY );
                        
          XDrawLine( XschGraphicDisplay,
                     XtWindow( XschGraphicWindow ),
                     XschGridGC,
                     PixelX, PixelY + 2,
                     PixelX, PixelY - 2 );
        }
      }
    }
  }

  autend();
  return( XSCH_TRUE );
}
