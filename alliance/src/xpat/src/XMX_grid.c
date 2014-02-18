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
| Tool    :                   XPAT                            |
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
# include "pat.h"
# include "XSB.h"
# include "XPT.h"
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

  float      XpatUnitGridStep;

  long       XpatUnitGridX;
  long       XpatUnitGridY;
  long       XpatUnitGridDx;
  long       XpatUnitGridDy;

  long       XpatPixelGridX;
  long       XpatPixelGridY;

/*------------------------------------------------------------\
|                                                             |
|                        User Unit Grid                       |
|                                                             |
\------------------------------------------------------------*/

  long       XpatUnitUserGridDx;
  long       XpatUnitUserGridDy;
  char       XpatUnitUserGrid;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  XpatInitializeUnitGrid                     |
|                                                             |
\------------------------------------------------------------*/

void XpatInitializeUnitGrid()

{
  autbegin();

  XpatUnitGridX      = XPAT_DEFAULT_GRID_X;
  XpatUnitGridY      = XPAT_DEFAULT_GRID_Y;
  XpatUnitGridDx     = XPAT_DEFAULT_GRID_DX;
  XpatUnitGridDy     = XPAT_DEFAULT_GRID_DY;
  XpatUnitUserGridDx = 4;
  XpatUnitUserGridDy = 4;
  XpatUnitUserGrid   = XPAT_FALSE;

  XpatComputeUnitGrid();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                   XpatComputeUnitGrid                      |
|                                                             |
\------------------------------------------------------------*/

void XpatComputeUnitGrid()

{
  float StepX;
  float StepY;

  autbegin();

  StepX = (float)(XpatGraphicDx) / (float)(XpatUnitGridDx);
  StepY = (float)(XpatGraphicDy) / (float)(XpatUnitGridDy);

  if ( StepX < StepY )
  {
    XpatUnitGridStep = StepX;
    XpatUnitGridDy   = 1 + ( XpatGraphicDy / StepX );
  }
  else
  {
    XpatUnitGridStep = StepY;
    XpatUnitGridDx   = 1 + ( XpatGraphicDx / StepY );
  }

  XpatPixelGridX = (float)(XpatUnitGridX) * XpatUnitGridStep;
  XpatPixelGridY = (float)(XpatUnitGridY) * XpatUnitGridStep;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XpatResizeUnitGrid                       |
|                                                             |
\------------------------------------------------------------*/

void XpatResizeUnitGrid()

{
  autbegin();

  XpatUnitGridDx = 1 + ( XpatGraphicDx / XpatUnitGridStep );
  XpatUnitGridDy = 1 + ( XpatGraphicDy / XpatUnitGridStep );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XpatDisplayUnitGrid                  |
|                                                             |
\------------------------------------------------------------*/

char XpatDisplayUnitGrid( GraphicX1, GraphicY1, GraphicX2, GraphicY2 )

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

  if ( XpatUnitGridStep < XPAT_LOWER_GRID_STEP ) 
  {
    UnitGrid = 0;
  }
  else 
  {
    UnitGrid = 1;
  }

  if ( XpatUnitUserGrid == 1  )
  {
    Check = ( XPAT_LOWER_GRID_STEP * 2 / XpatUnitUserGridDy );
#if THIS_IS_DISABLED
    if ( XpatUnitUserGridDx > XpatUnitUserGridDy )
    {
      Check = ( XPAT_LOWER_GRID_STEP * 2 / XpatUnitUserGridDy );
    }
    else
    {
      Check = ( XPAT_LOWER_GRID_STEP * 2 / XpatUnitUserGridDy );
    }
#endif

    if ( XpatUnitGridStep < Check ) UserGrid = 0;
    else                            UserGrid = 1;
  } 
  else
  {
    UserGrid = 0;
  }

  if ( ( UserGrid   == 0 ) &&
       ( UnitGrid == 0 ) )
  {
    autend();
    return( XPAT_FALSE );
  }

  MaxGridX = XpatUnitGridX + XpatUnitGridDx;
  MaxGridY = XpatUnitGridY + XpatUnitGridDy;
 
  if ( UnitGrid )
  {
    for ( X = XpatUnitGridX; X < MaxGridX; X = X + 1 )
    {
      PixelX = ((float)(X) * XpatUnitGridStep);
      PixelX = PixelX - XpatPixelGridX;

      if ( ( PixelX <= GraphicX2 ) &&
           ( PixelX >= GraphicX1 ) )

      for ( Y = XpatUnitGridY; Y < MaxGridY; Y = Y + 1 )
      {
        PixelY = ((float)(Y) * XpatUnitGridStep);
        PixelY = PixelY - XpatPixelGridY;
        PixelY = XpatGraphicDy - PixelY;

        if ( ( PixelY <= GraphicY2 ) &&
             ( PixelY >= GraphicY1 ) )
        {
          XDrawPoint( XpatGraphicDisplay,
                      XtWindow( XpatGraphicWindow ),
                      XpatGridGC,
                      PixelX, PixelY );
        }
      }
    }
  }

  if ( UserGrid )
  {
    Xmin  = ( XpatUnitGridX / XpatUnitUserGridDx ) * XpatUnitUserGridDx;
    Ymin  = ( XpatUnitGridY / XpatUnitUserGridDy ) * XpatUnitUserGridDy;

    for ( X = Xmin; X < MaxGridX ; X = X + XpatUnitUserGridDx )
    {
      PixelX = ((float)(X) * XpatUnitGridStep);
      PixelX = PixelX - XpatPixelGridX;

      if ( ( PixelX <= GraphicX2 ) &&
           ( PixelX >= GraphicX1 ) )

      for ( Y = Ymin; Y < MaxGridY; Y = Y + XpatUnitUserGridDy )
      {
        PixelY = ((float)(Y) * XpatUnitGridStep);
        PixelY = PixelY - XpatPixelGridY;
        PixelY = XpatGraphicDy - PixelY;

        if ( ( PixelY <= GraphicY2 ) &&
             ( PixelY >= GraphicY1 ) )
        {
          XDrawLine( XpatGraphicDisplay,
                     XtWindow( XpatGraphicWindow ),
                     XpatGridGC,
                     PixelX - 2, PixelY,
                     PixelX + 2, PixelY );
                        
          XDrawLine( XpatGraphicDisplay,
                     XtWindow( XpatGraphicWindow ),
                     XpatGridGC,
                     PixelX, PixelY + 2,
                     PixelX, PixelY - 2 );
        }
      }
    }
  }

  autend();
  return( XPAT_TRUE );
}
