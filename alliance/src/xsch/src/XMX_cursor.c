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
| Tool    :                   XSCH                           |
|                                                             |
| File    :                  Cursor.c                         |
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
# include <X11/Intrinsic.h>
# include <X11/StringDefs.h>
# include <Xm/Xm.h>

# include "mut.h"
# include "aut.h"
# include "mlo.h"
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

  static char  XschCursorBuffer[ XSCH_MAX_CURSOR_BUFFER ];

/*------------------------------------------------------------\
|                                                             |
|                         Coordinates                         |
|                                                             |
\------------------------------------------------------------*/

  Position XschCursorX       = 0;
  Position XschCursorY       = 0;
  Position XschCursorSaveX   = 0;
  Position XschCursorSaveY   = 0;
  int      XschCursorSaved   = XSCH_FALSE;
  int      XschCursorInside  = XSCH_FALSE;
  int      XschCursorType    = XSCH_INPUT_HALF_BOX;

  long     XschUnitCursorX = 0;
  long     XschUnitCursorY = 0;
  long     XschPixelCursorX  = 0;
  long     XschPixelCursorY  = 0;

  long     XschUnitCursorSaveX[ 2 ] = { 0, 0 };
  long     XschUnitCursorSaveY[ 2 ] = { 0, 0 };
  int      XschCursorIndex            = 0;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XschComputeCursor                     |
|                                                             |
\------------------------------------------------------------*/

