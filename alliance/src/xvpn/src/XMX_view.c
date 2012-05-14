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
| Tool    :                   XVPN                            |
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
# include <unistd.h>
# include <Xm/Xm.h>
 
# include "mut.h"
# include "aut.h"
# include "vex.h"
# include "vpn.h"
# include "XSB.h"
# include "XVP.h"
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

    static long         XvpnGraphicX1;
    static long         XvpnGraphicX2;
    static long         XvpnGraphicY1;
    static long         XvpnGraphicY2;
    static XFontStruct *XvpnTextFont;
    static GC           XvpnTextGC;
    static GC           XvpnDrawGC;
    static char         XvpnInterrupt = 0;
    static char         XvpnCheckInt  = 0;

    static xvpnview_list *XvpnHeadView = (xvpnview_list *)NULL;

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
|                         XvpnAllocView                       |
|                                                             |
\------------------------------------------------------------*/

xvpnview_list *XvpnAllocView()
{
  return( (xvpnview_list *)autallocheap( sizeof( xvpnview_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Free Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XvpnFreeView                       |
|                                                             |
\------------------------------------------------------------*/

void XvpnFreeView( FreeView )

   xvpnview_list *FreeView;
{
  autfreeheap( (char *)FreeView, sizeof( xvpnview_list ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        XvpnAddViewLater                     |
|                                                             |
\------------------------------------------------------------*/

void XvpnAddViewLater( Obj )

   xvpnobj_list *Obj;
{
  xvpnview_list *View;

  autbegin();

  View = XvpnAllocView();

  View->OBJECT  = Obj;
  View->NEXT    = XvpnHeadView;
  XvpnHeadView  = View;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XvpnDelView                          |
|                                                             |
\------------------------------------------------------------*/

void XvpnDelView()
{
  xvpnview_list *DelView;
  xvpnview_list *View;

  autbegin();

  View          = XvpnHeadView;
  XvpnHeadView = (xvpnview_list *)NULL;

  while( View != (xvpnview_list *)NULL )
  {
    DelView  = View;
    View     = View->NEXT;

    XvpnFreeView( DelView );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XvpnCheckInterrupt                      |
|                                                             |
\------------------------------------------------------------*/

char XvpnCheckInterrupt()
{
  XEvent       Event;
  KeySym       Key;
  char         Text;

  autbegin();

  if ( XvpnCheckInt )
  {
    if ( XCheckTypedEvent( XvpnGraphicDisplay, KeyPress, &Event ) )
    {
      XLookupString( &Event.xkey, &Text, 1, &Key, 0 );

      if ( Text == '\003' ) 
      {
        XvpnInterrupt = 1;

        XBell( XvpnGraphicDisplay, 0 );
      }
    }
  }

  XvpnCheckInt = 0;

  autend();

  return( XvpnInterrupt );
}

/*------------------------------------------------------------\
|                                                             |
|                     XvpnInterruptDisplay                    |
|                                                             |
\------------------------------------------------------------*/

void XvpnInterruptDisplay()
{
  autbegin();

  XvpnCheckInt = 1;

  signal( SIGALRM, XvpnInterruptDisplay );
  alarm( 1 );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XvpnFlushEventDisplay                   |
|                                                             |
\------------------------------------------------------------*/

void XvpnFlushEventDisplay()
{
  XEvent Event;
  
  autbegin();

  while ( XCheckTypedEvent( XvpnGraphicDisplay, KeyPress, &Event ) );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XvpnDisplayArrow                      |
|                                                             |
\------------------------------------------------------------*/

char XvpnComputeArrow( X1, Y1, X2, Y2, Points )

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
    Norm    = Norm  / XvpnUnitGridStep;
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

    return( XVPN_TRUE );
  }

  return( XVPN_FALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                     XvpnDisplayOneObject                    |
|                                                             |
\------------------------------------------------------------*/

void XvpnDisplayOneObject( Obj )

   xvpnobj_list *Obj;
{
  XPoint  Arrow[ 3 ];
  long    X1r;
  long    X2r;
  long    Y1r;
  long    Y2r;
  long    DeltaX;
  long    DeltaY;
  long    WidthText;
  long    HeightText;
  int     Length;

  if ( IsXvpnLineLeft( Obj ) )
  {
    X1r = Obj->X + Obj->DX;
    X2r = Obj->X;
  }
  else
  {
    X1r = Obj->X;
    X2r = Obj->X + Obj->DX;
  }

  if ( IsXvpnLineDown( Obj ) )
  {
    Y1r  = Obj->Y + Obj->DY;
    Y2r  = Obj->Y;
  }
  else
  {
    Y1r  = Obj->Y;
    Y2r  = Obj->Y + Obj->DY;
  }

  X1r  = (float)( X1r ) * XvpnUnitGridStep;
  Y1r  = (float)( Y1r ) * XvpnUnitGridStep;
  X2r  = (float)( X2r ) * XvpnUnitGridStep;
  Y2r  = (float)( Y2r ) * XvpnUnitGridStep;

  X1r  = ( X1r / XVPN_UNIT ) - XvpnPixelGridX;
  X2r  = ( X2r / XVPN_UNIT ) - XvpnPixelGridX;
  Y1r  = ( Y1r / XVPN_UNIT ) - XvpnPixelGridY;
  Y2r  = ( Y2r / XVPN_UNIT ) - XvpnPixelGridY;

  DeltaX = X2r - X1r;
  DeltaY = Y2r - Y1r;

  if ( DeltaX <= 0 ) DeltaX = 1;
  if ( DeltaY <= 0 ) DeltaY = 1;

  switch ( GetXvpnObjectType( Obj ) )
  {
    case XVPN_OBJECT_LINE :

      XDrawLine( XvpnGraphicDisplay,
                 XvpnGraphicPixmap,
                 XvpnDrawGC,
                 X1r, XvpnGraphicDy - Y1r,
                 X2r, XvpnGraphicDy - Y2r );
    break;

    case XVPN_OBJECT_ARROW :

      XDrawLine( XvpnGraphicDisplay,
                 XvpnGraphicPixmap,
                 XvpnDrawGC,
                 X1r, XvpnGraphicDy - Y1r,
                 X2r, XvpnGraphicDy - Y2r );

      if ( XvpnComputeArrow( X1r, Y1r, X2r, Y2r, Arrow ) )
      {
        Arrow[ 0 ].y = XvpnGraphicDy - Arrow[ 0 ].y;
        Arrow[ 1 ].y = XvpnGraphicDy - Arrow[ 1 ].y;
        Arrow[ 2 ].y = XvpnGraphicDy - Arrow[ 2 ].y;

        XFillPolygon( XvpnGraphicDisplay,
                      XvpnGraphicPixmap, 
                      XvpnDrawGC, Arrow, 3, 
                      Convex,
                      CoordModeOrigin );
      }

    break;

    case XVPN_OBJECT_CIRCLE :

     XFillArc( XvpnGraphicDisplay,
               XvpnGraphicPixmap,
               XvpnDrawGC,
               X1r, XvpnGraphicDy - Y2r, 
               DeltaX,
               DeltaY, 0, 23040 );
    break;

    case XVPN_OBJECT_RECTANGLE :

      XFillRectangle( XvpnGraphicDisplay,
                      XvpnGraphicPixmap,
                      XvpnDrawGC,
                      X1r, XvpnGraphicDy - Y2r,
                      DeltaX, DeltaY );

    break;

    case XVPN_OBJECT_TEXT :

      Length     = strlen( Obj->NAME );
      HeightText = XvpnTextFont->ascent;

      if ( IsXvpnTrans( Obj ) )
      {
        XDrawString( XvpnGraphicDisplay,
                     XvpnGraphicPixmap,
                     XvpnTextGC,
                     X1r,
                     XvpnGraphicDy - (( Y1r + Y2r - HeightText ) >> 1),
                     Obj->NAME, Length );
      }
      else
      {
        WidthText  = XTextWidth( XvpnTextFont, Obj->NAME, Length );

        XDrawString( XvpnGraphicDisplay,
                     XvpnGraphicPixmap,
                     XvpnTextGC,
                     (( X1r + X2r - WidthText ) >> 1),
                     XvpnGraphicDy - (( Y1r + Y2r - HeightText ) >> 1),
                     Obj->NAME, Length );
      }

    break;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     XvpnDisplayFigure                       |
|                                                             |
\------------------------------------------------------------*/

void XvpnDisplayFigure( GraphicX1, GraphicY1, GraphicX2, GraphicY2 )

   long GraphicX1;
   long GraphicY1;
   long GraphicX2;
   long GraphicY2;
{
  xvpnview_list *View;
  xvpnobj_list  *Obj;
  long           X1;
  long           Y1;
  long           X2;
  long           Y2;
  int            Layer;
  char           DisplayTextTrans;
  char           DisplayTextPlace;

  autbegin();

  DisplayTextTrans = XVPN_ACTIVE_NAME_TABLE[ XVPN_ACTIVE_NAME_TRANS ];
  DisplayTextPlace = XVPN_ACTIVE_NAME_TABLE[ XVPN_ACTIVE_NAME_PLACE ];

  XvpnGraphicX1 = GraphicX1;
  XvpnGraphicX2 = GraphicX2;
  XvpnGraphicY1 = GraphicY1;
  XvpnGraphicY2 = GraphicY2;

  if ( XvpnUnitGridStep > 15.0 )
  {
    XvpnTextGC       = XvpnLargeTextGC;
    XvpnTextFont     = XvpnLargeTextFont;
  }
  else
  if ( XvpnUnitGridStep > 10.0 )
  {
    XvpnTextGC       = XvpnMediumTextGC;
    XvpnTextFont     = XvpnMediumTextFont;
  }
  else
  if ( ( XvpnUnitGridStep > 6.0 ) ||
       ( XVPN_FORCE_DISPLAY     ) )
  {
    XvpnTextGC       = XvpnSmallTextGC;
    XvpnTextFont     = XvpnSmallTextFont;
  }
  else
  {
    DisplayTextTrans = 0;
    DisplayTextPlace = 0;
  }

  XvpnClearGraphicWindow( GraphicX1, 
                           XvpnGraphicDy - GraphicY2,
                           GraphicX2 - GraphicX1,
                           GraphicY2 - GraphicY1 );

  if ( XvpnFigure == (xvpnfig_list *)NULL ) return;
  
  X1 = GraphicX1 + XvpnPixelGridX;
  X2 = GraphicX2 + XvpnPixelGridX;
  Y1 = GraphicY1 + XvpnPixelGridY;
  Y2 = GraphicY2 + XvpnPixelGridY;

  X1 = ( X1 / XvpnUnitGridStep );
  Y1 = ( Y1 / XvpnUnitGridStep );
  X2 = ( X2 / XvpnUnitGridStep );
  Y2 = ( Y2 / XvpnUnitGridStep );

  if ( X2 >= 0 ) { X2 = X2 + 1; }
  if ( Y2 >= 0 ) { Y2 = Y2 + 1; }
  if ( X1 <= 0 ) { X1 = X1 - 1; }
  if ( Y1 <= 0 ) { Y1 = Y1 - 1; }

  X1 = X1 * XVPN_UNIT;
  X2 = X2 * XVPN_UNIT;
  Y1 = Y1 * XVPN_UNIT;
  Y2 = Y2 * XVPN_UNIT;

  XvpnInterrupt = 0;
  XvpnCheckInt  = 0;

  signal( SIGALRM, XvpnInterruptDisplay );
  alarm( 1 );

  for ( Layer = 0; Layer < XVPN_MAX_LAYER; Layer++ )
  {
    if ( XvpnCheckInterrupt() ) break;

    if ( XVPN_ACTIVE_LAYER_TABLE[ Layer ] == 0 ) continue;

    XvpnDrawGC = XvpnLayerDrawGC[ Layer ];

    for ( Obj  = XvpnFigure->OBJECT[ Layer ];
          Obj != (xvpnobj_list *)NULL;
          Obj  = Obj->NEXT )
    {
      if ( XvpnCheckInterrupt() ) break;

      if ( XVPN_ACTIVE_LAYER_TABLE[ Obj->LAYER ] == 0 ) continue;
  
      if ( ( Obj->X           <= X2 ) &&
           ( Obj->Y           <= Y2 ) &&
           ( Obj->X + Obj->DX >= X1 ) && 
           ( Obj->Y + Obj->DY >= Y1 ) )
      {
        if ( ( IsXvpnAccept( Obj  ) ) ||
             ( IsXvpnConnect( Obj ) ) )
        {
          XvpnAddViewLater( Obj );
        }
        else
        {
          XvpnDisplayOneObject( Obj );
        }
      }
    }
  }

  for ( View  = XvpnHeadView;
        View != (xvpnview_list *)NULL;
        View  = View->NEXT )
  {
    Obj = View->OBJECT;

    if ( IsXvpnAccept( Obj ) )
    {
      XvpnDrawGC = XvpnAcceptDrawGC;
    } 
    else
    {
      XvpnDrawGC = XvpnConnectDrawGC;
    } 
    
    XvpnDisplayOneObject( Obj );
  }

  XvpnDelView();

  for ( Layer = 0; Layer < XVPN_MAX_LAYER; Layer++ )
  {
    if ( XvpnCheckInterrupt() ) break;

    if ( XVPN_ACTIVE_LAYER_TABLE[ Layer ] == 0 ) continue;

    for ( Obj  = XvpnFigure->TEXT[ Layer ];
          Obj != (xvpnobj_list *)NULL;
          Obj  = Obj->NEXT )
    {
      if ( XvpnCheckInterrupt() ) break;

      if ( XVPN_ACTIVE_LAYER_TABLE[ Obj->LAYER ] == 0 ) continue;

      if ( ( Obj->X           <= X2 ) &&
           ( Obj->Y           <= Y2 ) &&
           ( Obj->X + Obj->DX >= X1 ) &&
           ( Obj->Y + Obj->DY >= Y1 ) )
      {
        if ( ( IsXvpnTrans( Obj ) && DisplayTextTrans ) ||
             ( IsXvpnPlace( Obj ) && DisplayTextPlace ) ||
             ( IsXvpnProc( Obj  ) && DisplayTextPlace ) )
        {
          XvpnDisplayOneObject( Obj );
        }
      }
    }
  }

  alarm( 0 );
  XvpnFlushEventDisplay();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XvpnDisplayObject                       |
|                                                             |
\------------------------------------------------------------*/

void XvpnDisplayObject( Obj )

   xvpnobj_list *Obj;
{
  xvpnobj_list *ScanObj;
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

  Xmin = (float)( Xmin - XVPN_UNIT ) * XvpnUnitGridStep;
  Ymin = (float)( Ymin - XVPN_UNIT ) * XvpnUnitGridStep;
  Xmax = (float)( Xmax + XVPN_UNIT ) * XvpnUnitGridStep;
  Ymax = (float)( Ymax + XVPN_UNIT ) * XvpnUnitGridStep;

  Xmin = ( Xmin / XVPN_UNIT ) - XvpnPixelGridX - 1;
  Xmax = ( Xmax / XVPN_UNIT ) - XvpnPixelGridX + 1;
  Ymin = ( Ymin / XVPN_UNIT ) - XvpnPixelGridY - 1;
  Ymax = ( Ymax / XVPN_UNIT ) - XvpnPixelGridY + 1;

  if ( Xmin < 0 ) Xmin = 0;
  if ( Ymin < 0 ) Ymin = 0;

  if ( Xmax > XvpnGraphicDx ) Xmax = XvpnGraphicDx;
  if ( Ymax > XvpnGraphicDy ) Ymax = XvpnGraphicDy;

  if ( ( Xmax > 0              ) &&
       ( Ymax > 0              ) &&
       ( Xmin < XvpnGraphicDx ) &&
       ( Ymin < XvpnGraphicDy ) )
  {
    XvpnDisplayFigure( Xmin, Ymin , Xmax, Ymax );

    XvpnRefreshGraphicWindow( Xmin, XvpnGraphicDy - Ymax, 
                              Xmax - Xmin, Ymax - Ymin );
  }

  autend();
}
