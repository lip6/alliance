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
| File    :                  bddvaraux.c                      |
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

  static bddhnode *BddAuxVarNodeArray  = (bddhnode *)0;
  static long      BddAuxVarNodeSize   = 0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                Add Bdd Single Auxiliary Variable            |
|                                                             |
\------------------------------------------------------------*/

bddnode *addbddvarauxsingle( BddSystem, BddAux )

  bddsystem *BddSystem;
  bddnode   *BddAux;
{
  bddindexnode  *IndexNode;
  bddnode       *BddNotAux;
  bddnode       *BddNodeAux;
  bddnode       *BddNodeNotAux;
  bddvar         Variable;
  bddindex       AuxIndex;
  bddindex       Index;

  Index = BddAux->INDEX;

  checkbddindex( BddSystem, Index, 1 );

  BddNotAux = decbddrefext( loc_applybddnot( BddAux ) );

  Variable  = BddLocalSystem->INDEX_TO_VAR[ Index ];
  AuxIndex  = newbddvar( (bddsystem *)0, Variable );

  IndexNode = BddLocalSystem->INDEX_NODE;

  delbddhnode( IndexNode[ Index ], BddNotAux );
  BddNodeNotAux = addbddnode( (bddsystem *)0, Index, BddNotAux->HIGH, BddNotAux->LOW );

  BddNotAux->INDEX   = AuxIndex;
  BddNotAux->HIGH    = BddLocalSystem->ZERO;
  BddNotAux->LOW     = BddLocalSystem->ONE;

  addbddhnode( IndexNode[ AuxIndex ], BddNotAux );

  delbddhnode( IndexNode[ Index ], BddAux );
  BddNodeAux = addbddnode( (bddsystem *)0, Index, BddAux->HIGH, BddAux->LOW );

  BddAux->INDEX   = AuxIndex;
  BddAux->HIGH    = BddLocalSystem->ONE;
  BddAux->LOW     = BddLocalSystem->ZERO;
  BddAux->REF_EXT = BDD_MAX_REF;

  addbddhnode( IndexNode[ AuxIndex ], BddAux );

  BddLocalSystem->VAR_NODE[ AuxIndex ] = BddAux;

  resetbddhopertable( BddLocalSystem->HASH_OPER );

  decbddrefext( BddNodeNotAux );

  return( BddNodeAux );
}

/*------------------------------------------------------------\
|                                                             |
|               Add Bdd Global Auxiliary Variable             |
|                                                             |
\------------------------------------------------------------*/

bddnode *addbddvarauxglobal( BddSystem, BddAux, UserFunc )

  bddsystem *BddSystem;
  bddnode   *BddAux;
  int      (*UserFunc)();
{
  bddindexnode  *IndexNode;
  bddhnodetable *HashTable;
  bddnode       *HashNode;
  bddnode        NewNode;
  bddnode       *BddNotAux;
  bddnode       *BddNodeAux;
  bddnode       *BddNodeNotAux;
  long           AuxVarNodeIndex;
  long           HashIndex;
  bddvar         Variable;
  bddindex       AuxIndex;
  bddindex       Index;

  Index = BddAux->INDEX;

  checkbddindex( BddSystem, Index, 1 );

  BddNotAux = decbddrefext( loc_applybddnot( BddAux ) );

  Variable  = BddLocalSystem->INDEX_TO_VAR[ Index ];
  AuxIndex  = newbddvar( (bddsystem *)0, Variable );

  IndexNode = BddLocalSystem->INDEX_NODE;
  HashTable = IndexNode[ Index ];

  if ( BddAuxVarNodeSize < HashTable->NUMBER_NODE )
  {
    freebddhnode( BddAuxVarNodeArray );
    BddAuxVarNodeSize  = HashTable->NUMBER_NODE << 1;
    BddAuxVarNodeArray = allocbddhnode( BddAuxVarNodeSize );
  }

  AuxVarNodeIndex = 0;

  for ( HashIndex = 0; HashIndex < HashTable->TABLE_SIZE; HashIndex++ )
  {
    HashNode = HashTable->TABLE[ HashIndex ];

    if ( ( HashNode != BDD_HASH_NODE_EMPTY   ) &&
         ( HashNode != BDD_HASH_NODE_DELETED ) )
    {
      if ( ( HashNode != BddAux    ) &&
           ( HashNode != BddNotAux ) )
      {
        BddAuxVarNodeArray[ AuxVarNodeIndex ] = HashNode;
        AuxVarNodeIndex = AuxVarNodeIndex + 1;
      }
    }
  }

  delbddhnode( IndexNode[ Index ], BddNotAux );
  BddNodeNotAux = addbddnode( (bddsystem *)0, Index, BddNotAux->HIGH, BddNotAux->LOW );

  BddNotAux->INDEX   = AuxIndex;
  BddNotAux->HIGH    = BddLocalSystem->ZERO;
  BddNotAux->LOW     = BddLocalSystem->ONE;

  addbddhnode( IndexNode[ AuxIndex ], BddNotAux );

  delbddhnode( IndexNode[ Index ], BddAux );
  BddNodeAux = addbddnode( (bddsystem *)0, Index, BddAux->HIGH, BddAux->LOW );

  BddAux->INDEX   = AuxIndex;
  BddAux->HIGH    = BddLocalSystem->ONE;
  BddAux->LOW     = BddLocalSystem->ZERO;
  BddAux->REF_EXT = BDD_MAX_REF;

  addbddhnode( IndexNode[ AuxIndex ], BddAux );

  BddLocalSystem->VAR_NODE[ AuxIndex ] = BddAux;

  resetbddhopertable( BddLocalSystem->HASH_OPER );

  BddLocalSystem->HASH_OPER->TABLE_FREEZE = 1;

  NewNode.INDEX   = AuxIndex;
  NewNode.REF_INT = 0;
  NewNode.REF_EXT = 1;
  NewNode.MARK    = 0;

  while ( AuxVarNodeIndex > 0 )
  {
    AuxVarNodeIndex = AuxVarNodeIndex - 1;
    HashNode = BddAuxVarNodeArray[ AuxVarNodeIndex ];

    NewNode.HIGH = decbddrefext( loc_cofactorbdd( HashNode, BddNodeAux    ) );
    NewNode.LOW  = decbddrefext( loc_cofactorbdd( HashNode, BddNodeNotAux ) );

    if ( ( NewNode.HIGH != NewNode.LOW ) &&
         ( NewNode.HIGH != HashNode    ) &&
         ( NewNode.LOW  != HashNode    ) )
    {
      if ( UserFunc )
      {
        if ( ! UserFunc( HashNode, &NewNode ) ) continue;
      }

      delbddhnode( IndexNode[ Index ], HashNode );

      HashNode->INDEX = AuxIndex;
      HashNode->HIGH  = incbddrefint( NewNode.HIGH );
      HashNode->LOW   = incbddrefint( NewNode.LOW  );

      addbddhnode( IndexNode[ AuxIndex ], HashNode );
    }
  }

  BddLocalSystem->HASH_OPER->TABLE_FREEZE = 0;

  decbddrefext( BddNodeNotAux );

  return( BddNodeAux );
}
