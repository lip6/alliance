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
| Tool    :                   XSCH                            |
|                                                             |
| File    :                   View.c                          |
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
# include <math.h>
# include <signal.h>
# include <Xm/Xm.h>
 
# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "scl.h"
# include "XSB.h"
# include "XSC.h"
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

    static long         XschGraphicX1;
    static long         XschGraphicX2;
    static long         XschGraphicY1;
    static long         XschGraphicY2;
    static XFontStruct *XschTextFont;
    static GC           XschTextGC;
    static GC           XschDrawGC;

    static xschview_list *XschHeadView = (xschview_list *)NULL;

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
|                         XschAllocView                       |
|                                                             |
\------------------------------------------------------------*/

xschview_list *XschAllocView()
{
  return( (xschview_list *)autallocheap( sizeof( xschview_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Free Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XschFreeView                       |
|                                                             |
\------------------------------------------------------------*/

void XschFreeView( FreeView )

   xschview_list *FreeView;
{
  autfreeheap( (char *)FreeView, sizeof( xschview_list ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        XschAddViewLater                     |
|                                                             |
\------------------------------------------------------------*/

void XschAddViewLater( Obj )

   xschobj_list *Obj;
{
  xschview_list *View;

  autbegin();

  View = XschAllocView();

  View->OBJECT  = Obj;
  View->NEXT    = XschHeadView;
  XschHeadView  = View;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                        XschDelView                          |
|                                                             |
\------------------------------------------------------------*/

void XschDelView()
{
  xschview_list *DelView;
  xschview_list *View;

  autbegin();

  View          = XschHeadView;
  XschHeadView = (xschview_list *)NULL;

  while( View != (xschview_list *)NULL )
  {
    DelView  = View;
    View     = View->NEXT;

    XschFreeView( DelView );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XschFlushEventDisplay                   |
|                                                             |
\------------------------------------------------------------*/

void XschFlushEventDisplay()
{
  XEvent Event;
  
  autbegin();

  while ( XCheckTypedEvent( XschGraphicDisplay, KeyPress, &Event ) );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XschComputePentagon                   |
|                                                             |
\------------------------------------------------------------*/

char XschComputePentagon( X1, Y1, X2, Y2, Points )

  long    X1;
  long    Y1;
  long    X2;
  long    Y2;
  XPoint *Points;
{
  long StepX;
  long StepY;

  StepY = ( Y2 - Y1 ) >> 1;
  StepX = StepY >> 1;

  if ( StepY <= 0 ) StepY = 1;
  if ( StepX <= 0 ) StepX = 1;

  Points[ 0 ].x = X1;
  Points[ 0 ].y = Y1;
  Points[ 1 ].x = X1;
  Points[ 1 ].y = Y2;
  Points[ 2 ].x = X1;
  Points[ 2 ].y = Y2;
  Points[ 3 ].x = X2 - StepX;
  Points[ 3 ].y = Y2;
  Points[ 4 ].x = X2;
  Points[ 4 ].y = Y1 + StepY;
  Points[ 5 ].x = X2 - StepX;
  Points[ 5 ].y = Y1;
  Points[ 6 ].x = X1;
  Points[ 6 ].y = Y1;

  return( XSCH_TRUE );
}

/*------------------------------------------------------------\
|                                                             |
|                       XschDisplayArrow                      |
|                                                             |
\------------------------------------------------------------*/

char XschComputeArrow( X1, Y1, X2, Y2, Points )

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
    Norm    = Norm  / XschUnitGridStep;
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

    return( XSCH_TRUE );
  }

  return( XSCH_FALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                       XschDrawTristate                      |
|                                                             |
\------------------------------------------------------------*/

static void XschDrawTristate( X1r, Y1r, X2r, Y2r, Ycr, Obj )

  long          X1r;
  long          Y1r;
  long          X2r;
  long          Y2r;
  long          Ycr;
  xschobj_list *Obj;
{
/* 
** Fred Petrot  (The King ) has initially written those lines !
** Ludo Jacomme (The Slave) has modified them :-)
*/
/*long gs;*/
  long is_x;
  long is_y;
  long cs;
  long csm;
  long x, /*x1,*/ y1, y2,y0;
  long DeltaX;
  long DeltaY;

  DeltaX = X2r - X1r;
  DeltaY = Y2r - Y1r;

  if ( DeltaX <= 0 ) DeltaX = 1;
  if ( DeltaY <= 0 ) DeltaY = 1;

# if 0
  XDrawRectangle( XschGraphicDisplay,
                  XschGraphicPixmap,
                  XschDrawGC,
                  X1r, XschGraphicDy - Y2r,
                  DeltaX, DeltaY );
# endif

/*gs = DeltaX;*/
  is_x = ( 5 * DeltaX ) / 16;
  is_y = ( 5 * DeltaY ) / 16;
  cs = DeltaX / 8;
  csm = DeltaX / 10;

  y1 = XschGraphicDy - Ycr;
/*x1 = (X2r + X1r) / 2;*/
  x  = X1r + cs;
 
  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap,
             XschDrawGC,
             x, y1 - is_y/2,
             x, y1 + is_y/2);
 
  XDrawLine(XschGraphicDisplay, XschGraphicPixmap, XschDrawGC,
            x       , y1 - is_y/2,
            x + is_x, y1);
  XDrawLine(XschGraphicDisplay, XschGraphicPixmap, XschDrawGC,
            x       , y1 + is_y/2,
            x + is_x, y1);

  y0 = Obj->Y + ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );
  y0 = (float)( y0 ) * XschUnitGridStep;
  y0 = ( y0 / XSCH_UNIT ) - XschPixelGridY;
  y0 = XschGraphicDy - y0;

  y2 = Obj->Y + ( SCP_BOX_CON_BASE_Y + SCP_BOX_CON_STEP_Y ) * XSCH_UNIT;
  y2 = (float)( y2 ) * XschUnitGridStep;
  y2 = ( y2 / XSCH_UNIT ) - XschPixelGridY;
  y2 = XschGraphicDy - y2;

  if ( ( Obj->ARG1 & 1) == 0 )
  {
     /* data line : centered vertically */
     XDrawLine( XschGraphicDisplay, XschGraphicPixmap, XschDrawGC,
               x - cs, y1,
               x     , y1 );
  }
  else
  {
     /* Data circle */
     XDrawArc( XschGraphicDisplay, XschGraphicPixmap, XschDrawGC,
              x - cs, y1 - cs/2, cs, cs, 0, 23040 );
  }

  /* Data Line */

  XDrawLine( XschGraphicDisplay, XschGraphicPixmap, XschDrawGC,
             x - cs, y0, x - cs, y1  );

  if ( ( Obj->ARG1 & 2) == 0 )
  {
    /* Command Line */
    XDrawLine(XschGraphicDisplay, XschGraphicPixmap, XschDrawGC,
              x - cs, y2, x + is_x/2, y2 );

    XDrawLine(XschGraphicDisplay, XschGraphicPixmap, XschDrawGC,
              x + is_x/2, y2, x + is_x/2, y1 - is_y/4 );
  }
  else
  {
    /* Command circle */
    XFillArc( XschGraphicDisplay, XschGraphicPixmap, XschDrawGC,
              x + is_x/2 - csm/2, y1 - is_y/2 - csm/4, csm, csm, 0, 23040 );

    /* Command Line */
    XDrawLine(XschGraphicDisplay, XschGraphicPixmap, XschDrawGC,
              x - cs, y2, x + is_x/2, y2 );

    XDrawLine(XschGraphicDisplay, XschGraphicPixmap, XschDrawGC,
              x + is_x/2, y2, x + is_x/2, y2 + csm );
  }
 
  if ( ( Obj->ARG2 ) == 0 )
  {
     /* Output line : centered vertically */
     XDrawLine( XschGraphicDisplay, XschGraphicPixmap, XschDrawGC,
               x + is_x     , y1,
               x + is_x + cs, y1 );
  }
  else
  {
     /* Output circle */
     XDrawArc( XschGraphicDisplay, XschGraphicPixmap, XschDrawGC,
              x + is_x, y1 - cs/2, cs, cs, 0, 23040 );
  }

  XDrawLine( XschGraphicDisplay, XschGraphicPixmap, XschDrawGC,
             x + is_x + cs, y1,
             X2r, y1 );
}

/*------------------------------------------------------------\
|                                                             |
|                       XschDrawConstant                      |
|                                                             |
\------------------------------------------------------------*/

static void XschDrawConstant( X1r, Y1r, X2r, Y2r, Ycr, Obj )

  long          X1r;
  long          Y1r;
  long          X2r;
  long          Y2r;
  long          Ycr;
  xschobj_list *Obj;
{
 long cs;
 long x, yc, y1,y2,y3;
 long DeltaX;
#if 0
 long DeltaY;
#endif
 int  i;
 int  n;
 
 n = Obj->ARG1;

 DeltaX = X2r - X1r;

 if ( DeltaX <= 0 ) DeltaX = 1;

# if 0
 DeltaY = Y2r - Y1r;
 if ( DeltaY <= 0 ) DeltaY = 1;

 XDrawRectangle( XschGraphicDisplay,
                 XschGraphicPixmap,
                 XschDrawGC,
                 X1r, XschGraphicDy - Y2r,
                 DeltaX, DeltaY );
# endif

  cs = DeltaX / 8;
  x  = X1r + cs;

  y1 = Obj->Y + ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );

  for ( i = 0; i < n; i++ )
  {
    yc = (float)( y1 ) * XschUnitGridStep;
    yc = ( yc / XSCH_UNIT ) - XschPixelGridY;
    yc = XschGraphicDy - yc;

    if (( Obj->ARG2 & (1 << i)) == 0)
    {
      y2 = yc + cs;
      y3 = y2 + cs;
  
      XDrawLine( XschGraphicDisplay,
                 XschGraphicPixmap,
                 XschDrawGC,
                 x, yc, x, y2 );
  
      XDrawLine( XschGraphicDisplay,
                 XschGraphicPixmap,
                 XschDrawGC,
                 x - cs, y2, x + cs, y2 );
  
      XDrawLine( XschGraphicDisplay,
                 XschGraphicPixmap,
                 XschDrawGC,
                 x - cs, y2, x, y3 );
  
      XDrawLine( XschGraphicDisplay,
                 XschGraphicPixmap,
                 XschDrawGC,
                 x + cs, y2, x, y3 );
    }
    else
    {
      y2 = yc - cs;
      y3 = y2 - cs;
  
      XDrawLine( XschGraphicDisplay,
                 XschGraphicPixmap,
                 XschDrawGC,
                 x, yc, x, y2 );
  
      XDrawLine( XschGraphicDisplay,
                 XschGraphicPixmap,
                 XschDrawGC,
                 x, y2, x - cs, y3 );
  
      XDrawLine( XschGraphicDisplay,
                 XschGraphicPixmap,
                 XschDrawGC,
                 x, y2, x + cs, y3 );
    }

    XDrawLine( XschGraphicDisplay, XschGraphicPixmap, XschDrawGC,
               x, yc, X2r, yc );

    y1 += ( SCP_BOX_CON_STEP_Y * XSCH_UNIT );
  }

# if 0
  y1 = XschGraphicDy - Ycr;

  if ( ! Obj->ARG2 ) /* constant zero */
  {
    y2 = y1 + cs;
    y3 = y2 + cs;

    XDrawLine( XschGraphicDisplay,
               XschGraphicPixmap,
               XschDrawGC,
               x, y1, x, y2 );

    XDrawLine( XschGraphicDisplay,
               XschGraphicPixmap,
               XschDrawGC,
               x - cs, y2, x + cs, y2 );

    XDrawLine( XschGraphicDisplay,
               XschGraphicPixmap,
               XschDrawGC,
               x - cs, y2, x, y3 );

    XDrawLine( XschGraphicDisplay,
               XschGraphicPixmap,
               XschDrawGC,
               x + cs, y2, x, y3 );
  }
  else              /* constant one */
  {
    y2 = y1 - cs;
    y3 = y2 - cs;

    XDrawLine( XschGraphicDisplay,
               XschGraphicPixmap,
               XschDrawGC,
               x, y1, x, y2 );

    XDrawLine( XschGraphicDisplay,
               XschGraphicPixmap,
               XschDrawGC,
               x, y2, x - cs, y3 );

    XDrawLine( XschGraphicDisplay,
               XschGraphicPixmap,
               XschDrawGC,
               x, y2, x + cs, y3 );
  }
# endif
}

/*------------------------------------------------------------\
|                                                             |
|                       XschDrawBuffer                        |
|                                                             |
\------------------------------------------------------------*/

static void XschDrawBuffer( X1r, Y1r, X2r, Y2r, Ycr, Obj )

  long          X1r;
  long          Y1r;
  long          X2r;
  long          Y2r;
  long          Ycr;
  xschobj_list *Obj;
{
/* 
** Fred Petrot  (The King ) has initially written those lines !
** Ludo Jacomme (The Slave) has modified them :-)
*/
 long is_x;
 long is_y;
 long cs;
 long x, y1;
 long DeltaX;
 long DeltaY;

 DeltaX = X2r - X1r;
 DeltaY = Y2r - Y1r;

 if ( DeltaX <= 0 ) DeltaX = 1;
 if ( DeltaY <= 0 ) DeltaY = 1;

# if 0
  XDrawRectangle( XschGraphicDisplay,
                  XschGraphicPixmap,
                  XschDrawGC,
                  X1r, XschGraphicDy - Y2r,
                  DeltaX, DeltaY );
# endif

   is_x = ( 5 * DeltaX ) / 16;
   is_y = ( 5 * DeltaY ) / 8;
   cs = DeltaX / 8;

  y1 = XschGraphicDy - Ycr;
  x  = X1r + cs;
 
  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap,
             XschDrawGC,
             x, y1 - is_y/2,
             x, y1 + is_y/2);
 
  XDrawLine(XschGraphicDisplay, XschGraphicPixmap, XschDrawGC,
            x       , y1 - is_y/2,
            x + is_x, y1);
  XDrawLine(XschGraphicDisplay, XschGraphicPixmap, XschDrawGC,
            x       , y1 + is_y/2,
            x + is_x, y1);

  if ( ( Obj->ARG1 ) == 0 )
  {
     /* input line : centered vertically */
     XDrawLine( XschGraphicDisplay, XschGraphicPixmap, XschDrawGC,
               x - cs, y1,
               x     , y1 );
  }
  else
  {
     /* Input circle */
     XDrawArc( XschGraphicDisplay, XschGraphicPixmap, XschDrawGC,
              x - cs, y1 - cs/2, cs, cs, 0, 23040 );
  }
 
  if ( ( Obj->ARG2 ) == 0 )
  {
     /* Output line : centered vertically */
     XDrawLine( XschGraphicDisplay, XschGraphicPixmap, XschDrawGC,
               x + is_x     , y1,
               x + is_x + cs, y1 );
  }
  else
  {
     /* Output circle */
     XDrawArc( XschGraphicDisplay, XschGraphicPixmap, XschDrawGC,
              x + is_x, y1 - cs/2, cs, cs, 0, 23040 );
  }

  XDrawLine( XschGraphicDisplay, XschGraphicPixmap, XschDrawGC,
             x + is_x + cs, y1,
             X2r, y1 );
}

/*------------------------------------------------------------\
|                                                             |
|                       XschDrawTransistor                    |
|                                                             |
\------------------------------------------------------------*/

static void XschDrawTransistor( X1r, Y1r, X2r, Y2r, Ycr, Obj )

  long          X1r;
  long          Y1r;
  long          X2r;
  long          Y2r;
  long          Ycr;
  xschobj_list *Obj;
{
/* 
** Fred Petrot  (The King ) has initially written those lines !
** Ludo Jacomme (The Slave) has modified them :-)
*/
  long cs;
  long x, x1, y1, y2,y0;
  long DeltaX;
  long DeltaY;

  DeltaX = X2r - X1r;
  DeltaY = Y2r - Y1r;

  if ( DeltaX <= 0 ) DeltaX = 1;
  if ( DeltaY <= 0 ) DeltaY = 1;

  cs = DeltaX / 8;

  y0 = Obj->Y + ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );
  y0 = (float)( y0 ) * XschUnitGridStep;
  y0 = ( y0 / XSCH_UNIT ) - XschPixelGridY;
  y0 = XschGraphicDy - y0;

  y2 = Obj->Y + ( SCP_BOX_CON_BASE_Y + SCP_BOX_CON_STEP_Y ) * XSCH_UNIT;
  y2 = (float)( y2 ) * XschUnitGridStep;
  y2 = ( y2 / XSCH_UNIT ) - XschPixelGridY;
  y2 = XschGraphicDy - y2;

  y1 = XschGraphicDy - Ycr;
  /*
  x1 = X2r - cs;
  */
  x1 = X1r + 3 * cs;
  x  = X1r;

  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap,
             XschDrawGC,
             x, y0, x, y0 - cs );

  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap,
             XschDrawGC,
             x, y0 - cs, x1 - cs, y0 - cs );

  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap,
             XschDrawGC,
             x1 - cs, y0 - cs, x1 - cs, y2 + cs );

  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap,
             XschDrawGC,
             x1 - cs, y2 + cs, x, y2 + cs );

  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap,
             XschDrawGC,
             x, y2 + cs, x, y2 );

  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap,
             XschDrawGC,
             x1, y0 - cs, x1, y2 + cs );

  if ( ( Obj->ARG1 ) == 1 ) /* This is TRANSN */
  {
     /* Output line : centered vertically */
     XDrawLine( XschGraphicDisplay, XschGraphicPixmap, XschDrawGC,
               x1, y1, x1 + cs, y1 );
  }
  else
  {
     /* Output circle */
     XDrawArc( XschGraphicDisplay, XschGraphicPixmap, XschDrawGC,
              x1, y1 - cs/2, cs, cs, 0, 23040 );
  }

  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap,
             XschDrawGC,
             x1 + cs, y1, X2r, y1 );
}

