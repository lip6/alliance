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
| Tool    :                   XVPN                            |
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
# include <signal.h>
# include <string.h>
# include <setjmp.h>
# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "vex.h"
# include "vpn.h"
# include "vtl.h"
# include "XSB.h" 
# include "XVP.h"
# include "XVP_object.h"
# include "XVP_error.h"

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

  xvpnfig_list *XvpnFigure    = (xvpnfig_list *)0;
  vpnfig_list  *XvpnFigureVpn = (vpnfig_list  *)0;

  int           XvpnReduce      = 1;
  int           XvpnReduceAfter = 0;
  int           XvpnElaborate   = 0;
  int           XvpnBdd         = 0;
  int           XvpnSimp        = 0;
  int           XvpnBound       = 0;

  static long   XvpnMinY        = 0;

/*------------------------------------------------------------\
|                                                             |
|                       For Set Long Jump                     |
|                                                             |
\------------------------------------------------------------*/

  static jmp_buf XvpnJumpBuffer;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 Xvpn Exit Handler Functions                 |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XvpnExitHandler                        |
|                                                             |
\------------------------------------------------------------*/

void XvpnExitHandler()
{
  XvpnFlushErrorMessage();

  longjmp( XvpnJumpBuffer, 1);
}

/*------------------------------------------------------------\
|                                                             |
|                      XvpnActiveExitHandler                  |
|                                                             |
\------------------------------------------------------------*/

void XvpnActiveExitHandler()
{
  MBK_EXIT_FUNCTION = XvpnExitHandler;
}

/*------------------------------------------------------------\
|                                                             |
|                       XvpnResetExitHandler                  |
|                                                             |
\------------------------------------------------------------*/

void XvpnResetExitHandler()
{
  MBK_EXIT_FUNCTION = NULL;
}

/*------------------------------------------------------------\
|                                                             |
|                          Xvpngetvpnfig                      |
|                                                             |
\------------------------------------------------------------*/

