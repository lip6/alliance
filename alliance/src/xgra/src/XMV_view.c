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
| File    :                   View.c                          |
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
# include "XTB.h"
# include "XGR.h"
# include "XMX.h"
# include "XMV.h"

# include "XMV_view.h"
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

 xgrazoom *XgraHeadZoom    = (xgrazoom *)NULL;

 int        XgraPercentZoom  = XGRA_PERCENT_ZOOM;
 int        XgraPercentMoveX = XGRA_PERCENT_MOVE;
 int        XgraPercentMoveY = XGRA_PERCENT_MOVE;

/*------------------------------------------------------------\
|                                                             |
|                            Menu                             |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        XgraAllocZoom                        |
|                                                             |
\------------------------------------------------------------*/

xgrazoom *XgraAllocZoom ()

{
  return ( (xgrazoom *) autalloc( sizeof ( xgrazoom ), 1));
}

/*------------------------------------------------------------\
|                                                             |
|                        XgraFreeZoom                         |
|                                                             |
\------------------------------------------------------------*/

void XgraFreeZoom ( FreeZoom )

  xgrazoom *FreeZoom;

{
  autfree( (char *)FreeZoom, sizeof(FreeZoom) );
}

/*------------------------------------------------------------\
|                                                             |
|                        XgraAddZoom                          |
|                                                             |
\------------------------------------------------------------*/
 
void XgraAddZoom ()
 
