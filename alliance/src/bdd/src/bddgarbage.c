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
| File    :                  bddgarbage.c                     |
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
# include "bddgarbage.h"
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
|                   Bdd Garbage Collect System                |
|                                                             |
\------------------------------------------------------------*/

void garbagebddsystem( BddSystem )

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

  resetbddhopertable( BddLocalSystem->HASH_OPER );

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

  for ( Variable = 0; Variable < NumberVar; Variable++ )
  {
    HashTable = IndexNode[ VarToIndex[ Variable ] ];

    for ( HashIndex = 0; HashIndex < HashTable->TABLE_SIZE; HashIndex++ )
    {
      HashNode = HashTable->TABLE[ HashIndex ];

      if ( ( HashNode != BDD_HASH_NODE_EMPTY   ) &&
           ( HashNode != BDD_HASH_NODE_DELETED ) )
      {
        if ( ( HashNode->REF_INT ) ||
             ( HashNode->REF_EXT ) )
        {
          incbddrefint( HashNode->HIGH );
          incbddrefint( HashNode->LOW  );
        }
      }
    }
  }

  for ( Variable = 0; Variable < NumberVar; Variable++ )
  {
    sweepbddvar( (bddsystem *)0, Variable, 0 );
  }
}
