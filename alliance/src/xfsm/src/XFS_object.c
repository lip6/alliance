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
# include "abl.h"
# include "bdd.h"
# include "fsm.h"
# include "ftl.h"
# include "XSB.h" 
# include "XFS.h"
# include "XFS_object.h"
# include "XFS_bdd.h"
# include "XFS_fsm.h"

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

  xfsmfig_list *XfsmFigure = (xfsmfig_list *)0;

  static long XfsmScanStepX1 = 0;
  static long XfsmScanStepX2 = 0;


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

xfsmfig_list *XfsmAllocFigure()
{
  return( (xfsmfig_list *)autallocblock( sizeof( xfsmfig_list ) ) );
}

xfsmobj_list *XfsmAllocObject()
{
  return( (xfsmobj_list *)autallocheap( sizeof( xfsmobj_list ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                         Free Functions                      |
|                                                             |
\------------------------------------------------------------*/

void XfsmFreeFigure( Figure )

  xfsmfig_list *Figure;
{
  autfreeblock( Figure );
}

void XfsmFreeObject( Object )

  xfsmobj_list *Object;
{
  autfreeheap( Object, sizeof( xfsmobj_list ) );
}

/*------------------------------------------------------------\
|                                                             |
|                         Add Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XfsmAddCircle                      |
|                                                             |
\------------------------------------------------------------*/

xfsmobj_list *XfsmAddCircle( X, Y, R, Layer )

  long  X;
  long  Y;
  long  R;
  short Layer;
{
  xfsmobj_list *Object;

  autbegin();

  Object        = XfsmAllocObject();
  Object->TYPE  = XFSM_OBJECT_CIRCLE;
  Object->LAYER = Layer;
  Object->NEXT  = XfsmFigure->OBJECT[ Layer ];
  Object->X     = X - R;
  Object->Y     = Y - R;
  Object->DX    = ( R << 1 );
  Object->DY    = ( R << 1 );

  XfsmFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XfsmAddLine                        |
|                                                             |
\------------------------------------------------------------*/

xfsmobj_list *XfsmAddLine( X1, Y1, X2, Y2, LayerFrom, LayerTo )

  long  X1;
  long  Y1;
  long  X2;
  long  Y2;
  short LayerFrom;
  short LayerTo;
{
  xfsmobj_list *Object;

  autbegin();

  Object        = XfsmAllocObject();
  Object->LAYER = LayerFrom;
  Object->TYPE  = XFSM_OBJECT_LINE;
  Object->NEXT  = XfsmFigure->OBJECT[ LayerTo ];

  if ( X1 > X2 )
  {
    SetXfsmLineLeft( Object );

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
    SetXfsmLineDown( Object );

    Object->Y  = Y2;
    Object->DY = Y1 - Y2;
  }
  else
  {
    Object->Y  = Y1;
    Object->DY = Y2 - Y1;
  }

  XfsmFigure->OBJECT[ LayerTo ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XfsmAddArrow                       |
|                                                             |
\------------------------------------------------------------*/

xfsmobj_list *XfsmAddArrow( X1, Y1, X2, Y2, LayerFrom, LayerTo )

  long  X1;
  long  Y1;
  long  X2;
  long  Y2;
  short LayerFrom;
  short LayerTo;
{
  xfsmobj_list *Object;

  autbegin();

  Object        = XfsmAllocObject();
  Object->LAYER = LayerFrom;
  Object->TYPE  = XFSM_OBJECT_ARROW;
  Object->NEXT  = XfsmFigure->OBJECT[ LayerTo ];

  if ( X1 > X2 )
  {
    SetXfsmLineLeft( Object );

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
    SetXfsmLineDown( Object );

    Object->Y  = Y2;
    Object->DY = Y1 - Y2;
  }
  else
  {
    Object->Y  = Y1;
    Object->DY = Y2 - Y1;
  }

  XfsmFigure->OBJECT[ LayerTo ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XfsmAddRectangle                   |
|                                                             |
\------------------------------------------------------------*/

xfsmobj_list *XfsmAddRectangle( X, Y, DX, DY, Layer )

  long  X;
  long  Y;
  long  DX;
  long  DY;
  short Layer;
{
  xfsmobj_list  *Object;

  autbegin();

  Object        = XfsmAllocObject();
  Object->TYPE  = XFSM_OBJECT_RECTANGLE;
  Object->LAYER = Layer;
  Object->NEXT  = XfsmFigure->OBJECT[ Layer ];
  Object->X     = X;
  Object->Y     = Y;
  Object->DX    = DX;
  Object->DY    = DY;

  XfsmFigure->OBJECT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XfsmAddText                        |
|                                                             |
\------------------------------------------------------------*/

xfsmobj_list *XfsmAddText( X, Y, Name, Layer )

  long  X;
  long  Y;
  char *Name;
  short Layer;
{
  xfsmobj_list *Object;

  autbegin();

  Object        = XfsmAllocObject();
  Object->LAYER = Layer;
  Object->TYPE  = XFSM_OBJECT_TEXT;
  Object->NEXT  = XfsmFigure->TEXT[ Layer ];
  Object->NAME  = namealloc( Name );
  Object->X     = X;
  Object->Y     = Y;

  XfsmFigure->TEXT[ Layer ] = Object;

  autend();
  return( Object );
}

/*------------------------------------------------------------\
|                                                             |
|                          XfsmAddState                       |
|                                                             |
\------------------------------------------------------------*/

xfsmobj_list *XfsmAddState( State, X, Y, Layer )

  fsmstate_list *State;
  long           X;
  long           Y;
  short          Layer;
{
  xfsmobj_list   *Circle;
  xfsmobj_list   *Text;
  xfsmcoord      *Coord;

  Coord    = XFSM_COORD( State );
  Coord->X = X;
  Coord->Y = Y;

  X *= XFSM_UNIT;
  Y *= XFSM_UNIT;

  Circle = XfsmAddCircle( X, Y, XFSM_UNIT, Layer );
  Text   = XfsmAddText( X, Y - ( ( 3 * XFSM_UNIT ) >> 1 ),
                        State->NAME, Layer );

  Text->LINK   = Circle;
  Circle->LINK = Text;

  SetXfsmState( Circle );
  SetXfsmState( Text   );

  Circle->USER  = (void *)State;
  Text->USER    = (void *)State;

  Coord->OBJECT = Circle;

  autend();
  return( Circle );
}

/*------------------------------------------------------------\
|                                                             |
|                          XfsmAddTrans                       |
|                                                             |
\------------------------------------------------------------*/

xfsmobj_list *XfsmAddTrans( Trans )

  fsmtrans_list *Trans;
{
  xfsmcoord    *ToCoord;
  xfsmcoord    *FromCoord;
  xfsmobj_list *FirstLine;
  xfsmobj_list *LastLine;
  xfsmobj_list *Arrow;
  xfsmobj_list *ObjFrom;
  xfsmobj_list *ObjTo;
  int           LayerFrom;
  int           LayerTo;
  long          ArrowX1;
  long          ArrowY1;
  long          ArrowX2;
  long          ArrowY2;
  long          DeltaX;
  long          DeltaY;
  long          StepX;

  autbegin();

  FromCoord = XFSM_COORD( Trans->FROM );
  ToCoord   = XFSM_COORD( Trans->TO );

  ObjFrom = FromCoord->OBJECT;
  ObjTo   = ToCoord->OBJECT;

  if ( IsFsmStarTrans( Trans ) )
  {
    LayerFrom = LayerTo = 0;
  }
  else
  {
    LayerTo   = ObjTo->LAYER;
    LayerFrom = ObjFrom->LAYER;
  }

  FirstLine = (xfsmobj_list *)0;
  LastLine  = (xfsmobj_list *)0;

  ArrowX1 = ( ToCoord->X   ) * XFSM_UNIT; 
  ArrowY1 = ( ToCoord->Y   ) * XFSM_UNIT;
  ArrowX2 = ( FromCoord->X ) * XFSM_UNIT;
  ArrowY2 = ( FromCoord->Y ) * XFSM_UNIT;

  DeltaX = ToCoord->X - FromCoord->X;
  DeltaY = ToCoord->Y - FromCoord->Y;

  /*
  if ( DeltaY > 0 )
  */
  if ( DeltaY > 0 )
  {
    XfsmScanStepX1++ ;

    if ( XfsmScanStepX1 >= 4 ) XfsmScanStepX1 = 0;

    StepX = ( 4 + XfsmScanStepX1 ) * XFSM_UNIT;

    if ( DeltaX > 0 ) DeltaX =   StepX;
    else              DeltaX = - StepX;

    FirstLine = XfsmAddLine( ArrowX2 - XFSM_UNIT, ArrowY2,
                             ArrowX2 + DeltaX, ArrowY2,
                             LayerFrom, LayerTo );

    SetXfsmTrans( FirstLine );
    FirstLine->USER = (void *)Trans;

    ArrowX2 = ArrowX2 + DeltaX;

    LastLine = XfsmAddLine( ArrowX2, ArrowY2,
                            ArrowX2, ArrowY1,
                            LayerFrom, LayerTo );

    SetXfsmTrans( LastLine );
    LastLine->USER = (void *)Trans;

    ArrowY2 = ArrowY1;

    FirstLine->LINK  = LastLine;
  }
  else
  if ( ( DeltaX == 0  ) &&
       ( DeltaY < -16 ) )
  {
    XfsmScanStepX2++ ;

    if ( XfsmScanStepX2 >= 4 ) XfsmScanStepX2 = 0;

    StepX = ( 4 + XfsmScanStepX2 ) * XFSM_UNIT;

    if ( DeltaX > 0 ) DeltaX = - StepX;
    else              DeltaX =   StepX;

    FirstLine = XfsmAddLine( ArrowX2 - XFSM_UNIT, ArrowY2,
                             ArrowX2 + DeltaX, ArrowY2,
                             LayerFrom, LayerTo );

    SetXfsmTrans( FirstLine );
    FirstLine->USER = (void *)Trans;

    ArrowX2 = ArrowX2 + DeltaX;

    LastLine = XfsmAddLine( ArrowX2, ArrowY2,
                            ArrowX2, ArrowY1,
                            LayerFrom, LayerTo );

    SetXfsmTrans( LastLine );
    LastLine->USER = (void *)Trans;

    ArrowY2 = ArrowY1;

    FirstLine->LINK  = LastLine;
  }

  if ( ObjFrom == ObjTo )
  {
    ArrowX2 = ArrowX1 - ( XFSM_UNIT << 1 );
    ArrowY2 = ArrowY1 + ( XFSM_UNIT << 1 );
  }

  Arrow = XfsmAddArrow( ArrowX1, ArrowY1,
                        ArrowX2, ArrowY2,
                        LayerFrom, LayerTo );

  SetXfsmTrans( Arrow );
  Arrow->USER = (void *)Trans;

  if ( FirstLine != (xfsmobj_list *)0 )
  {
    Arrow->LINK    = FirstLine;
    LastLine->LINK = Arrow;
  }
  else
  {
    Arrow->LINK    = Arrow;
  }

  Trans->USER = (void *)Arrow;

  autend();
  return( Arrow );
}

/*------------------------------------------------------------\
|                                                             |
|                          XfsmPlaceState                     |
|                                                             |
\------------------------------------------------------------*/

long XfsmPlaceState( FsmState, X, Y )

  fsmstate_list *FsmState;
  long           X;
  long           Y;
{
  fsmtrans_list *Trans;
  fsmstate_list *State;
  chain_list    *ScanChain;
  int            Layer;
  int            First;

  if ( FsmState->USER == (void *)0 )
  {
    if ( IsFsmFirstState( FsmState ) ) Layer = XFSM_FIRST_LAYER;
    else
    if ( IsFsmStarState( FsmState ) )  Layer = XFSM_STAR_LAYER;
    else                               Layer = XFSM_STATE_LAYER;

    FsmState->USER = (void *)autallocheap( sizeof( xfsmcoord ) );
    XfsmAddState( FsmState, X, Y, Layer );

    First = 1;
    Y     = Y - 8;

    for ( ScanChain  = FsmState->FROM;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      Trans = (fsmtrans_list *)( ScanChain->DATA );
      State = Trans->TO;

      if ( ( ! First ) && ( State != FsmState ) )
      {
        if ( State->USER == (void *)0 ) X += 10;
      }

      if ( First )
      {
        First = ( ( State       == FsmState  ) || 
                  ( State->USER != (void *)0 ) );
      }

      X = XfsmPlaceState( State, X, Y - 8 );
      XfsmAddTrans( Trans );
    }
  }
  
  return( X );
}

/*------------------------------------------------------------\
|                                                             |
|                          XfsmAddFigure                      |
|                                                             |
\------------------------------------------------------------*/

xfsmfig_list *XfsmAddFigure()
{
  fsmfig_list   *FigureFsm;
  chain_list    *ScanChain;
  fsmstate_list *ScanState;
  void          *Pointer;
  long           X;

  autbegin();

  XfsmFigure = XfsmAllocFigure();
  XfsmFigure->NAME = XfsmFigureFsm->NAME;

  X = 0;
  XfsmScanStepX1 = 0;
  XfsmScanStepX2 = 0;
  
  for ( ScanChain  = XfsmFigureFsm->MULTI;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    FigureFsm = (fsmfig_list *)ScanChain->DATA;

    if ( FigureFsm->FIRST_STATE != (fsmstate_list *)0 )
    {
      X = 15 + XfsmPlaceState( FigureFsm->FIRST_STATE, X, 0 );
    }
  
    if ( FigureFsm->STAR_STATE != (fsmstate_list *)0 )
    {
      ScanState = FigureFsm->STAR_STATE;
      if ( ScanState->USER == (void *)0 )
      {
        X = 15 + XfsmPlaceState( ScanState, X, 0 );
      }
    }
  
    for ( ScanState  = FigureFsm->STATE;
          ScanState != (fsmstate_list *)0;
          ScanState  = ScanState->NEXT )
    {
      if ( ScanState->USER == (void *)0 )
      {
        X = 15 + XfsmPlaceState( ScanState, X, 0 );
      }
    }
  
    for ( ScanState  = FigureFsm->STATE;
          ScanState != (fsmstate_list *)0;
          ScanState  = ScanState->NEXT )
    {
      Pointer = ScanState->USER;
      ScanState->USER = (void *)(((xfsmcoord *)Pointer)->OBJECT);
  
      autfreeheap( Pointer, sizeof( xfsmcoord ) );
    }
  }

  autend();
  return( XfsmFigure );
}

/*------------------------------------------------------------\
|                                                             |
|                         Load Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XfsmLoadFigure                     |
|                                                             |
\------------------------------------------------------------*/

void XfsmLoadFigure( FileName )

  char *FileName;
{
  autbegin();

  XfsmFigureFsm = Xfsmgetfsmfig( FileName );

  if ( XfsmFigureFsm != (fsmfig_list *)0 )
  {
    XfsmSimplifyFigure( XfsmFigureFsm );

    XfsmAddFigure();
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
|                          XfsmDelFigure                      |
|                                                             |
\------------------------------------------------------------*/

void XfsmDelFigure()
{
  xfsmobj_list   *ScanObject;
  xfsmobj_list   *DelObject;
  short           Layer;

  if ( XfsmFigure == (xfsmfig_list *)0 ) return;

  autbegin();

  for ( Layer = 0; Layer < XFSM_MAX_LAYER; Layer++ )
  {
    ScanObject = XfsmFigure->OBJECT[ Layer ];

    while ( ScanObject != (xfsmobj_list *)0 )
    {
      DelObject  = ScanObject;
      ScanObject = ScanObject->NEXT;

      XfsmFreeObject( DelObject );
    }

    ScanObject = XfsmFigure->TEXT[ Layer ];

    while ( ScanObject != (xfsmobj_list *)0 )
    {
      DelObject  = ScanObject;
      ScanObject = ScanObject->NEXT;

      XfsmFreeObject( DelObject );
    }
  }

  XfsmFreeFigure( XfsmFigure );

  while ( HEAD_FSMFIG != (fsmfig_list *)0 )
  {
    delfsmfig( HEAD_FSMFIG->NAME );
  }

  XfsmFigure    = (xfsmfig_list *)0;
  XfsmFigureFsm = (fsmfig_list  *)0;

  autend();
}