{
  xgrazoom *XgraZoom;

  autbegin();
 
  XgraZoom       = XgraAllocZoom ();
  XgraZoom->X    = XgraUnitGridX;
  XgraZoom->Y    = XgraUnitGridY;
  XgraZoom->DX   = XgraUnitGridDx;
  XgraZoom->DY   = XgraUnitGridDy;
  XgraZoom->NEXT = XgraHeadZoom;
  XgraHeadZoom   = XgraZoom;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XgraDelZoom                        |
|                                                             |
\------------------------------------------------------------*/

char XgraDelZoom()

{
  xgrazoom *XgraZoom;

  if ( XgraHeadZoom != (xgrazoom *) NULL )
  {
    autbegin();

    XgraZoom     = XgraHeadZoom;
    XgraHeadZoom = XgraHeadZoom->NEXT;
    XgraFreeZoom( XgraZoom );

    autend();
    return( XGRA_TRUE ); 
  }

  return( XGRA_FALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                      XgraInitializeZoom                     |
|                                                             |
\------------------------------------------------------------*/

void XgraInitializeZoom()

{
  autbegin();

  while( XgraDelZoom() != XGRA_FALSE );

  if ( XgraComputeBound() == XGRA_FALSE )
  {
    XgraInitializeUnitGrid();
  }
  else
  {
    XgraUnitGridX  = ( XgraBoundXmin / XGRA_UNIT ) - 1;
    XgraUnitGridY  = ( XgraBoundYmin / XGRA_UNIT ) - 1;
    XgraUnitGridDx = ( XgraBoundXmax / XGRA_UNIT );
    XgraUnitGridDy = ( XgraBoundYmax / XGRA_UNIT );
    XgraUnitGridDx = XgraUnitGridDx - XgraUnitGridX + 1;
    XgraUnitGridDy = XgraUnitGridDy - XgraUnitGridY + 1;

    XgraComputeUnitGrid();
  }

  XgraComputeAndDisplayMap();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XgraZoomUndo                        |
|                                                             |
\------------------------------------------------------------*/

void XgraZoomUndo()

{
  autbegin();

  if ( XgraHeadZoom == (xgrazoom *)NULL )
  {
    XgraErrorMessage( XgraMainWindow, "No previous zoom !" ); 
  }
  else
  {
    XgraSetMouseCursor( XgraGraphicWindow, XGRA_WATCH_CURSOR );

    XgraUnitGridX  = XgraHeadZoom->X;
    XgraUnitGridY  = XgraHeadZoom->Y;
    XgraUnitGridDx = XgraHeadZoom->DX;
    XgraUnitGridDy = XgraHeadZoom->DY;

    XgraComputeUnitGrid();
    XgraComputeAndDisplayMap();

    XgraDisplayFigure( 0, 0,
                        XgraGraphicDx, XgraGraphicDy );

    XgraRefreshGraphicWindow( 0, 0,
                               XgraGraphicDx, XgraGraphicDy);

    XgraSetMouseCursor( XgraGraphicWindow, XGRA_NORMAL_CURSOR );

    XgraDelZoom();
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XgraZoomRight                       |
|                                                             |
\------------------------------------------------------------*/

void XgraZoomRight()

{
  long      Delta;
  long      Offset;
  long      XgraOldPixelGridX;

  autbegin();

  XgraSetMouseCursor( XgraGraphicWindow, XGRA_WATCH_CURSOR );

  Delta = ( XgraPercentMoveX * XgraUnitGridDx ) / 100;

  if ( Delta == 0 ) Delta = 1;

  XgraUnitGridX   = XgraUnitGridX + Delta;
  XgraOldPixelGridX = XgraPixelGridX;
  XgraPixelGridX    = (float)(XgraUnitGridX) * XgraUnitGridStep;
  Offset             = XgraPixelGridX - XgraOldPixelGridX;

  XCopyArea( XgraGraphicDisplay,
             XgraGraphicPixmap,
             XgraGraphicPixmap,
             XgraBackgroundGC,
             Offset, 0,
             XgraGraphicDx - Offset,
             XgraGraphicDy,
             0, 0
           ); 

  XgraComputeAndDisplayMap();
  XgraDisplayFigure( XgraGraphicDx - Offset, 0, 
                      XgraGraphicDx, XgraGraphicDy ); 
  XgraRefreshGraphicWindow( 0, 0,
                             XgraGraphicDx, XgraGraphicDy); 

  XgraSetMouseCursor( XgraGraphicWindow, XGRA_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XgraZoomLeft                        |
|                                                             |
\------------------------------------------------------------*/

void XgraZoomLeft()

{
  long      Delta;
  long      Offset;
  long      XgraOldPixelGridX;

  autbegin();

  XgraSetMouseCursor( XgraGraphicWindow, XGRA_WATCH_CURSOR );

  Delta = ( XgraPercentMoveX * XgraUnitGridDx ) / 100;

  if ( Delta == 0 ) Delta = 1;

  XgraUnitGridX   = XgraUnitGridX - Delta;
  XgraOldPixelGridX = XgraPixelGridX;
  XgraPixelGridX    = (float)(XgraUnitGridX) * XgraUnitGridStep;
  Offset             = XgraOldPixelGridX - XgraPixelGridX;

  XCopyArea( XgraGraphicDisplay,
             XgraGraphicPixmap,
             XgraGraphicPixmap,
             XgraBackgroundGC,
             0, 0,
             XgraGraphicDx - Offset,
             XgraGraphicDy,
             Offset, 0
           ); 

  XgraComputeAndDisplayMap();
  XgraDisplayFigure( 0, 0, Offset, XgraGraphicDy );
  XgraRefreshGraphicWindow( 0, 0, XgraGraphicDx, XgraGraphicDy);

  XgraSetMouseCursor( XgraGraphicWindow, XGRA_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XgraZoomDown                       |
|                                                             |
\------------------------------------------------------------*/

void XgraZoomDown()

{
  long      Delta;
  long      Offset;
  long      XgraOldPixelGridY;

  autbegin();

  XgraSetMouseCursor( XgraGraphicWindow, XGRA_WATCH_CURSOR );

  Delta = ( XgraPercentMoveY * XgraUnitGridDy ) / 100;

  if ( Delta == 0 ) Delta = 1;

  XgraUnitGridY   = XgraUnitGridY - Delta;
  XgraOldPixelGridY = XgraPixelGridY;
  XgraPixelGridY    = (float)(XgraUnitGridY) * XgraUnitGridStep;
  Offset             = XgraOldPixelGridY - XgraPixelGridY;

  XCopyArea( XgraGraphicDisplay,
             XgraGraphicPixmap,
             XgraGraphicPixmap,
             XgraBackgroundGC,
             0, Offset,
             XgraGraphicDx,
             XgraGraphicDy - Offset,
             0, 0
           ); 

  XgraComputeAndDisplayMap();
  XgraDisplayFigure( 0, 0, XgraGraphicDx, Offset ); 
  XgraRefreshGraphicWindow( 0, 0, XgraGraphicDx, XgraGraphicDy);

  XgraSetMouseCursor( XgraGraphicWindow, XGRA_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XgraZoomUp                         |
|                                                             |
\------------------------------------------------------------*/

void XgraZoomUp()

{
  long      Delta;
  long      Offset;
  long      XgraOldPixelGridY;

  autbegin();

  XgraSetMouseCursor( XgraGraphicWindow, XGRA_WATCH_CURSOR );

  Delta = ( XgraPercentMoveY * XgraUnitGridDy ) / 100;

  if ( Delta == 0 ) Delta = 1;

  XgraUnitGridY   = XgraUnitGridY + Delta;
  XgraOldPixelGridY = XgraPixelGridY;
  XgraPixelGridY    = (float)(XgraUnitGridY) * XgraUnitGridStep;
  Offset             = XgraPixelGridY - XgraOldPixelGridY;

  XCopyArea( XgraGraphicDisplay,
             XgraGraphicPixmap,
             XgraGraphicPixmap,
             XgraBackgroundGC,
             0, 0,
             XgraGraphicDx,
             XgraGraphicDy - Offset,
             0, Offset
           ); 

  XgraComputeAndDisplayMap();
  XgraDisplayFigure( 0, XgraGraphicDy - Offset,
                      XgraGraphicDx, XgraGraphicDy ); 
  XgraRefreshGraphicWindow( 0, 0, XgraGraphicDx, XgraGraphicDy);

  XgraSetMouseCursor( XgraGraphicWindow, XGRA_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XgraZoomMore                        |
|                                                             |
\------------------------------------------------------------*/

void XgraZoomMore()

{
  long DeltaX;
  long DeltaY;

  autbegin();

  XgraSetMouseCursor( XgraGraphicWindow, XGRA_WATCH_CURSOR );

  DeltaX = ( XgraPercentZoom * XgraUnitGridDx ) / 100;
  DeltaY = ( XgraPercentZoom * XgraUnitGridDy ) / 100;

  if ( ( DeltaX >= 2 ) &&
       ( DeltaY >= 2 ) )
  {
    XgraAddZoom();

    XgraUnitGridX  = XgraUnitGridX + (DeltaX >> 1);
    XgraUnitGridY  = XgraUnitGridY + (DeltaY >> 1);
    XgraUnitGridDx = XgraUnitGridDx - DeltaX;
    XgraUnitGridDy = XgraUnitGridDy - DeltaY;

    XgraComputeUnitGrid();
    XgraComputeAndDisplayMap();
    XgraDisplayFigure( 0, 0, XgraGraphicDx, XgraGraphicDy );
    XgraRefreshGraphicWindow( 0, 0, XgraGraphicDx, XgraGraphicDy );
  }

  XgraSetMouseCursor( XgraGraphicWindow, XGRA_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XgraZoomLess                        |
|                                                             |
\------------------------------------------------------------*/

void XgraZoomLess()

{
  long DeltaX;
  long DeltaY;

  autbegin();

  XgraSetMouseCursor( XgraGraphicWindow, XGRA_WATCH_CURSOR );

  DeltaX = 100 * XgraUnitGridDx / ( 100 - XgraPercentZoom );
  DeltaY = 100 * XgraUnitGridDy / ( 100 - XgraPercentZoom );

  XgraAddZoom();
 
  XgraUnitGridX  = XgraUnitGridX - ((DeltaX - XgraUnitGridDx) >> 1);
  XgraUnitGridY  = XgraUnitGridY - ((DeltaY - XgraUnitGridDy) >> 1);
  XgraUnitGridDx = DeltaX;
  XgraUnitGridDy = DeltaY;

  XgraComputeUnitGrid();
  XgraComputeAndDisplayMap();
  XgraDisplayFigure( 0, 0, XgraGraphicDx, XgraGraphicDy );
  XgraRefreshGraphicWindow( 0, 0, XgraGraphicDx, XgraGraphicDy );

  XgraSetMouseCursor( XgraGraphicWindow, XGRA_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XgraZoomFit                         |
|                                                             |
\------------------------------------------------------------*/

void XgraZoomFit()

{
  autbegin();

  XgraSetMouseCursor( XgraGraphicWindow, XGRA_WATCH_CURSOR );

  if ( XgraComputeBound() == XGRA_FALSE )
  {
    XgraErrorMessage( XgraMainWindow, "No element to display !" );
  }
  else
  {
    XgraAddZoom();

    XgraUnitGridX  = ( XgraBoundXmin / XGRA_UNIT ) - 1;
    XgraUnitGridY  = ( XgraBoundYmin / XGRA_UNIT ) - 1;
    XgraUnitGridDx = XgraBoundXmax / XGRA_UNIT;
    XgraUnitGridDy = XgraBoundYmax / XGRA_UNIT;
    XgraUnitGridDx = XgraUnitGridDx - XgraUnitGridX + 1;
    XgraUnitGridDy = XgraUnitGridDy - XgraUnitGridY + 1;

    XgraComputeUnitGrid();
    XgraComputeAndDisplayMap();
    XgraDisplayFigure( 0, 0, XgraGraphicDx, XgraGraphicDy );
    XgraRefreshGraphicWindow( 0, 0, XgraGraphicDx, XgraGraphicDy );
  }

  XgraSetMouseCursor( XgraGraphicWindow, XGRA_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XgraZoomCenter                      |
|                                                             |
\------------------------------------------------------------*/

void XgraZoomCenter( UnitX, UnitY )

   long UnitX;
   long UnitY;
{
  autbegin();

  XgraSetMouseCursor( XgraGraphicWindow, XGRA_WATCH_CURSOR );

  XgraUnitGridX = UnitX - ( XgraUnitGridDx >> 1 );
  XgraUnitGridY = UnitY - ( XgraUnitGridDy >> 1 );

  XgraComputeUnitGrid();
  XgraComputeAndDisplayMap();
  XgraDisplayFigure( 0, 0, XgraGraphicDx, XgraGraphicDy );
  XgraRefreshGraphicWindow( 0, 0, XgraGraphicDx, XgraGraphicDy );

  XgraSetMouseCursor( XgraGraphicWindow, XGRA_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XgraZoomIn                         |
|                                                             |
\------------------------------------------------------------*/
 
void XgraZoomIn( UnitX1, UnitY1, UnitX2, UnitY2 )
 
   long UnitX1;
   long UnitY1;
   long UnitX2;
   long UnitY2;
{
  long Swap;

  autbegin();

  if ( ( UnitX1 != UnitX2 ) && 
       ( UnitY1 != UnitY2 ) )
  {
    if ( UnitX1 > UnitX2 ) 
    {
      Swap = UnitX1; UnitX1 = UnitX2; UnitX2 = Swap;
    }

    if ( UnitY1 > UnitY2 ) 
    {
      Swap = UnitY1; UnitY1 = UnitY2; UnitY2 = Swap;
    }

    XgraSetMouseCursor( XgraGraphicWindow, XGRA_WATCH_CURSOR );

    XgraAddZoom();

    XgraUnitGridX  = UnitX1;
    XgraUnitGridY  = UnitY1;
    XgraUnitGridDx = UnitX2 - UnitX1;
    XgraUnitGridDy = UnitY2 - UnitY1;

    XgraComputeUnitGrid();
    XgraComputeAndDisplayMap();
    XgraDisplayFigure( 0, 0, XgraGraphicDx, XgraGraphicDy );
    XgraRefreshGraphicWindow( 0, 0, XgraGraphicDx, XgraGraphicDy );

    XgraSetMouseCursor( XgraGraphicWindow, XGRA_NORMAL_CURSOR );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XgraZoomPan                        |
|                                                             |
\------------------------------------------------------------*/

void XgraZoomPan( UnitX1, UnitY1, UnitX2, UnitY2 )

   long UnitX1;
   long UnitY1;
   long UnitX2;
   long UnitY2;
{
  if ( ( UnitX1 != UnitX2 ) ||
       ( UnitY1 != UnitY2 ) )
  {
    autbegin();

    XgraSetMouseCursor( XgraGraphicWindow, XGRA_WATCH_CURSOR );
 
    XgraUnitGridX  = XgraUnitGridX + ( UnitX1 - UnitX2 );
    XgraUnitGridY  = XgraUnitGridY + ( UnitY1 - UnitY2 );

    XgraComputeUnitGrid();
    XgraComputeAndDisplayMap();
    XgraDisplayFigure( 0, 0, XgraGraphicDx, XgraGraphicDy );
    XgraRefreshGraphicWindow( 0, 0, XgraGraphicDx, XgraGraphicDy );

    XgraSetMouseCursor( XgraGraphicWindow, XGRA_NORMAL_CURSOR );

    autend();
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          XgraZoomRefresh                    |
|                                                             |
\------------------------------------------------------------*/

void XgraZoomRefresh()

{
  autbegin();

  XgraSetMouseCursor( XgraGraphicWindow, XGRA_WATCH_CURSOR );

  XgraDisplayFigure( 0, 0, XgraGraphicDx, XgraGraphicDy );
  XgraRefreshGraphicWindow( 0, 0, XgraGraphicDx, XgraGraphicDy );

  XgraSetMouseCursor( XgraGraphicWindow, XGRA_NORMAL_CURSOR );

  autend();
}
