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
| File    :                  bddnode.c                        |
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
# include <string.h>
# include <memory.h>
# include "bddnode.h"
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

  static bddnode BddTerminalHighNode = 
  {
    (bddnode *)0,		/* HIGH			*/
    (bddnode *)0,		/* LOW			*/
    BDD_MAX_INDEX,		/* INDEX		*/
    (bddmark)0,			/* MARK			*/
    (bddflag)0,			/* FLAG			*/
    BDD_MAX_REF,		/* REF_EXT		*/
    BDD_MAX_REF			/* REF_INT		*/
  };

  static bddnode BddTerminalLowNode = 
  {
    (bddnode *)0,		/* HIGH			*/
    (bddnode *)0,		/* LOW			*/
    BDD_MAX_INDEX,		/* INDEX		*/
    (bddmark)0,			/* MARK			*/
    (bddflag)0,			/* FLAG			*/
    BDD_MAX_REF,		/* REF_EXT		*/
    BDD_MAX_REF			/* REF_INT		*/
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Get Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Add Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Add Bdd Node                        |
|                                                             |
\------------------------------------------------------------*/

bddnode *addbddnode( BddSystem, Index, High, Low  )

  bddsystem *BddSystem;
  bddindex   Index;
  bddnode   *High;
  bddnode   *Low;
{
  bddblock *BddBlock;
  bddnode  *BddNode;
  bddnode  *NewBddNode;

  setbddlocalsystem( BddSystem );

  if ( Index >= BddLocalSystem->NUMBER_INDEX )
  {
    bdderror( BDD_BAD_INDEX_ERROR, Index );
  }

  if ( ( ! IsBddSystemSwapVar( BddLocalSystem )                         ) &&
       ( IsBddSystemExplosionOn( BddLocalSystem )                       ) &&
       ( BddLocalSystem->EXPLOSION_LIMIT <= BddLocalSystem->NUMBER_NODE ) )
  {
    if ( ! IsBddSystemExplosion( BddLocalSystem ) )
    {
      BddLocalSystem->HASH_OPER->TABLE_FREEZE;

      if ( BddLocalSystem->EXPLOSION_FUNC != NULL )
      {
        BddLocalSystem->EXPLOSION_FUNC( BddLocalSystem );
      }
      else
      {
        if ( ! IsBddSystemNoWarning( BddLocalSystem ) )
        {
          bddwarning( BDD_DYNAMIC_EXPLOSION_WARNING,
                      BddLocalSystem->NUMBER_NODE, BddLocalSystem->EXPLOSION_LIMIT );
        }
      }

      SetBddSystemExplosion( BddLocalSystem );
    }

    return( BddLocalSystem->ZERO );
  }

  if ( High == (bddnode *)0 ) High = &BddTerminalHighNode;
  if ( Low  == (bddnode *)0 ) Low  = &BddTerminalLowNode;

  if ( High == Low )
  {
    return( incbddrefext( High ) );
  }

  BddBlock = BddLocalSystem->BLOCK;

  if ( BddLocalSystem->NODE_FREE == (bddnode *)0 )
  {
    if ( ( BddBlock == (bddblock *)0 ) ||
         ( BddBlock->NODE_FREE == 0  ) )
    {
      BddBlock = addbddblock( (bddsystem *)0 );
    }

    BddBlock->NODE_FREE--;
    BddNode = &BddBlock->NODE[ BddBlock->NODE_FREE ];
  }
  else
  {
    BddNode = BddLocalSystem->NODE_FREE;
    BddLocalSystem->NODE_FREE = BddNode->HIGH;

    BddLocalSystem->NUMBER_FREE--;
  }

  BddNode->HIGH  = High;
  BddNode->LOW   = Low;
  BddNode->INDEX = Index;

  NewBddNode = addbddhnode( BddLocalSystem->INDEX_NODE[ Index ], BddNode );

  if ( NewBddNode == BddNode )
  {
    incbddrefint( High );
    incbddrefint( Low  );
    BddNode->MARK = 0;
    BddNode->FLAG = 0;

    BddLocalSystem->NUMBER_NODE++;

    if ( BddLocalSystem->NUMBER_NODE > BddLocalSystem->TOP_NODE )
    {
      BddLocalSystem->TOP_NODE = BddLocalSystem->NUMBER_NODE;
    }
  }
  else
  {
    BddNode->LOW   = (bddnode *)0;
    BddNode->INDEX = 0;

    BddNode->HIGH  = BddLocalSystem->NODE_FREE;
    BddLocalSystem->NODE_FREE = BddNode;

    BddLocalSystem->NUMBER_FREE++;
  }

  return( incbddrefext( NewBddNode ) );
}

/*------------------------------------------------------------\
|                                                             |
|                      Bdd Add List Node                      |
|                                                             |
\------------------------------------------------------------*/

chain_list *addbddnodelist( BddSystem, HeadList, BddNode )

  bddsystem  *BddSystem;
  chain_list *HeadList;
  bddnode    *BddNode;
{
  chain_list **PrevList;
  chain_list  *ScanList;
  chain_list  *NewList;
  bddvar      *IndexToVar;
  bddvar       Variable;
  bddvar       ScanVar;

  setbddlocalsystem( BddSystem );

  NewList = addchain( (chain_list *)0, (void *)incbddrefext( BddNode ) );

  if ( HeadList == (chain_list *)0 )
  {
    return( NewList );
  }

  IndexToVar = BddLocalSystem->INDEX_TO_VAR;

  PrevList = &HeadList;
  ScanList = HeadList;
  Variable = IndexToVar[ BddNode->INDEX ];

  while ( ScanList != (chain_list *)0 )
  {
    ScanVar = IndexToVar[ ((bddnode *)ScanList->DATA)->INDEX ];

    if ( Variable <= ScanVar ) break;

    PrevList = &ScanList->NEXT;
    ScanList = ScanList->NEXT;
  }

  NewList->NEXT = ScanList;
  *PrevList     = NewList;

  return( HeadList );
}

/*------------------------------------------------------------\
|                                                             |
|                         Del Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   Local Bdd Del Node                        |
|                                                             |
\------------------------------------------------------------*/

static void loc_delbdd( BddNode )

  bddnode *BddNode;
{
  if ( ( BddNode->REF_INT == 0 ) &&
       ( BddNode->REF_EXT == 0 ) )
  {
    decbddrefint( BddNode->HIGH );
    decbddrefint( BddNode->LOW  );

    loc_delbdd( BddNode->HIGH );
    loc_delbdd( BddNode->LOW  );

    delbddhnode( BddLocalSystem->INDEX_NODE[ BddNode->INDEX ], BddNode );

    memset( (void *)BddNode, 0, sizeof( bddnode ) );

    BddNode->HIGH  = BddLocalSystem->NODE_FREE;
    BddLocalSystem->NODE_FREE = BddNode;
    BddLocalSystem->NUMBER_NODE--;
    BddLocalSystem->NUMBER_FREE++;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                         Bdd Del Node                        |
|                                                             |
\------------------------------------------------------------*/

void delbddnode( BddSystem, BddNode )

  bddsystem *BddSystem;
  bddnode   *BddNode;
{
  long NumberNode;

  setbddlocalsystem( BddSystem );

  NumberNode = BddLocalSystem->NUMBER_NODE;

  loc_delbdd( BddNode );

  if ( BddLocalSystem->NUMBER_NODE < NumberNode )
  {
    resetbddhopertable( BddLocalSystem->HASH_OPER );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      Bdd Del List Node                      |
|                                                             |
\------------------------------------------------------------*/

void delbddnodelist( BddSystem, HeadList )

  bddsystem  *BddSystem;
  chain_list *HeadList;
{
  chain_list  *ScanList;

  setbddlocalsystem( BddSystem );

  for ( ScanList  = HeadList;
        ScanList != (chain_list *)0;
        ScanList  = ScanList->NEXT )
  {
    decbddrefext( (bddnode *)ScanList->DATA );
  }

  freechain( HeadList );
}

/*------------------------------------------------------------\
|                                                             |
|                        View Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Bdd View Node                        |
|                                                             |
\------------------------------------------------------------*/

void viewbddnode( BddSystem, BddNode )

  bddsystem *BddSystem;
  bddnode   *BddNode;
{
  if ( BddSystem == (bddsystem *)0 )
  {
    BddSystem = BddLocalSystem;
  }

  fprintf( stdout, 
    "Node VAR: %ld  INDEX: %ld  MARK: %ld  FLAG: %ld  REF_EXT: %ld  REF_INT: %ld\n",
           (long)( BddSystem->INDEX_TO_VAR[ BddNode->INDEX ] ),
           (long)( BddNode->INDEX   ),
           (long)( BddNode->MARK    ),
           (long)( BddNode->FLAG    ),
           (long)( BddNode->REF_EXT ),
           (long)( BddNode->REF_INT ) );
}
