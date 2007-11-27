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
| Tool    :                   XGRA                           |
|                                                             |
| File    :                  Cursor.c                         |
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
# include <X11/Intrinsic.h>
# include <X11/StringDefs.h>
# include <Xm/Xm.h>

# include "mut.h"
# include "aut.h"
# include "XSB.h"
# include "XMX.h"
# include "XMX_grid.h"
# include "XMX_cursor.h"

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

  static char  XgraCursorBuffer[ XGRA_MAX_CURSOR_BUFFER ];

/*------------------------------------------------------------\
|                                                             |
|                         Coordinates                         |
|                                                             |
\------------------------------------------------------------*/

  Position XgraCursorX       = 0;
  Position XgraCursorY       = 0;
  Position XgraCursorSaveX   = 0;
  Position XgraCursorSaveY   = 0;
  char     XgraCursorSaved   = XGRA_FALSE;
  char     XgraCursorInside  = XGRA_FALSE;
  char     XgraCursorType    = XGRA_INPUT_HALF_BOX;

  long     XgraUnitCursorX = 0;
  long     XgraUnitCursorY = 0;
  long     XgraPixelCursorX  = 0;
  long     XgraPixelCursorY  = 0;

  long     XgraUnitCursorSaveX[ 2 ] = { 0, 0 };
  long     XgraUnitCursorSaveY[ 2 ] = { 0, 0 };
  char     XgraCursorIndex            = 0;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XgraComputeCursor                     |
|                                                             |
\------------------------------------------------------------*/