vpnfig_list *Xvpngetvpnfig( FileName )

  char *FileName;
{
  vpnfig_list   *FigureVpn;

  autbegin();

  XvpnActiveExitHandler();

  if ( setjmp( XvpnJumpBuffer ) == 0 )
  {
    FigureVpn = addvpnfig( FileName );
    loadvpnfig( FigureVpn, FileName, XvpnReduce );

    if ( XvpnSimp )
    {
      simpvpnfig( FigureVpn );
    }
/*\
    if ( XvpnReduceAfter )
    {
      reducevhlvpnfig( FigureVpn );
    }

    if ( XvpnBdd )
    {
      BddSystem = createbddsystem( 100, 1000, 100, 5000000 );

      makevhlbddcircuit( FigureVpn, BddSystem );
      makevhlbddnode( FigureVpn );
      convertvhlbddnode( FigureVpn );

      destroybddcircuit( FigureVpn->CIRCUIT );
      destroybddsystem( BddSystem );
    }

    if ( XvpnElaborate )
    {
      elaboratevhlvpnfig( FigureVpn );
    }
\*/

    XvpnResetExitHandler();

    autend();
    return( FigureVpn );
  }

  XvpnResetExitHandler();

  delvpnfig( FileName );

  autend();
  return( (vpnfig_list *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                         Alloc Functions                     |
|                                                             |
\------------------------------------------------------------*/

xvpnfig_list *XvpnAllocFigure()
{
  return( (xvpnfig_list *)autallocblock( sizeof( xvpnfig_list ) ) );
}

xvpnobj_list *XvpnAllocObject()
{
  return( (xvpnobj_list *)autallocheap( sizeof( xvpnobj_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                         Free Functions                      |
|                                                             |
\------------------------------------------------------------*/

void XvpnFreeFigure( Figure )

  xvpnfig_list *Figure;
{
  autfreeblock( Figure );
}

void XvpnFreeObject( Object )

  xvpnobj_list *Object;
{
  autfreeheap( Object, sizeof( xvpnobj_list ) );
}

/*------------------------------------------------------------\
|                                                             |
|                         Add Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XvpnAddCircle                      |
|                                                             |
\------------------------------------------------------------*/

xvpnobj_list *XvpnAddCircle( X, Y, R, Layer )

  long  X;
  long  Y;
  long  R;
  short Layer;
{
  xvpnobj_list *Object;

  autbegin();

  Object        = XvpnAllocObject();
  Object->TYPE  = XVPN_OBJECT_CIRCLE;
  Object->LAYER = Layer;
  Object->NEXT  = XvpnFigure->OBJECT[ Layer ];
  Object->X     = X - R;
  Object->Y     = Y - R;
  Object->DX    = ( R << 1 );
  Object->DY    = ( R << 1 );

  XvpnFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XvpnAddLine                        |
|                                                             |
\------------------------------------------------------------*/

xvpnobj_list *XvpnAddLine( X1, Y1, X2, Y2, LayerFrom, LayerTo )

  long  X1;
  long  Y1;
  long  X2;
  long  Y2;
  short LayerFrom;
  short LayerTo;
{
  xvpnobj_list *Object;

  autbegin();

  Object        = XvpnAllocObject();
  Object->LAYER = LayerFrom;
  Object->TYPE  = XVPN_OBJECT_LINE;
  Object->NEXT  = XvpnFigure->OBJECT[ LayerTo ];

  if ( X1 > X2 )
  {
    SetXvpnLineLeft( Object );

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
    SetXvpnLineDown( Object );

    Object->Y  = Y2;
    Object->DY = Y1 - Y2;
  }
  else
  {
    Object->Y  = Y1;
    Object->DY = Y2 - Y1;
  }

  XvpnFigure->OBJECT[ LayerTo ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XvpnAddArrow                       |
|                                                             |
\------------------------------------------------------------*/

xvpnobj_list *XvpnAddArrow( X1, Y1, X2, Y2, LayerFrom, LayerTo )

  long  X1;
  long  Y1;
  long  X2;
  long  Y2;
  short LayerFrom;
  short LayerTo;
{
  xvpnobj_list *Object;

  autbegin();

  Object        = XvpnAllocObject();
  Object->LAYER = LayerFrom;
  Object->TYPE  = XVPN_OBJECT_ARROW;
  Object->NEXT  = XvpnFigure->OBJECT[ LayerTo ];

  if ( X1 > X2 )
  {
    SetXvpnLineLeft( Object );

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
    SetXvpnLineDown( Object );

    Object->Y  = Y2;
    Object->DY = Y1 - Y2;
  }
  else
  {
    Object->Y  = Y1;
    Object->DY = Y2 - Y1;
  }

  XvpnFigure->OBJECT[ LayerTo ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XvpnAddRectangle                   |
|                                                             |
\------------------------------------------------------------*/

xvpnobj_list *XvpnAddRectangle( X, Y, DX, DY, Layer )

  long  X;
  long  Y;
  long  DX;
  long  DY;
  short Layer;
{
  xvpnobj_list  *Object;

  autbegin();

  Object        = XvpnAllocObject();
  Object->TYPE  = XVPN_OBJECT_RECTANGLE;
  Object->LAYER = Layer;
  Object->NEXT  = XvpnFigure->OBJECT[ Layer ];
  Object->X     = X;
  Object->Y     = Y;
  Object->DX    = DX;
  Object->DY    = DY;

  XvpnFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XvpnAddText                        |
|                                                             |
\------------------------------------------------------------*/

xvpnobj_list *XvpnAddText( X, Y, Name, Layer )

  long  X;
  long  Y;
  char *Name;
  short Layer;
{
  xvpnobj_list *Object;
  int           Length;
  char          Buffer[ 40 ];

  autbegin();

  Length = strlen( Name );

  if ( Length > 20 )
  {
    strncpy( Buffer, Name, 17 );
    strcpy( Buffer + 17, "..." );

    Name = namealloc( Buffer );
  }

  Object        = XvpnAllocObject();
  Object->LAYER = Layer;
  Object->TYPE  = XVPN_OBJECT_TEXT;
  Object->NEXT  = XvpnFigure->TEXT[ Layer ];
  Object->NAME  = namealloc( Name );
  Object->X     = X;
  Object->Y     = Y;

  XvpnFigure->TEXT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XvpnAddProc                        |
|                                                             |
\------------------------------------------------------------*/

xvpnobj_list *XvpnAddProc( Process, X, Y )

  vpnproc_list *Process;
  long           X;
  long           Y;
{
  xvpnobj_list   *Circle;
  xvpnobj_list   *Text;

  autbegin();

  X = X * XVPN_UNIT;
  Y = Y * XVPN_UNIT;

  Circle = XvpnAddCircle( X, Y, XVPN_UNIT, XVPN_LAYER_PROCESS );
  Text   = XvpnAddText( X, Y - ( ( 3 * XVPN_UNIT ) >> 1 ),
                        Process->NAME, XVPN_LAYER_PROCESS );

  SetXvpnProc( Circle );
  SetXvpnProc( Text   );

  Circle->USER  = (void *)Process;
  Text->USER    = (void *)Process;

  Circle->LINK = Text;
  Text->LINK   = Circle;

  Process->USER = (void *)Circle;

  autend();
  return( Circle );
}

/*------------------------------------------------------------\
|                                                             |
|                          XvpnAddFunc                        |
|                                                             |
\------------------------------------------------------------*/

xvpnobj_list *XvpnAddFunc( Function, X, Y )

  vpnfunc_list *Function;
  long          X;
  long          Y;
{
  xvpnobj_list   *Circle;
  xvpnobj_list   *Text;

  autbegin();

  X = X * XVPN_UNIT;
  Y = Y * XVPN_UNIT;

  Circle = XvpnAddCircle( X, Y, XVPN_UNIT, XVPN_LAYER_FUNCTION );
  Text   = XvpnAddText( X, Y - ( ( 3 * XVPN_UNIT ) >> 1 ),
                        Function->NAME, XVPN_LAYER_FUNCTION );

  SetXvpnFunc( Circle );
  SetXvpnFunc( Text   );

  Circle->USER  = (void *)Function;
  Text->USER    = (void *)Function;

  Circle->LINK = Text;
  Text->LINK   = Circle;

  Function->USER = (void *)Circle;

  autend();
  return( Circle );
}

/*------------------------------------------------------------\
|                                                             |
|                          XvpnAddPlace                       |
|                                                             |
\------------------------------------------------------------*/

xvpnobj_list *XvpnAddPlace( Place, X, Y )

  vpnplace_list *Place;
  long            X;
  long            Y;
{
  xvpnobj_list   *Circle;
  xvpnobj_list   *Text;
  xvpnobj_list   *Token;
  xvpncoord      *Coord;

  autbegin();

  Coord    = XVPN_COORD( Place );
  Coord->X = X;
  Coord->Y = Y;

  X = X * XVPN_UNIT;
  Y = Y * XVPN_UNIT;

  Circle = XvpnAddCircle( X, Y, XVPN_UNIT, Place->FATHER );
  Text   = XvpnAddText( X, Y - ( ( 3 * XVPN_UNIT ) >> 1 ),
                        Place->NAME, Place->FATHER );

  SetXvpnPlace( Circle );
  SetXvpnPlace( Text   );

  Circle->USER  = (void *)Place;
  Text->USER    = (void *)Place;

  Circle->LINK = Text;

  if ( Place->TOKEN )
  {
    Token = XvpnAddCircle( X, Y, ( XVPN_UNIT >> 1 ), XVPN_LAYER_TOKEN );
    Token->LINK = Token;
  }

  Text->LINK = Circle;
  Coord->OBJECT = Circle;

  autend();
  return( Circle );
}

/*------------------------------------------------------------\
|                                                             |
|                          XvpnAddTrans                       |
|                                                             |
\------------------------------------------------------------*/

xvpnobj_list *XvpnAddTrans( Trans, X, Y )

  vpntrans_list *Trans;
  long           X;
  long           Y;
{
  xvpnobj_list *Rectangle;
  xvpnobj_list *Text;
  xvpncoord    *Coord;
  int           TypeSize;
  short         Layer;
  long          DX;
  long          DY;

  autbegin();

  Coord    = XVPN_COORD( Trans );
  Coord->X = X;
  Coord->Y = Y;

  TypeSize = ( Trans->TYPE << 1 ) + 1;

  X  = ( X - 1 ) * XVPN_UNIT;
  Y  = ( Y     ) * XVPN_UNIT - TypeSize;
  DX = ( XVPN_UNIT   << 1 );
  DY = ( TypeSize    << 1 );

  switch ( Trans->TYPE )
  {
    case VPN_TRANS_ACT_EXEC         : 
    case VPN_TRANS_ACT_EXEC_CONC    : Layer = XVPN_LAYER_ASSIGN;
    break;

    case VPN_TRANS_SUP_WAIT         :
    case VPN_TRANS_INF_WAIT         : Layer = XVPN_LAYER_WAIT;
    break;

    case VPN_TRANS_GUARDED          : Layer = XVPN_LAYER_GUARDED;
    break;

    case VPN_TRANS_ACT_GUARDED      :
    case VPN_TRANS_ACT_GUARDED_CONC : Layer = XVPN_LAYER_ACT_GUARDED;
    break;

    default                         : Layer = Trans->FATHER;
  }

  Rectangle = XvpnAddRectangle( X, Y, DX, DY, Layer );
  Text      = XvpnAddText( X + DX + ( XVPN_UNIT >> 1 ), Y + ( DY >> 1 ),
                           Trans->NAME, Layer );

  Text->LINK      = Rectangle;
  Rectangle->LINK = Text;

  SetXvpnTrans( Rectangle );
  SetXvpnTrans( Text      );

  Rectangle->USER = (void *)Trans;
  Text->USER      = (void *)Trans;

  Coord->OBJECT = Rectangle;

  autend();
  return( Rectangle );
}

/*------------------------------------------------------------\
|                                                             |
|                          XvpnAddArcPlace                    |
|                                                             |
\------------------------------------------------------------*/

xvpnobj_list *XvpnAddArcPlace( Place, Trans, Tra  )

  vpnplace_list *Place;
  vpntrans_list *Trans;
  vpnarc        *Tra;
{
  xvpncoord    *TransCoord;
  xvpncoord    *PlaceCoord;
  xvpnobj_list *Line;

  autbegin();

  TransCoord = XVPN_COORD( Trans );
  PlaceCoord = XVPN_COORD( Place );

  Line = XvpnAddArrow( ( TransCoord->X ) * XVPN_UNIT,
                       ( TransCoord->Y ) * XVPN_UNIT,
                       ( PlaceCoord->X ) * XVPN_UNIT,
                       ( PlaceCoord->Y ) * XVPN_UNIT,
                       Place->FATHER, Place->FATHER );
  Line->LINK = Line;

  SetXvpnArcPlace( Line );

  Line->USER = (void *)Tra;
  Tra->USER  = (void *)Line;

  autend();
  return( Line );
}

/*------------------------------------------------------------\
|                                                             |
|                          XvpnAddArcTrans                    |
|                                                             |
\------------------------------------------------------------*/

xvpnobj_list *XvpnAddArcTrans( Trans, Place, Pla  )

  vpntrans_list *Trans;
  vpnplace_list *Place;
  vpnarc        *Pla;
{
  xvpncoord    *TransCoord;
  xvpncoord    *PlaceCoord;
  xvpnobj_list *FirstLine;
  xvpnobj_list *SecondLine;
  xvpnobj_list *LastLine;
  xvpnobj_list *Arrow;
  long          ArrowX1;
  long          ArrowY1;
  long          ArrowX2;
  long          ArrowY2;
  long          DeltaX;
  long          DeltaY;

  autbegin();

  TransCoord = XVPN_COORD( Trans );
  PlaceCoord = XVPN_COORD( Place );

  FirstLine = (xvpnobj_list *)0;
  LastLine  = (xvpnobj_list *)0;

  ArrowX1 = ( PlaceCoord->X ) * XVPN_UNIT; 
  ArrowY1 = ( PlaceCoord->Y ) * XVPN_UNIT;
  ArrowX2 = ( TransCoord->X ) * XVPN_UNIT;
  ArrowY2 = ( TransCoord->Y ) * XVPN_UNIT;

  DeltaX = PlaceCoord->X - TransCoord->X;
  DeltaY = PlaceCoord->Y - TransCoord->Y;

  if ( DeltaY >= 0 )
  {
    if ( DeltaX > 0 ) DeltaX =   ( XVPN_UNIT << 2 );
    else              DeltaX = - ( XVPN_UNIT << 2 );

    FirstLine = XvpnAddLine( ArrowX2, ArrowY2,
                             ArrowX2, ArrowY2 - XVPN_UNIT,
                             Place->FATHER, Place->FATHER );

    SetXvpnArcTrans( FirstLine );
    FirstLine->USER = (void *)Pla;

    ArrowY2 = ArrowY2 - XVPN_UNIT;

    SecondLine = XvpnAddLine( ArrowX2, ArrowY2,
                              ArrowX2 + DeltaX, ArrowY2,
                              Place->FATHER, Place->FATHER );

    SetXvpnArcTrans( SecondLine );
    SecondLine->USER = (void *)Pla;

    ArrowX2 = ArrowX2 + DeltaX;

    LastLine = XvpnAddLine( ArrowX2, ArrowY2,
                            ArrowX2, ArrowY1,
                            Place->FATHER, Place->FATHER );

    SetXvpnArcTrans( LastLine );
    LastLine->USER = (void *)Pla;

    ArrowY2 = ArrowY1;

    FirstLine->LINK  = SecondLine;
    SecondLine->LINK = LastLine;
  }

  Arrow = XvpnAddArrow( ArrowX1, ArrowY1,
                        ArrowX2, ArrowY2,
                        Place->FATHER, Place->FATHER );

  SetXvpnArcTrans( Arrow );
  Arrow->USER = (void *)Pla;

  if ( FirstLine != (xvpnobj_list *)0 )
  {
    Arrow->LINK    = FirstLine;
    LastLine->LINK = Arrow;
  }
  else
  {
    Arrow->LINK    = Arrow;
  }

  Pla->USER = (void *)Arrow;

  autend();
  return( Arrow );
}

/*------------------------------------------------------------\
|                                                             |
|                          XvpnPlacement                      |
|                                                             |
\------------------------------------------------------------*/

static long XvpnPlacement( VpnPlace, X, Y )

  vpnplace_list *VpnPlace;
  long           X;
  long           Y;
{
  vpntrans_list *Trans;
  vpnplace_list *Place;
  vpnarc        *ArcTrans;
  vpnarc        *ArcPlace;
  chain_list    *ScanChain1;
  chain_list    *ScanChain2;

  if ( XVPN_COORD_OBJECT( VpnPlace ) == (xvpnobj_list *)0 )
  {
    if ( Y < XvpnMinY ) XvpnMinY = Y;

    XvpnAddPlace( VpnPlace, X, Y );

    Y -=  5;

    for ( ScanChain1  = VpnPlace->TRANS_OUT;
          ScanChain1 != (chain_list *)0;
          ScanChain1  = ScanChain1->NEXT )
    {
      if ( ScanChain1 != VpnPlace->TRANS_OUT ) X += 15;

      ArcTrans = (vpnarc        *)( ScanChain1->DATA );
      Trans    = (vpntrans_list *)( ArcTrans->TARGET  );

      if ( XVPN_COORD_OBJECT( Trans ) == (xvpnobj_list *)0 )
      {
        XvpnAddTrans( Trans, X, Y );

        for ( ScanChain2  = Trans->PLACE_OUT;
              ScanChain2 != (chain_list *)0;
              ScanChain2  = ScanChain2->NEXT )
        {
          ArcPlace = (vpnarc        *)( ScanChain2->DATA );
          Place    = (vpnplace_list *)( ArcPlace->TARGET );

          X = XvpnPlacement( Place, X, Y - 5 );

          XvpnAddArcTrans( Trans, Place, ArcPlace );
        }
      }

      XvpnAddArcPlace( VpnPlace, Trans, ArcTrans );
    }
  }
  
  return( X );
}

/*------------------------------------------------------------\
|                                                             |
|                        XvpnAddPlaceCoord                    |
|                                                             |
\------------------------------------------------------------*/

void XvpnAddPlaceCoord( HeadPlace )

  vpnplace_list *HeadPlace;
{
  vpnplace_list *ScanPlace;

  for ( ScanPlace  = HeadPlace;
        ScanPlace != (vpnplace_list *)0;
        ScanPlace  = ScanPlace->NEXT )
  {
    ScanPlace->USER = (void *)autallocheap( sizeof( xvpncoord ) );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        XvpnAddTransCoord                    |
|                                                             |
\------------------------------------------------------------*/

void XvpnAddTransCoord( HeadTrans )

  vpntrans_list *HeadTrans;
{
  vpntrans_list *ScanTrans;

  for ( ScanTrans  = HeadTrans;
        ScanTrans != (vpntrans_list *)0;
        ScanTrans  = ScanTrans->NEXT )
  {
    ScanTrans->USER = (void *)autallocheap( sizeof( xvpncoord ) );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        XvpnDelPlaceCoord                    |
|                                                             |
\------------------------------------------------------------*/

void XvpnDelPlaceCoord( HeadPlace )

  vpnplace_list *HeadPlace;
{
  vpnplace_list *ScanPlace;
  void          *Pointer;

  for ( ScanPlace  = HeadPlace;
        ScanPlace != (vpnplace_list *)0;
        ScanPlace  = ScanPlace->NEXT )
  {
    Pointer = ScanPlace->USER;
    ScanPlace->USER = (void *)(((xvpncoord *)Pointer)->OBJECT);

    autfreeheap( Pointer, sizeof( xvpncoord ) );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        XvpnDelTransCoord                    |
|                                                             |
\------------------------------------------------------------*/

void XvpnDelTransCoord( HeadTrans )

  vpntrans_list *HeadTrans;
{
  vpntrans_list *ScanTrans;
  void          *Pointer;

  for ( ScanTrans  = HeadTrans;
        ScanTrans != (vpntrans_list *)0;
        ScanTrans  = ScanTrans->NEXT )
  {
    Pointer = ScanTrans->USER;
    ScanTrans->USER = (void *)(((xvpncoord *)Pointer)->OBJECT);

    autfreeheap( Pointer, sizeof( xvpncoord ) );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          XvpnAddFigure                      |
|                                                             |
\------------------------------------------------------------*/

xvpnfig_list *XvpnAddFigure()

{
  vpnproc_list  *ScanProc;
  vpnfunc_list  *ScanFunc;
  vpnplace_list *ScanPlace;
  vpnarc        *VpnArc; 
  vpntrans_list *VpnTrans;
  long           X;

  autbegin();

  XvpnFigure = XvpnAllocFigure();
  XvpnFigure->NAME = XvpnFigureVpn->NAME;

  for ( ScanProc  = XvpnFigureVpn->PROCESS;
        ScanProc != (vpnproc_list *)0;
        ScanProc  = ScanProc->NEXT )
  {
    XvpnAddPlaceCoord( ScanProc->PLACE );
    XvpnAddTransCoord( ScanProc->TRANS );
  }

  X        = 0;
  XvpnMinY = 0;

  for ( ScanProc  = XvpnFigureVpn->PROCESS;
        ScanProc != (vpnproc_list *)0;
        ScanProc  = ScanProc->NEXT ) 
  {
    XvpnAddProc( ScanProc, X, 0 );

    VpnTrans  = ScanProc->FIRST;

    if ( VpnTrans == (vpntrans_list *)0 )
    {
      VpnTrans = ScanProc->ELABO;
    }

    if ( VpnTrans == (vpntrans_list *)0 )
    {
      VpnTrans = ScanProc->TRANS;
    }

    if ( VpnTrans != (vpntrans_list *)0 )
    {
      VpnArc    = (vpnarc        *)VpnTrans->PLACE_IN->DATA;
      ScanPlace = (vpnplace_list *)VpnArc->SOURCE;

      X = 25 + XvpnPlacement( ScanPlace, X, -5 );
    }

    for ( ScanPlace  = ScanProc->PLACE;
          ScanPlace != (vpnplace_list *)0;
          ScanPlace  = ScanPlace->NEXT )
    {
      if ( XVPN_COORD_OBJECT( ScanPlace ) == (xvpnobj_list *)0 )
      {
         X = 25 + XvpnPlacement( ScanPlace, X, -5 );
      }
    }
  }

  for ( ScanProc  = XvpnFigureVpn->PROCESS;
        ScanProc != (vpnproc_list *)0;
        ScanProc  = ScanProc->NEXT ) 
  {
    XvpnDelPlaceCoord( ScanProc->PLACE );
    XvpnDelTransCoord( ScanProc->TRANS );
  }

  for ( ScanFunc  = XvpnFigureVpn->FUNCTION;
        ScanFunc != (vpnfunc_list *)0;
        ScanFunc  = ScanFunc->NEXT )
  {
    XvpnAddPlaceCoord( ScanFunc->PLACE );
    XvpnAddTransCoord( ScanFunc->TRANS );
  }

  for ( ScanFunc  = XvpnFigureVpn->FUNCTION;
        ScanFunc != (vpnfunc_list *)0;
        ScanFunc  = ScanFunc->NEXT ) 
  {
    XvpnAddFunc( ScanFunc, X, 0 );

    VpnTrans  = ScanFunc->FIRST;

    if ( VpnTrans == (vpntrans_list *)0 )
    {
      VpnTrans = ScanFunc->ELABO;
    }

    if ( VpnTrans == (vpntrans_list *)0 )
    {
      VpnTrans = ScanFunc->TRANS;
    }

    if ( VpnTrans != (vpntrans_list *)0 )
    {
      VpnArc    = (vpnarc        *)VpnTrans->PLACE_IN->DATA;
      ScanPlace = (vpnplace_list *)VpnArc->SOURCE;

      X = 25 + XvpnPlacement( ScanPlace, X, -5 );
    }

    for ( ScanPlace  = ScanFunc->PLACE;
          ScanPlace != (vpnplace_list *)0;
          ScanPlace  = ScanPlace->NEXT )
    {
      if ( XVPN_COORD_OBJECT( ScanPlace ) == (xvpnobj_list *)0 )
      {
         X = 25 + XvpnPlacement( ScanPlace, X, -5 );
      }
    }
  }

  for ( ScanFunc  = XvpnFigureVpn->FUNCTION;
        ScanFunc != (vpnfunc_list *)0;
        ScanFunc  = ScanFunc->NEXT )
  {
    XvpnDelPlaceCoord( ScanFunc->PLACE );
    XvpnDelTransCoord( ScanFunc->TRANS );
  }

  autend();
  return( XvpnFigure );
}

/*------------------------------------------------------------\
|                                                             |
|                         Del Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XvpnDelFigure                      |
|                                                             |
\------------------------------------------------------------*/

void XvpnDelFigure()
{
  xvpnobj_list   *ScanObject;
  xvpnobj_list   *DelObject;
  short           Layer;

  if ( XvpnFigure == (xvpnfig_list *)0 ) return;

  autbegin();

  for ( Layer = 0; Layer < XVPN_MAX_LAYER; Layer++ )
  {
    ScanObject = XvpnFigure->OBJECT[ Layer ];

    while ( ScanObject != (xvpnobj_list *)0 )
    {
      DelObject  = ScanObject;
      ScanObject = ScanObject->NEXT;

      XvpnFreeObject( DelObject );
    }

    ScanObject = XvpnFigure->TEXT[ Layer ];

    while ( ScanObject != (xvpnobj_list *)0 )
    {
      DelObject  = ScanObject;
      ScanObject = ScanObject->NEXT;

      XvpnFreeObject( DelObject );
    }
  }

  XvpnFreeFigure( XvpnFigure );

  delvpnfig( XvpnFigureVpn->NAME );

  XvpnFigure    = (xvpnfig_list *)0;
  XvpnFigureVpn = (vpnfig_list  *)0;

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                         Load Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XvpnLoadFigure                     |
|                                                             |
\------------------------------------------------------------*/

void XvpnLoadFigure( FileName )

  char *FileName;
{
  autbegin();

  XvpnFigureVpn = Xvpngetvpnfig( FileName );

  if ( XvpnFigureVpn != (vpnfig_list *)0 )
  {
    XvpnAddFigure();
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          Vex Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         XvpnVex2String                      |
|                                                             |
\------------------------------------------------------------*/

char *XvpnVex2String( Expr )

  vexexpr  *Expr;
{
  if ( XvpnBound ) return( viewvexexprstrbound( Expr ) );

  return( viewvexexprstr( Expr ) );
}

/*------------------------------------------------------------\
|                                                             |
|                     Elaborate Functions                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       XvpnElaborateFigure                   |
|                                                             |
\------------------------------------------------------------*/

void XvpnElaborateFigure()
{
/*\
  ScanObject = XvpnFigure->OBJECT[ XVPN_LAYER_TOKEN ];
  XvpnFigure->OBJECT[ XVPN_LAYER_TOKEN ] = (xvpnobj_list *)0;

  while ( ScanObject != (xvpnobj_list *)0 )
  {
    DelObject  = ScanObject;
    ScanObject = ScanObject->NEXT;

    XvpnFreeObject( DelObject );
  }

  elaboratevhlvpnfig( XvpnFigureVpn );

  for ( ScanProc  = XvpnFigureVpn->PROCESS;
        ScanProc != (vpnproc_list *)0;
        ScanProc  = ScanProc->NEXT )
  {
    ScanTrans = ScanProc->ELABO; 
    ScanArc   = (vpnarc        *)ScanTrans->PLACE_IN->DATA;
    ScanPlace = (vpnplace_list *)ScanArc->SOURCE;

    if ( ScanPlace->TOKEN )
    {
      ScanObject = (xvpnobj_list *)( ScanPlace->USER );

      Token = XvpnAddCircle( ScanObject->X + XVPN_UNIT,
                             ScanObject->Y + XVPN_UNIT, 
                             ( XVPN_UNIT >> 1 ), XVPN_LAYER_TOKEN );
      Token->LINK = Token;
    }
  }
\*/
}
