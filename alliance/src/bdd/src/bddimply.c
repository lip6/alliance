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
| File    :                   bddimply.c                      |
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

# include MUT_H
# include AUT_H
# include ABL_H
# include BDD_H

# include <stdio.h>

# include "bddapply.h"
# include "bddimply.h"
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
|                    Local Intersect Bdd Node                 |
|                                                             |
\------------------------------------------------------------*/

static bddnode *loc_intersectbdd( BddNode1, BddNode2 )

  bddnode   *BddNode1;
  bddnode   *BddNode2;
{
  bddhoper *HashOper;
  bddnode  *BddNode;
  bddindex  Index1;
  bddindex  Index2;

  Index1 = BddNode1->INDEX;
  Index2 = BddNode2->INDEX;

  if ( Index1 < BDD_INDEX_MIN )
  {
    if ( Index1 == BDD_INDEX_ZERO )
    {
      return( BddLocalSystem->ZERO );
    }

    return( incbddrefext( BddNode2 ) );
  }

  if ( Index2 < BDD_INDEX_MIN )
  {
    if ( Index2 == BDD_INDEX_ZERO )
    {
      return( BddLocalSystem->ZERO );
    }

    return( incbddrefext( BddNode1 ) );
  }

  if ( BddNode1 == BddNode2 )
  {
     return( incbddrefext( BddNode1 ) );
  }

  HashOper = searchbddhoper( BddLocalSystem->HASH_OPER,
                             BDD_OPER_INTERSECT, BddNode1, BddNode2 );
 
  if ( HashOper != (bddhoper *)0 )
  {
    return( incbddrefext( HashOper->FATHER ) );
  }

  if ( Index1 == Index2 )
  {
    BddNode = decbddrefext( loc_intersectbdd( BddNode1->HIGH, BddNode2->HIGH ) );

    if ( BddNode != BddLocalSystem->ZERO )
    {
      BddNode = addbddnode( (bddsystem *)0, Index1, 
                            BddNode, BddLocalSystem->ZERO ); 
    }
    else
    {
      BddNode = decbddrefext( loc_intersectbdd( BddNode1->LOW, BddNode2->LOW ) );

      BddNode = addbddnode( (bddsystem *)0, Index1, 
                            BddLocalSystem->ZERO, BddNode );
    }
  }
  else
  if ( BddLocalSystem->INDEX_TO_VAR[ Index1 ] > 
       BddLocalSystem->INDEX_TO_VAR[ Index2 ] )
  {
    BddNode = decbddrefext( loc_intersectbdd( BddNode1, BddNode2->HIGH ) );

    if ( BddNode != BddLocalSystem->ZERO )
    {
      BddNode = addbddnode( (bddsystem *)0, Index2, 
                             BddNode, BddLocalSystem->ZERO );
    }
    else
    {
      BddNode = decbddrefext( loc_intersectbdd( BddNode1, BddNode2->LOW ) );

      BddNode = addbddnode( (bddsystem *)0, Index2, 
                            BddLocalSystem->ZERO, BddNode );
    }
  }
  else
  {
    BddNode = decbddrefext( loc_intersectbdd( BddNode1->HIGH, BddNode2 ) );

    if ( BddNode != BddLocalSystem->ZERO )
    {
      BddNode = addbddnode( (bddsystem *)0, Index1, 
                             BddNode, BddLocalSystem->ZERO ); 
    }
    else
    {
      BddNode = decbddrefext( loc_intersectbdd( BddNode1->LOW, BddNode2 ) );

      BddNode = addbddnode( (bddsystem *)0, Index1, 
                            BddLocalSystem->ZERO, BddNode );
    }
  }

  addbddhoper( BddLocalSystem->HASH_OPER, 
               BDD_OPER_INTERSECT, 
               BddNode, BddNode1, BddNode2 );

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                       Imply Bdd Node                        |
|                                                             |
\------------------------------------------------------------*/

bddnode *implybddnode( BddSystem, BddNode1, BddNode2 )

  bddsystem *BddSystem;
  bddnode   *BddNode1;
  bddnode   *BddNode2;
{
  setbddlocalsystem( BddSystem );

  BddNode2 = decbddrefext( loc_applybddnot( BddNode2 ) );
  BddNode1 = loc_intersectbdd( BddNode1, BddNode2 );

  checkbddmaxnode( (bddsystem *)0, 1 );

  return( BddNode1 );
}

/*------------------------------------------------------------\
|                                                             |
|                     Intersect Bdd Node                      |
|                                                             |
\------------------------------------------------------------*/

bddnode *intersectbddnode( BddSystem, BddNode1, BddNode2 )

  bddsystem *BddSystem;
  bddnode   *BddNode1;
  bddnode   *BddNode2;
{
  setbddlocalsystem( BddSystem );

  BddNode1 = loc_intersectbdd( BddNode1, BddNode2 );

  checkbddmaxnode( (bddsystem *)0, 1 );

  return( BddNode1 );
}
