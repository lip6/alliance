/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     Bdd                           |
|                                                             |
| File    :                  bddconvert.c                     |
|                                                             |
| Date    :                   03.12.96                        |
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

# include <stdio.h>
# include "bddconvert.h"
# include "bddapply.h"
# include "bdderror.h"

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

  static char       **BddConvertNameArray  = (char      **)0;
  static bddindex    *BddConvertIndexArray = (bddindex   *)0;
  static chain_list  *BddConvertSumProd    = (chain_list *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   Local  Convert Bdd Index Abl              |
|                                                             |
\------------------------------------------------------------*/

static chain_list *loc_convertbddindex( Index )

  bddindex Index;
{
  if ( Index < BDD_INDEX_MIN )
  {
    if ( Index == BDD_INDEX_ZERO )
    {
      return( createablatomzero() );
    }

    return( createablatomone() );
  }

  Index = Index - BDD_INDEX_MIN;

  if ( BddConvertIndexArray != (bddindex *)0 )
  {
    Index = BddConvertIndexArray[ Index ];
  }

  return( createablatom( BddConvertNameArray[ Index ] ) );
}

/*------------------------------------------------------------\
|                                                             |
|                     Convert Bdd Index Abl                   |
|                                                             |
\------------------------------------------------------------*/

chain_list *convertbddindexabl( BddSystem, NameArray, IndexArray, Index )

  bddsystem  *BddSystem;
  char      **NameArray;
  bddindex   *IndexArray;
  bddindex    Index;
{
  setbddlocalsystem( BddSystem );

  BddConvertNameArray  = NameArray;
  BddConvertIndexArray = IndexArray;

  return( loc_convertbddindex( Index ) );
}

/*------------------------------------------------------------\
|                                                             |
|                 Local Convert Bdd Mux Abl                   |
|                                                             |
\------------------------------------------------------------*/

static chain_list *loc_convertbddmux( Equation, BddHigh, BddLow )

  chain_list *Equation;
  bddnode    *BddHigh;
  bddnode    *BddLow;
{
  bddnode    *NodeAnd;
  bddnode    *NodeOr;
  chain_list *EquationHigh;
  chain_list *EquationLow;
  bddindex    IndexHigh;
  bddindex    IndexLow;

  IndexHigh = BddHigh->INDEX;
  IndexLow  = BddLow->INDEX;

  if ( ( IndexLow  < BDD_INDEX_MIN ) ||
       ( IndexHigh < BDD_INDEX_MIN ) )
  {
    if ( ( IndexLow  < BDD_INDEX_MIN ) &&
         ( IndexHigh < BDD_INDEX_MIN ) )
    {
      if ( IndexLow == BDD_INDEX_ONE )
      {
        Equation = optimablnotexpr( Equation );
      }
  
      return( Equation );
    }

    if ( IndexLow == BDD_INDEX_ONE )
    {
      Equation = optimablbinexpr( ABL_OR,
                                  loc_convertbddnode( BddHigh ),
                                  optimablnotexpr( Equation ) );
    }
    else
    if ( IndexLow == BDD_INDEX_ZERO )
    {
      Equation = optimablbinexpr( ABL_AND,
                                   loc_convertbddnode( BddHigh ),
                                   Equation );
    }
    else
    if ( IndexHigh == BDD_INDEX_ONE )
    {
      Equation = optimablbinexpr( ABL_OR,
                                   loc_convertbddnode( BddLow ),
                                   Equation );
    }
    else
    {
      Equation = optimablbinexpr( ABL_AND,
                                   loc_convertbddnode( BddLow ),
                                   optimablnotexpr( Equation ) );
    }

    return( Equation );
  }

  NodeAnd = decbddrefext( loc_applybdd( ABL_AND, BddHigh, BddLow ) );

  if ( NodeAnd->INDEX == BDD_INDEX_ZERO )
  {
    NodeOr = decbddrefext( loc_applybdd( ABL_OR, BddHigh, BddLow ) );

    if ( NodeOr->INDEX == BDD_INDEX_ONE )
    {
      Equation = optimablbinexpr( ABL_XOR,
                                   loc_convertbddnode( BddLow ),
                                   Equation );
      return( Equation );
    }
  }

  if ( NodeAnd == BddHigh )
  {
    EquationLow  = optimablbinexpr( ABL_AND,
                                     loc_convertbddnode( BddLow ),
                                     optimablnotexpr( Equation ) );

    EquationHigh = loc_convertbddnode( BddHigh );
  }
  else
  if ( NodeAnd == BddLow )
  {
    EquationLow  = optimablbinexpr( ABL_AND,
                                     loc_convertbddnode( BddHigh ),
                                     Equation );

    EquationHigh = loc_convertbddnode( BddLow );
  }
  else
  {
    EquationLow  = optimablbinexpr( ABL_AND,
                                     loc_convertbddnode( BddLow ),
                                     optimablnotexpr( dupablexpr( Equation ) ) );

    EquationHigh = optimablbinexpr( ABL_AND,
                                     loc_convertbddnode( BddHigh ),
                                     Equation );
  }

  Equation = optimablbinexpr( ABL_OR, EquationLow, EquationHigh );

  return( Equation );
}
  
/*------------------------------------------------------------\
|                                                             |
|                      Convert Bdd Mux Abl                    |
|                                                             |
\------------------------------------------------------------*/

chain_list *convertbddmuxabl( BddSystem, NameArray, IndexArray, Equation, BddHigh, BddLow )

  bddsystem  *BddSystem;
  char      **NameArray;
  bddindex   *IndexArray;
  chain_list *Equation;
  bddnode    *BddHigh;
  bddnode    *BddLow;
{
  setbddlocalsystem( BddSystem );

  BddConvertNameArray  = NameArray;
  BddConvertIndexArray = IndexArray;

  return( loc_convertbddmux( Equation, BddHigh, BddLow ) );
}

/*------------------------------------------------------------\
|                                                             |
|                      Convert Node In Abl                    |
|                                                             |
\------------------------------------------------------------*/

chain_list *loc_convertbddnode( BddNode )

  bddnode *BddNode;
{
  chain_list *Equation;
  chain_list *Atom;
  bddnode    *ScanNode;
  bddindex    IndexLow;
  bddindex    IndexHigh;

  if ( BddNode->INDEX == BDD_INDEX_ONE )
  {
    return( createablatomone() );
  }

  if ( BddNode->INDEX == BDD_INDEX_ZERO )
  {
    return( createablatomzero() );
  }

  IndexLow  = BddNode->LOW->INDEX;
  IndexHigh = BddNode->HIGH->INDEX;

  if ( ( IndexLow  < BDD_INDEX_MIN ) ||
       ( IndexHigh < BDD_INDEX_MIN ) )
  {
    Atom = loc_convertbddindex( BddNode->INDEX );

    if ( ( IndexLow  < BDD_INDEX_MIN ) &&
         ( IndexHigh < BDD_INDEX_MIN ) )
    {
      if ( IndexLow == BDD_INDEX_ONE )
      {
        Atom = optimablnotexpr( Atom );
      }

      return( Atom );
    }

    if ( IndexLow == BDD_INDEX_ONE )
    {
      Equation = optimablbinexpr( ABL_OR, 
                                   loc_convertbddnode( BddNode->HIGH ),
                                   optimablnotexpr( Atom ) );
    }
    else
    if ( IndexLow == BDD_INDEX_ZERO )
    {
      Equation = optimablbinexpr( ABL_AND, 
                                   loc_convertbddnode( BddNode->HIGH ),
                                   Atom );
    }
    else
    if ( IndexHigh == BDD_INDEX_ONE )
    {
      Equation = optimablbinexpr( ABL_OR, 
                                   loc_convertbddnode( BddNode->LOW ),
                                   Atom );
    }
    else
    {
      Equation = optimablbinexpr( ABL_AND, 
                                   loc_convertbddnode( BddNode->LOW ),
                                   optimablnotexpr( Atom ) );
    }

    return( Equation );
  }

  if ( ( BddNode->HIGH == BddNode->LOW->HIGH ) ||
       ( BddNode->HIGH == BddNode->LOW->LOW  ) )
  {
    Equation = createabloper( ABL_OR );

    Atom = loc_convertbddindex( BddNode->INDEX );
    addablhexpr( Equation, Atom );

    ScanNode = BddNode->LOW;

    do
    {
      Atom = loc_convertbddindex( ScanNode->INDEX );

      if ( ScanNode->HIGH == BddNode->HIGH )
      {
        ScanNode = ScanNode->LOW;
      }
      else
      {
        ScanNode = ScanNode->HIGH;
        Atom     = optimablnotexpr( Atom );
      }

      addablhexpr( Equation, Atom );
    }
    while ( ( ScanNode->HIGH == BddNode->HIGH ) ||
            ( ScanNode->LOW  == BddNode->HIGH ) );

    Equation = loc_convertbddmux( Equation, BddNode->HIGH, ScanNode );

    return( Equation );
  }

  if ( ( BddNode->LOW == BddNode->HIGH->HIGH ) ||
       ( BddNode->LOW == BddNode->HIGH->LOW  ) )
  {
    Equation = createabloper( ABL_AND );

    Atom = loc_convertbddindex( BddNode->INDEX );
    addablhexpr( Equation, Atom );

    ScanNode = BddNode->HIGH;

    do
    {
      Atom = loc_convertbddindex( ScanNode->INDEX );

      if ( ScanNode->LOW == BddNode->LOW )
      {
        ScanNode = ScanNode->HIGH;
      }
      else
      {
        ScanNode = ScanNode->LOW;
        Atom     = optimablnotexpr( Atom );
      }

      addablhexpr( Equation, Atom );
    }
    while ( ( ScanNode->HIGH == BddNode->LOW ) ||
            ( ScanNode->LOW  == BddNode->LOW ) );

    Equation = loc_convertbddmux( Equation, ScanNode, BddNode->LOW );

    return( Equation );
  }

  if ( ( IndexLow             == IndexHigh           ) &&
       ( BddNode->HIGH->HIGH  == BddNode->LOW->LOW   ) &&
       ( BddNode->HIGH->LOW   == BddNode->LOW->HIGH  ) )
  {
    Equation = optimablbinexpr( ABL_XOR,
                                 loc_convertbddindex( BddNode->INDEX      ),
                                 loc_convertbddindex( BddNode->LOW->INDEX ) );

    Equation = loc_convertbddmux( Equation, BddNode->LOW->HIGH, BddNode->LOW->LOW );

    return( Equation );
  }

  Atom = loc_convertbddindex( BddNode->INDEX );

  Equation = loc_convertbddmux( Atom, BddNode->HIGH, BddNode->LOW );

  return( Equation );
}

/*------------------------------------------------------------\
|                                                             |
|                       Convert Bdd Node Abl                  |
|                                                             |
\------------------------------------------------------------*/

chain_list *convertbddnodeabl( BddSystem, NameArray, IndexArray, BddNode )

  bddsystem *BddSystem;
  char     **NameArray;
  bddindex  *IndexArray;
  bddnode   *BddNode;
{
  setbddlocalsystem( BddSystem );

  BddConvertNameArray  = NameArray;
  BddConvertIndexArray = IndexArray;

  return( loc_convertbddnode( BddNode ) );
}

/*------------------------------------------------------------\
|                                                             |
|           Local Convert Node In Sum of Product Abl          |
|                                                             |
\------------------------------------------------------------*/

void loc_convertbddnodesum( BddNode, SumProd )

  bddnode    *BddNode;
  chain_list *SumProd;
{
  chain_list *Equation;
  bddindex    IndexLow;
  bddindex    IndexHigh;

  IndexLow  = BddNode->LOW->INDEX;
  IndexHigh = BddNode->HIGH->INDEX;

  if ( ( IndexHigh == BDD_INDEX_ONE ) ||
       ( IndexLow  == BDD_INDEX_ONE ) )
  {
    Equation = loc_convertbddindex( BddNode->INDEX );

    if ( IndexLow == BDD_INDEX_ONE )
    {
      Equation = optimablnotexpr( Equation );
    }

    if ( SumProd != (chain_list *)0 )
    {
      Equation = optimablbinexpr( ABL_AND, dupablexpr( SumProd ), Equation );
    }

    addablhexpr( BddConvertSumProd, Equation );
  }

  if ( IndexHigh >= BDD_INDEX_MIN )
  {
    Equation = loc_convertbddindex( BddNode->INDEX );

    if ( SumProd != (chain_list *)0 )
    {
      Equation = optimablbinexpr( ABL_AND, dupablexpr( SumProd ), Equation );
    }

    loc_convertbddnodesum( BddNode->HIGH, Equation );

    delablexpr( Equation );
  }

  if ( IndexLow >= BDD_INDEX_MIN )
  {
    Equation = loc_convertbddindex( BddNode->INDEX );
    Equation = optimablnotexpr( Equation );

    if ( SumProd != (chain_list *)0 )
    {
      Equation = optimablbinexpr( ABL_AND, dupablexpr( SumProd ), Equation );
    }

    loc_convertbddnodesum( BddNode->LOW, Equation );

    delablexpr( Equation );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                Convert Bdd Sum of Product Abl               |
|                                                             |
\------------------------------------------------------------*/

chain_list *convertbddnodesumabl( BddSystem, NameArray, IndexArray, BddNode )

  bddsystem  *BddSystem;
  char      **NameArray;
  bddindex   *IndexArray;
  bddnode    *BddNode;
{
  chain_list *SumProd;

  setbddlocalsystem( BddSystem );

  BddConvertNameArray  = NameArray;
  BddConvertIndexArray = IndexArray;

  if ( BddNode->INDEX == BDD_INDEX_ONE )
  {
    return( createablatomone() );
  }

  if ( BddNode->INDEX == BDD_INDEX_ZERO )
  {
    return( createablatomzero() );
  }

  BddConvertSumProd = createabloper( ABL_OR );

  loc_convertbddnodesum( BddNode, (chain_list *)0 );

  if ( ABL_CDDR( BddConvertSumProd ) == (chain_list *)0 )
  {
    SumProd = ABL_CADR( BddConvertSumProd );
    ABL_CADR( BddConvertSumProd ) = (chain_list *)0;

    freeablexpr( BddConvertSumProd );
    BddConvertSumProd = SumProd;
  }

  return( BddConvertSumProd );
}
