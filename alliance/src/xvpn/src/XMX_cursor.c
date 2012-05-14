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
| Tool    :                   XVPN                           |
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

  static char  XvpnCursorBuffer[ XVPN_MAX_CURSOR_BUFFER ];

/*------------------------------------------------------------\
|                                                             |
|                         Coordinates                         |
|                                                             |
\------------------------------------------------------------*/

  Position XvpnCursorX       = 0;
  Position XvpnCursorY       = 0;
  Position XvpnCursorSaveX   = 0;
  Position XvpnCursorSaveY   = 0;
  short    XvpnCursorSaved   = XVPN_FALSE;
  short    XvpnCursorInside  = XVPN_FALSE;
  short    XvpnCursorType    = XVPN_INPUT_HALF_BOX;

  long     XvpnUnitCursorX = 0;
  long     XvpnUnitCursorY = 0;
  long     XvpnPixelCursorX  = 0;
  long     XvpnPixelCursorY  = 0;

  long     XvpnUnitCursorSaveX[ 2 ] = { 0, 0 };
  long     XvpnUnitCursorSaveY[ 2 ] = { 0, 0 };
  short    XvpnCursorIndex            = 0;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XvpnComputeCursor                     |
|                                                             |
\------------------------------------------------------------*/

void XvpnComputeCursor( X, Y )

     Position X;
     Position Y;
{
  autbegin();

  Y = XvpnGraphicDy - Y;

  XvpnUnitCursorX = X + XvpnPixelGridX;
  XvpnUnitCursorY = Y + XvpnPixelGridY;

  if ( XvpnUnitCursorX < 0 )
  {
    XvpnUnitCursorX = ((float)(XvpnUnitCursorX) / XvpnUnitGridStep) - 0.5 ;
  }
  else
  {
    XvpnUnitCursorX = ((float)(XvpnUnitCursorX) / XvpnUnitGridStep) + 0.5 ;
  }

  if ( XvpnUnitCursorY < 0 )
  {
    XvpnUnitCursorY = ((float)(XvpnUnitCursorY) / XvpnUnitGridStep) - 0.5 ;
  }
  else
  {
    XvpnUnitCursorY = ((float)(XvpnUnitCursorY) / XvpnUnitGridStep) + 0.5 ;
  }

  XvpnPixelCursorX  = ((float)(XvpnUnitCursorX) * XvpnUnitGridStep);
  XvpnPixelCursorY  = ((float)(XvpnUnitCursorY) * XvpnUnitGridStep);

  XvpnCursorX = XvpnPixelCursorX - XvpnPixelGridX;
  XvpnCursorY = XvpnPixelCursorY - XvpnPixelGridY;
  XvpnCursorY = XvpnGraphicDy - XvpnCursorY;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XvpnDisplayCoordinates                  |
|                                                             |
\------------------------------------------------------------*/

void XvpnDisplayCoordinates()

{
  autbegin();

  sprintf( XvpnCursorBuffer, "%ld", XvpnUnitCursorX );
  XvpnDisplayMessage( XVPN_MESSAGE_X, XvpnCursorBuffer );

  sprintf( XvpnCursorBuffer, "%ld", XvpnUnitCursorY );
  XvpnDisplayMessage( XVPN_MESSAGE_Y, XvpnCursorBuffer );

  sprintf( XvpnCursorBuffer, "%ld", XvpnUnitCursorX - XvpnUnitCursorSaveX[0] );
  XvpnDisplayMessage( XVPN_MESSAGE_DX, XvpnCursorBuffer );

  sprintf( XvpnCursorBuffer, "%ld", XvpnUnitCursorY - XvpnUnitCursorSaveY[0] );
  XvpnDisplayMessage( XVPN_MESSAGE_DY, XvpnCursorBuffer );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XvpnPointCursor                     |
|                                                             |
\------------------------------------------------------------*/

void XvpnPointCursor()

{
  autbegin();

  XvpnUndisplayCursor();

  XvpnUnitCursorSaveX[ XvpnCursorIndex ] = XvpnUnitCursorX;
  XvpnUnitCursorSaveY[ XvpnCursorIndex ] = XvpnUnitCursorY;

  XvpnCursorIndex = XvpnCursorIndex + 1;

  XvpnDisplayCursor();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XvpnResetCursor                     |
|                                                             |
\------------------------------------------------------------*/

void XvpnResetCursor()

{
  autbegin();

  XvpnUndisplayCursor();

  XvpnCursorIndex = XvpnCursorIndex - 1;

  XvpnDisplayCursor();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XvpnChangeCursor                    |
|                                                             |
\------------------------------------------------------------*/

void XvpnChangeCursorType( ArrayX, ArrayY, Index, Type )

   long *ArrayX;
   long *ArrayY;
   short Index;
   short Type;
{
  autbegin();

  XvpnUndisplayCursor();

  XvpnCursorType  = Type;
  XvpnCursorIndex = Index;

  if ( Index != 0 )
  {
    XvpnUnitCursorSaveX[ 0 ] = ArrayX[0];
    XvpnUnitCursorSaveY[ 0 ] = ArrayY[0];
    XvpnUnitCursorSaveX[ 1 ] = ArrayX[1];
    XvpnUnitCursorSaveY[ 1 ] = ArrayY[1];
  }
  
  XvpnDisplayCursor();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XvpnDrawCursor                      |
|                                                             |
\------------------------------------------------------------*/

void XvpnDrawCursor()

{
  long  X1;
  long  Y1;
  long  X2;
  long  Y2;
  long  DeltaX;
  long  DeltaY;
  long  Swap;
  short DrawLine;

  autbegin();

  XDrawLine( XvpnGraphicDisplay,
             XtWindow( XvpnGraphicWindow ),
             XvpnXorGC,
             XvpnCursorSaveX - XVPN_CURSOR_SIZE, 
             XvpnCursorSaveY - XVPN_CURSOR_SIZE,
             XvpnCursorSaveX + XVPN_CURSOR_SIZE, 
             XvpnCursorSaveY + XVPN_CURSOR_SIZE );

  XDrawLine( XvpnGraphicDisplay,
             XtWindow( XvpnGraphicWindow ),
             XvpnXorGC,
             XvpnCursorSaveX - XVPN_CURSOR_SIZE,
             XvpnCursorSaveY + XVPN_CURSOR_SIZE,
             XvpnCursorSaveX + XVPN_CURSOR_SIZE, 
             XvpnCursorSaveY - XVPN_CURSOR_SIZE );

  if ( XvpnCursorIndex > 0 )
  {
    X1 = (float)(XvpnUnitCursorSaveX[0]) * XvpnUnitGridStep;
    Y1 = (float)(XvpnUnitCursorSaveY[0]) * XvpnUnitGridStep;
    X1 = X1 - XvpnPixelGridX;
    Y1 = Y1 - XvpnPixelGridY;
    Y1 = XvpnGraphicDy - Y1;

    if ( XvpnCursorIndex == 1 )
    {
      X2 = XvpnCursorSaveX;
      Y2 = XvpnCursorSaveY;
    }
    else
    {
      X2 = (float)(XvpnUnitCursorSaveX[1]) * XvpnUnitGridStep;
      Y2 = (float)(XvpnUnitCursorSaveY[1]) * XvpnUnitGridStep;
      X2 = X2 - XvpnPixelGridX;
      Y2 = Y2 - XvpnPixelGridY;
      Y2 = XvpnGraphicDy - Y2;
    }

    switch( XvpnCursorType )
    {
      case XVPN_INPUT_POINT :

        break;

      case XVPN_INPUT_LINE  :

        XDrawLine( XvpnGraphicDisplay,
                   XtWindow( XvpnGraphicWindow ),
                   XvpnXorGC,
                   X1, Y1, X2, Y2 );
        break;

      case XVPN_INPUT_HALF_BOX :

        XDrawLine( XvpnGraphicDisplay,
                   XtWindow( XvpnGraphicWindow ),
                   XvpnXorGC,
                   X1, Y1, X2, Y1 );

        XDrawLine( XvpnGraphicDisplay,
                   XtWindow( XvpnGraphicWindow ),
                   XvpnXorGC,
                   X2, Y1, X2, Y2 );
        break; 

      case XVPN_INPUT_ORTHO   :
      case XVPN_INPUT_SORTHO  :
      case XVPN_INPUT_LSTRING :

        DeltaX = X2 - X1; if ( DeltaX < 0 ) DeltaX = - DeltaX;
        DeltaY = Y2 - Y1; if ( DeltaY < 0 ) DeltaY = - DeltaY;

        if ( DeltaX > DeltaY )
        {
          Y2 = Y1;

          if ( X1 > X2 ) { Swap = X1; X1 = X2; X2 = Swap; }

          if ( X1 < 0              ) X1 = 0;
          if ( X2 > XvpnGraphicDx ) X2 = XvpnGraphicDx;

          if ( ( X1 <  X2             ) &&
               ( Y1 >= 0              ) &&
               ( Y1 <= XvpnGraphicDy ) )
          {
            XDrawLine( XvpnGraphicDisplay,
                       XtWindow( XvpnGraphicWindow ),
                       XvpnXorGC,
                       X1, Y1, X2, Y2 );
          }
        }
        else
        {
          X2 = X1;

          if ( Y1 > Y2 ) { Swap = Y1; Y1 = Y2; Y2 = Swap; }

          if ( Y1 < 0              ) Y1 = 0;
          if ( Y2 > XvpnGraphicDy ) Y2 = XvpnGraphicDy;

          if ( ( Y1 <  Y2             ) &&
               ( X1 >= 0              ) &&
               ( X1 <= XvpnGraphicDx ) )
          {
            XDrawLine( XvpnGraphicDisplay, 
                       XtWindow( XvpnGraphicWindow ), 
                       XvpnXorGC,   
                       X1, Y1, X2, Y2 ); 
          }
        }

        break;

      case XVPN_INPUT_BOX :
 
        if ( X1 > X2 ) { Swap = X1; X1 = X2; X2 = Swap; }
        if ( Y1 > Y2 ) { Swap = Y1; Y1 = Y2; Y2 = Swap; }

        DrawLine = 0;

        if ( X1 < 0              ) { X1 = 0;              DrawLine |= XVPN_WEST;  }
        if ( X2 > XvpnGraphicDx ) { X2 = XvpnGraphicDx; DrawLine |= XVPN_EAST;  }
        if ( Y1 < 0              ) { Y1 = 0;              DrawLine |= XVPN_SOUTH; }
        if ( Y2 > XvpnGraphicDy ) { Y2 = XvpnGraphicDy; DrawLine |= XVPN_NORTH; }

        if ( DrawLine == 0 )
        {
          XDrawRectangle( XvpnGraphicDisplay,
                          XtWindow( XvpnGraphicWindow ),
                          XvpnXorGC,
                          X1, Y1,
                          X2 - X1, Y2 - Y1 );
        }
        else 
        {
          if ( ( DrawLine & XVPN_WEST ) == 0 )
          {
            XDrawLine( XvpnGraphicDisplay,
                       XtWindow( XvpnGraphicWindow ),
                       XvpnXorGC,
                       X1, Y2,
                       X1, Y1 );
          }

          if ( ( DrawLine & XVPN_EAST ) == 0 )
          {
            XDrawLine( XvpnGraphicDisplay,
                       XtWindow( XvpnGraphicWindow ),
                       XvpnXorGC,
                       X2, Y2,
                       X2, Y1 );
          }

          if ( ( DrawLine & XVPN_SOUTH ) == 0 )
          {
            XDrawLine( XvpnGraphicDisplay,
                       XtWindow( XvpnGraphicWindow ),
                       XvpnXorGC,
                       X1, Y1,
                       X2, Y1 );
          }

          if ( ( DrawLine & XVPN_NORTH ) == 0 )
          {
            XDrawLine( XvpnGraphicDisplay,
                       XtWindow( XvpnGraphicWindow ),
                       XvpnXorGC,
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
|                    XvpnUndisplayCursor                     |
|                                                             |
\------------------------------------------------------------*/

void XvpnUndisplayCursor()

{
  autbegin();

  if ( XvpnCursorInside == XVPN_TRUE )
  {
    if ( XvpnCursorSaved == XVPN_TRUE )
    {
      XvpnDrawCursor();
    }

    XvpnCursorSaved = XVPN_FALSE;
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XvpnDisplayCursor                     |
|                                                             |
\------------------------------------------------------------*/

void XvpnDisplayCursor()

{
  autbegin();

  if ( XvpnCursorInside == XVPN_TRUE )
  {
    if ( XvpnCursorSaved == XVPN_TRUE )
    {
      XvpnDrawCursor();

      XvpnCursorSaved = XVPN_FALSE;
    }

    if ( ( XvpnCursorY >= 0                       ) &&
         ( XvpnCursorX <= (Position)XvpnGraphicDx ) )
    {
      XvpnCursorSaveX = XvpnCursorX;
      XvpnCursorSaveY = XvpnCursorY;

      XvpnDrawCursor();

      XvpnCursorSaved = XVPN_TRUE;
    }
  }

  autend();
}
