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
| Tool    :                   XPAT                            |
|                                                             |
| File    :                 Object.c                          |
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
# include <stdlib.h>
# include <math.h>
# include <signal.h>
# include <string.h>
# include <setjmp.h>
# include "mut.h"
# include "aut.h"
# include "pat.h"
# include "XSB.h" 
# include "XPT.h"
# include "XPT_place.h"
# include "XPT_object.h"
# include "XPT_pat.h"

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

  xpatfig_list *XpatFigure = (xpatfig_list *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Alloc Functions                     |
|                                                             |
\------------------------------------------------------------*/

xpatfig_list *XpatAllocFigure()
{
  return( (xpatfig_list *)autallocblock( sizeof( xpatfig_list ) ) );
}

xpatobj_list *XpatAllocObject()
{
  return( (xpatobj_list *)autallocheap( sizeof( xpatobj_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                         Free Functions                      |
|                                                             |
\------------------------------------------------------------*/

void XpatFreeFigure( Figure )

  xpatfig_list *Figure;
{
  autfreeblock( Figure );
}

void XpatFreeObject( Object )

  xpatobj_list *Object;
{
  autfreeheap( Object, sizeof( xpatobj_list ) );
}

/*------------------------------------------------------------\
|                                                             |
|                         Add Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XpatAddCircle                      |
|                                                             |
\------------------------------------------------------------*/

xpatobj_list *XpatAddCircle( X, Y, R, Layer )

  long  X;
  long  Y;
  long  R;
  short Layer;
{
  xpatobj_list *Object;

  autbegin();

  Object        = XpatAllocObject();
  Object->TYPE  = XPAT_OBJECT_CIRCLE;
  Object->LAYER = Layer;
  Object->NEXT  = XpatFigure->OBJECT[ Layer ];
  Object->X     = X - R;
  Object->Y     = Y - R;
  Object->DX    = ( R << 1 );
  Object->DY    = ( R << 1 );

  XpatFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XpatAddLine                        |
|                                                             |
\------------------------------------------------------------*/

xpatobj_list *XpatAddLine( X1, Y1, X2, Y2, Layer )

  long  X1;
  long  Y1;
  long  X2;
  long  Y2;
  short Layer;
{
  xpatobj_list *Object;

  autbegin();

  Object        = XpatAllocObject();
  Object->LAYER = Layer;
  Object->TYPE  = XPAT_OBJECT_LINE;
  Object->NEXT  = XpatFigure->OBJECT[ Layer ];

  if ( X1 > X2 )
  {
    SetXpatLineLeft( Object );

    Object->X  = X2;
    Object->DX = X1 - X2;
  }
  else
  {
    Object->X  = X1;
    Object->DX = X2 - X1;
  }

  if ( Y1 > Y2 )
  {
    SetXpatLineDown( Object );

    Object->Y  = Y2;
    Object->DY = Y1 - Y2;
  }
  else
  {
    Object->Y  = Y1;
    Object->DY = Y2 - Y1;
  }

  XpatFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XpatAddArrow                       |
|                                                             |
\------------------------------------------------------------*/

xpatobj_list *XpatAddArrow( X1, Y1, X2, Y2, Layer )

  long  X1;
  long  Y1;
  long  X2;
  long  Y2;
  short Layer;
{
  xpatobj_list *Object;

  autbegin();

  Object        = XpatAllocObject();
  Object->LAYER = Layer;
  Object->TYPE  = XPAT_OBJECT_ARROW;
  Object->NEXT  = XpatFigure->OBJECT[ Layer ];

  if ( X1 > X2 )
  {
    SetXpatLineLeft( Object );

    Object->X  = X2;
    Object->DX = X1 - X2;
  }
  else
  {
    Object->X  = X1;
    Object->DX = X2 - X1;
  }

  if ( Y1 > Y2 )
  {
    SetXpatLineDown( Object );

    Object->Y  = Y2;
    Object->DY = Y1 - Y2;
  }
  else
  {
    Object->Y  = Y1;
    Object->DY = Y2 - Y1;
  }

  XpatFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XpatAddRectangle                   |
|                                                             |
\------------------------------------------------------------*/

xpatobj_list *XpatAddRectangle( X, Y, DX, DY, Layer )

  long  X;
  long  Y;
  long  DX;
  long  DY;
  short Layer;
{
  xpatobj_list  *Object;

  autbegin();

  Object        = XpatAllocObject();
  Object->TYPE  = XPAT_OBJECT_RECTANGLE;
  Object->LAYER = Layer;
  Object->NEXT  = XpatFigure->OBJECT[ Layer ];
  Object->X     = X;
  Object->Y     = Y;
  Object->DX    = DX;
  Object->DY    = DY;

  XpatFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XpatAddHexagon                     |
|                                                             |
\------------------------------------------------------------*/

xpatobj_list *XpatAddHexagon( X, Y, DX, DY, Layer )

  long  X;
  long  Y;
  long  DX;
  long  DY;
  short Layer;
{
  xpatobj_list  *Object;

  autbegin();

  Object        = XpatAllocObject();
  Object->TYPE  = XPAT_OBJECT_HEXAGON;
  Object->LAYER = Layer;
  Object->NEXT  = XpatFigure->OBJECT[ Layer ];
  Object->X     = X;
  Object->Y     = Y;
  Object->DX    = DX;
  Object->DY    = DY;

  XpatFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XpatAddText                        |
|                                                             |
\------------------------------------------------------------*/

xpatobj_list *XpatAddText( X, Y, Name, Layer )

  long  X;
  long  Y;
  char *Name;
  short Layer;
{
  xpatobj_list *Object;
  long          Width;

  autbegin();

  Width = strlen( Name ) * XPAT_PATTERN_STEP_Y * XPAT_UNIT / 2.0;

  Object        = XpatAllocObject();
  Object->LAYER = Layer;
  Object->TYPE  = XPAT_OBJECT_TEXT;
  Object->NEXT  = XpatFigure->OBJECT[ Layer ];
  Object->NAME  = autnamealloc( Name );
  Object->X     = X - Width;
  Object->Y     = Y;
  Object->DX    = Width;
  Object->DY    = 0;

  XpatFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XpatAddFigure                      |
|                                                             |
\------------------------------------------------------------*/

xpatfig_list *XpatAddFigure()
{
  autbegin();

  XpatFigure = XpatAllocFigure();
  XpatFigure->NAME = XpatFigurePat->NAME;

  XpatPlaceFigure();

  autend();
  return( XpatFigure );
}

/*------------------------------------------------------------\
|                                                             |
|                         Load Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XpatLoadFigure                     |
|                                                             |
\------------------------------------------------------------*/

void XpatLoadFigure( FileName )

  char *FileName;
{
  autbegin();

  XpatFigurePat = Xpatloadpatfig( FileName );

  if ( XpatFigurePat != (patfig_list *)NULL )
  {
    XpatAddFigure();
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         Del Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XpatDelFigure                      |
|                                                             |
\------------------------------------------------------------*/

void XpatDelFigure()
{
  xpatobj_list   *ScanObject;
  xpatobj_list   *DelObject;
  short           Layer;

  if ( XpatFigure == (xpatfig_list *)NULL ) return;

  autbegin();

  for ( Layer = 0; Layer < XPAT_MAX_LAYER; Layer++ )
  {
    ScanObject = XpatFigure->OBJECT[ Layer ];

    while ( ScanObject != (xpatobj_list *)NULL )
    {
      DelObject  = ScanObject;
      ScanObject = ScanObject->NEXT;

      XpatFreeObject( DelObject );
    }
  }

  XpatFreeFigure( XpatFigure );
  pat_delpatfig( XpatFigurePat );

  XpatFigure    = (xpatfig_list *)NULL;
  XpatFigurePat = (patfig_list  *)NULL;

  autend();
}
