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
| Tool    :                   DREAL                           |
|                                                             |
| File    :                   Grid.c                          |
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
# include "GSB.h"
# include "GRD.h"
# include "GMX.h"

# include "GMX_grid.h"

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
|                         Lambda Grid                         |
|                                                             |
\------------------------------------------------------------*/

  float      DrealLambdaGridStep;

  long       DrealLambdaGridX;
  long       DrealLambdaGridY;
  long       DrealLambdaGridDx;
  long       DrealLambdaGridDy;

  long       DrealPixelGridX;
  long       DrealPixelGridY;

/*------------------------------------------------------------\
|                                                             |
|                      User Lambda Grid                       |
|                                                             |
\------------------------------------------------------------*/

  long       DrealLambdaUserGridDx;
  long       DrealLambdaUserGridDy;
  char       DrealLambdaUserGrid;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                DrealInitializeLambdaGrid                    |
|                                                             |
\------------------------------------------------------------*/

void DrealInitializeLambdaGrid()

{
  rdsbegin();

  DrealLambdaGridX      = DREAL_DEFAULT_GRID_X;
  DrealLambdaGridY      = DREAL_DEFAULT_GRID_Y;
  DrealLambdaGridDx     = DREAL_DEFAULT_GRID_DX;
  DrealLambdaGridDy     = DREAL_DEFAULT_GRID_DY;
  DrealLambdaUserGridDx = 4;
  DrealLambdaUserGridDy = 4;
  DrealLambdaUserGrid   = DREAL_FALSE;

  DrealComputeLambdaGrid();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 DrealComputeLambdaGrid                      |
|                                                             |
\------------------------------------------------------------*/

void DrealComputeLambdaGrid()

{
  float StepX;
  float StepY;

  rdsbegin();

  StepX = (float)(DrealGraphicDx) / (float)(DrealLambdaGridDx);
  StepY = (float)(DrealGraphicDy) / (float)(DrealLambdaGridDy);

  if ( StepX < StepY )
  {
    DrealLambdaGridStep = StepX;
    DrealLambdaGridDy   = 1 + ( DrealGraphicDy / StepX );
  }
  else
  {
    DrealLambdaGridStep = StepY;
    DrealLambdaGridDx   = 1 + ( DrealGraphicDx / StepY );
  }

  DrealPixelGridX = (float)(DrealLambdaGridX) * DrealLambdaGridStep;
  DrealPixelGridY = (float)(DrealLambdaGridY) * DrealLambdaGridStep;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  DrealResizeLambdaGrid                      |
|                                                             |
\------------------------------------------------------------*/

void DrealResizeLambdaGrid()

{
  rdsbegin();

  DrealLambdaGridDx = 1 + ( DrealGraphicDx / DrealLambdaGridStep );
  DrealLambdaGridDy = 1 + ( DrealGraphicDy / DrealLambdaGridStep );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     DrealDisplayLambdaGrid                  |
|                                                             |
\------------------------------------------------------------*/

char DrealDisplayLambdaGrid( GraphicX1, GraphicY1, GraphicX2, GraphicY2 )

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
  char      LambdaGrid;
  char      TraceCroix;
  char      TracePoint;
  float     Check;

  rdsbegin();

  if ( DrealLambdaGridStep < DREAL_LOWER_GRID_STEP ) 
  {
    LambdaGrid = 0;
  }
  else 
  {
    LambdaGrid = 1;
  }

  if ( DrealLambdaUserGrid == 1  )
  {
    if ( DrealLambdaUserGridDx > DrealLambdaUserGridDy )
    {
      Check = ( DREAL_LOWER_GRID_STEP * 2 / DrealLambdaUserGridDy );
    }
    else
    {
      Check = ( DREAL_LOWER_GRID_STEP * 2 / DrealLambdaUserGridDy );
    }

    if ( DrealLambdaGridStep < Check ) UserGrid = 0;
    else                               UserGrid = 1;
  } 
  else
  {
    UserGrid = 0;
  }

  if ( ( UserGrid   == 0 ) &&
       ( LambdaGrid == 0 ) )
  {
    rdsend();
    return( DREAL_FALSE );
  }

  MaxGridX = DrealLambdaGridX + DrealLambdaGridDx;
  MaxGridY = DrealLambdaGridY + DrealLambdaGridDy;
 
  if ( LambdaGrid )
  {
    for ( X = DrealLambdaGridX; X < MaxGridX; X = X + 1 )
    {
      PixelX = ((float)(X) * DrealLambdaGridStep);
      PixelX = PixelX - DrealPixelGridX;

      if ( ( PixelX <= GraphicX2 ) &&
           ( PixelX >= GraphicX1 ) )

      for ( Y = DrealLambdaGridY; Y < MaxGridY; Y = Y + 1 )
      {
        PixelY = ((float)(Y) * DrealLambdaGridStep);
        PixelY = PixelY - DrealPixelGridY;
        PixelY = DrealGraphicDy - PixelY;

        if ( ( PixelY <= GraphicY2 ) &&
             ( PixelY >= GraphicY1 ) )
        {
          XDrawPoint( DrealGraphicDisplay,
                      XtWindow( DrealGraphicWindow ),
                      DrealGridGC,
                      PixelX, PixelY );
        }
      }
    }
  }

  if ( UserGrid )
  {
    Xmin  = ( DrealLambdaGridX / DrealLambdaUserGridDx ) * DrealLambdaUserGridDx;
    Ymin  = ( DrealLambdaGridY / DrealLambdaUserGridDy ) * DrealLambdaUserGridDy;

    for ( X = Xmin; X < MaxGridX ; X = X + DrealLambdaUserGridDx )
    {
      PixelX = ((float)(X) * DrealLambdaGridStep);
      PixelX = PixelX - DrealPixelGridX;

      if ( ( PixelX <= GraphicX2 ) &&
           ( PixelX >= GraphicX1 ) )

      for ( Y = Ymin; Y < MaxGridY; Y = Y + DrealLambdaUserGridDy )
      {
        PixelY = ((float)(Y) * DrealLambdaGridStep);
        PixelY = PixelY - DrealPixelGridY;
        PixelY = DrealGraphicDy - PixelY;

        if ( ( PixelY <= GraphicY2 ) &&
             ( PixelY >= GraphicY1 ) )
        {
          XDrawLine( DrealGraphicDisplay,
                     XtWindow( DrealGraphicWindow ),
                     DrealGridGC,
                     PixelX - 2, PixelY,
                     PixelX + 2, PixelY );
                        
          XDrawLine( DrealGraphicDisplay,
                     XtWindow( DrealGraphicWindow ),
                     DrealGridGC,
                     PixelX, PixelY + 2,
                     PixelX, PixelY - 2 );
        }
      }
    }
  }


  rdsend();
  return( DREAL_TRUE );
}
