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
| File    :                 bddcofactor.c                     |
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
# include "bddcofactor.h"
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
|                   Local Cofactor Bdd Node                   |
|                                                             |
\------------------------------------------------------------*/

bddnode *loc_cofactorbdd( BddNode1, BddNode2 )

  bddnode   *BddNode1;
  bddnode   *BddNode2;
{
  bddhoper *HashOper;
  bddnode  *BddNode;
  bddnode  *BddHigh;
  bddnode  *BddLow;
  bddindex  Index1;
  bddindex  Index2;

  Index1 = BddNode1->INDEX;
  Index2 = BddNode2->INDEX;

  if ( Index2 < BDD_INDEX_MIN )
  {
    if ( Index2 == BDD_INDEX_ZERO )
    {
      return( (bddnode *)0 );
    }

    return( incbddrefext( BddNode1 ) );
  }

  if ( Index1 < BDD_INDEX_MIN )
  {
    return( BddNode1 );
  }

  HashOper = searchbddhoper( BddLocalSystem->HASH_OPER,
                             BDD_OPER_COFACTOR, BddNode1, BddNode2 );
 
  if ( HashOper != (bddhoper *)0 )
  {
    return( incbddrefext( HashOper->FATHER ) );
  }

  if ( Index1 == Index2 )
  {
    BddHigh = loc_cofactorbdd( BddNode1->HIGH, BddNode2->HIGH );
    BddLow  = loc_cofactorbdd( BddNode1->LOW , BddNode2->LOW  );
  }
  else
  if ( BddLocalSystem->INDEX_TO_VAR[ Index1 ] > 
       BddLocalSystem->INDEX_TO_VAR[ Index2 ] )
  {
    Index1  = Index2;
    BddHigh = loc_cofactorbdd( BddNode1, BddNode2->HIGH );
    BddLow  = loc_cofactorbdd( BddNode1, BddNode2->LOW  );
  }
  else
  {
    BddHigh = loc_cofactorbdd( BddNode1->HIGH, BddNode2 );
    BddLow  = loc_cofactorbdd( BddNode1->LOW , BddNode2 );
  }

  if ( BddHigh == (bddnode *)0 ) BddNode = BddLow;
  else
  if ( BddLow  == (bddnode *)0 ) BddNode = BddHigh;
  else
  BddNode = addbddnode( (bddsystem *)0, Index1,
                        decbddrefext( BddHigh ),
                        decbddrefext( BddLow  ) ); 

  addbddhoper( BddLocalSystem->HASH_OPER, 
               BDD_OPER_COFACTOR,
               BddNode1, BddNode2, BddNode );

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                        Cofact Bdd Node                      |
|                                                             |
\------------------------------------------------------------*/

bddnode *cofactorbddnode( BddSystem, BddNode1, BddNode2 )

  bddsystem *BddSystem;
  bddnode   *BddNode1;
  bddnode   *BddNode2;
{
  setbddlocalsystem( BddSystem );

  if ( BddNode2 == BddLocalSystem->ZERO )
  {
    bdderror( BDD_BAD_INDEX_ERROR, BDD_INDEX_ZERO );
  }

  BddNode1 = loc_cofactorbdd( BddNode1, BddNode2 );

  checkbddmaxnode( (bddsystem *)0, 1 );

  return( BddNode1 );
}
