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
| Tool    :                   XGRA                            |
|                                                             |
| File    :                   View.c                          |
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
# include <math.h>
# include <signal.h>
# include <Xm/Xm.h>
 
# include "mut.h"
# include "aut.h"
# include "XSB.h"
# include "XGR.h"
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

    static long         XgraGraphicX1;
    static long         XgraGraphicX2;
    static long         XgraGraphicY1;
    static long         XgraGraphicY2;
    static XFontStruct *XgraTextFont;
    static GC           XgraTextGC;
    static GC           XgraDrawGC;
    static char         XgraInterrupt = 0;

    static xgraview_list *XgraHeadView = (xgraview_list *)NULL;

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
|                         XgraAllocView                       |
|                                                             |
\------------------------------------------------------------*/

xgraview_list *XgraAllocView()
{
  return( (xgraview_list *)autallocheap( sizeof( xgraview_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Free Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XgraFreeView                       |
|                                                             |
\------------------------------------------------------------*/

void XgraFreeView( FreeView )

   xgraview_list *FreeView;
{
  autfreeheap( (char *)FreeView, sizeof( xgraview_list ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        XgraAddViewLater                     |
|                                                             |
\------------------------------------------------------------*/

void XgraAddViewLater( Obj )

   xgraobj_list *Obj;
{
  xgraview_list *View;

  autbegin();

  View = XgraAllocView();

  View->OBJECT  = Obj;
  View->NEXT    = XgraHeadView;
  XgraHeadView  = View;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XgraDelView                          |
|                                                             |
\------------------------------------------------------------*/

void XgraDelView()
{
  xgraview_list *DelView;
  xgraview_list *View;

  autbegin();

  View          = XgraHeadView;
  XgraHeadView = (xgraview_list *)NULL;

  while( View != (xgraview_list *)NULL )
  {
    DelView  = View;
    View     = View->NEXT;

    XgraFreeView( DelView );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XgraInterruptDisplay                    |
|                                                             |
\------------------------------------------------------------*/

void XgraInterruptDisplay()
{
  XEvent       Event;
  KeySym       Key;
  char         Text;

  autbegin();

# ifdef XGRA_CTRL_C
  if ( XCheckTypedEvent( XgraGraphicDisplay, KeyPress, &Event ) )
  {
    XLookupString( &Event.xkey, &Text, 1, &Key, 0 );

    if ( Text == '\003' ) 
    {
      XgraInterrupt = 1;

      XBell( XgraGraphicDisplay, 0 );
    }
  }
# endif

  signal( SIGALRM, XgraInterruptDisplay );
  alarm( 1 );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XgraFlushEventDisplay                   |
|                                                             |
\------------------------------------------------------------*/

void XgraFlushEventDisplay()
{
  XEvent Event;
  
  autbegin();

# ifdef XGRA_CTRL_C
  while ( XCheckTypedEvent( XgraGraphicDisplay, KeyPress, &Event ) );
# endif

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XgraDisplayArrow                      |
|                                                             |
\------------------------------------------------------------*/

char XgraComputeArrow( X1, Y1, X2, Y2, Points )

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

  if ( ( LineX != 0.0 ) ||
       ( LineY != 0.0 ) )
  {
    Norm    = sqrt( ( LineX * LineX ) + ( LineY * LineY ) );
    Norm    = Norm  / XgraUnitGridStep;
    LineX   = LineX / Norm;
    LineY   = LineY / Norm;
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

    return( XGRA_TRUE );
  }

  return( XGRA_FALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                     XgraDisplayOneObject                    |
|                                                             |
\------------------------------------------------------------*/

void XgraDisplayOneObject( Obj )

   xgraobj_list *Obj;
{
  XPoint  Arrow[ 3 ];
  long    X1r;
  long    X2r;
  long    Y1r;
  long    Y2r;
  long    WidthText;
  long    HeightText;
  int     Length;

  if ( IsXgraLineLeft( Obj ) )
  {
    X1r = Obj->X + Obj->DX;
    X2r = Obj->X;
  }
  else
  {
    X1r = Obj->X;
    X2r = Obj->X + Obj->DX;
  }

  if ( IsXgraLineDown( Obj ) )
  {
    Y1r  = Obj->Y + Obj->DY;
    Y2r  = Obj->Y;
  }
  else
  {
    Y1r  = Obj->Y;
    Y2r  = Obj->Y + Obj->DY;
  }

  X1r  = (float)( X1r ) * XgraUnitGridStep;
  Y1r  = (float)( Y1r ) * XgraUnitGridStep;
  X2r  = (float)( X2r ) * XgraUnitGridStep;
  Y2r  = (float)( Y2r ) * XgraUnitGridStep;

  X1r  = ( X1r / XGRA_UNIT ) - XgraPixelGridX;
  X2r  = ( X2r / XGRA_UNIT ) - XgraPixelGridX;
  Y1r  = ( Y1r / XGRA_UNIT ) - XgraPixelGridY;
  Y2r  = ( Y2r / XGRA_UNIT ) - XgraPixelGridY;

  switch ( GetXgraObjectType( Obj ) )
  {
    case XGRA_OBJECT_LINE :

      XDrawLine( XgraGraphicDisplay,
                 XgraGraphicPixmap,
                 XgraDrawGC,
                 X1r, XgraGraphicDy - Y1r,
                 X2r, XgraGraphicDy - Y2r );
    break;

    case XGRA_OBJECT_ARROW :

      XDrawLine( XgraGraphicDisplay,
                 XgraGraphicPixmap,
                 XgraDrawGC,
                 X1r, XgraGraphicDy - Y1r,
                 X2r, XgraGraphicDy - Y2r );

      if ( XgraComputeArrow( X1r, Y1r, X2r, Y2r, Arrow ) )
      {
        Arrow[ 0 ].y = XgraGraphicDy - Arrow[ 0 ].y;
        Arrow[ 1 ].y = XgraGraphicDy - Arrow[ 1 ].y;
        Arrow[ 2 ].y = XgraGraphicDy - Arrow[ 2 ].y;

        XFillPolygon( XgraGraphicDisplay,
                      XgraGraphicPixmap, 
                      XgraDrawGC, Arrow, 3, 
                      Convex,
                      CoordModeOrigin );
      }

    break;

    case XGRA_OBJECT_CIRCLE :

     XFillArc( XgraGraphicDisplay,
               XgraGraphicPixmap,
               XgraDrawGC,
               X1r, XgraGraphicDy - Y2r, 
               X2r - X1r,
               Y2r - Y1r, 0, 23040 );
    break;

    case XGRA_OBJECT_RECTANGLE :

      XFillRectangle( XgraGraphicDisplay,
                      XgraGraphicPixmap,
                      XgraDrawGC,
                      X1r, XgraGraphicDy - Y2r,
                      X2r - X1r,
                      Y2r - Y1r );

    break;

    case XGRA_OBJECT_TEXT :

      Length     = strlen( Obj->NAME );
      HeightText = XgraTextFont->ascent;

      if ( IsXgraArc( Obj ) )
      {
        XDrawString( XgraGraphicDisplay,
                     XgraGraphicPixmap,
                     XgraTextGC,
                     X1r,
                     XgraGraphicDy - (( Y1r + Y2r - HeightText ) >> 1),
                     Obj->NAME, Length );
      }
      else
      {
        WidthText  = XTextWidth( XgraTextFont, Obj->NAME, Length );

        XDrawString( XgraGraphicDisplay,
                     XgraGraphicPixmap,
                     XgraTextGC,
                     (( X1r + X2r - WidthText ) >> 1),
                     XgraGraphicDy - (( Y1r + Y2r - HeightText ) >> 1),
                     Obj->NAME, Length );
      }

    break;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     XgraDisplayFigure                       |
|                                                             |
\------------------------------------------------------------*/

void XgraDisplayFigure( GraphicX1, GraphicY1, GraphicX2, GraphicY2 )

   long GraphicX1;
   long GraphicY1;
   long GraphicX2;
   long GraphicY2;
{
  xgraview_list *View;
  xgraobj_list  *Obj;
  long           X1;
  long           Y1;
  long           X2;
  long           Y2;
  int            Layer;
  char           DisplayTextArc;
  char           DisplayTextNode;

  autbegin();

  DisplayTextArc = XGRA_ACTIVE_NAME_TABLE[ XGRA_ACTIVE_NAME_ARC ];
  DisplayTextNode = XGRA_ACTIVE_NAME_TABLE[ XGRA_ACTIVE_NAME_NODE ];

  XgraGraphicX1 = GraphicX1;
  XgraGraphicX2 = GraphicX2;
  XgraGraphicY1 = GraphicY1;
  XgraGraphicY2 = GraphicY2;

  if ( XgraUnitGridStep > 10.0 )
  {
    XgraTextGC       = XgraLargeTextGC;
    XgraTextFont     = XgraLargeTextFont;
  }
  else
  if ( XgraUnitGridStep > 8.0 )
  {
    XgraTextGC       = XgraMediumTextGC;
    XgraTextFont     = XgraMediumTextFont;
  }
  else
  if ( XgraUnitGridStep > 4.0 )
  {
    XgraTextGC       = XgraSmallTextGC;
    XgraTextFont     = XgraSmallTextFont;
  }
  else
  {
    DisplayTextArc = 0;
    DisplayTextNode = 0;
  }

  XgraClearGraphicWindow( GraphicX1, 
                           XgraGraphicDy - GraphicY2,
                           GraphicX2 - GraphicX1,
                           GraphicY2 - GraphicY1 );

  if ( XgraFigure == (xautgraph *)NULL ) return;
  
  X1 = GraphicX1 + XgraPixelGridX;
  X2 = GraphicX2 + XgraPixelGridX;
  Y1 = GraphicY1 + XgraPixelGridY;
  Y2 = GraphicY2 + XgraPixelGridY;

  X1 = ( X1 / XgraUnitGridStep );
  Y1 = ( Y1 / XgraUnitGridStep );
  X2 = ( X2 / XgraUnitGridStep );
  Y2 = ( Y2 / XgraUnitGridStep );

  if ( X2 >= 0 ) { X2 = X2 + 1; }
  if ( Y2 >= 0 ) { Y2 = Y2 + 1; }
  if ( X1 <= 0 ) { X1 = X1 - 1; }
  if ( Y1 <= 0 ) { Y1 = Y1 - 1; }

  X1 = X1 * XGRA_UNIT;
  X2 = X2 * XGRA_UNIT;
  Y1 = Y1 * XGRA_UNIT;
  Y2 = Y2 * XGRA_UNIT;

  XgraInterrupt = 0;

  signal( SIGALRM, XgraInterruptDisplay );
  alarm( 1 );

  for ( Layer = 0; Layer < XGRA_MAX_LAYER; Layer++ )
  {
    if ( XgraInterrupt ) break;

    if ( XGRA_ACTIVE_LAYER_TABLE[ Layer ] == 0 ) continue;

    XgraDrawGC = XgraLayerDrawGC[ Layer ];

    for ( Obj  = XgraFigure->OBJECT[ Layer ];
          Obj != (xgraobj_list *)NULL;
          Obj  = Obj->NEXT )
    {
      if ( XgraInterrupt ) break;

      if ( XGRA_ACTIVE_LAYER_TABLE[ Obj->LAYER ] == 0 ) continue;
  
      if ( ( Obj->X           <= X2 ) &&
           ( Obj->Y           <= Y2 ) &&
           ( Obj->X + Obj->DX >= X1 ) && 
           ( Obj->Y + Obj->DY >= Y1 ) )
      {
        if ( ( IsXgraAccept( Obj  ) ) ||
             ( IsXgraConnect( Obj ) ) )
        {
          XgraAddViewLater( Obj );
        }
        else
        {
          XgraDisplayOneObject( Obj );
        }
      }
    }
  }

  for ( View  = XgraHeadView;
        View != (xgraview_list *)NULL;
        View  = View->NEXT )
  {
    Obj = View->OBJECT;

    if ( IsXgraAccept( Obj ) )
    {
      XgraDrawGC = XgraAcceptDrawGC;
    } 
    else
    {
      XgraDrawGC = XgraConnectDrawGC;
    } 
    
    XgraDisplayOneObject( Obj );
  }

  XgraDelView();

  for ( Layer = 0; Layer < XGRA_MAX_LAYER; Layer++ )
  {
    if ( XgraInterrupt ) break;

    if ( XGRA_ACTIVE_LAYER_TABLE[ Layer ] == 0 ) continue;

    for ( Obj  = XgraFigure->TEXT[ Layer ];
          Obj != (xgraobj_list *)NULL;
          Obj  = Obj->NEXT )
    {
      if ( XgraInterrupt ) break;

      if ( XGRA_ACTIVE_LAYER_TABLE[ Obj->LAYER ] == 0 ) continue;

      if ( ( Obj->X           <= X2 ) &&
           ( Obj->Y           <= Y2 ) &&
           ( Obj->X + Obj->DX >= X1 ) &&
           ( Obj->Y + Obj->DY >= Y1 ) )
      {
        if ( ( IsXgraArc( Obj ) && DisplayTextArc ) ||
             ( IsXgraNode( Obj ) && DisplayTextNode ) )
        {
          XgraDisplayOneObject( Obj );
        }
      }
    }
  }

  alarm( 0 );
  XgraFlushEventDisplay();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XgraDisplayObject                       |
|                                                             |
\------------------------------------------------------------*/

void XgraDisplayObject( Obj )

   xgraobj_list *Obj;
{
  xgraobj_list *ScanObj;
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

  Xmin = (float)( Xmin - XGRA_UNIT ) * XgraUnitGridStep;
  Ymin = (float)( Ymin - XGRA_UNIT ) * XgraUnitGridStep;
  Xmax = (float)( Xmax + XGRA_UNIT ) * XgraUnitGridStep;
  Ymax = (float)( Ymax + XGRA_UNIT ) * XgraUnitGridStep;

  Xmin = ( Xmin / XGRA_UNIT ) - XgraPixelGridX - 1;
  Xmax = ( Xmax / XGRA_UNIT ) - XgraPixelGridX + 1;
  Ymin = ( Ymin / XGRA_UNIT ) - XgraPixelGridY - 1;
  Ymax = ( Ymax / XGRA_UNIT ) - XgraPixelGridY + 1;

  if ( Xmin < 0 ) Xmin = 0;
  if ( Ymin < 0 ) Ymin = 0;

  if ( Xmax > XgraGraphicDx ) Xmax = XgraGraphicDx;
  if ( Ymax > XgraGraphicDy ) Ymax = XgraGraphicDy;

  if ( ( Xmax > 0              ) &&
       ( Ymax > 0              ) &&
       ( Xmin < XgraGraphicDx ) &&
       ( Ymin < XgraGraphicDy ) )
  {
    XgraDisplayFigure( Xmin, Ymin , Xmax, Ymax );

    XgraRefreshGraphicWindow( Xmin, XgraGraphicDy - Ymax, 
                              Xmax - Xmin, Ymax - Ymin );
  }

  autend();
}
