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
| File    :                  bddcompose.c                     |
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
# include "bddcompose.h"
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

  static bddvar    BddLocalVariable = (bddvar    )0;
  static bddindex  BddLocalIndex    = (bddindex  )0;
  static bddnode  *BddLocalNode     = (bddnode  *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     Local Restrict Bdd Node                 |
|                                                             |
\------------------------------------------------------------*/

static bddnode *loc_restrictbdd( BddNode )

  bddnode *BddNode;
{
  bddhoper *HashOper;
  bddnode  *BddRestrict;
  bddvar    Variable;

  Variable = BddLocalSystem->INDEX_TO_VAR[ BddNode->INDEX ];

  if ( BddLocalVariable < Variable )
  {
    return( incbddrefext( BddNode ) );
  }

  if ( BddLocalVariable == Variable )
  {
    if ( BddLocalNode->INDEX == BDD_INDEX_ZERO )
    {
      return( incbddrefext( BddNode->LOW ) );
    }
    else
    {
      return( incbddrefext( BddNode->HIGH ) );
    }
  }

  HashOper = searchbddhoper( BddLocalSystem->HASH_OPER,
                             BDD_OPER_RESTRICT + BddLocalIndex, 
                             BddNode, BddLocalNode );
 
  if ( HashOper != (bddhoper *)0 )
  {
    return( incbddrefext( HashOper->FATHER ) );
  }

  BddRestrict = addbddnode( (bddsystem *)0, BddNode->INDEX,
                            decbddrefext( loc_restrictbdd( BddNode->HIGH ) ),
                            decbddrefext( loc_restrictbdd( BddNode->LOW  ) ) );

  addbddhoper( BddLocalSystem->HASH_OPER, 
               BDD_OPER_RESTRICT + BddLocalIndex, 
               BddNode, BddLocalNode, BddRestrict );

  return( BddRestrict );
}
  
/*------------------------------------------------------------\
|                                                             |
|                      Restrict Bdd Node                      |
|                                                             |
\------------------------------------------------------------*/

bddnode *restrictbddnode( BddSystem, BddNode, Variable, BddSubst )

  bddsystem *BddSystem;
  bddnode   *BddNode;
  bddvar     Variable;
  bddnode   *BddSubst;
{
  checkbddvar( BddSystem, Variable, 1 );

  if ( BddSubst->INDEX >= BDD_INDEX_MIN )
  {
    bdderror( BDD_BAD_INDEX_ERROR, BddSubst->INDEX );
  }

  BddLocalIndex    = BddLocalSystem->VAR_TO_INDEX[ Variable ];
  BddLocalVariable = Variable;
  BddLocalNode     = BddSubst;

  BddNode = loc_restrictbdd( BddNode );

  checkbddmaxnode( (bddsystem *)0, 1 );

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                     Local Compose Bdd Node                  |
|                                                             |
\------------------------------------------------------------*/

static bddnode *loc_composebdd( BddNode, BddSubst )

  bddnode   *BddNode;
  bddnode   *BddSubst;
{
  bddhoper *HashOper;
  bddnode  *BddCompose;
  bddnode  *BddHigh;
  bddnode  *BddLow;
  bddvar    Variable;
  bddindex  IndexNode;
  bddindex  IndexSubst;

  if ( BddSubst->INDEX < BDD_INDEX_MIN )
  {
    BddLocalNode = BddSubst;

    return( loc_restrictbdd( BddNode ) );
  }

  Variable = BddLocalSystem->INDEX_TO_VAR[ BddNode->INDEX ];

  if ( BddLocalVariable < Variable )
  {
    return( incbddrefext( BddNode ) );
  }

  HashOper = searchbddhoper( BddLocalSystem->HASH_OPER,
                             BDD_OPER_COMPOSE + BddLocalIndex,
                             BddNode, BddSubst );
 
  if ( HashOper != (bddhoper *)0 )
  {
    return( incbddrefext( HashOper->FATHER ) );
  }

  if ( BddLocalVariable == Variable )
  {
    BddCompose = loc_applybddite( BddSubst, BddNode->HIGH , BddNode->LOW );

    return( BddCompose );
  }

  IndexNode  = BddNode->INDEX;
  IndexSubst = BddSubst->INDEX;

  if ( BddLocalSystem->INDEX_TO_VAR[ IndexNode  ] > 
       BddLocalSystem->INDEX_TO_VAR[ IndexSubst ] )
  {
    IndexNode = IndexSubst; 
    BddHigh   = loc_composebdd( BddNode, BddSubst->HIGH );
    BddLow    = loc_composebdd( BddNode, BddSubst->LOW  );
  }
  else
  {
    BddHigh   = loc_composebdd( BddNode->HIGH, BddSubst );
    BddLow    = loc_composebdd( BddNode->LOW,  BddSubst );
  }

  BddCompose = addbddnode( (bddsystem *)0, IndexNode,
                           decbddrefext( BddHigh ),
                           decbddrefext( BddLow  ) );

  addbddhoper( BddLocalSystem->HASH_OPER, 
               BDD_OPER_RESTRICT + BddLocalIndex, 
               BddNode, BddSubst, BddCompose );

  return( BddCompose );
}

/*------------------------------------------------------------\
|                                                             |
|                       Compose Bdd Node                      |
|                                                             |
\------------------------------------------------------------*/

bddnode *composebddnode( BddSystem, BddNode, Variable, BddSubst )

  bddsystem *BddSystem;
  bddnode   *BddNode;
  bddvar     Variable;
  bddnode   *BddSubst;
{
  checkbddvar( BddSystem, Variable, 1 );

  BddLocalVariable = Variable;
  BddLocalIndex    = BddLocalSystem->VAR_TO_INDEX[ Variable ];

  BddNode = loc_composebdd( BddNode, BddSubst );

  checkbddmaxnode( (bddsystem *)0, 1 );

  return( BddNode );
}
