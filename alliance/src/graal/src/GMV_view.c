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
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                   View.c                          |
|                                                             |
| Author  :                Jacomme Ludovic                    |
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
# include <stdlib.h>
# include <Xm/Xm.h>

# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "rfm.h"
# include "GRM.h"
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

 graalzoom *GraalHeadZoom    = (graalzoom *)NULL;

 int        GraalPercentZoom  = GRAAL_PERCENT_ZOOM;
 int        GraalPercentMoveX = GRAAL_PERCENT_MOVE;
 int        GraalPercentMoveY = GRAAL_PERCENT_MOVE;

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
|                       GraalAllocZoom                        |
|                                                             |
\------------------------------------------------------------*/

graalzoom *GraalAllocZoom ()

{
  return ( (graalzoom *) rdsalloc ( sizeof ( graalzoom ), 1));
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalFreeZoom                         |
|                                                             |
\------------------------------------------------------------*/

void GraalFreeZoom ( FreeZoom )

  graalzoom *FreeZoom;

{
  rdsfree( (char *)FreeZoom, sizeof(FreeZoom) );
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalAddZoom                          |
|                                                             |
\------------------------------------------------------------*/
 
void GraalAddZoom ()
 
{
  graalzoom *GraalZoom;

  rdsbegin();
 
  GraalZoom       = GraalAllocZoom ();
  GraalZoom->X    = GraalLambdaGridX;
  GraalZoom->Y    = GraalLambdaGridY;
  GraalZoom->DX   = GraalLambdaGridDx;
  GraalZoom->DY   = GraalLambdaGridDy;
  GraalZoom->NEXT = GraalHeadZoom;
  GraalHeadZoom   = GraalZoom;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                         GraalDelZoom                        |
|                                                             |
\------------------------------------------------------------*/

char GraalDelZoom()

{
  graalzoom *GraalZoom;

  if ( GraalHeadZoom != (graalzoom *) NULL )
  {
    rdsbegin();

    GraalZoom     = GraalHeadZoom;
    GraalHeadZoom = GraalHeadZoom->NEXT;
    GraalFreeZoom( GraalZoom );

    rdsend();
    return( GRAAL_TRUE ); 
  }

  return( GRAAL_FALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalInitializeZoom                     |
|                                                             |
\------------------------------------------------------------*/

void GraalInitializeZoom()

{
  rdsbegin();

  while( GraalDelZoom() != GRAAL_FALSE );

  if ( GraalComputeBound() == GRAAL_FALSE )
  {
    GraalInitializeLambdaGrid();
  }
  else
  {
    GraalLambdaGridX  = ( GraalBoundXmin / GRAAL_RDS_LAMBDA ) - 1;
    GraalLambdaGridY  = ( GraalBoundYmin / GRAAL_RDS_LAMBDA ) - 1;
    GraalLambdaGridDx = GraalBoundXmax / GRAAL_RDS_LAMBDA;
    GraalLambdaGridDy = GraalBoundYmax / GRAAL_RDS_LAMBDA;
    GraalLambdaGridDx = GraalLambdaGridDx - GraalLambdaGridX + 1;
    GraalLambdaGridDy = GraalLambdaGridDy - GraalLambdaGridY + 1;

    GraalComputeLambdaGrid();
  }

  GraalComputeAndDisplayMap();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalZoomUndo                        |
|                                                             |
\------------------------------------------------------------*/

void GraalZoomUndo()

{
  rdsbegin();

  if ( GraalHeadZoom == (graalzoom *)NULL )
  {
    GraalErrorMessage( GraalMainWindow, "No previous zoom !" ); 
  }
  else
  {
    GraalSetMouseCursor( GraalGraphicWindow, GRAAL_WATCH_CURSOR );

    GraalLambdaGridX  = GraalHeadZoom->X;
    GraalLambdaGridY  = GraalHeadZoom->Y;
    GraalLambdaGridDx = GraalHeadZoom->DX;
    GraalLambdaGridDy = GraalHeadZoom->DY;

    GraalComputeLambdaGrid();
    GraalComputeAndDisplayMap();

    GraalDisplayFigure( 0, 0,
                        GraalGraphicDx, GraalGraphicDy );

    GraalRefreshGraphicWindow( 0, 0,
                               GraalGraphicDx, GraalGraphicDy);

    GraalSetMouseCursor( GraalGraphicWindow, GRAAL_NORMAL_CURSOR );

    GraalDelZoom();
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalZoomRight                       |
|                                                             |
\------------------------------------------------------------*/

void GraalZoomRight()

{
  long      Delta;
  long      Offset;
  long      GraalOldPixelGridX;

  rdsbegin();

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_WATCH_CURSOR );

  Delta = ( GraalPercentMoveX * GraalLambdaGridDx ) / 100;

  if ( Delta == 0 ) Delta = 1;

  GraalLambdaGridX   = GraalLambdaGridX + Delta;
  GraalOldPixelGridX = GraalPixelGridX;
  GraalPixelGridX    = (float)(GraalLambdaGridX) * GraalLambdaGridStep;
  Offset             = GraalPixelGridX - GraalOldPixelGridX;

  XCopyArea( GraalGraphicDisplay,
             GraalGraphicPixmap,
             GraalGraphicPixmap,
             GraalBackgroundGC,
             Offset, 0,
             GraalGraphicDx - Offset,
             GraalGraphicDy,
             0, 0
           ); 

  GraalComputeAndDisplayMap();
  GraalDisplayFigure( GraalGraphicDx - Offset, 0, 
                      GraalGraphicDx, GraalGraphicDy ); 
  GraalRefreshGraphicWindow( 0, 0,
                             GraalGraphicDx, GraalGraphicDy); 

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_NORMAL_CURSOR );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalZoomLeft                        |
|                                                             |
\------------------------------------------------------------*/

void GraalZoomLeft()

{
  long      Delta;
  long      Offset;
  long      GraalOldPixelGridX;

  rdsbegin();

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_WATCH_CURSOR );

  Delta = ( GraalPercentMoveX * GraalLambdaGridDx ) / 100;

  if ( Delta == 0 ) Delta = 1;

  GraalLambdaGridX   = GraalLambdaGridX - Delta;
  GraalOldPixelGridX = GraalPixelGridX;
  GraalPixelGridX    = (float)(GraalLambdaGridX) * GraalLambdaGridStep;
  Offset             = GraalOldPixelGridX - GraalPixelGridX;

  XCopyArea( GraalGraphicDisplay,
             GraalGraphicPixmap,
             GraalGraphicPixmap,
             GraalBackgroundGC,
             0, 0,
             GraalGraphicDx - Offset,
             GraalGraphicDy,
             Offset, 0
           ); 

  GraalComputeAndDisplayMap();
  GraalDisplayFigure( 0, 0, Offset, GraalGraphicDy );
  GraalRefreshGraphicWindow( 0, 0, GraalGraphicDx, GraalGraphicDy);

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_NORMAL_CURSOR );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                         GraalZoomDown                       |
|                                                             |
\------------------------------------------------------------*/

void GraalZoomDown()

{
  long      Delta;
  long      Offset;
  long      GraalOldPixelGridY;

  rdsbegin();

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_WATCH_CURSOR );

  Delta = ( GraalPercentMoveY * GraalLambdaGridDy ) / 100;

  if ( Delta == 0 ) Delta = 1;

  GraalLambdaGridY   = GraalLambdaGridY - Delta;
  GraalOldPixelGridY = GraalPixelGridY;
  GraalPixelGridY    = (float)(GraalLambdaGridY) * GraalLambdaGridStep;
  Offset             = GraalOldPixelGridY - GraalPixelGridY;

  XCopyArea( GraalGraphicDisplay,
             GraalGraphicPixmap,
             GraalGraphicPixmap,
             GraalBackgroundGC,
             0, Offset,
             GraalGraphicDx,
             GraalGraphicDy - Offset,
             0, 0
           ); 

  GraalComputeAndDisplayMap();
  GraalDisplayFigure( 0, 0, GraalGraphicDx, Offset ); 
  GraalRefreshGraphicWindow( 0, 0, GraalGraphicDx, GraalGraphicDy);

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_NORMAL_CURSOR );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                         GraalZoomUp                         |
|                                                             |
\------------------------------------------------------------*/

void GraalZoomUp()

{
  long      Delta;
  long      Offset;
  long      GraalOldPixelGridY;

  rdsbegin();

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_WATCH_CURSOR );

  Delta = ( GraalPercentMoveY * GraalLambdaGridDy ) / 100;

  if ( Delta == 0 ) Delta = 1;

  GraalLambdaGridY   = GraalLambdaGridY + Delta;
  GraalOldPixelGridY = GraalPixelGridY;
  GraalPixelGridY    = (float)(GraalLambdaGridY) * GraalLambdaGridStep;
  Offset             = GraalPixelGridY - GraalOldPixelGridY;

  XCopyArea( GraalGraphicDisplay,
             GraalGraphicPixmap,
             GraalGraphicPixmap,
             GraalBackgroundGC,
             0, 0,
             GraalGraphicDx,
             GraalGraphicDy - Offset,
             0, Offset
           ); 

  GraalComputeAndDisplayMap();
  GraalDisplayFigure( 0, GraalGraphicDy - Offset,
                      GraalGraphicDx, GraalGraphicDy ); 
  GraalRefreshGraphicWindow( 0, 0, GraalGraphicDx, GraalGraphicDy);

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_NORMAL_CURSOR );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalZoomMore                        |
|                                                             |
\------------------------------------------------------------*/

void GraalZoomMore()

{
  long DeltaX;
  long DeltaY;

  rdsbegin();

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_WATCH_CURSOR );

  DeltaX = ( GraalPercentZoom * GraalLambdaGridDx ) / 100;
  DeltaY = ( GraalPercentZoom * GraalLambdaGridDy ) / 100;

  if ( ( DeltaX >= 2 ) &&
       ( DeltaY >= 2 ) )
  {
    GraalAddZoom();

    GraalLambdaGridX  = GraalLambdaGridX + (DeltaX >> 1);
    GraalLambdaGridY  = GraalLambdaGridY + (DeltaY >> 1);
    GraalLambdaGridDx = GraalLambdaGridDx - DeltaX;
    GraalLambdaGridDy = GraalLambdaGridDy - DeltaY;

    GraalComputeLambdaGrid();
    GraalComputeAndDisplayMap();
    GraalDisplayFigure( 0, 0, GraalGraphicDx, GraalGraphicDy );
    GraalRefreshGraphicWindow( 0, 0, GraalGraphicDx, GraalGraphicDy );
  }

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_NORMAL_CURSOR );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalZoomLess                        |
|                                                             |
\------------------------------------------------------------*/

void GraalZoomLess()

{
  long DeltaX;
  long DeltaY;

  rdsbegin();

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_WATCH_CURSOR );

  DeltaX = 100 * GraalLambdaGridDx / ( 100 - GraalPercentZoom );
  DeltaY = 100 * GraalLambdaGridDy / ( 100 - GraalPercentZoom );

  GraalAddZoom();
 
  GraalLambdaGridX  = GraalLambdaGridX - ((DeltaX - GraalLambdaGridDx) >> 1);
  GraalLambdaGridY  = GraalLambdaGridY - ((DeltaY - GraalLambdaGridDy) >> 1);
  GraalLambdaGridDx = DeltaX;
  GraalLambdaGridDy = DeltaY;

  GraalComputeLambdaGrid();
  GraalComputeAndDisplayMap();
  GraalDisplayFigure( 0, 0, GraalGraphicDx, GraalGraphicDy );
  GraalRefreshGraphicWindow( 0, 0, GraalGraphicDx, GraalGraphicDy );

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_NORMAL_CURSOR );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalZoomFit                         |
|                                                             |
\------------------------------------------------------------*/

void GraalZoomFit()

{
  rdsbegin();

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_WATCH_CURSOR );

  if ( GraalComputeBound() == GRAAL_FALSE )
  {
    GraalErrorMessage( GraalMainWindow, "No element to display !" );
  }
  else
  {
    GraalAddZoom();

    GraalLambdaGridX  = ( GraalBoundXmin / GRAAL_RDS_LAMBDA ) - 1;
    GraalLambdaGridY  = ( GraalBoundYmin / GRAAL_RDS_LAMBDA ) - 1;
    GraalLambdaGridDx = GraalBoundXmax / GRAAL_RDS_LAMBDA;
    GraalLambdaGridDy = GraalBoundYmax / GRAAL_RDS_LAMBDA;
    GraalLambdaGridDx = GraalLambdaGridDx - GraalLambdaGridX + 1;
    GraalLambdaGridDy = GraalLambdaGridDy - GraalLambdaGridY + 1;

    GraalComputeLambdaGrid();
    GraalComputeAndDisplayMap();
    GraalDisplayFigure( 0, 0, GraalGraphicDx, GraalGraphicDy );
    GraalRefreshGraphicWindow( 0, 0, GraalGraphicDx, GraalGraphicDy );
  }

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_NORMAL_CURSOR );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalZoomCenter                      |
|                                                             |
\------------------------------------------------------------*/

void GraalZoomCenter( LambdaX, LambdaY )

   long LambdaX;
   long LambdaY;
{
  rdsbegin();

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_WATCH_CURSOR );

  GraalLambdaGridX = LambdaX - ( GraalLambdaGridDx >> 1 );
  GraalLambdaGridY = LambdaY - ( GraalLambdaGridDy >> 1 );

  GraalComputeLambdaGrid();
  GraalComputeAndDisplayMap();
  GraalDisplayFigure( 0, 0, GraalGraphicDx, GraalGraphicDy );
  GraalRefreshGraphicWindow( 0, 0, GraalGraphicDx, GraalGraphicDy );

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_NORMAL_CURSOR );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                         GraalZoomIn                         |
|                                                             |
\------------------------------------------------------------*/
 
