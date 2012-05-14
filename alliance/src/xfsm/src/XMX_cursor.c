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
| Tool    :                   XFSM                           |
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
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
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

  static char  XfsmCursorBuffer[ XFSM_MAX_CURSOR_BUFFER ];

/*------------------------------------------------------------\
|                                                             |
|                         Coordinates                         |
|                                                             |
\------------------------------------------------------------*/

  Position XfsmCursorX       = 0;
  Position XfsmCursorY       = 0;
  Position XfsmCursorSaveX   = 0;
  Position XfsmCursorSaveY   = 0;
  char     XfsmCursorSaved   = XFSM_FALSE;
  char     XfsmCursorInside  = XFSM_FALSE;
  char     XfsmCursorType    = XFSM_INPUT_HALF_BOX;

  long     XfsmUnitCursorX = 0;
  long     XfsmUnitCursorY = 0;
  long     XfsmPixelCursorX  = 0;
  long     XfsmPixelCursorY  = 0;

  long     XfsmUnitCursorSaveX[ 2 ] = { 0, 0 };
  long     XfsmUnitCursorSaveY[ 2 ] = { 0, 0 };
  char     XfsmCursorIndex            = 0;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XfsmComputeCursor                     |
|                                                             |
\------------------------------------------------------------*/

void XfsmComputeCursor( X, Y )

     Position X;
     Position Y;
{
  autbegin();

  Y = XfsmGraphicDy - Y;

  XfsmUnitCursorX = X + XfsmPixelGridX;
  XfsmUnitCursorY = Y + XfsmPixelGridY;

  if ( XfsmUnitCursorX < 0 )
  {
    XfsmUnitCursorX = ((float)(XfsmUnitCursorX) / XfsmUnitGridStep) - 0.5 ;
  }
  else
  {
    XfsmUnitCursorX = ((float)(XfsmUnitCursorX) / XfsmUnitGridStep) + 0.5 ;
  }

  if ( XfsmUnitCursorY < 0 )
  {
    XfsmUnitCursorY = ((float)(XfsmUnitCursorY) / XfsmUnitGridStep) - 0.5 ;
  }
  else
  {
    XfsmUnitCursorY = ((float)(XfsmUnitCursorY) / XfsmUnitGridStep) + 0.5 ;
  }

  XfsmPixelCursorX  = ((float)(XfsmUnitCursorX) * XfsmUnitGridStep);
  XfsmPixelCursorY  = ((float)(XfsmUnitCursorY) * XfsmUnitGridStep);

  XfsmCursorX = XfsmPixelCursorX - XfsmPixelGridX;
  XfsmCursorY = XfsmPixelCursorY - XfsmPixelGridY;
  XfsmCursorY = XfsmGraphicDy - XfsmCursorY;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XfsmDisplayCoordinates                  |
|                                                             |
\------------------------------------------------------------*/

void XfsmDisplayCoordinates()

{
  autbegin();

  sprintf( XfsmCursorBuffer, "%d", XfsmUnitCursorX );
  XfsmDisplayMessage( XFSM_MESSAGE_X, XfsmCursorBuffer );

  sprintf( XfsmCursorBuffer, "%d", XfsmUnitCursorY );
  XfsmDisplayMessage( XFSM_MESSAGE_Y, XfsmCursorBuffer );

  sprintf( XfsmCursorBuffer, "%d", XfsmUnitCursorX - XfsmUnitCursorSaveX[0] );
  XfsmDisplayMessage( XFSM_MESSAGE_DX, XfsmCursorBuffer );

  sprintf( XfsmCursorBuffer, "%d", XfsmUnitCursorY - XfsmUnitCursorSaveY[0] );
  XfsmDisplayMessage( XFSM_MESSAGE_DY, XfsmCursorBuffer );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XfsmPointCursor                     |
|                                                             |
\------------------------------------------------------------*/

void XfsmPointCursor()

{
  autbegin();

  XfsmUndisplayCursor();

  XfsmUnitCursorSaveX[ XfsmCursorIndex ] = XfsmUnitCursorX;
  XfsmUnitCursorSaveY[ XfsmCursorIndex ] = XfsmUnitCursorY;

  XfsmCursorIndex = XfsmCursorIndex + 1;

  XfsmDisplayCursor();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XfsmResetCursor                     |
|                                                             |
\------------------------------------------------------------*/

void XfsmResetCursor()

{
  autbegin();

  XfsmUndisplayCursor();

  XfsmCursorIndex = XfsmCursorIndex - 1;

  XfsmDisplayCursor();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XfsmChangeCursor                    |
|                                                             |
\------------------------------------------------------------*/

void XfsmChangeCursorType( ArrayX, ArrayY, Index, Type )

   long *ArrayX;
   long *ArrayY;
   char  Index;
   char  Type;
{
  autbegin();

  XfsmUndisplayCursor();

  XfsmCursorType  = Type;
  XfsmCursorIndex = Index;

  if ( Index != 0 )
  {
    XfsmUnitCursorSaveX[ 0 ] = ArrayX[0];
    XfsmUnitCursorSaveY[ 0 ] = ArrayY[0];
    XfsmUnitCursorSaveX[ 1 ] = ArrayX[1];
    XfsmUnitCursorSaveY[ 1 ] = ArrayY[1];
  }
  
  XfsmDisplayCursor();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XfsmDrawCursor                      |
|                                                             |
\------------------------------------------------------------*/

void XfsmDrawCursor()

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

  XDrawLine( XfsmGraphicDisplay,
             XtWindow( XfsmGraphicWindow ),
             XfsmXorGC,
             XfsmCursorSaveX - XFSM_CURSOR_SIZE, 
             XfsmCursorSaveY - XFSM_CURSOR_SIZE,
             XfsmCursorSaveX + XFSM_CURSOR_SIZE, 
             XfsmCursorSaveY + XFSM_CURSOR_SIZE );

  XDrawLine( XfsmGraphicDisplay,
             XtWindow( XfsmGraphicWindow ),
             XfsmXorGC,
             XfsmCursorSaveX - XFSM_CURSOR_SIZE,
             XfsmCursorSaveY + XFSM_CURSOR_SIZE,
             XfsmCursorSaveX + XFSM_CURSOR_SIZE, 
             XfsmCursorSaveY - XFSM_CURSOR_SIZE );

  if ( XfsmCursorIndex > 0 )
  {
    X1 = (float)(XfsmUnitCursorSaveX[0]) * XfsmUnitGridStep;
    Y1 = (float)(XfsmUnitCursorSaveY[0]) * XfsmUnitGridStep;
    X1 = X1 - XfsmPixelGridX;
    Y1 = Y1 - XfsmPixelGridY;
    Y1 = XfsmGraphicDy - Y1;

    if ( XfsmCursorIndex == 1 )
    {
      X2 = XfsmCursorSaveX;
      Y2 = XfsmCursorSaveY;
    }
    else
    {
      X2 = (float)(XfsmUnitCursorSaveX[1]) * XfsmUnitGridStep;
      Y2 = (float)(XfsmUnitCursorSaveY[1]) * XfsmUnitGridStep;
      X2 = X2 - XfsmPixelGridX;
      Y2 = Y2 - XfsmPixelGridY;
      Y2 = XfsmGraphicDy - Y2;
    }

    switch( XfsmCursorType )
    {
      case XFSM_INPUT_POINT :

        break;

      case XFSM_INPUT_LINE  :

        XDrawLine( XfsmGraphicDisplay,
                   XtWindow( XfsmGraphicWindow ),
                   XfsmXorGC,
                   X1, Y1, X2, Y2 );
        break;

      case XFSM_INPUT_HALF_BOX :

        XDrawLine( XfsmGraphicDisplay,
                   XtWindow( XfsmGraphicWindow ),
                   XfsmXorGC,
                   X1, Y1, X2, Y1 );

        XDrawLine( XfsmGraphicDisplay,
                   XtWindow( XfsmGraphicWindow ),
                   XfsmXorGC,
                   X2, Y1, X2, Y2 );
        break; 

      case XFSM_INPUT_ORTHO   :
      case XFSM_INPUT_SORTHO  :
      case XFSM_INPUT_LSTRING :

        DeltaX = X2 - X1; if ( DeltaX < 0 ) DeltaX = - DeltaX;
        DeltaY = Y2 - Y1; if ( DeltaY < 0 ) DeltaY = - DeltaY;

        if ( DeltaX > DeltaY )
        {
          Y2 = Y1;

          if ( X1 > X2 ) { Swap = X1; X1 = X2; X2 = Swap; }

          if ( X1 < 0              ) X1 = 0;
          if ( X2 > XfsmGraphicDx ) X2 = XfsmGraphicDx;

          if ( ( X1 <  X2             ) &&
               ( Y1 >= 0              ) &&
               ( Y1 <= XfsmGraphicDy ) )
          {
            XDrawLine( XfsmGraphicDisplay,
                       XtWindow( XfsmGraphicWindow ),
                       XfsmXorGC,
                       X1, Y1, X2, Y2 );
          }
        }
        else
        {
          X2 = X1;

          if ( Y1 > Y2 ) { Swap = Y1; Y1 = Y2; Y2 = Swap; }

          if ( Y1 < 0              ) Y1 = 0;
          if ( Y2 > XfsmGraphicDy ) Y2 = XfsmGraphicDy;

          if ( ( Y1 <  Y2             ) &&
               ( X1 >= 0              ) &&
               ( X1 <= XfsmGraphicDx ) )
          {
            XDrawLine( XfsmGraphicDisplay, 
                       XtWindow( XfsmGraphicWindow ), 
                       XfsmXorGC,   
                       X1, Y1, X2, Y2 ); 
          }
        }

        break;

      case XFSM_INPUT_BOX :
 
        if ( X1 > X2 ) { Swap = X1; X1 = X2; X2 = Swap; }
        if ( Y1 > Y2 ) { Swap = Y1; Y1 = Y2; Y2 = Swap; }

        DrawLine = 0;

        if ( X1 < 0              ) { X1 = 0;              DrawLine |= XFSM_WEST;  }
        if ( X2 > XfsmGraphicDx ) { X2 = XfsmGraphicDx; DrawLine |= XFSM_EAST;  }
        if ( Y1 < 0              ) { Y1 = 0;              DrawLine |= XFSM_SOUTH; }
        if ( Y2 > XfsmGraphicDy ) { Y2 = XfsmGraphicDy; DrawLine |= XFSM_NORTH; }

        if ( DrawLine == 0 )
        {
          XDrawRectangle( XfsmGraphicDisplay,
                          XtWindow( XfsmGraphicWindow ),
                          XfsmXorGC,
                          X1, Y1,
                          X2 - X1, Y2 - Y1 );
        }
        else 
        {
          if ( ( DrawLine & XFSM_WEST ) == 0 )
          {
            XDrawLine( XfsmGraphicDisplay,
                       XtWindow( XfsmGraphicWindow ),
                       XfsmXorGC,
                       X1, Y2,
                       X1, Y1 );
          }

          if ( ( DrawLine & XFSM_EAST ) == 0 )
          {
            XDrawLine( XfsmGraphicDisplay,
                       XtWindow( XfsmGraphicWindow ),
                       XfsmXorGC,
                       X2, Y2,
                       X2, Y1 );
          }

          if ( ( DrawLine & XFSM_SOUTH ) == 0 )
          {
            XDrawLine( XfsmGraphicDisplay,
                       XtWindow( XfsmGraphicWindow ),
                       XfsmXorGC,
                       X1, Y1,
                       X2, Y1 );
          }

          if ( ( DrawLine & XFSM_NORTH ) == 0 )
          {
            XDrawLine( XfsmGraphicDisplay,
                       XtWindow( XfsmGraphicWindow ),
                       XfsmXorGC,
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
|                    XfsmUndisplayCursor                     |
|                                                             |
\------------------------------------------------------------*/

void XfsmUndisplayCursor()

{
  autbegin();

  if ( XfsmCursorInside == XFSM_TRUE )
  {
    if ( XfsmCursorSaved == XFSM_TRUE )
    {
      XfsmDrawCursor();
    }

    XfsmCursorSaved = XFSM_FALSE;
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XfsmDisplayCursor                     |
|                                                             |
\------------------------------------------------------------*/

void XfsmDisplayCursor()

{
  autbegin();

  if ( XfsmCursorInside == XFSM_TRUE )
  {
    if ( XfsmCursorSaved == XFSM_TRUE )
    {
      XfsmDrawCursor();

      XfsmCursorSaved = XFSM_FALSE;
    }

    if ( ( XfsmCursorY >= 0              ) &&
         ( XfsmCursorX <= XfsmGraphicDx ) )
    {
      XfsmCursorSaveX = XfsmCursorX;
      XfsmCursorSaveY = XfsmCursorY;

      XfsmDrawCursor();

      XfsmCursorSaved = XFSM_TRUE;
    }
  }

  autend();
}
