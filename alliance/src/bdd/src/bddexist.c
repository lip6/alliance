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
| File    :                  bddexist.c                       |
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
|                Local Exist Node With Association            |
|                                                             |
\------------------------------------------------------------*/

bddnode *loc_existbddnode( BddNode )

  bddnode *BddNode;
{
  bddhoper *HashOper;
  bddnode  *BddExist;
  bddnode  *BddHigh;
  bddnode  *BddLow;
  bddvar    NodeVar;

  NodeVar = BddLocalSystem->INDEX_TO_VAR[ BddNode->INDEX ];

  if ( BddLocalAssoc->LAST < NodeVar )
  {
    return( incbddrefext( BddNode ) );
  }

  HashOper = searchbddhoper( BddLocalSystem->HASH_OPER,
                             BDD_OPER_EXIST + BddLocalAssoc->IDENT, 
                             BddNode, BddNode );
 
  if ( HashOper != (bddhoper *)0 )
  {
    return( incbddrefext( HashOper->FATHER ) );
  }

  BddExist = BddLocalAssocNode[ NodeVar ];
  BddHigh  = loc_existbddnode( BddNode->HIGH );

  if ( ( BddExist       != (bddnode *)0  ) &&
       ( BddHigh->INDEX == BDD_INDEX_ONE ) )
  {
    BddExist = BddHigh;
  }
  else
  {
    BddLow  = decbddrefext( loc_existbddnode( BddNode->LOW ) );
    BddHigh = decbddrefext( BddHigh );

    if ( BddExist != (bddnode *)0 )
    {
      BddExist = loc_applybdd( ABL_OR, BddHigh, BddLow );
    }
    else
    {
      BddExist = addbddnode( (bddsystem *)0, BddNode->INDEX, BddHigh, BddLow );
    }
  }

  addbddhoper( BddLocalSystem->HASH_OPER,
               BDD_OPER_EXIST + BddLocalAssoc->IDENT, 
               BddNode, BddNode, BddExist );

  BddLocalAssoc->CACHE = 1;

  return( BddExist );
}

/*------------------------------------------------------------\
|                                                             |
|                Exist Bdd Node With Association              |
|                                                             |
\------------------------------------------------------------*/

bddnode *existbddnodeassoc( BddSystem, BddNode, BddAssoc )
 
  bddsystem *BddSystem;
  bddnode   *BddNode;
  bddassoc  *BddAssoc;
{
  bddnode *BddExist;

  if ( checkbddassoc( BddSystem, BddAssoc, 1 ) == 2 )
  {
    return( incbddrefext( BddNode ) );
  }

  BddLocalAssoc     = BddAssoc;
  BddLocalAssocNode = BddAssoc->ASSOC_NODE;

  BddExist = loc_existbddnode( BddNode );

  checkbddmaxnode( (bddsystem *)0, 1 );

  return( BddExist );
}

/*------------------------------------------------------------\
|                                                             |
|               Forall Bdd Node With Association              |
|                                                             |
\------------------------------------------------------------*/

bddnode *forallbddnodeassoc( BddSystem, BddNode, BddAssoc )
 
  bddsystem *BddSystem;
  bddnode   *BddNode;
  bddassoc  *BddAssoc;
{
  bddnode *BddForall;

  if ( checkbddassoc( BddSystem, BddAssoc, 1 ) == 2 )
  {
    return( incbddrefext( BddNode ) );
  }

  BddLocalAssoc     = BddAssoc;
  BddLocalAssocNode = BddAssoc->ASSOC_NODE;

  BddNode   = decbddrefext( loc_applybddnot( BddNode ) );
  BddForall = decbddrefext( loc_existbddnode( BddNode ) );
  BddForall = loc_applybddnot( BddForall );

  checkbddmaxnode( (bddsystem *)0, 1 );

  return( BddForall );
}

