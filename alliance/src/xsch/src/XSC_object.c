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
| Tool    :                   XSCH                            |
|                                                             |
| File    :                 Object.c                          |
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
# include <stdlib.h>
# include <math.h>
# include <signal.h>
# include <string.h>
# include <setjmp.h>
# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "scl.h"
# include "XSB.h" 
# include "XSC.h"
# include "XSC_place.h"
# include "XSC_object.h"
# include "XSC_schem.h"

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

  xschfig_list *XschFigure = (xschfig_list *)NULL;

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

xschfig_list *XschAllocFigure()
{
  return( (xschfig_list *)autallocblock( sizeof( xschfig_list ) ) );
}

xschobj_list *XschAllocObject()
{
  return( (xschobj_list *)autallocheap( sizeof( xschobj_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                         Free Functions                      |
|                                                             |
\------------------------------------------------------------*/

void XschFreeFigure( Figure )

  xschfig_list *Figure;
{
  autfreeblock( Figure );
}

void XschFreeObject( Object )

  xschobj_list *Object;
{
  autfreeheap( Object, sizeof( xschobj_list ) );
}

/*------------------------------------------------------------\
|                                                             |
|                         Add Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XschAddCircle                      |
|                                                             |
\------------------------------------------------------------*/

xschobj_list *XschAddCircle( X, Y, R, Layer )

  long  X;
  long  Y;
  long  R;
  short Layer;
{
  xschobj_list *Object;

  autbegin();

  Object        = XschAllocObject();
  Object->TYPE  = XSCH_OBJECT_CIRCLE;
  Object->LAYER = Layer;
  Object->NEXT  = XschFigure->OBJECT[ Layer ];
  Object->X     = X - R;
  Object->Y     = Y - R;
  Object->DX    = ( R << 1 );
  Object->DY    = ( R << 1 );

  XschFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XschAddLine                        |
|                                                             |
\------------------------------------------------------------*/

xschobj_list *XschAddLine( X1, Y1, X2, Y2, LayerFrom, LayerTo )

  long  X1;
  long  Y1;
  long  X2;
  long  Y2;
  short LayerFrom;
  short LayerTo;
{
  xschobj_list *Object;

  autbegin();

  Object        = XschAllocObject();
  Object->LAYER = LayerFrom;
  Object->TYPE  = XSCH_OBJECT_LINE;
  Object->NEXT  = XschFigure->OBJECT[ LayerTo ];

  if ( X1 > X2 )
  {
    SetXschLineLeft( Object );

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
    SetXschLineDown( Object );

    Object->Y  = Y2;
    Object->DY = Y1 - Y2;
  }
  else
  {
    Object->Y  = Y1;
    Object->DY = Y2 - Y1;
  }

  XschFigure->OBJECT[ LayerTo ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XschAddArrow                       |
|                                                             |
\------------------------------------------------------------*/

xschobj_list *XschAddArrow( X1, Y1, X2, Y2, LayerFrom, LayerTo )

  long  X1;
  long  Y1;
  long  X2;
  long  Y2;
  short LayerFrom;
  short LayerTo;
{
  xschobj_list *Object;

  autbegin();

  Object        = XschAllocObject();
  Object->LAYER = LayerFrom;
  Object->TYPE  = XSCH_OBJECT_ARROW;
  Object->NEXT  = XschFigure->OBJECT[ LayerTo ];

  if ( X1 > X2 )
  {
    SetXschLineLeft( Object );

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
    SetXschLineDown( Object );

    Object->Y  = Y2;
    Object->DY = Y1 - Y2;
  }
  else
  {
    Object->Y  = Y1;
    Object->DY = Y2 - Y1;
  }

  XschFigure->OBJECT[ LayerTo ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XschAddTriangle                    |
|                                                             |
\------------------------------------------------------------*/

xschobj_list *XschAddTriangle( X, Y, DX, DY, Layer )

  long  X;
  long  Y;
  long  DX;
  long  DY;
  short Layer;
{
  xschobj_list  *Object;

  autbegin();

  Object        = XschAllocObject();
  Object->TYPE  = XSCH_OBJECT_TRIANGLE;
  Object->LAYER = Layer;
  Object->NEXT  = XschFigure->OBJECT[ Layer ];
  Object->X     = X;
  Object->Y     = Y;
  Object->DX    = DX;
  Object->DY    = DY;

  XschFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XschAddTristate                    |
|                                                             |
\------------------------------------------------------------*/

xschobj_list *XschAddTristate( X, Y, DX, DY, NotIn, NotOut, Layer )

  long  X;
  long  Y;
  long  DX;
  long  DY;
  short NotIn;
  short NotOut;
  short Layer;
{
  xschobj_list  *Object;

  autbegin();

  Object        = XschAllocObject();
  Object->TYPE  = XSCH_OBJECT_TRISTATE;
  Object->LAYER = Layer;
  Object->NEXT  = XschFigure->OBJECT[ Layer ];
  Object->X     = X;
  Object->Y     = Y;
  Object->DX    = DX;
  Object->DY    = DY;
  Object->ARG1  = NotIn;
  Object->ARG2  = NotOut;

  XschFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XschAddConstant                    |
|                                                             |
\------------------------------------------------------------*/

xschobj_list *XschAddConstant( X, Y, DX, DY, NumberOut, Value, Layer )

  long  X;
  long  Y;
  long  DX;
  long  DY;
  short NumberOut;
  short Value;
  short Layer;
{
  xschobj_list  *Object;

  autbegin();

  Object        = XschAllocObject();
  Object->TYPE  = XSCH_OBJECT_CONSTANT;
  Object->LAYER = Layer;
  Object->NEXT  = XschFigure->OBJECT[ Layer ];
  Object->X     = X;
  Object->Y     = Y;
  Object->DX    = DX;
  Object->DY    = DY;
  Object->ARG1  = NumberOut;
  Object->ARG2  = Value;

  XschFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XschAddTransistor                  |
|                                                             |
\------------------------------------------------------------*/

xschobj_list *XschAddTransistor( X, Y, DX, DY, NTrans, Layer )

  long  X;
  long  Y;
  long  DX;
  long  DY;
  short NTrans;
  short Layer;
{
  xschobj_list  *Object;

  autbegin();

  Object        = XschAllocObject();
  Object->TYPE  = XSCH_OBJECT_TRANSISTOR;
  Object->LAYER = Layer;
  Object->NEXT  = XschFigure->OBJECT[ Layer ];
  Object->X     = X;
  Object->Y     = Y;
  Object->DX    = DX;
  Object->DY    = DY;
  Object->ARG1  = NTrans;

  XschFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XschAddBuffer                      |
|                                                             |
\------------------------------------------------------------*/

xschobj_list *XschAddBuffer( X, Y, DX, DY, NotIn, NotOut, Layer )

  long  X;
  long  Y;
  long  DX;
  long  DY;
  short NotIn;
  short NotOut;
  short Layer;
{
  xschobj_list  *Object;

  autbegin();

  Object        = XschAllocObject();
  Object->TYPE  = XSCH_OBJECT_BUFFER;
  Object->LAYER = Layer;
  Object->NEXT  = XschFigure->OBJECT[ Layer ];
  Object->X     = X;
  Object->Y     = Y;
  Object->DX    = DX;
  Object->DY    = DY;
  Object->ARG1  = NotIn;
  Object->ARG2  = NotOut;

  XschFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XschAddOrAnd                       |
|                                                             |
\------------------------------------------------------------*/

xschobj_list *XschAddOrAnd( X, Y, DX, DY, NumberIn, NotIn, NotOut, Layer )

  long  X;
  long  Y;
  long  DX;
  long  DY;
  short NumberIn;
  short NotIn;
  short NotOut;
  short Layer;
{
  xschobj_list  *Object;

  autbegin();

  Object        = XschAllocObject();
  Object->TYPE  = XSCH_OBJECT_ORAND;
  Object->LAYER = Layer;
  Object->NEXT  = XschFigure->OBJECT[ Layer ];
  Object->X     = X;
  Object->Y     = Y;
  Object->DX    = DX;
  Object->DY    = DY;
  Object->ARG1  = NumberIn;
  Object->ARG2  = NotIn;
  Object->ARG3  = NotOut;

  XschFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XschAddAndOr                       |
|                                                             |
\------------------------------------------------------------*/

xschobj_list *XschAddAndOr( X, Y, DX, DY, NumberIn, NotIn, NotOut, Layer )

  long  X;
  long  Y;
  long  DX;
  long  DY;
  short NumberIn;
  short NotIn;
  short NotOut;
  short Layer;
{
  xschobj_list  *Object;

  autbegin();

  Object        = XschAllocObject();
  Object->TYPE  = XSCH_OBJECT_ANDOR;
  Object->LAYER = Layer;
  Object->NEXT  = XschFigure->OBJECT[ Layer ];
  Object->X     = X;
  Object->Y     = Y;
  Object->DX    = DX;
  Object->DY    = DY;
  Object->ARG1  = NumberIn;
  Object->ARG2  = NotIn;
  Object->ARG3  = NotOut;

  XschFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XschAddMux                         |
|                                                             |
\------------------------------------------------------------*/

xschobj_list *XschAddMux( X, Y, DX, DY, NumberIn, NumberSel, NotIn, NotOut, Layer )

  long  X;
  long  Y;
  long  DX;
  long  DY;
  short NumberIn;
  short NumberSel;
  short NotIn;
  short NotOut;
  short Layer;
{
  xschobj_list  *Object;

  autbegin();

  Object        = XschAllocObject();
  Object->TYPE  = XSCH_OBJECT_MUX;
  Object->LAYER = Layer;
  Object->NEXT  = XschFigure->OBJECT[ Layer ];
  Object->X     = X;
  Object->Y     = Y;
  Object->DX    = DX;
  Object->DY    = DY;
  Object->ARG1  = NumberIn;
  Object->ARG2  = NumberSel;
  Object->ARG3  = NotIn;
  Object->ARG4  = NotOut;

  XschFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XschAddOr                          |
|                                                             |
\------------------------------------------------------------*/

xschobj_list *XschAddOr( X, Y, DX, DY, NumberIn, NotIn, NotOut, Layer )

  long  X;
  long  Y;
  long  DX;
  long  DY;
  short NumberIn;
  short NotIn;
  short NotOut;
  short Layer;
{
  xschobj_list  *Object;

  autbegin();

  Object        = XschAllocObject();
  Object->TYPE  = XSCH_OBJECT_OR;
  Object->LAYER = Layer;
  Object->NEXT  = XschFigure->OBJECT[ Layer ];
  Object->X     = X;
  Object->Y     = Y;
  Object->DX    = DX;
  Object->DY    = DY;
  Object->ARG1  = NumberIn;
  Object->ARG2  = NotIn;
  Object->ARG3  = NotOut;

  XschFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XschAddAnd                         |
|                                                             |
\------------------------------------------------------------*/

xschobj_list *XschAddAnd( X, Y, DX, DY, NumberIn, NotIn, NotOut, Layer )

  long  X;
  long  Y;
  long  DX;
  long  DY;
  short NumberIn;
  short NotIn;
  short NotOut;
  short Layer;
{
  xschobj_list  *Object;

  autbegin();

  Object        = XschAllocObject();
  Object->TYPE  = XSCH_OBJECT_AND;
  Object->LAYER = Layer;
  Object->NEXT  = XschFigure->OBJECT[ Layer ];
  Object->X     = X;
  Object->Y     = Y;
  Object->DX    = DX;
  Object->DY    = DY;
  Object->ARG1  = NumberIn;
  Object->ARG2  = NotIn;
  Object->ARG3  = NotOut;

  XschFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XschAddXor                         |
|                                                             |
\------------------------------------------------------------*/

xschobj_list *XschAddXor( X, Y, DX, DY, NumberIn, NotIn, NotOut, Layer )

  long  X;
  long  Y;
  long  DX;
  long  DY;
  short NumberIn;
  short NotIn;
  short NotOut;
  short Layer;
{
  xschobj_list  *Object;

  autbegin();

  Object        = XschAllocObject();
  Object->TYPE  = XSCH_OBJECT_XOR;
  Object->LAYER = Layer;
  Object->NEXT  = XschFigure->OBJECT[ Layer ];
  Object->X     = X;
  Object->Y     = Y;
  Object->DX    = DX;
  Object->DY    = DY;
  Object->ARG1  = NumberIn;
  Object->ARG2  = NotIn;
  Object->ARG3  = NotOut;

  XschFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XschAddRegister                    |
|                                                             |
\------------------------------------------------------------*/

xschobj_list *XschAddRegister( X, Y, DX, DY, NumberIn, NotIn, ClockIn, NotOut, Edge, Layer )

  long  X;
  long  Y;
  long  DX;
  long  DY;
  short NumberIn;
  short NotIn;
  short ClockIn;
  short NotOut;
  short Edge;
  short Layer;
{
  xschobj_list  *Object;

  autbegin();

  Object        = XschAllocObject();
  Object->TYPE  = XSCH_OBJECT_REGISTER;
  Object->LAYER = Layer;
  Object->NEXT  = XschFigure->OBJECT[ Layer ];
  Object->X     = X;
  Object->Y     = Y;
  Object->DX    = DX;
  Object->DY    = DY;
  Object->ARG1  = NumberIn;
  Object->ARG2  = NotIn;
  Object->ARG3  = ClockIn;
  Object->ARG4  = NotOut;
  Object->ARG5  = Edge;

  XschFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XschAddRectangle                   |
|                                                             |
\------------------------------------------------------------*/

xschobj_list *XschAddRectangle( X, Y, DX, DY, Layer )

  long  X;
  long  Y;
  long  DX;
  long  DY;
  short Layer;
{
  xschobj_list  *Object;

  autbegin();

  Object        = XschAllocObject();
  Object->TYPE  = XSCH_OBJECT_RECTANGLE;
  Object->LAYER = Layer;
  Object->NEXT  = XschFigure->OBJECT[ Layer ];
  Object->X     = X;
  Object->Y     = Y;
  Object->DX    = DX;
  Object->DY    = DY;

  XschFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XschAddCrossBox                    |
|                                                             |
\------------------------------------------------------------*/

xschobj_list *XschAddCrossBox( X, Y, DX, DY, Layer )

  long  X;
  long  Y;
  long  DX;
  long  DY;
  short Layer;
{
  xschobj_list  *Object;

  autbegin();

  Object        = XschAllocObject();
  Object->TYPE  = XSCH_OBJECT_CROSS_BOX;
  Object->LAYER = Layer;
  Object->NEXT  = XschFigure->OBJECT[ Layer ];
  Object->X     = X;
  Object->Y     = Y;
  Object->DX    = DX;
  Object->DY    = DY;

  XschFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XschAddTextWidth                   |
|                                                             |
\------------------------------------------------------------*/

xschobj_list *XschAddTextWidth( X, Y, Width, Type, Name, Layer )

  long  X;
  long  Y;
  long  Width;
  long  Type;
  char *Name;
  short Layer;
{
  xschobj_list *Object;

  autbegin();

  Object        = XschAllocObject();
  Object->LAYER = Layer;
  Object->TYPE  = Type;
  Object->NEXT  = XschFigure->OBJECT[ Layer ];
  Object->NAME  = namealloc( Name );
  Object->X     = X;
  Object->Y     = Y;
  Object->DX    = Width;

  XschFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XschAddText                        |
|                                                             |
\------------------------------------------------------------*/

xschobj_list *XschAddText( X, Y, Type, Name, Layer )

  long  X;
  long  Y;
  long  Type;
  char *Name;
  short Layer;
{
  xschobj_list *Object;
  long          Width;

  autbegin();

  Width = strlen( Name ) * XSCH_UNIT * 2 / 3;

  if ( Type == XSCH_OBJECT_TEXT_CENTER ) 
  {
    X = X - ( Width >> 1 );
  }

  if ( Type == XSCH_OBJECT_TEXT_LEFT  )
  {
    X = X - Width;
  }

  Object        = XschAllocObject();
  Object->LAYER = Layer;
  Object->TYPE  = Type;
  Object->NEXT  = XschFigure->OBJECT[ Layer ];
  Object->NAME  = namealloc( Name );
  Object->X     = X;
  Object->Y     = Y;
  Object->DX    = Width;


  XschFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XschAddPentagon                    |
|                                                             |
\------------------------------------------------------------*/

xschobj_list *XschAddPentagon( X, Y, DX, DY, Layer )

  long  X;
  long  Y;
  long  DX;
  long  DY;
  short Layer;
{
  xschobj_list  *Object;

  autbegin();

  Object        = XschAllocObject();
  Object->TYPE  = XSCH_OBJECT_PENTAGON;
  Object->LAYER = Layer;
  Object->NEXT  = XschFigure->OBJECT[ Layer ];
  Object->X     = X;
  Object->Y     = Y;
  Object->DX    = DX;
  Object->DY    = DY;

  XschFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XschAddFigure                      |
|                                                             |
\------------------------------------------------------------*/

xschfig_list *XschAddFigure()
{
  autbegin();

  XschFigure = XschAllocFigure();
  XschFigure->NAME = XschFigureSch->NAME;

  XschPlaceFigure( XschFigureSch );

  autend();
  return( XschFigure );
}

/*------------------------------------------------------------\
|                                                             |
|                         Load Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XschLoadFigure                     |
|                                                             |
\------------------------------------------------------------*/

void XschLoadFigure( FileName )

  char *FileName;
{
  autbegin();

  XschFigureSch = (schfig_list *)Xschgetschfig( FileName );

  if ( XschFigureSch != (schfig_list *)NULL )
  {
    XschAddFigure();
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
|                          XschDelFigure                      |
|                                                             |
\------------------------------------------------------------*/

void XschDelFigure()
{
  xschobj_list   *ScanObject;
  xschobj_list   *DelObject;
  short           Layer;

  if ( XschFigure == (xschfig_list *)NULL ) return;

  autbegin();

  for ( Layer = 0; Layer < XSCH_MAX_LAYER; Layer++ )
  {
    ScanObject = XschFigure->OBJECT[ Layer ];

    while ( ScanObject != (xschobj_list *)NULL )
    {
      DelObject  = ScanObject;
      ScanObject = ScanObject->NEXT;

      XschFreeObject( DelObject );
    }
  }

  XschFreeFigure( XschFigure );

  delschfig( XschFigureSch );

  while ( HEAD_LOFIG != (lofig_list *)0 )
  {
    dellofig( HEAD_LOFIG->NAME );
  }

  XschFigure    = (xschfig_list *)NULL;
  XschFigureSch = (schfig_list  *)NULL;

  autend();
}
