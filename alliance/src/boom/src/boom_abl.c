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
| Tool    :                     BOOM                          |
|                                                             |
| File    :                   boom_abl.c                      |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   06.06.00                        |
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
# include "abe.h"
# include "abv.h"
# include "abt.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "boom_shared.h"
# include "boom_error.h"
# include "boom_abl.h"
# include "boom_hash.h"
# include "boom_debug.h"
# include "boom_error.h"

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

  static boomhabltable *BoomHashAblTable  = (boomhabltable *)0;
  static char         **BoomAblNameArray  = (char        **)0;
  static bddindex      *BoomAblIndexArray = (bddindex     *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

   extern bddnode *loc_applybdd();
   extern bddnode *loc_simpbdddcon();
   extern bddnode *loc_simpbdddcoff();
   extern bddnode *loc_applybddnot();

/*------------------------------------------------------------\
|                                                             |
|                        BoomGiveBestExpr                     |
|                                                             |
\------------------------------------------------------------*/

static chain_list *BoomGiveBestExpr( Expr1, Expr2 )

  chain_list *Expr1;
  chain_list *Expr2;
{
  if ( getablexprnumatom( Expr1 ) >
       getablexprnumatom( Expr2 ) )
  {
    freeablexpr( Expr1 );

    return( Expr2 );
  }

  freeablexpr( Expr2 );

  return( Expr1 );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Index 2 Abl                       |
|                                                             |
\------------------------------------------------------------*/

static chain_list *BoomBddIndex2Abl( Index )

  bddindex Index;
{
  ablexpr *Atom;
  char    *Name;

  if ( Index < BDD_INDEX_MIN )
  {
    if ( Index == BDD_INDEX_ZERO )
    {
      return( createablatom( "'0'" ) );
    }

    return( createablatom( "'1'" ) );
  }

  Index = Index - BDD_INDEX_MIN;

  if ( BoomAblIndexArray != (bddindex *)0 )
  {
    Index = BoomAblIndexArray[ Index ];
  }

  Name = BoomAblNameArray[ Index ];
  Atom = createablatom( Name );

  Name = isbddstablename( Name );

  if ( Name != (char *)0 )
  {
    ABL_CAR_L( Atom ) = addchain( (chain_list *)0, (void *)ABL_STABLE );
    addablhexpr( Atom, createablatom( Name ) );
  }

  return( Atom );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Node 2 Mux                        |
|                                                             |
\------------------------------------------------------------*/

static chain_list *BoomBddNode2Mux( Equation, BddHigh, BddLow )

  chain_list *Equation;
  bddnode    *BddHigh;
  bddnode    *BddLow;
{
  bddnode    *NodeAnd;
  bddnode    *NodeAux1;
  bddnode    *NodeAux2;
  chain_list *Equation1;
  chain_list *Equation2;
  chain_list *Equation3;
  short       IndexHigh;
  short       IndexLow;

  IndexHigh = BddHigh->INDEX;
  IndexLow  = BddLow->INDEX;

/*
**  Terminal multiplexor
*/
  if ( ( IndexLow  < BDD_INDEX_MIN ) ||
       ( IndexHigh < BDD_INDEX_MIN ) )
  {
    if ( ( IndexLow  < BDD_INDEX_MIN ) &&
         ( IndexHigh < BDD_INDEX_MIN ) )
    {
/*
**  F = a'
*/
      if ( IndexLow == BDD_INDEX_ONE )
      {
        Equation = createablnotexpr( Equation );
      }
/*
**  F = a
*/
      return( Equation );
    }
/*
**  F = a' + H
*/
    if ( IndexLow == BDD_INDEX_ONE )
    {
      Equation = createablbinexpr( ABL_OR,
                                   createablnotexpr( Equation ),
                                   BoomBddNode2Abl( BddHigh ) );
    }
    else
/*
**  F = a.H
*/
    if ( IndexLow == BDD_INDEX_ZERO )
    {
      Equation = createablbinexpr( ABL_AND,
                                   Equation,
                                   BoomBddNode2Abl( BddHigh ) );
    }
    else
/*
**  F = a + L
*/
    if ( IndexHigh == BDD_INDEX_ONE )
    {
      Equation = createablbinexpr( ABL_OR,
                                   Equation,
                                   BoomBddNode2Abl( BddLow ) );
    }
    else
/*
**  F = a'.L
*/
    {
      Equation = createablbinexpr( ABL_AND,
                                   createablnotexpr( Equation ),
                                   BoomBddNode2Abl( BddLow ) );
    }

    return( Equation );
  }

  NodeAnd = decbddrefext( loc_applybdd( ABL_AND, BddHigh, BddLow ) );

  if ( NodeAnd->INDEX == BDD_INDEX_ZERO )
  {
    NodeAux1 = decbddrefext( loc_applybdd( ABL_OR, BddHigh, BddLow ) );
/*
**  F = a ^ L
*/
    if ( NodeAux1->INDEX == BDD_INDEX_ONE )
    {
      Equation = createablbinexpr( ABL_XOR,
                                   Equation,
                                   BoomBddNode2Abl( BddLow ) );
      return( Equation );
    }
  }
/*
**  F = H + a'.L(H=*)
*/
  if ( NodeAnd == BddHigh )
  {
    NodeAux1 = decbddrefext( loc_simpbdddcon( BddLow, BddHigh ) );

    Equation1 = BoomBddNode2Abl( BddHigh );
    Equation2 = createablbinexpr( ABL_AND,
                                  createablnotexpr( dupablexpr( Equation ) ),
                                  BoomBddNode2Abl( NodeAux1 ) );
                    
    Equation3 = createablbinexpr( ABL_OR, Equation1, Equation2 );

    NodeAux2 = decbddrefext( loc_applybddnot( BddLow ) );
    NodeAux2 = decbddrefext( loc_simpbdddcoff( BddHigh, NodeAux2 ) );

    Equation1 = BoomBddNode2Abl( BddLow );
    Equation2 = createablbinexpr( ABL_OR,
                                  createablnotexpr( Equation ),
                                  BoomBddNode2Abl( NodeAux2 ) );

    Equation = createablbinexpr( ABL_AND, Equation1, Equation2 );

    Equation = BoomGiveBestExpr( Equation, Equation3 );
    
    return( Equation );
  }
/*
**  F = L + a.H(L=*)
*/
  if ( NodeAnd == BddLow )
  {
    NodeAux1 = decbddrefext( loc_simpbdddcon( BddHigh, BddLow ) );

    Equation1 = BoomBddNode2Abl( BddLow );
    Equation2 = createablbinexpr( ABL_AND,
                                  dupablexpr( Equation ),
                                  BoomBddNode2Abl( NodeAux1 ) );

    Equation3 = createablbinexpr( ABL_OR, Equation1, Equation2 );

    NodeAux2 = decbddrefext( loc_applybddnot( BddHigh ) );
    NodeAux2 = decbddrefext( loc_simpbdddcoff( BddLow, NodeAux2 ) );

    Equation1 = BoomBddNode2Abl( BddHigh );
    Equation2 = createablbinexpr( ABL_OR,
                                  Equation,
                                  BoomBddNode2Abl( NodeAux2 ) );

    Equation = createablbinexpr( ABL_AND, Equation1, Equation2 );

    Equation = BoomGiveBestExpr( Equation, Equation3 );
    
    return( Equation );
  }
/*
**  F = a'.L(H.L=*) + a.H(H.L=*) + H.L(PHL=*), PHL = L(H.L=*).H(H.L=*)
*/
  NodeAux1 = decbddrefext( loc_simpbdddcon( BddLow , NodeAnd ) ); 
  NodeAux2 = decbddrefext( loc_simpbdddcon( BddHigh, NodeAnd ) );

  if ( ( NodeAux1 != BddLow  ) &&
       ( NodeAux2 != BddHigh ) )
  {
    Equation1 = createablbinexpr( ABL_AND,
                                  createablnotexpr( dupablexpr( Equation ) ),
                                  BoomBddNode2Abl( NodeAux1 ) );

    Equation2 = createablbinexpr( ABL_AND,
                                  dupablexpr( Equation ),
                                  BoomBddNode2Abl( NodeAux2 ) );

    NodeAux1 = decbddrefext( loc_applybdd( ABL_AND, NodeAux1, NodeAux2 ) );
    NodeAux1 = decbddrefext( loc_simpbdddcon( NodeAnd, NodeAux1 ) );

    Equation1 = createablbinexpr( ABL_OR, Equation1, Equation2 );

    Equation3 = createablbinexpr( ABL_OR, 
                                  Equation1,
                                  BoomBddNode2Abl( NodeAux1 ) );

    Equation1 = createablbinexpr( ABL_AND,
                                  createablnotexpr( dupablexpr( Equation ) ),
                                  BoomBddNode2Abl( BddLow ) );

    Equation2 = createablbinexpr( ABL_AND,
                                  Equation,
                                  BoomBddNode2Abl( BddHigh ) );

    Equation = createablbinexpr( ABL_OR, Equation1, Equation2 );

    Equation = BoomGiveBestExpr( Equation, Equation3 );
  }
  else
  {
    Equation1 = createablbinexpr( ABL_AND,
                                  createablnotexpr( dupablexpr( Equation ) ),
                                  BoomBddNode2Abl( BddLow ) );

    Equation2 = createablbinexpr( ABL_AND,
                                  Equation,
                                  BoomBddNode2Abl( BddHigh ) );

    Equation = createablbinexpr( ABL_OR, Equation1, Equation2 );
  }

  return( Equation );
}

/*------------------------------------------------------------\
|                                                             |
|                       BoomBddNode2Abl                       |
|                                                             |
\------------------------------------------------------------*/

chain_list *BoomBddNode2Abl( BddNode )

  bddnode *BddNode;
{
  boomhabl    *HashAbl;
  chain_list *Equation;
  chain_list *Atom;
  chain_list *Cumul;
  bddnode    *ScanNode;
  bddnode    *BddHigh;
  bddnode    *BddLow;
/*
**  F = '1'
*/
  if ( BddNode->INDEX == BDD_INDEX_ONE )
  {
    return( createablatom( "'1'" ) );
  }
/*
**  F = '0'
*/
  if ( BddNode->INDEX == BDD_INDEX_ZERO )
  {
    return( createablatom( "'0'" ) );
  }

  BddHigh = BddNode->HIGH;
  BddLow  = BddNode->LOW;

/*
**  Terminal Variable
*/
  if ( ( BddLow->INDEX  < BDD_INDEX_MIN ) ||
       ( BddHigh->INDEX < BDD_INDEX_MIN ) )
  {
    Atom = BoomBddIndex2Abl( BddNode->INDEX );

    if ( ( BddLow->INDEX  < BDD_INDEX_MIN ) &&
         ( BddHigh->INDEX < BDD_INDEX_MIN ) )
    {
      if ( BddLow->INDEX == BDD_INDEX_ONE )
      {
/*
**  F = a
*/
        Atom = createablnotexpr( Atom );
      }
/*
**  F = a'
*/
      return( Atom );
    }
/*
**  F = a' + H
*/
    if ( BddLow->INDEX == BDD_INDEX_ONE )
    {
      Equation = createablbinexpr( ABL_OR,
                                   createablnotexpr( Atom ),
                                   BoomBddNode2Abl( BddHigh ) );
    }
    else
/*
**  F = a.H
*/
    if ( BddLow->INDEX == BDD_INDEX_ZERO )
    {
      Equation = createablbinexpr( ABL_AND,
                                   Atom,
                                   BoomBddNode2Abl( BddHigh ) );
    }
    else
/*
**  F = a + L
*/
    if ( BddHigh->INDEX == BDD_INDEX_ONE )
    {
      Equation = createablbinexpr( ABL_OR,
                                   Atom,
                                   BoomBddNode2Abl( BddLow ) );
    }
/*
**  F = a'.L
*/
    else
    {
      Equation = createablbinexpr( ABL_AND,
                                   createablnotexpr( Atom ),
                                   BoomBddNode2Abl( BddLow ) );
    }

    BoomAddHashAbl( BoomHashAblTable, BddNode, Equation );

    return( Equation );
  }

  HashAbl = BoomSearchHashAbl( BoomHashAblTable, BddNode );

  if ( HashAbl != (boomhabl *)0 )
  {
    return( dupablexpr( HashAbl->ABL ) );
  }
/*
**  Simplify First Level
*/
  Equation = BoomBddIndex2Abl( BddNode->INDEX );
  ScanNode = (bddnode    *)0;

  do
  {
    Cumul = (chain_list *)0;
/*
**  Kernel OR
*/
    if ( ( BddHigh == BddLow->HIGH ) ||
         ( BddHigh == BddLow->LOW  ) )
    {
      Cumul = createabloper( ABL_OR );
      addablhexpr( Cumul, Equation );

      ScanNode = BddLow;

      do
      {
        Atom = BoomBddIndex2Abl( ScanNode->INDEX );

        if ( ScanNode->HIGH == BddHigh )
        {
          ScanNode = ScanNode->LOW;
        }
        else
        {
          ScanNode = ScanNode->HIGH;
          Atom     = createablnotexpr( Atom );
        }

        addablqexpr( Cumul, Atom );
      }
      while ( ( ScanNode->HIGH == BddHigh ) ||
              ( ScanNode->LOW  == BddHigh ) );

      BddLow   = ScanNode;
      Equation = Cumul;
    }
/*
**  Kernel AND
*/
    if ( ( BddLow == BddHigh->HIGH ) ||
         ( BddLow == BddHigh->LOW  ) )
    {
      Cumul = createabloper( ABL_AND );
      addablhexpr( Cumul, Equation );

      ScanNode = BddHigh;

      do
      {
        Atom = BoomBddIndex2Abl( ScanNode->INDEX );

        if ( ScanNode->LOW == BddLow )
        {
          ScanNode = ScanNode->HIGH;
        }
        else
        {
          ScanNode = ScanNode->LOW;
          Atom     = createablnotexpr( Atom );
        }

        addablqexpr( Cumul, Atom );
      }
      while ( ( ScanNode->HIGH == BddLow ) ||
              ( ScanNode->LOW  == BddLow ) );

      BddHigh  = ScanNode;
      Equation = Cumul;
    }
/*
**  Kernel XOR
*/
    if ( ( BddLow->INDEX  == BddHigh->INDEX ) &&
         ( BddHigh->HIGH  == BddLow->LOW    ) &&
         ( BddHigh->LOW   == BddLow->HIGH   ) )
    {
      Cumul = createabloper( ABL_XOR );
      addablhexpr( Cumul, Equation );

      ScanNode = BddLow;

      Atom = BoomBddIndex2Abl( ScanNode->INDEX );
      addablqexpr( Cumul, Atom );

      BddHigh = BddHigh->LOW;
      BddLow  = BddLow->LOW;

      Equation = Cumul;
    }

    if ( Cumul == (chain_list *)0 ) break;
  }
  while ( ( BddHigh->INDEX >= BDD_INDEX_MIN ) &&
          ( BddLow->INDEX  >= BDD_INDEX_MIN ) );

/*
**  Simplify Second Level
*/
  if ( BddHigh->INDEX == BddLow->INDEX )
  {
    if ( ( BddHigh->LOW        == BddLow->HIGH  ) &&
         ( BddHigh->LOW->INDEX >= BDD_INDEX_MIN ) )
    {
/*
**  Carry
*/
      if ( ( BddHigh->HIGH->INDEX == BDD_INDEX_ONE  ) &&
           ( BddLow->LOW->INDEX   == BDD_INDEX_ZERO ) )
      {
        Atom  = BoomBddIndex2Abl( BddHigh->INDEX );
        Cumul = createablbinexpr( ABL_AND,  
                                  dupablexpr( Atom     ),
                                  dupablexpr( Equation ) );

        Atom  = createablbinexpr( ABL_OR, Atom, Equation );

        Equation = BoomBddNode2Abl( BddHigh->LOW );
/*
**  ( A.B + HL ).( A + B )
*/
        if ( ( ! ABL_ATOM( Equation )           ) &&
             (   ABL_OPER( Equation ) == ABL_OR ) )
        {
          Cumul    = createablbinexpr( ABL_OR , Cumul, Equation );
          Equation = createablbinexpr( ABL_AND, Cumul, Atom     );
        }
/*
**  A.B + HL.( A + B )
*/
        else
        {
          Atom     = createablbinexpr( ABL_AND, Atom , Equation );
          Equation = createablbinexpr( ABL_OR , Cumul, Atom     );
        }

        BoomAddHashAbl( BoomHashAblTable, BddNode, Equation );

        return( Equation );
      }

      if ( ( BddHigh->HIGH->INDEX == BDD_INDEX_ZERO ) &&
           ( BddLow->LOW->INDEX   == BDD_INDEX_ONE  ) )
      {
        Equation = createablnotexpr( Equation );
        Atom     = createablnotexpr( BoomBddIndex2Abl( BddHigh->INDEX ) );

        Cumul = createablbinexpr( ABL_AND,
                                  dupablexpr( Atom     ),
                                  dupablexpr( Equation ) );

        Atom  = createablbinexpr( ABL_OR, Atom, Equation );

        Equation = BoomBddNode2Abl( BddHigh->LOW );
/*
**  ( A'.B' + HL ).( A' + B' )
*/
        if ( ( ! ABL_ATOM( Equation )           ) &&
             (   ABL_OPER( Equation ) == ABL_OR ) )
        {
          Cumul    = createablbinexpr( ABL_OR , Cumul, Equation );
          Equation = createablbinexpr( ABL_AND, Cumul, Atom     );
        }
/*
**  A'.B' + HL.( A' + B' )
*/
        else
        {
          Atom     = createablbinexpr( ABL_AND, Atom , Equation );
          Equation = createablbinexpr( ABL_OR , Cumul, Atom     );
        }

        BoomAddHashAbl( BoomHashAblTable, BddNode, Equation );

        return( Equation );
      }
    }

    if ( ( BddHigh->HIGH        == BddLow->LOW   ) &&
         ( BddHigh->HIGH->INDEX >= BDD_INDEX_MIN ) )
    {
/*
**  Carry
*/
      if ( ( BddHigh->LOW->INDEX == BDD_INDEX_ONE  ) &&
           ( BddLow->HIGH->INDEX == BDD_INDEX_ZERO ) )
      {
        Atom  = createablnotexpr( BoomBddIndex2Abl( BddHigh->INDEX ) );
        Cumul = createablbinexpr( ABL_AND,  
                                  dupablexpr( Atom     ),
                                  dupablexpr( Equation ) );

        Atom  = createablbinexpr( ABL_OR, Atom, Equation );

        Equation = BoomBddNode2Abl( BddHigh->HIGH );
/*
**  ( A.B' + HH ).( A + B' )
*/
        if ( ( ! ABL_ATOM( Equation )           ) &&
             (   ABL_OPER( Equation ) == ABL_OR ) )
        {
          Cumul    = createablbinexpr( ABL_OR , Cumul, Equation );
          Equation = createablbinexpr( ABL_AND, Cumul, Atom     );
        }
/*
**  A.B' + HH.( A + B' )
*/
        else
        {
          Atom     = createablbinexpr( ABL_AND, Atom , Equation );
          Equation = createablbinexpr( ABL_OR , Cumul, Atom     );
        }

        BoomAddHashAbl( BoomHashAblTable, BddNode, Equation );

        return( Equation );
      }

      if ( ( BddHigh->LOW->INDEX == BDD_INDEX_ZERO ) &&
           ( BddLow->HIGH->INDEX == BDD_INDEX_ONE  ) )
      {
        Equation = createablnotexpr( Equation );
        Atom     = BoomBddIndex2Abl( BddHigh->INDEX );

        Cumul = createablbinexpr( ABL_AND,
                                  dupablexpr( Atom     ),
                                  dupablexpr( Equation ) );

        Atom  = createablbinexpr( ABL_OR, Atom, Equation );

        Equation = BoomBddNode2Abl( BddHigh->HIGH );
/*
**  ( A'.B + HL ).( A' + B )
*/
        if ( ( ! ABL_ATOM( Equation )           ) &&
             (   ABL_OPER( Equation ) == ABL_OR ) )
        {
          Cumul    = createablbinexpr( ABL_OR , Cumul, Equation );
          Equation = createablbinexpr( ABL_AND, Cumul, Atom     );
        }
/*
**  A'.B + HL.( A' + B )
*/
        else
        {
          Atom     = createablbinexpr( ABL_AND, Atom , Equation );
          Equation = createablbinexpr( ABL_OR , Cumul, Atom     );
        }

        BoomAddHashAbl( BoomHashAblTable, BddNode, Equation );

        return( Equation );
      }
    }
  }

  Equation = BoomBddNode2Mux( Equation, BddHigh, BddLow );

  BoomAddHashAbl( BoomHashAblTable, BddNode, Equation );

  return( Equation );
}



/*------------------------------------------------------------\
|                                                             |
|                     BoomBehBddNode2Abl                      |
|                                                             |
\------------------------------------------------------------*/

void BoomBehBddNode2Abl( BehFigure )

  befig_list *BehFigure;
{
  bddcircuit  *BddCircuit;
  bddsystem   *BddSystem;
  beout_list  *BehOut;
  beaux_list  *BehAux;
  bebux_list  *BehBux;
  bebus_list  *BehBus;
  bereg_list  *BehReg;
  bddnode     *BddNode;
  binode_list *BiNode;
  biabl_list  *BiAbl;
  chain_list  *Equation;

  if ( IsBoomDebugLevel1() )
  {
    BoomPrintf( stdout, "--> BoomBehBddNode2Abl !\n" );
  }

  BddCircuit = BehFigure->CIRCUI;
  BddSystem  = BddCircuit->BDD_SYSTEM;

  setbddlocalcircuit( BddCircuit );
  setbddlocalsystem( BddSystem );

  BoomAblNameArray  = BddCircuit->NAME_IN;
  BoomAblIndexArray = BddCircuit->INDEX_IN;

  if ( BoomHashAblTable == (boomhabltable *)0 )
  {
    BoomHashAblTable = BoomCreateHashAblTable( 10000 );
  }

  for ( BehAux  = BehFigure->BEAUX;
        BehAux != (beaux_list *)0;
        BehAux  = BehAux->NEXT )
  {
    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "BehAux %s !\n", BehAux->NAME );
    }

    if ( IsBoomBehAuxDontTouch( BehAux ) ) continue;

    BddNode  = BehAux->NODE;
    Equation = polarablexpr( BoomBddNode2Abl( BddNode ), 1 );

    freeablexpr( BehAux->ABL );
    BehAux->ABL = Equation;

    if ( IsBoomDebugLevel1() )
    {
      viewablexprln( BehAux->ABL, ABL_VIEW_VHDL );
    }
  }

  for ( BehOut  = BehFigure->BEOUT;
        BehOut != (beout_list *)0;
        BehOut  = BehOut->NEXT )
  {
    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "BehOut %s !\n", BehOut->NAME );
    }
    if ( IsBoomBehAuxDontTouch( BehOut ) ) continue;

    BddNode  = BehOut->NODE;
    Equation = polarablexpr( BoomBddNode2Abl( BddNode ), 1 );

    freeablexpr( BehOut->ABL );
    BehOut->ABL = Equation;

    if ( IsBoomDebugLevel1() )
    {
      viewablexprln( Equation, ABL_VIEW_VHDL );
    }
  }

  for ( BehBus  = BehFigure->BEBUS;
        BehBus != (bebus_list *)0;
        BehBus  = BehBus->NEXT )
  {
    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "BehBus %s !\n", BehBus->NAME );
    }

    if ( IsBoomBehAuxDontTouch( BehBus ) ) continue;

    BiAbl = BehBus->BIABL;

    for ( BiNode  = BehBus->BINODE;
          BiNode != (binode_list *)0;
          BiNode  = BiNode->NEXT )
    {
      if ( BiNode->VALNODE != (bddnode *)0 )
      {
        BddNode  = BiNode->VALNODE;
        Equation = polarablexpr( BoomBddNode2Abl( BddNode ), 1 );

        freeablexpr( BiAbl->VALABL );
        BiAbl->VALABL = Equation;

        if ( IsBoomDebugLevel1() )
        {
          viewablexprln( Equation, ABL_VIEW_VHDL );
        }
      }

      if ( BiNode->CNDNODE != (bddnode *)0 )
      {
        BddNode  = BiNode->CNDNODE;
        Equation = polarablexpr( BoomBddNode2Abl( BddNode ), 1 );

        freeablexpr( BiAbl->CNDABL );
        BiAbl->CNDABL = Equation;

        if ( IsBoomDebugLevel1() )
        {
          viewablexprln( Equation, ABL_VIEW_VHDL );
        }
      }

      BiAbl = BiAbl->NEXT;
    }
  }

  for ( BehBux  = BehFigure->BEBUX;
        BehBux != (bebux_list *)0;
        BehBux  = BehBux->NEXT )
  {
    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "BehBux %s !\n", BehBux->NAME );
    }

    if ( IsBoomBehAuxDontTouch( BehBux ) ) continue;

    BiAbl = BehBux->BIABL;

    for ( BiNode  = BehBux->BINODE;
          BiNode != (binode_list *)0;
          BiNode  = BiNode->NEXT )
    {
      if ( BiNode->VALNODE != (bddnode *)0 )
      {
        BddNode  = BiNode->VALNODE;
        Equation = polarablexpr( BoomBddNode2Abl( BddNode ), 1 );

        freeablexpr( BiAbl->VALABL );
        BiAbl->VALABL = Equation;

        if ( IsBoomDebugLevel1() )
        {
          viewablexprln( Equation, ABL_VIEW_VHDL );
        }
      }

      if ( BiNode->CNDNODE != (bddnode *)0 )
      {
        BddNode  = BiNode->CNDNODE;
        Equation = polarablexpr( BoomBddNode2Abl( BddNode ), 1 );

        freeablexpr( BiAbl->CNDABL );
        BiAbl->CNDABL = Equation;

        if ( IsBoomDebugLevel1() )
        {
          viewablexprln( Equation, ABL_VIEW_VHDL );
        }
      }

      BiAbl = BiAbl->NEXT;
    }
  }

  for ( BehReg  = BehFigure->BEREG;
        BehReg != (bereg_list *)0;
        BehReg  = BehReg->NEXT )
  {
    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "BehReg %s !\n", BehReg->NAME );
    }

    if ( IsBoomBehAuxDontTouch( BehReg ) ) continue;

    BiAbl = BehReg->BIABL;

    for ( BiNode  = BehReg->BINODE;
          BiNode != (binode_list *)0;
          BiNode  = BiNode->NEXT )
    {
      if ( BiNode->VALNODE != (bddnode *)0 )
      {
        BddNode  = BiNode->VALNODE;
        Equation = polarablexpr( BoomBddNode2Abl( BddNode ), 1 );

        freeablexpr( BiAbl->VALABL );
        BiAbl->VALABL = Equation;

        if ( IsBoomDebugLevel1() )
        {
          viewablexprln( Equation, ABL_VIEW_VHDL );
        }
      }

      if ( BiNode->CNDNODE != (bddnode *)0 )
      {
        BddNode  = BiNode->CNDNODE;
        Equation = polarablexpr( BoomBddNode2Abl( BddNode ), 1 );

        freeablexpr( BiAbl->CNDABL );
        BiAbl->CNDABL = Equation;

        if ( IsBoomDebugLevel1() )
        {
          viewablexprln( Equation, ABL_VIEW_VHDL );
        }
      }

      BiAbl = BiAbl->NEXT;
    }
  }

  BoomResetHashAblTable( BoomHashAblTable );

  if ( IsBoomDebugLevel1() )
  {
    BoomPrintf( stdout, "<-- BoomBehBddNode2Abl !\n" );
  }

}

