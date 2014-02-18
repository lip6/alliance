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
| Tool    :                   DREAL                           |
|                                                             |
| File    :                  Cursor.c                         |
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
# include <X11/Intrinsic.h>
# include <X11/StringDefs.h>
# include <Xm/Xm.h>

# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "GSB.h"
# include "GMX.h"
# include "GMX_view.h"
# include "GMX_grid.h"
# include "GMX_cursor.h"

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
|                            Buffer                           |
|                                                             |
\------------------------------------------------------------*/

  static char  DrealCursorBuffer[ DREAL_MAX_CURSOR_BUFFER ];

/*------------------------------------------------------------\
|                                                             |
|                         Coordinates                         |
|                                                             |
\------------------------------------------------------------*/

  Position DrealCursorX       = 0;
  Position DrealCursorY       = 0;
  Position DrealCursorSaveX   = 0;
  Position DrealCursorSaveY   = 0;
  char     DrealCursorSaved   = DREAL_FALSE;
  char     DrealCursorInside  = DREAL_FALSE;
  char     DrealCursorType    = DREAL_INPUT_HALF_BOX;

  long     DrealLambdaCursorX = 0;
  long     DrealLambdaCursorY = 0;
  long     DrealPixelCursorX  = 0;
  long     DrealPixelCursorY  = 0;

  long     DrealLambdaCursorSaveX[ 2 ] = { 0, 0 };
  long     DrealLambdaCursorSaveY[ 2 ] = { 0, 0 };
  char     DrealCursorIndex            = 0;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      DrealComputeCursor                     |
|                                                             |
\------------------------------------------------------------*/

