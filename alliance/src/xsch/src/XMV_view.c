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
| Tool    :                   XSCH                            |
|                                                             |
| File    :                   View.c                          |
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

 xschzoom *XschHeadZoom    = (xschzoom *)NULL;

 int        XschPercentZoom  = XSCH_PERCENT_ZOOM;
 int        XschPercentMoveX = XSCH_PERCENT_MOVE;
 int        XschPercentMoveY = XSCH_PERCENT_MOVE;

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
|                        XschAllocZoom                        |
|                                                             |
\------------------------------------------------------------*/

xschzoom *XschAllocZoom ()

{
  return ( (xschzoom *) autalloc( sizeof ( xschzoom ), 1));
}

/*------------------------------------------------------------\
|                                                             |
|                        XschFreeZoom                         |
|                                                             |
\------------------------------------------------------------*/

void XschFreeZoom ( FreeZoom )

  xschzoom *FreeZoom;

{
  autfree( (char *)FreeZoom, sizeof(FreeZoom) );
}

/*------------------------------------------------------------\
|                                                             |
|                        XschAddZoom                          |
|                                                             |
\------------------------------------------------------------*/
 
void XschAddZoom ()
 
{
  xschzoom *XschZoom;

  autbegin();
 
  XschZoom       = XschAllocZoom ();
  XschZoom->X    = XschUnitGridX;
  XschZoom->Y    = XschUnitGridY;
  XschZoom->DX   = XschUnitGridDx;
  XschZoom->DY   = XschUnitGridDy;
  XschZoom->NEXT = XschHeadZoom;
  XschHeadZoom   = XschZoom;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XschDelZoom                        |
|                                                             |
\------------------------------------------------------------*/

char XschDelZoom()

{
  xschzoom *XschZoom;

  if ( XschHeadZoom != (xschzoom *) NULL )
  {
    autbegin();

    XschZoom     = XschHeadZoom;
    XschHeadZoom = XschHeadZoom->NEXT;
    XschFreeZoom( XschZoom );

    autend();
    return( XSCH_TRUE ); 
  }

  return( XSCH_FALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                      XschInitializeZoom                     |
|                                                             |
\------------------------------------------------------------*/

void XschInitializeZoom()

{
  autbegin();

  while( XschDelZoom() != XSCH_FALSE );

  if ( XschComputeBound() == XSCH_FALSE )
  {
    XschInitializeUnitGrid();
  }
  else
  {
    XschUnitGridX  = ( XschBoundXmin / XSCH_UNIT ) - 1;
    XschUnitGridY  = ( XschBoundYmin / XSCH_UNIT ) - 1;
    XschUnitGridDx = ( XschBoundXmax / XSCH_UNIT );
    XschUnitGridDy = ( XschBoundYmax / XSCH_UNIT );
    XschUnitGridDx = XschUnitGridDx - XschUnitGridX + 1;
    XschUnitGridDy = XschUnitGridDy - XschUnitGridY + 1;

    XschComputeUnitGrid();
  }

  XschComputeAndDisplayMap();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XschZoomUndo                        |
|                                                             |
\------------------------------------------------------------*/

void XschZoomUndo()

{
  autbegin();

  if ( XschHeadZoom == (xschzoom *)NULL )
  {
    XschErrorMessage( XschMainWindow, "No previous zoom !" ); 
  }
  else
  {
    XschSetMouseCursor( XschGraphicWindow, XSCH_WATCH_CURSOR );

    XschUnitGridX  = XschHeadZoom->X;
    XschUnitGridY  = XschHeadZoom->Y;
    XschUnitGridDx = XschHeadZoom->DX;
    XschUnitGridDy = XschHeadZoom->DY;

    XschComputeUnitGrid();
    XschComputeAndDisplayMap();

    XschDisplayFigure( 0, 0,
                        XschGraphicDx, XschGraphicDy );

    XschRefreshGraphicWindow( 0, 0,
                               XschGraphicDx, XschGraphicDy);

    XschSetMouseCursor( XschGraphicWindow, XSCH_NORMAL_CURSOR );

    XschDelZoom();
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XschZoomRight                       |
|                                                             |
\------------------------------------------------------------*/

void XschZoomRight()

{
  long      Delta;
  long      Offset;
  long      XschOldPixelGridX;

  autbegin();

  XschSetMouseCursor( XschGraphicWindow, XSCH_WATCH_CURSOR );

  Delta = ( XschPercentMoveX * XschUnitGridDx ) / 100;

  if ( Delta == 0 ) Delta = 1;

  XschUnitGridX   = XschUnitGridX + Delta;
  XschOldPixelGridX = XschPixelGridX;
  XschPixelGridX    = (float)(XschUnitGridX) * XschUnitGridStep;
  Offset             = XschPixelGridX - XschOldPixelGridX;

  XCopyArea( XschGraphicDisplay,
             XschGraphicPixmap,
             XschGraphicPixmap,
             XschBackgroundGC,
             Offset, 0,
             XschGraphicDx - Offset,
             XschGraphicDy,
             0, 0
           ); 

  XschComputeAndDisplayMap();
  XschDisplayFigure( XschGraphicDx - Offset, 0, 
                      XschGraphicDx, XschGraphicDy ); 
  XschRefreshGraphicWindow( 0, 0,
                             XschGraphicDx, XschGraphicDy); 

  XschSetMouseCursor( XschGraphicWindow, XSCH_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XschZoomLeft                        |
|                                                             |
\------------------------------------------------------------*/

void XschZoomLeft()

{
  long      Delta;
  long      Offset;
  long      XschOldPixelGridX;

  autbegin();

  XschSetMouseCursor( XschGraphicWindow, XSCH_WATCH_CURSOR );

  Delta = ( XschPercentMoveX * XschUnitGridDx ) / 100;

  if ( Delta == 0 ) Delta = 1;

  XschUnitGridX   = XschUnitGridX - Delta;
  XschOldPixelGridX = XschPixelGridX;
  XschPixelGridX    = (float)(XschUnitGridX) * XschUnitGridStep;
  Offset             = XschOldPixelGridX - XschPixelGridX;

  XCopyArea( XschGraphicDisplay,
             XschGraphicPixmap,
             XschGraphicPixmap,
             XschBackgroundGC,
             0, 0,
             XschGraphicDx - Offset,
             XschGraphicDy,
             Offset, 0
           ); 

  XschComputeAndDisplayMap();
  XschDisplayFigure( 0, 0, Offset, XschGraphicDy );
  XschRefreshGraphicWindow( 0, 0, XschGraphicDx, XschGraphicDy);

  XschSetMouseCursor( XschGraphicWindow, XSCH_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XschZoomDown                       |
|                                                             |
\------------------------------------------------------------*/

void XschZoomDown()

{
  long      Delta;
  long      Offset;
  long      XschOldPixelGridY;

  autbegin();

  XschSetMouseCursor( XschGraphicWindow, XSCH_WATCH_CURSOR );

  Delta = ( XschPercentMoveY * XschUnitGridDy ) / 100;

  if ( Delta == 0 ) Delta = 1;

  XschUnitGridY   = XschUnitGridY - Delta;
  XschOldPixelGridY = XschPixelGridY;
  XschPixelGridY    = (float)(XschUnitGridY) * XschUnitGridStep;
  Offset             = XschOldPixelGridY - XschPixelGridY;

  XCopyArea( XschGraphicDisplay,
             XschGraphicPixmap,
             XschGraphicPixmap,
             XschBackgroundGC,
             0, Offset,
             XschGraphicDx,
             XschGraphicDy - Offset,
             0, 0
           ); 

  XschComputeAndDisplayMap();
  XschDisplayFigure( 0, 0, XschGraphicDx, Offset ); 
  XschRefreshGraphicWindow( 0, 0, XschGraphicDx, XschGraphicDy);

  XschSetMouseCursor( XschGraphicWindow, XSCH_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XschZoomUp                         |
|                                                             |
\------------------------------------------------------------*/

void XschZoomUp()

{
  long      Delta;
  long      Offset;
  long      XschOldPixelGridY;

  autbegin();

  XschSetMouseCursor( XschGraphicWindow, XSCH_WATCH_CURSOR );

  Delta = ( XschPercentMoveY * XschUnitGridDy ) / 100;

  if ( Delta == 0 ) Delta = 1;

  XschUnitGridY   = XschUnitGridY + Delta;
  XschOldPixelGridY = XschPixelGridY;
  XschPixelGridY    = (float)(XschUnitGridY) * XschUnitGridStep;
  Offset             = XschPixelGridY - XschOldPixelGridY;

  XCopyArea( XschGraphicDisplay,
             XschGraphicPixmap,
             XschGraphicPixmap,
             XschBackgroundGC,
             0, 0,
             XschGraphicDx,
             XschGraphicDy - Offset,
             0, Offset
           ); 

  XschComputeAndDisplayMap();
  XschDisplayFigure( 0, XschGraphicDy - Offset,
                      XschGraphicDx, XschGraphicDy ); 
  XschRefreshGraphicWindow( 0, 0, XschGraphicDx, XschGraphicDy);

  XschSetMouseCursor( XschGraphicWindow, XSCH_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XschZoomMore                        |
|                                                             |
\------------------------------------------------------------*/

void XschZoomMore()

{
  long DeltaX;
  long DeltaY;

  autbegin();

  XschSetMouseCursor( XschGraphicWindow, XSCH_WATCH_CURSOR );

  DeltaX = ( XschPercentZoom * XschUnitGridDx ) / 100;
  DeltaY = ( XschPercentZoom * XschUnitGridDy ) / 100;

  if ( ( DeltaX >= 2 ) &&
       ( DeltaY >= 2 ) )
  {
    XschAddZoom();

    XschUnitGridX  = XschUnitGridX + (DeltaX >> 1);
    XschUnitGridY  = XschUnitGridY + (DeltaY >> 1);
    XschUnitGridDx = XschUnitGridDx - DeltaX;
    XschUnitGridDy = XschUnitGridDy - DeltaY;

    XschComputeUnitGrid();
    XschComputeAndDisplayMap();
    XschDisplayFigure( 0, 0, XschGraphicDx, XschGraphicDy );
    XschRefreshGraphicWindow( 0, 0, XschGraphicDx, XschGraphicDy );
  }

  XschSetMouseCursor( XschGraphicWindow, XSCH_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XschZoomLess                        |
|                                                             |
\------------------------------------------------------------*/

void XschZoomLess()

{
  long DeltaX;
  long DeltaY;

  autbegin();

  XschSetMouseCursor( XschGraphicWindow, XSCH_WATCH_CURSOR );

  DeltaX = 100 * XschUnitGridDx / ( 100 - XschPercentZoom );
  DeltaY = 100 * XschUnitGridDy / ( 100 - XschPercentZoom );

  XschAddZoom();
 
  XschUnitGridX  = XschUnitGridX - ((DeltaX - XschUnitGridDx) >> 1);
  XschUnitGridY  = XschUnitGridY - ((DeltaY - XschUnitGridDy) >> 1);
  XschUnitGridDx = DeltaX;
  XschUnitGridDy = DeltaY;

  XschComputeUnitGrid();
  XschComputeAndDisplayMap();
  XschDisplayFigure( 0, 0, XschGraphicDx, XschGraphicDy );
  XschRefreshGraphicWindow( 0, 0, XschGraphicDx, XschGraphicDy );

  XschSetMouseCursor( XschGraphicWindow, XSCH_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XschZoomFit                         |
|                                                             |
\------------------------------------------------------------*/

void XschZoomFit()

{
  autbegin();

  XschSetMouseCursor( XschGraphicWindow, XSCH_WATCH_CURSOR );

  if ( XschComputeBound() == XSCH_FALSE )
  {
    XschErrorMessage( XschMainWindow, "No element to display !" );
  }
  else
  {
    XschAddZoom();

    XschUnitGridX  = ( XschBoundXmin / XSCH_UNIT ) - 1;
    XschUnitGridY  = ( XschBoundYmin / XSCH_UNIT ) - 1;
    XschUnitGridDx = XschBoundXmax / XSCH_UNIT;
    XschUnitGridDy = XschBoundYmax / XSCH_UNIT;
    XschUnitGridDx = XschUnitGridDx - XschUnitGridX + 1;
    XschUnitGridDy = XschUnitGridDy - XschUnitGridY + 1;

    XschComputeUnitGrid();
    XschComputeAndDisplayMap();
    XschDisplayFigure( 0, 0, XschGraphicDx, XschGraphicDy );
    XschRefreshGraphicWindow( 0, 0, XschGraphicDx, XschGraphicDy );
  }

  XschSetMouseCursor( XschGraphicWindow, XSCH_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XschZoomCenter                      |
|                                                             |
\------------------------------------------------------------*/

void XschZoomCenter( UnitX, UnitY )

   long UnitX;
   long UnitY;
{
  autbegin();

  XschSetMouseCursor( XschGraphicWindow, XSCH_WATCH_CURSOR );

  XschUnitGridX = UnitX - ( XschUnitGridDx >> 1 );
  XschUnitGridY = UnitY - ( XschUnitGridDy >> 1 );

  XschComputeUnitGrid();
  XschComputeAndDisplayMap();
  XschDisplayFigure( 0, 0, XschGraphicDx, XschGraphicDy );
  XschRefreshGraphicWindow( 0, 0, XschGraphicDx, XschGraphicDy );

  XschSetMouseCursor( XschGraphicWindow, XSCH_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XschZoomIn                         |
|                                                             |
\------------------------------------------------------------*/
 
void XschZoomIn( UnitX1, UnitY1, UnitX2, UnitY2 )
 
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

    XschSetMouseCursor( XschGraphicWindow, XSCH_WATCH_CURSOR );

    XschAddZoom();

    XschUnitGridX  = UnitX1;
    XschUnitGridY  = UnitY1;
    XschUnitGridDx = UnitX2 - UnitX1;
    XschUnitGridDy = UnitY2 - UnitY1;

    XschComputeUnitGrid();
    XschComputeAndDisplayMap();
    XschDisplayFigure( 0, 0, XschGraphicDx, XschGraphicDy );
    XschRefreshGraphicWindow( 0, 0, XschGraphicDx, XschGraphicDy );

    XschSetMouseCursor( XschGraphicWindow, XSCH_NORMAL_CURSOR );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XschZoomPan                        |
|                                                             |
\------------------------------------------------------------*/

void XschZoomPan( UnitX1, UnitY1, UnitX2, UnitY2 )

   long UnitX1;
   long UnitY1;
   long UnitX2;
   long UnitY2;
{
  if ( ( UnitX1 != UnitX2 ) ||
       ( UnitY1 != UnitY2 ) )
  {
    autbegin();

    XschSetMouseCursor( XschGraphicWindow, XSCH_WATCH_CURSOR );
 
    XschUnitGridX  = XschUnitGridX + ( UnitX1 - UnitX2 );
    XschUnitGridY  = XschUnitGridY + ( UnitY1 - UnitY2 );

    XschComputeUnitGrid();
    XschComputeAndDisplayMap();
    XschDisplayFigure( 0, 0, XschGraphicDx, XschGraphicDy );
    XschRefreshGraphicWindow( 0, 0, XschGraphicDx, XschGraphicDy );

    XschSetMouseCursor( XschGraphicWindow, XSCH_NORMAL_CURSOR );

    autend();
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          XschZoomRefresh                    |
|                                                             |
\------------------------------------------------------------*/

void XschZoomRefresh()

{
  autbegin();

  XschSetMouseCursor( XschGraphicWindow, XSCH_WATCH_CURSOR );

  XschDisplayFigure( 0, 0, XschGraphicDx, XschGraphicDy );
  XschRefreshGraphicWindow( 0, 0, XschGraphicDx, XschGraphicDy );

  XschSetMouseCursor( XschGraphicWindow, XSCH_NORMAL_CURSOR );

  autend();
}
