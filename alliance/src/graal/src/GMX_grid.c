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
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                   Grid.c                          |
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
# include <Xm/Xm.h>
 
# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "GSB.h"
# include "GRM.h"
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

  float      GraalLambdaGridStep;

  long       GraalLambdaGridX;
  long       GraalLambdaGridY;
  long       GraalLambdaGridDx;
  long       GraalLambdaGridDy;

  long       GraalPixelGridX;
  long       GraalPixelGridY;

/*------------------------------------------------------------\
|                                                             |
|                      User Lambda Grid                       |
|                                                             |
\------------------------------------------------------------*/

  long       GraalLambdaUserGridDx;
  long       GraalLambdaUserGridDy;
  char       GraalLambdaUserGrid;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                GraalInitializeLambdaGrid                    |
|                                                             |
\------------------------------------------------------------*/

void GraalInitializeLambdaGrid()

{
  rdsbegin();

  GraalLambdaGridX      = GRAAL_DEFAULT_GRID_X;
  GraalLambdaGridY      = GRAAL_DEFAULT_GRID_Y;
  GraalLambdaGridDx     = GRAAL_DEFAULT_GRID_DX;
  GraalLambdaGridDy     = GRAAL_DEFAULT_GRID_DY;
  GraalLambdaUserGridDx = 4;
  GraalLambdaUserGridDy = 4;
  GraalLambdaUserGrid   = GRAAL_FALSE;

  GraalComputeLambdaGrid();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 GraalComputeLambdaGrid                      |
|                                                             |
\------------------------------------------------------------*/

void GraalComputeLambdaGrid()

{
  float StepX;
  float StepY;

  rdsbegin();

  StepX = (float)(GraalGraphicDx) / (float)(GraalLambdaGridDx);
  StepY = (float)(GraalGraphicDy) / (float)(GraalLambdaGridDy);

  if ( StepX < StepY )
  {
    GraalLambdaGridStep = StepX;
    GraalLambdaGridDy   = 1 + ( GraalGraphicDy / StepX );
  }
  else
  {
    GraalLambdaGridStep = StepY;
    GraalLambdaGridDx   = 1 + ( GraalGraphicDx / StepY );
  }

  GraalPixelGridX = (float)(GraalLambdaGridX) * GraalLambdaGridStep;
  GraalPixelGridY = (float)(GraalLambdaGridY) * GraalLambdaGridStep;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  GraalResizeLambdaGrid                      |
|                                                             |
\------------------------------------------------------------*/

void GraalResizeLambdaGrid()

{
  rdsbegin();

  GraalLambdaGridDx = 1 + ( GraalGraphicDx / GraalLambdaGridStep );
  GraalLambdaGridDy = 1 + ( GraalGraphicDy / GraalLambdaGridStep );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalDisplayLambdaGrid                  |
|                                                             |
\------------------------------------------------------------*/

char GraalDisplayLambdaGrid( GraphicX1, GraphicY1, GraphicX2, GraphicY2 )

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
  float     Check;

  rdsbegin();

  if ( GraalLambdaGridStep < GRAAL_LOWER_GRID_STEP ) 
  {
    LambdaGrid = 0;
  }
  else 
  {
    LambdaGrid = 1;
  }

  if ( GraalLambdaUserGrid == 1  )
  {
    if ( GraalLambdaUserGridDx > GraalLambdaUserGridDy )
    {
      Check = ( GRAAL_LOWER_GRID_STEP / GraalLambdaUserGridDx );
    }
    else
    {
      Check = ( GRAAL_LOWER_GRID_STEP / GraalLambdaUserGridDy );
    }

    if ( GraalLambdaGridStep < Check ) UserGrid = 0;
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
    return( GRAAL_FALSE );
  }

  MaxGridX = GraalLambdaGridX + GraalLambdaGridDx;
  MaxGridY = GraalLambdaGridY + GraalLambdaGridDy;
 
  if ( LambdaGrid )
  {
    for ( X = GraalLambdaGridX; X < MaxGridX; X = X + 1 )
    {
      PixelX = ((float)(X) * GraalLambdaGridStep);
      PixelX = PixelX - GraalPixelGridX;

      if ( ( PixelX <= GraphicX2 ) &&
           ( PixelX >= GraphicX1 ) )

      for ( Y = GraalLambdaGridY; Y < MaxGridY; Y = Y + 1 )
      {
        PixelY = ((float)(Y) * GraalLambdaGridStep);
        PixelY = PixelY - GraalPixelGridY;
        PixelY = GraalGraphicDy - PixelY;

        if ( ( PixelY <= GraphicY2 ) &&
             ( PixelY >= GraphicY1 ) )
        {
          XDrawPoint( GraalGraphicDisplay,
                      XtWindow( GraalGraphicWindow ),
                      GraalGridGC,
                      PixelX, PixelY );
        }
      }
    }
  }

  if ( UserGrid )
  {
    Xmin  = ( GraalLambdaGridX / GraalLambdaUserGridDx ) * GraalLambdaUserGridDx;
    Ymin  = ( GraalLambdaGridY / GraalLambdaUserGridDy ) * GraalLambdaUserGridDy;

    for ( X = Xmin; X < MaxGridX ; X = X + GraalLambdaUserGridDx )
    {
      PixelX = ((float)(X) * GraalLambdaGridStep);
      PixelX = PixelX - GraalPixelGridX;

      if ( ( PixelX <= GraphicX2 ) &&
           ( PixelX >= GraphicX1 ) )

      for ( Y = Ymin; Y < MaxGridY; Y = Y + GraalLambdaUserGridDy )
      {
        PixelY = ((float)(Y) * GraalLambdaGridStep);
        PixelY = PixelY - GraalPixelGridY;
        PixelY = GraalGraphicDy - PixelY;

        if ( ( PixelY <= GraphicY2 ) &&
             ( PixelY >= GraphicY1 ) )
        {
          XDrawLine( GraalGraphicDisplay,
                     XtWindow( GraalGraphicWindow ),
                     GraalGridGC,
                     PixelX - 2, PixelY,
                     PixelX + 2, PixelY );
                        
          XDrawLine( GraalGraphicDisplay,
                     XtWindow( GraalGraphicWindow ),
                     GraalGridGC,
                     PixelX, PixelY + 2,
                     PixelX, PixelY - 2 );
        }
      }
    }
  }

  rdsend();
  return( GRAAL_TRUE );
}
