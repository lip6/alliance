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
| File    :                   bddvar.c                        |
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
# include <memory.h>
# include <string.h>

# include "bddvar.h"
# include "bddhnode.h"
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

  long BDD_VAR_STRETCH_FACTOR = VAR_STRETCH_FACTOR;

  static bddhnode *BddSwapVarNodeArray  = (bddhnode *)0;
  static long      BddSwapVarNodeSize   = 0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        New Bdd Variable                     |
|                                                             |
\------------------------------------------------------------*/

bddindex newbddvar( BddSystem, Variable )

  bddsystem *BddSystem;
  bddvar     Variable;
{
  bddvarnode   *VarNode;
  bddindexnode *IndexNode;
  bddindex     *IndexToVar;
  bddvar       *VarToIndex;
  bddassoc     *BddAssoc;
  bddassocnode *BddAssocNode;
  long          MaxVar;
  long          MaxIndex;
  long          NumberVar;
  long          NumberIndex;
  long          Counter;
  
  setbddlocalsystem( BddSystem );

  NumberVar   = BddLocalSystem->NUMBER_VAR;
  MaxVar      = BddLocalSystem->MAX_VAR;

  if ( NumberVar >= BDD_MAX_VAR )
  {
    bdderror( BDD_NO_MORE_VAR_ERROR, 0 );
  }

  if ( Variable > NumberVar )
  {
    bdderror( BDD_BAD_VAR_ERROR, Variable );
  }

  if ( NumberVar == MaxVar )
  {
    MaxVar = MaxVar * BDD_VAR_STRETCH_FACTOR;

    if ( MaxVar >= BDD_MAX_VAR )
    {
      MaxVar = BDD_MAX_VAR - 1;
    }

    MaxIndex = MaxVar + BDD_INDEX_MIN;

    VarNode = resizebddvarnode( BddLocalSystem->VAR_NODE, 
                                BddLocalSystem->MAX_INDEX, MaxIndex );

    IndexNode = resizebddindexnode( BddLocalSystem->INDEX_NODE,
                                    BddLocalSystem->MAX_INDEX, MaxIndex );

    IndexToVar = resizebddvar( BddLocalSystem->INDEX_TO_VAR,
                               BddLocalSystem->MAX_INDEX, MaxIndex );

    VarToIndex = resizebddindex( BddLocalSystem->VAR_TO_INDEX,
                                 BddLocalSystem->MAX_VAR, MaxVar );

    for ( BddAssoc  = BddLocalSystem->ASSOC;
          BddAssoc != (bddassoc *)0;
          BddAssoc  = BddAssoc->NEXT )
    {
      BddAssoc->ASSOC_NODE = 

        resizebddassocnode( BddAssoc->ASSOC_NODE,
                            BddLocalSystem->MAX_VAR, MaxVar );
    }

    BddLocalSystem->MAX_VAR      = MaxVar;
    BddLocalSystem->MAX_INDEX    = MaxIndex;
    BddLocalSystem->VAR_NODE     = VarNode;
    BddLocalSystem->INDEX_NODE   = IndexNode;
    BddLocalSystem->INDEX_TO_VAR = IndexToVar;
    BddLocalSystem->VAR_TO_INDEX = VarToIndex;
  }
  else
  {
    VarNode    = BddLocalSystem->VAR_NODE;
    IndexNode  = BddLocalSystem->INDEX_NODE;
    IndexToVar = BddLocalSystem->INDEX_TO_VAR;
    VarToIndex = BddLocalSystem->VAR_TO_INDEX;
  }

  NumberIndex = BddLocalSystem->NUMBER_INDEX;

  if ( Variable < NumberVar )
  {
    for ( Counter = BDD_INDEX_MIN; Counter < NumberIndex; Counter++ )
    {
      if ( IndexToVar[ Counter ] >= Variable ) 
      {
        IndexToVar[ Counter ]++;
      }
    }

    for ( BddAssoc  = BddLocalSystem->ASSOC;
          BddAssoc != (bddassoc *)0;
          BddAssoc  = BddAssoc->NEXT )
    {
      if ( ( BddAssoc->FIRST != BDD_MAX_VAR ) &&
           ( BddAssoc->LAST  != BDD_MAX_VAR ) &&
           ( BddAssoc->LAST  >= Variable    ) )
      {
        BddAssoc->LAST = BddAssoc->LAST + 1;
        BddAssocNode   = BddAssoc->ASSOC_NODE;

        for ( Counter = BddAssoc->LAST; Counter > Variable; Counter-- )
        {
          if ( Counter == BddAssoc->FIRST ) break;

          BddAssocNode[ Counter ] = BddAssocNode[ Counter - 1 ];
        }

        if ( Counter == BddAssoc->FIRST )
        {
          BddAssoc->FIRST = BddAssoc->FIRST + 1; 
        }

        BddAssocNode[ Counter ] = (bddnode *)0;
      }
    }
  }

  IndexToVar[ NumberIndex ] = Variable;

  for ( Counter = NumberVar; Counter > Variable; Counter-- )
  {
    VarToIndex[ Counter ] = VarToIndex[ Counter - 1 ];
  }

  VarToIndex[ Variable ] = NumberIndex;

  BddLocalSystem->NUMBER_VAR++;
  BddLocalSystem->NUMBER_INDEX++;

  if ( IndexNode[ NumberIndex ] == (bddhnodetable *)0 )
  {
    IndexNode[ NumberIndex ] = createbddhnodetable( BddLocalSystem->VAR_MODEL );
  }

  return( (bddindex)NumberIndex );
}

