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
| File    :                  bddsubst.c                       |
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
# include "bddassoc.h"
# include "bddapply.h"
# include "bddsubst.h"
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
|                   Substitute Node With Association          |
|                                                             |
\------------------------------------------------------------*/

static bddnode *loc_substbddnode( BddNode )

  bddnode *BddNode;
{
  bddhoper *HashOper;
  bddnode  *BddSubst;
  bddnode  *BddHigh;
  bddnode  *BddLow;
  bddvar    NodeVar;

  NodeVar = BddLocalSystem->INDEX_TO_VAR[ BddNode->INDEX ];

  if ( BddLocalAssoc->LAST < NodeVar )
  {
    return( incbddrefext( BddNode ) );
  }

  HashOper = searchbddhoper( BddLocalSystem->HASH_OPER,
                             BDD_OPER_SUBSTITUTE + BddLocalAssoc->IDENT, 
                             BddNode, BddNode );
 
  if ( HashOper != (bddhoper *)0 )
  {
    return( incbddrefext( HashOper->FATHER ) );
  }

  BddSubst = BddLocalAssocNode[ NodeVar ];

  if ( BddSubst == (bddnode *)0 )
  {
    BddSubst = BddNode;
  }

  if ( BddSubst->INDEX < BDD_INDEX_MIN )
  {
    if ( BddSubst->INDEX == BDD_INDEX_ONE )
    {
      BddSubst = loc_substbddnode( BddNode->HIGH );
    }
    else
    {
      BddSubst = loc_substbddnode( BddNode->LOW  );
    }
  }
  else
  {
    BddHigh = decbddrefext( loc_substbddnode( BddNode->HIGH ) );
    BddLow  = decbddrefext( loc_substbddnode( BddNode->LOW  ) );

    if ( ( BddSubst->INDEX == BddNode->INDEX                                 ) &&
         ( NodeVar         <  BddLocalSystem->INDEX_TO_VAR[ BddHigh->INDEX ] ) &&
         ( NodeVar         <  BddLocalSystem->INDEX_TO_VAR[ BddLow->INDEX  ] ) )
    {
      BddSubst = addbddnode( (bddsystem *)0, BddNode->INDEX, BddHigh, BddLow );
    }
    else
    {
      BddSubst = loc_applybddite( BddSubst, BddHigh, BddLow );
    }
  }

  addbddhoper( BddLocalSystem->HASH_OPER, 
               BDD_OPER_SUBSTITUTE + BddLocalAssoc->IDENT, 
               BddNode, BddNode, BddSubst );

  BddLocalAssoc->CACHE = 1;

  return( BddSubst );
}

/*------------------------------------------------------------\
|                                                             |
|                Substitute Bdd Node With Association         |
|                                                             |
\------------------------------------------------------------*/

bddnode *substbddnodeassoc( BddSystem, BddNode, BddAssoc )
 
  bddsystem *BddSystem;
  bddnode   *BddNode;
  bddassoc  *BddAssoc;
{
  bddnode *BddSubst;

  if ( checkbddassoc( BddSystem, BddAssoc, 1 ) == 2 )
  {
    return( incbddrefext( BddNode ) );
  }

  BddLocalAssoc     = BddAssoc;
  BddLocalAssocNode = BddAssoc->ASSOC_NODE;

  BddSubst = loc_substbddnode( BddNode );

  checkbddmaxnode( (bddsystem *)0, 1 );

  return( BddSubst );
}
