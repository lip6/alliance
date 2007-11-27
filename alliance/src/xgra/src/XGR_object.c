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
# include "XSB.h"
# include "XGR.h"
# include "XGR_place.h"
# include "XGR_object.h"
# include "XGR_graph.h"

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

  xautgraph *XgraFigure = (xautgraph *)NULL;

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

xautgraph *XgraAllocFigure()
{
  return( (xautgraph *)autallocblock( sizeof( xautgraph ) ) );
}

xgraobj_list *XgraAllocObject()
{
  return( (xgraobj_list *)autallocheap( sizeof( xgraobj_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                         Free Functions                      |
|                                                             |
\------------------------------------------------------------*/

void XgraFreeFigure( Figure )

  xautgraph *Figure;
{
  autfreeblock( Figure );
}

void XgraFreeObject( Object )

  xgraobj_list *Object;
{
  autfreeheap( Object, sizeof( xgraobj_list ) );
}

/*------------------------------------------------------------\
|                                                             |
|                         Add Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XgraAddCircle                      |
|                                                             |
\------------------------------------------------------------*/

xgraobj_list *XgraAddCircle( X, Y, R, Layer )

  long  X;
  long  Y;
  long  R;
  short Layer;
{
  xgraobj_list *Object;

  autbegin();

  Object        = XgraAllocObject();
  Object->TYPE  = XGRA_OBJECT_CIRCLE;
  Object->LAYER = Layer;
  Object->NEXT  = XgraFigure->OBJECT[ Layer ];
  Object->X     = X - R;
  Object->Y     = Y - R;
  Object->DX    = ( R << 1 );
  Object->DY    = ( R << 1 );

  XgraFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XgraAddLine                        |
|                                                             |
\------------------------------------------------------------*/

xgraobj_list *XgraAddLine( X1, Y1, X2, Y2, LayerFrom, LayerTo )

  long  X1;
  long  Y1;
  long  X2;
  long  Y2;
  short LayerFrom;
  short LayerTo;
{
  xgraobj_list *Object;

  autbegin();

  Object        = XgraAllocObject();
  Object->LAYER = LayerFrom;
  Object->TYPE  = XGRA_OBJECT_LINE;
  Object->NEXT  = XgraFigure->OBJECT[ LayerTo ];

  if ( X1 > X2 )
  {
    SetXgraLineLeft( Object );

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
    SetXgraLineDown( Object );

    Object->Y  = Y2;
    Object->DY = Y1 - Y2;
  }
  else
  {
    Object->Y  = Y1;
    Object->DY = Y2 - Y1;
  }

  XgraFigure->OBJECT[ LayerTo ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XgraAddArrow                       |
|                                                             |
\------------------------------------------------------------*/

xgraobj_list *XgraAddArrow( X1, Y1, X2, Y2, LayerFrom, LayerTo )

  long  X1;
  long  Y1;
  long  X2;
  long  Y2;
  short LayerFrom;
  short LayerTo;
{
  xgraobj_list *Object;

  autbegin();

  Object        = XgraAllocObject();
  Object->LAYER = LayerFrom;
  Object->TYPE  = XGRA_OBJECT_ARROW;
  Object->NEXT  = XgraFigure->OBJECT[ LayerTo ];

  if ( X1 > X2 )
  {
    SetXgraLineLeft( Object );

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
    SetXgraLineDown( Object );

    Object->Y  = Y2;
    Object->DY = Y1 - Y2;
  }
  else
  {
    Object->Y  = Y1;
    Object->DY = Y2 - Y1;
  }

  XgraFigure->OBJECT[ LayerTo ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XgraAddRectangle                   |
|                                                             |
\------------------------------------------------------------*/

xgraobj_list *XgraAddRectangle( X, Y, DX, DY, Layer )

  long  X;
  long  Y;
  long  DX;
  long  DY;
  short Layer;
{
  xgraobj_list  *Object;

  autbegin();

  Object        = XgraAllocObject();
  Object->TYPE  = XGRA_OBJECT_RECTANGLE;
  Object->LAYER = Layer;
  Object->NEXT  = XgraFigure->OBJECT[ Layer ];
  Object->X     = X;
  Object->Y     = Y;
  Object->DX    = DX;
  Object->DY    = DY;

  XgraFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XgraAddText                        |
|                                                             |
\------------------------------------------------------------*/

xgraobj_list *XgraAddText( X, Y, Name, Layer )

  long  X;
  long  Y;
  char *Name;
  short Layer;
{
  xgraobj_list *Object;

  autbegin();

  Object        = XgraAllocObject();
  Object->LAYER = Layer;
  Object->TYPE  = XGRA_OBJECT_TEXT;
  Object->NEXT  = XgraFigure->TEXT[ Layer ];
  Object->NAME  = namealloc( Name );
  Object->X     = X;
  Object->Y     = Y;

  XgraFigure->TEXT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XgraAddNode                        |
|                                                             |
\------------------------------------------------------------*/

xgraobj_list *XgraAddNode( Node, X, Y, Layer )

  autnode_list *Node;
  long          X;
  long          Y;
  short         Layer;
{
  xgraobj_list   *Circle;
  xgraobj_list   *Text;

  X *= XGRA_UNIT;
  Y *= XGRA_UNIT;

  Circle = XgraAddCircle( X, Y, XGRA_UNIT, Layer );
  Text   = XgraAddText( X, Y - ( ( 3 * XGRA_UNIT ) >> 1 ),
                        Node->NAME, Layer );

  Text->LINK   = Circle;
  Circle->LINK = Text;

  SetXgraNode( Circle );
  SetXgraNode( Text   );

  Circle->USER  = (void *)Node;
  Text->USER    = (void *)Node;
  Node->USER   = (void *)Circle;

  autend();
  return( Circle );
}

/*------------------------------------------------------------\
|                                                             |
|                          XgraAddArc                       |
|                                                             |
\------------------------------------------------------------*/

xgraobj_list *XgraAddArc( Arc )

  autarc_list *Arc;
{
  xgraobj_list   *Arrow;
  xgraobj_list   *ObjFrom;
  xgraobj_list   *ObjTo;
  long            X1;
  long            Y1;
  long            X2;
  long            Y2;
  unsigned long   Layer;
  double          Distance;

  ObjFrom = (xgraobj_list *)(Arc->NODE_FROM->USER);
  ObjTo   = (xgraobj_list *)(Arc->NODE_TO->USER);

  Layer = ((unsigned long)Arc->FLAGS) % XGRA_MAX_LAYER;

  if ( ObjFrom != ObjTo )
  {
    X1 = ObjTo->X   + ( ObjTo->DX   >> 1 );
    Y1 = ObjTo->Y   + ( ObjTo->DX   >> 1 );
    X2 = ObjFrom->X + ( ObjFrom->DX >> 1 );
    Y2 = ObjFrom->Y + ( ObjFrom->DY >> 1 );
  }
  else
  {
    X1 = ObjFrom->X + ( ObjFrom->DX >> 1 );
    Y1 = ObjFrom->Y + ( ObjFrom->DY >> 1 );

    Distance = sqrt( (double)( X1 * X1 ) + (double)( Y1 * Y1 ) );

    X2 = X1 + (long)( (double)( X1 * ( XGRA_UNIT << 2 ) ) / Distance );
    Y2 = Y1 + (long)( (double)( Y1 * ( XGRA_UNIT << 2 ) ) / Distance );
  }

  Arrow = XgraAddArrow( X1, Y1, X2, Y2, (short)Layer, (short)Layer );

  Arrow->LINK = Arrow;
  SetXgraArc( Arrow );

  Arrow->USER  = (void *)Arc;
  Arc->USER  = (void *)Arrow;

  autend();
  return( Arrow );
}

/*------------------------------------------------------------\
|                                                             |
|                          XgraAddFigure                      |
|                                                             |
\------------------------------------------------------------*/

xautgraph *XgraAddFigure()
{
  autnode_list *ScanNode;
  autarc_list  *ScanArc;
  xgrapoint    *Point;
  unsigned long Layer;

  autbegin();

  XgraFigure = XgraAllocFigure();
  XgraFigure->NAME = namealloc( "graph" );

  XgraPlaceFigure();

  for ( ScanNode  = XgraFigureGra->NODE;
        ScanNode != (autnode_list *)NULL;
        ScanNode  = ScanNode->NEXT )
  {
    Point = (xgrapoint *)( ScanNode->USER );
    Layer = ((unsigned long)ScanNode->FLAGS) % XGRA_MAX_LAYER;

    XgraAddNode( ScanNode, Point->X, Point->Y, (short)Layer );
  }

  for ( ScanArc  = XgraFigureGra->ARC;
        ScanArc != (autarc_list *)NULL;
        ScanArc  = ScanArc->NEXT )
  {
    XgraAddArc( ScanArc );
  }

  autend();
  return( XgraFigure );
}

/*------------------------------------------------------------\
|                                                             |
|                         Load Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XgraLoadFigure                     |
|                                                             |
\------------------------------------------------------------*/

void XgraLoadFigure( FileName )

  char *FileName;
{
  autbegin();

  XgraFigureGra = Xgragetgrafig( FileName );

  if ( XgraFigureGra != (autgraph *)NULL )
  {
    XgraAddFigure();
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
|                          XgraDelFigure                      |
|                                                             |
\------------------------------------------------------------*/

void XgraDelFigure()
{
  xgraobj_list   *ScanObject;
  xgraobj_list   *DelObject;
  short           Layer;

  if ( XgraFigure == (xautgraph *)NULL ) return;

  autbegin();

  for ( Layer = 0; Layer < XGRA_MAX_LAYER; Layer++ )
  {
    ScanObject = XgraFigure->OBJECT[ Layer ];

    while ( ScanObject != (xgraobj_list *)NULL )
    {
      DelObject  = ScanObject;
      ScanObject = ScanObject->NEXT;

      XgraFreeObject( DelObject );
    }

    ScanObject = XgraFigure->TEXT[ Layer ];

    while ( ScanObject != (xgraobj_list *)NULL )
    {
      DelObject  = ScanObject;
      ScanObject = ScanObject->NEXT;

      XgraFreeObject( DelObject );
    }
  }

  XgraFreeFigure( XgraFigure );

  destroyautgraph( XgraFigureGra );

  XgraFigure    = (xautgraph *)NULL;
  XgraFigureGra = (autgraph  *)NULL;

  autend();
}