/*------------------------------------------------------------\
|                                                             |
|                        Add Bdd Variable                     |
|                                                             |
\------------------------------------------------------------*/

bddnode *addbddvar( BddSystem, Variable )

  bddsystem *BddSystem;
  bddvar     Variable;
{
  bddnode    *BddNode;
  bddindex    Index;

  Index = newbddvar( BddSystem, Variable );

  BddNode = addbddnode( (bddsystem *)0, Index,
                        BddLocalSystem->ONE, BddLocalSystem->ZERO );

  BddNode->REF_EXT = BDD_MAX_REF;

  BddLocalSystem->VAR_NODE[ Index ] = BddNode;

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                        Add Bdd Last Variable                |
|                                                             |
\------------------------------------------------------------*/

bddnode *addbddvarlast( BddSystem )

  bddsystem *BddSystem;
{
  setbddlocalsystem( BddSystem );

  return( addbddvar( (bddsystem *)0,
                     BddLocalSystem->NUMBER_VAR ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Add Bdd First Variable               |
|                                                             |
\------------------------------------------------------------*/

bddnode *addbddvarfirst( BddSystem )

  bddsystem *BddSystem;
{
  setbddlocalsystem( BddSystem );

  return( addbddvar( (bddsystem *)0, 0 ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Add Bdd Before Variable              |
|                                                             |
\------------------------------------------------------------*/

bddnode *addbddvarbefore( BddSystem, Index )

  bddsystem *BddSystem;
  bddindex   Index;
{
  bddvar   Variable;

  checkbddindex( BddSystem, Index, 1 );

  Variable = BddLocalSystem->INDEX_TO_VAR[ Index ];

  return( addbddvar( (bddsystem *)0, Variable ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Add Bdd After Variable               |
|                                                             |
\------------------------------------------------------------*/

bddnode *addbddvarafter( BddSystem, Index )

  bddsystem *BddSystem;
  bddindex   Index;
{
  bddvar   Variable;

  checkbddindex( BddSystem, Index, 1 );

  Variable = BddLocalSystem->INDEX_TO_VAR[ Index ] + 1;

  return( addbddvar( (bddsystem *)0, Variable ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Sweep Bdd Variable                    |
|                                                             |
\------------------------------------------------------------*/

void sweepbddvar( BddSystem, Variable, SweepRef )

  bddsystem *BddSystem;
  bddvar     Variable;
  int        SweepRef;
{
  bddindexnode  *IndexNode;
  bddhnodetable *HashTable;
  bddnode       *HashNode;
  bddindex      *VarToIndex;
  long           HashIndex;

  checkbddvar( BddSystem, Variable, 1 );

  VarToIndex = BddLocalSystem->VAR_TO_INDEX;
  IndexNode  = BddLocalSystem->INDEX_NODE;

  HashTable = IndexNode[ VarToIndex[ Variable ] ];

  for ( HashIndex = 0; HashIndex < HashTable->TABLE_SIZE; HashIndex++ )
  {
    HashNode = HashTable->TABLE[ HashIndex ];

    if ( ( HashNode != BDD_HASH_NODE_EMPTY   ) &&
         ( HashNode != BDD_HASH_NODE_DELETED ) )
    {
      if ( ( ! HashNode->REF_INT ) &&
           ( ! HashNode->REF_EXT ) )
      {
        HashTable->NUMBER_NODE--;
        HashTable->TABLE[ HashIndex ] = BDD_HASH_NODE_DELETED;

        if ( SweepRef )
        { 
          decbddrefint( HashNode->LOW  );
          decbddrefint( HashNode->HIGH );
        }

        memset( (void *)HashNode, 0, sizeof( bddnode ) );

        HashNode->HIGH  = BddLocalSystem->NODE_FREE;
        BddLocalSystem->NODE_FREE = HashNode;
        BddLocalSystem->NUMBER_NODE--;
        BddLocalSystem->NUMBER_FREE++;
      }
    }
  }

  resizebddhnodetable( HashTable );
  resetbddhopertable( BddLocalSystem->HASH_OPER );
}

/*------------------------------------------------------------\
|                                                             |
|                       Swap Bdd Variable                     |
|                                                             |
\------------------------------------------------------------*/

int swapbddvar( BddSystem, Variable )

  bddsystem *BddSystem;
  bddvar     Variable;
{
  bddindexnode  *IndexNode;
  bddhnodetable *HashTable;
  bddhnodetable *NextTable;
  bddvar        *VarToIndex;
  bddindex      *IndexToVar;
  bddassoc      *BddAssoc;
  bddassocnode  *BddAssocNode;
  bddnode       *HashNode;
  bddnode       *BddNodeLow;
  bddnode       *BddNodeHigh;
  bddnode       *BddNodeF11;
  bddnode       *BddNodeF01;
  bddnode       *BddNodeF10;
  bddnode       *BddNodeF00;
  bddvar         NextVar;
  bddvar         FirstVar;
  bddvar         LastVar;
  bddindex       Index;
  bddindex       NextIndex;
  long           HashIndex;
  long           SwapVarNodeIndex;
  long           NodeIndex;

  NextVar = Variable + 1;

  checkbddvar( BddSystem, NextVar, 1 );

  SetBddSystemSwapVar( BddLocalSystem );

  IndexNode   = BddLocalSystem->INDEX_NODE;
  VarToIndex  = BddLocalSystem->VAR_TO_INDEX;
  IndexToVar  = BddLocalSystem->INDEX_TO_VAR;

  Index     = VarToIndex[ Variable ];
  NextIndex = VarToIndex[ NextVar  ];

  HashTable = IndexNode[ Index     ];
  NextTable = IndexNode[ NextIndex ];

  if ( BddSwapVarNodeSize < HashTable->NUMBER_NODE )
  {
    if ( BddSwapVarNodeArray != (bddhnode *)0 )
    {
      freebddhnode( BddSwapVarNodeArray );
    }

    BddSwapVarNodeSize  = HashTable->NUMBER_NODE << 1;
    BddSwapVarNodeArray = allocbddhnode( BddSwapVarNodeSize );
  }

  SwapVarNodeIndex = 0;

  for ( HashIndex = 0; HashIndex < HashTable->TABLE_SIZE; HashIndex++ )
  {
    HashNode = HashTable->TABLE[ HashIndex ];

    if ( ( HashNode != BDD_HASH_NODE_EMPTY   ) &&
         ( HashNode != BDD_HASH_NODE_DELETED ) )
    {
      if ( ( HashNode->HIGH->INDEX == NextIndex ) ||
           ( HashNode->LOW->INDEX  == NextIndex ) )
      {
        HashTable->NUMBER_NODE--;
        HashTable->TABLE[ HashIndex ] = BDD_HASH_NODE_DELETED;

        BddSwapVarNodeArray[ SwapVarNodeIndex ] = HashNode;
        SwapVarNodeIndex = SwapVarNodeIndex + 1;
      }
    }
  }

  for ( NodeIndex = 0; NodeIndex < SwapVarNodeIndex; NodeIndex++ )
  {
    HashNode = BddSwapVarNodeArray[ NodeIndex ];

    BddNodeLow  = HashNode->LOW;
    BddNodeHigh = HashNode->HIGH;

    if ( BddNodeHigh->INDEX == NextIndex )
    {
      BddNodeF11 = BddNodeHigh->HIGH;
      BddNodeF10 = BddNodeHigh->LOW;
    }
    else
    {
      BddNodeF11 = BddNodeHigh;
      BddNodeF10 = BddNodeHigh;
    }

    if ( BddNodeLow->INDEX == NextIndex )
    {
      BddNodeF01 = BddNodeLow->HIGH;
      BddNodeF00 = BddNodeLow->LOW;
    }
    else
    {
      BddNodeF01 = BddNodeLow;
      BddNodeF00 = BddNodeLow;
    }

    decbddrefint( BddNodeHigh );
    decbddrefint( BddNodeLow  );

    BddNodeHigh = unsetbddrefext( addbddnode( (bddsystem *)0, Index,
                                              BddNodeF11, BddNodeF01 ) );
    BddNodeLow  = unsetbddrefext( addbddnode( (bddsystem *)0, Index,
                                              BddNodeF10, BddNodeF00 ) );
    HashNode->INDEX = NextIndex;
    HashNode->HIGH  = BddNodeHigh;
    HashNode->LOW   = BddNodeLow;

    addbddhnode( NextTable, HashNode );
  }

  VarToIndex[ NextVar  ] = Index;
  VarToIndex[ Variable ] = NextIndex;

  IndexToVar[ NextIndex ] = Variable;
  IndexToVar[ Index     ] = NextVar;

  for ( BddAssoc  = BddLocalSystem->ASSOC;
        BddAssoc != (bddassoc *)0;
        BddAssoc  = BddAssoc->NEXT )
  {
    BddAssocNode = BddAssoc->ASSOC_NODE;

    BddNodeLow  = BddAssocNode[ Variable ];
    BddNodeHigh = BddAssocNode[ NextVar  ];

    FirstVar = BddAssoc->FIRST;
    LastVar  = BddAssoc->LAST ;

    if ( ( Variable    == FirstVar     ) &&
         ( BddNodeHigh == (bddnode *)0 ) ) BddAssoc->FIRST++;

    if ( ( NextVar     == LastVar      ) &&
         ( BddNodeLow  == (bddnode *)0 ) ) BddAssoc->LAST --;

    if ( NextVar  == FirstVar ) BddAssoc->FIRST--;
    if ( Variable == LastVar  ) BddAssoc->LAST ++;

    BddAssocNode[ NextVar  ] = BddNodeLow;
    BddAssocNode[ Variable ] = BddNodeHigh;
  }

  sweepbddvar( (bddsystem *)0, Variable, 1 );

  ClearBddSystemSwapVar( BddLocalSystem );

  return ( SwapVarNodeIndex != 0L );
}

/*------------------------------------------------------------\
|                                                             |
|                   Get Bdd Variable By Index                 |
|                                                             |
\------------------------------------------------------------*/

bddvar getbddvarbyindex( BddSystem, Index )

  bddsystem *BddSystem;
  bddindex   Index;
{
  checkbddindex( BddSystem, Index, 1 );

  return( BddLocalSystem->INDEX_TO_VAR[ Index ] );
}

/*------------------------------------------------------------\
|                                                             |
|                   Get Bdd Variable Index                    |
|                                                             |
\------------------------------------------------------------*/

bddindex getbddvarindex( BddSystem, Variable )

  bddsystem *BddSystem;
  bddvar     Variable;
{
  checkbddvar( BddSystem, Variable, 1 );

  return( BddLocalSystem->VAR_TO_INDEX[ Variable ] );
}

/*------------------------------------------------------------\
|                                                             |
|                   Get Bdd Variable Node                     |
|                                                             |
\------------------------------------------------------------*/

bddnode *getbddvarnode( BddSystem, Variable )

  bddsystem *BddSystem;
  bddvar     Variable;
{
  bddindex Index;

  checkbddvar( BddSystem, Variable, 1 );

  Index = BddLocalSystem->VAR_TO_INDEX[ Variable ];

  return( BddLocalSystem->VAR_NODE[ Index ] );
}

/*------------------------------------------------------------\
|                                                             |
|               Get Bdd Variable Node By Index                |
|                                                             |
\------------------------------------------------------------*/

bddnode *getbddvarnodebyindex( BddSystem, Index )

  bddsystem *BddSystem;
  bddindex   Index;
{
  checkbddindex( BddSystem, Index, 1 );

  return( BddLocalSystem->VAR_NODE[ Index ] );
}
