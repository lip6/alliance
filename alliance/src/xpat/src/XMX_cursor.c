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
| Tool    :                   XPAT                           |
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
# include "pat.h"
# include "XSB.h"
# include "XPT.h"
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

  static char  XpatCursorBuffer[ XPAT_MAX_CURSOR_BUFFER ];

  char *XpatTimeUnit[ PAT_TU__MS + 1 ] =
  {
    "vu",
    "fs",
    "ps",
    "ns",
    "us",
    "ms"
  };

/*------------------------------------------------------------\
|                                                             |
|                         Coordinates                         |
|                                                             |
\------------------------------------------------------------*/

  Position XpatCursorX       = 0;
  Position XpatCursorY       = 0;
  Position XpatCursorSaveX   = 0;
  Position XpatCursorSaveY   = 0;
  char     XpatCursorSaved   = XPAT_FALSE;
  char     XpatCursorInside  = XPAT_FALSE;
  char     XpatCursorType    = XPAT_INPUT_HALF_BOX;

  long     XpatUnitCursorX = 0;
  long     XpatUnitCursorY = 0;
  long     XpatPixelCursorX  = 0;
  long     XpatPixelCursorY  = 0;

  long     XpatUnitCursorSaveX[ 2 ] = { 0, 0 };
  long     XpatUnitCursorSaveY[ 2 ] = { 0, 0 };
  char     XpatCursorIndex            = 0;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XpatComputeCursor                      |
|                                                             |
\------------------------------------------------------------*/