void XschComputeCursor( X, Y )

     Position X;
     Position Y;
{
  autbegin();

  Y = XschGraphicDy - Y;

  XschUnitCursorX = X + XschPixelGridX;
  XschUnitCursorY = Y + XschPixelGridY;

  if ( XschUnitCursorX < 0 )
  {
    XschUnitCursorX = ((float)(XschUnitCursorX) / XschUnitGridStep) - 0.5 ;
  }
  else
  {
    XschUnitCursorX = ((float)(XschUnitCursorX) / XschUnitGridStep) + 0.5 ;
  }

  if ( XschUnitCursorY < 0 )
  {
    XschUnitCursorY = ((float)(XschUnitCursorY) / XschUnitGridStep) - 0.5 ;
  }
  else
  {
    XschUnitCursorY = ((float)(XschUnitCursorY) / XschUnitGridStep) + 0.5 ;
  }

  XschPixelCursorX  = ((float)(XschUnitCursorX) * XschUnitGridStep);
  XschPixelCursorY  = ((float)(XschUnitCursorY) * XschUnitGridStep);

  XschCursorX = XschPixelCursorX - XschPixelGridX;
  XschCursorY = XschPixelCursorY - XschPixelGridY;
  XschCursorY = XschGraphicDy - XschCursorY;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XschDisplayCoordinates                  |
|                                                             |
\------------------------------------------------------------*/

void XschDisplayCoordinates()

{
  autbegin();

  sprintf( XschCursorBuffer, "%ld", XschUnitCursorX );
  XschDisplayMessage( XSCH_MESSAGE_X, XschCursorBuffer );

  sprintf( XschCursorBuffer, "%ld", XschUnitCursorY );
  XschDisplayMessage( XSCH_MESSAGE_Y, XschCursorBuffer );

  sprintf( XschCursorBuffer, "%ld", XschUnitCursorX - XschUnitCursorSaveX[0] );
  XschDisplayMessage( XSCH_MESSAGE_DX, XschCursorBuffer );

  sprintf( XschCursorBuffer, "%ld", XschUnitCursorY - XschUnitCursorSaveY[0] );
  XschDisplayMessage( XSCH_MESSAGE_DY, XschCursorBuffer );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XschPointCursor                     |
|                                                             |
\------------------------------------------------------------*/

void XschPointCursor()

{
  autbegin();

  XschUndisplayCursor();

  XschUnitCursorSaveX[ XschCursorIndex ] = XschUnitCursorX;
  XschUnitCursorSaveY[ XschCursorIndex ] = XschUnitCursorY;

  XschCursorIndex = XschCursorIndex + 1;

  XschDisplayCursor();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XschResetCursor                     |
|                                                             |
\------------------------------------------------------------*/

void XschResetCursor()

{
  autbegin();

  XschUndisplayCursor();

  XschCursorIndex = XschCursorIndex - 1;

  XschDisplayCursor();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XschChangeCursor                    |
|                                                             |
\------------------------------------------------------------*/

void XschChangeCursorType( ArrayX, ArrayY, Index, Type )

   long *ArrayX;
   long *ArrayY;
   int   Index;
   int   Type;
{
  autbegin();

  XschUndisplayCursor();

  XschCursorType  = Type;
  XschCursorIndex = Index;

  if ( Index != 0 )
  {
    XschUnitCursorSaveX[ 0 ] = ArrayX[0];
    XschUnitCursorSaveY[ 0 ] = ArrayY[0];
    XschUnitCursorSaveX[ 1 ] = ArrayX[1];
    XschUnitCursorSaveY[ 1 ] = ArrayY[1];
  }
  
  XschDisplayCursor();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XschDrawCursor                      |
|                                                             |
\------------------------------------------------------------*/

void XschDrawCursor()

{
  long X1;
  long Y1;
  long X2;
  long Y2;
  long DeltaX;
  long DeltaY;
  long Swap;
  int  DrawLine;

  autbegin();

  XDrawLine( XschGraphicDisplay,
             XtWindow( XschGraphicWindow ),
             XschXorGC,
             XschCursorSaveX - XSCH_CURSOR_SIZE, 
             XschCursorSaveY - XSCH_CURSOR_SIZE,
             XschCursorSaveX + XSCH_CURSOR_SIZE, 
             XschCursorSaveY + XSCH_CURSOR_SIZE );

  XDrawLine( XschGraphicDisplay,
             XtWindow( XschGraphicWindow ),
             XschXorGC,
             XschCursorSaveX - XSCH_CURSOR_SIZE,
             XschCursorSaveY + XSCH_CURSOR_SIZE,
             XschCursorSaveX + XSCH_CURSOR_SIZE, 
             XschCursorSaveY - XSCH_CURSOR_SIZE );

  if ( XschCursorIndex > 0 )
  {
    X1 = (float)(XschUnitCursorSaveX[0]) * XschUnitGridStep;
    Y1 = (float)(XschUnitCursorSaveY[0]) * XschUnitGridStep;
    X1 = X1 - XschPixelGridX;
    Y1 = Y1 - XschPixelGridY;
    Y1 = XschGraphicDy - Y1;

    if ( XschCursorIndex == 1 )
    {
      X2 = XschCursorSaveX;
      Y2 = XschCursorSaveY;
    }
    else
    {
      X2 = (float)(XschUnitCursorSaveX[1]) * XschUnitGridStep;
      Y2 = (float)(XschUnitCursorSaveY[1]) * XschUnitGridStep;
      X2 = X2 - XschPixelGridX;
      Y2 = Y2 - XschPixelGridY;
      Y2 = XschGraphicDy - Y2;
    }

    switch( XschCursorType )
    {
      case XSCH_INPUT_POINT :

        break;

      case XSCH_INPUT_LINE  :

        XDrawLine( XschGraphicDisplay,
                   XtWindow( XschGraphicWindow ),
                   XschXorGC,
                   X1, Y1, X2, Y2 );
        break;

      case XSCH_INPUT_HALF_BOX :

        XDrawLine( XschGraphicDisplay,
                   XtWindow( XschGraphicWindow ),
                   XschXorGC,
                   X1, Y1, X2, Y1 );

        XDrawLine( XschGraphicDisplay,
                   XtWindow( XschGraphicWindow ),
                   XschXorGC,
                   X2, Y1, X2, Y2 );
        break; 

      case XSCH_INPUT_ORTHO   :
      case XSCH_INPUT_SORTHO  :
      case XSCH_INPUT_LSTRING :

        DeltaX = X2 - X1; if ( DeltaX < 0 ) DeltaX = - DeltaX;
        DeltaY = Y2 - Y1; if ( DeltaY < 0 ) DeltaY = - DeltaY;

        if ( DeltaX > DeltaY )
        {
          Y2 = Y1;

          if ( X1 > X2 ) { Swap = X1; X1 = X2; X2 = Swap; }

          if ( X1 < 0              ) X1 = 0;
          if ( X2 > XschGraphicDx ) X2 = XschGraphicDx;

          if ( ( X1 <  X2             ) &&
               ( Y1 >= 0              ) &&
               ( Y1 <= XschGraphicDy ) )
          {
            XDrawLine( XschGraphicDisplay,
                       XtWindow( XschGraphicWindow ),
                       XschXorGC,
                       X1, Y1, X2, Y2 );
          }
        }
        else
        {
          X2 = X1;

          if ( Y1 > Y2 ) { Swap = Y1; Y1 = Y2; Y2 = Swap; }

          if ( Y1 < 0              ) Y1 = 0;
          if ( Y2 > XschGraphicDy ) Y2 = XschGraphicDy;

          if ( ( Y1 <  Y2             ) &&
               ( X1 >= 0              ) &&
               ( X1 <= XschGraphicDx ) )
          {
            XDrawLine( XschGraphicDisplay, 
                       XtWindow( XschGraphicWindow ), 
                       XschXorGC,   
                       X1, Y1, X2, Y2 ); 
          }
        }

        break;

      case XSCH_INPUT_BOX :
 
        if ( X1 > X2 ) { Swap = X1; X1 = X2; X2 = Swap; }
        if ( Y1 > Y2 ) { Swap = Y1; Y1 = Y2; Y2 = Swap; }

        DrawLine = 0;

        if ( X1 < 0              ) { X1 = 0;              DrawLine |= XSCH_WEST;  }
        if ( X2 > XschGraphicDx ) { X2 = XschGraphicDx; DrawLine |= XSCH_EAST;  }
        if ( Y1 < 0              ) { Y1 = 0;              DrawLine |= XSCH_SOUTH; }
        if ( Y2 > XschGraphicDy ) { Y2 = XschGraphicDy; DrawLine |= XSCH_NORTH; }

        if ( DrawLine == 0 )
        {
          XDrawRectangle( XschGraphicDisplay,
                          XtWindow( XschGraphicWindow ),
                          XschXorGC,
                          X1, Y1,
                          X2 - X1, Y2 - Y1 );
        }
        else 
        {
          if ( ( DrawLine & XSCH_WEST ) == 0 )
          {
            XDrawLine( XschGraphicDisplay,
                       XtWindow( XschGraphicWindow ),
                       XschXorGC,
                       X1, Y2,
                       X1, Y1 );
          }

          if ( ( DrawLine & XSCH_EAST ) == 0 )
          {
            XDrawLine( XschGraphicDisplay,
                       XtWindow( XschGraphicWindow ),
                       XschXorGC,
                       X2, Y2,
                       X2, Y1 );
          }

          if ( ( DrawLine & XSCH_SOUTH ) == 0 )
          {
            XDrawLine( XschGraphicDisplay,
                       XtWindow( XschGraphicWindow ),
                       XschXorGC,
                       X1, Y1,
                       X2, Y1 );
          }

          if ( ( DrawLine & XSCH_NORTH ) == 0 )
          {
            XDrawLine( XschGraphicDisplay,
                       XtWindow( XschGraphicWindow ),
                       XschXorGC,
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
|                    XschUndisplayCursor                     |
|                                                             |
\------------------------------------------------------------*/

void XschUndisplayCursor()

{
  autbegin();

  if ( XschCursorInside == XSCH_TRUE )
  {
    if ( XschCursorSaved == XSCH_TRUE )
    {
      XschDrawCursor();
    }

    XschCursorSaved = XSCH_FALSE;
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XschDisplayCursor                     |
|                                                             |
\------------------------------------------------------------*/

void XschDisplayCursor()

{
  autbegin();

  if ( XschCursorInside == XSCH_TRUE )
  {
    if ( XschCursorSaved == XSCH_TRUE )
    {
      XschDrawCursor();

      XschCursorSaved = XSCH_FALSE;
    }

    if ( ( XschCursorY >= 0                       ) &&
         ( XschCursorX <= (Position)XschGraphicDx ) )
    {
      XschCursorSaveX = XschCursorX;
      XschCursorSaveY = XschCursorY;

      XschDrawCursor();

      XschCursorSaved = XSCH_TRUE;
    }
  }

  autend();
}