/*------------------------------------------------------------\
|                                                             |
|                      BoomBehMapAbl                          |
|                                                             |
\------------------------------------------------------------*/

void BoomBehMapAbl( BehFigure, Function, All )

  befig_list *BehFigure;
  chain_list *(*Function)();
  int         All;
{
  beout_list  *BehOut;
  beaux_list  *BehAux;
  bebux_list  *BehBux;
  bebus_list  *BehBus;
  bereg_list  *BehReg;
  biabl_list  *BiAbl;

  for ( BehOut  = BehFigure->BEOUT;
        BehOut != (beout_list *)0;
        BehOut  = BehOut->NEXT )
  {
    if ( ( ! All ) && ( IsBoomBehAuxDontTouch( BehOut ) ) ) continue;

    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "BehOut %s\n", BehOut->NAME );
    }

    BehOut->ABL = Function( BehOut->ABL );
  }

  for ( BehBus  = BehFigure->BEBUS;
        BehBus != (bebus_list *)0;
        BehBus  = BehBus->NEXT )
  {
    if ( ( ! All ) && ( IsBoomBehAuxDontTouch( BehBus ) ) ) continue;

    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "BehBus %s\n", BehBus->NAME );
    }

    BiAbl = BehBus->BIABL;

    for ( BiAbl  = BehBus->BIABL;
          BiAbl != (biabl_list *)0;
          BiAbl  = BiAbl->NEXT )
    {
      if ( BiAbl->VALABL != (chain_list *)0 )
      {
        BiAbl->VALABL = Function( BiAbl->VALABL );
      }

      if ( BiAbl->CNDABL != (chain_list *)0 )
      {
        BiAbl->CNDABL = Function( BiAbl->CNDABL );
      }
    }
  }

  for ( BehBux  = BehFigure->BEBUX;
        BehBux != (bebux_list *)0;
        BehBux  = BehBux->NEXT )
  {
    if ( ( ! All ) && ( IsBoomBehAuxDontTouch( BehBux ) ) ) continue;

    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "BehBux %s\n", BehBux->NAME );
    }

    BiAbl = BehBux->BIABL;

    for ( BiAbl  = BehBux->BIABL;
          BiAbl != (biabl_list *)0;
          BiAbl  = BiAbl->NEXT )
    {
      if ( BiAbl->VALABL != (chain_list *)0 )
      {
        BiAbl->VALABL = Function( BiAbl->VALABL );
      }

      if ( BiAbl->CNDABL != (chain_list *)0 )
      {
        BiAbl->CNDABL = Function( BiAbl->CNDABL );
      }
    }
  }

  for ( BehReg  = BehFigure->BEREG;
        BehReg != (bereg_list *)0;
        BehReg  = BehReg->NEXT )
  {
    if ( ( ! All ) && ( IsBoomBehAuxDontTouch( BehReg ) ) ) continue;

    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "BehReg %s\n", BehReg->NAME );
    }

    BiAbl = BehReg->BIABL;

    for ( BiAbl  = BehReg->BIABL;
          BiAbl != (biabl_list *)0;
          BiAbl  = BiAbl->NEXT )
    {
      if ( BiAbl->VALABL != (chain_list *)0 )
      {
        BiAbl->VALABL = Function( BiAbl->VALABL );
      }

      if ( BiAbl->CNDABL != (chain_list *)0 )
      {
        BiAbl->CNDABL = Function( BiAbl->CNDABL );
      }
    }
  }

  for ( BehAux  = BehFigure->BEAUX;
        BehAux != (beaux_list *)0;
        BehAux  = BehAux->NEXT )
  {
    if ( ( ! All ) && ( IsBoomBehAuxDontTouch( BehAux ) ) ) continue;

    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "BehAux %s\n", BehAux->NAME );
    }

    BehAux->ABL = Function( BehAux->ABL );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomBehScanAbl                         |
