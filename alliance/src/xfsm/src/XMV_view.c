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
| Tool    :                   XFSM                            |
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
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "XSB.h"
# include "XFS.h"
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

 xfsmzoom *XfsmHeadZoom    = (xfsmzoom *)NULL;

 int        XfsmPercentZoom  = XFSM_PERCENT_ZOOM;
 int        XfsmPercentMoveX = XFSM_PERCENT_MOVE;
 int        XfsmPercentMoveY = XFSM_PERCENT_MOVE;

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
|                        XfsmAllocZoom                        |
|                                                             |
\------------------------------------------------------------*/

xfsmzoom *XfsmAllocZoom ()

{
  return ( (xfsmzoom *) autalloc( sizeof ( xfsmzoom ), 1));
}

/*------------------------------------------------------------\
|                                                             |
|                        XfsmFreeZoom                         |
|                                                             |
\------------------------------------------------------------*/

void XfsmFreeZoom ( FreeZoom )

  xfsmzoom *FreeZoom;

{
  autfree( (char *)FreeZoom, sizeof(FreeZoom) );
}

/*------------------------------------------------------------\
|                                                             |
|                        XfsmAddZoom                          |
|                                                             |
\------------------------------------------------------------*/
 
void XfsmAddZoom ()
 
{
  xfsmzoom *XfsmZoom;

  autbegin();
 
  XfsmZoom       = XfsmAllocZoom ();
  XfsmZoom->X    = XfsmUnitGridX;
  XfsmZoom->Y    = XfsmUnitGridY;
  XfsmZoom->DX   = XfsmUnitGridDx;
  XfsmZoom->DY   = XfsmUnitGridDy;
  XfsmZoom->NEXT = XfsmHeadZoom;
  XfsmHeadZoom   = XfsmZoom;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XfsmDelZoom                        |
|                                                             |
\------------------------------------------------------------*/

char XfsmDelZoom()

{
  xfsmzoom *XfsmZoom;

  if ( XfsmHeadZoom != (xfsmzoom *) NULL )
  {
    autbegin();

    XfsmZoom     = XfsmHeadZoom;
    XfsmHeadZoom = XfsmHeadZoom->NEXT;
    XfsmFreeZoom( XfsmZoom );

    autend();
    return( XFSM_TRUE ); 
  }

  return( XFSM_FALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                      XfsmInitializeZoom                     |
|                                                             |
\------------------------------------------------------------*/

void XfsmInitializeZoom()

{
  autbegin();

  while( XfsmDelZoom() != XFSM_FALSE );

  if ( XfsmComputeBound() == XFSM_FALSE )
  {
    XfsmInitializeUnitGrid();
  }
  else
  {
    XfsmUnitGridX  = ( XfsmBoundXmin / XFSM_UNIT ) - 1;
    XfsmUnitGridY  = ( XfsmBoundYmin / XFSM_UNIT ) - 1;
    XfsmUnitGridDx = ( XfsmBoundXmax / XFSM_UNIT );
    XfsmUnitGridDy = ( XfsmBoundYmax / XFSM_UNIT );
    XfsmUnitGridDx = XfsmUnitGridDx - XfsmUnitGridX + 1;
    XfsmUnitGridDy = XfsmUnitGridDy - XfsmUnitGridY + 1;

    XfsmComputeUnitGrid();
  }

  XfsmComputeAndDisplayMap();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XfsmZoomUndo                        |
|                                                             |
\------------------------------------------------------------*/

void XfsmZoomUndo()

{
  autbegin();

  if ( XfsmHeadZoom == (xfsmzoom *)NULL )
  {
    XfsmErrorMessage( XfsmMainWindow, "No previous zoom !" ); 
  }
  else
  {
    XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_WATCH_CURSOR );

    XfsmUnitGridX  = XfsmHeadZoom->X;
    XfsmUnitGridY  = XfsmHeadZoom->Y;
    XfsmUnitGridDx = XfsmHeadZoom->DX;
    XfsmUnitGridDy = XfsmHeadZoom->DY;

    XfsmComputeUnitGrid();
    XfsmComputeAndDisplayMap();

    XfsmDisplayFigure( 0, 0,
                        XfsmGraphicDx, XfsmGraphicDy );

    XfsmRefreshGraphicWindow( 0, 0,
                               XfsmGraphicDx, XfsmGraphicDy);

    XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_NORMAL_CURSOR );

    XfsmDelZoom();
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XfsmZoomRight                       |
|                                                             |
\------------------------------------------------------------*/

void XfsmZoomRight()

{
  long      Delta;
  long      Offset;
  long      XfsmOldPixelGridX;

  autbegin();

  XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_WATCH_CURSOR );

  Delta = ( XfsmPercentMoveX * XfsmUnitGridDx ) / 100;

  if ( Delta == 0 ) Delta = 1;

  XfsmUnitGridX   = XfsmUnitGridX + Delta;
  XfsmOldPixelGridX = XfsmPixelGridX;
  XfsmPixelGridX    = (float)(XfsmUnitGridX) * XfsmUnitGridStep;
  Offset             = XfsmPixelGridX - XfsmOldPixelGridX;

  XCopyArea( XfsmGraphicDisplay,
             XfsmGraphicPixmap,
             XfsmGraphicPixmap,
             XfsmBackgroundGC,
             Offset, 0,
             XfsmGraphicDx - Offset,
             XfsmGraphicDy,
             0, 0
           ); 

  XfsmComputeAndDisplayMap();
  XfsmDisplayFigure( XfsmGraphicDx - Offset, 0, 
                      XfsmGraphicDx, XfsmGraphicDy ); 
  XfsmRefreshGraphicWindow( 0, 0,
                             XfsmGraphicDx, XfsmGraphicDy); 

  XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XfsmZoomLeft                        |
|                                                             |
\------------------------------------------------------------*/

void XfsmZoomLeft()

{
  long      Delta;
  long      Offset;
  long      XfsmOldPixelGridX;

  autbegin();

  XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_WATCH_CURSOR );

  Delta = ( XfsmPercentMoveX * XfsmUnitGridDx ) / 100;

  if ( Delta == 0 ) Delta = 1;

  XfsmUnitGridX   = XfsmUnitGridX - Delta;
  XfsmOldPixelGridX = XfsmPixelGridX;
  XfsmPixelGridX    = (float)(XfsmUnitGridX) * XfsmUnitGridStep;
  Offset             = XfsmOldPixelGridX - XfsmPixelGridX;

  XCopyArea( XfsmGraphicDisplay,
             XfsmGraphicPixmap,
             XfsmGraphicPixmap,
             XfsmBackgroundGC,
             0, 0,
             XfsmGraphicDx - Offset,
             XfsmGraphicDy,
             Offset, 0
           ); 

  XfsmComputeAndDisplayMap();
  XfsmDisplayFigure( 0, 0, Offset, XfsmGraphicDy );
  XfsmRefreshGraphicWindow( 0, 0, XfsmGraphicDx, XfsmGraphicDy);

  XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XfsmZoomDown                       |
|                                                             |
\------------------------------------------------------------*/

void XfsmZoomDown()

{
  long      Delta;
  long      Offset;
  long      XfsmOldPixelGridY;

  autbegin();

  XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_WATCH_CURSOR );

  Delta = ( XfsmPercentMoveY * XfsmUnitGridDy ) / 100;

  if ( Delta == 0 ) Delta = 1;

  XfsmUnitGridY   = XfsmUnitGridY - Delta;
  XfsmOldPixelGridY = XfsmPixelGridY;
  XfsmPixelGridY    = (float)(XfsmUnitGridY) * XfsmUnitGridStep;
  Offset             = XfsmOldPixelGridY - XfsmPixelGridY;

  XCopyArea( XfsmGraphicDisplay,
             XfsmGraphicPixmap,
             XfsmGraphicPixmap,
             XfsmBackgroundGC,
             0, Offset,
             XfsmGraphicDx,
             XfsmGraphicDy - Offset,
             0, 0
           ); 

  XfsmComputeAndDisplayMap();
  XfsmDisplayFigure( 0, 0, XfsmGraphicDx, Offset ); 
  XfsmRefreshGraphicWindow( 0, 0, XfsmGraphicDx, XfsmGraphicDy);

  XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XfsmZoomUp                         |
|                                                             |
\------------------------------------------------------------*/

void XfsmZoomUp()

{
  long      Delta;
  long      Offset;
  long      XfsmOldPixelGridY;

  autbegin();

  XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_WATCH_CURSOR );

  Delta = ( XfsmPercentMoveY * XfsmUnitGridDy ) / 100;

  if ( Delta == 0 ) Delta = 1;

  XfsmUnitGridY   = XfsmUnitGridY + Delta;
  XfsmOldPixelGridY = XfsmPixelGridY;
  XfsmPixelGridY    = (float)(XfsmUnitGridY) * XfsmUnitGridStep;
  Offset             = XfsmPixelGridY - XfsmOldPixelGridY;

  XCopyArea( XfsmGraphicDisplay,
             XfsmGraphicPixmap,
             XfsmGraphicPixmap,
             XfsmBackgroundGC,
             0, 0,
             XfsmGraphicDx,
             XfsmGraphicDy - Offset,
             0, Offset
           ); 

  XfsmComputeAndDisplayMap();
  XfsmDisplayFigure( 0, XfsmGraphicDy - Offset,
                      XfsmGraphicDx, XfsmGraphicDy ); 
  XfsmRefreshGraphicWindow( 0, 0, XfsmGraphicDx, XfsmGraphicDy);

  XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XfsmZoomMore                        |
|                                                             |
\------------------------------------------------------------*/

void XfsmZoomMore()

{
  long DeltaX;
  long DeltaY;

  autbegin();

  XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_WATCH_CURSOR );

  DeltaX = ( XfsmPercentZoom * XfsmUnitGridDx ) / 100;
  DeltaY = ( XfsmPercentZoom * XfsmUnitGridDy ) / 100;

  if ( ( DeltaX >= 2 ) &&
       ( DeltaY >= 2 ) )
  {
    XfsmAddZoom();

    XfsmUnitGridX  = XfsmUnitGridX + (DeltaX >> 1);
    XfsmUnitGridY  = XfsmUnitGridY + (DeltaY >> 1);
    XfsmUnitGridDx = XfsmUnitGridDx - DeltaX;
    XfsmUnitGridDy = XfsmUnitGridDy - DeltaY;

    XfsmComputeUnitGrid();
    XfsmComputeAndDisplayMap();
    XfsmDisplayFigure( 0, 0, XfsmGraphicDx, XfsmGraphicDy );
    XfsmRefreshGraphicWindow( 0, 0, XfsmGraphicDx, XfsmGraphicDy );
  }

  XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XfsmZoomLess                        |
|                                                             |
\------------------------------------------------------------*/

void XfsmZoomLess()

{
  long DeltaX;
  long DeltaY;

  autbegin();

  XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_WATCH_CURSOR );

  DeltaX = 100 * XfsmUnitGridDx / ( 100 - XfsmPercentZoom );
  DeltaY = 100 * XfsmUnitGridDy / ( 100 - XfsmPercentZoom );

  XfsmAddZoom();
 
  XfsmUnitGridX  = XfsmUnitGridX - ((DeltaX - XfsmUnitGridDx) >> 1);
  XfsmUnitGridY  = XfsmUnitGridY - ((DeltaY - XfsmUnitGridDy) >> 1);
  XfsmUnitGridDx = DeltaX;
  XfsmUnitGridDy = DeltaY;

  XfsmComputeUnitGrid();
  XfsmComputeAndDisplayMap();
  XfsmDisplayFigure( 0, 0, XfsmGraphicDx, XfsmGraphicDy );
  XfsmRefreshGraphicWindow( 0, 0, XfsmGraphicDx, XfsmGraphicDy );

  XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XfsmZoomFit                         |
|                                                             |
\------------------------------------------------------------*/

void XfsmZoomFit()

{
  autbegin();

  XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_WATCH_CURSOR );

  if ( XfsmComputeBound() == XFSM_FALSE )
  {
    XfsmErrorMessage( XfsmMainWindow, "No element to display !" );
  }
  else
  {
    XfsmAddZoom();

    XfsmUnitGridX  = ( XfsmBoundXmin / XFSM_UNIT ) - 1;
    XfsmUnitGridY  = ( XfsmBoundYmin / XFSM_UNIT ) - 1;
    XfsmUnitGridDx = XfsmBoundXmax / XFSM_UNIT;
    XfsmUnitGridDy = XfsmBoundYmax / XFSM_UNIT;
    XfsmUnitGridDx = XfsmUnitGridDx - XfsmUnitGridX + 1;
    XfsmUnitGridDy = XfsmUnitGridDy - XfsmUnitGridY + 1;

    XfsmComputeUnitGrid();
    XfsmComputeAndDisplayMap();
    XfsmDisplayFigure( 0, 0, XfsmGraphicDx, XfsmGraphicDy );
    XfsmRefreshGraphicWindow( 0, 0, XfsmGraphicDx, XfsmGraphicDy );
  }

  XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XfsmZoomCenter                      |
|                                                             |
\------------------------------------------------------------*/

void XfsmZoomCenter( UnitX, UnitY )

   long UnitX;
   long UnitY;
{
  autbegin();

  XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_WATCH_CURSOR );

  XfsmUnitGridX = UnitX - ( XfsmUnitGridDx >> 1 );
  XfsmUnitGridY = UnitY - ( XfsmUnitGridDy >> 1 );

  XfsmComputeUnitGrid();
  XfsmComputeAndDisplayMap();
  XfsmDisplayFigure( 0, 0, XfsmGraphicDx, XfsmGraphicDy );
  XfsmRefreshGraphicWindow( 0, 0, XfsmGraphicDx, XfsmGraphicDy );

  XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_NORMAL_CURSOR );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XfsmZoomIn                         |
|                                                             |
\------------------------------------------------------------*/
 
void XfsmZoomIn( UnitX1, UnitY1, UnitX2, UnitY2 )
 
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

    XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_WATCH_CURSOR );

    XfsmAddZoom();

    XfsmUnitGridX  = UnitX1;
    XfsmUnitGridY  = UnitY1;
    XfsmUnitGridDx = UnitX2 - UnitX1;
    XfsmUnitGridDy = UnitY2 - UnitY1;

    XfsmComputeUnitGrid();
    XfsmComputeAndDisplayMap();
    XfsmDisplayFigure( 0, 0, XfsmGraphicDx, XfsmGraphicDy );
    XfsmRefreshGraphicWindow( 0, 0, XfsmGraphicDx, XfsmGraphicDy );

    XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_NORMAL_CURSOR );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XfsmZoomPan                        |
|                                                             |
\------------------------------------------------------------*/

void XfsmZoomPan( UnitX1, UnitY1, UnitX2, UnitY2 )

   long UnitX1;
   long UnitY1;
   long UnitX2;
   long UnitY2;
{
  if ( ( UnitX1 != UnitX2 ) ||
       ( UnitY1 != UnitY2 ) )
  {
    autbegin();

    XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_WATCH_CURSOR );
 
    XfsmUnitGridX  = XfsmUnitGridX + ( UnitX1 - UnitX2 );
    XfsmUnitGridY  = XfsmUnitGridY + ( UnitY1 - UnitY2 );

    XfsmComputeUnitGrid();
    XfsmComputeAndDisplayMap();
    XfsmDisplayFigure( 0, 0, XfsmGraphicDx, XfsmGraphicDy );
    XfsmRefreshGraphicWindow( 0, 0, XfsmGraphicDx, XfsmGraphicDy );

    XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_NORMAL_CURSOR );

    autend();
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          XfsmZoomRefresh                    |
|                                                             |
\------------------------------------------------------------*/

void XfsmZoomRefresh()

{
  autbegin();

  XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_WATCH_CURSOR );

  XfsmDisplayFigure( 0, 0, XfsmGraphicDx, XfsmGraphicDy );
  XfsmRefreshGraphicWindow( 0, 0, XfsmGraphicDx, XfsmGraphicDy );

  XfsmSetMouseCursor( XfsmGraphicWindow, XFSM_NORMAL_CURSOR );

  autend();
}