/*------------------------------------------------------------\
|                                                             |
|                       XschDrawAnd                           |
|                                                             |
\------------------------------------------------------------*/

static void XschDrawAnd( X1r, Y1r, X2r, Y2r, Ycr, Obj )

  long          X1r;
  long          Y1r;
  long          X2r;
  long          Y2r;
  long          Ycr;
  xschobj_list *Obj;
{
/* 
** Fred Petrot  (The King ) has initially written those lines !
** Ludo Jacomme (The Slave) has modified them :-)
*/
  long gs_y;
  long gs_x;
  long cs;
  int i;
  int n;
  long x, y;
  long yc;
  long y1;
 long DeltaX;
 long DeltaY;

 DeltaX = X2r - X1r;
 DeltaY = Y2r - Y1r;

 if ( DeltaX <= 0 ) DeltaX = 1;
 if ( DeltaY <= 0 ) DeltaY = 1;

# if 0
XDrawRectangle( XschGraphicDisplay,
                XschGraphicPixmap,
                XschDrawGC,
                X1r, XschGraphicDy - Y2r,
                DeltaX, DeltaY );
# endif

  gs_y = ( 7 * DeltaY ) / 8;
  gs_x = ( 6 * DeltaX ) / 8;
  cs   = DeltaX / 8;

  yc = XschGraphicDy - Ycr;
  y  = XschGraphicDy - Ycr - gs_y/2;
  x = X1r + cs;
  n = Obj->ARG1;

  /* And shape alone */

  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap,
             XschDrawGC,
             x, y, x + gs_x/2, y );

  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap, 
             XschDrawGC,
             x         , y + gs_y,
             x + gs_x/2, y + gs_y );

  XDrawArc( XschGraphicDisplay,
            XschGraphicPixmap,
            XschDrawGC,
            x, y, gs_x, gs_y, 90 * 64, -180 * 64);

  if ( Obj->ARG3 == 0 )
  {
     /* Output line : centered vertically */
     XDrawLine( XschGraphicDisplay,
                XschGraphicPixmap,
                XschDrawGC,
                x + gs_x, yc, x + gs_x + cs, yc );
  }
  else 
  {
     /* Output circle (for a NAND) */
     XDrawArc( XschGraphicDisplay,
               XschGraphicPixmap,
               XschDrawGC,
               x + gs_x, yc - cs/2, cs, cs, 0 * 64, 360 * 64);
  }

  /* Input lines */
  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap,
             XschDrawGC, x, y, x, y + gs_y);

  y1 = Obj->Y + ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );

  /* Circle for inversion */
  for (i = 0; i < n; i++)
  {
    yc = (float)( y1 ) * XschUnitGridStep;
    yc = ( yc / XSCH_UNIT ) - XschPixelGridY;
    yc = XschGraphicDy - yc;

    if (( Obj->ARG2 & (1 << i)) == 0)
    {
       XDrawLine( XschGraphicDisplay,
                  XschGraphicPixmap,
                  XschDrawGC,
                  x - cs, yc, x, yc );
    }
    else
    {
      XDrawArc( XschGraphicDisplay,
                XschGraphicPixmap,
                XschDrawGC,
                x  - cs,
                yc - cs/2,
                cs, cs, 0 * 64, 360 * 64);
    }

    y1 += ( SCP_BOX_CON_STEP_Y * XSCH_UNIT );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       XschDrawRegister                      |
|                                                             |
\------------------------------------------------------------*/

static void XschDrawRegister( X1r, Y1r, X2r, Y2r, Ycr, Obj )

  long          X1r;
  long          Y1r;
  long          X2r;
  long          Y2r;
  long          Ycr;
  xschobj_list *Obj;
{
/* 
** Fred Petrot  (The King ) has initially written those lines !
** Ludo Jacomme (The Slave) has modified them :-)
*/
  long gs_y;
  long gs_x;
  long cs;
  long es_x;
  long es_y;
  int i;
  int ni;
  long x, y;
  long yc;
  long xc;
  long y1;
  long DeltaX;
  long DeltaY;

  DeltaX = X2r - X1r;
  DeltaY = Y2r - Y1r;

  if ( DeltaX <= 0 ) DeltaX = 1;
  if ( DeltaY <= 0 ) DeltaY = 1;

# if 0
  XDrawRectangle( XschGraphicDisplay,
                  XschGraphicPixmap,
                  XschDrawGC,
                  X1r, XschGraphicDy - Y2r,
                  DeltaX, DeltaY );
# endif

  gs_y = ( 7 * DeltaY ) / 8;
  gs_x = ( 6 * DeltaX ) / 8;
  cs   = DeltaX / 8;

  es_y = DeltaX / 4;
  es_x = DeltaX / 8;

  yc = XschGraphicDy - Ycr;
  y  = XschGraphicDy - Ycr - gs_y/2;
  x = X1r + cs;
  xc = (X2r + X1r) /2;
  ni = Obj->ARG1;

  /* shape alone */

  XDrawRectangle( XschGraphicDisplay,
                  XschGraphicPixmap,
                  XschDrawGC,
                  x, y, gs_x, gs_y );

/*
** Rising/Falling Edge
*/
  if ( ( Obj->ARG5 == 1 ) ||
       ( Obj->ARG5 == 2 ) )
  {
    XDrawLine( XschGraphicDisplay,
               XschGraphicPixmap,
               XschDrawGC,
               xc, yc - es_y, xc, yc + es_y );

/*
** Rising
*/
    if ( Obj->ARG5 == 1 )
    {
      XDrawLine( XschGraphicDisplay,
                 XschGraphicPixmap,
                 XschDrawGC,
                 xc, yc + es_y, xc - es_x, yc + es_y );

      XDrawLine( XschGraphicDisplay,
                 XschGraphicPixmap,
                 XschDrawGC,
                 xc, yc - es_y, xc + es_x, yc - es_y );
    }
    else
/*
** Falling
*/
    {
      XDrawLine( XschGraphicDisplay,
                 XschGraphicPixmap,
                 XschDrawGC,
                 xc, yc - es_y, xc - es_x, yc - es_y );

      XDrawLine( XschGraphicDisplay,
                 XschGraphicPixmap,
                 XschDrawGC,
                 xc, yc + es_y, xc + es_x, yc + es_y );
    }
  }

  if ( Obj->ARG4 == 0 )
  {
     /* Output line : centered vertically */
     XDrawLine( XschGraphicDisplay,
                XschGraphicPixmap,
                XschDrawGC,
                x + gs_x, yc, x + gs_x + cs, yc );
  }
  else 
  {
     /* Output circle */
     XDrawArc( XschGraphicDisplay,
               XschGraphicPixmap,
               XschDrawGC,
               x + gs_x, yc - cs/2, cs, cs, 0 * 64, 360 * 64);
  }


  /* Input lines */
  /* Circle for inversion */

  y1 = Obj->Y + ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );

  for (i = 0; i < ni; i++)
  {
    yc = (float)( y1 ) * XschUnitGridStep;
    yc = ( yc / XSCH_UNIT ) - XschPixelGridY;
    yc = XschGraphicDy - yc;

    if (( Obj->ARG2 & (1 << i)) == 0)
    {
       XDrawLine( XschGraphicDisplay,
                  XschGraphicPixmap,
                  XschDrawGC,
                  x - cs, yc, x, yc );
    }
    else
    {
      XDrawArc( XschGraphicDisplay,
                XschGraphicPixmap,
                XschDrawGC,
                x  - cs,
                yc - cs/2,
                cs, cs, 0 * 64, 360 * 64);
    }

    /* Clock */

    if (( Obj->ARG3 & (1 << i)) != 0)
    {
       XDrawLine( XschGraphicDisplay,
                  XschGraphicPixmap,
                  XschDrawGC,
                  x, yc - cs/2, x + cs, yc  );

       XDrawLine( XschGraphicDisplay,
                  XschGraphicPixmap,
                  XschDrawGC,
                  x + cs, yc, x, yc + cs/2  );
    }

    y1 += ( SCP_BOX_CON_STEP_Y * XSCH_UNIT );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       XschDrawOr                            |
|                                                             |
\------------------------------------------------------------*/

static void XschDrawOr( X1r, Y1r, X2r, Y2r, Ycr, Obj )

  long          X1r;
  long          Y1r;
  long          X2r;
  long          Y2r;
  long          Ycr;
  xschobj_list *Obj;
{
/* 
** Fred Petrot  (The King ) has initially written those lines !
** Ludo Jacomme (The Slave) has modified them :-)
*/
  long gs_y;
  long gs_x;
  long cs;
  long Radius;
  int i;
  int n;
  long x, y;
  long yc;
  long y1;
  long DeltaX;
  long DeltaY;

  DeltaX = X2r - X1r;
  DeltaY = Y2r - Y1r;

  if ( DeltaX <= 0 ) DeltaX = 1;
  if ( DeltaY <= 0 ) DeltaY = 1;

# if 0
  XDrawRectangle( XschGraphicDisplay,
                  XschGraphicPixmap,
                  XschDrawGC,
                  X1r, XschGraphicDy - Y2r,
                  DeltaX, DeltaY );
# endif

  gs_y = ( 7 * DeltaY ) / 8;
  gs_x = ( 6 * DeltaX ) / 8;
  cs   = DeltaX / 8;

  yc = XschGraphicDy - Ycr;
  y  = XschGraphicDy - Ycr - gs_y/2;
  x = X1r + cs;
  n = Obj->ARG1;

  /* Or shape alone */

  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap,
             XschDrawGC,
             x, y, x + gs_x/2, y );

  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap, 
             XschDrawGC,
             x         , y + gs_y,
             x + gs_x/2, y + gs_y );

  XDrawArc( XschGraphicDisplay,
            XschGraphicPixmap,
            XschDrawGC,
            x, y, gs_x, gs_y, 90 * 64, -180 * 64);

  Radius = (float)(gs_y) * 0.707106781185;

  XDrawArc( XschGraphicDisplay,
            XschGraphicPixmap,
            XschDrawGC,
            x - (Radius * 5 / 3), yc - Radius,
            Radius * 2, Radius * 2, 45 * 64, -90 * 64);

  if ( Obj->ARG3 == 0 )
  {
     /* Output line : centered vertically */
     XDrawLine( XschGraphicDisplay,
                XschGraphicPixmap,
                XschDrawGC,
                x + gs_x, yc, x + gs_x + cs, yc );
  }
  else 
  {
     /* Output circle */
     XDrawArc( XschGraphicDisplay,
               XschGraphicPixmap,
               XschDrawGC,
               x + gs_x, yc - cs/2, cs, cs, 0 * 64, 360 * 64);
  }

  /* Input lines */
# if 0
  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap,
             XschDrawGC, x, y, x, y + gs_y);