|                                                             |
\------------------------------------------------------------*/

void BoomBehScanAbl( BehFigure, Function, All )

  befig_list *BehFigure;
  void       (*Function)();
  int         All;
{
  beout_list  *BehOut;
  beaux_list  *BehAux;
  bebux_list  *BehBux;
  bebus_list  *BehBus;
  bereg_list  *BehReg;
  biabl_list  *BiAbl;

  for ( BehOut  = BehFigure->BEOUT;
        BehOut != (beout_list *)0;
        BehOut  = BehOut->NEXT )
  {
    if ( ( ! All ) && ( IsBoomBehAuxDontTouch( BehOut ) ) ) continue;

    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "BehOut %s\n", BehOut->NAME );
    }

    Function( BehOut->ABL );
  }

  for ( BehBus  = BehFigure->BEBUS;
        BehBus != (bebus_list *)0;
        BehBus  = BehBus->NEXT )
  {
    if ( ( ! All ) && ( IsBoomBehAuxDontTouch( BehBus ) ) ) continue;

    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "BehBus %s\n", BehBus->NAME );
    }

    BiAbl = BehBus->BIABL;

    for ( BiAbl  = BehBus->BIABL;
          BiAbl != (biabl_list *)0;
          BiAbl  = BiAbl->NEXT )
    {
      if ( BiAbl->VALABL != (chain_list *)0 )
      {
        Function( BiAbl->VALABL );
      }

      if ( BiAbl->CNDABL != (chain_list *)0 )
      {
        Function( BiAbl->CNDABL );
      }
    }
  }

  for ( BehBux  = BehFigure->BEBUX;
        BehBux != (bebux_list *)0;
        BehBux  = BehBux->NEXT )
  {
    if ( ( ! All ) && ( IsBoomBehAuxDontTouch( BehBux ) ) ) continue;

    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "BehBux %s\n", BehBux->NAME );
    }

    BiAbl = BehBux->BIABL;

    for ( BiAbl  = BehBux->BIABL;
          BiAbl != (biabl_list *)0;
          BiAbl  = BiAbl->NEXT )
    {
      if ( BiAbl->VALABL != (chain_list *)0 )
      {
        Function( BiAbl->VALABL );
      }

      if ( BiAbl->CNDABL != (chain_list *)0 )
      {
        Function( BiAbl->CNDABL );
      }
    }
  }

  for ( BehReg  = BehFigure->BEREG;
        BehReg != (bereg_list *)0;
        BehReg  = BehReg->NEXT )
  {
    if ( ( ! All ) && ( IsBoomBehAuxDontTouch( BehReg ) ) ) continue;

    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "BehReg %s\n", BehReg->NAME );
    }

    BiAbl = BehReg->BIABL;

    for ( BiAbl  = BehReg->BIABL;
          BiAbl != (biabl_list *)0;
          BiAbl  = BiAbl->NEXT )
    {
      if ( BiAbl->VALABL != (chain_list *)0 )
      {
        Function( BiAbl->VALABL );
      }

      if ( BiAbl->CNDABL != (chain_list *)0 )
      {
        Function( BiAbl->CNDABL );
      }
    }
  }

  for ( BehAux  = BehFigure->BEAUX;
        BehAux != (beaux_list *)0;
        BehAux  = BehAux->NEXT )
  {
    if ( ( ! All ) && ( IsBoomBehAuxDontTouch( BehAux ) ) ) continue;

    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "BehAux %s\n", BehAux->NAME );
    }

    Function( BehAux->ABL );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomBehMapAblWithName                  |
