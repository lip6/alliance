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
| Tool    :                   XPAT                            |
|                                                             |
| File    :                   View.c                          |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                  28.01.95                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <math.h>
# include <signal.h>
# include <Xm/Xm.h>
 
# include "mut.h"
# include "aut.h"
# include "pat.h"
# include "XSB.h"
# include "XPT.h"
# include "XMX.h"

# include "XMX_view.h" 

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

    static long         XpatGraphicX1;
    static long         XpatGraphicX2;
    static long         XpatGraphicY1;
    static long         XpatGraphicY2;
    static XFontStruct *XpatTextFont;
    static GC           XpatTextGC;
    static GC           XpatDrawGC;
    static char         XpatInterrupt = 0;

    static xpatview_list *XpatHeadView = (xpatview_list *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Alloc Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         XpatAllocView                       |
|                                                             |
\------------------------------------------------------------*/

xpatview_list *XpatAllocView()
{
  return( (xpatview_list *)autallocheap( sizeof( xpatview_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Free Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XpatFreeView                       |
|                                                             |
\*-----------------------------------------------------------*/

void XpatFreeView( FreeView )

   xpatview_list *FreeView;
{
  autfreeheap( (char *)FreeView, sizeof( xpatview_list ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        XpatAddViewLater                     |
|                                                             |
\------------------------------------------------------------*/

void XpatAddViewLater( Obj )

   xpatobj_list *Obj;
{
  xpatview_list *View;

  autbegin();

  View = XpatAllocView();

  View->OBJECT  = Obj;
  View->NEXT    = XpatHeadView;
  XpatHeadView  = View;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XpatDelView                          |
|                                                             |
\------------------------------------------------------------*/

void XpatDelView()
{
  xpatview_list *DelView;
  xpatview_list *View;

  autbegin();

  View          = XpatHeadView;
  XpatHeadView = (xpatview_list *)NULL;

  while( View != (xpatview_list *)NULL )
  {
    DelView  = View;
    View     = View->NEXT;

    XpatFreeView( DelView );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XpatInterruptDisplay                    |
|                                                             |
\------------------------------------------------------------*/

void XpatInterruptDisplay()
{
  XEvent       Event;
  KeySym       Key;
  char         Text;

  autbegin();

# ifdef XPAT_CTRL_C
  if ( XCheckTypedEvent( XpatGraphicDisplay, KeyPress, &Event ) )
  {
    XLookupString( &Event.xkey, &Text, 1, &Key, 0 );

    if ( Text == '\003' ) 
    {
      XpatInterrupt = 1;

      XBell( XpatGraphicDisplay, 0 );
    }
  }
# endif

  signal( SIGALRM, XpatInterruptDisplay );
  alarm( 1 );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XpatFlushEventDisplay                   |
|                                                             |
\------------------------------------------------------------*/

void XpatFlushEventDisplay()
{
  XEvent Event;
  
  autbegin();

# ifdef XPAT_CTRL_C
  while ( XCheckTypedEvent( XpatGraphicDisplay, KeyPress, &Event ) );
# endif

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XpatComputeArrow                      |
|                                                             |
\------------------------------------------------------------*/

char XpatComputeArrow( X1, Y1, X2, Y2, Points )

  long    X1;
  long    Y1;
  long    X2;
  long    Y2;
  XPoint *Points;
{
  float LineX;
  float LineY;
  float HalfX;
  float HalfY;
  float CrossX;
  float CrossY;
  float Norm;

  LineX = (float)( X2 - X1 );
  LineY = (float)( Y2 - Y1 );

  LineY = LineY * 2.0 / XPAT_PATTERN_STEP_Y;

  if ( ( LineX != 0.0 ) ||
       ( LineY != 0.0 ) )
  {
    Norm    = 3 * sqrt( ( LineX * LineX ) + ( LineY * LineY ) );
    Norm    = Norm  / XpatUnitGridStep;
    LineX   = LineX / Norm;
    LineY   = LineY / Norm;

    LineY   = LineY * XPAT_PATTERN_STEP_Y / 2.0;

    HalfX   = LineX / 2;
    HalfY   = LineY / 2;
    CrossX  = - HalfY;
    CrossY  =   HalfX;

    Points[ 0 ].x = X1 + LineX;
    Points[ 0 ].y = Y1 + LineY;
    Points[ 1 ].x = Points[ 0 ].x + LineX + HalfX + CrossX;
    Points[ 1 ].y = Points[ 0 ].y + LineY + HalfY + CrossY;
    Points[ 2 ].x = Points[ 0 ].x + LineX + HalfX - CrossX;
    Points[ 2 ].y = Points[ 0 ].y + LineY + HalfY - CrossY;

    return( XPAT_TRUE );
  }

  return( XPAT_FALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                       XpatComputeHexagon                    |
|                                                             |
\------------------------------------------------------------*/

char XpatComputeHexagon( X1, Y1, X2, Y2, Points )

  long    X1;
  long    Y1;
  long    X2;
  long    Y2;
  XPoint *Points;
{
  long StepX;
  long StepY;
  long DeltaX;

  StepY = ( Y2 - Y1 ) >> 1;
  StepX = StepY >> 1;

  DeltaX = X2 - X1;

  if ( DeltaX < StepY ) StepX = (DeltaX >> 1);

  if ( ( StepY > 0 ) &&
       ( StepX > 0 ) )
  {
    Points[ 0 ].x = X1 + StepX;
    Points[ 0 ].y = Y1;
    Points[ 1 ].x = X1;
    Points[ 1 ].y = Y1 + StepY;
    Points[ 2 ].x = X1 + StepX;
    Points[ 2 ].y = Y2;
    Points[ 3 ].x = X2 - StepX;
    Points[ 3 ].y = Y2;
    Points[ 4 ].x = X2;
    Points[ 4 ].y = Y1 + StepY;
    Points[ 5 ].x = X2 - StepX;
    Points[ 5 ].y = Y1;
    Points[ 6 ].x = X1 + StepX;
    Points[ 6 ].y = Y1;

    return( XPAT_TRUE );
  }

  return( XPAT_FALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                     XpatDisplayOneObject                    |
|                                                             |
\------------------------------------------------------------*/

void XpatDisplayOneObject( Obj )

   xpatobj_list *Obj;
{
  XPoint   Arrow[ 3 ];
  XPoint   Hexagon[ 7 ];
  int      Index;
  long     X1r;
  long     X2r;
  long     Y1r;
  long     Y2r;
  long     DeltaX;
  long     DeltaY;

  if ( IsXpatLineLeft( Obj ) )
  {
    X1r = Obj->X + Obj->DX;
    X2r = Obj->X;
  }
  else
  {
    X1r = Obj->X;
    X2r = Obj->X + Obj->DX;
  }

  if ( IsXpatLineDown( Obj ) )
  {
    Y1r  = Obj->Y + Obj->DY;
    Y2r  = Obj->Y;
  }
  else
  {
    Y1r  = Obj->Y;
    Y2r  = Obj->Y + Obj->DY;
  }

  X1r  = (float)( X1r ) * XpatUnitGridStep;
  Y1r  = (float)( Y1r ) * XpatUnitGridStep;
  X2r  = (float)( X2r ) * XpatUnitGridStep;
  Y2r  = (float)( Y2r ) * XpatUnitGridStep;

  X1r  = ( X1r / XPAT_UNIT ) - XpatPixelGridX;
  X2r  = ( X2r / XPAT_UNIT ) - XpatPixelGridX;
  Y1r  = ( Y1r / XPAT_UNIT ) - XpatPixelGridY;
  Y2r  = ( Y2r / XPAT_UNIT ) - XpatPixelGridY;

  if ( ( ( X1r < 0                   ) && 
         ( X2r < 0                   ) ) ||
       ( ( X1r > (long)XpatGraphicDx ) &&
         ( X2r > (long)XpatGraphicDx ) ) ||
       ( ( Y1r < 0                   ) && 
         ( Y2r < 0                   ) ) ||
       ( ( Y1r > (long)XpatGraphicDy ) &&
         ( Y2r > (long)XpatGraphicDy ) ) )
  {
    return;
  }

  switch ( GetXpatObjectType( Obj ) )
  {
    case XPAT_OBJECT_LINE :

      if ( Y1r == Y2r )
      {
        if ( X1r < 0                   ) X1r = 0;
        if ( X2r < 0                   ) X2r = 0;
        if ( X1r > (long)XpatGraphicDx ) X1r = (long)XpatGraphicDx;
        if ( X2r > (long)XpatGraphicDx ) X2r = (long)XpatGraphicDx;
      }

      XDrawLine( XpatGraphicDisplay,
                 XpatGraphicPixmap,
                 XpatDrawGC,
                 X1r, XpatGraphicDy - Y1r,
                 X2r, XpatGraphicDy - Y2r );
    break;

    case XPAT_OBJECT_ARROW :

      XDrawLine( XpatGraphicDisplay,
                 XpatGraphicPixmap,
                 XpatDrawGC,
                 X1r, XpatGraphicDy - Y1r,
                 X2r, XpatGraphicDy - Y2r );

      if ( XpatComputeArrow( X1r, Y1r, X2r, Y2r, Arrow ) )
      {
        for ( Index = 0; Index < 3; Index++ )
        {
           Arrow[ Index ].y = XpatGraphicDy - Arrow[ Index ].y;
        }

        XFillPolygon( XpatGraphicDisplay,
                      XpatGraphicPixmap, 
                      XpatDrawGC, Arrow, 3, 
                      Convex,
                      CoordModeOrigin );
      }

    break;

    case XPAT_OBJECT_CIRCLE :

      DeltaX = X2r - X1r;
      DeltaY = Y2r - Y1r;

      if ( DeltaX <= 0 ) DeltaX = 1;
      if ( DeltaY <= 0 ) DeltaY = 1;

      XFillArc( XpatGraphicDisplay,
                XpatGraphicPixmap,
                XpatDrawGC,
                X1r, XpatGraphicDy - Y2r, 
                DeltaX, DeltaY, 0, 23040 );
    break;

    case XPAT_OBJECT_RECTANGLE :

      if ( X1r < -1                  ) X1r = -1;
      if ( X2r < -1                  ) X2r = -1;
      if ( X1r > (long)XpatGraphicDx ) X1r = (long)XpatGraphicDx + 1;
      if ( X2r > (long)XpatGraphicDx ) X2r = (long)XpatGraphicDx + 1;

      DeltaX = X2r - X1r;
      DeltaY = Y2r - Y1r;

      if ( DeltaX <= 0 ) DeltaX = 1;
      if ( DeltaY <= 0 ) DeltaY = 1;

      XFillRectangle( XpatGraphicDisplay,
                      XpatGraphicPixmap,
                      XpatDrawGC,
                      X1r, XpatGraphicDy - Y2r,
                      DeltaX, DeltaY );

    break;

    case XPAT_OBJECT_HEXAGON :

      if ( X1r < -100                     ) X1r = -100;
      if ( X2r < -100                     ) X2r = -100;
      if ( X1r > (long)XpatGraphicDx + 100) X1r = (long)XpatGraphicDx + 100;
      if ( X2r > (long)XpatGraphicDx + 100) X2r = (long)XpatGraphicDx + 100;

      if ( XpatComputeHexagon( X1r, Y1r, X2r, Y2r, Hexagon ) )
      {
        for ( Index = 0; Index < 7; Index++ )
        {
           Hexagon[ Index ].y = XpatGraphicDy - Hexagon[ Index ].y;
        }

        XFillPolygon( XpatGraphicDisplay,
                      XpatGraphicPixmap,
                      XpatDrawGC, Hexagon, 7,
                      Convex, CoordModeOrigin );
      }
      else
      {
        XDrawLine( XpatGraphicDisplay,
                   XpatGraphicPixmap,
                   XpatDrawGC,
                   X1r, XpatGraphicDy - Y1r,
                   X2r, XpatGraphicDy - Y2r );
      }

    break;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     XpatDisplayOneString                    |
|                                                             |
\------------------------------------------------------------*/

void XpatDisplayOneString( Obj )

   xpatobj_list *Obj;
{
  long    X1r;
  long    X2r;
  long    Y1r;
  long    Y2r;
  long    DeltaX;
  long    DeltaY;
  long    WidthText;
  long    HeightText;
  int     Length;

  if ( IsXpatLineLeft( Obj ) )
  {
    X1r = Obj->X + Obj->DX;
    X2r = Obj->X;
  }
  else
  {
    X1r = Obj->X;
    X2r = Obj->X + Obj->DX;
  }

  if ( IsXpatLineDown( Obj ) )
  {
    Y1r  = Obj->Y + Obj->DY;
    Y2r  = Obj->Y;
  }
  else
  {
    Y1r  = Obj->Y;
    Y2r  = Obj->Y + Obj->DY;
  }

  X1r  = (float)( X1r ) * XpatUnitGridStep;
  Y1r  = (float)( Y1r ) * XpatUnitGridStep;
  X2r  = (float)( X2r ) * XpatUnitGridStep;
  Y2r  = (float)( Y2r ) * XpatUnitGridStep;

  X1r  = ( X1r / XPAT_UNIT ) - XpatPixelGridX;
  X2r  = ( X2r / XPAT_UNIT ) - XpatPixelGridX;
  Y1r  = ( Y1r / XPAT_UNIT ) - XpatPixelGridY;
  Y2r  = ( Y2r / XPAT_UNIT ) - XpatPixelGridY;

  DeltaX = X2r - X1r;
  DeltaY = Y2r - Y1r;

  if ( DeltaX <= 0 ) DeltaX = 1;
  if ( DeltaY <= 0 ) DeltaY = 1;

  if ( IsXpatIO( Obj ) )
  {
    Length     = strlen( Obj->NAME );
    WidthText  = XTextWidth( XpatTextFont, Obj->NAME, Length );
    HeightText = XpatTextFont->ascent;

    XDrawString( XpatGraphicDisplay,
                 XpatGraphicPixmap,
                 XpatTextGC,
                 ( X2r - WidthText ),
                 XpatGraphicDy - (( Y1r + Y2r - HeightText ) >> 1),
                 Obj->NAME, Length );
  }
  else
  if ( IsXpatCursor( Obj ) )
  {
    Length     = strlen( Obj->NAME );
    WidthText  = XTextWidth( XpatTextFont, Obj->NAME, Length );
    HeightText = XpatTextFont->ascent;

    XDrawString( XpatGraphicDisplay,
                 XpatGraphicPixmap,
                 XpatTextGC,
                 X2r - (WidthText >> 1),
                 XpatGraphicDy - ( Y2r + HeightText ),
                 Obj->NAME, Length );
  }
  else
  {
    Length     = strlen( Obj->NAME );
    WidthText  = XTextWidth( XpatTextFont, Obj->NAME, Length );
    HeightText = XpatTextFont->ascent;

    if ( WidthText <= ( X2r - X1r ) )
    {
      XDrawString( XpatGraphicDisplay,
                   XpatGraphicPixmap,
                   XpatTextGC,
                   (( X1r + X2r - WidthText ) >> 1),
                   XpatGraphicDy - (( Y1r + Y2r - HeightText ) >> 1),
                   Obj->NAME, Length );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     XpatDisplayFigure                       |
|                                                             |
\------------------------------------------------------------*/

void XpatDisplayFigure( GraphicX1, GraphicY1, GraphicX2, GraphicY2 )

   long GraphicX1;
   long GraphicY1;
   long GraphicX2;
   long GraphicY2;
{
  xpatview_list *View;
  xpatobj_list  *Obj;
  long           X1;
  long           Y1;
  long           X2;
  long           Y2;
  int            Layer;
  char           DisplayTextIO;
  char           DisplayTextVector;

  autbegin();

  DisplayTextIO     = XPAT_ACTIVE_NAME_TABLE[ XPAT_ACTIVE_NAME_IO     ];
  DisplayTextVector = XPAT_ACTIVE_NAME_TABLE[ XPAT_ACTIVE_NAME_VECTOR ];

  XpatGraphicX1 = GraphicX1;
  XpatGraphicX2 = GraphicX2;
  XpatGraphicY1 = GraphicY1;
  XpatGraphicY2 = GraphicY2;

  if ( XpatUnitGridStep > ( 22.0 / XPAT_PATTERN_STEP_Y ) )
  {
    XpatTextGC       = XpatLargeTextGC;
    XpatTextFont     = XpatLargeTextFont;
  }
  else
  if ( XpatUnitGridStep > ( 18.0 / XPAT_PATTERN_STEP_Y ) )
  {
    XpatTextGC       = XpatMediumTextGC;
    XpatTextFont     = XpatMediumTextFont;
  }
  else
  if ( ( XpatUnitGridStep > ( 10.0 / XPAT_PATTERN_STEP_Y ) ) ||
       ( XPAT_FORCE_DISPLAY                               ) )
  {
    XpatTextGC       = XpatSmallTextGC;
    XpatTextFont     = XpatSmallTextFont;
  }
  else
  {
    DisplayTextIO     = 0;
    DisplayTextVector = 0;
  }

  XpatClearGraphicWindow( GraphicX1, 
                          XpatGraphicDy - GraphicY2,
                          GraphicX2 - GraphicX1,
                          GraphicY2 - GraphicY1 );

  if ( XpatFigure == (xpatfig_list *)NULL ) return;
  
  X1 = GraphicX1 + XpatPixelGridX;
  X2 = GraphicX2 + XpatPixelGridX;
  Y1 = GraphicY1 + XpatPixelGridY;
  Y2 = GraphicY2 + XpatPixelGridY;

  X1 = ( X1 / XpatUnitGridStep );
  Y1 = ( Y1 / XpatUnitGridStep );
  X2 = ( X2 / XpatUnitGridStep );
  Y2 = ( Y2 / XpatUnitGridStep );

  if ( X2 >= 0 ) { X2 = X2 + 1; }
  if ( Y2 >= 0 ) { Y2 = Y2 + 1; }
  if ( X1 <= 0 ) { X1 = X1 - 1; }
  if ( Y1 <= 0 ) { Y1 = Y1 - 1; }

  X1 = X1 * XPAT_UNIT;
  X2 = X2 * XPAT_UNIT;
  Y1 = Y1 * XPAT_UNIT;
  Y2 = Y2 * XPAT_UNIT;

  XpatInterrupt = 0;

  signal( SIGALRM, XpatInterruptDisplay );
  alarm( 1 );

  for ( Layer = 0; Layer < XPAT_MAX_LAYER; Layer++ )
  {
    if ( XpatInterrupt ) break;

    if ( XPAT_ACTIVE_LAYER_TABLE[ Layer ] == 0 ) continue;

    XpatDrawGC = XpatLayerDrawGC[ Layer ];

    for ( Obj  = XpatFigure->OBJECT[ Layer ];
          Obj != (xpatobj_list *)NULL;
          Obj  = Obj->NEXT )
    {
      if ( XpatInterrupt ) break;

      if ( XPAT_ACTIVE_LAYER_TABLE[ Obj->LAYER ] == 0 ) continue;

      if ( IsXpatDelete( Obj ) ) continue;
  
      if ( ( Obj->X           <= X2 ) &&
           ( Obj->Y           <= Y2 ) &&
           ( Obj->X + Obj->DX >= X1 ) && 
           ( Obj->Y + Obj->DY >= Y1 ) )
      {
        if ( ( IsXpatAccept( Obj  ) ) ||
             ( IsXpatConnect( Obj ) ) ||
             ( IsXpatCursor( Obj  ) ) )
        {
          XpatAddViewLater( Obj );
        }
        else
        {
          XpatDisplayOneObject( Obj );

          if ( Obj->NAME != (char *)0 )
          {
            if ( ( IsXpatIO( Obj )     && DisplayTextIO     ) ||
                 ( IsXpatVector( Obj ) && DisplayTextVector ) )
            {
              XpatDisplayOneString( Obj );
            }
          }
        }
      }
    }
  }

  for ( View  = XpatHeadView;
        View != (xpatview_list *)NULL;
        View  = View->NEXT )
  {
    Obj = View->OBJECT;

    if ( IsXpatAccept( Obj ) )
    {
      XpatDrawGC = XpatAcceptDrawGC;
    } 
    else
    if ( IsXpatConnect( Obj ) )
    {
      XpatDrawGC = XpatConnectDrawGC;
    }
    else
    {
      XpatDrawGC = XpatLayerDrawGC[ Obj->LAYER ];
    }
    
    XpatDisplayOneObject( Obj );

    if ( Obj->NAME != (char *)0 )
    {
      if ( ( IsXpatIO( Obj )     && DisplayTextIO     ) ||
           ( IsXpatVector( Obj ) && DisplayTextVector ) ||
           ( IsXpatCursor( Obj )                      ) )
      {
        XpatDisplayOneString( Obj );
      }
    }
  }

  XpatDelView();

  alarm( 0 );
  XpatFlushEventDisplay();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XpatDisplayObject                       |
|                                                             |
\------------------------------------------------------------*/

void XpatDisplayObject( Obj )

   xpatobj_list *Obj;
{
  xpatobj_list *ScanObj;
  long          Xmin;
  long          Ymin;
  long          Xmax;
  long          Ymax;

  autbegin();

  Xmin = Obj->X;
  Ymin = Obj->Y;
  Xmax = Xmin + Obj->DX;
  Ymax = Ymin + Obj->DY;

  ScanObj = Obj->LINK;

  while ( ScanObj != Obj )
  {
    if ( Xmin > ScanObj->X )
    {
      Xmin = ScanObj->X;
    }

    if ( Xmax < ( ScanObj->X + ScanObj->DX ) )
    {
      Xmax = ScanObj->X + ScanObj->DX;
    }

    if ( Ymin > ScanObj->Y )
    { 
      Ymin = ScanObj->Y;
    }

    if ( Ymax < ( ScanObj->Y + ScanObj->DY ) )
    { 
      Ymax = ScanObj->Y + ScanObj->DY;
    }

    ScanObj = ScanObj->LINK;
  }

  Xmin = (float)( Xmin - XPAT_UNIT ) * XpatUnitGridStep;
  Ymin = (float)( Ymin - XPAT_UNIT ) * XpatUnitGridStep;
  Xmax = (float)( Xmax + XPAT_UNIT ) * XpatUnitGridStep;
  Ymax = (float)( Ymax + XPAT_UNIT ) * XpatUnitGridStep;

  Xmin = ( Xmin / XPAT_UNIT ) - XpatPixelGridX - 1;
  Xmax = ( Xmax / XPAT_UNIT ) - XpatPixelGridX + 1;
  Ymin = ( Ymin / XPAT_UNIT ) - XpatPixelGridY - 1;
  Ymax = ( Ymax / XPAT_UNIT ) - XpatPixelGridY + 1;

  if ( Xmin < 0 ) Xmin = 0;
  if ( Ymin < 0 ) Ymin = 0;

  if ( Xmax > XpatGraphicDx ) Xmax = XpatGraphicDx;
  if ( Ymax > XpatGraphicDy ) Ymax = XpatGraphicDy;

  if ( ( Xmax > 0              ) &&
       ( Ymax > 0              ) &&
       ( Xmin < XpatGraphicDx ) &&
       ( Ymin < XpatGraphicDy ) )
  {
    XpatDisplayFigure( Xmin, Ymin , Xmax, Ymax );

    XpatRefreshGraphicWindow( Xmin, XpatGraphicDy - Ymax, 
                              Xmax - Xmin, Ymax - Ymin );
  }

  autend();
}