# endif

  y1 = Obj->Y + ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );

  /* Circle for inversion */
  for (i = 0; i < n; i++)
  {
    yc = (float)( y1 ) * XschUnitGridStep;
    yc = ( yc / XSCH_UNIT ) - XschPixelGridY;
    yc = XschGraphicDy - yc;

    if (( Obj->ARG2 & (1 << i)) == 0)
    {
       XDrawLine( XschGraphicDisplay,
                  XschGraphicPixmap,
                  XschDrawGC,
                  x - cs, yc, x, yc );
    }
    else
    {
      XDrawArc( XschGraphicDisplay,
                XschGraphicPixmap,
                XschDrawGC,
                x  - cs,
                yc - cs/2,
                cs, cs, 0 * 64, 360 * 64);
    }

    y1 += ( SCP_BOX_CON_STEP_Y * XSCH_UNIT );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       XschDrawXor                           |
|                                                             |
\------------------------------------------------------------*/

static void XschDrawXor( X1r, Y1r, X2r, Y2r, Ycr, Obj )

  long          X1r;
  long          Y1r;
  long          X2r;
  long          Y2r;
  long          Ycr;
  xschobj_list *Obj;
{
/* 
** Fred Petrot  (The King ) has initially written those lines !
** Ludo Jacomme (The Slave) has modified them :-)
*/
  long gs_y;
  long gs_x;
  long cs;
  long Radius;
  int i;
  int n;
  long x, y;
  long yc;
  long y1;
  long DeltaX;
  long DeltaY;

  DeltaX = X2r - X1r;
  DeltaY = Y2r - Y1r;

  if ( DeltaX <= 0 ) DeltaX = 1;
  if ( DeltaY <= 0 ) DeltaY = 1;

# if 0
  XDrawRectangle( XschGraphicDisplay,
                  XschGraphicPixmap,
                  XschDrawGC,
                  X1r, XschGraphicDy - Y2r,
                  DeltaX, DeltaY );
# endif

  gs_y = ( 7 * DeltaY ) / 8;
  gs_x = ( 6 * DeltaX ) / 8;
  cs   = DeltaX / 8;

  yc = XschGraphicDy - Ycr;
  y  = XschGraphicDy - Ycr - gs_y/2;
  x = X1r + cs;
  n = Obj->ARG1;

  /* Xor shape alone */

  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap,
             XschDrawGC,
             x + gs_x/4, y, x + gs_x/2, y );

  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap, 
             XschDrawGC,
             x + gs_x/4, y + gs_y,
             x + gs_x/2, y + gs_y );

  XDrawArc( XschGraphicDisplay,
            XschGraphicPixmap,
            XschDrawGC,
            x, y, gs_x, gs_y, 90 * 64, -180 * 64);

  Radius = (float)(gs_y) * 0.707106781185; /* sqrt(2) */

  XDrawArc( XschGraphicDisplay,
            XschGraphicPixmap,
            XschDrawGC,
            x - (Radius * 5 / 3), yc - Radius,
            Radius * 2, Radius * 2, 45 * 64, -90 * 64);

  XDrawArc( XschGraphicDisplay,
            XschGraphicPixmap,
            XschDrawGC,
            x - (Radius * 4 / 3), yc - Radius,
            Radius * 2, Radius * 2, 45 * 64, -90 * 64);

  if ( Obj->ARG3 == 0 )
  {
     /* Output line : centered vertically */
     XDrawLine( XschGraphicDisplay,
                XschGraphicPixmap,
                XschDrawGC,
                x + gs_x, yc, x + gs_x + cs, yc );
  }
  else 
  {
     /* Output circle */
     XDrawArc( XschGraphicDisplay,
               XschGraphicPixmap,
               XschDrawGC,
               x + gs_x, yc - cs/2, cs, cs, 0 * 64, 360 * 64);
  }

  /* Input lines */
