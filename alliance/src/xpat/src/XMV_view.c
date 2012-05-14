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
# include "pat.h"
# include "XSB.h"
# include "XPT.h"
# include "XTB.h"
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

 xpatzoom *XpatHeadZoom    = (xpatzoom *)NULL;

 int        XpatPercentZoom  = XPAT_PERCENT_ZOOM;
 int        XpatPercentMoveX = XPAT_PERCENT_MOVE;
 int        XpatPercentMoveY = XPAT_PERCENT_MOVE;

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
|                        XpatAllocZoom                        |
|                                                             |
\------------------------------------------------------------*/

xpatzoom *XpatAllocZoom ()

{
  return ( (xpatzoom *) autalloc( sizeof ( xpatzoom ), 1));
}

/*------------------------------------------------------------\
|                                                             |
|                        XpatFreeZoom                         |
|                                                             |
\------------------------------------------------------------*/

void XpatFreeZoom ( FreeZoom )

  xpatzoom *FreeZoom;

{
  autfree( (char *)FreeZoom, sizeof(FreeZoom) );
}

/*------------------------------------------------------------\
|                                                             |
|                        XpatAddZoom                          |
|                                                             |
\------------------------------------------------------------*/
 
void XpatAddZoom ()
 
{
  xpatzoom *XpatZoom;

  autbegin();
 
  XpatZoom       = XpatAllocZoom ();
  XpatZoom->X    = XpatUnitGridX;
  XpatZoom->Y    = XpatUnitGridY;
  XpatZoom->DX   = XpatUnitGridDx;
  XpatZoom->DY   = XpatUnitGridDy;
  XpatZoom->NEXT = XpatHeadZoom;
  XpatHeadZoom   = XpatZoom;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XpatDelZoom                        |
|                                                             |
\------------------------------------------------------------*/

char XpatDelZoom()

{
  xpatzoom *XpatZoom;

  if ( XpatHeadZoom != (xpatzoom *) NULL )
  {
    autbegin();

    XpatZoom     = XpatHeadZoom;
    XpatHeadZoom = XpatHeadZoom->NEXT;
    XpatFreeZoom( XpatZoom );

    autend();
    return( XPAT_TRUE ); 
  }

  return( XPAT_FALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                      XpatInitializeZoom                     |
|                                                             |
\------------------------------------------------------------*/

void XpatInitializeZoom()

{
  autbegin();

  while( XpatDelZoom() != XPAT_FALSE );

  if ( XpatComputeBound() == XPAT_FALSE )
  {
    XpatInitializeUnitGrid();
  }
  else
  {
    XpatZoomFit();

    while( XpatDelZoom() != XPAT_FALSE );
  }

  XpatComputeAndDisplayMap();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XpatZoomUndo                        |
|                                                             |
\------------------------------------------------------------*/

void XpatZoomUndo()

{
  autbegin();

  if ( XpatHeadZoom == (xpatzoom *)NULL )
  {
    XpatErrorMessage( XpatMainWindow, "No previous zoom !" ); 
  }
  else
  {
    XpatSetMouseCursor( XpatGraphicWindow, XPAT_WATCH_CURSOR );

    XpatUnitGridX  = XpatHeadZoom->X;
    XpatUnitGridY  = XpatHeadZoom->Y;
    XpatUnitGridDx = XpatHeadZoom->DX;
    XpatUnitGridDy = XpatHeadZoom->DY;

    XpatComputeUnitGrid();
    XpatComputeAndDisplayMap();

    XpatDisplayFigure( 0, 0,
                        XpatGraphicDx, XpatGraphicDy );

    XpatRefreshGraphicWindow( 0, 0,
                               XpatGraphicDx, XpatGraphicDy);

    XpatSetMouseCursor( XpatGraphicWindow, XPAT_NORMAL_CURSOR );

    XpatDelZoom();
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XpatZoomRight                       |
|                                                             |
\------------------------------------------------------------*/

void XpatZoomRight()

{
  long      Delta;
  long      Offset;
  long      XpatOldPixelGridX;

  autbegin();

  XpatSetMouseCursor( XpatGraphicWindow, XPAT_WATCH_CURSOR );

  Delta = ( XpatPercentMoveX * XpatUnitGridDx ) / 100;

  if ( Delta == 0 ) Delta = 1;

  XpatUnitGridX   = XpatUnitGridX + Delta;
  XpatOldPixelGridX = XpatPixelGridX;
  XpatPixelGridX    = (float)(XpatUnitGridX) * XpatUnitGridStep;
  Offset             = XpatPixelGridX - XpatOldPixelGridX;

  XCopyArea( XpatGraphicDisplay,
             XpatGraphicPixmap,
             XpatGraphicPixmap,
             XpatBackgroundGC,
             Offset, 0,
             XpatGraphicDx - Offset,
             XpatGraphicDy,
             0, 0
           ); 

  XpatComputeAndDisplayMap();
  XpatDisplayFigure( XpatGraphicDx - Offset, 0, 
                      XpatGraphicDx, XpatGraphicDy ); 
  XpatRefreshGraphicWindow( 0, 0,
                             XpatGraphicDx, XpatGraphicDy); 

  XpatSetMouseCursor( XpatGraphicWindow, XPAT_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XpatZoomLeft                        |
|                                                             |
\------------------------------------------------------------*/

void XpatZoomLeft()

{
  long      Delta;
  long      Offset;
  long      XpatOldPixelGridX;

  autbegin();

  XpatSetMouseCursor( XpatGraphicWindow, XPAT_WATCH_CURSOR );

  Delta = ( XpatPercentMoveX * XpatUnitGridDx ) / 100;

  if ( Delta == 0 ) Delta = 1;

  XpatUnitGridX   = XpatUnitGridX - Delta;
  XpatOldPixelGridX = XpatPixelGridX;
  XpatPixelGridX    = (float)(XpatUnitGridX) * XpatUnitGridStep;
  Offset             = XpatOldPixelGridX - XpatPixelGridX;

  XCopyArea( XpatGraphicDisplay,
             XpatGraphicPixmap,
             XpatGraphicPixmap,
             XpatBackgroundGC,
             0, 0,
             XpatGraphicDx - Offset,
             XpatGraphicDy,
             Offset, 0
           ); 

  XpatComputeAndDisplayMap();
  XpatDisplayFigure( 0, 0, Offset, XpatGraphicDy );
  XpatRefreshGraphicWindow( 0, 0, XpatGraphicDx, XpatGraphicDy);

  XpatSetMouseCursor( XpatGraphicWindow, XPAT_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XpatZoomDown                       |
|                                                             |
\------------------------------------------------------------*/

void XpatZoomDown()

{
  long      Delta;
  long      Offset;
  long      XpatOldPixelGridY;

  autbegin();

  XpatSetMouseCursor( XpatGraphicWindow, XPAT_WATCH_CURSOR );

  Delta = ( XpatPercentMoveY * XpatUnitGridDy ) / 100;

  if ( Delta == 0 ) Delta = 1;

  XpatUnitGridY   = XpatUnitGridY - Delta;
  XpatOldPixelGridY = XpatPixelGridY;
  XpatPixelGridY    = (float)(XpatUnitGridY) * XpatUnitGridStep;
  Offset             = XpatOldPixelGridY - XpatPixelGridY;

  XCopyArea( XpatGraphicDisplay,
             XpatGraphicPixmap,
             XpatGraphicPixmap,
             XpatBackgroundGC,
             0, Offset,
             XpatGraphicDx,
             XpatGraphicDy - Offset,
             0, 0
           ); 

  XpatComputeAndDisplayMap();
  XpatDisplayFigure( 0, 0, XpatGraphicDx, Offset ); 
  XpatRefreshGraphicWindow( 0, 0, XpatGraphicDx, XpatGraphicDy);

  XpatSetMouseCursor( XpatGraphicWindow, XPAT_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XpatZoomUp                         |
|                                                             |
\------------------------------------------------------------*/

void XpatZoomUp()

{
  long      Delta;
  long      Offset;
  long      XpatOldPixelGridY;

  autbegin();

  XpatSetMouseCursor( XpatGraphicWindow, XPAT_WATCH_CURSOR );

  Delta = ( XpatPercentMoveY * XpatUnitGridDy ) / 100;

  if ( Delta == 0 ) Delta = 1;

  XpatUnitGridY   = XpatUnitGridY + Delta;
  XpatOldPixelGridY = XpatPixelGridY;
  XpatPixelGridY    = (float)(XpatUnitGridY) * XpatUnitGridStep;
  Offset             = XpatPixelGridY - XpatOldPixelGridY;

  XCopyArea( XpatGraphicDisplay,
             XpatGraphicPixmap,
             XpatGraphicPixmap,
             XpatBackgroundGC,
             0, 0,
             XpatGraphicDx,
             XpatGraphicDy - Offset,
             0, Offset
           ); 

  XpatComputeAndDisplayMap();
  XpatDisplayFigure( 0, XpatGraphicDy - Offset,
                      XpatGraphicDx, XpatGraphicDy ); 
  XpatRefreshGraphicWindow( 0, 0, XpatGraphicDx, XpatGraphicDy);

  XpatSetMouseCursor( XpatGraphicWindow, XPAT_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XpatZoomMore                        |
|                                                             |
\------------------------------------------------------------*/

void XpatZoomMore()

{
  long DeltaX;
  long DeltaY;

  autbegin();

  XpatSetMouseCursor( XpatGraphicWindow, XPAT_WATCH_CURSOR );

  DeltaX = ( XpatPercentZoom * XpatUnitGridDx ) / 100;
  DeltaY = ( XpatPercentZoom * XpatUnitGridDy ) / 100;

  if ( ( DeltaX >= 2 ) &&
       ( DeltaY >= 2 ) )
  {
    XpatAddZoom();

    XpatUnitGridX  = XpatUnitGridX + (DeltaX >> 1);
    XpatUnitGridY  = XpatUnitGridY + (DeltaY >> 1);
    XpatUnitGridDx = XpatUnitGridDx - DeltaX;
    XpatUnitGridDy = XpatUnitGridDy - DeltaY;

    XpatComputeUnitGrid();
    XpatComputeAndDisplayMap();
    XpatDisplayFigure( 0, 0, XpatGraphicDx, XpatGraphicDy );
    XpatRefreshGraphicWindow( 0, 0, XpatGraphicDx, XpatGraphicDy );
  }

  XpatSetMouseCursor( XpatGraphicWindow, XPAT_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XpatZoomLess                        |
|                                                             |
\------------------------------------------------------------*/

void XpatZoomLess()

{
  long DeltaX;
  long DeltaY;

  autbegin();

  XpatSetMouseCursor( XpatGraphicWindow, XPAT_WATCH_CURSOR );

  DeltaX = 100 * XpatUnitGridDx / ( 100 - XpatPercentZoom );
  DeltaY = 100 * XpatUnitGridDy / ( 100 - XpatPercentZoom );

  XpatAddZoom();
 
  XpatUnitGridX  = XpatUnitGridX - ((DeltaX - XpatUnitGridDx) >> 1);
  XpatUnitGridY  = XpatUnitGridY - ((DeltaY - XpatUnitGridDy) >> 1);
  XpatUnitGridDx = DeltaX;
  XpatUnitGridDy = DeltaY;

  XpatComputeUnitGrid();
  XpatComputeAndDisplayMap();
  XpatDisplayFigure( 0, 0, XpatGraphicDx, XpatGraphicDy );
  XpatRefreshGraphicWindow( 0, 0, XpatGraphicDx, XpatGraphicDy );

  XpatSetMouseCursor( XpatGraphicWindow, XPAT_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XpatZoomFit                         |
|                                                             |
\------------------------------------------------------------*/

void XpatZoomFit()

{
  long DeltaX;
  long DeltaY;
  long Delta;

  autbegin();

  XpatSetMouseCursor( XpatGraphicWindow, XPAT_WATCH_CURSOR );

  if ( XpatComputeBound() == XPAT_FALSE )
  {
    XpatErrorMessage( XpatMainWindow, "No element to display !" );
  }
  else
  {
    XpatAddZoom();

    XpatUnitGridX  = 0;
    XpatUnitGridY  = ( XpatBoundYmin / XPAT_UNIT ) - 1;
    XpatUnitGridDx = 1;
    XpatUnitGridDy = XpatBoundYmax / XPAT_UNIT;
    XpatUnitGridDx = XpatUnitGridDx - XpatUnitGridX + 1;
    XpatUnitGridDy = XpatUnitGridDy - XpatUnitGridY + 1;

    XpatComputeUnitGrid();

    DeltaX = 5 * XpatUnitGridDx / 4;
    DeltaY = 5 * XpatUnitGridDy / 4;

    XpatUnitGridX  = XpatUnitGridX - ((DeltaX - XpatUnitGridDx) >> 1);
    XpatUnitGridY  = XpatUnitGridY - ((DeltaY - XpatUnitGridDy) >> 1);
    XpatUnitGridDx = DeltaX;
    XpatUnitGridDy = DeltaY;

    XpatComputeUnitGrid();

    Delta = ( 3 * XpatUnitGridDx ) / 10;

    if ( Delta == 0 ) Delta = 1;

    XpatUnitGridX     = XpatUnitGridX - Delta;
    XpatPixelGridX    = (float)(XpatUnitGridX) * XpatUnitGridStep;

    XpatComputeUnitGrid();
    XpatComputeAndDisplayMap();
    XpatDisplayFigure( 0, 0, XpatGraphicDx, XpatGraphicDy );
    XpatRefreshGraphicWindow( 0, 0, XpatGraphicDx, XpatGraphicDy );
  }

  XpatSetMouseCursor( XpatGraphicWindow, XPAT_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XpatZoomCenter                      |
|                                                             |
\------------------------------------------------------------*/

void XpatZoomCenter( UnitX, UnitY )

   long UnitX;
   long UnitY;
{
  autbegin();

  XpatSetMouseCursor( XpatGraphicWindow, XPAT_WATCH_CURSOR );

  XpatUnitGridX = UnitX - ( XpatUnitGridDx >> 1 );
  XpatUnitGridY = UnitY - ( XpatUnitGridDy >> 1 );

  XpatComputeUnitGrid();
  XpatComputeAndDisplayMap();
  XpatDisplayFigure( 0, 0, XpatGraphicDx, XpatGraphicDy );
  XpatRefreshGraphicWindow( 0, 0, XpatGraphicDx, XpatGraphicDy );

  XpatSetMouseCursor( XpatGraphicWindow, XPAT_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XpatZoomIn                         |
|                                                             |
\------------------------------------------------------------*/
 
void XpatZoomIn( UnitX1, UnitY1, UnitX2, UnitY2 )
 
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

    XpatSetMouseCursor( XpatGraphicWindow, XPAT_WATCH_CURSOR );

    XpatAddZoom();

    XpatUnitGridX  = UnitX1;
    XpatUnitGridY  = UnitY1;
    XpatUnitGridDx = UnitX2 - UnitX1;
    XpatUnitGridDy = UnitY2 - UnitY1;

    XpatComputeUnitGrid();
    XpatComputeAndDisplayMap();
    XpatDisplayFigure( 0, 0, XpatGraphicDx, XpatGraphicDy );
    XpatRefreshGraphicWindow( 0, 0, XpatGraphicDx, XpatGraphicDy );

    XpatSetMouseCursor( XpatGraphicWindow, XPAT_NORMAL_CURSOR );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XpatZoomPan                        |
|                                                             |
\------------------------------------------------------------*/

void XpatZoomPan( UnitX1, UnitY1, UnitX2, UnitY2 )

   long UnitX1;
   long UnitY1;
   long UnitX2;
   long UnitY2;
{
  if ( ( UnitX1 != UnitX2 ) ||
       ( UnitY1 != UnitY2 ) )
  {
    autbegin();

    XpatSetMouseCursor( XpatGraphicWindow, XPAT_WATCH_CURSOR );
 
    XpatUnitGridX  = XpatUnitGridX + ( UnitX1 - UnitX2 );
    XpatUnitGridY  = XpatUnitGridY + ( UnitY1 - UnitY2 );

    XpatComputeUnitGrid();
    XpatComputeAndDisplayMap();
    XpatDisplayFigure( 0, 0, XpatGraphicDx, XpatGraphicDy );
    XpatRefreshGraphicWindow( 0, 0, XpatGraphicDx, XpatGraphicDy );

    XpatSetMouseCursor( XpatGraphicWindow, XPAT_NORMAL_CURSOR );

    autend();
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          XpatZoomRefresh                    |
|                                                             |
\------------------------------------------------------------*/

void XpatZoomRefresh()

{
  autbegin();

  XpatSetMouseCursor( XpatGraphicWindow, XPAT_WATCH_CURSOR );

  XpatDisplayFigure( 0, 0, XpatGraphicDx, XpatGraphicDy );
  XpatRefreshGraphicWindow( 0, 0, XpatGraphicDx, XpatGraphicDy );

  XpatSetMouseCursor( XpatGraphicWindow, XPAT_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XpatReScale                        |
|                                                             |
\------------------------------------------------------------*/

void XpatReScale( Less )

   int Less;
{
  double Ratio;

  autbegin();

  Ratio = XpatReScaleFigure( Less );

  XpatBoundYmin *= Ratio;
  XpatBoundYmax *= Ratio;

  XpatUnitGridY  *= Ratio;
  XpatUnitGridDx  = 1;
  XpatUnitGridDy *= Ratio;

  if ( XpatUnitGridX < 0 ) XpatUnitGridX *= Ratio;

  XpatComputeUnitGrid();
  XpatComputeAndDisplayMap();
  XpatZoomRefresh();

  autend();
}
