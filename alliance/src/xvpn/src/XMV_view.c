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
# include "vex.h"
# include "vpn.h"
# include "XSB.h"
# include "XTB.h"
# include "XVP.h"
# include "XMX.h"
# include "XMV.h"

# include "XMV_view.h"

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

 xvpnzoom *XvpnHeadZoom    = (xvpnzoom *)NULL;

 int        XvpnPercentZoom  = XVPN_PERCENT_ZOOM;
 int        XvpnPercentMoveX = XVPN_PERCENT_MOVE;
 int        XvpnPercentMoveY = XVPN_PERCENT_MOVE;

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
|                        XvpnAllocZoom                        |
|                                                             |
\------------------------------------------------------------*/

xvpnzoom *XvpnAllocZoom ()

{
  return ( (xvpnzoom *) autallocheap( sizeof ( xvpnzoom )));
}

/*------------------------------------------------------------\
|                                                             |
|                        XvpnFreeZoom                         |
|                                                             |
\------------------------------------------------------------*/

void XvpnFreeZoom ( FreeZoom )

  xvpnzoom *FreeZoom;

{
  autfreeheap( (char *)FreeZoom, sizeof(FreeZoom) );
}

/*------------------------------------------------------------\
|                                                             |
|                        XvpnAddZoom                          |
|                                                             |
\------------------------------------------------------------*/
 
void XvpnAddZoom ()
 