# if 0
  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap,
             XschDrawGC, x, y, x, y + gs_y);
# endif

  y1 = Obj->Y + ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );

  /* Circle for inversion */
  for (i = 0; i < n; i++)
  {
    yc = (float)( y1 ) * XschUnitGridStep;
    yc = ( yc / XSCH_UNIT ) - XschPixelGridY;
    yc = XschGraphicDy - yc;

     if (( Obj->ARG2 & (1 << i)) == 0)
     {
        XDrawLine( XschGraphicDisplay,
                   XschGraphicPixmap,
                   XschDrawGC,
                   x - cs, yc, x, yc );
     }
     else
     {
       XDrawArc( XschGraphicDisplay,
                 XschGraphicPixmap,
                 XschDrawGC,
                 x  - cs,
                 yc - cs/2,
                 cs, cs, 0 * 64, 360 * 64);
     }

     y1 += ( SCP_BOX_CON_STEP_Y * XSCH_UNIT );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       XschDrawAndOr                         |
|                                                             |
\------------------------------------------------------------*/

static void XschDrawAndOr( X1r, Y1r, X2r, Y2r, Ycr, Obj )

  long          X1r;
  long          Y1r;
  long          X2r;
  long          Y2r;
  long          Ycr;
  xschobj_list *Obj;
{
  long gs_y;
  long gs_x;
  long cs;
  int i;
  int n;
  int na;
  long gos_y;
  long gos_x;
  long gas_y;
  long gas_x;
  long Radius;

  long x, y;
  long yc;
  long y1;
  long DeltaX;
  long DeltaY;

 DeltaX = X2r - X1r;
 DeltaY = Y2r - Y1r;

 if ( DeltaX <= 0 ) DeltaX = 1;
 if ( DeltaY <= 0 ) DeltaY = 1;

# if 0
XDrawRectangle( XschGraphicDisplay,
                XschGraphicPixmap,
                XschDrawGC,
                X1r, XschGraphicDy - Y2r,
                DeltaX, DeltaY );
# endif

  n = Obj->ARG1;

  gs_y = ( 7 * DeltaY ) / 8;
  gs_x = ( 6 * DeltaX ) / 8;
  cs   = DeltaX / 8;

  yc = XschGraphicDy - Ycr;
  y  = XschGraphicDy - Ycr - gs_y/2;
  x = X1r + cs;

  if ( Obj->ARG3 == 0 )
  {
     /* Output line : centered vertically */
     XDrawLine( XschGraphicDisplay,
                XschGraphicPixmap,
                XschDrawGC,
                x + gs_x, yc, x + gs_x + cs, yc );
  }
  else 
  {
     /* Output circle */
     XDrawArc( XschGraphicDisplay,
               XschGraphicPixmap,
               XschDrawGC,
               x + gs_x, yc - cs/2, cs, cs, 0 * 64, 360 * 64);
  }

/*
** Input connectors
*/
  y1 = Obj->Y + ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );

  /* Circle for inversion */
  for (i = 0; i < n; i++)
  {
    yc = (float)( y1 ) * XschUnitGridStep;
    yc = ( yc / XSCH_UNIT ) - XschPixelGridY;
    yc = XschGraphicDy - yc;

    if (( Obj->ARG2 & (1 << i)) == 0)
    {
       XDrawLine( XschGraphicDisplay,
                  XschGraphicPixmap,
                  XschDrawGC,
                  x - cs, yc, x, yc );
    }
    else
    {
      XDrawArc( XschGraphicDisplay,
                XschGraphicPixmap,
                XschDrawGC,
                x  - cs,
                yc - cs/2,
                cs, cs, 0 * 64, 360 * 64);
    }

    y1 += ( SCP_BOX_CON_STEP_Y * XSCH_UNIT );
  }
/*
** Draw Input And gates
*/
  na     = n / 2;

  if ( n & 1 ) gas_y   = 7 * DeltaY / ( 8 * ( na + 1 ) );
  else         gas_y   = 7 * DeltaY / ( 8 * na );

  gas_x = gs_x / 3;

  y1 = Obj->Y + Obj->DY - ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );
  y1 -= SCP_BOX_CON_STEP_Y * XSCH_UNIT / 2;

  for ( i = 0; i < na; i++ )
  {
    yc = (float)( y1 ) * XschUnitGridStep;
    yc = ( yc / XSCH_UNIT ) - XschPixelGridY;
    yc = XschGraphicDy - yc;

    XDrawLine( XschGraphicDisplay,
               XschGraphicPixmap,
               XschDrawGC, x, yc - gas_y /2, x, yc + gas_y/2);

    /* And Shape */

    XDrawLine( XschGraphicDisplay,
               XschGraphicPixmap,
               XschDrawGC,
               x, yc - gas_y/2, x + gas_x/2, yc - gas_y/2 );

    XDrawLine( XschGraphicDisplay,
               XschGraphicPixmap, 
               XschDrawGC,
               x         , yc + gas_y/2,
               x + gas_x/2, yc + gas_y/2 );

    XDrawArc( XschGraphicDisplay,
              XschGraphicPixmap,
              XschDrawGC,
              x, yc - gas_y/2, gas_x, gas_y -1, 90 * 64, -180 * 64);

    y1 -= 2 * ( SCP_BOX_CON_STEP_Y * XSCH_UNIT );
  }

  if ( n & 1 )
  {
    y1  = y1 + ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );

    yc = (float)( y1 ) * XschUnitGridStep;
    yc = ( yc / XSCH_UNIT ) - XschPixelGridY;
    yc = XschGraphicDy - yc;

    XDrawLine( XschGraphicDisplay,
               XschGraphicPixmap, 
               XschDrawGC,
               x , yc, x + gas_x, yc  );


  }
