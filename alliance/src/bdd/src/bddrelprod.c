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
| File    :                 bddrelprod.c                      |
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
# include "bddassoc.h"
# include "bddrelprod.h"
# include "bddapply.h"
# include "bddexist.h"
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
|             Local Relation Product With Association         |
|                                                             |
\------------------------------------------------------------*/

static bddnode *loc_relprodbddnode( BddNode1, BddNode2 )

  bddnode *BddNode1;
  bddnode *BddNode2;
{
  bddhoper *HashOper;
  bddnode  *BddRelProd;
  bddnode  *BddHigh;
  bddnode  *BddLow;
  bddvar    NodeVar1;
  bddvar    NodeVar2;
  
  if ( ( BddNode1->INDEX < BDD_INDEX_MIN ) ||
       ( BddNode2->INDEX < BDD_INDEX_MIN ) )
  {
    if ( ( BddNode1->INDEX == BDD_INDEX_ZERO ) ||
         ( BddNode2->INDEX == BDD_INDEX_ZERO ) )
    {
      return( BddLocalSystem->ZERO );
    }

    if ( ( BddNode1->INDEX == BDD_INDEX_ONE ) &&
         ( BddNode2->INDEX == BDD_INDEX_ONE ) )
    {
      return( BddLocalSystem->ONE );
    }

    if ( BddNode1->INDEX == BDD_INDEX_ONE )
    {
      return( loc_existbddnode( BddNode2 ) );
    }

    return( loc_existbddnode( BddNode1 ) );
  }

  NodeVar1 = BddLocalSystem->INDEX_TO_VAR[ BddNode1->INDEX ];
  NodeVar2 = BddLocalSystem->INDEX_TO_VAR[ BddNode2->INDEX ];

  if ( ( BddLocalAssoc->LAST < NodeVar1 ) &&
       ( BddLocalAssoc->LAST < NodeVar2 ) )
  {
    return( loc_applybdd( ABL_AND, BddNode1, BddNode2 ) );
  }

  HashOper = searchbddhoper( BddLocalSystem->HASH_OPER,
                             BDD_OPER_REL_PROD + BddLocalAssoc->IDENT, 
                             BddNode1, BddNode2 );
 
  if ( HashOper != (bddhoper *)0 )
  {
    return( incbddrefext( HashOper->FATHER ) );
  }

  if ( NodeVar2 == NodeVar1 )
  {
    BddHigh    = loc_relprodbddnode( BddNode1->HIGH, BddNode2->HIGH );
    BddRelProd = BddLocalAssocNode[ NodeVar1 ];
  }
  else
  if ( NodeVar2 > NodeVar1 )
  {
    BddHigh    = loc_relprodbddnode( BddNode1->HIGH, BddNode2 );
    BddRelProd = BddLocalAssocNode[ NodeVar1 ];
  }
  else
  {
    BddHigh    = loc_relprodbddnode( BddNode2->HIGH, BddNode1 );
    BddRelProd = BddLocalAssocNode[ NodeVar2 ];
  }

  if ( ( BddRelProd     != (bddnode *)0  ) &&
       ( BddHigh->INDEX == BDD_INDEX_ONE ) )
  {
    BddRelProd = BddHigh;
  }
  else
  {
    if ( NodeVar2 == NodeVar1 )
    {
      BddLow = loc_relprodbddnode( BddNode1->LOW, BddNode2->LOW );
    }
    else
    if ( NodeVar2 > NodeVar1 )
    {
      BddLow = loc_relprodbddnode( BddNode1->LOW, BddNode2 );
    }
    else
    {
      BddLow = loc_relprodbddnode( BddNode2->LOW, BddNode1 );
    }

    BddHigh = decbddrefext( BddHigh );
    BddLow  = decbddrefext( BddLow  );

    if ( BddRelProd != (bddnode *)0 )
    {
      BddRelProd = loc_applybdd( ABL_OR, BddHigh, BddLow );
    }
    else
    if ( NodeVar2 >= NodeVar1 )
    {
      BddRelProd = addbddnode( (bddsystem *)0, BddNode1->INDEX, BddHigh, BddLow );
    }
    else
    {
      BddRelProd = addbddnode( (bddsystem *)0, BddNode2->INDEX, BddHigh, BddLow );
    }
  }

  addbddhoper( BddLocalSystem->HASH_OPER,
               BDD_OPER_REL_PROD + BddLocalAssoc->IDENT, 
               BddNode1, BddNode2, BddRelProd );

  BddLocalAssoc->CACHE = 1;

  return( BddRelProd );
}

/*------------------------------------------------------------\
|                                                             |
|            Relational Product Bdd Node With Association     |
|                                                             |
\------------------------------------------------------------*/

bddnode *relprodbddnodeassoc( BddSystem, BddNode1, BddNode2, BddAssoc )
 
  bddsystem *BddSystem;
  bddnode   *BddNode1;
  bddnode   *BddNode2;
  bddassoc  *BddAssoc;
{
  bddnode *BddRelProd;

  if ( checkbddassoc( BddSystem, BddAssoc, 1 ) == 2 )
  {
    return( loc_applybdd( ABL_AND, BddNode1, BddNode2 ) );
  }

  BddLocalAssoc     = BddAssoc;
  BddLocalAssocNode = BddAssoc->ASSOC_NODE;

  BddRelProd = loc_relprodbddnode( BddNode1, BddNode2 );

  checkbddmaxnode( (bddsystem *)0, 1 );

  return( BddRelProd );
}
