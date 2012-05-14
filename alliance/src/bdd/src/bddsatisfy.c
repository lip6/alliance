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
| Tool    :                     Bdd                           |
|                                                             |
| File    :                  bddsatisfy.c                     |
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
# include "bddsatisfy.h"
# include "bddassoc.h"
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
|                  Local Satisfy Bdd Node                     |
|                                                             |
\------------------------------------------------------------*/

static bddnode *loc_satisfybdd( BddNode )

  bddnode  *BddNode;
{
  if ( BddNode->INDEX >= BDD_INDEX_MIN )
  {
    if ( BddNode->HIGH == BddLocalSystem->ZERO )
    {
      BddNode = addbddnode( (bddsystem *)0, BddNode->INDEX,
                            BddLocalSystem->ZERO,
                            decbddrefext( loc_satisfybdd( BddNode->LOW ) ) );
    }
    else
    {
      BddNode = addbddnode( (bddsystem *)0, BddNode->INDEX,
                            decbddrefext( loc_satisfybdd( BddNode->HIGH ) ),
                            BddLocalSystem->ZERO );
    }
  }

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                    Satisfy Bdd Node                         |
|                                                             |
\------------------------------------------------------------*/

bddnode *satisfybddnode( BddSystem, BddNode )

  bddsystem *BddSystem;
  bddnode   *BddNode;
{
  setbddlocalsystem( BddSystem );

  if ( BddNode->INDEX == BDD_INDEX_ZERO )
  {
    bdderror( BDD_BAD_INDEX_ERROR, BDD_INDEX_ZERO );
  }

  BddNode = loc_satisfybdd( BddNode );

  checkbddmaxnode( (bddsystem *)0, 1 );

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                  Local Satisfy Bdd Node Assoc               |
|                                                             |
\------------------------------------------------------------*/

static bddnode *loc_satisfybddassoc( BddNode )

  bddnode  *BddNode;
{
  bddnode *NodeVar;
  bddvar   Variable;

  if ( BddNode->INDEX >= BDD_INDEX_MIN )
  {
    if ( BddNode->HIGH == BddLocalSystem->ZERO )
    {
      BddNode = addbddnode( (bddsystem *)0, BddNode->INDEX,
                            BddLocalSystem->ZERO,
                            decbddrefext( loc_satisfybddassoc( BddNode->LOW ) ) );
    }
    else
    {
      BddNode = addbddnode( (bddsystem *)0, BddNode->INDEX,
                            decbddrefext( loc_satisfybddassoc( BddNode->HIGH ) ),
                            BddLocalSystem->ZERO );
    }

    if ( BddNode->INDEX >= BDD_INDEX_MIN )
    {
      Variable = BddLocalSystem->INDEX_TO_VAR[ BddNode->INDEX ];

      if ( ( Variable >= BddLocalAssoc->FIRST ) &&
           ( Variable <= BddLocalAssoc->LAST  ) )
      {
        NodeVar = BddLocalSystem->VAR_NODE[ BddNode->INDEX ];
        NodeVar->FLAG |= BDD_FLAG_NUM_MASK;
      }
    }
  }

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                    Satisfy Bdd Node                         |
|                                                             |
\------------------------------------------------------------*/

bddnode *satisfybddnodeassoc( BddSystem, BddNode, BddAssoc )

  bddsystem *BddSystem;
  bddnode   *BddNode;
  bddassoc  *BddAssoc;
{
  bddnode *BddMore;
  bddnode *NodeVar;
  long     Variable;
  bddindex Index;

  if ( BddNode->INDEX == BDD_INDEX_ZERO )
  {
    bdderror( BDD_BAD_INDEX_ERROR, BDD_INDEX_ZERO );
  }

  if ( checkbddassoc( BddSystem, BddAssoc, 1 ) == 2 )
  {
    return( incbddrefext( BddNode ) );
  }

  BddLocalAssoc     = BddAssoc;
  BddLocalAssocNode = BddAssoc->ASSOC_NODE;

  BddNode = loc_satisfybddassoc( BddNode );
  BddMore = BddLocalSystem->ONE;

  for ( Variable  = (long)BddLocalAssoc->LAST;
        Variable >= (long)BddLocalAssoc->FIRST;
        Variable  = Variable - 1 )
  {
    if ( BddLocalAssocNode[ Variable ] != (bddnode *)0 )
    {
      Index   = BddLocalSystem->VAR_TO_INDEX[ Variable ];
      NodeVar = BddLocalSystem->VAR_NODE[ Index ];

      if ( NodeVar->FLAG & BDD_FLAG_NUM_MASK )
      {
        NodeVar->FLAG &= ~BDD_FLAG_NUM_MASK;
      }
      else
      {
        BddMore = loc_applybdd( ABL_AND, decbddrefext( BddMore ), NodeVar );
      }
    }
  }

  BddNode = loc_applybdd( ABL_AND,
                          decbddrefext( BddNode ),
                          decbddrefext( BddMore ) );
       
  checkbddmaxnode( (bddsystem *)0, 1 );

  return( BddNode );
}
