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
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                  Cursor.c                         |
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

  static char  GraalCursorBuffer[ GRAAL_MAX_CURSOR_BUFFER ];

/*------------------------------------------------------------\
|                                                             |
|                         Coordinates                         |
|                                                             |
\------------------------------------------------------------*/

  Position GraalCursorX       = 0;
  Position GraalCursorY       = 0;
  Position GraalCursorSaveX   = 0;
  Position GraalCursorSaveY   = 0;
  char     GraalCursorSaved   = GRAAL_FALSE;
  char     GraalCursorInside  = GRAAL_FALSE;
  char     GraalCursorType    = GRAAL_INPUT_HALF_BOX;

  long     GraalLambdaCursorX = 0;
  long     GraalLambdaCursorY = 0;
  long     GraalPixelCursorX  = 0;
  long     GraalPixelCursorY  = 0;

  long     GraalLambdaCursorSaveX[ 2 ] = { 0, 0 };
  long     GraalLambdaCursorSaveY[ 2 ] = { 0, 0 };
  char     GraalCursorIndex            = 0;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      GraalComputeCursor                     |
|                                                             |
\------------------------------------------------------------*/

void GraalComputeCursor( X, Y )

     Position X;
     Position Y;
{
  rdsbegin();

  Y = GraalGraphicDy - Y;

  GraalLambdaCursorX = X + GraalPixelGridX;
  GraalLambdaCursorY = Y + GraalPixelGridY;

  if ( GraalLambdaCursorX < 0 )
  {
    GraalLambdaCursorX = ((float)(GraalLambdaCursorX) / GraalLambdaGridStep) - 0.5 ;
  }
  else
  {
    GraalLambdaCursorX = ((float)(GraalLambdaCursorX) / GraalLambdaGridStep) + 0.5 ;
  }

  if ( GraalLambdaCursorY < 0 )
  {
    GraalLambdaCursorY = ((float)(GraalLambdaCursorY) / GraalLambdaGridStep) - 0.5 ;
  }
  else
  {
    GraalLambdaCursorY = ((float)(GraalLambdaCursorY) / GraalLambdaGridStep) + 0.5 ;
  }

  GraalPixelCursorX  = ((float)(GraalLambdaCursorX) * GraalLambdaGridStep);
  GraalPixelCursorY  = ((float)(GraalLambdaCursorY) * GraalLambdaGridStep);

  GraalCursorX = GraalPixelCursorX - GraalPixelGridX;
  GraalCursorY = GraalPixelCursorY - GraalPixelGridY;
  GraalCursorY = GraalGraphicDy - GraalCursorY;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    GraalDisplayCoordinates                  |
|                                                             |
\------------------------------------------------------------*/

void GraalDisplayCoordinates()

{
  rdsbegin();

  sprintf( GraalCursorBuffer, "%.2f", (float)GraalLambdaCursorX / (float)GRAAL_SCALE );
  GraalDisplayMessage( GRAAL_MESSAGE_X, GraalCursorBuffer );

  sprintf( GraalCursorBuffer, "%.2f", (float)GraalLambdaCursorY / (float)GRAAL_SCALE );
  GraalDisplayMessage( GRAAL_MESSAGE_Y, GraalCursorBuffer );

  sprintf( GraalCursorBuffer, "%.2f", 
      (float)( GraalLambdaCursorX - GraalLambdaCursorSaveX[0] ) / (float)GRAAL_SCALE );
  GraalDisplayMessage( GRAAL_MESSAGE_DX, GraalCursorBuffer );

  sprintf( GraalCursorBuffer, "%.2f",
      (float)( GraalLambdaCursorY - GraalLambdaCursorSaveY[0] ) / (float)GRAAL_SCALE );
  GraalDisplayMessage( GRAAL_MESSAGE_DY, GraalCursorBuffer );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalPointCursor                     |
|                                                             |
\------------------------------------------------------------*/

void GraalPointCursor()

{
  rdsbegin();

  GraalUndisplayCursor();

  GraalLambdaCursorSaveX[ GraalCursorIndex ] = GraalLambdaCursorX;
  GraalLambdaCursorSaveY[ GraalCursorIndex ] = GraalLambdaCursorY;

  GraalCursorIndex = GraalCursorIndex + 1;

  GraalDisplayCursor();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalResetCursor                     |
|                                                             |
\------------------------------------------------------------*/

void GraalResetCursor()

{
  rdsbegin();

  GraalUndisplayCursor();

  GraalCursorIndex = GraalCursorIndex - 1;

  GraalDisplayCursor();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalChangeCursor                    |
|                                                             |
\------------------------------------------------------------*/

void GraalChangeCursorType( ArrayX, ArrayY, Index, Type )

   long *ArrayX;
   long *ArrayY;
   char  Index;
   char  Type;
{
  rdsbegin();

  GraalUndisplayCursor();

  GraalCursorType  = Type;
  GraalCursorIndex = Index;

  if ( Index != 0 )
  {
    GraalLambdaCursorSaveX[ 0 ] = ArrayX[0];
    GraalLambdaCursorSaveY[ 0 ] = ArrayY[0];
    GraalLambdaCursorSaveX[ 1 ] = ArrayX[1];
    GraalLambdaCursorSaveY[ 1 ] = ArrayY[1];
  }
  
  GraalDisplayCursor();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalDrawCursor                      |
|                                                             |
\------------------------------------------------------------*/

void GraalDrawCursor()

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

  XDrawLine( GraalGraphicDisplay,
             XtWindow( GraalGraphicWindow ),
             GraalXorGC,
             GraalCursorSaveX - GRAAL_CURSOR_SIZE, 
             GraalCursorSaveY - GRAAL_CURSOR_SIZE,
             GraalCursorSaveX + GRAAL_CURSOR_SIZE, 
             GraalCursorSaveY + GRAAL_CURSOR_SIZE );

  XDrawLine( GraalGraphicDisplay,
             XtWindow( GraalGraphicWindow ),
             GraalXorGC,
             GraalCursorSaveX - GRAAL_CURSOR_SIZE,
             GraalCursorSaveY + GRAAL_CURSOR_SIZE,
             GraalCursorSaveX + GRAAL_CURSOR_SIZE, 
             GraalCursorSaveY - GRAAL_CURSOR_SIZE );

  if ( GraalCursorIndex > 0 )
  {
    X1 = (float)(GraalLambdaCursorSaveX[0]) * GraalLambdaGridStep;
    Y1 = (float)(GraalLambdaCursorSaveY[0]) * GraalLambdaGridStep;
    X1 = X1 - GraalPixelGridX;
    Y1 = Y1 - GraalPixelGridY;
    Y1 = GraalGraphicDy - Y1;

    if ( GraalCursorIndex == 1 )
    {
      X2 = GraalCursorSaveX;
      Y2 = GraalCursorSaveY;
    }
    else
    {
      X2 = (float)(GraalLambdaCursorSaveX[1]) * GraalLambdaGridStep;
      Y2 = (float)(GraalLambdaCursorSaveY[1]) * GraalLambdaGridStep;
      X2 = X2 - GraalPixelGridX;
      Y2 = Y2 - GraalPixelGridY;
      Y2 = GraalGraphicDy - Y2;
    }

    switch( GraalCursorType )
    {
      case GRAAL_INPUT_POINT :

        break;

      case GRAAL_INPUT_LINE  :

        XDrawLine( GraalGraphicDisplay,
                   XtWindow( GraalGraphicWindow ),
                   GraalXorGC,
                   X1, Y1, X2, Y2 );
        break;

      case GRAAL_INPUT_HALF_BOX :

        XDrawLine( GraalGraphicDisplay,
                   XtWindow( GraalGraphicWindow ),
                   GraalXorGC,
                   X1, Y1, X2, Y1 );

        XDrawLine( GraalGraphicDisplay,
                   XtWindow( GraalGraphicWindow ),
                   GraalXorGC,
                   X2, Y1, X2, Y2 );
        break; 

      case GRAAL_INPUT_ORTHO   :
      case GRAAL_INPUT_SORTHO  :
      case GRAAL_INPUT_LSTRING :

        DeltaX = X2 - X1; if ( DeltaX < 0 ) DeltaX = - DeltaX;
        DeltaY = Y2 - Y1; if ( DeltaY < 0 ) DeltaY = - DeltaY;

        if ( DeltaX > DeltaY )
        {
          Y2 = Y1;

          if ( X1 > X2 ) { Swap = X1; X1 = X2; X2 = Swap; }

          if ( X1 < 0              ) X1 = 0;
          if ( X2 > GraalGraphicDx ) X2 = GraalGraphicDx;

          if ( ( X1 <  X2             ) &&
               ( Y1 >= 0              ) &&
               ( Y1 <= GraalGraphicDy ) )
          {
            XDrawLine( GraalGraphicDisplay,
                       XtWindow( GraalGraphicWindow ),
                       GraalXorGC,
                       X1, Y1, X2, Y2 );
          }
        }
        else
        {
          X2 = X1;

          if ( Y1 > Y2 ) { Swap = Y1; Y1 = Y2; Y2 = Swap; }

          if ( Y1 < 0              ) Y1 = 0;
          if ( Y2 > GraalGraphicDy ) Y2 = GraalGraphicDy;

          if ( ( Y1 <  Y2             ) &&
               ( X1 >= 0              ) &&
               ( X1 <= GraalGraphicDx ) )
          {
            XDrawLine( GraalGraphicDisplay, 
                       XtWindow( GraalGraphicWindow ), 
                       GraalXorGC,   
                       X1, Y1, X2, Y2 ); 
          }
        }

        break;

      case GRAAL_INPUT_BOX :
 
        if ( X1 > X2 ) { Swap = X1; X1 = X2; X2 = Swap; }
        if ( Y1 > Y2 ) { Swap = Y1; Y1 = Y2; Y2 = Swap; }

        DrawLine = 0;

        if ( X1 < 0 )
        { 
          X1 = 0; DrawLine |= GRAAL_WEST_MASK;
        }

        if ( X2 > GraalGraphicDx ) 
        {
          X2 = GraalGraphicDx; DrawLine |= GRAAL_EAST_MASK;
        }

        if ( Y1 < 0 ) 
        { 
          Y1 = 0; DrawLine |= GRAAL_SOUTH_MASK;
        }

        if ( Y2 > GraalGraphicDy ) 
        {
          Y2 = GraalGraphicDy; DrawLine |= GRAAL_NORTH_MASK;
        }

        if ( DrawLine == 0 )
        {
          XDrawRectangle( GraalGraphicDisplay,
                          XtWindow( GraalGraphicWindow ),
                          GraalXorGC,
                          X1, Y1,
                          X2 - X1, Y2 - Y1 );
        }
        else 
        {
          if ( ( DrawLine & GRAAL_WEST_MASK ) == 0 )
          {
            XDrawLine( GraalGraphicDisplay,
                       XtWindow( GraalGraphicWindow ),
                       GraalXorGC,
                       X1, Y2,
                       X1, Y1 );
          }

          if ( ( DrawLine & GRAAL_EAST_MASK ) == 0 )
          {
            XDrawLine( GraalGraphicDisplay,
                       XtWindow( GraalGraphicWindow ),
                       GraalXorGC,
                       X2, Y2,
                       X2, Y1 );
          }

          if ( ( DrawLine & GRAAL_SOUTH_MASK ) == 0 )
          {
            XDrawLine( GraalGraphicDisplay,
                       XtWindow( GraalGraphicWindow ),
                       GraalXorGC,
                       X1, Y1,
                       X2, Y1 );
          }

          if ( ( DrawLine & GRAAL_NORTH_MASK ) == 0 )
          {
            XDrawLine( GraalGraphicDisplay,
                       XtWindow( GraalGraphicWindow ),
                       GraalXorGC,
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
|                    GraalUndisplayCursor                     |
|                                                             |
\------------------------------------------------------------*/

void GraalUndisplayCursor()

{
  rdsbegin();

  if ( GraalCursorInside == GRAAL_TRUE )
  {
    if ( GraalCursorSaved == GRAAL_TRUE )
    {
      GraalDrawCursor();
    }

    GraalCursorSaved = GRAAL_FALSE;
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalDisplayCursor                     |
|                                                             |
\------------------------------------------------------------*/

void GraalDisplayCursor()

{
  rdsbegin();

  if ( GraalCursorInside == GRAAL_TRUE )
  {
    if ( GraalCursorSaved == GRAAL_TRUE )
    {
      GraalDrawCursor();

      GraalCursorSaved = GRAAL_FALSE;
    }

    if ( ( GraalCursorY >= 0              ) &&
         ( GraalCursorX <= GraalGraphicDx ) )
    {
      GraalCursorSaveX = GraalCursorX;
      GraalCursorSaveY = GraalCursorY;

      GraalDrawCursor();

      GraalCursorSaved = GRAAL_TRUE;
    }
  }

  rdsend();
}
