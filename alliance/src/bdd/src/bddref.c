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
| File    :                   bddref.c                        |
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
# include "bddref.h"
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
|                      Bdd Update Reference                   |
|                                                             |
\------------------------------------------------------------*/

void updatebddref( BddNode )

  bddnode *BddNode;
{
  if ( ( BddNode->REF_INT == 0 ) &&
       ( BddNode->REF_EXT == 0 ) )
  {
    decbddrefint( BddNode->HIGH );
    decbddrefint( BddNode->LOW  );

    updatebddref( BddNode->HIGH );
    updatebddref( BddNode->LOW  );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    Bdd Inc External Reference               |
|                                                             |
\------------------------------------------------------------*/

bddnode *incbddrefext( BddNode )

  bddnode *BddNode;
{
  if ( ( BddNode->INDEX   >= BDD_INDEX_MIN ) &&
       ( BddNode->REF_EXT <  BDD_MAX_REF   ) )
  {
    BddNode->REF_EXT++;

    if ( BddNode->REF_EXT == BDD_MAX_REF )
    {
      if ( ! IsBddSystemNoWarning( BddLocalSystem ) )
      {
        bddwarning( BDD_OVERFLOW_REF_EXT_WARNING, BddNode->INDEX, 0 );
      }
    }
  }

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                    Bdd Inc Internal Reference               |
|                                                             |
\------------------------------------------------------------*/

bddnode *incbddrefint( BddNode )

  bddnode *BddNode;
{
  if ( ( BddNode->INDEX   >= BDD_INDEX_MIN ) &&
       ( BddNode->REF_INT <  BDD_MAX_REF   ) )
  {
    BddNode->REF_INT++;

    if ( BddNode->REF_INT == BDD_MAX_REF )
    {
      if ( ! IsBddSystemNoWarning( BddLocalSystem ) )
      {
        bddwarning( BDD_OVERFLOW_REF_INT_WARNING, BddNode->INDEX, 0 );
      }
    }
  }

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                    Bdd Dec External Reference               |
|                                                             |
\------------------------------------------------------------*/

bddnode *decbddrefext( BddNode )

  bddnode *BddNode;
{
  if ( BddNode->INDEX >= BDD_INDEX_MIN )
  {
    if ( BddNode->REF_EXT > 0 )
    {
      if ( BddNode->REF_EXT < BDD_MAX_REF )
      {
        BddNode->REF_EXT--;
      }
    }
    else
    {
      bdderror( BDD_NEGATIVE_REF_EXT_ERROR, BddNode->INDEX );
    }
  }

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                    Bdd Dec Internal Reference               |
|                                                             |
\------------------------------------------------------------*/

bddnode *decbddrefint( BddNode )

  bddnode *BddNode;
{
  if ( ( BddNode->INDEX   >= BDD_INDEX_MIN ) &&
       ( BddNode->REF_INT >  0             ) &&
       ( BddNode->REF_INT < BDD_MAX_REF    ) )
  {
    BddNode->REF_INT--;
  }

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                    Bdd Set External Reference               |
|                                                             |
\------------------------------------------------------------*/

bddnode *setbddrefext( BddNode )

  bddnode *BddNode;
{
  if ( BddNode->INDEX >= BDD_INDEX_MIN )
  {
    if ( BddNode->REF_INT > 0 )
    {
      if ( BddNode->REF_INT < BDD_MAX_REF )
      {
        BddNode->REF_INT--;
      }
    }
    else
    {
      bdderror( BDD_NEGATIVE_REF_INT_ERROR, BddNode->INDEX );
    }

    if ( BddNode->REF_EXT < BDD_MAX_REF )
    {
      BddNode->REF_EXT++;

      if ( BddNode->REF_EXT == BDD_MAX_REF )
      {
        if ( ! IsBddSystemNoWarning( BddLocalSystem ) )
        {
          bddwarning( BDD_OVERFLOW_REF_EXT_WARNING, BddNode->INDEX, 0 );
        }
      }
    }
  }

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                    Bdd UnSet External Reference             |
|                                                             |
\------------------------------------------------------------*/

bddnode *unsetbddrefext( BddNode )

  bddnode *BddNode;
{
  if ( BddNode->INDEX >= BDD_INDEX_MIN )
  {
    if ( BddNode->REF_EXT > 0 )
    {
      if ( BddNode->REF_EXT < BDD_MAX_REF )
      {
        BddNode->REF_EXT--;
      }
    }
    else
    {
      bdderror( BDD_NEGATIVE_REF_EXT_ERROR, BddNode->INDEX );
    }

    if ( BddNode->REF_INT < BDD_MAX_REF )
    {
      BddNode->REF_INT++;

      if ( BddNode->REF_INT == BDD_MAX_REF )
      {
        if ( ! IsBddSystemNoWarning( BddLocalSystem ) )
        {
          bddwarning( BDD_OVERFLOW_REF_INT_WARNING, BddNode->INDEX, 0 );
        }
      }
    }
  }

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                   Bdd Clear Internal Reference              |
|                                                             |
\------------------------------------------------------------*/

void clearbddsystemrefint( BddSystem )

  bddsystem *BddSystem;
{
  bddindexnode  *IndexNode;
  bddhnodetable *HashTable;
  bddnode       *HashNode;
  bddindex      *VarToIndex;
  bddvar         NumberVar;
  bddvar         Variable;
  long           HashIndex;

  setbddlocalsystem( BddSystem );

  NumberVar   = BddLocalSystem->NUMBER_VAR;
  IndexNode   = BddLocalSystem->INDEX_NODE;
  VarToIndex  = BddLocalSystem->VAR_TO_INDEX;

  for ( Variable = 0; Variable < NumberVar; Variable++ )
  {
    HashTable = IndexNode[ VarToIndex[ Variable ] ];

    for ( HashIndex = 0; HashIndex < HashTable->TABLE_SIZE; HashIndex++ )
    {
      HashNode = HashTable->TABLE[ HashIndex ];

      if ( ( HashNode != BDD_HASH_NODE_EMPTY   ) &&
           ( HashNode != BDD_HASH_NODE_DELETED ) )
      {
        HashNode->REF_INT = 0;
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   Bdd Clear External Reference              |
|                                                             |
\------------------------------------------------------------*/

void clearbddsystemrefext( BddSystem )

  bddsystem *BddSystem;
{
  bddindexnode  *IndexNode;
  bddhnodetable *HashTable;
  bddnode       *HashNode;
  bddnode       *BddNode;
  bddindex      *VarToIndex;
  bddvarnode    *VarNode;
  bddvar         NumberVar;
  bddvar         Variable;
  bddindex       Index;
  long           HashIndex;

  setbddlocalsystem( BddSystem );

  NumberVar   = BddLocalSystem->NUMBER_VAR;
  IndexNode   = BddLocalSystem->INDEX_NODE;
  VarToIndex  = BddLocalSystem->VAR_TO_INDEX;
  VarNode     = BddLocalSystem->VAR_NODE;

  for ( Variable = 0; Variable < NumberVar; Variable++ )
  {
    Index     = VarToIndex[ Variable ];
    BddNode   = VarNode   [ Index    ];
    HashTable = IndexNode [ Index    ];

    for ( HashIndex = 0; HashIndex < HashTable->TABLE_SIZE; HashIndex++ )
    {
      HashNode = HashTable->TABLE[ HashIndex ];

      if ( ( HashNode != BDD_HASH_NODE_EMPTY   ) &&
           ( HashNode != BDD_HASH_NODE_DELETED ) &&
           ( HashNode != BddNode               ) )
      {
        HashNode->REF_EXT = 0;
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      Bdd Clear Reference                    |
|                                                             |
\------------------------------------------------------------*/

void clearbddsystemref( BddSystem )

  bddsystem *BddSystem;
{
  bddindexnode  *IndexNode;
  bddhnodetable *HashTable;
  bddnode       *HashNode;
  bddnode       *BddNode;
  bddindex      *VarToIndex;
  bddvarnode    *VarNode;
  bddvar         NumberVar;
  bddvar         Variable;
  bddindex       Index;
  long           HashIndex;

  setbddlocalsystem( BddSystem );

  NumberVar   = BddLocalSystem->NUMBER_VAR;
  IndexNode   = BddLocalSystem->INDEX_NODE;
  VarToIndex  = BddLocalSystem->VAR_TO_INDEX;
  VarNode     = BddLocalSystem->VAR_NODE;

  for ( Variable = 0; Variable < NumberVar; Variable++ )
  {
    Index     = VarToIndex[ Variable ];
    BddNode   = VarNode   [ Index    ];
    HashTable = IndexNode [ Index    ];

    for ( HashIndex = 0; HashIndex < HashTable->TABLE_SIZE; HashIndex++ )
    {
      HashNode = HashTable->TABLE[ HashIndex ];

      if ( ( HashNode != BDD_HASH_NODE_EMPTY   ) &&
           ( HashNode != BDD_HASH_NODE_DELETED ) )
      {
        if ( HashNode != BddNode )
        {
          HashNode->REF_EXT = 0;
        }

        HashNode->REF_INT = 0;
      }
    }
  }
}