void DrealComputeCursor( X, Y )

     Position X;
     Position Y;
{
  rdsbegin();

  Y = DrealGraphicDy - Y;

  DrealLambdaCursorX = X + DrealPixelGridX;
  DrealLambdaCursorY = Y + DrealPixelGridY;

  if ( DrealLambdaCursorX < 0 )
  {
    DrealLambdaCursorX = ((float)(DrealLambdaCursorX) / DrealLambdaGridStep) - 0.5 ;
  }
  else
  {
    DrealLambdaCursorX = ((float)(DrealLambdaCursorX) / DrealLambdaGridStep) + 0.5 ;
  }

  if ( DrealLambdaCursorY < 0 )
  {
    DrealLambdaCursorY = ((float)(DrealLambdaCursorY) / DrealLambdaGridStep) - 0.5 ;
  }
  else
  {
    DrealLambdaCursorY = ((float)(DrealLambdaCursorY) / DrealLambdaGridStep) + 0.5 ;
  }

  DrealPixelCursorX  = ((float)(DrealLambdaCursorX) * DrealLambdaGridStep);
  DrealPixelCursorY  = ((float)(DrealLambdaCursorY) * DrealLambdaGridStep);

  DrealCursorX = DrealPixelCursorX - DrealPixelGridX;
  DrealCursorY = DrealPixelCursorY - DrealPixelGridY;
  DrealCursorY = DrealGraphicDy - DrealCursorY;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    DrealDisplayCoordinates                  |
|                                                             |
\------------------------------------------------------------*/

void DrealDisplayCoordinates()

{
  rdsbegin();

  sprintf( DrealCursorBuffer, "%.3f", 
    (float)( DrealLambdaCursorX ) * DREAL_PHYSICAL_GRID_TO_MICRON );
  DrealDisplayMessage( DREAL_MESSAGE_X, DrealCursorBuffer );

  sprintf( DrealCursorBuffer, "%.3f", 
    (float)( DrealLambdaCursorY )  * DREAL_PHYSICAL_GRID_TO_MICRON );
  DrealDisplayMessage( DREAL_MESSAGE_Y, DrealCursorBuffer );

  sprintf( DrealCursorBuffer, "%.3f", 
    (float)( DrealLambdaCursorX - DrealLambdaCursorSaveX[0] ) * 
    DREAL_PHYSICAL_GRID_TO_MICRON );
  DrealDisplayMessage( DREAL_MESSAGE_DX, DrealCursorBuffer );

  sprintf( DrealCursorBuffer, "%.3f", 
    (float)( DrealLambdaCursorY - DrealLambdaCursorSaveY[0] ) *
    DREAL_PHYSICAL_GRID_TO_MICRON );
  DrealDisplayMessage( DREAL_MESSAGE_DY, DrealCursorBuffer );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        DrealPointCursor                     |
|                                                             |
\------------------------------------------------------------*/

void DrealPointCursor()

{
  rdsbegin();

  DrealUndisplayCursor();

  DrealLambdaCursorSaveX[ (int)DrealCursorIndex ] = DrealLambdaCursorX;
  DrealLambdaCursorSaveY[ (int)DrealCursorIndex ] = DrealLambdaCursorY;

  DrealCursorIndex = DrealCursorIndex + 1;

  DrealDisplayCursor();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        DrealResetCursor                     |
|                                                             |
\------------------------------------------------------------*/

void DrealResetCursor()

{
  rdsbegin();

  DrealUndisplayCursor();

  DrealCursorIndex = DrealCursorIndex - 1;

  DrealDisplayCursor();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        DrealChangeCursor                    |
|                                                             |
\------------------------------------------------------------*/

void DrealChangeCursorType( ArrayX, ArrayY, Index, Type )

   long *ArrayX;
   long *ArrayY;
   char  Index;
   char  Type;
{
  rdsbegin();

  DrealUndisplayCursor();

  DrealCursorType  = Type;
  DrealCursorIndex = Index;

  if ( Index != 0 )
  {
    DrealLambdaCursorSaveX[ 0 ] = ArrayX[0];
    DrealLambdaCursorSaveY[ 0 ] = ArrayY[0];
    DrealLambdaCursorSaveX[ 1 ] = ArrayX[1];
    DrealLambdaCursorSaveY[ 1 ] = ArrayY[1];
  }
  
  DrealDisplayCursor();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        DrealDrawCursor                      |
|                                                             |
\------------------------------------------------------------*/

void DrealDrawCursor()

{
  long X1;
  long Y1;
  long X2;
  long Y2;
  long DeltaX;
  long DeltaY;
  long Swap;
  char DrawLine;

  rdsbegin();

  XDrawLine( DrealGraphicDisplay,
             XtWindow( DrealGraphicWindow ),
             DrealXorGC,
             DrealCursorSaveX - DREAL_CURSOR_SIZE, 
             DrealCursorSaveY - DREAL_CURSOR_SIZE,
             DrealCursorSaveX + DREAL_CURSOR_SIZE, 
             DrealCursorSaveY + DREAL_CURSOR_SIZE );

  XDrawLine( DrealGraphicDisplay,
             XtWindow( DrealGraphicWindow ),
             DrealXorGC,
             DrealCursorSaveX - DREAL_CURSOR_SIZE, 
             DrealCursorSaveY + DREAL_CURSOR_SIZE,
             DrealCursorSaveX + DREAL_CURSOR_SIZE,
             DrealCursorSaveY - DREAL_CURSOR_SIZE );

  if ( DrealCursorIndex > 0 )
  {
    X1 = (float)(DrealLambdaCursorSaveX[0]) * DrealLambdaGridStep;
    Y1 = (float)(DrealLambdaCursorSaveY[0]) * DrealLambdaGridStep;
    X1 = X1 - DrealPixelGridX;
    Y1 = Y1 - DrealPixelGridY;
    Y1 = DrealGraphicDy - Y1;

    if ( DrealCursorIndex == 1 )
    {
      X2 = DrealCursorSaveX;
      Y2 = DrealCursorSaveY;
    }
    else
    {
      X2 = (float)(DrealLambdaCursorSaveX[1]) * DrealLambdaGridStep;
      Y2 = (float)(DrealLambdaCursorSaveY[1]) * DrealLambdaGridStep;
      X2 = X2 - DrealPixelGridX;
      Y2 = Y2 - DrealPixelGridY;
      Y2 = DrealGraphicDy - Y2;
    }

    switch( DrealCursorType )
    {
      case DREAL_INPUT_POINT :

        break;

      case DREAL_INPUT_LINE  :

        XDrawLine( DrealGraphicDisplay,
                   XtWindow( DrealGraphicWindow ),
                   DrealXorGC,
                   X1, Y1, X2, Y2 );
        break;

      case DREAL_INPUT_HALF_BOX :

        XDrawLine( DrealGraphicDisplay,
                   XtWindow( DrealGraphicWindow ),
                   DrealXorGC,
                   X1, Y1, X2, Y1 );

        XDrawLine( DrealGraphicDisplay,
                   XtWindow( DrealGraphicWindow ),
                   DrealXorGC,
                   X2, Y1, X2, Y2 );
        break; 

      case DREAL_INPUT_ORTHO   :
      case DREAL_INPUT_SORTHO  :
      case DREAL_INPUT_LSTRING :

        DeltaX = X2 - X1; if ( DeltaX < 0 ) DeltaX = - DeltaX;
        DeltaY = Y2 - Y1; if ( DeltaY < 0 ) DeltaY = - DeltaY;

        if ( DeltaX > DeltaY )
        {
          Y2 = Y1;

          if ( X1 > X2 ) { Swap = X1; X1 = X2; X2 = Swap; }

          if ( X1 < 0              ) X1 = 0;
          if ( X2 > DrealGraphicDx ) X2 = DrealGraphicDx;

          if ( ( X1 <  X2             ) &&
               ( Y1 >= 0              ) &&
               ( Y1 <= DrealGraphicDy ) )
          {
            XDrawLine( DrealGraphicDisplay,
                       XtWindow( DrealGraphicWindow ),
                       DrealXorGC,
                       X1, Y1, X2, Y2 );
          }
        }
        else
        {
          X2 = X1;

          if ( Y1 > Y2 ) { Swap = Y1; Y1 = Y2; Y2 = Swap; }

          if ( Y1 < 0              ) Y1 = 0;
          if ( Y2 > DrealGraphicDy ) Y2 = DrealGraphicDy;

          if ( ( Y1 <  Y2             ) &&
               ( X1 >= 0              ) &&
               ( X1 <= DrealGraphicDx ) )
          {
            XDrawLine( DrealGraphicDisplay,
                       XtWindow( DrealGraphicWindow ), 
                       DrealXorGC,   
                       X1, Y1, X2, Y2 ); 
          }
        }

        break;

      case DREAL_INPUT_BOX :
 
        if ( X1 > X2 ) { Swap = X1; X1 = X2; X2 = Swap; }
        if ( Y1 > Y2 ) { Swap = Y1; Y1 = Y2; Y2 = Swap; }

        DrawLine = 0;

        if ( X1 < 0 ) 
        {
          X1 = 0; DrawLine |= DREAL_WEST_MASK;
        }

        if ( X2 > DrealGraphicDx ) 
        {
          X2 = DrealGraphicDx; DrawLine |= DREAL_EAST_MASK;
        }

        if ( Y1 < 0 ) 
        {
          Y1 = 0; DrawLine |= DREAL_SOUTH_MASK;
        }

        if ( Y2 > DrealGraphicDy ) 
        {
          Y2 = DrealGraphicDy; DrawLine |= DREAL_NORTH_MASK;
        }

        if ( DrawLine == 0 )
        {
          XDrawRectangle( DrealGraphicDisplay,
                          XtWindow( DrealGraphicWindow ),
                          DrealXorGC,
                          X1, Y1,
                          X2 - X1, Y2 - Y1 );
        }
        else 
        {
          if ( ( DrawLine & DREAL_WEST_MASK ) == 0 )
          {
            XDrawLine( DrealGraphicDisplay,
                       XtWindow( DrealGraphicWindow ),
                       DrealXorGC,
                       X1, Y2,
                       X1, Y1 );
          }

          if ( ( DrawLine & DREAL_EAST_MASK ) == 0 )
          {
            XDrawLine( DrealGraphicDisplay,
                       XtWindow( DrealGraphicWindow ),
                       DrealXorGC,
                       X2, Y2,
                       X2, Y1 );
          }

          if ( ( DrawLine & DREAL_SOUTH_MASK ) == 0 )
          {
            XDrawLine( DrealGraphicDisplay,
                       XtWindow( DrealGraphicWindow ),
                       DrealXorGC,
                       X1, Y1,
                       X2, Y1 );
          }

          if ( ( DrawLine & DREAL_NORTH_MASK ) == 0 )
          {
            XDrawLine( DrealGraphicDisplay,
                       XtWindow( DrealGraphicWindow ),
                       DrealXorGC,
                       X1, Y2,
                       X2, Y2 );
          }
        }

      break;

    }
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    DrealUndisplayCursor                     |
|                                                             |
\------------------------------------------------------------*/

void DrealUndisplayCursor()

{
  rdsbegin();

  if ( DrealCursorInside == DREAL_TRUE )
  {
    if ( DrealCursorSaved == DREAL_TRUE )
    {
      DrealDrawCursor();
    }

    DrealCursorSaved = DREAL_FALSE;
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealDisplayCursor                     |
|                                                             |
\------------------------------------------------------------*/

void DrealDisplayCursor()

{
  rdsbegin();

  if ( DrealCursorInside == DREAL_TRUE )
  {
    if ( DrealCursorSaved == DREAL_TRUE )
    {
      DrealDrawCursor();

      DrealCursorSaved = DREAL_FALSE;
    }

    if ( ( DrealCursorY >= 0              ) &&
         ( DrealCursorX <= DrealGraphicDx ) )
    {
      DrealCursorSaveX = DrealCursorX;
      DrealCursorSaveY = DrealCursorY;

      DrealDrawCursor();

      DrealCursorSaved = DREAL_TRUE;
    }
  }

  rdsend();
}