/*
** Draw Output Or gate
*/
  /* Or shape alone */
  gos_y   = 2 * DeltaY / 3;
  gos_x   = ( 2 * gs_x ) / 3;
  yc      = XschGraphicDy - Ycr;
  y       = yc + gos_y /2;
  x       = x + gas_x;

  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap,
             XschDrawGC,
             x, y, x + gos_x/2, y );

  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap, 
             XschDrawGC,
             x          , y - gos_y,
             x + gos_x/2, y - gos_y );

  XDrawArc( XschGraphicDisplay,
            XschGraphicPixmap,
            XschDrawGC,
            x, y - gos_y, gos_x, gos_y, 90 * 64, -180 * 64);

  Radius = (float)(gos_y) * 0.707106781185;

  XDrawArc( XschGraphicDisplay,
            XschGraphicPixmap,
            XschDrawGC,
            x - (Radius * 5 / 3), yc - Radius,
            Radius * 2, Radius * 2, 45 * 64, -90 * 64);
}

/*------------------------------------------------------------\
|                                                             |
|                       XschDrawMux                           |
|                                                             |
\------------------------------------------------------------*/

static void XschDrawMux( X1r, Y1r, X2r, Y2r, Ycr, Obj )

  long          X1r;
  long          Y1r;
  long          X2r;
  long          Y2r;
  long          Ycr;
  xschobj_list *Obj;
{
#if 0
  long gs_y;
#endif
  long gs_x;
  long ms_x;
  long ms_y;
  long cs;
  int i;
  int ni;
  int ns;

  long x;
  long yc;
  long y1;
  long y2;
  long y3;
  long x3;
  long DeltaX;
  long DeltaY;

 DeltaX = X2r - X1r;
 DeltaY = Y2r - Y1r;

 if ( DeltaX <= 0 ) DeltaX = 1;
 if ( DeltaY <= 0 ) DeltaY = 1;

 ns = Obj->ARG1;
 ni = Obj->ARG2;

# if 0
XDrawRectangle( XschGraphicDisplay,
                XschGraphicPixmap,
                XschDrawGC,
                X1r, XschGraphicDy - Y2r,
                DeltaX, DeltaY );

  gs_y = ( 7 * DeltaY ) / 8;
# endif

  gs_x = ( 6 * DeltaX ) / 8;
  cs   = DeltaX / 8;

  yc = XschGraphicDy - Ycr;
  x = X1r + cs;

/*
** Draw Mux Rectangle
*/
  ms_x = gs_x / 2;

  y1 = Obj->Y + (2 * SCP_BOX_CON_BASE_Y + (ni - 1) * SCP_BOX_CON_STEP_Y) * XSCH_UNIT;

  y2 = (float)( y1 ) * XschUnitGridStep;
  y2 = ( y2 / XSCH_UNIT ) - XschPixelGridY;

  ms_y = y2 - Y1r;

  y2 = XschGraphicDy - y2;

  XDrawRectangle( XschGraphicDisplay,
                  XschGraphicPixmap,
                  XschDrawGC,
                  x, y2, ms_x, ms_y );

  XDrawLine( XschGraphicDisplay,
                 XschGraphicPixmap,
                 XschDrawGC,
                 x, y2, x + ms_x, y2 + ms_y / 2 );

  XDrawLine( XschGraphicDisplay,
                 XschGraphicPixmap,
                 XschDrawGC,
                 x + ms_x, y2 + ms_y/2, x, y2 + ms_y );

  if ( Obj->ARG4 == 0 )
  {
     /* Output line : centered vertically */
     XDrawLine( XschGraphicDisplay,
                XschGraphicPixmap,
                XschDrawGC,
                x + ms_x, y2 + ms_y/2, x + ms_x + cs, y2 + ms_y/2 );
  }
  else 
  {
     /* Output circle */
     XDrawArc( XschGraphicDisplay,
               XschGraphicPixmap,
               XschDrawGC,
               x + ms_x, y2 + ms_y/2 - cs/2, cs, cs, 0 * 64, 360 * 64);
  }

  y3 = y2 + ms_y/2;

  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap,
             XschDrawGC,
             x + ms_x + cs, y3, x + gs_x + cs, y3 );

  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap,
             XschDrawGC,
             x + gs_x + cs, y3, x + gs_x + cs, yc );
/*
** Input connectors
*/
  y1 = Obj->Y + ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );

  /* Circle for inversion */
  for (i = 0; i < ni; i++)
  {
    y3 = (float)( y1 ) * XschUnitGridStep;
    y3 = ( y3 / XSCH_UNIT ) - XschPixelGridY;
    y3 = XschGraphicDy - y3;

    if (( Obj->ARG3 & (1 << i)) == 0)
    {
      XDrawLine( XschGraphicDisplay,
                 XschGraphicPixmap,
                 XschDrawGC,
                 x - cs, y3, x, y3 );
    }
    else
    {
      XDrawArc( XschGraphicDisplay,
                XschGraphicPixmap,
                XschDrawGC,
                x  - cs,
                y3 - cs/2,
                cs, cs, 0 * 64, 360 * 64);
    }

    y1 += ( SCP_BOX_CON_STEP_Y * XSCH_UNIT );
  }

  x3 = x;
/*
** Select connectors
*/
  for (i = 0; i < ns; i++)
  {
    y3 = (float)( y1 ) * XschUnitGridStep;
    y3 = ( y3 / XSCH_UNIT ) - XschPixelGridY;
    y3 = XschGraphicDy - y3;
    
    x3 += ms_x / ( ns + 1 );

    XDrawLine( XschGraphicDisplay,
               XschGraphicPixmap,
               XschDrawGC,
               x - cs, y3, x3, y3 );

    XDrawLine( XschGraphicDisplay,
               XschGraphicPixmap,
               XschDrawGC,
               x3, y3, x3, y2 );

    y1 += ( SCP_BOX_CON_STEP_Y * XSCH_UNIT );
  }