|                                                             |
\------------------------------------------------------------*/

void BoomBehMapAblWithName( BehFigure, Function )

  befig_list *BehFigure;
  chain_list *(*Function)();
{
  beout_list  *BehOut;
  beaux_list  *BehAux;
  bebux_list  *BehBux;
  bebus_list  *BehBus;
  bereg_list  *BehReg;
  biabl_list  *BiAbl;

  for ( BehOut  = BehFigure->BEOUT;
        BehOut != (beout_list *)0;
        BehOut  = BehOut->NEXT )
  {
    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "BehOut %s\n", BehOut->NAME );
    }

    BehOut->ABL = Function( BehOut->NAME, BehOut->ABL );
  }

  for ( BehBus  = BehFigure->BEBUS;
        BehBus != (bebus_list *)0;
        BehBus  = BehBus->NEXT )
  {
    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "BehBus %s\n", BehBus->NAME );
    }

    BiAbl = BehBus->BIABL;

    for ( BiAbl  = BehBus->BIABL;
          BiAbl != (biabl_list *)0;
          BiAbl  = BiAbl->NEXT )
    {
      if ( BiAbl->VALABL != (chain_list *)0 )
      {
        BiAbl->VALABL = Function( BehBus->NAME, BiAbl->VALABL );
      }

      /*
      if ( BiAbl->CNDABL != (chain_list *)0 )
      {
        BiAbl->CNDABL = Function( BiAbl->CNDABL );
      }
      */
    }
  }

  for ( BehBux  = BehFigure->BEBUX;
        BehBux != (bebux_list *)0;
        BehBux  = BehBux->NEXT )
  {
    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "BehBux %s\n", BehBux->NAME );
    }

    BiAbl = BehBux->BIABL;

    for ( BiAbl  = BehBux->BIABL;
          BiAbl != (biabl_list *)0;
          BiAbl  = BiAbl->NEXT )
    {
      if ( BiAbl->VALABL != (chain_list *)0 )
      {
        BiAbl->VALABL = Function( BehBux->NAME, BiAbl->VALABL );
      }

      /*
      if ( BiAbl->CNDABL != (chain_list *)0 )
      {
        BiAbl->CNDABL = Function( BiAbl->CNDABL );
      }
      */
    }
  }

  for ( BehReg  = BehFigure->BEREG;
        BehReg != (bereg_list *)0;
        BehReg  = BehReg->NEXT )
  {
    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "BehReg %s\n", BehReg->NAME );
    }

    BiAbl = BehReg->BIABL;

    for ( BiAbl  = BehReg->BIABL;
          BiAbl != (biabl_list *)0;
          BiAbl  = BiAbl->NEXT )
    {
      if ( BiAbl->VALABL != (chain_list *)0 )
      {
        BiAbl->VALABL = Function( BehReg->NAME, BiAbl->VALABL );
      }

      /*
      if ( BiAbl->CNDABL != (chain_list *)0 )
      {
        BiAbl->CNDABL = Function( BiAbl->CNDABL );
      }
      */
    }
  }

  for ( BehAux  = BehFigure->BEAUX;
        BehAux != (beaux_list *)0;
        BehAux  = BehAux->NEXT )
  {
    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "BehAux %s\n", BehAux->NAME );
    }

    BehAux->ABL = Function( BehAux->NAME, BehAux->ABL );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     BoomNormalizeAbl                        |
