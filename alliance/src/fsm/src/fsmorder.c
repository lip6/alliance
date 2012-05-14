/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU Library General|
| Public License as published by the Free Software Foundation |
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
| Tool    :                     Fsm                           |
|                                                             |
| File    :                  fsmorder.c                       |
|                                                             |
| Date    :                   04.12.96                        |
|                                                             |
| Author  :               Jacomme Ludovic                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "fsm.h"

# include <stdio.h>
# include "fsmorder.h"
# include "fsmerror.h"

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

  static authtable  *FsmHashTableOrder  = (authtable  *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                           sortfsmcompare                    |
|                                                             |
\------------------------------------------------------------*/

static long sortfsmcompare( ValueArray, Index1, Index2 )

  authelem *ValueArray;
  long      Index1;
  long      Index2;
{
  return( ValueArray[ Index1 ].VALUE -
          ValueArray[ Index2 ].VALUE );
}

/*------------------------------------------------------------\
|                                                             |
|                       makefsmbddorder                       |
|                                                             |
\------------------------------------------------------------*/

static void makefsmbddorder( Expr )

  ablexpr    *Expr;
{
  char       *AtomValue;
  authelem   *Element;
  long        Oper;

  if ( Expr == (ablexpr *)0 )
  {
    fsmerror( FSM_EXPR_NULL_ERROR, (char *)0 );
  }

  if ( ABL_ATOM( Expr ) )
  {
    AtomValue = ABL_ATOM_VALUE( Expr );

    if ( ( AtomValue == ABL_ATOM_NAME_ONE  ) ||
         ( AtomValue == ABL_ATOM_NAME_ZERO ) ||
         ( AtomValue == ABL_ATOM_NAME_DC   ) )
    {
      return;
    }

    Element = searchauthelem( FsmHashTableOrder, AtomValue );

    if ( Element == (authelem *)0 )
    {
      fsmerror( FSM_UNKNOWN_ATOM_ERROR, AtomValue );
    }

    Element->VALUE++;

    return;
  }

  Oper = ABL_OPER( Expr );
  Expr = ABL_CDR( Expr );

  if ( Oper == ABL_NOT )
  {
    if ( ABL_CDR( Expr ) != (ablexpr *)0 )
    {
      fsmerror( FSM_OPERATOR_ERROR, Oper );
    }

    makefsmbddorder( ABL_CAR( Expr ) );

    return;
  }

  if ( ( isablunaryoper( Oper )          ) ||
       ( ABL_CDR( Expr ) == (ablexpr *)0 ) )
  {
    fsmerror( FSM_OPERATOR_ERROR, Oper );
  }

  makefsmbddorder( ABL_CAR( Expr ) );

  while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
  {
    makefsmbddorder( ABL_CAR( Expr ) );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       makefsmbddcircuit                     |
|                                                             |
\------------------------------------------------------------*/

void makefsmbddcircuit( FsmFigure, BddSystem, Order )

  fsmfig_list *FsmFigure;
  bddsystem   *BddSystem;
  int          Order;
{
  bddcircuit     *BddCircuit;
  fsmstate_list  *ScanState;
  fsmtrans_list  *ScanTrans;
  fsmstack_list  *ScanStack;
  fsmlocout_list *ScanLocout;
  fsmin_list     *ScanIn;
  authelem       *Element;
  authelem       *ValueArray;
  long           *IndexArray;
  long            SizeArray;
  long            Index;

  BddCircuit = createbddcircuit( FsmFigure->NAME,
                                 FsmFigure->NUMBER_IN,
                                 FsmFigure->NUMBER_OUT, BddSystem );
  if ( ! Order )
  {
    for ( ScanIn  = FsmFigure->IN;
          ScanIn != (fsmin_list *)0;
          ScanIn  = ScanIn->NEXT )
    {
      addbddcircuitin( (bddcircuit *)0, ScanIn->NAME,
                       (bddindex    )0, BDD_IN_MODE_LAST );
    }
  }
  else
  {
    FsmHashTableOrder = createauthtable( FsmFigure->NUMBER_IN << 1 );

    for ( ScanIn  = FsmFigure->IN;
          ScanIn != (fsmin_list *)0;
          ScanIn  = ScanIn->NEXT )
    {
      addauthelem( FsmHashTableOrder, ScanIn->NAME, 0 );
    }

    for ( ScanTrans  = FsmFigure->TRANS;
          ScanTrans != (fsmtrans_list *)0;
          ScanTrans  = ScanTrans->NEXT )
    {
      if ( ScanTrans->ABL != (ablexpr *)0 )
      {
        makefsmbddorder( ScanTrans->ABL );
      }
    }

    for ( ScanStack  = FsmFigure->STACK;
          ScanStack != (fsmstack_list *)0;
          ScanStack  = ScanStack->NEXT )
    {
      if ( ScanStack->ABL != (ablexpr *)0 )
      {
        makefsmbddorder( ScanStack->ABL );
      }
    }

    for ( ScanState  = FsmFigure->STATE;
          ScanState != (fsmstate_list *)0;
          ScanState  = ScanState->NEXT )
    {
      for ( ScanLocout  = ScanState->LOCOUT;
            ScanLocout != (fsmlocout_list *)0;
            ScanLocout  = ScanLocout->NEXT )
      {
        if ( ScanLocout->ABL != (ablexpr *)0 )
        {
          makefsmbddorder( ScanLocout->ABL );
        }

        if ( ScanLocout->ABL_DC != (ablexpr *)0 )
        {
          makefsmbddorder( ScanLocout->ABL_DC );
        }
      }
    }

    SizeArray  = FsmHashTableOrder->TABLE_SIZE;
    ValueArray = FsmHashTableOrder->TABLE;
    IndexArray = (long *)autallocblock( sizeof( long ) * SizeArray );

    sortautarray( ValueArray, IndexArray, SizeArray, sortfsmcompare );

    for ( Index = 0; Index < SizeArray; Index++ )
    {
      Element = &ValueArray[ IndexArray[ Index ] ];

      if ( Element->KEY != AUT_HASH_KEY_EMPTY )
      {
        addbddcircuitin( (bddcircuit *)0, Element->KEY,
                         (bddindex    )0, BDD_IN_MODE_LAST );
      }
    }

    autfreeblock( IndexArray );
    destroyauthtable( FsmHashTableOrder );
  }

  FsmFigure->CIRCUIT = BddCircuit;
}
