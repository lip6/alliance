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
| Tool    :                   XFSM                            |
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
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "XSB.h"
# include "XFS.h"
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

    static long         XfsmGraphicX1;
    static long         XfsmGraphicX2;
    static long         XfsmGraphicY1;
    static long         XfsmGraphicY2;
    static XFontStruct *XfsmTextFont;
    static GC           XfsmTextGC;
    static GC           XfsmDrawGC;
    static char         XfsmInterrupt = 0;

    static xfsmview_list *XfsmHeadView = (xfsmview_list *)NULL;

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
|                         XfsmAllocView                       |
|                                                             |
\------------------------------------------------------------*/

xfsmview_list *XfsmAllocView()
{
  return( (xfsmview_list *)autallocheap( sizeof( xfsmview_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Free Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XfsmFreeView                       |
|                                                             |
\------------------------------------------------------------*/

void XfsmFreeView( FreeView )

   xfsmview_list *FreeView;
{
  autfreeheap( (char *)FreeView, sizeof( xfsmview_list ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        XfsmAddViewLater                     |
|                                                             |
\------------------------------------------------------------*/

void XfsmAddViewLater( Obj )

   xfsmobj_list *Obj;
{
  xfsmview_list *View;

  autbegin();

  View = XfsmAllocView();

  View->OBJECT  = Obj;
  View->NEXT    = XfsmHeadView;
  XfsmHeadView  = View;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XfsmDelView                          |
|                                                             |
\------------------------------------------------------------*/

void XfsmDelView()
{
  xfsmview_list *DelView;
  xfsmview_list *View;

  autbegin();

  View          = XfsmHeadView;
  XfsmHeadView = (xfsmview_list *)NULL;

  while( View != (xfsmview_list *)NULL )
  {
    DelView  = View;
    View     = View->NEXT;

    XfsmFreeView( DelView );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XfsmInterruptDisplay                    |
|                                                             |
\------------------------------------------------------------*/

void XfsmInterruptDisplay()
{
# ifdef XFSM_CTRL_C
  XEvent       Event;
  KeySym       Key;
  char         Text;
#endif

  autbegin();

# ifdef XFSM_CTRL_C
  if ( XCheckTypedEvent( XfsmGraphicDisplay, KeyPress, &Event ) )
  {
    XLookupString( &Event.xkey, &Text, 1, &Key, 0 );

    if ( Text == '\003' ) 
    {
      XfsmInterrupt = 1;

      XBell( XfsmGraphicDisplay, 0 );
    }
  }
# endif

  signal( SIGALRM, XfsmInterruptDisplay );
  alarm( 1 );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XfsmFlushEventDisplay                   |
|                                                             |
\------------------------------------------------------------*/

void XfsmFlushEventDisplay()
{
# ifdef XFSM_CTRL_C
  XEvent Event;
#endif
  
  autbegin();

# ifdef XFSM_CTRL_C
  while ( XCheckTypedEvent( XfsmGraphicDisplay, KeyPress, &Event ) );
# endif

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XfsmDisplayArrow                      |
|                                                             |
\------------------------------------------------------------*/

char XfsmComputeArrow( X1, Y1, X2, Y2, Points )

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
    Norm    = Norm  / XfsmUnitGridStep;
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

    return( XFSM_TRUE );
  }

  return( XFSM_FALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                     XfsmDisplayOneObject                    |
|                                                             |
\------------------------------------------------------------*/

void XfsmDisplayOneObject( Obj )

   xfsmobj_list *Obj;
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

  if ( IsXfsmLineLeft( Obj ) )
  {
    X1r = Obj->X + Obj->DX;
    X2r = Obj->X;
  }
  else
  {
    X1r = Obj->X;
    X2r = Obj->X + Obj->DX;
  }

  if ( IsXfsmLineDown( Obj ) )
  {
    Y1r  = Obj->Y + Obj->DY;
    Y2r  = Obj->Y;
  }
  else
  {
    Y1r  = Obj->Y;
    Y2r  = Obj->Y + Obj->DY;
  }

  X1r  = (float)( X1r ) * XfsmUnitGridStep;
  Y1r  = (float)( Y1r ) * XfsmUnitGridStep;
  X2r  = (float)( X2r ) * XfsmUnitGridStep;
  Y2r  = (float)( Y2r ) * XfsmUnitGridStep;

  X1r  = ( X1r / XFSM_UNIT ) - XfsmPixelGridX;
  X2r  = ( X2r / XFSM_UNIT ) - XfsmPixelGridX;
  Y1r  = ( Y1r / XFSM_UNIT ) - XfsmPixelGridY;
  Y2r  = ( Y2r / XFSM_UNIT ) - XfsmPixelGridY;

  DeltaX = X2r - X1r;
  DeltaY = Y2r - Y1r;

  if ( DeltaX <= 0 ) DeltaX = 1;
  if ( DeltaY <= 0 ) DeltaY = 1;

  switch ( GetXfsmObjectType( Obj ) )
  {
    case XFSM_OBJECT_LINE :

      XDrawLine( XfsmGraphicDisplay,
                 XfsmGraphicPixmap,
                 XfsmDrawGC,
                 X1r, XfsmGraphicDy - Y1r,
                 X2r, XfsmGraphicDy - Y2r );
    break;

    case XFSM_OBJECT_ARROW :

      XDrawLine( XfsmGraphicDisplay,
                 XfsmGraphicPixmap,
                 XfsmDrawGC,
                 X1r, XfsmGraphicDy - Y1r,
                 X2r, XfsmGraphicDy - Y2r );

      if ( XfsmComputeArrow( X1r, Y1r, X2r, Y2r, Arrow ) )
      {
        Arrow[ 0 ].y = XfsmGraphicDy - Arrow[ 0 ].y;
        Arrow[ 1 ].y = XfsmGraphicDy - Arrow[ 1 ].y;
        Arrow[ 2 ].y = XfsmGraphicDy - Arrow[ 2 ].y;

        XFillPolygon( XfsmGraphicDisplay,
                      XfsmGraphicPixmap, 
                      XfsmDrawGC, Arrow, 3, 
                      Convex,
                      CoordModeOrigin );
      }

    break;

    case XFSM_OBJECT_CIRCLE :

     XFillArc( XfsmGraphicDisplay,
               XfsmGraphicPixmap,
               XfsmDrawGC,
               X1r, XfsmGraphicDy - Y2r, 
               DeltaX, DeltaY, 0, 23040 );
    break;

    case XFSM_OBJECT_RECTANGLE :

      XFillRectangle( XfsmGraphicDisplay,
                      XfsmGraphicPixmap,
                      XfsmDrawGC,
                      X1r, XfsmGraphicDy - Y2r,
                      DeltaX, DeltaY );

    break;

    case XFSM_OBJECT_TEXT :

      Length     = strlen( Obj->NAME );
      HeightText = XfsmTextFont->ascent;

      if ( IsXfsmTrans( Obj ) )
      {
        XDrawString( XfsmGraphicDisplay,
                     XfsmGraphicPixmap,
                     XfsmTextGC,
                     X1r,
                     XfsmGraphicDy - (( Y1r + Y2r - HeightText ) >> 1),
                     Obj->NAME, Length );
      }
      else
      {
        WidthText  = XTextWidth( XfsmTextFont, Obj->NAME, Length );

        XDrawString( XfsmGraphicDisplay,
                     XfsmGraphicPixmap,
                     XfsmTextGC,
                     (( X1r + X2r - WidthText ) >> 1),
                     XfsmGraphicDy - (( Y1r + Y2r - HeightText ) >> 1),
                     Obj->NAME, Length );
      }

    break;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     XfsmDisplayFigure                       |
|                                                             |
\------------------------------------------------------------*/

void XfsmDisplayFigure( GraphicX1, GraphicY1, GraphicX2, GraphicY2 )

   long GraphicX1;
   long GraphicY1;
   long GraphicX2;
   long GraphicY2;
{
  xfsmview_list *View;
  xfsmobj_list  *Obj;
  long           X1;
  long           Y1;
  long           X2;
  long           Y2;
  int            Layer;
  char           DisplayTextTrans;
  char           DisplayTextState;

  autbegin();

  DisplayTextTrans = XFSM_ACTIVE_NAME_TABLE[ XFSM_ACTIVE_NAME_TRANS ];
  DisplayTextState = XFSM_ACTIVE_NAME_TABLE[ XFSM_ACTIVE_NAME_STATE ];

  XfsmGraphicX1 = GraphicX1;
  XfsmGraphicX2 = GraphicX2;
  XfsmGraphicY1 = GraphicY1;
  XfsmGraphicY2 = GraphicY2;

  if ( XfsmUnitGridStep > 10.0 )
  {
    XfsmTextGC       = XfsmLargeTextGC;
    XfsmTextFont     = XfsmLargeTextFont;
  }
  else
  if ( XfsmUnitGridStep > 6.0 )
  {
    XfsmTextGC       = XfsmMediumTextGC;
    XfsmTextFont     = XfsmMediumTextFont;
  }
  else
  if ( ( XfsmUnitGridStep > 3.0 ) || 
       ( XFSM_FORCE_DISPLAY     ) )
  {
    XfsmTextGC       = XfsmSmallTextGC;
    XfsmTextFont     = XfsmSmallTextFont;
  }
  else
  {
    DisplayTextTrans = 0;
    DisplayTextState = 0;
  }

  XfsmClearGraphicWindow( GraphicX1, 
                           XfsmGraphicDy - GraphicY2,
                           GraphicX2 - GraphicX1,
                           GraphicY2 - GraphicY1 );

  if ( XfsmFigure == (xfsmfig_list *)NULL ) return;
  
  X1 = GraphicX1 + XfsmPixelGridX;
  X2 = GraphicX2 + XfsmPixelGridX;
  Y1 = GraphicY1 + XfsmPixelGridY;
  Y2 = GraphicY2 + XfsmPixelGridY;

  X1 = ( X1 / XfsmUnitGridStep );
  Y1 = ( Y1 / XfsmUnitGridStep );
  X2 = ( X2 / XfsmUnitGridStep );
  Y2 = ( Y2 / XfsmUnitGridStep );

  if ( X2 >= 0 ) { X2 = X2 + 1; }
  if ( Y2 >= 0 ) { Y2 = Y2 + 1; }
  if ( X1 <= 0 ) { X1 = X1 - 1; }
  if ( Y1 <= 0 ) { Y1 = Y1 - 1; }

  X1 = X1 * XFSM_UNIT;
  X2 = X2 * XFSM_UNIT;
  Y1 = Y1 * XFSM_UNIT;
  Y2 = Y2 * XFSM_UNIT;

  XfsmInterrupt = 0;

  signal( SIGALRM, XfsmInterruptDisplay );
  alarm( 1 );

  for ( Layer = 0; Layer < XFSM_MAX_LAYER; Layer++ )
  {
    if ( XfsmInterrupt ) break;

    if ( XFSM_ACTIVE_LAYER_TABLE[ Layer ] == 0 ) continue;

    XfsmDrawGC = XfsmLayerDrawGC[ Layer ];

    for ( Obj  = XfsmFigure->OBJECT[ Layer ];
          Obj != (xfsmobj_list *)NULL;
          Obj  = Obj->NEXT )
    {
      if ( XfsmInterrupt ) break;

      if ( XFSM_ACTIVE_LAYER_TABLE[ Obj->LAYER ] == 0 ) continue;
  
      if ( ( Obj->X           <= X2 ) &&
           ( Obj->Y           <= Y2 ) &&
           ( Obj->X + Obj->DX >= X1 ) && 
           ( Obj->Y + Obj->DY >= Y1 ) )
      {
        if ( ( IsXfsmAccept( Obj  ) ) ||
             ( IsXfsmConnect( Obj ) ) )
        {
          XfsmAddViewLater( Obj );
        }
        else
        {
          XfsmDisplayOneObject( Obj );
        }
      }
    }
  }

  for ( View  = XfsmHeadView;
        View != (xfsmview_list *)NULL;
        View  = View->NEXT )
  {
    Obj = View->OBJECT;

    if ( IsXfsmAccept( Obj ) )
    {
      XfsmDrawGC = XfsmAcceptDrawGC;
    } 
    else
    {
      XfsmDrawGC = XfsmConnectDrawGC;
    } 
    
    XfsmDisplayOneObject( Obj );
  }

  XfsmDelView();

  for ( Layer = 0; Layer < XFSM_MAX_LAYER; Layer++ )
  {
    if ( XfsmInterrupt ) break;

    if ( XFSM_ACTIVE_LAYER_TABLE[ Layer ] == 0 ) continue;

    for ( Obj  = XfsmFigure->TEXT[ Layer ];
          Obj != (xfsmobj_list *)NULL;
          Obj  = Obj->NEXT )
    {
      if ( XfsmInterrupt ) break;

      if ( XFSM_ACTIVE_LAYER_TABLE[ Obj->LAYER ] == 0 ) continue;

      if ( ( Obj->X           <= X2 ) &&
           ( Obj->Y           <= Y2 ) &&
           ( Obj->X + Obj->DX >= X1 ) &&
           ( Obj->Y + Obj->DY >= Y1 ) )
      {
        if ( ( IsXfsmTrans( Obj ) && DisplayTextTrans ) ||
             ( IsXfsmState( Obj ) && DisplayTextState ) )
        {
          XfsmDisplayOneObject( Obj );
        }
      }
    }
  }

  alarm( 0 );
  XfsmFlushEventDisplay();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XfsmDisplayObject                       |
|                                                             |
\------------------------------------------------------------*/

void XfsmDisplayObject( Obj )

   xfsmobj_list *Obj;
{
  xfsmobj_list *ScanObj;
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

  Xmin = (float)( Xmin - XFSM_UNIT ) * XfsmUnitGridStep;
  Ymin = (float)( Ymin - XFSM_UNIT ) * XfsmUnitGridStep;
  Xmax = (float)( Xmax + XFSM_UNIT ) * XfsmUnitGridStep;
  Ymax = (float)( Ymax + XFSM_UNIT ) * XfsmUnitGridStep;

  Xmin = ( Xmin / XFSM_UNIT ) - XfsmPixelGridX - 1;
  Xmax = ( Xmax / XFSM_UNIT ) - XfsmPixelGridX + 1;
  Ymin = ( Ymin / XFSM_UNIT ) - XfsmPixelGridY - 1;
  Ymax = ( Ymax / XFSM_UNIT ) - XfsmPixelGridY + 1;

  if ( Xmin < 0 ) Xmin = 0;
  if ( Ymin < 0 ) Ymin = 0;

  if ( Xmax > XfsmGraphicDx ) Xmax = XfsmGraphicDx;
  if ( Ymax > XfsmGraphicDy ) Ymax = XfsmGraphicDy;

  if ( ( Xmax > 0              ) &&
       ( Ymax > 0              ) &&
       ( Xmin < XfsmGraphicDx ) &&
       ( Ymin < XfsmGraphicDy ) )
  {
    XfsmDisplayFigure( Xmin, Ymin , Xmax, Ymax );

    XfsmRefreshGraphicWindow( Xmin, XfsmGraphicDy - Ymax, 
                              Xmax - Xmin, Ymax - Ymin );
  }

  autend();
}
