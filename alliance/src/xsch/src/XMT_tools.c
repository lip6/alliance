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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
| Tool    :                   XSCH                            |
|                                                             |
| File    :                  Tools.c                          |
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
# include <Xm/Xm.h>
# include <Xm/PushBG.h>

# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "scl.h"
# include "XSB.h"
# include "XTB.h"
# include "XMX.h"
# include "XSC.h"
# include "XMT.h"

# include "XMT_tools.h"
# include "XMT_message.h"

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

   xschhierarchy *XschHeadHierarchy = (xschhierarchy *)0;

   char XschToolsFilter[ 10 ]    = "*.";
   char XschToolsExtention[ 10 ] = ".";

   char XschToolsBuffer[ 128 ];
   char XschToolsDirectoryBuffer[ 512 ];

   static FILE *XschXfigFile  = (FILE *)0;
   static int   XschXfigColor = 0;
   static int   XschXfigDepth = 0;
   static long  XschXfigDy    = 0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       XschAllocHierarchy                   |
|                                                             |
\------------------------------------------------------------*/

xschhierarchy *XschAllocHierarchy ()

{
  return ( (xschhierarchy *)autallocheap( sizeof ( xschhierarchy ) ));
}

/*------------------------------------------------------------\
|                                                             |
|                       XschFreeHierarchy                    |
|                                                             |
\------------------------------------------------------------*/

void XschFreeHierarchy ( FreeHierarchy )

  xschhierarchy *FreeHierarchy;

{
  autfreeheap( (char *)FreeHierarchy, sizeof(FreeHierarchy) );
}

/*------------------------------------------------------------\
|                                                             |
|                       XschAddHierarchy                     |
|                                                             |
\------------------------------------------------------------*/
 