|                                                             |
\------------------------------------------------------------*/

# if 0
static void BoomSortAbl( expr, func, direction)

  chain_list *expr;
  long (*func) ();
  int direction;
{
  chain_list *expr1;
  int okPermu = 1;

  if (!ABL_ATOM (expr))		/* ce n'est pas un atome */
    {
      while (okPermu)
	{
	  expr1 = ABL_CDR (expr);
	  okPermu = 0;
	  while (ABL_CDR (expr1))
	    {
	      if ((direction == 1 &&
		   ((*func) ABL_CAR (expr1) > (*func) ABL_CADR (expr1))) ||
		  (direction == 0 &&
		   ((*func) ABL_CAR (expr1) < (*func) ABL_CADR (expr1))))
		{
		  chain_list *permu;

		  permu = ABL_CAR (expr1);
		  expr1->DATA = (void *) ABL_CADR (expr1);
		  (expr1->NEXT)->DATA = (void *) permu;
		  okPermu = 1;
		}
	      expr1 = ABL_CDR (expr1);
	    }
	}
    }
}

static int LocalBoomNormalizeAbl( Expr )

  ablexpr *Expr;
{
  if ( ABL_ATOM( Expr ) )
  {
    return ( (int)ABL_ATOM_VALUE( Expr ) );
  }

  if ( ABL_OPER (Expr) == ABL_NOT)
  {
    if (ABL_ATOM(ABL_CADR(Expr)))
    {
      return ( ABL_NOT * 100 + 10);
    }
    else
    {
      return (ABL_NOT * 100 + ABL_OPER(ABL_CADR (Expr)));
    }
  }

  return ABL_OPER(Expr) * 100 + getablexprlength(Expr);
}

