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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     Bdd                           |
|                                                             |
| File    :                  bddapply.c                       |
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
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   Local Apply Bdd Node Not                  |
|                                                             |
\------------------------------------------------------------*/

bddnode *loc_applybddnot( BddNode )

  bddnode *BddNode;
{
  bddhoper *HashOper;
  bddnode  *BddNot;

  if ( BddNode == BddLocalSystem->ONE )
  {
    return( BddLocalSystem->ZERO );
  }

  if ( BddNode == BddLocalSystem->ZERO )
  {
    return( BddLocalSystem->ONE );
  }

  HashOper = searchbddhoper( BddLocalSystem->HASH_OPER,
                             ABL_NOT, BddNode, BddNode );
 
  if ( HashOper != (bddhoper *)0 )
  {
    return( incbddrefext( HashOper->FATHER ) );
  }

  BddNot = addbddnode( (bddsystem *)0, BddNode->INDEX, 
                       decbddrefext( loc_applybddnot( BddNode->HIGH ) ),
                       decbddrefext( loc_applybddnot( BddNode->LOW  ) ) );

  addbddhoper( BddLocalSystem->HASH_OPER, 
               ABL_NOT, BddNode, BddNode, BddNot );

  return( BddNot );
}

/*------------------------------------------------------------\
|                                                             |
|                       Apply Bdd Not Node                    |
|                                                             |
\------------------------------------------------------------*/