void XpatComputeCursor( X, Y )

     Position X;
     Position Y;
{
  autbegin();

  Y = XpatGraphicDy - Y;

  XpatUnitCursorX = X + XpatPixelGridX;
  XpatUnitCursorY = Y + XpatPixelGridY;

  if ( XpatUnitCursorX < 0 )
  {
    XpatUnitCursorX = ((float)(XpatUnitCursorX) / XpatUnitGridStep) - 0.5 ;
  }
  else
  {
    XpatUnitCursorX = ((float)(XpatUnitCursorX) / XpatUnitGridStep) + 0.5 ;
  }

  if ( XpatUnitCursorY < 0 )
  {
    XpatUnitCursorY = ((float)(XpatUnitCursorY) / XpatUnitGridStep) - 0.5 ;
  }
  else
  {
    XpatUnitCursorY = ((float)(XpatUnitCursorY) / XpatUnitGridStep) + 0.5 ;
  }

  XpatPixelCursorX  = ((float)(XpatUnitCursorX) * XpatUnitGridStep);
  XpatPixelCursorY  = ((float)(XpatUnitCursorY) * XpatUnitGridStep);

  XpatCursorX = XpatPixelCursorX - XpatPixelGridX;
  XpatCursorY = XpatPixelCursorY - XpatPixelGridY;
  XpatCursorY = XpatGraphicDy - XpatCursorY;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XpatDisplayCoordinates                   |
|                                                             |
\------------------------------------------------------------*/

void XpatDisplayCoordinates()

{
  char **NameArray;
  long   NumberIO;
  char  *NameX;
  char  *NameY;
  int    Unit;
  long   X;
  long   Y;
  long   Dx;
  long   Dy;
  long   TimeDelta;

  autbegin();

  X  = XpatUnitCursorX / XPAT_PATTERN_STEP_X;
  Y  = XpatUnitCursorY / XPAT_PATTERN_STEP_Y;
  Dx = X - ( XpatUnitCursorSaveX[0] / XPAT_PATTERN_STEP_X );
  Dy = Y - ( XpatUnitCursorSaveX[1] / XPAT_PATTERN_STEP_Y );

  NameX = (char *)0;
  NameY = (char *)0;

  if ( XpatFigurePat != (patfig_list *)0 )
  {
    NameArray = XpatFigurePat->IO_NAME_ARRAY;
    NumberIO  = XpatFigurePat->IO_NUMBER;

    if ( ( Y < NumberIO ) &&
         ( Y >= 0       ) )
    {
      NameY = NameArray[ Y ];;
    }
  }

  if ( NameY == (char *)0 ) XpatCursorBuffer[ 0 ] = '\0';
  else                      sprintf( XpatCursorBuffer, "%s", NameY );

  XpatDisplayMessage( XPAT_MESSAGE_Y, XpatCursorBuffer );

  if ( XpatFigurePat != (patfig_list *)0 )
  {
    Unit      = XpatFigurePat->TIME_UNIT;
    TimeDelta = XpatFigurePat->TIME_DELTA;
  }
  else
  {
    Unit      = 0;
    TimeDelta = 1;
  }

  sprintf( XpatCursorBuffer, "%ld %s", X * TimeDelta, XpatTimeUnit[ Unit ] );
  XpatDisplayMessage( XPAT_MESSAGE_X, XpatCursorBuffer );

  sprintf( XpatCursorBuffer, "%ld %s", Dx * TimeDelta, XpatTimeUnit[ Unit ] );
  XpatDisplayMessage( XPAT_MESSAGE_DX, XpatCursorBuffer );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XpatPointCursor                     |
|                                                             |
\------------------------------------------------------------*/

void XpatPointCursor()

{
  autbegin();

  XpatUndisplayCursor();

  XpatUnitCursorSaveX[ XpatCursorIndex ] = XpatUnitCursorX;
  XpatUnitCursorSaveY[ XpatCursorIndex ] = XpatUnitCursorY;

  XpatCursorIndex = XpatCursorIndex + 1;

  XpatDisplayCursor();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XpatResetCursor                     |
|                                                             |
\------------------------------------------------------------*/

void XpatResetCursor()

{
  autbegin();

  XpatUndisplayCursor();

  XpatCursorIndex = XpatCursorIndex - 1;

  XpatDisplayCursor();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XpatChangeCursor                    |
|                                                             |
\------------------------------------------------------------*/

void XpatChangeCursorType( ArrayX, ArrayY, Index, Type )

   long *ArrayX;
   long *ArrayY;
   char  Index;
   char  Type;
{
  autbegin();

  XpatUndisplayCursor();

  XpatCursorType  = Type;
  XpatCursorIndex = Index;

  if ( Index != 0 )
  {
    XpatUnitCursorSaveX[ 0 ] = ArrayX[0];
    XpatUnitCursorSaveY[ 0 ] = ArrayY[0];
    XpatUnitCursorSaveX[ 1 ] = ArrayX[1];
    XpatUnitCursorSaveY[ 1 ] = ArrayY[1];
  }
  
  XpatDisplayCursor();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XpatDrawCursor                      |
|                                                             |
\------------------------------------------------------------*/

void XpatDrawCursor()

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

  XDrawLine( XpatGraphicDisplay,
             XtWindow( XpatGraphicWindow ),
             XpatXorGC,
             XpatCursorSaveX - XPAT_CURSOR_SIZE, 
             XpatCursorSaveY - XPAT_CURSOR_SIZE,
             XpatCursorSaveX + XPAT_CURSOR_SIZE, 
             XpatCursorSaveY + XPAT_CURSOR_SIZE );

  XDrawLine( XpatGraphicDisplay,
             XtWindow( XpatGraphicWindow ),
             XpatXorGC,
             XpatCursorSaveX - XPAT_CURSOR_SIZE,
             XpatCursorSaveY + XPAT_CURSOR_SIZE,
             XpatCursorSaveX + XPAT_CURSOR_SIZE, 
             XpatCursorSaveY - XPAT_CURSOR_SIZE );

  if ( XpatCursorIndex > 0 )
  {
    X1 = (float)(XpatUnitCursorSaveX[0]) * XpatUnitGridStep;
    Y1 = (float)(XpatUnitCursorSaveY[0]) * XpatUnitGridStep;
    X1 = X1 - XpatPixelGridX;
    Y1 = Y1 - XpatPixelGridY;
    Y1 = XpatGraphicDy - Y1;

    if ( XpatCursorIndex == 1 )
    {
      X2 = XpatCursorSaveX;
      Y2 = XpatCursorSaveY;
    }
    else
    {
      X2 = (float)(XpatUnitCursorSaveX[1]) * XpatUnitGridStep;
      Y2 = (float)(XpatUnitCursorSaveY[1]) * XpatUnitGridStep;
      X2 = X2 - XpatPixelGridX;
      Y2 = Y2 - XpatPixelGridY;
      Y2 = XpatGraphicDy - Y2;
    }

    switch( XpatCursorType )
    {
      case XPAT_INPUT_POINT :

        break;

      case XPAT_INPUT_LINE  :

        XDrawLine( XpatGraphicDisplay,
                   XtWindow( XpatGraphicWindow ),
                   XpatXorGC,
                   X1, Y1, X2, Y2 );
        break;

      case XPAT_INPUT_HALF_BOX :

        XDrawLine( XpatGraphicDisplay,
                   XtWindow( XpatGraphicWindow ),
                   XpatXorGC,
                   X1, Y1, X2, Y1 );

        XDrawLine( XpatGraphicDisplay,
                   XtWindow( XpatGraphicWindow ),
                   XpatXorGC,
                   X2, 0, X2, XpatGraphicDy );

        break; 

      case XPAT_INPUT_ORTHO   :
      case XPAT_INPUT_SORTHO  :
      case XPAT_INPUT_LSTRING :

        DeltaX = X2 - X1; if ( DeltaX < 0 ) DeltaX = - DeltaX;
        DeltaY = Y2 - Y1; if ( DeltaY < 0 ) DeltaY = - DeltaY;

        if ( DeltaX > DeltaY )
        {
          Y2 = Y1;

          if ( X1 > X2 ) { Swap = X1; X1 = X2; X2 = Swap; }

          if ( X1 < 0              ) X1 = 0;
          if ( X2 > XpatGraphicDx ) X2 = XpatGraphicDx;

          if ( ( X1 <  X2             ) &&
               ( Y1 >= 0              ) &&
               ( Y1 <= XpatGraphicDy ) )
          {
            XDrawLine( XpatGraphicDisplay,
                       XtWindow( XpatGraphicWindow ),
                       XpatXorGC,
                       X1, Y1, X2, Y2 );
          }
        }
        else
        {
          X2 = X1;

          if ( Y1 > Y2 ) { Swap = Y1; Y1 = Y2; Y2 = Swap; }

          if ( Y1 < 0              ) Y1 = 0;
          if ( Y2 > XpatGraphicDy ) Y2 = XpatGraphicDy;

          if ( ( Y1 <  Y2             ) &&
               ( X1 >= 0              ) &&
               ( X1 <= XpatGraphicDx ) )
          {
            XDrawLine( XpatGraphicDisplay, 
                       XtWindow( XpatGraphicWindow ), 
                       XpatXorGC,   
                       X1, Y1, X2, Y2 ); 
          }
        }

        break;

      case XPAT_INPUT_BOX :
 
        if ( X1 > X2 ) { Swap = X1; X1 = X2; X2 = Swap; }
        if ( Y1 > Y2 ) { Swap = Y1; Y1 = Y2; Y2 = Swap; }

        DrawLine = 0;

        if ( X1 < 0              ) { X1 = 0;              DrawLine |= XPAT_WEST;  }
        if ( X2 > XpatGraphicDx ) { X2 = XpatGraphicDx; DrawLine |= XPAT_EAST;  }
        if ( Y1 < 0              ) { Y1 = 0;              DrawLine |= XPAT_SOUTH; }
        if ( Y2 > XpatGraphicDy ) { Y2 = XpatGraphicDy; DrawLine |= XPAT_NORTH; }

        if ( DrawLine == 0 )
        {
          XDrawRectangle( XpatGraphicDisplay,
                          XtWindow( XpatGraphicWindow ),
                          XpatXorGC,
                          X1, Y1,
                          X2 - X1, Y2 - Y1 );
        }
        else 
        {
          if ( ( DrawLine & XPAT_WEST ) == 0 )
          {
            XDrawLine( XpatGraphicDisplay,
                       XtWindow( XpatGraphicWindow ),
                       XpatXorGC,
                       X1, Y2,
                       X1, Y1 );
          }

          if ( ( DrawLine & XPAT_EAST ) == 0 )
          {
            XDrawLine( XpatGraphicDisplay,
                       XtWindow( XpatGraphicWindow ),
                       XpatXorGC,
                       X2, Y2,
                       X2, Y1 );
          }

          if ( ( DrawLine & XPAT_SOUTH ) == 0 )
          {
            XDrawLine( XpatGraphicDisplay,
                       XtWindow( XpatGraphicWindow ),
                       XpatXorGC,
                       X1, Y1,
                       X2, Y1 );
          }

          if ( ( DrawLine & XPAT_NORTH ) == 0 )
          {
            XDrawLine( XpatGraphicDisplay,
                       XtWindow( XpatGraphicWindow ),
                       XpatXorGC,
                       X1, Y2,
                       X2, Y2 );
          }
        }

      break;

    }
  }
  else
  {
    XDrawLine( XpatGraphicDisplay,
               XtWindow( XpatGraphicWindow ),
               XpatXorGC,
               XpatCursorSaveX, 0, XpatCursorSaveX, XpatGraphicDy );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XpatUndisplayCursor                     |
|                                                             |
\------------------------------------------------------------*/

void XpatUndisplayCursor()

{
  autbegin();

  if ( XpatCursorInside == XPAT_TRUE )
  {
    if ( XpatCursorSaved == XPAT_TRUE )
    {
      XpatDrawCursor();
    }

    XpatCursorSaved = XPAT_FALSE;
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XpatDisplayCursor                     |
|                                                             |
\------------------------------------------------------------*/

void XpatDisplayCursor()

{
  autbegin();

  if ( XpatCursorInside == XPAT_TRUE )
  {
    if ( XpatCursorSaved == XPAT_TRUE )
    {
      XpatDrawCursor();

      XpatCursorSaved = XPAT_FALSE;
    }

    if ( ( XpatCursorY >= 0              ) &&
         ( XpatCursorX <= XpatGraphicDx ) )
    {
      XpatCursorSaveX = XpatCursorX;
      XpatCursorSaveY = XpatCursorY;

      XpatDrawCursor();

      XpatCursorSaved = XPAT_TRUE;
    }
  }

  autend();
}