# if 0
/*
** Draw Input And gates
*/
  na     = n / 2;

  if ( n & 1 ) gas_y   = 7 * DeltaY / ( 8 * ( na + 1 ) );
  else         gas_y   = 7 * DeltaY / ( 8 * na );

  gas_x = gs_x / 3;

  y1 = Obj->Y + Obj->DY - ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );
  y1 -= SCP_BOX_CON_STEP_Y * XSCH_UNIT / 2;

  for ( i = 0; i < na; i++ )
  {
    yc = (float)( y1 ) * XschUnitGridStep;
    yc = ( yc / XSCH_UNIT ) - XschPixelGridY;
    yc = XschGraphicDy - yc;

    XDrawLine( XschGraphicDisplay,
               XschGraphicPixmap,
               XschDrawGC, x, yc - gas_y /2, x, yc + gas_y/2);

    /* And Shape */

    XDrawLine( XschGraphicDisplay,
               XschGraphicPixmap,
               XschDrawGC,
               x, yc - gas_y/2, x + gas_x/2, yc - gas_y/2 );

    XDrawLine( XschGraphicDisplay,
               XschGraphicPixmap, 
               XschDrawGC,
               x         , yc + gas_y/2,
               x + gas_x/2, yc + gas_y/2 );

    XDrawArc( XschGraphicDisplay,
              XschGraphicPixmap,
              XschDrawGC,
              x, yc - gas_y/2, gas_x, gas_y -1, 90 * 64, -180 * 64);

    y1 -= 2 * ( SCP_BOX_CON_STEP_Y * XSCH_UNIT );
  }

  if ( n & 1 )
  {
    y1  = y1 + ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );

    yc = (float)( y1 ) * XschUnitGridStep;
    yc = ( yc / XSCH_UNIT ) - XschPixelGridY;
    yc = XschGraphicDy - yc;

    XDrawLine( XschGraphicDisplay,
               XschGraphicPixmap, 
               XschDrawGC,
               x , yc, x + gas_x, yc  );


  }
/*
** Draw Output Or gate
*/
  /* Or shape alone */
  gos_y   = 2 * DeltaY / 3;
  gos_x   = ( 2 * gs_x ) / 3;
  yc      = XschGraphicDy - Ycr;
  y       = yc + gos_y /2;
  x       = x + gas_x;

  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap,
             XschDrawGC,
             x, y, x + gos_x/2, y );

  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap, 
             XschDrawGC,
             x          , y - gos_y,
             x + gos_x/2, y - gos_y );

  XDrawArc( XschGraphicDisplay,
            XschGraphicPixmap,
            XschDrawGC,
            x, y - gos_y, gos_x, gos_y, 90 * 64, -180 * 64);

  Radius = (float)(gos_y) * 0.707106781185;

  XDrawArc( XschGraphicDisplay,
            XschGraphicPixmap,
            XschDrawGC,
            x - (Radius * 5 / 3), yc - Radius,
            Radius * 2, Radius * 2, 45 * 64, -90 * 64);
# endif
}

/*------------------------------------------------------------\
|                                                             |
|                       XschDrawOrAnd                         |
|                                                             |
\------------------------------------------------------------*/

static void XschDrawOrAnd( X1r, Y1r, X2r, Y2r, Ycr, Obj )

  long          X1r;
  long          Y1r;
  long          X2r;
  long          Y2r;
  long          Ycr;
  xschobj_list *Obj;
{
  long gs_y;
  long gs_x;
  long cs;
  int i;
  int n;
  int no;
  long gos_y;
  long gos_x;
  long gas_y;
  long gas_x;
  long Radius;

  long x, y;
  long yc;
  long y1;
  long DeltaX;
  long DeltaY;

 DeltaX = X2r - X1r;
 DeltaY = Y2r - Y1r;

 if ( DeltaX <= 0 ) DeltaX = 1;

# if 0
 if ( DeltaY <= 0 ) DeltaY = 1;

XDrawRectangle( XschGraphicDisplay,
                XschGraphicPixmap,
                XschDrawGC,
                X1r, XschGraphicDy - Y2r,
                DeltaX, DeltaY );

# endif

  n = Obj->ARG1;

  gs_x = ( 6 * DeltaX ) / 8;
  gs_y = ( 7 * DeltaY ) / 8;
  cs   = DeltaX / 8;

  yc = XschGraphicDy - Ycr;
  y  = XschGraphicDy - Ycr - gs_y/2;
  x = X1r + cs;

  if ( Obj->ARG3 == 0 )
  {
     /* Output line : centered vertically */
     XDrawLine( XschGraphicDisplay,
                XschGraphicPixmap,
                XschDrawGC,
                x + gs_x, yc, x + gs_x + cs, yc );
  }
  else 
  {
     /* Output circle */
     XDrawArc( XschGraphicDisplay,
               XschGraphicPixmap,
               XschDrawGC,
               x + gs_x, yc - cs/2, cs, cs, 0 * 64, 360 * 64);
  }

/*
** Input connectors
*/
  y1 = Obj->Y + ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );

  /* Circle for inversion */
  for (i = 0; i < n; i++)
  {
    yc = (float)( y1 ) * XschUnitGridStep;
    yc = ( yc / XSCH_UNIT ) - XschPixelGridY;
    yc = XschGraphicDy - yc;

    if (( Obj->ARG2 & (1 << i)) == 0)
    {
       XDrawLine( XschGraphicDisplay,
                  XschGraphicPixmap,
                  XschDrawGC,
                  x - cs, yc, x, yc );
    }
    else
    {
      XDrawArc( XschGraphicDisplay,
                XschGraphicPixmap,
                XschDrawGC,
                x  - cs,
                yc - cs/2,
                cs, cs, 0 * 64, 360 * 64);
    }

    y1 += ( SCP_BOX_CON_STEP_Y * XSCH_UNIT );
  }
/*
** Draw Input Or gates
*/
  no     = n / 2;

  if ( n & 1 ) gos_y   = 7 * DeltaY / ( 8 * ( no + 1 ) );
  else         gos_y   = 7 * DeltaY / ( 8 * no );

  gos_x = gs_x / 3;

  y1 = Obj->Y + Obj->DY - ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );
  y1 -= SCP_BOX_CON_STEP_Y * XSCH_UNIT / 2;

  for ( i = 0; i < no; i++ )
  {
    yc = (float)( y1 ) * XschUnitGridStep;
    yc = ( yc / XSCH_UNIT ) - XschPixelGridY;
    yc = XschGraphicDy - yc;

    /* Or shape alone */
  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap,
             XschDrawGC,
             x          , yc + gos_y/2,
             x + gos_x/2, yc + gos_y/2 );

  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap, 
             XschDrawGC,
             x          , yc - gos_y/2,
             x + gos_x/2, yc - gos_y/2 );

  XDrawArc( XschGraphicDisplay,
            XschGraphicPixmap,
            XschDrawGC,
            x, yc - gos_y/2, gos_x, gos_y-1, 90 * 64, -180 * 64);

  Radius = (float)(gos_y) * 0.707106781185;

  XDrawArc( XschGraphicDisplay,
            XschGraphicPixmap,
            XschDrawGC,
            x - (Radius * 5 / 3), yc - Radius,
            Radius * 2, Radius * 2, 45 * 64, -90 * 64);


    y1 -= 2 * ( SCP_BOX_CON_STEP_Y * XSCH_UNIT );
  }

  if ( n & 1 )
  {
    y1  = y1 + ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );

    yc = (float)( y1 ) * XschUnitGridStep;
    yc = ( yc / XSCH_UNIT ) - XschPixelGridY;
    yc = XschGraphicDy - yc;

    XDrawLine( XschGraphicDisplay,
               XschGraphicPixmap, 
               XschDrawGC,
               x , yc, x + gos_x, yc  );

  }

  /* Draw output And Gate */
  gas_y   = 2 * DeltaY / 3;
  gas_x   = ( 2 * gs_x ) / 3;
  yc      = XschGraphicDy - Ycr;
/*y       = yc + gas_y /2;*/
  x       = x + gos_x;

  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap,
             XschDrawGC, x, yc - gas_y /2, x, yc + gas_y/2);


  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap,
             XschDrawGC,
             x, yc - gas_y/2, x + gas_x/2, yc - gas_y/2 );

  XDrawLine( XschGraphicDisplay,
             XschGraphicPixmap, 
             XschDrawGC,
             x         , yc + gas_y/2,
             x + gas_x/2, yc + gas_y/2 );

  XDrawArc( XschGraphicDisplay,
            XschGraphicPixmap,
            XschDrawGC,
            x, yc - gas_y/2, gas_x, gas_y -1, 90 * 64, -180 * 64);
}

/*------------------------------------------------------------\
|                                                             |
|                     XschDisplayOneObject                    |
|                                                             |
\------------------------------------------------------------*/