static void BoomNormalizeAbl( Expr )

  ablexpr *Expr;
{
  if ( ! ABL_ATOM( Expr ) )
  {
    BoomSortAbl( Expr, LocalBoomNormalizeAbl, 1 );

    while ( ( Expr = ABL_CDR( Expr ) ) != (chain_list *)0 )
    {
      BoomNormalizeAbl( ABL_CAR( Expr ) );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     BoomBehNormalizeAbl                     |
|                                                             |
\------------------------------------------------------------*/

void BoomBehNormalizeAbl( BehFigure )

  befig_list *BehFigure;
{
  BoomBehMapAbl( BehFigure, BoomNormalizeAbl, 0 );
}
# endif

/*------------------------------------------------------------\
|                                                             |
|                     BoomBehUnflatAbl                        |
|                                                             |
\------------------------------------------------------------*/

void BoomBehUnflatAbl( BehFigure )

  befig_list *BehFigure;
{
  BoomBehMapAbl( BehFigure, unflatablexpr, 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                     BoomBehFlatAbl                          |
|                                                             |
\------------------------------------------------------------*/

void BoomBehFlatAbl( BehFigure )

  befig_list *BehFigure;
{
  BoomBehScanAbl( BehFigure, flatablexpr, 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                     BoomBehSimplifyAbl                      |
|                                                             |
\------------------------------------------------------------*/

void BoomBehSimplifyAbl( BehFigure )

  befig_list *BehFigure;
{
  BoomBehMapAbl( BehFigure, simpablexpr, 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomReverseAbl                         |
|                                                             |
\------------------------------------------------------------*/

static chain_list *BoomReverseAbl( Expr )

   chain_list *Expr;
{
  chain_list *ScanExpr;

  if ( ! ABL_ATOM( Expr ) )
  {
    if ( ABL_CDDR( Expr ) != (chain_list *)0 )
    {
      ABL_CDR( Expr ) = reverse( ABL_CDR( Expr ) );
    }

    ScanExpr = Expr;

    while ( ( ScanExpr = ABL_CDR( ScanExpr ) ) != (chain_list *)0 )
    {
      ABL_CAR_L( ScanExpr ) = BoomReverseAbl( ABL_CAR( ScanExpr ) );
    }
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomBehReverseAbl                      |
|                                                             |
\------------------------------------------------------------*/

void BoomBehReverseAbl( BehFigure )

  befig_list *BehFigure;
{
  BoomBehMapAbl( BehFigure, BoomReverseAbl, 0 );
}