void XgraComputeCursor( X, Y )

     Position X;
     Position Y;
{
  autbegin();

  Y = XgraGraphicDy - Y;

  XgraUnitCursorX = X + XgraPixelGridX;
  XgraUnitCursorY = Y + XgraPixelGridY;

  if ( XgraUnitCursorX < 0 )
  {
    XgraUnitCursorX = ((float)(XgraUnitCursorX) / XgraUnitGridStep) - 0.5 ;
  }
  else
  {
    XgraUnitCursorX = ((float)(XgraUnitCursorX) / XgraUnitGridStep) + 0.5 ;
  }

  if ( XgraUnitCursorY < 0 )
  {
    XgraUnitCursorY = ((float)(XgraUnitCursorY) / XgraUnitGridStep) - 0.5 ;
  }
  else
  {
    XgraUnitCursorY = ((float)(XgraUnitCursorY) / XgraUnitGridStep) + 0.5 ;
  }

  XgraPixelCursorX  = ((float)(XgraUnitCursorX) * XgraUnitGridStep);
  XgraPixelCursorY  = ((float)(XgraUnitCursorY) * XgraUnitGridStep);

  XgraCursorX = XgraPixelCursorX - XgraPixelGridX;
  XgraCursorY = XgraPixelCursorY - XgraPixelGridY;
  XgraCursorY = XgraGraphicDy - XgraCursorY;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XgraDisplayCoordinates                  |
|                                                             |
\------------------------------------------------------------*/

void XgraDisplayCoordinates()

{
  autbegin();

  sprintf( XgraCursorBuffer, "%d", XgraUnitCursorX );
  XgraDisplayMessage( XGRA_MESSAGE_X, XgraCursorBuffer );

  sprintf( XgraCursorBuffer, "%d", XgraUnitCursorY );
  XgraDisplayMessage( XGRA_MESSAGE_Y, XgraCursorBuffer );

  sprintf( XgraCursorBuffer, "%d", XgraUnitCursorX - XgraUnitCursorSaveX[0] );
  XgraDisplayMessage( XGRA_MESSAGE_DX, XgraCursorBuffer );

  sprintf( XgraCursorBuffer, "%d", XgraUnitCursorY - XgraUnitCursorSaveY[0] );
  XgraDisplayMessage( XGRA_MESSAGE_DY, XgraCursorBuffer );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XgraPointCursor                     |
|                                                             |
\------------------------------------------------------------*/

void XgraPointCursor()

{
  autbegin();

  XgraUndisplayCursor();

  XgraUnitCursorSaveX[ XgraCursorIndex ] = XgraUnitCursorX;
  XgraUnitCursorSaveY[ XgraCursorIndex ] = XgraUnitCursorY;

  XgraCursorIndex = XgraCursorIndex + 1;

  XgraDisplayCursor();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XgraResetCursor                     |
|                                                             |
\------------------------------------------------------------*/

void XgraResetCursor()

{
  autbegin();

  XgraUndisplayCursor();

  XgraCursorIndex = XgraCursorIndex - 1;

  XgraDisplayCursor();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XgraChangeCursor                    |
|                                                             |
\------------------------------------------------------------*/

void XgraChangeCursorType( ArrayX, ArrayY, Index, Type )

   long *ArrayX;
   long *ArrayY;
   char  Index;
   char  Type;
{
  autbegin();

  XgraUndisplayCursor();

  XgraCursorType  = Type;
  XgraCursorIndex = Index;

  if ( Index != 0 )
  {
    XgraUnitCursorSaveX[ 0 ] = ArrayX[0];
    XgraUnitCursorSaveY[ 0 ] = ArrayY[0];
    XgraUnitCursorSaveX[ 1 ] = ArrayX[1];
    XgraUnitCursorSaveY[ 1 ] = ArrayY[1];
  }
  
  XgraDisplayCursor();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XgraDrawCursor                      |
|                                                             |
\------------------------------------------------------------*/

void XgraDrawCursor()

{
  long X1;
  long Y1;
  long X2;
  long Y2;
  long DeltaX;
  long DeltaY;
  long Swap;
  char DrawLine;

  autbegin();

  XDrawLine( XgraGraphicDisplay,
             XtWindow( XgraGraphicWindow ),
             XgraXorGC,
             XgraCursorSaveX - XGRA_CURSOR_SIZE, 
             XgraCursorSaveY - XGRA_CURSOR_SIZE,
             XgraCursorSaveX + XGRA_CURSOR_SIZE, 
             XgraCursorSaveY + XGRA_CURSOR_SIZE );

  XDrawLine( XgraGraphicDisplay,
             XtWindow( XgraGraphicWindow ),
             XgraXorGC,
             XgraCursorSaveX - XGRA_CURSOR_SIZE,
             XgraCursorSaveY + XGRA_CURSOR_SIZE,
             XgraCursorSaveX + XGRA_CURSOR_SIZE, 
             XgraCursorSaveY - XGRA_CURSOR_SIZE );

  if ( XgraCursorIndex > 0 )
  {
    X1 = (float)(XgraUnitCursorSaveX[0]) * XgraUnitGridStep;
    Y1 = (float)(XgraUnitCursorSaveY[0]) * XgraUnitGridStep;
    X1 = X1 - XgraPixelGridX;
    Y1 = Y1 - XgraPixelGridY;
    Y1 = XgraGraphicDy - Y1;

    if ( XgraCursorIndex == 1 )
    {
      X2 = XgraCursorSaveX;
      Y2 = XgraCursorSaveY;
    }
    else
    {
      X2 = (float)(XgraUnitCursorSaveX[1]) * XgraUnitGridStep;
      Y2 = (float)(XgraUnitCursorSaveY[1]) * XgraUnitGridStep;
      X2 = X2 - XgraPixelGridX;
      Y2 = Y2 - XgraPixelGridY;
      Y2 = XgraGraphicDy - Y2;
    }

    switch( XgraCursorType )
    {
      case XGRA_INPUT_POINT :

        break;

      case XGRA_INPUT_LINE  :

        XDrawLine( XgraGraphicDisplay,
                   XtWindow( XgraGraphicWindow ),
                   XgraXorGC,
                   X1, Y1, X2, Y2 );
        break;

      case XGRA_INPUT_HALF_BOX :

        XDrawLine( XgraGraphicDisplay,
                   XtWindow( XgraGraphicWindow ),
                   XgraXorGC,
                   X1, Y1, X2, Y1 );

        XDrawLine( XgraGraphicDisplay,
                   XtWindow( XgraGraphicWindow ),
                   XgraXorGC,
                   X2, Y1, X2, Y2 );
        break; 

      case XGRA_INPUT_ORTHO   :
      case XGRA_INPUT_SORTHO  :
      case XGRA_INPUT_LSTRING :

        DeltaX = X2 - X1; if ( DeltaX < 0 ) DeltaX = - DeltaX;
        DeltaY = Y2 - Y1; if ( DeltaY < 0 ) DeltaY = - DeltaY;

        if ( DeltaX > DeltaY )
        {
          Y2 = Y1;

          if ( X1 > X2 ) { Swap = X1; X1 = X2; X2 = Swap; }

          if ( X1 < 0              ) X1 = 0;
          if ( X2 > XgraGraphicDx ) X2 = XgraGraphicDx;

          if ( ( X1 <  X2             ) &&
               ( Y1 >= 0              ) &&
               ( Y1 <= XgraGraphicDy ) )
          {
            XDrawLine( XgraGraphicDisplay,
                       XtWindow( XgraGraphicWindow ),
                       XgraXorGC,
                       X1, Y1, X2, Y2 );
          }
        }
        else
        {
          X2 = X1;

          if ( Y1 > Y2 ) { Swap = Y1; Y1 = Y2; Y2 = Swap; }

          if ( Y1 < 0              ) Y1 = 0;
          if ( Y2 > XgraGraphicDy ) Y2 = XgraGraphicDy;

          if ( ( Y1 <  Y2             ) &&
               ( X1 >= 0              ) &&
               ( X1 <= XgraGraphicDx ) )
          {
            XDrawLine( XgraGraphicDisplay, 
                       XtWindow( XgraGraphicWindow ), 
                       XgraXorGC,   
                       X1, Y1, X2, Y2 ); 
          }
        }

        break;

      case XGRA_INPUT_BOX :
 
        if ( X1 > X2 ) { Swap = X1; X1 = X2; X2 = Swap; }
        if ( Y1 > Y2 ) { Swap = Y1; Y1 = Y2; Y2 = Swap; }

        DrawLine = 0;

        if ( X1 < 0              ) { X1 = 0;              DrawLine |= XGRA_WEST;  }
        if ( X2 > XgraGraphicDx ) { X2 = XgraGraphicDx; DrawLine |= XGRA_EAST;  }
        if ( Y1 < 0              ) { Y1 = 0;              DrawLine |= XGRA_SOUTH; }
        if ( Y2 > XgraGraphicDy ) { Y2 = XgraGraphicDy; DrawLine |= XGRA_NORTH; }

        if ( DrawLine == 0 )
        {
          XDrawRectangle( XgraGraphicDisplay,
                          XtWindow( XgraGraphicWindow ),
                          XgraXorGC,
                          X1, Y1,
                          X2 - X1, Y2 - Y1 );
        }
        else 
        {
          if ( ( DrawLine & XGRA_WEST ) == 0 )
          {
            XDrawLine( XgraGraphicDisplay,
                       XtWindow( XgraGraphicWindow ),
                       XgraXorGC,
                       X1, Y2,
                       X1, Y1 );
          }

          if ( ( DrawLine & XGRA_EAST ) == 0 )
          {
            XDrawLine( XgraGraphicDisplay,
                       XtWindow( XgraGraphicWindow ),
                       XgraXorGC,
                       X2, Y2,
                       X2, Y1 );
          }

          if ( ( DrawLine & XGRA_SOUTH ) == 0 )
          {
            XDrawLine( XgraGraphicDisplay,
                       XtWindow( XgraGraphicWindow ),
                       XgraXorGC,
                       X1, Y1,
                       X2, Y1 );
          }

          if ( ( DrawLine & XGRA_NORTH ) == 0 )
          {
            XDrawLine( XgraGraphicDisplay,
                       XtWindow( XgraGraphicWindow ),
                       XgraXorGC,
                       X1, Y2,
                       X2, Y2 );
          }
        }

      break;

    }
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XgraUndisplayCursor                     |
|                                                             |
\------------------------------------------------------------*/

void XgraUndisplayCursor()

{
  autbegin();

  if ( XgraCursorInside == XGRA_TRUE )
  {
    if ( XgraCursorSaved == XGRA_TRUE )
    {
      XgraDrawCursor();
    }

    XgraCursorSaved = XGRA_FALSE;
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XgraDisplayCursor                     |
|                                                             |
\------------------------------------------------------------*/

void XgraDisplayCursor()

{
  autbegin();

  if ( XgraCursorInside == XGRA_TRUE )
  {
    if ( XgraCursorSaved == XGRA_TRUE )
    {
      XgraDrawCursor();

      XgraCursorSaved = XGRA_FALSE;
    }

    if ( ( XgraCursorY >= 0              ) &&
         ( XgraCursorX <= XgraGraphicDx ) )
    {
      XgraCursorSaveX = XgraCursorX;
      XgraCursorSaveY = XgraCursorY;

      XgraDrawCursor();

      XgraCursorSaved = XGRA_TRUE;
    }
  }

  autend();
}
