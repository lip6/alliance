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
| Tool    :                   XVPN                            |
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

  float      XvpnUnitGridStep;

  long       XvpnUnitGridX;
  long       XvpnUnitGridY;
  long       XvpnUnitGridDx;
  long       XvpnUnitGridDy;

  long       XvpnPixelGridX;
  long       XvpnPixelGridY;

/*------------------------------------------------------------\
|                                                             |
|                        User Unit Grid                       |
|                                                             |
\------------------------------------------------------------*/

  long       XvpnUnitUserGridDx;
  long       XvpnUnitUserGridDy;
  short      XvpnUnitUserGrid;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  XvpnInitializeUnitGrid                    |
|                                                             |
\------------------------------------------------------------*/

void XvpnInitializeUnitGrid()

{
  autbegin();

  XvpnUnitGridX      = XVPN_DEFAULT_GRID_X;
  XvpnUnitGridY      = XVPN_DEFAULT_GRID_Y;
  XvpnUnitGridDx     = XVPN_DEFAULT_GRID_DX;
  XvpnUnitGridDy     = XVPN_DEFAULT_GRID_DY;
  XvpnUnitUserGridDx = 4;
  XvpnUnitUserGridDy = 4;
  XvpnUnitUserGrid   = XVPN_FALSE;

  XvpnComputeUnitGrid();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                   XvpnComputeUnitGrid                      |
|                                                             |
\------------------------------------------------------------*/

void XvpnComputeUnitGrid()

{
  float StepX;
  float StepY;

  autbegin();

  StepX = (float)(XvpnGraphicDx) / (float)(XvpnUnitGridDx);
  StepY = (float)(XvpnGraphicDy) / (float)(XvpnUnitGridDy);

  if ( StepX < StepY )
  {
    XvpnUnitGridStep = StepX;
    XvpnUnitGridDy   = 1 + ( XvpnGraphicDy / StepX );
  }
  else
  {
    XvpnUnitGridStep = StepY;
    XvpnUnitGridDx   = 1 + ( XvpnGraphicDx / StepY );
  }

  XvpnPixelGridX = (float)(XvpnUnitGridX) * XvpnUnitGridStep;
  XvpnPixelGridY = (float)(XvpnUnitGridY) * XvpnUnitGridStep;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XvpnResizeUnitGrid                      |
|                                                             |
\------------------------------------------------------------*/

void XvpnResizeUnitGrid()

{
  autbegin();

  XvpnUnitGridDx = 1 + ( XvpnGraphicDx / XvpnUnitGridStep );
  XvpnUnitGridDy = 1 + ( XvpnGraphicDy / XvpnUnitGridStep );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XvpnDisplayUnitGrid                  |
|                                                             |
\------------------------------------------------------------*/

short XvpnDisplayUnitGrid( GraphicX1, GraphicY1, GraphicX2, GraphicY2 )

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
  short     UserGrid;
  short     UnitGrid;
  float     Check;

  autbegin();

  if ( XvpnUnitGridStep < XVPN_LOWER_GRID_STEP ) 
  {
    UnitGrid = 0;
  }
  else 
  {
    UnitGrid = 1;
  }

  if ( XvpnUnitUserGrid == 1  )
  {
    if ( XvpnUnitUserGridDx > XvpnUnitUserGridDy )
    {
      Check = ( XVPN_LOWER_GRID_STEP * 2 / XvpnUnitUserGridDy );
    }
    else
    {
      Check = ( XVPN_LOWER_GRID_STEP * 2 / XvpnUnitUserGridDy );
    }

    if ( XvpnUnitGridStep < Check ) UserGrid = 0;
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
    return( XVPN_FALSE );
  }

  MaxGridX = XvpnUnitGridX + XvpnUnitGridDx;
  MaxGridY = XvpnUnitGridY + XvpnUnitGridDy;
 
  if ( UnitGrid )
  {
    for ( X = XvpnUnitGridX; X < MaxGridX; X = X + 1 )
    {
      PixelX = ((float)(X) * XvpnUnitGridStep);
      PixelX = PixelX - XvpnPixelGridX;

      if ( ( PixelX <= GraphicX2 ) &&
           ( PixelX >= GraphicX1 ) )

      for ( Y = XvpnUnitGridY; Y < MaxGridY; Y = Y + 1 )
      {
        PixelY = ((float)(Y) * XvpnUnitGridStep);
        PixelY = PixelY - XvpnPixelGridY;
        PixelY = XvpnGraphicDy - PixelY;

        if ( ( PixelY <= GraphicY2 ) &&
             ( PixelY >= GraphicY1 ) )
        {
          XDrawPoint( XvpnGraphicDisplay,
                      XtWindow( XvpnGraphicWindow ),
                      XvpnGridGC,
                      PixelX, PixelY );
        }
      }
    }
  }

  if ( UserGrid )
  {
    Xmin  = ( XvpnUnitGridX / XvpnUnitUserGridDx ) * XvpnUnitUserGridDx;
    Ymin  = ( XvpnUnitGridY / XvpnUnitUserGridDy ) * XvpnUnitUserGridDy;

    for ( X = Xmin; X < MaxGridX ; X = X + XvpnUnitUserGridDx )
    {
      PixelX = ((float)(X) * XvpnUnitGridStep);
      PixelX = PixelX - XvpnPixelGridX;

      if ( ( PixelX <= GraphicX2 ) &&
           ( PixelX >= GraphicX1 ) )

      for ( Y = Ymin; Y < MaxGridY; Y = Y + XvpnUnitUserGridDy )
      {
        PixelY = ((float)(Y) * XvpnUnitGridStep);
        PixelY = PixelY - XvpnPixelGridY;
        PixelY = XvpnGraphicDy - PixelY;

        if ( ( PixelY <= GraphicY2 ) &&
             ( PixelY >= GraphicY1 ) )
        {
          XDrawLine( XvpnGraphicDisplay,
                     XtWindow( XvpnGraphicWindow ),
                     XvpnGridGC,
                     PixelX - 2, PixelY,
                     PixelX + 2, PixelY );
                        
          XDrawLine( XvpnGraphicDisplay,
                     XtWindow( XvpnGraphicWindow ),
                     XvpnGridGC,
                     PixelX, PixelY + 2,
                     PixelX, PixelY - 2 );
        }
      }
    }
  }

  autend();
  return( XVPN_TRUE );
}