void XschDisplayOneObject( Obj )

   xschobj_list *Obj;
{
  XPoint  Points[ 7 ];
  int     Index;
  long    X1r;
  long    X2r;
  long    Y1r;
  long    Y2r;
  long    Ycr;
  long    DeltaX;
  long    DeltaY;
  short   ObjType;

  if ( IsXschLineLeft( Obj ) )
  {
    X1r = Obj->X + Obj->DX;
    X2r = Obj->X;
  }
  else
  {
    X1r = Obj->X;
    X2r = Obj->X + Obj->DX;
  }

  if ( IsXschLineDown( Obj ) )
  {
    Y1r  = Obj->Y + Obj->DY;
    Y2r  = Obj->Y;
  }
  else
  {
    Y1r  = Obj->Y;
    Y2r  = Obj->Y + Obj->DY;
  }

  Ycr  = (float)( ( Y2r + Y1r) >> 1 ) * XschUnitGridStep;
  X1r  = (float)( X1r ) * XschUnitGridStep;
  Y1r  = (float)( Y1r ) * XschUnitGridStep;
  X2r  = (float)( X2r ) * XschUnitGridStep;
  Y2r  = (float)( Y2r ) * XschUnitGridStep;

  Ycr  = ( Ycr / XSCH_UNIT ) - XschPixelGridY;
  X1r  = ( X1r / XSCH_UNIT ) - XschPixelGridX;
  X2r  = ( X2r / XSCH_UNIT ) - XschPixelGridX;
  Y1r  = ( Y1r / XSCH_UNIT ) - XschPixelGridY;
  Y2r  = ( Y2r / XSCH_UNIT ) - XschPixelGridY;

  DeltaX = X2r - X1r;
  DeltaY = Y2r - Y1r;

  if ( DeltaX <= 0 ) DeltaX = 1;
  if ( DeltaY <= 0 ) DeltaY = 1;

  ObjType = GetXschObjectType( Obj ); 

  if ( ( ObjType >= XSCH_OBJECT_BUFFER ) &&
       ( ObjType <= XSCH_OBJECT_MUX    ) )
  {
    if ( ( DeltaX < 5 ) ||
         ( DeltaY < 5 ) ) ObjType = XSCH_OBJECT_RECTANGLE;
  }

  switch ( ObjType )
  {
    case XSCH_OBJECT_LINE :

      if ( Y1r == Y2r )
      {
        if ( X1r < 0                   ) X1r = 0;
        if ( X2r < 0                   ) X2r = 0;
        if ( X1r > (long)XschGraphicDx ) X1r = (long)XschGraphicDx;
        if ( X2r > (long)XschGraphicDx ) X2r = (long)XschGraphicDx;
      }
      else
      if ( X1r == X2r )
      {
        if ( Y1r < 0                   ) Y1r = 0;
        if ( Y2r < 0                   ) Y2r = 0;
        if ( Y1r > (long)XschGraphicDy ) Y1r = (long)XschGraphicDy;
        if ( Y2r > (long)XschGraphicDy ) Y2r = (long)XschGraphicDy;
      }

      XDrawLine( XschGraphicDisplay,
                 XschGraphicPixmap,
                 XschDrawGC,
                 X1r, XschGraphicDy - Y1r,
                 X2r, XschGraphicDy - Y2r );
    break;

    case XSCH_OBJECT_ARROW :

      XDrawLine( XschGraphicDisplay,
                 XschGraphicPixmap,
                 XschDrawGC,
                 X1r, XschGraphicDy - Y1r,
                 X2r, XschGraphicDy - Y2r );

      if ( XschComputeArrow( X1r, Y1r, X2r, Y2r, Points ) )
      {
        Points[ 0 ].y = XschGraphicDy - Points[ 0 ].y;
        Points[ 1 ].y = XschGraphicDy - Points[ 1 ].y;
        Points[ 2 ].y = XschGraphicDy - Points[ 2 ].y;

        XFillPolygon( XschGraphicDisplay,
                      XschGraphicPixmap, 
                      XschDrawGC, Points, 3, 
                      Convex,
                      CoordModeOrigin );
      }

    break;

    case XSCH_OBJECT_PENTAGON :

      if ( XschComputePentagon( X1r, Y1r, X2r, Y2r, Points ) )
      {
        for ( Index = 0; Index < 7; Index++ )
        {
           Points[ Index ].y = XschGraphicDy - Points[ Index ].y;
        }

        XDrawLines( XschGraphicDisplay,
                    XschGraphicPixmap,
                    XschDrawGC, Points, 7,
                    CoordModeOrigin );
      }

    break;

    case XSCH_OBJECT_CIRCLE :

     XDrawArc( XschGraphicDisplay,
               XschGraphicPixmap,
               XschDrawGC,
               X1r, XschGraphicDy - Y2r, 
               DeltaX, DeltaY, 0, 23040 );
    break;

    case XSCH_OBJECT_RECTANGLE :

      XDrawRectangle( XschGraphicDisplay,
                      XschGraphicPixmap,
                      XschDrawGC,
                      X1r, XschGraphicDy - Y2r,
                      DeltaX, DeltaY );

    break;

    case XSCH_OBJECT_TRIANGLE :

      XDrawLine( XschGraphicDisplay,
                 XschGraphicPixmap,
                 XschDrawGC,
                 X1r, XschGraphicDy - Y1r,
                 X2r, ( XschGraphicDy - Y1r - ( DeltaY >> 1 ) ) );

      XDrawLine( XschGraphicDisplay,
                 XschGraphicPixmap,
                 XschDrawGC,
                 X1r, XschGraphicDy - Y1r,
                 X1r, XschGraphicDy - Y2r );

      XDrawLine( XschGraphicDisplay,
                 XschGraphicPixmap,
                 XschDrawGC,
                 X1r, XschGraphicDy - Y2r,
                 X2r, ( XschGraphicDy - Y1r - ( DeltaY >> 1 ) ) );

    break;

    case XSCH_OBJECT_CROSS_BOX :

      XDrawLine( XschGraphicDisplay,
                 XschGraphicPixmap,
                 XschDrawGC,
                 X1r, XschGraphicDy - Y1r,
                 X2r, XschGraphicDy - Y2r );

      XDrawLine( XschGraphicDisplay,
                 XschGraphicPixmap,
                 XschDrawGC,
                 X1r, XschGraphicDy - Y2r,
                 X2r, XschGraphicDy - Y1r );

      XDrawRectangle( XschGraphicDisplay,
                      XschGraphicPixmap,
                      XschDrawGC,
                      X1r, XschGraphicDy - Y2r,
                      DeltaX, DeltaY );
    break;

    case XSCH_OBJECT_BUFFER :

      XschDrawBuffer( X1r, Y1r, X2r, Y2r, Ycr, Obj );

    break;

    case XSCH_OBJECT_AND :

     XschDrawAnd( X1r, Y1r, X2r, Y2r, Ycr, Obj );


    break;

    case XSCH_OBJECT_OR :

     XschDrawOr( X1r, Y1r, X2r, Y2r, Ycr, Obj );

    break;

    case XSCH_OBJECT_XOR :

     XschDrawXor( X1r, Y1r, X2r, Y2r, Ycr, Obj );

    break;

    case XSCH_OBJECT_TRISTATE :

      XschDrawTristate( X1r, Y1r, X2r, Y2r, Ycr, Obj );

    break;

    case XSCH_OBJECT_REGISTER :

      XschDrawRegister( X1r, Y1r, X2r, Y2r, Ycr, Obj );

    break;

    case XSCH_OBJECT_TRANSISTOR :

      XschDrawTransistor( X1r, Y1r, X2r, Y2r, Ycr, Obj );

    break;

    case XSCH_OBJECT_CONSTANT :

      XschDrawConstant( X1r, Y1r, X2r, Y2r, Ycr, Obj );

    break;

    case XSCH_OBJECT_ANDOR :

      XschDrawAndOr( X1r, Y1r, X2r, Y2r, Ycr, Obj );

    break;

    case XSCH_OBJECT_ORAND :

      XschDrawOrAnd( X1r, Y1r, X2r, Y2r, Ycr, Obj );

    break;

    case XSCH_OBJECT_MUX :

      XschDrawMux( X1r, Y1r, X2r, Y2r, Ycr, Obj );

    break;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     XschDisplayOneString                    |
|                                                             |
\------------------------------------------------------------*/

void XschDisplayOneString( Obj )

   xschobj_list *Obj;
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

  if ( IsXschLineLeft( Obj ) )
  {
    X1r = Obj->X + Obj->DX;
    X2r = Obj->X;
  }
  else
  {
    X1r = Obj->X;
    X2r = Obj->X + Obj->DX;
  }

  if ( IsXschLineDown( Obj ) )
  {
    Y1r  = Obj->Y + Obj->DY;
    Y2r  = Obj->Y;
  }
  else
  {
    Y1r  = Obj->Y;
    Y2r  = Obj->Y + Obj->DY;
  }

  X1r  = (float)( X1r ) * XschUnitGridStep;
  Y1r  = (float)( Y1r ) * XschUnitGridStep;
  X2r  = (float)( X2r ) * XschUnitGridStep;
  Y2r  = (float)( Y2r ) * XschUnitGridStep;

  X1r  = ( X1r / XSCH_UNIT ) - XschPixelGridX;
  X2r  = ( X2r / XSCH_UNIT ) - XschPixelGridX;
  Y1r  = ( Y1r / XSCH_UNIT ) - XschPixelGridY;
  Y2r  = ( Y2r / XSCH_UNIT ) - XschPixelGridY;

  DeltaX = X2r - X1r;
  DeltaY = Y2r - Y1r;

  if ( DeltaX <= 0 ) DeltaX = 1;
  if ( DeltaY <= 0 ) DeltaY = 1;

  Length     = strlen( Obj->NAME );
  WidthText  = XTextWidth( XschTextFont, Obj->NAME, Length );
  HeightText = XschTextFont->ascent;

  if ( ( IsXschSchNet( Obj ) ) &&
       ( DeltaX < WidthText  ) ) return;

  if ( Obj->TYPE == XSCH_OBJECT_TEXT_RIGHT )
  {
    XDrawString( XschGraphicDisplay,
                 XschGraphicPixmap,
                 XschTextGC,
                 X1r,
                 XschGraphicDy - (( Y1r + Y2r - HeightText ) >> 1),
                 Obj->NAME, Length );
  }
  else
  if ( Obj->TYPE == XSCH_OBJECT_TEXT_LEFT )
  {
    XDrawString( XschGraphicDisplay,
                 XschGraphicPixmap,
                 XschTextGC,
                 X2r - WidthText,
                 XschGraphicDy - (( Y1r + Y2r - HeightText ) >> 1),
                 Obj->NAME, Length );
  }
  else
  {
    XDrawString( XschGraphicDisplay,
                 XschGraphicPixmap,
                 XschTextGC,
                 ( X2r + X1r - WidthText ) >> 1,
                 XschGraphicDy - (( Y1r + Y2r - HeightText ) >> 1),
                 Obj->NAME, Length );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     XschDisplayFigure                       |
|                                                             |
\------------------------------------------------------------*/

void XschDisplayFigure( GraphicX1, GraphicY1, GraphicX2, GraphicY2 )

   long GraphicX1;
   long GraphicY1;
   long GraphicX2;
   long GraphicY2;
{
  xschview_list *View;
  xschobj_list  *Obj;
  long           X1;
  long           Y1;
  long           X2;
  long           Y2;
  int            Layer;
  char           DisplayText;

  autbegin();

  DisplayText = 1;

  XschGraphicX1 = GraphicX1;
  XschGraphicX2 = GraphicX2;
  XschGraphicY1 = GraphicY1;
  XschGraphicY2 = GraphicY2;

  if ( XschUnitGridStep > 10.0 )
  {
    XschTextGC       = XschLargeTextGC;
    XschTextFont     = XschLargeTextFont;
  }
  else
  if ( XschUnitGridStep > 6.0 )
  {
    XschTextGC       = XschMediumTextGC;
    XschTextFont     = XschMediumTextFont;
  }
  else
  if ( ( XschUnitGridStep > 3.0 ) ||
       ( XSCH_FORCE_DISPLAY     ) )
  {
    XschTextGC    = XschSmallTextGC;
    XschTextFont  = XschSmallTextFont;
  }
  else
  {
    DisplayText = 0;
  }

  XschClearGraphicWindow( GraphicX1, 
                           XschGraphicDy - GraphicY2,
                           GraphicX2 - GraphicX1,
                           GraphicY2 - GraphicY1 );

  if ( XschFigure == (xschfig_list *)NULL ) return;
  
  X1 = GraphicX1 + XschPixelGridX;
  X2 = GraphicX2 + XschPixelGridX;
  Y1 = GraphicY1 + XschPixelGridY;
  Y2 = GraphicY2 + XschPixelGridY;

  X1 = ( X1 / XschUnitGridStep );
  Y1 = ( Y1 / XschUnitGridStep );
  X2 = ( X2 / XschUnitGridStep );
  Y2 = ( Y2 / XschUnitGridStep );

  if ( X2 >= 0 ) { X2 = X2 + 1; }
  if ( Y2 >= 0 ) { Y2 = Y2 + 1; }
  if ( X1 <= 0 ) { X1 = X1 - 1; }
  if ( Y1 <= 0 ) { Y1 = Y1 - 1; }

  X1 = X1 * XSCH_UNIT;
  X2 = X2 * XSCH_UNIT;
  Y1 = Y1 * XSCH_UNIT;
  Y2 = Y2 * XSCH_UNIT;

  for ( Layer = 0; Layer < XSCH_MAX_LAYER; Layer++ )
  {
    if ( XSCH_ACTIVE_LAYER_TABLE[ Layer ] == 0 ) continue;

    XschDrawGC = XschLayerDrawGC[ Layer ];

    for ( Obj  = XschFigure->OBJECT[ Layer ];
          Obj != (xschobj_list *)NULL;
          Obj  = Obj->NEXT )
    {
      if ( XSCH_ACTIVE_LAYER_TABLE[ Obj->LAYER ] == 0 ) continue;
  
      if ( ( Obj->X           <= X2 ) &&
           ( Obj->Y           <= Y2 ) &&
           ( Obj->X + Obj->DX >= X1 ) && 
           ( Obj->Y + Obj->DY >= Y1 ) )
      {
        if ( ( IsXschAccept( Obj  ) ) ||
             ( IsXschConnect( Obj ) ) )
        {
          XschAddViewLater( Obj );
        }
        else
        {
          XschDisplayOneObject( Obj );

          if ( Obj->NAME != (char *)0 )
          {
            if ( DisplayText )
            {
              if ( ( ( IsXschSchCon( Obj )                                 ) && 
                     ( ( ( IsXschSchConExt( Obj )                        ) &&
                         ( XSCH_ACTIVE_NAME_TABLE[ XSCH_CONNECTOR_NAME ] ) ) ||
                       ( ( ! IsXschSchConExt( Obj )                      ) &&
                         ( XSCH_ACTIVE_NAME_TABLE[ XSCH_INS_CON_NAME   ] ) ) ) ) ||
                   ( ( IsXschSchBox( Obj                                   ) && 
                       XSCH_ACTIVE_NAME_TABLE[ XSCH_INSTANCE_NAME  ]     ) )     ||
                   ( ( IsXschSchNet( Obj                                   ) && 
                       XSCH_ACTIVE_NAME_TABLE[ XSCH_NET_NAME       ]     ) ) )
              {
                XschDisplayOneString( Obj );
              }
            }
          }
        }
      }
    }
  }

  for ( View  = XschHeadView;
        View != (xschview_list *)NULL;
        View  = View->NEXT )
  {
    Obj = View->OBJECT;

    if ( IsXschAccept( Obj ) )
    {
      XschDrawGC = XschAcceptDrawGC;
    } 
    else
    {
      XschDrawGC = XschConnectDrawGC;
    } 
    
    XschDisplayOneObject( Obj );

    if ( Obj->NAME != (char *)0 )
    {
      if ( DisplayText )
      {
        XschDisplayOneString( Obj );
      }
    }
  }

  XschDelView();
  XschFlushEventDisplay();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XschDisplayObject                       |
|                                                             |
\------------------------------------------------------------*/

void XschDisplayObject( Obj )

   xschobj_list *Obj;
{
  xschobj_list *ScanObj;
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

  Xmin = (float)( Xmin - XSCH_UNIT ) * XschUnitGridStep;
  Ymin = (float)( Ymin - XSCH_UNIT ) * XschUnitGridStep;
  Xmax = (float)( Xmax + XSCH_UNIT ) * XschUnitGridStep;
  Ymax = (float)( Ymax + XSCH_UNIT ) * XschUnitGridStep;

  Xmin = ( Xmin / XSCH_UNIT ) - XschPixelGridX - 1;
  Xmax = ( Xmax / XSCH_UNIT ) - XschPixelGridX + 1;
  Ymin = ( Ymin / XSCH_UNIT ) - XschPixelGridY - 1;
  Ymax = ( Ymax / XSCH_UNIT ) - XschPixelGridY + 1;

  if ( Xmin < 0 ) Xmin = 0;
  if ( Ymin < 0 ) Ymin = 0;

  if ( Xmax > XschGraphicDx ) Xmax = XschGraphicDx;
  if ( Ymax > XschGraphicDy ) Ymax = XschGraphicDy;

  if ( ( Xmax > 0              ) &&
       ( Ymax > 0              ) &&
       ( Xmin < XschGraphicDx ) &&
       ( Ymin < XschGraphicDy ) )
  {
    XschDisplayFigure( Xmin, Ymin , Xmax, Ymax );

    XschRefreshGraphicWindow( Xmin, XschGraphicDy - Ymax, 
                              Xmax - Xmin, Ymax - Ymin );
  }

  autend();
}