void GraalZoomIn( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )
 
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

    GraalSetMouseCursor( GraalGraphicWindow, GRAAL_WATCH_CURSOR );

    GraalAddZoom();

    GraalLambdaGridX  = LambdaX1;
    GraalLambdaGridY  = LambdaY1;
    GraalLambdaGridDx = LambdaX2 - LambdaX1;
    GraalLambdaGridDy = LambdaY2 - LambdaY1;

    GraalComputeLambdaGrid();
    GraalComputeAndDisplayMap();
    GraalDisplayFigure( 0, 0, GraalGraphicDx, GraalGraphicDy );
    GraalRefreshGraphicWindow( 0, 0, GraalGraphicDx, GraalGraphicDy );

    GraalSetMouseCursor( GraalGraphicWindow, GRAAL_NORMAL_CURSOR );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                         GraalZoomPan                        |
|                                                             |
\------------------------------------------------------------*/

void GraalZoomPan( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )

   long LambdaX1;
   long LambdaY1;
   long LambdaX2;
   long LambdaY2;
{
  if ( ( LambdaX1 != LambdaX2 ) ||
       ( LambdaY1 != LambdaY2 ) )
  {
    rdsbegin();

    GraalSetMouseCursor( GraalGraphicWindow, GRAAL_WATCH_CURSOR );
 
    GraalLambdaGridX  = GraalLambdaGridX + ( LambdaX1 - LambdaX2 );
    GraalLambdaGridY  = GraalLambdaGridY + ( LambdaY1 - LambdaY2 );

    GraalComputeLambdaGrid();
    GraalComputeAndDisplayMap();
    GraalDisplayFigure( 0, 0, GraalGraphicDx, GraalGraphicDy );
    GraalRefreshGraphicWindow( 0, 0, GraalGraphicDx, GraalGraphicDy );

    GraalSetMouseCursor( GraalGraphicWindow, GRAAL_NORMAL_CURSOR );

    rdsend();
  }
}

/*------------------------------------------------------------\
|                                                             |
|                         GraalZoomRefresh                    |
|                                                             |
\------------------------------------------------------------*/

void GraalZoomRefresh()

{
  rdsbegin();

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_WATCH_CURSOR );

  GraalDisplayFigure( 0, 0, GraalGraphicDx, GraalGraphicDy );
  GraalRefreshGraphicWindow( 0, 0, GraalGraphicDx, GraalGraphicDy );

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_NORMAL_CURSOR );

  rdsend();
}
