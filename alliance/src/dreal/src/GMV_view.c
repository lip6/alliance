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
| Tool    :                   DREAL                           |
|                                                             |
| File    :                   View.c                          |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                  28.03.95                         |
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
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "rfm.h"
# include "GRD.h"
# include "GSB.h"
# include "GTB.h"
# include "GMX.h"
# include "GMV.h"

# include "GMV_view.h"

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

 drealzoom *DrealHeadZoom    = (drealzoom *)NULL;

 int        DrealPercentZoom = DREAL_PERCENT_ZOOM;
 int        DrealPercentMove = DREAL_PERCENT_MOVE;

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
|                       DrealAllocZoom                        |
|                                                             |
\------------------------------------------------------------*/

drealzoom *DrealAllocZoom ()

{
  return ( (drealzoom *) rdsallocheap( sizeof ( drealzoom ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       DrealFreeZoom                         |
|                                                             |
\------------------------------------------------------------*/

void DrealFreeZoom ( FreeZoom )

  drealzoom *FreeZoom;

{
  rdsfreeheap( (char *)FreeZoom, sizeof(FreeZoom) );
}

/*------------------------------------------------------------\
|                                                             |
|                       DrealAddZoom                          |
|                                                             |
\------------------------------------------------------------*/
 
 void DrealAddZoom ()
 
{
  drealzoom *DrealZoom;

  rdsbegin();
 
  DrealZoom       = DrealAllocZoom ();
  DrealZoom->X    = DrealLambdaGridX;
  DrealZoom->Y    = DrealLambdaGridY;
  DrealZoom->DX   = DrealLambdaGridDx;
  DrealZoom->DY   = DrealLambdaGridDy;
  DrealZoom->NEXT = DrealHeadZoom;
  DrealHeadZoom   = DrealZoom;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                         DrealDelZoom                        |
|                                                             |
\------------------------------------------------------------*/

char DrealDelZoom()

{
  drealzoom *DrealZoom;

  rdsbegin();

  if ( DrealHeadZoom != (drealzoom *) NULL )
  {
    DrealZoom     = DrealHeadZoom;
    DrealHeadZoom = DrealHeadZoom->NEXT;
    DrealFreeZoom( DrealZoom );

    rdsend();
    return( DREAL_TRUE ); 
  }


  rdsend();
  return( DREAL_FALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                     DrealInitializeZoom                     |
|                                                             |
\------------------------------------------------------------*/

void DrealInitializeZoom()

{
  rdsbegin();

  while( DrealDelZoom() != DREAL_FALSE );

  if ( DrealComputeBound() == DREAL_FALSE )
  {
    DrealInitializeLambdaGrid();
  }
  else
  {
    DrealLambdaGridX  = ( DrealBoundXmin / RDS_PHYSICAL_GRID ) - 1;
    DrealLambdaGridY  = ( DrealBoundYmin / RDS_PHYSICAL_GRID ) - 1;
    DrealLambdaGridDx = DrealBoundXmax / RDS_PHYSICAL_GRID;
    DrealLambdaGridDy = DrealBoundYmax / RDS_PHYSICAL_GRID;
    DrealLambdaGridDx = DrealLambdaGridDx - DrealLambdaGridX + 1;
    DrealLambdaGridDy = DrealLambdaGridDy - DrealLambdaGridY + 1;

    DrealComputeLambdaGrid();
  }

  DrealComputeAndDisplayMap();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        DrealZoomUndo                        |
|                                                             |
\------------------------------------------------------------*/

void DrealZoomUndo()

{
  rdsbegin();

  if ( DrealHeadZoom == (drealzoom *)NULL )
  {
    DrealErrorMessage( DrealMainWindow, "No previous zoom !" ); 
  }
  else
  {
    DrealSetMouseCursor( DrealGraphicWindow, DREAL_WATCH_CURSOR );

    DrealLambdaGridX  = DrealHeadZoom->X;
    DrealLambdaGridY  = DrealHeadZoom->Y;
    DrealLambdaGridDx = DrealHeadZoom->DX;
    DrealLambdaGridDy = DrealHeadZoom->DY;

    DrealComputeLambdaGrid();
    DrealComputeAndDisplayMap();

    DrealDisplayFigure( 0, 0,
                        DrealGraphicDx, DrealGraphicDy );

    DrealRefreshGraphicWindow( 0, 0,
                               DrealGraphicDx, DrealGraphicDy);

    DrealSetMouseCursor( DrealGraphicWindow, DREAL_NORMAL_CURSOR );

    DrealDelZoom();
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        DrealZoomRight                       |
|                                                             |
\------------------------------------------------------------*/

void DrealZoomRight()

{
  long      Delta;
  long      Offset;
  long      DrealOldPixelGridX;

  rdsbegin();

  DrealSetMouseCursor( DrealGraphicWindow, DREAL_WATCH_CURSOR );

  Delta = ( DrealPercentMove * DrealLambdaGridDx ) / 100;

  if ( Delta == 0 ) Delta = 1;

  DrealLambdaGridX   = DrealLambdaGridX + Delta;
  DrealOldPixelGridX = DrealPixelGridX;
  DrealPixelGridX    = (float)(DrealLambdaGridX) * DrealLambdaGridStep;
  Offset             = DrealPixelGridX - DrealOldPixelGridX;

  XCopyArea( DrealGraphicDisplay,
             DrealGraphicPixmap,
             DrealGraphicPixmap,
             DrealBackgroundGC,
             Offset, 0,
             DrealGraphicDx - Offset,
             DrealGraphicDy,
             0, 0
           ); 

  DrealComputeAndDisplayMap();
  DrealDisplayFigure( DrealGraphicDx - Offset, 0, 
                      DrealGraphicDx, DrealGraphicDy ); 
  DrealRefreshGraphicWindow( 0, 0,
                             DrealGraphicDx, DrealGraphicDy); 

  DrealSetMouseCursor( DrealGraphicWindow, DREAL_NORMAL_CURSOR );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        DrealZoomLeft                        |
|                                                             |
\------------------------------------------------------------*/

void DrealZoomLeft()

{
  long      Delta;
  long      Offset;
  long      DrealOldPixelGridX;

  rdsbegin();

  DrealSetMouseCursor( DrealGraphicWindow, DREAL_WATCH_CURSOR );

  Delta = ( DrealPercentMove * DrealLambdaGridDx ) / 100;

  if ( Delta == 0 ) Delta = 1;

  DrealLambdaGridX   = DrealLambdaGridX - Delta;
  DrealOldPixelGridX = DrealPixelGridX;
  DrealPixelGridX    = (float)(DrealLambdaGridX) * DrealLambdaGridStep;
  Offset             = DrealOldPixelGridX - DrealPixelGridX;

  XCopyArea( DrealGraphicDisplay,
             DrealGraphicPixmap,
             DrealGraphicPixmap,
             DrealBackgroundGC,
             0, 0,
             DrealGraphicDx - Offset,
             DrealGraphicDy,
             Offset, 0
           ); 

  DrealComputeAndDisplayMap();
  DrealDisplayFigure( 0, 0, Offset, DrealGraphicDy );
  DrealRefreshGraphicWindow( 0, 0, DrealGraphicDx, DrealGraphicDy);

  DrealSetMouseCursor( DrealGraphicWindow, DREAL_NORMAL_CURSOR );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                         DrealZoomDown                       |
|                                                             |
\------------------------------------------------------------*/

void DrealZoomDown()

{
  long      Delta;
  long      Offset;
  long      DrealOldPixelGridY;

  rdsbegin();

  DrealSetMouseCursor( DrealGraphicWindow, DREAL_WATCH_CURSOR );

  Delta = ( DrealPercentMove * DrealLambdaGridDy ) / 100;

  if ( Delta == 0 ) Delta = 1;

  DrealLambdaGridY   = DrealLambdaGridY - Delta;
  DrealOldPixelGridY = DrealPixelGridY;
  DrealPixelGridY    = (float)(DrealLambdaGridY) * DrealLambdaGridStep;
  Offset             = DrealOldPixelGridY - DrealPixelGridY;

  XCopyArea( DrealGraphicDisplay,
             DrealGraphicPixmap,
             DrealGraphicPixmap,
             DrealBackgroundGC,
             0, Offset,
             DrealGraphicDx,
             DrealGraphicDy - Offset,
             0, 0
           ); 

  DrealComputeAndDisplayMap();
  DrealDisplayFigure( 0, 0, DrealGraphicDx, Offset ); 
  DrealRefreshGraphicWindow( 0, 0, DrealGraphicDx, DrealGraphicDy);

  DrealSetMouseCursor( DrealGraphicWindow, DREAL_NORMAL_CURSOR );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                         DrealZoomUp                         |
|                                                             |
\------------------------------------------------------------*/

void DrealZoomUp()

{
  long      Delta;
  long      Offset;
  long      DrealOldPixelGridY;

  rdsbegin();

  DrealSetMouseCursor( DrealGraphicWindow, DREAL_WATCH_CURSOR );

  Delta = ( DrealPercentMove * DrealLambdaGridDy ) / 100;

  if ( Delta == 0 ) Delta = 1;

  DrealLambdaGridY   = DrealLambdaGridY + Delta;
  DrealOldPixelGridY = DrealPixelGridY;
  DrealPixelGridY    = (float)(DrealLambdaGridY) * DrealLambdaGridStep;
  Offset             = DrealPixelGridY - DrealOldPixelGridY;

  XCopyArea( DrealGraphicDisplay,
             DrealGraphicPixmap,
             DrealGraphicPixmap,
             DrealBackgroundGC,
             0, 0,
             DrealGraphicDx,
             DrealGraphicDy - Offset,
             0, Offset
           ); 

  DrealComputeAndDisplayMap();
  DrealDisplayFigure( 0, DrealGraphicDy - Offset,
                      DrealGraphicDx, DrealGraphicDy ); 
  DrealRefreshGraphicWindow( 0, 0, DrealGraphicDx, DrealGraphicDy);

  DrealSetMouseCursor( DrealGraphicWindow, DREAL_NORMAL_CURSOR );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        DrealZoomMore                        |
|                                                             |
\------------------------------------------------------------*/

void DrealZoomMore()

{
  long DeltaX;
  long DeltaY;

  rdsbegin();

  DrealSetMouseCursor( DrealGraphicWindow, DREAL_WATCH_CURSOR );

  DeltaX = ( DrealPercentZoom * DrealLambdaGridDx ) / 100;
  DeltaY = ( DrealPercentZoom * DrealLambdaGridDy ) / 100;

  if ( ( DeltaX >= 2 ) &&
       ( DeltaY >= 2 ) )
  {
    DrealAddZoom();

    DrealLambdaGridX  = DrealLambdaGridX + (DeltaX >> 1);
    DrealLambdaGridY  = DrealLambdaGridY + (DeltaY >> 1);
    DrealLambdaGridDx = DrealLambdaGridDx - DeltaX;
    DrealLambdaGridDy = DrealLambdaGridDy - DeltaY;

    DrealComputeLambdaGrid();
    DrealComputeAndDisplayMap();
    DrealDisplayFigure( 0, 0, DrealGraphicDx, DrealGraphicDy );
    DrealRefreshGraphicWindow( 0, 0, DrealGraphicDx, DrealGraphicDy );
  }

  DrealSetMouseCursor( DrealGraphicWindow, DREAL_NORMAL_CURSOR );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        DrealZoomLess                        |
|                                                             |
\------------------------------------------------------------*/

void DrealZoomLess()

{
  long DeltaX;
  long DeltaY;

  rdsbegin();

  DrealSetMouseCursor( DrealGraphicWindow, DREAL_WATCH_CURSOR );

  DeltaX = 100 * DrealLambdaGridDx / ( 100 - DrealPercentZoom );
  DeltaY = 100 * DrealLambdaGridDy / ( 100 - DrealPercentZoom );

  DrealAddZoom();
 
  DrealLambdaGridX  = DrealLambdaGridX - ((DeltaX - DrealLambdaGridDx) >> 1);
  DrealLambdaGridY  = DrealLambdaGridY - ((DeltaY - DrealLambdaGridDy) >> 1);
  DrealLambdaGridDx = DeltaX;
  DrealLambdaGridDy = DeltaY;

  DrealComputeLambdaGrid();
  DrealComputeAndDisplayMap();
  DrealDisplayFigure( 0, 0, DrealGraphicDx, DrealGraphicDy );
  DrealRefreshGraphicWindow( 0, 0, DrealGraphicDx, DrealGraphicDy );

  DrealSetMouseCursor( DrealGraphicWindow, DREAL_NORMAL_CURSOR );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        DrealZoomFit                         |
|                                                             |
\------------------------------------------------------------*/

void DrealZoomFit()

{
  rdsbegin();

  DrealSetMouseCursor( DrealGraphicWindow, DREAL_WATCH_CURSOR );

  if ( DrealComputeBound() == DREAL_FALSE )
  {
    DrealErrorMessage( DrealMainWindow, "No element to display !" );
  }
  else
  {
    DrealAddZoom();

    DrealLambdaGridX  = ( DrealBoundXmin / RDS_PHYSICAL_GRID ) - 1;
    DrealLambdaGridY  = ( DrealBoundYmin / RDS_PHYSICAL_GRID ) - 1;
    DrealLambdaGridDx = DrealBoundXmax / RDS_PHYSICAL_GRID;
    DrealLambdaGridDy = DrealBoundYmax / RDS_PHYSICAL_GRID;
    DrealLambdaGridDx = DrealLambdaGridDx - DrealLambdaGridX + 1;
    DrealLambdaGridDy = DrealLambdaGridDy - DrealLambdaGridY + 1;

    DrealComputeLambdaGrid();
    DrealComputeAndDisplayMap();
    DrealDisplayFigure( 0, 0, DrealGraphicDx, DrealGraphicDy );
    DrealRefreshGraphicWindow( 0, 0, DrealGraphicDx, DrealGraphicDy );
  }

  DrealSetMouseCursor( DrealGraphicWindow, DREAL_NORMAL_CURSOR );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        DrealZoomCenter                      |
|                                                             |
\------------------------------------------------------------*/

void DrealZoomCenter( LambdaX, LambdaY )

   long LambdaX;
   long LambdaY;
{
  rdsbegin();

  DrealSetMouseCursor( DrealGraphicWindow, DREAL_WATCH_CURSOR );

  DrealLambdaGridX = LambdaX - ( DrealLambdaGridDx >> 1 );
  DrealLambdaGridY = LambdaY - ( DrealLambdaGridDy >> 1 );

  DrealComputeLambdaGrid();
  DrealComputeAndDisplayMap();
  DrealDisplayFigure( 0, 0, DrealGraphicDx, DrealGraphicDy );
  DrealRefreshGraphicWindow( 0, 0, DrealGraphicDx, DrealGraphicDy );

  DrealSetMouseCursor( DrealGraphicWindow, DREAL_NORMAL_CURSOR );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                         DrealZoomIn                         |
|                                                             |
\------------------------------------------------------------*/
 
void DrealZoomIn( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )
 
   long LambdaX1;
   long LambdaY1;
   long LambdaX2;
   long LambdaY2;
{
  long Swap;

  rdsbegin();

  if ( ( LambdaX1 != LambdaX2 ) && 
       ( LambdaY1 != LambdaY2 ) )
  {
    if ( LambdaX1 > LambdaX2 ) 
    {
      Swap = LambdaX1; LambdaX1 = LambdaX2; LambdaX2 = Swap;
    }

    if ( LambdaY1 > LambdaY2 ) 
    {
      Swap = LambdaY1; LambdaY1 = LambdaY2; LambdaY2 = Swap;
    }

    DrealSetMouseCursor( DrealGraphicWindow, DREAL_WATCH_CURSOR );

    DrealAddZoom();

    DrealLambdaGridX  = LambdaX1;
    DrealLambdaGridY  = LambdaY1;
    DrealLambdaGridDx = LambdaX2 - LambdaX1;
    DrealLambdaGridDy = LambdaY2 - LambdaY1;

    DrealComputeLambdaGrid();
    DrealComputeAndDisplayMap();
    DrealDisplayFigure( 0, 0, DrealGraphicDx, DrealGraphicDy );
    DrealRefreshGraphicWindow( 0, 0, DrealGraphicDx, DrealGraphicDy );

    DrealSetMouseCursor( DrealGraphicWindow, DREAL_NORMAL_CURSOR );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                         DrealZoomPan                        |
|                                                             |
\------------------------------------------------------------*/

void DrealZoomPan( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )

   long LambdaX1;
   long LambdaY1;
   long LambdaX2;
   long LambdaY2;
{
  rdsbegin();

  if ( ( LambdaX1 != LambdaX2 ) ||
       ( LambdaY1 != LambdaY2 ) )
  {
    DrealSetMouseCursor( DrealGraphicWindow, DREAL_WATCH_CURSOR );
 
    DrealLambdaGridX  = DrealLambdaGridX + ( LambdaX1 - LambdaX2 );
    DrealLambdaGridY  = DrealLambdaGridY + ( LambdaY1 - LambdaY2 );

    DrealComputeLambdaGrid();
    DrealComputeAndDisplayMap();
    DrealDisplayFigure( 0, 0, DrealGraphicDx, DrealGraphicDy );
    DrealRefreshGraphicWindow( 0, 0, DrealGraphicDx, DrealGraphicDy );

    DrealSetMouseCursor( DrealGraphicWindow, DREAL_NORMAL_CURSOR );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                         DrealZoomRefresh                    |
|                                                             |
\------------------------------------------------------------*/

void DrealZoomRefresh()

{
  rdsbegin();

  DrealSetMouseCursor( DrealGraphicWindow, DREAL_WATCH_CURSOR );

  DrealDisplayFigure( 0, 0, DrealGraphicDx, DrealGraphicDy );
  DrealRefreshGraphicWindow( 0, 0, DrealGraphicDx, DrealGraphicDy );

  DrealSetMouseCursor( DrealGraphicWindow, DREAL_NORMAL_CURSOR );

  rdsend();
}