bddnode *applybddnodenot( BddSystem, BddNode )

  bddsystem *BddSystem;
  bddnode   *BddNode;
{
  setbddlocalsystem( BddSystem );

  BddNode = loc_applybddnot( BddNode );

  checkbddmaxnode( (bddsystem *)0, 1 );

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                Local Apply Terminal Node                    |
|                                                             |
\------------------------------------------------------------*/

bddnode *loc_applybddterm( Oper, Index, BddNode )

  long      Oper;
  bddindex  Index;
  bddnode  *BddNode;
{
  if ( Index == BDD_INDEX_ONE ) 
  {
    if ( Oper == ABL_OR )
    {
      return( BddLocalSystem->ONE );
    }

    if ( ( Oper == ABL_NAND ) ||
         ( Oper == ABL_XOR  ) )
    {
      return( loc_applybddnot( BddNode ) );
    }

    if ( ( Oper == ABL_AND  ) ||
         ( Oper == ABL_NXOR ) )
    {
      return( incbddrefext( BddNode ) );
    }

    return( BddLocalSystem->ZERO );
  }

  if ( Oper == ABL_AND )
  {
    return( BddLocalSystem->ZERO );
  }

  if ( ( Oper == ABL_NOR  ) ||
       ( Oper == ABL_NXOR ) )
  {
    return( loc_applybddnot( BddNode ) );
  }

  if ( ( Oper == ABL_OR  ) || 
       ( Oper == ABL_XOR ) )
  {
    return( incbddrefext( BddNode ) );
  }

  return( BddLocalSystem->ONE );
}

/*------------------------------------------------------------\
|                                                             |
|                      Apply Bdd Terminal Node                |
|                                                             |
\------------------------------------------------------------*/

bddnode *applybddnodeterm( BddSystem, Oper, Index, BddNode )

  bddsystem *BddSystem;
  long       Oper;
  bddindex   Index;
  bddnode   *BddNode;
{
  checkbddoper( BddSystem, Oper, 1 );

  BddNode = loc_applybddterm( Oper, Index, BddNode );

  checkbddmaxnode( (bddsystem *)0, 1 );

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                     Local Apply Bdd Node                    |
|                                                             |
\------------------------------------------------------------*/

bddnode *loc_applybdd( Oper, BddNode1, BddNode2 )

  long     Oper;
  bddnode *BddNode1;
  bddnode *BddNode2;
{
  bddhoper *HashOper;
  bddnode  *BddNode;
  bddnode  *BddHigh;
  bddnode  *BddLow;
  bddindex  Index1;
  bddindex  Index2;

  Index1 = BddNode1->INDEX;
  Index2 = BddNode2->INDEX;

  if ( ( Index1 < BDD_INDEX_MIN ) ||
       ( Index2 < BDD_INDEX_MIN ) )
  {
    if ( ( Index1 < BDD_INDEX_MIN ) &&
         ( Index2 < BDD_INDEX_MIN ) )
    {
      if ( Index1 != Index2 )
      {
        if ( ( Oper == ABL_NAND ) || 
             ( Oper == ABL_OR   ) ||
             ( Oper == ABL_XOR  ) )
        {
          return( BddLocalSystem->ONE );
        }

        return( BddLocalSystem->ZERO );
      }

      if ( Index1 == BDD_INDEX_ZERO )
      {
        if ( ( Oper == ABL_NAND ) || 
             ( Oper == ABL_NOR  ) ||
             ( Oper == ABL_NXOR ) )
        {
          return( BddLocalSystem->ONE );
        }

        return( BddLocalSystem->ZERO );
      }

      if ( ( Oper == ABL_OR   ) || 
           ( Oper == ABL_AND  ) || 
           ( Oper == ABL_NXOR ) )
      {
        return( BddLocalSystem->ONE );
      }

      return( BddLocalSystem->ZERO );
    }

    if ( Index1 < BDD_INDEX_MIN )
    {
      return( loc_applybddterm( Oper, Index1, BddNode2 ) );
    }

    return( loc_applybddterm( Oper, Index2, BddNode1 ) );
  }

  HashOper = searchbddhoper( BddLocalSystem->HASH_OPER,
                             Oper, BddNode1, BddNode2 );

  if ( HashOper != (bddhoper *)0 )
  {
    return( incbddrefext( HashOper->FATHER ) );
  }

  if ( Index1 == Index2 )
  {
    if ( BddNode1 == BddNode2 )
    {
      if ( ( Oper == ABL_OR  ) ||
           ( Oper == ABL_AND ) )
      {
        return( incbddrefext( BddNode1 ) );
      }
 
      if ( ( Oper == ABL_NOR  ) ||
           ( Oper == ABL_NAND ) )
      {
        return( loc_applybddnot( BddNode1 ) );
      }

      if ( Oper == ABL_XOR )
      {
        return( BddLocalSystem->ZERO );
      }

      return( BddLocalSystem->ONE );
    }

    BddHigh = loc_applybdd( Oper, BddNode1->HIGH, BddNode2->HIGH );
    BddLow  = loc_applybdd( Oper, BddNode1->LOW , BddNode2->LOW  );
  }
  else
  if ( BddLocalSystem->INDEX_TO_VAR[ Index1 ] > 
       BddLocalSystem->INDEX_TO_VAR[ Index2 ] )
  {
    Index1  = Index2;
    BddHigh = loc_applybdd( Oper, BddNode1, BddNode2->HIGH );
    BddLow  = loc_applybdd( Oper, BddNode1, BddNode2->LOW  );
  }
  else
  {
    BddHigh = loc_applybdd( Oper, BddNode1->HIGH, BddNode2 );
    BddLow  = loc_applybdd( Oper, BddNode1->LOW,  BddNode2 );
  }

  BddNode = addbddnode( (bddsystem *)0, Index1,
                        decbddrefext( BddHigh ),
                        decbddrefext( BddLow  ) );

  addbddhoper( BddLocalSystem->HASH_OPER, 
               Oper, BddNode1, BddNode2, BddNode );

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                       Apply Bdd Node                        |
|                                                             |
\------------------------------------------------------------*/

bddnode *applybddnode( BddSystem, Oper, BddNode1, BddNode2 )

  bddsystem *BddSystem;
  long       Oper;
  bddnode   *BddNode1;
  bddnode   *BddNode2;
{
  setbddlocalsystem( BddSystem );

  if ( isablunaryoper( Oper ) )
  {
    bdderror( BDD_OPERATOR_ERROR, Oper );
  }

  BddNode1 = loc_applybdd( Oper, BddNode1, BddNode2 );

  checkbddmaxnode( (bddsystem *)0, 1 );

  return( BddNode1 );
}

/*------------------------------------------------------------\
|                                                             |
|                  Local Apply Bdd Ite Node                   |
|                                                             |
\------------------------------------------------------------*/

bddnode *loc_applybddite( BddIf, BddThen, BddElse )

  bddnode   *BddIf;
  bddnode   *BddThen;
  bddnode   *BddElse;
{
  BddThen = loc_applybdd( ABL_AND, BddIf, BddThen );
  BddIf   = loc_applybddnot( BddIf );
  BddElse = loc_applybdd( ABL_AND,
                          decbddrefext( BddIf ), BddElse );
  BddIf   = loc_applybdd( ABL_OR ,
                          decbddrefext( BddThen ),
                          decbddrefext( BddElse ) );
  return( BddIf );
}

/*------------------------------------------------------------\
|                                                             |
|                       Apply Bdd Ite Node                    |
|                                                             |
\------------------------------------------------------------*/

bddnode *applybddnodeite( BddSystem, BddIf, BddThen, BddElse )

  bddsystem *BddSystem;
  bddnode   *BddIf;
  bddnode   *BddThen;
  bddnode   *BddElse;
{
  setbddlocalsystem( BddSystem );

  BddThen = loc_applybdd( ABL_AND, BddIf, BddThen );
  checkbddmaxnode( (bddsystem *)0, 1 );
  BddIf   = loc_applybddnot( BddIf );
  checkbddmaxnode( (bddsystem *)0, 1 );
  BddElse = loc_applybdd( ABL_AND,
                          decbddrefext( BddIf   ),
                          decbddrefext( BddElse ) );
  checkbddmaxnode( (bddsystem *)0, 1 );
  BddIf   = loc_applybdd( ABL_OR ,
                          decbddrefext( BddThen ),
                          decbddrefext( BddElse ) );
  checkbddmaxnode( (bddsystem *)0, 1 );

  return( BddIf );
}

/*------------------------------------------------------------\
|                                                             |
|                       Apply List Node                       |
|                                                             |
\------------------------------------------------------------*/

bddnode *loc_applybddlist( Oper, BddList )

  long        Oper;
  chain_list *BddList;
{
  bddnode *BddNode1;
  bddnode *BddNode2;

  BddNode1 = (bddnode *)( BddList->DATA       );
  BddNode2 = (bddnode *)( BddList->NEXT->DATA );

  if ( BddList->NEXT->NEXT != (chain_list *)0 )
  {
    BddNode2 = decbddrefext( loc_applybddlist( Oper, BddNode2 ) );
  }

  return( loc_applybdd( Oper, BddNode1, BddNode2 ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Apply Bdd List Node                   |
|                                                             |
\------------------------------------------------------------*/

bddnode *applybddnodelist( BddSystem, Oper, BddList )

  bddsystem  *BddSystem;
  long        Oper;
  chain_list *BddList;
{
  bddnode *BddNode;

  setbddlocalsystem( BddSystem );

  if ( BddList == (chain_list *)0 )
  {
    bdderror( BDD_BAD_LIST_ERROR, 0 );
  }

  if ( Oper == ABL_NOT )
  {
    if ( BddList->NEXT != (chain_list *)0 )
    {
      bdderror( BDD_OPERATOR_ERROR, Oper );
    }

    BddNode = (bddnode *)( BddList->DATA );
    BddNode = loc_applybddnot( BddNode );
  }
  else
  {
    if ( ( isablunaryoper( Oper )           ) ||
         ( BddList->NEXT == (chain_list *)0 ) )
    {
      bdderror( BDD_OPERATOR_ERROR, Oper );
    }

    if ( ( getabloperpolar( Oper ) == ABL_POLAR_POSITIVE ) ||
         ( BddList->NEXT->NEXT     == (chain_list *)0    ) )
    {
      BddNode = loc_applybddlist( Oper, BddList );
    }
    else
    {
      Oper    = getablopernot( Oper );
      BddNode = loc_applybddlist( Oper, BddList );
      BddNode = loc_applybddnot( decbddrefext( BddNode ) );
    }
  }

  checkbddmaxnode( (bddsystem *)0, 1 );

  return( BddNode );
}