void XschAddHierarchy( Name )
 
  char *Name;
{
  xschhierarchy *XschHierarchy;

  autbegin();
 
  XschHierarchy       = XschAllocHierarchy ();
  XschHierarchy->NAME = Name;
  XschHierarchy->X    = XschUnitGridX;
  XschHierarchy->Y    = XschUnitGridY;
  XschHierarchy->DX   = XschUnitGridDx;
  XschHierarchy->DY   = XschUnitGridDy;
  XschHierarchy->NEXT = XschHeadHierarchy;
  XschHeadHierarchy   = XschHierarchy;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         XschDelHierarchy                   |
|                                                             |
\------------------------------------------------------------*/

char XschDelHierarchy()

{
  xschhierarchy *XschHierarchy;

  if ( XschHeadHierarchy != (xschhierarchy *) NULL )
  {
    autbegin();

    XschHierarchy     = XschHeadHierarchy;
    XschHeadHierarchy = XschHeadHierarchy->NEXT;
    XschFreeHierarchy( XschHierarchy );

    autend();
    return( XSCH_TRUE ); 
  }

  return( XSCH_FALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                      XschToolsHierarchyDown                 |
|                                                             |
\------------------------------------------------------------*/

void XschToolsHierarchyDown()
{
  xschselect_list *Select;
  xschobj_list    *Obj;
  schbox_list     *SchBox;
  loins_list      *LoIns;
  char            *ModelName;

  autbegin();

  if ( XschHeadSelect != (xschselect_list *)0 )
  {
    Obj = XschHeadSelect->OBJECT;
    XschDelSelect();

    XschDisplayObject( Obj );

    if ( IsXschSchBox( Obj ) )
    {
      SchBox = (schbox_list *)Obj->USER;

      if ( SchBox->SOURCE_TYPE == SCH_SOURCE_LOINS )
      {
        LoIns = (loins_list *)SchBox->SOURCE;
        ModelName = LoIns->FIGNAME;

        XschAddHierarchy( XschFigureSch->NAME );

        XschFileOpen( ModelName, 1 );
      }
    }
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                     XschToolsHierarchyUp                   |
|                                                             |
\------------------------------------------------------------*/

void XschToolsHierarchyUp()
{
  if ( XschHeadHierarchy == (xschhierarchy *)0 )
  {
    XschErrorMessage( XschMainWindow, "No previous model !" ); 
  }
  else
  {
    XschFileOpen( XschHeadHierarchy->NAME, 0 );

    XschUnitGridX  = XschHeadHierarchy->X;
    XschUnitGridY  = XschHeadHierarchy->Y;
    XschUnitGridDx = XschHeadHierarchy->DX;
    XschUnitGridDy = XschHeadHierarchy->DY;

    XschComputeUnitGrid();
    XschComputeAndDisplayMap();

    XschZoomRefresh();
    XschDelHierarchy();
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      XschToolsHierarchyReload               |
|                                                             |
\------------------------------------------------------------*/

void XschToolsHierarchyReload()
{
  long SaveUnitGridX;
  long SaveUnitGridY;
  long SaveUnitGridDx;
  long SaveUnitGridDy;

  if ( XschFigureSch != (schfig_list *)NULL )
  {
    SaveUnitGridX  = XschUnitGridX;
    SaveUnitGridY  = XschUnitGridY;
    SaveUnitGridDx = XschUnitGridDx;
    SaveUnitGridDy = XschUnitGridDy;

    XschFileOpen( XschFigureSch->NAME, 0 );

    XschUnitGridX  = SaveUnitGridX;
    XschUnitGridY  = SaveUnitGridY;
    XschUnitGridDx = SaveUnitGridDx;
    XschUnitGridDy = SaveUnitGridDy;

    XschComputeUnitGrid();
    XschComputeAndDisplayMap();

    XschZoomRefresh();
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      XschToolsDelAllHierarchy               |
|                                                             |
\------------------------------------------------------------*/

void XschToolsDelAllHierarchy()
{
  while ( XschHeadHierarchy != (xschhierarchy *)0 )
  {
    XschDelHierarchy();
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      XschToolsSaveXfigLine                  |
|                                                             |
\------------------------------------------------------------*/

static void XschToolsSaveXfigLine( X1r, Y1r, X2r, Y2r )

  long          X1r;
  long          Y1r;
  long          X2r;
  long          Y2r;
{
  fprintf( XschXfigFile,
           "2 1 0 1 %d 0 %d  0 -1 0.000 0 0 -1 0 0 2\n", XschXfigColor, XschXfigDepth );
  /*        ^ ^ ^ ^ ^  ^ ^   ^  ^ ^     ^ ^  ^ ^ ^ ^
   *        | | | | |  | |   |  | |     | |  | | | + Number Points
   *        | | | | |  | |   |  | |     | |  | | + Backward Arrow
   *        | | | | |  | |   |  | |     | |  | + Forward Arrow
   *        | | | | |  | |   |  | |     | |  + Radius
   *        | | | | |  | |   |  | |     | + Cap style
   *        | | | | |  | |   |  | |     + Join style
   *        | | | | |  | |   |  | + Style Val
   *        | | | | |  | |   |  + Area Fill
   *        | | | | |  | |   + Pen Style
   *        | | | | |  | + Depth
   *        | | | | |  + Fill Color
   *        | | | | + Pen Color
   *        | | | + Width
   *        | | + Line Style
   *        | + Poly Line
   *        + Poly Line
   */

  fprintf( XschXfigFile, "  %ld %ld %ld %dl\n", X1r, XschXfigDy - Y1r,
                                                X2r, XschXfigDy - Y2r );
}

/*------------------------------------------------------------\
|                                                             |
|                      XschToolsSaveXfigRectangle             |
|                                                             |
\------------------------------------------------------------*/

static void XschToolsSaveXfigRectangle( X1r, Y1r, X2r, Y2r )

  long          X1r;
  long          Y1r;
  long          X2r;
  long          Y2r;
{
  fprintf( XschXfigFile,
           "2 2 0 1 %d 0 %d  0 -1 0.000 0 0 -1 0 0 5\n", XschXfigColor, XschXfigDepth );
  /*        ^ ^ ^ ^ ^  ^ ^   ^  ^ ^     ^ ^  ^ ^ ^ ^
   *        | | | | |  | |   |  | |     | |  | | | + Number Points
   *        | | | | |  | |   |  | |     | |  | | + Backward Arrow
   *        | | | | |  | |   |  | |     | |  | + Forward Arrow
   *        | | | | |  | |   |  | |     | |  + Radius
   *        | | | | |  | |   |  | |     | + Cap style
   *        | | | | |  | |   |  | |     + Join style
   *        | | | | |  | |   |  | + Style Val
   *        | | | | |  | |   |  + Area Fill
   *        | | | | |  | |   + Pen Style
   *        | | | | |  | + Depth
   *        | | | | |  + Fill Color
   *        | | | | + Pen Color
   *        | | | + Width
   *        | | + Line Style
   *        | + Poly Line
   *        + Poly Line
   */

  fprintf( XschXfigFile, "%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",
           X1r, XschXfigDy - Y1r,
           X2r, XschXfigDy - Y1r,
           X2r, XschXfigDy - Y2r,
           X1r, XschXfigDy - Y2r,
           X1r, XschXfigDy - Y1r );
}

/*------------------------------------------------------------\
|                                                             |
|                      XschToolsSaveXfigCircle                |
|                                                             |
\------------------------------------------------------------*/

static void XschToolsSaveXfigCircle( X1r, Y1r, Dx, Dy )

  long          X1r;
  long          Y1r;
  long          Dx;
  long          Dy;
{
  long Xc;
  long Yc;
  long Radx;
  long Rady;

  Xc   = X1r + ( Dx / 2 );
  Yc   = Y1r + ( Dy / 2 );
  Radx = Dx / 2;
  Rady = Dy / 2;

  fprintf( XschXfigFile,
           "1 3 0 1 %d 0 %d  0 -1 0.000 1 0.000 %d %d %d %d %d %d %d %d\n",
  /*        ^ ^ ^ ^ ^  ^ ^   ^  ^ ^     ^ ^     ^  ^  ^  ^  ^  ^  ^  ^
   *        | | | | |  | |   |  | |     | |     |  |  |  |  |  |  |  + end_y
   *        | | | | |  | |   |  | |     | |     |  |  |  |  |  |  + end_x
   *        | | | | |  | |   |  | |     | |     |  |  |  |  |  + start_y
   *        | | | | |  | |   |  | |     | |     |  |  |  |  + start_x
   *        | | | | |  | |   |  | |     | |     |  |  |  + rad_y
   *        | | | | |  | |   |  | |     | |     |  |  + rad_x
   *        | | | | |  | |   |  | |     | |     |  + Cy
   *        | | | | |  | |   |  | |     | |     + Cx
   *        | | | | |  | |   |  | |     | + Angle
   *        | | | | |  | |   |  | |     + Direction
   *        | | | | |  | |   |  | + Style Val
   *        | | | | |  | |   |  + Area Fill
   *        | | | | |  | |   + Pen Style
   *        | | | | |  | + Depth
   *        | | | | |  + Fill Color
   *        | | | | + Pen Color
   *        | | | + Width
   *        | | + Line Style
   *        | + Poly Line
   *        + Poly Line
   */
           XschXfigColor, XschXfigDepth, Xc, XschXfigDy - Yc, Radx, Rady,
                          Xc, XschXfigDy - Yc,
                          Xc + Radx, XschXfigDy - Yc );
}

/*------------------------------------------------------------\
|                                                             |
|                      XschToolsSaveXfigArc                   |
|                                                             |
\------------------------------------------------------------*/

# if 0 
static void XschToolsSaveXfigArc( X1r, Y1r, Dx, Dy, AngleFrom, AngleTo )

  long          X1r;
  long          Y1r;
  long          Dx;
  long          Dy;
  long          AngleFrom;
  long          AngleTo;
{
  long   MiddleAngle;
  long   Xc;
  long   Yc;
  long   Width;
  long   Height;
  long   X1;
  long   Y1;
  long   X2;
  long   Y2;
  long   X3;
  long   Y3;
  double Rad;

  Width  = Dx / 2;
  Height = Dy / 2;
  Xc     = X1r + Width;
  Yc     = Y1r + Height;

  Rad = ( (double) AngleFrom * M_PI ) / ( 64.0 * 180.0 );

  X1 = Xc + (long)( (double)Width  * cos( Rad ) );
  Y1 = Yc + (long)( (double)Height * sin( Rad ) );

  Rad = ( (double) ( AngleTo + AngleFrom ) * M_PI ) / ( 64.0 * 180.0 );

  X3 = Xc + (long)( (double)Width  * cos( Rad ) );
  Y3 = Yc + (long)( (double)Height * sin( Rad ) );

  Rad = ( (double)( AngleTo + ( AngleFrom / 2 ) ) * M_PI ) / ( 2.0 * 64.0 * 180.0 );

  X2 = Xc + (long)( (double)Width  * cos( Rad ) );
  Y2 = Yc + (long)( (double)Height * sin( Rad ) );

  fprintf( XschXfigFile,
           "5 1 0 1 %d 0 %d  0 -1 0.000 0 1 0 0 %f %f %ld %ld %ld %ld %ld %ld\n",
  /*        ^ ^ ^ ^ ^  ^ ^   ^  ^ ^     ^ ^ ^ ^ ^     ^       ^        ^ 
   *        | | | | |  | |   |  | |     | | | | |     |       + Second  Third
   *        | | | | |  | |   |  | |     | | | | |     + 1st Point
   *        | | | | |  | |   |  | |     | | | | + Center
   *        | | | | |  | |   |  | |     | | | + Backward Arrow
   *        | | | | |  | |   |  | |     | | + Forward Arrow
   *        | | | | |  | |   |  | |     | + Direction
   *        | | | | |  | |   |  | |     + Cap style
   *        | | | | |  | |   |  | + Style Val
   *        | | | | |  | |   |  + Area Fill
   *        | | | | |  | |   + Pen Style
   *        | | | | |  | + Depth
   *        | | | | |  + Fill Color
   *        | | | | + Pen Color
   *        | | | + Width
   *        | | + Line Style
   *        | + Arc
   *        + Arc
   */
           XschXfigColor, XschXfigDepth,
           (float)Xc, (float)Yc, X1, Y1, X2, Y2, X3, Y3 );
}
# endif

static void XschToolsSaveXfigArc( X1r, Y1r, Dx, Dy, AngleFrom, AngleTo )

  long          X1r;
  long          Y1r;
  long          Dx;
  long          Dy;
  long          AngleFrom;
  long          AngleTo;
{
  long   MiddleAngle;
  long   Xc;
  long   Yc;
  long   Width;
  long   Height;
  long   X1;
  long   Y1;
  long   X2;
  long   Y2;
  long   X3;
  long   Y3;
  double Rad;

  Width  = Dx / 2;
  Height = Dy / 2;
  Xc     = X1r + Width;
  Yc     = Y1r + Height;

  Rad = ( (double) AngleFrom * M_PI ) / ( 64.0 * 180.0 );

  X1 = Xc + (long)( (double)Width  * cos( Rad ) );
  Y1 = Yc + (long)( (double)Height * sin( Rad ) );

  Rad = ( (double) ( AngleFrom + AngleTo ) * M_PI ) / ( 64.0 * 180.0 );

  X3 = Xc + (long)( (double)Width  * cos( Rad ) );
  Y3 = Yc + (long)( (double)Height * sin( Rad ) );

  Rad = ( (double)( AngleFrom + ( AngleTo / 2 ) ) * M_PI ) / ( 2.0 * 64.0 * 180.0 );

  X2 = Xc + (long)( (double)Width  * cos( Rad ) );
  Y2 = Yc + (long)( (double)Height * sin( Rad ) );

  fprintf( XschXfigFile,
           "3 4 0 1 %d 0 %d  0 -1 0.000 0 0 0 3\n", XschXfigColor, XschXfigDepth );
  /*        ^ ^ ^ ^ ^  ^ ^   ^  ^ ^     ^ ^ ^ ^
   *        | | | | |  | |   |  | |     | | | |
   *        | | | | |  | |   |  | |     | | | |
   *        | | | | |  | |   |  | |     | | | + Number points
   *        | | | | |  | |   |  | |     | | + Backward Arrow
   *        | | | | |  | |   |  | |     | + Forward Arrow
   *        | | | | |  | |   |  | |     + Cap style
   *        | | | | |  | |   |  | + Style Val
   *        | | | | |  | |   |  + Area Fill
   *        | | | | |  | |   + Pen Style
   *        | | | | |  | + Depth
   *        | | | | |  + Fill Color
   *        | | | | + Pen Color
   *        | | | + Width
   *        | | + Line Style
   *        | + Arc
   *        + Arc
   */

  fprintf( XschXfigFile, "  %ld %ld %ld %ld %ld %ld\n",
           X1, XschXfigDy - Y1,
           X2, XschXfigDy - Y2,
           X3, XschXfigDy - Y3 );

  fprintf( XschXfigFile, "  0.000 -1.000 0.000\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                 XschToolsSaveXfigBuffer                     |
|                                                             |
\------------------------------------------------------------*/

static void XschToolsSaveXfigBuffer( X1r, Y1r, X2r, Y2r, Ycr, Obj )

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
  long gs;
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

  gs = DeltaX;
  is_x = ( 5 * DeltaX ) / 16;
  is_y = ( 5 * DeltaY ) / 8;
  cs = DeltaX / 8;

  y1 = Ycr;
  x  = X1r + cs;
 
  XschToolsSaveXfigLine( x, y1 - is_y/2, x, y1 + is_y/2);
  XschToolsSaveXfigLine( x       , y1 - is_y/2, x + is_x, y1);
  XschToolsSaveXfigLine( x       , y1 + is_y/2, x + is_x, y1);

  if ( ( Obj->ARG1 ) == 0 )
  {
     /* input line : centered vertically */
     XschToolsSaveXfigLine( x - cs, y1, x     , y1 );
  }
  else
  {
     /* Input circle */
     XschToolsSaveXfigCircle( x - cs, y1 - cs/2, cs, cs );
  }
 
  if ( ( Obj->ARG2 ) == 0 )
  {
     /* Output line : centered vertically */
     XschToolsSaveXfigLine( x + is_x     , y1, x + is_x + cs, y1 );
  }
  else
  {
     /* Output circle */
     XschToolsSaveXfigCircle( x + is_x, y1 - cs/2, cs, cs );
  }

  XschToolsSaveXfigLine( x + is_x + cs, y1, X2r, y1 );
}


/*------------------------------------------------------------\
|                                                             |
|                   XschToolsSaveXfigTristate                 |
|                                                             |
\------------------------------------------------------------*/

static void XschToolsSaveXfigTristate( X1r, Y1r, X2r, Y2r, Ycr, Obj )

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
  long gs;
  long is_x;
  long is_y;
  long cs;
  long csm;
  long x, x1, y1, y2,y0;
  long DeltaX;
  long DeltaY;

  DeltaX = X2r - X1r;
  DeltaY = Y2r - Y1r;

  if ( DeltaX <= 0 ) DeltaX = 1;
  if ( DeltaY <= 0 ) DeltaY = 1;

  gs = DeltaX;
  is_x = ( 5 * DeltaX ) / 16;
  is_y = ( 5 * DeltaY ) / 16;
  cs = DeltaX / 8;
  csm = DeltaX / 10;

  y1 = Ycr;
  x1 = (X2r + X1r) / 2;
  x  = X1r + cs;
 
  XschToolsSaveXfigLine( x, y1 - is_y/2, x, y1 + is_y/2);
 
  XschToolsSaveXfigLine( x       , y1 - is_y/2, x + is_x, y1);
  XschToolsSaveXfigLine( x       , y1 + is_y/2, x + is_x, y1);

  y0 = Obj->Y + ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );
  y0 -= XschBoundYmin;

  y2 = Obj->Y + ( SCP_BOX_CON_BASE_Y + SCP_BOX_CON_STEP_Y ) * XSCH_UNIT;
  y2 -= XschBoundYmin;

  if ( ( Obj->ARG1 & 1) == 0 )
  {
     /* data line : centered vertically */
     XschToolsSaveXfigLine( x - cs, y1, x     , y1 );
  }
  else
  {
     /* Data circle */
     XschToolsSaveXfigCircle( x - cs, y1 - cs/2, cs, cs );
  }

  /* Data Line */

  XschToolsSaveXfigLine( x - cs, y0, x - cs, y1  );

  if ( ( Obj->ARG1 & 2) == 0 )
  {
    /* Command Line */
    XschToolsSaveXfigLine( x - cs, y2, x + is_x/2, y2 );

    XschToolsSaveXfigLine( x + is_x/2, y2, x + is_x/2, y1 - is_y/4 );
  }
  else
  {
    /* Command circle */
    XschToolsSaveXfigCircle( x + is_x/2 - csm/2, y1 - is_y/2 - csm/4, csm, csm );

    /* Command Line */
    XschToolsSaveXfigLine( x - cs, y2, x + is_x/2, y2 );

    XschToolsSaveXfigLine( x + is_x/2, y2, x + is_x/2, y2 + csm );
  }
 
  if ( ( Obj->ARG2 ) == 0 )
  {
     /* Output line : centered vertically */
     XschToolsSaveXfigLine( x + is_x     , y1, x + is_x + cs, y1 );
  }
  else
  {
     /* Output circle */
     XschToolsSaveXfigCircle( x + is_x, y1 - cs/2, cs, cs );
  }

  XschToolsSaveXfigLine( x + is_x + cs, y1, X2r, y1 );
}

/*------------------------------------------------------------\
|                                                             |
|                 XschToolsSaveXfigConstant                   |
|                                                             |
\------------------------------------------------------------*/

static void XschToolsSaveXfigConstant( X1r, Y1r, X2r, Y2r, Ycr, Obj )

  long          X1r;
  long          Y1r;
  long          X2r;
  long          Y2r;
  long          Ycr;
  xschobj_list *Obj;
{
 long is_x;
 long is_y;
 long cs;
 long x, yc, y1,y2,y3;
 long DeltaX;
 long DeltaY;
 int  i;
 int  n;
 
 n = Obj->ARG1;

 DeltaX = X2r - X1r;
 DeltaY = Y2r - Y1r;

 if ( DeltaX <= 0 ) DeltaX = 1;
 if ( DeltaY <= 0 ) DeltaY = 1;

  cs = DeltaX / 8;
  x  = X1r + cs;

  y1 = Obj->Y + ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );
  y1 -= XschBoundYmin;

  for ( i = 0; i < n; i++ )
  {
    yc = y1;

    if (( Obj->ARG2 & (1 << i)) == 0)
    {
      y2 = yc + cs;
      y3 = y2 + cs;
  
      XschToolsSaveXfigLine( 
                 x, yc, x, y2 );
  
      XschToolsSaveXfigLine( x - cs, y2, x + cs, y2 );
  
      XschToolsSaveXfigLine( x - cs, y2, x, y3 );
  
      XschToolsSaveXfigLine( x + cs, y2, x, y3 );
    }
    else
    {
      y2 = yc - cs;
      y3 = y2 - cs;
  
      XschToolsSaveXfigLine(
                 x, yc, x, y2 );
  
      XschToolsSaveXfigLine( 
                 x, y2, x - cs, y3 );
  
      XschToolsSaveXfigLine(
                 x, y2, x + cs, y3 );
    }

    XschToolsSaveXfigLine( x, yc, X2r, yc );

    y1 += ( SCP_BOX_CON_STEP_Y * XSCH_UNIT );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                  XschToolsSaveXfigTransistor                |
|                                                             |
\------------------------------------------------------------*/

static void XschToolsSaveXfigTransistor( X1r, Y1r, X2r, Y2r, Ycr, Obj )

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
  long gs;
  long is_x;
  long is_y;
  long cs;
  long csm;
  long x, x1, y1, y2,y0;
  long DeltaX;
  long DeltaY;

  DeltaX = X2r - X1r;
  DeltaY = Y2r - Y1r;

  if ( DeltaX <= 0 ) DeltaX = 1;
  if ( DeltaY <= 0 ) DeltaY = 1;

  gs = DeltaX;
  is_x = ( 5 * DeltaX ) / 16;
  is_y = ( 5 * DeltaY ) / 16;
  cs = DeltaX / 8;

  y0 = Obj->Y + ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );
  y0 -= XschBoundYmin;

  y2 = Obj->Y + ( SCP_BOX_CON_BASE_Y + SCP_BOX_CON_STEP_Y ) * XSCH_UNIT;
  y2 -= XschBoundYmin;

  y1 = Ycr;
  /*
  x1 = X2r - cs;
  */
  x1 = X1r + 3 * cs;
  x  = X1r;

  XschToolsSaveXfigLine( x, y0, x, y0 - cs );

  XschToolsSaveXfigLine( x, y0 - cs, x1 - cs, y0 - cs );

  XschToolsSaveXfigLine( x1 - cs, y0 - cs, x1 - cs, y2 + cs );

  XschToolsSaveXfigLine( x1 - cs, y2 + cs, x, y2 + cs );

  XschToolsSaveXfigLine( x, y2 + cs, x, y2 );

  XschToolsSaveXfigLine( x1, y0 - cs, x1, y2 + cs );

  if ( ( Obj->ARG1 ) == 0 )
  {
     /* Output line : centered vertically */
     XschToolsSaveXfigLine( x1, y1, x1 + cs, y1 );
  }
  else
  {
     /* Output circle */
     XschToolsSaveXfigCircle( x1, y1 - cs/2, cs, cs );
  }

  XschToolsSaveXfigLine( x1 + cs, y1, X2r, y1 );
}

/*------------------------------------------------------------\
|                                                             |
|                   XschToolsSaveXfigAnd                      |
|                                                             |
\------------------------------------------------------------*/

static void XschToolsSaveXfigAnd( X1r, Y1r, X2r, Y2r, Ycr, Obj )

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

  gs_y = ( 7 * DeltaY ) / 8;
  gs_x = ( 6 * DeltaX ) / 8;
  cs   = DeltaX / 8;

  yc = Ycr;
  y  = Ycr - gs_y/2;
  x = X1r + cs;
  n = Obj->ARG1;

  /* And shape alone */

  XschToolsSaveXfigLine( x, y, x + gs_x/2, y );

  XschToolsSaveXfigLine( x         , y + gs_y, x + gs_x/2, y + gs_y );

  XschToolsSaveXfigArc( x, y, gs_x, gs_y, 90 * 64, -180 * 64);

  if ( Obj->ARG3 == 0 )
  {
     /* Output line : centered vertically */
     XschToolsSaveXfigLine( x + gs_x, yc, x + gs_x + cs, yc );
  }
  else 
  {
     /* Output circle (for a NAND) */
     XschToolsSaveXfigCircle( x + gs_x, yc - cs/2, cs, cs );
  }

  /* Input lines */
  XschToolsSaveXfigLine( x, y, x, y + gs_y);

  y1 = Obj->Y + ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );
  y1 -= XschBoundYmin;

  /* Circle for inversion */
  for (i = 0; i < n; i++)
  {
    yc = y1;

    if (( Obj->ARG2 & (1 << i)) == 0)
    {
       XschToolsSaveXfigLine( x - cs, yc, x, yc );
    }
    else
    {
      XschToolsSaveXfigCircle( x  - cs, yc - cs/2, cs, cs );
    }

    y1 += ( SCP_BOX_CON_STEP_Y * XSCH_UNIT );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    XschToolsSaveXfigRegister                |
|                                                             |
\------------------------------------------------------------*/

static void XschToolsSaveXfigRegister( X1r, Y1r, X2r, Y2r, Ycr, Obj )

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

  gs_y = ( 7 * DeltaY ) / 8;
  gs_x = ( 6 * DeltaX ) / 8;
  cs   = DeltaX / 8;

  es_y = DeltaX / 4;
  es_x = DeltaX / 8;

  yc = Ycr;
  y  = Ycr - gs_y/2;
  x = X1r + cs;
  xc = (X2r + X1r) /2;
  ni = Obj->ARG1;

  /* shape alone */

  XschToolsSaveXfigRectangle( x, y, x + gs_x, y + gs_y );

/*
** Rising/Falling Edge
*/
  if ( ( Obj->ARG5 == 1 ) ||
       ( Obj->ARG5 == 2 ) )
  {
    XschToolsSaveXfigLine( xc, yc - es_y, xc, yc + es_y );

/*
** Rising
*/
    if ( Obj->ARG5 == 1 )
    {
      XschToolsSaveXfigLine( xc, yc + es_y, xc - es_x, yc + es_y );

      XschToolsSaveXfigLine( xc, yc - es_y, xc + es_x, yc - es_y );
    }
    else
/*
** Falling
*/
    {
      XschToolsSaveXfigLine( xc, yc - es_y, xc - es_x, yc - es_y );

      XschToolsSaveXfigLine( xc, yc + es_y, xc + es_x, yc + es_y );
    }
  }

  if ( Obj->ARG4 == 0 )
  {
     /* Output line : centered vertically */
     XschToolsSaveXfigLine( x + gs_x, yc, x + gs_x + cs, yc );
  }
  else 
  {
     /* Output circle */
     XschToolsSaveXfigCircle( x + gs_x, yc - cs/2, cs, cs );
  }


  /* Input lines */
  /* Circle for inversion */

  y1 = Obj->Y + ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );
  y1 -= XschBoundYmin;

  for (i = 0; i < ni; i++)
  {
    yc = y1;

    if (( Obj->ARG2 & (1 << i)) == 0)
    {
       XschToolsSaveXfigLine( x - cs, yc, x, yc );
    }
    else
    {
      XschToolsSaveXfigCircle( x  - cs, yc - cs/2, cs, cs );
    }

    /* Clock */

    if (( Obj->ARG3 & (1 << i)) != 0)
    {
       XschToolsSaveXfigLine( x, yc - cs/2, x + cs, yc  );

       XschToolsSaveXfigLine( x + cs, yc, x, yc + cs/2  );
    }

    y1 += ( SCP_BOX_CON_STEP_Y * XSCH_UNIT );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       XschToolsSaveXfigOr                   |
|                                                             |
\------------------------------------------------------------*/

static void XschToolsSaveXfigOr( X1r, Y1r, X2r, Y2r, Ycr, Obj )

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

  gs_y = ( 7 * DeltaY ) / 8;
  gs_x = ( 6 * DeltaX ) / 8;
  cs   = DeltaX / 8;

  yc = Ycr;
  y  = Ycr - gs_y/2;
  x = X1r + cs;
  n = Obj->ARG1;

  /* Or shape alone */

  XschToolsSaveXfigLine( x, y, x + gs_x/2, y );

  XschToolsSaveXfigLine( x         , y + gs_y, x + gs_x/2, y + gs_y );

  XschToolsSaveXfigArc( x, y, gs_x, gs_y, 90 * 64, -180 * 64);

  Radius = (float)(gs_y) * 0.707106781185;

  XschToolsSaveXfigArc( 
            x - (Radius * 5 / 3), yc - Radius,
            Radius * 2, Radius * 2, 45 * 64, -90 * 64);

  if ( Obj->ARG3 == 0 )
  {
     /* Output line : centered vertically */
     XschToolsSaveXfigLine( x + gs_x, yc, x + gs_x + cs, yc );
  }
  else 
  {
     /* Output circle */
     XschToolsSaveXfigCircle( x + gs_x, yc - cs/2, cs, cs );
  }

  /* Input lines */

  y1 = Obj->Y + ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );
  y1 -= XschBoundYmin;

  /* Circle for inversion */
  for (i = 0; i < n; i++)
  {
    yc = y1;

    if (( Obj->ARG2 & (1 << i)) == 0)
    {
       XschToolsSaveXfigLine( x - cs, yc, x, yc );
    }
    else
    {
      XschToolsSaveXfigCircle( x  - cs, yc - cs/2, cs, cs );
    }

    y1 += ( SCP_BOX_CON_STEP_Y * XSCH_UNIT );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       XschToolsSaveXfigXor                  |
|                                                             |
\------------------------------------------------------------*/

static void XschToolsSaveXfigXor( X1r, Y1r, X2r, Y2r, Ycr, Obj )

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

  gs_y = ( 7 * DeltaY ) / 8;
  gs_x = ( 6 * DeltaX ) / 8;
  cs   = DeltaX / 8;

  yc = Ycr;
  y  = Ycr - gs_y/2;
  x = X1r + cs;
  n = Obj->ARG1;

  /* Xor shape alone */

  XschToolsSaveXfigLine( x + gs_x/4, y, x + gs_x/2, y );

  XschToolsSaveXfigLine( x + gs_x/4, y + gs_y, x + gs_x/2, y + gs_y );

  XschToolsSaveXfigArc( x, y, gs_x, gs_y, 90 * 64, -180 * 64);

  Radius = (float)(gs_y) * 0.707106781185; /* sqrt(2) */

  XschToolsSaveXfigArc( x - (Radius * 5 / 3), yc - Radius,
            Radius * 2, Radius * 2, 45 * 64, -90 * 64);

  XschToolsSaveXfigArc( x - (Radius * 4 / 3), yc - Radius,
            Radius * 2, Radius * 2, 45 * 64, -90 * 64);

  if ( Obj->ARG3 == 0 )
  {
     /* Output line : centered vertically */
     XschToolsSaveXfigLine( x + gs_x, yc, x + gs_x + cs, yc );
  }
  else 
  {
     /* Output circle */
     XschToolsSaveXfigCircle( x + gs_x, yc - cs/2, cs, cs );
  }

  /* Input lines */

  y1 = Obj->Y + ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );
  y1 -= XschBoundYmin;

  /* Circle for inversion */
  for (i = 0; i < n; i++)
  {
     yc = y1;

     if (( Obj->ARG2 & (1 << i)) == 0)
     {
        XschToolsSaveXfigLine( x - cs, yc, x, yc );
     }
     else
     {
       XschToolsSaveXfigCircle( x  - cs, yc - cs/2, cs, cs );
     }

     y1 += ( SCP_BOX_CON_STEP_Y * XSCH_UNIT );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    XschToolsSaveXfigAndOr                   |
|                                                             |
\------------------------------------------------------------*/

static void XschToolsSaveXfigAndOr( X1r, Y1r, X2r, Y2r, Ycr, Obj )

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

  n = Obj->ARG1;

  gs_y = ( 7 * DeltaY ) / 8;
  gs_x = ( 6 * DeltaX ) / 8;
  cs   = DeltaX / 8;

  yc = Ycr;
  y  = Ycr - gs_y/2;
  x = X1r + cs;

  if ( Obj->ARG3 == 0 )
  {
     /* Output line : centered vertically */
     XschToolsSaveXfigLine( x + gs_x, yc, x + gs_x + cs, yc );
  }
  else 
  {
     /* Output circle */
     XschToolsSaveXfigCircle( x + gs_x, yc - cs/2, cs, cs );
  }

/*
** Input connectors
*/
  y1 = Obj->Y + ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );
  y1 -= XschBoundYmin;

  /* Circle for inversion */
  for (i = 0; i < n; i++)
  {
    yc = y1;

    if (( Obj->ARG2 & (1 << i)) == 0)
    {
       XschToolsSaveXfigLine( x - cs, yc, x, yc );
    }
    else
    {
      XschToolsSaveXfigCircle( x  - cs, yc - cs/2, cs, cs );
    }

    y1 += ( SCP_BOX_CON_STEP_Y * XSCH_UNIT );
  }
/*
** ToolsSaveXfig Input And gates
*/
  na     = n / 2;

  if ( n & 1 ) gas_y   = 7 * DeltaY / ( 8 * ( na + 1 ) );
  else         gas_y   = 7 * DeltaY / ( 8 * na );

  gas_x = gs_x / 3;

  y1 = Obj->Y + Obj->DY - ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );
  y1 -= XschBoundYmin;
  y1 -= SCP_BOX_CON_STEP_Y * XSCH_UNIT / 2;

  for ( i = 0; i < na; i++ )
  {
    yc = y1;

    XschToolsSaveXfigLine( x, yc - gas_y /2, x, yc + gas_y/2);

    /* And Shape */

    XschToolsSaveXfigLine( 
               x, yc - gas_y/2, x + gas_x/2, yc - gas_y/2 );

    XschToolsSaveXfigLine(
               x         , yc + gas_y/2,
               x + gas_x/2, yc + gas_y/2 );

    XschToolsSaveXfigArc( x, yc - gas_y/2, gas_x, gas_y -1, 90 * 64, -180 * 64);

    y1 -= 2 * ( SCP_BOX_CON_STEP_Y * XSCH_UNIT );
  }

  if ( n & 1 )
  {
    y1  = y1 + ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );
    yc  = y1;

    XschToolsSaveXfigLine( x , yc, x + gas_x, yc  );


  }
/*
** ToolsSaveXfig Output Or gate
*/
  /* Or shape alone */
  gos_y   = 2 * DeltaY / 3;
  gos_x   = ( 2 * gs_x ) / 3;
  yc      = Ycr;
  y       = yc + gos_y /2;
  x       = x + gas_x;

  XschToolsSaveXfigLine( 
             x, y, x + gos_x/2, y );

  XschToolsSaveXfigLine( 
             x          , y - gos_y,
             x + gos_x/2, y - gos_y );

  XschToolsSaveXfigArc( x, y - gos_y, gos_x, gos_y, 90 * 64, -180 * 64);

  Radius = (float)(gos_y) * 0.707106781185;

  XschToolsSaveXfigArc( x - (Radius * 5 / 3), yc - Radius,
                       Radius * 2, Radius * 2, 45 * 64, -90 * 64);
}

/*------------------------------------------------------------\
|                                                             |
|                       XschToolsSaveXfigMux                  |
|                                                             |
\------------------------------------------------------------*/

static void XschToolsSaveXfigMux( X1r, Y1r, X2r, Y2r, Ycr, Obj )

  long          X1r;
  long          Y1r;
  long          X2r;
  long          Y2r;
  long          Ycr;
  xschobj_list *Obj;
{
  long gs_y;
  long gs_x;
  long ms_x;
  long ms_y;
  long cs;
  int i;
  int ni;
  int ns;

  long x, y;
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

  gs_y = ( 7 * DeltaY ) / 8;
  gs_x = ( 6 * DeltaX ) / 8;
  cs   = DeltaX / 8;

  yc = Ycr;
  x = X1r + cs;

/*
** ToolsSaveXfig Mux Rectangle
*/
  ms_x = gs_x / 2;

  y1 = Obj->Y + (2 * SCP_BOX_CON_BASE_Y + (ni - 1) * SCP_BOX_CON_STEP_Y) * XSCH_UNIT;
  y1 -= XschBoundYmin;

  ms_y = y2 - Y1r;

  y2 = y2;

  XschToolsSaveXfigRectangle( x, y2, ms_x, ms_y );

  XschToolsSaveXfigLine( x, y2, x + ms_x, y2 + ms_y / 2 );

  XschToolsSaveXfigLine( x + ms_x, y2 + ms_y/2, x, y2 + ms_y );

  if ( Obj->ARG4 == 0 )
  {
     /* Output line : centered vertically */
     XschToolsSaveXfigLine( x + ms_x, y2 + ms_y/2, x + ms_x + cs, y2 + ms_y/2 );
  }
  else 
  {
     /* Output circle */
     XschToolsSaveXfigCircle( x + ms_x, y2 + ms_y/2 - cs/2, cs, cs );
  }

  y3 = y2 + ms_y/2;

  XschToolsSaveXfigLine( x + ms_x + cs, y3, x + gs_x + cs, y3 );

  XschToolsSaveXfigLine( x + gs_x + cs, y3, x + gs_x + cs, yc );
/*
** Input connectors
*/
  y1 = Obj->Y + ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );
  y1 -= XschBoundYmin;

  /* Circle for inversion */
  for (i = 0; i < ni; i++)
  {
    if (( Obj->ARG3 & (1 << i)) == 0)
    {
      XschToolsSaveXfigLine( x - cs, y3, x, y3 );
    }
    else
    {
      XschToolsSaveXfigCircle( x  - cs, y3 - cs/2, cs, cs );
    }

    y1 += ( SCP_BOX_CON_STEP_Y * XSCH_UNIT );
  }

  x3 = x;
/*
** Select connectors
*/
  for (i = 0; i < ns; i++)
  {
    x3 += ms_x / ( ns + 1 );

    XschToolsSaveXfigLine( x - cs, y3, x3, y3 );

    XschToolsSaveXfigLine( x3, y3, x3, y2 );

    y1 += ( SCP_BOX_CON_STEP_Y * XSCH_UNIT );
  }
}


/*------------------------------------------------------------\
|                                                             |
|                   XschToolsSaveXfigOrAnd                    |
|                                                             |
\------------------------------------------------------------*/

static void XschToolsSaveXfigOrAnd( X1r, Y1r, X2r, Y2r, Ycr, Obj )

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
 if ( DeltaY <= 0 ) DeltaY = 1;

  n = Obj->ARG1;

  gs_y = ( 7 * DeltaY ) / 8;
  gs_x = ( 6 * DeltaX ) / 8;
  cs   = DeltaX / 8;

  yc = Ycr;
  y  = Ycr - gs_y/2;
  x = X1r + cs;

  if ( Obj->ARG3 == 0 )
  {
     /* Output line : centered vertically */
     XschToolsSaveXfigLine( x + gs_x, yc, x + gs_x + cs, yc );
  }
  else 
  {
     /* Output circle */
     XschToolsSaveXfigCircle( x + gs_x, yc - cs/2, cs, cs );
  }

/*
** Input connectors
*/
  y1 = Obj->Y + ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );
  y1 -= XschBoundYmin;

  /* Circle for inversion */
  for (i = 0; i < n; i++)
  {
    yc = y1;

    if (( Obj->ARG2 & (1 << i)) == 0)
    {
       XschToolsSaveXfigLine( x - cs, yc, x, yc );
    }
    else
    {
      XschToolsSaveXfigCircle( x  - cs, yc - cs/2, cs, cs );
    }

    y1 += ( SCP_BOX_CON_STEP_Y * XSCH_UNIT );
  }
/*
** ToolsSaveXfig Input Or gates
*/
  no     = n / 2;

  if ( n & 1 ) gos_y   = 7 * DeltaY / ( 8 * ( no + 1 ) );
  else         gos_y   = 7 * DeltaY / ( 8 * no );

  gos_x = gs_x / 3;

  y1 = Obj->Y + Obj->DY - ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );
  y1 -= XschBoundYmin;
  y1 -= SCP_BOX_CON_STEP_Y * XSCH_UNIT / 2;

  for ( i = 0; i < no; i++ )
  {
    yc = y1;

    /* Or shape alone */
  XschToolsSaveXfigLine( x          , yc + gos_y/2, x + gos_x/2, yc + gos_y/2 );

  XschToolsSaveXfigLine( x          , yc - gos_y/2, x + gos_x/2, yc - gos_y/2 );

  XschToolsSaveXfigArc( x, yc - gos_y/2, gos_x, gos_y-1, 90 * 64, -180 * 64);

  Radius = (float)(gos_y) * 0.707106781185;

  XschToolsSaveXfigArc( x - (Radius * 5 / 3), yc - Radius,
            Radius * 2, Radius * 2, 45 * 64, -90 * 64);


    y1 -= 2 * ( SCP_BOX_CON_STEP_Y * XSCH_UNIT );
  }

  if ( n & 1 )
  {
    y1  = y1 + ( SCP_BOX_CON_BASE_Y * XSCH_UNIT );
    yc  = y1;

    XschToolsSaveXfigLine( x , yc, x + gos_x, yc  );

  }

  /* ToolsSaveXfig output And Gate */
  gas_y   = 2 * DeltaY / 3;
  gas_x   = ( 2 * gs_x ) / 3;
  yc      = Ycr;
  y       = yc + gas_y /2;
  x       = x + gos_x;

  XschToolsSaveXfigLine( x, yc - gas_y /2, x, yc + gas_y/2);


  XschToolsSaveXfigLine( x, yc - gas_y/2, x + gas_x/2, yc - gas_y/2 );

  XschToolsSaveXfigLine( x         , yc + gas_y/2, x + gas_x/2, yc + gas_y/2 );

  XschToolsSaveXfigArc( x, yc - gas_y/2, gas_x, gas_y -1, 90 * 64, -180 * 64);
}

/*------------------------------------------------------------\
|                                                             |
|                      XschToolsSaveXfigPentagon              |
|                                                             |
\------------------------------------------------------------*/

void XschToolsSaveXfigPentagon( X1, Y1, X2, Y2, Obj )

  long          X1;
  long          Y1;
  long          X2;
  long          Y2;
  xschobj_list *Obj;
{
  long StepX;
  long StepY;

  StepY = ( Y2 - Y1 ) >> 1;
  StepX = StepY >> 1;

  if ( StepY <= 0 ) StepY = 1;
  if ( StepX <= 0 ) StepX = 1;

  fprintf( XschXfigFile,
          "2 3 0 1 %d 0 %d  0 -1 0.000 0 0 -1 0 0 7\n", XschXfigColor, XschXfigDepth );
  /*       ^ ^ ^ ^ ^  ^ ^   ^  ^ ^     ^ ^  ^ ^ ^ ^
   *       | | | | |  | |   |  | |     | |  | | | + Number Points
   *       | | | | |  | |   |  | |     | |  | | + Backward Arrow
   *       | | | | |  | |   |  | |     | |  | + Forward Arrow
   *       | | | | |  | |   |  | |     | |  + Radius
   *       | | | | |  | |   |  | |     | + Cap style
   *       | | | | |  | |   |  | |     + Join style
   *       | | | | |  | |   |  | + Style Val
   *       | | | | |  | |   |  + Area Fill
   *       | | | | |  | |   + Pen Style
   *       | | | | |  | + Depth
   *       | | | | |  + Fill Color
   *       | | | | + Pen Color
   *       | | | + Width
   *       | | + Line Style
   *       | + Poly Line
   *       + Poly Line
   */

  fprintf( XschXfigFile, "  %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",
           X1        , XschXfigDy - Y1,
           X1        , XschXfigDy - Y2,
           X1        , XschXfigDy - Y2,
           X2 - StepX, XschXfigDy - Y2,
           X2        , XschXfigDy - ( Y1 + StepY ),
           X2 - StepX, XschXfigDy - Y1,
           X1        , XschXfigDy - Y1 );
}

/*------------------------------------------------------------\
|                                                             |
|                      XschToolsSaveXfigString                |
|                                                             |
\------------------------------------------------------------*/

void XschToolsSaveXfigString( X1, Y1, Text, Justified )

  long  X1;
  long  Y1;
  char *Text;
  int   Justified;
{
  fprintf( XschXfigFile,
          "4 %d %d %d 0 0 0.0 0.0 0 0.0 0.0 %ld %ld %s\\001\n",
  /*       ^ ^  ^  ^  ^ ^ ^   ^   ^ ^   ^   ^   ^   ^
   *       | |  |  |  | | |   |   | |   |   |   |   + String
   *       | |  |  |  | | |   |   | |   |   |   + Y
   *       | |  |  |  | | |   |   | |   |   + X
   *       | |  |  |  | | |   |   | |   + Length
   *       | |  |  |  | | |   |   | + Height
   *       | |  |  |  | | |   |   + Font flags
   *       | |  |  |  | | |   + Font Angle
   *       | |  |  |  | | + Font size
   *       | |  |  |  | + Font
   *       | |  |  |  + Pen Style
   *       | |  |  + Depth
   *       | |  + Pen Color
   *       | + Justified
   *       + String
   */
          Justified, XschXfigColor, XschXfigDepth, X1, XschXfigDy - Y1, Text );
}

/*------------------------------------------------------------\
|                                                             |
|                          XschToolsSaveXfigObject            |
|                                                             |
\------------------------------------------------------------*/

void XschToolsSaveXfigObject( Obj )

   xschobj_list *Obj;
{
  int     Index;
  long    X1r;
  long    X2r;
  long    Y1r;
  long    Y2r;
  long    Ycr;
  long    DeltaX;
  long    DeltaY;
  short   ObjType;
  long    Length;
  long    WidthText;
  long    HeightText;

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

  X1r -= XschBoundXmin;
  X2r -= XschBoundXmin;
  Y1r -= XschBoundYmin;
  Y2r -= XschBoundYmin;

  Ycr  = (float)( ( Y2r + Y1r) >> 1 );
  X1r  = (float)( X1r );
  Y1r  = (float)( Y1r );
  X2r  = (float)( X2r );
  Y2r  = (float)( Y2r );

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

  if ( ObjType >= XSCH_OBJECT_CROSS_BOX )
  {
    fprintf( XschXfigFile, "6 %ld %ld %ld %ld\n",
             X1r, XschXfigDy - Y1r,
             X2r, XschXfigDy - Y2r );
  }

  switch ( ObjType )
  {
    case XSCH_OBJECT_LINE :

      XschToolsSaveXfigLine( X1r, Y1r, X2r, Y2r );

    break;

    case XSCH_OBJECT_PENTAGON :

      XschToolsSaveXfigPentagon( X1r, Y1r, X2r, Y2r, Obj ); 

    break;

    case XSCH_OBJECT_RECTANGLE :

      XschToolsSaveXfigRectangle( X1r, Y1r, X2r, Y2r );

    break;

    case XSCH_OBJECT_CROSS_BOX :

      XschToolsSaveXfigRectangle( X1r, Y1r, X2r, Y2r );
      XschToolsSaveXfigLine( X1r, Y1r, X2r, Y2r );
      XschToolsSaveXfigLine( X1r, Y2r, X2r, Y1r );

    break;

    case XSCH_OBJECT_BUFFER :

      XschToolsSaveXfigBuffer( X1r, Y1r, X2r, Y2r, Ycr, Obj );

    break;

    case XSCH_OBJECT_AND :

      XschToolsSaveXfigAnd( X1r, Y1r, X2r, Y2r, Ycr, Obj );

    break;

    case XSCH_OBJECT_OR :

      XschToolsSaveXfigOr( X1r, Y1r, X2r, Y2r, Ycr, Obj );

    break;

    case XSCH_OBJECT_XOR :

      XschToolsSaveXfigXor( X1r, Y1r, X2r, Y2r, Ycr, Obj );

    break;

    case XSCH_OBJECT_TRISTATE :

      XschToolsSaveXfigTristate( X1r, Y1r, X2r, Y2r, Ycr, Obj );

    break;

    case XSCH_OBJECT_REGISTER :

      XschToolsSaveXfigRegister( X1r, Y1r, X2r, Y2r, Ycr, Obj );

    break;

    case XSCH_OBJECT_TRANSISTOR :

      XschToolsSaveXfigTransistor( X1r, Y1r, X2r, Y2r, Ycr, Obj );

    break;

    case XSCH_OBJECT_CONSTANT :

      XschToolsSaveXfigConstant( X1r, Y1r, X2r, Y2r, Ycr, Obj );

    break;

    case XSCH_OBJECT_ANDOR :

      XschToolsSaveXfigAndOr( X1r, Y1r, X2r, Y2r, Ycr, Obj );

    break;

    case XSCH_OBJECT_ORAND :

      XschToolsSaveXfigOrAnd( X1r, Y1r, X2r, Y2r, Ycr, Obj );

    break;

    case XSCH_OBJECT_MUX :

      XschToolsSaveXfigMux( X1r, Y1r, X2r, Y2r, Ycr, Obj );

    break;
  }

  if ( Obj->NAME != (char *)0 )
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
      Length     = strlen( Obj->NAME );
      WidthText  = Length * 15;
      HeightText = 15;

      if ( Obj->TYPE == XSCH_OBJECT_TEXT_RIGHT )
      {
        /* Left */
        XschToolsSaveXfigString( X1r, Y1r, Obj->NAME, 0 );
      }
      else
      if ( Obj->TYPE == XSCH_OBJECT_TEXT_LEFT )
      {
        /* Right */
        XschToolsSaveXfigString( X2r, Y1r, Obj->NAME, 2 );
      }
      else
      {
        /* Center */
        XschToolsSaveXfigString( ( X2r + X1r ) >> 1, Y1r, Obj->NAME, 1 );
      }
    }
  }

  if ( ObjType >= XSCH_OBJECT_CROSS_BOX )
  {
    fprintf( XschXfigFile, "-6\n" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          XschToolsSaveXfig                  |
|                                                             |
\------------------------------------------------------------*/

void XschToolsSaveXfig( FileName )

  char *FileName;
{
  xschobj_list *Obj;
  char         *NewFileName;
  int           Layer;


  FileName = autbasename( FileName, "fig" );

  XschXfigFile = mbkfopen( FileName, "fig", "w" );

  if ( XschXfigFile == (FILE *)0 )
  {
    XschErrorMessage( XschMainWindow, "Unable to save fig file !" );

    return;
  }

  XschSetMouseCursor( XschGraphicWindow, XSCH_WATCH_CURSOR );

  XschXfigDy = XschBoundYmax - XschBoundYmin;
/*
** Xfig header file
*/
  fprintf( XschXfigFile, 
      "#FIG 3.2\nLandscape\nCenter\nMetric\nA4\n100.00\nSingle\n-2\n1200 2\n" );

  for ( Layer = 0; Layer < XSCH_MAX_LAYER; Layer++ )
  {
    fprintf( XschXfigFile, "0 %d #%06x\n", Layer + 32, XSCH_LAYER_XFIG_COLOR[ Layer ] );
  }

  for ( Layer = 0; Layer < XSCH_MAX_LAYER; Layer++ )
  {
    if ( XSCH_ACTIVE_LAYER_TABLE[ Layer ] == 0 ) continue;

    XschXfigDepth = Layer;
    XschXfigColor = 32 + Layer;

    if ( ( XschXfigColor == 0 ) || ( XschXfigColor == 8 ) ) XschXfigColor++;

    for ( Obj  = XschFigure->OBJECT[ Layer ];
          Obj != (xschobj_list *)NULL;
          Obj  = Obj->NEXT )
    {
      if ( XSCH_ACTIVE_LAYER_TABLE[ Obj->LAYER ] == 0 ) continue;

      XschToolsSaveXfigObject( Obj );

    }
  }

  fclose( XschXfigFile );

  XschSetMouseCursor( XschGraphicWindow, XSCH_NORMAL_CURSOR );
}