{
  xvpnzoom *XvpnZoom;

  autbegin();
 
  XvpnZoom       = XvpnAllocZoom ();
  XvpnZoom->X    = XvpnUnitGridX;
  XvpnZoom->Y    = XvpnUnitGridY;
  XvpnZoom->DX   = XvpnUnitGridDx;
  XvpnZoom->DY   = XvpnUnitGridDy;
  XvpnZoom->NEXT = XvpnHeadZoom;
  XvpnHeadZoom   = XvpnZoom;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XvpnDelZoom                        |
|                                                             |
\------------------------------------------------------------*/

char XvpnDelZoom()

{
  xvpnzoom *XvpnZoom;

  if ( XvpnHeadZoom != (xvpnzoom *) NULL )
  {
    autbegin();

    XvpnZoom     = XvpnHeadZoom;
    XvpnHeadZoom = XvpnHeadZoom->NEXT;
    XvpnFreeZoom( XvpnZoom );

    autend();
    return( XVPN_TRUE ); 
  }

  return( XVPN_FALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                      XvpnInitializeZoom                     |
|                                                             |
\------------------------------------------------------------*/

void XvpnInitializeZoom()

{
  autbegin();

  while( XvpnDelZoom() != XVPN_FALSE );

  if ( XvpnComputeBound() == XVPN_FALSE )
  {
    XvpnInitializeUnitGrid();
  }
  else
  {
    XvpnUnitGridX  = ( XvpnBoundXmin / XVPN_UNIT ) - 1;
    XvpnUnitGridY  = ( XvpnBoundYmin / XVPN_UNIT ) - 1;
    XvpnUnitGridDx = ( XvpnBoundXmax / XVPN_UNIT );
    XvpnUnitGridDy = ( XvpnBoundYmax / XVPN_UNIT );
    XvpnUnitGridDx = XvpnUnitGridDx - XvpnUnitGridX + 1;
    XvpnUnitGridDy = XvpnUnitGridDy - XvpnUnitGridY + 1;

    XvpnComputeUnitGrid();
  }

  XvpnComputeAndDisplayMap();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XvpnZoomUndo                        |
|                                                             |
\------------------------------------------------------------*/

void XvpnZoomUndo()

{
  autbegin();

  if ( XvpnHeadZoom == (xvpnzoom *)NULL )
  {
    XvpnErrorMessage( XvpnMainWindow, "No previous zoom !" ); 
  }
  else
  {
    XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_WATCH_CURSOR );

    XvpnUnitGridX  = XvpnHeadZoom->X;
    XvpnUnitGridY  = XvpnHeadZoom->Y;
    XvpnUnitGridDx = XvpnHeadZoom->DX;
    XvpnUnitGridDy = XvpnHeadZoom->DY;

    XvpnComputeUnitGrid();
    XvpnComputeAndDisplayMap();

    XvpnDisplayFigure( 0, 0,
                        XvpnGraphicDx, XvpnGraphicDy );

    XvpnRefreshGraphicWindow( 0, 0,
                               XvpnGraphicDx, XvpnGraphicDy);

    XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_NORMAL_CURSOR );

    XvpnDelZoom();
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XvpnZoomRight                       |
|                                                             |
\------------------------------------------------------------*/

void XvpnZoomRight()

{
  long      Delta;
  long      Offset;
  long      XvpnOldPixelGridX;

  autbegin();

  XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_WATCH_CURSOR );

  Delta = ( XvpnPercentMoveX * XvpnUnitGridDx ) / 100;

  if ( Delta == 0 ) Delta = 1;

  XvpnUnitGridX   = XvpnUnitGridX + Delta;
  XvpnOldPixelGridX = XvpnPixelGridX;
  XvpnPixelGridX    = (float)(XvpnUnitGridX) * XvpnUnitGridStep;
  Offset             = XvpnPixelGridX - XvpnOldPixelGridX;

  XCopyArea( XvpnGraphicDisplay,
             XvpnGraphicPixmap,
             XvpnGraphicPixmap,
             XvpnBackgroundGC,
             Offset, 0,
             XvpnGraphicDx - Offset,
             XvpnGraphicDy,
             0, 0
           ); 

  XvpnComputeAndDisplayMap();
  XvpnDisplayFigure( XvpnGraphicDx - Offset, 0, 
                      XvpnGraphicDx, XvpnGraphicDy ); 
  XvpnRefreshGraphicWindow( 0, 0,
                             XvpnGraphicDx, XvpnGraphicDy); 

  XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XvpnZoomLeft                        |
|                                                             |
\------------------------------------------------------------*/

void XvpnZoomLeft()

{
  long      Delta;
  long      Offset;
  long      XvpnOldPixelGridX;

  autbegin();

  XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_WATCH_CURSOR );

  Delta = ( XvpnPercentMoveX * XvpnUnitGridDx ) / 100;

  if ( Delta == 0 ) Delta = 1;

  XvpnUnitGridX   = XvpnUnitGridX - Delta;
  XvpnOldPixelGridX = XvpnPixelGridX;
  XvpnPixelGridX    = (float)(XvpnUnitGridX) * XvpnUnitGridStep;
  Offset             = XvpnOldPixelGridX - XvpnPixelGridX;

  XCopyArea( XvpnGraphicDisplay,
             XvpnGraphicPixmap,
             XvpnGraphicPixmap,
             XvpnBackgroundGC,
             0, 0,
             XvpnGraphicDx - Offset,
             XvpnGraphicDy,
             Offset, 0
           ); 

  XvpnComputeAndDisplayMap();
  XvpnDisplayFigure( 0, 0, Offset, XvpnGraphicDy );
  XvpnRefreshGraphicWindow( 0, 0, XvpnGraphicDx, XvpnGraphicDy);

  XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XvpnZoomDown                       |
|                                                             |
\------------------------------------------------------------*/

void XvpnZoomDown()

{
  long      Delta;
  long      Offset;
  long      XvpnOldPixelGridY;

  autbegin();

  XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_WATCH_CURSOR );

  Delta = ( XvpnPercentMoveY * XvpnUnitGridDy ) / 100;

  if ( Delta == 0 ) Delta = 1;

  XvpnUnitGridY   = XvpnUnitGridY - Delta;
  XvpnOldPixelGridY = XvpnPixelGridY;
  XvpnPixelGridY    = (float)(XvpnUnitGridY) * XvpnUnitGridStep;
  Offset             = XvpnOldPixelGridY - XvpnPixelGridY;

  XCopyArea( XvpnGraphicDisplay,
             XvpnGraphicPixmap,
             XvpnGraphicPixmap,
             XvpnBackgroundGC,
             0, Offset,
             XvpnGraphicDx,
             XvpnGraphicDy - Offset,
             0, 0
           ); 

  XvpnComputeAndDisplayMap();
  XvpnDisplayFigure( 0, 0, XvpnGraphicDx, Offset ); 
  XvpnRefreshGraphicWindow( 0, 0, XvpnGraphicDx, XvpnGraphicDy);

  XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XvpnZoomUp                         |
|                                                             |
\------------------------------------------------------------*/

void XvpnZoomUp()

{
  long      Delta;
  long      Offset;
  long      XvpnOldPixelGridY;

  autbegin();

  XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_WATCH_CURSOR );

  Delta = ( XvpnPercentMoveY * XvpnUnitGridDy ) / 100;

  if ( Delta == 0 ) Delta = 1;

  XvpnUnitGridY   = XvpnUnitGridY + Delta;
  XvpnOldPixelGridY = XvpnPixelGridY;
  XvpnPixelGridY    = (float)(XvpnUnitGridY) * XvpnUnitGridStep;
  Offset             = XvpnPixelGridY - XvpnOldPixelGridY;

  XCopyArea( XvpnGraphicDisplay,
             XvpnGraphicPixmap,
             XvpnGraphicPixmap,
             XvpnBackgroundGC,
             0, 0,
             XvpnGraphicDx,
             XvpnGraphicDy - Offset,
             0, Offset
           ); 

  XvpnComputeAndDisplayMap();
  XvpnDisplayFigure( 0, XvpnGraphicDy - Offset,
                      XvpnGraphicDx, XvpnGraphicDy ); 
  XvpnRefreshGraphicWindow( 0, 0, XvpnGraphicDx, XvpnGraphicDy);

  XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XvpnZoomMore                        |
|                                                             |
\------------------------------------------------------------*/

void XvpnZoomMore()

{
  long DeltaX;
  long DeltaY;

  autbegin();

  XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_WATCH_CURSOR );

  DeltaX = ( XvpnPercentZoom * XvpnUnitGridDx ) / 100;
  DeltaY = ( XvpnPercentZoom * XvpnUnitGridDy ) / 100;

  if ( ( DeltaX >= 2 ) &&
       ( DeltaY >= 2 ) )
  {
    XvpnAddZoom();

    XvpnUnitGridX  = XvpnUnitGridX + (DeltaX >> 1);
    XvpnUnitGridY  = XvpnUnitGridY + (DeltaY >> 1);
    XvpnUnitGridDx = XvpnUnitGridDx - DeltaX;
    XvpnUnitGridDy = XvpnUnitGridDy - DeltaY;

    XvpnComputeUnitGrid();
    XvpnComputeAndDisplayMap();
    XvpnDisplayFigure( 0, 0, XvpnGraphicDx, XvpnGraphicDy );
    XvpnRefreshGraphicWindow( 0, 0, XvpnGraphicDx, XvpnGraphicDy );
  }

  XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XvpnZoomLess                        |
|                                                             |
\------------------------------------------------------------*/

void XvpnZoomLess()

{
  long DeltaX;
  long DeltaY;

  autbegin();

  XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_WATCH_CURSOR );

  DeltaX = 100 * XvpnUnitGridDx / ( 100 - XvpnPercentZoom );
  DeltaY = 100 * XvpnUnitGridDy / ( 100 - XvpnPercentZoom );

  XvpnAddZoom();
 
  XvpnUnitGridX  = XvpnUnitGridX - ((DeltaX - XvpnUnitGridDx) >> 1);
  XvpnUnitGridY  = XvpnUnitGridY - ((DeltaY - XvpnUnitGridDy) >> 1);
  XvpnUnitGridDx = DeltaX;
  XvpnUnitGridDy = DeltaY;

  XvpnComputeUnitGrid();
  XvpnComputeAndDisplayMap();
  XvpnDisplayFigure( 0, 0, XvpnGraphicDx, XvpnGraphicDy );
  XvpnRefreshGraphicWindow( 0, 0, XvpnGraphicDx, XvpnGraphicDy );

  XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XvpnZoomFit                         |
|                                                             |
\------------------------------------------------------------*/

void XvpnZoomFit()

{
  autbegin();

  XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_WATCH_CURSOR );

  if ( XvpnComputeBound() == XVPN_FALSE )
  {
    XvpnErrorMessage( XvpnMainWindow, "No element to display !" );
  }
  else
  {
    XvpnAddZoom();

    XvpnUnitGridX  = ( XvpnBoundXmin / XVPN_UNIT ) - 1;
    XvpnUnitGridY  = ( XvpnBoundYmin / XVPN_UNIT ) - 1;
    XvpnUnitGridDx = XvpnBoundXmax / XVPN_UNIT;
    XvpnUnitGridDy = XvpnBoundYmax / XVPN_UNIT;
    XvpnUnitGridDx = XvpnUnitGridDx - XvpnUnitGridX + 1;
    XvpnUnitGridDy = XvpnUnitGridDy - XvpnUnitGridY + 1;

    XvpnComputeUnitGrid();
    XvpnComputeAndDisplayMap();
    XvpnDisplayFigure( 0, 0, XvpnGraphicDx, XvpnGraphicDy );
    XvpnRefreshGraphicWindow( 0, 0, XvpnGraphicDx, XvpnGraphicDy );
  }

  XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XvpnZoomCenter                      |
|                                                             |
\------------------------------------------------------------*/

void XvpnZoomCenter( UnitX, UnitY )

   long UnitX;
   long UnitY;
{
  autbegin();

  XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_WATCH_CURSOR );

  XvpnUnitGridX = UnitX - ( XvpnUnitGridDx >> 1 );
  XvpnUnitGridY = UnitY - ( XvpnUnitGridDy >> 1 );

  XvpnComputeUnitGrid();
  XvpnComputeAndDisplayMap();
  XvpnDisplayFigure( 0, 0, XvpnGraphicDx, XvpnGraphicDy );
  XvpnRefreshGraphicWindow( 0, 0, XvpnGraphicDx, XvpnGraphicDy );

  XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XvpnZoomIn                         |
|                                                             |
\------------------------------------------------------------*/
 
void XvpnZoomIn( UnitX1, UnitY1, UnitX2, UnitY2 )
 
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

    XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_WATCH_CURSOR );

    XvpnAddZoom();

    XvpnUnitGridX  = UnitX1;
    XvpnUnitGridY  = UnitY1;
    XvpnUnitGridDx = UnitX2 - UnitX1;
    XvpnUnitGridDy = UnitY2 - UnitY1;

    XvpnComputeUnitGrid();
    XvpnComputeAndDisplayMap();
    XvpnDisplayFigure( 0, 0, XvpnGraphicDx, XvpnGraphicDy );
    XvpnRefreshGraphicWindow( 0, 0, XvpnGraphicDx, XvpnGraphicDy );

    XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_NORMAL_CURSOR );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XvpnZoomPan                        |
|                                                             |
\------------------------------------------------------------*/

void XvpnZoomPan( UnitX1, UnitY1, UnitX2, UnitY2 )

   long UnitX1;
   long UnitY1;
   long UnitX2;
   long UnitY2;
{
  if ( ( UnitX1 != UnitX2 ) ||
       ( UnitY1 != UnitY2 ) )
  {
    autbegin();

    XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_WATCH_CURSOR );
 
    XvpnUnitGridX  = XvpnUnitGridX + ( UnitX1 - UnitX2 );
    XvpnUnitGridY  = XvpnUnitGridY + ( UnitY1 - UnitY2 );

    XvpnComputeUnitGrid();
    XvpnComputeAndDisplayMap();
    XvpnDisplayFigure( 0, 0, XvpnGraphicDx, XvpnGraphicDy );
    XvpnRefreshGraphicWindow( 0, 0, XvpnGraphicDx, XvpnGraphicDy );

    XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_NORMAL_CURSOR );

    autend();
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          XvpnZoomRefresh                    |
|                                                             |
\------------------------------------------------------------*/

void XvpnZoomRefresh()

{
  autbegin();

  XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_WATCH_CURSOR );

  XvpnDisplayFigure( 0, 0, XvpnGraphicDx, XvpnGraphicDy );
  XvpnRefreshGraphicWindow( 0, 0, XvpnGraphicDx, XvpnGraphicDy );

  XvpnSetMouseCursor( XvpnGraphicWindow, XVPN_NORMAL_CURSOR );

  autend();
}