/*------------------------------------------------------------\
|                                                             |
|           Local Exist Node With Missing Association         |
|                                                             |
\------------------------------------------------------------*/

bddnode *loc_existbddnodemiss( BddNode )

  bddnode *BddNode;
{
  bddhoper *HashOper;
  bddnode  *BddExist;
  bddnode  *BddHigh;
  bddnode  *BddLow;
  bddvar    NodeVar;
  short     Exist;

  if ( BddNode->INDEX < BDD_INDEX_MIN )
  {
    return( BddNode );
  }

  NodeVar = BddLocalSystem->INDEX_TO_VAR[ BddNode->INDEX ];

  if ( BddLocalAssoc->LAST < NodeVar )
  {
    Exist = 1;
  }
  else
  {
    Exist = ( BddLocalAssocNode[ NodeVar ] == (bddnode *)0 );
  }

  HashOper = searchbddhoper( BddLocalSystem->HASH_OPER,
                             BDD_OPER_EXIST_MISS + BddLocalAssoc->IDENT, 
                             BddNode, BddNode );
 
  if ( HashOper != (bddhoper *)0 )
  {
    return( incbddrefext( HashOper->FATHER ) );
  }

  BddHigh = loc_existbddnodemiss( BddNode->HIGH );

  if ( ( Exist                           ) &&
       ( BddHigh->INDEX == BDD_INDEX_ONE ) )
  {
    BddExist = BddHigh;
  }
  else
  {
    BddLow  = decbddrefext( loc_existbddnodemiss( BddNode->LOW ) );
    BddHigh = decbddrefext( BddHigh );

    if ( Exist )
    {
      BddExist = loc_applybdd( ABL_OR, BddHigh, BddLow );
    }
    else
    {
      BddExist = addbddnode( (bddsystem *)0, BddNode->INDEX, BddHigh, BddLow );
    }
  }

  addbddhoper( BddLocalSystem->HASH_OPER,
               BDD_OPER_EXIST_MISS + BddLocalAssoc->IDENT, 
               BddNode, BddNode, BddExist );

  BddLocalAssoc->CACHE = 1;

  return( BddExist );
}

/*------------------------------------------------------------\
|                                                             |
|               Exist Bdd Node Missing Association            |
|                                                             |
\------------------------------------------------------------*/

bddnode *existbddnodemissassoc( BddSystem, BddNode, BddAssoc )
 
  bddsystem *BddSystem;
  bddnode   *BddNode;
  bddassoc  *BddAssoc;
{
  bddnode *BddExist;

  if ( checkbddassoc( BddSystem, BddAssoc, 1 ) == 2 )
  {
    return( incbddrefext( BddNode ) );
  }

  BddLocalAssoc     = BddAssoc;
  BddLocalAssocNode = BddAssoc->ASSOC_NODE;

  BddExist = loc_existbddnodemiss( BddNode );

  checkbddmaxnode( (bddsystem *)0, 1 );

  return( BddExist );
}

/*------------------------------------------------------------\
|                                                             |
|               Forall Bdd Node With Missing Association      |
|                                                             |
\------------------------------------------------------------*/

bddnode *forallbddnodemissassoc( BddSystem, BddNode, BddAssoc )
 
  bddsystem *BddSystem;
  bddnode   *BddNode;
  bddassoc  *BddAssoc;
{
  bddnode *BddMiss;

  if ( checkbddassoc( BddSystem, BddAssoc, 1 ) == 2 )
  {
    return( incbddrefext( BddNode ) );
  }

  BddLocalAssoc     = BddAssoc;
  BddLocalAssocNode = BddAssoc->ASSOC_NODE;

  BddNode  = decbddrefext( loc_applybddnot( BddNode ) );
  BddMiss = decbddrefext( loc_existbddnodemiss( BddNode ) );
  BddMiss = loc_applybddnot( BddMiss );

  checkbddmaxnode( (bddsystem *)0, 1 );

  return( BddMiss );
}
