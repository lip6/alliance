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
| File    :                   bddblock.c                      |
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
# include <stdlib.h>
# include <memory.h>
# include <string.h>

# include "bddblock.h"
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
|                       Bdd Add Block                         |
|                                                             |
\------------------------------------------------------------*/

bddblock *addbddblock( BddSystem )

  bddsystem *BddSystem;
{
  bddblock *BddBlock;

  setbddlocalsystem( BddSystem );

  BddBlock              = allocbddblock();
  BddBlock->NEXT        = BddLocalSystem->BLOCK;
  BddLocalSystem->BLOCK = BddBlock;

  BddBlock->NODE      = allocbddnodeblock( BDD_NODE_BLOCK_SIZE );
  BddBlock->NODE_FREE = BDD_NODE_BLOCK_SIZE;

  return( BddBlock );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Create Block                      |
|                                                             |
\------------------------------------------------------------*/

bddblock *createbddblock( BddSystem )

  bddsystem *BddSystem;
{
  return( addbddblock( BddSystem ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Reset Block                       |
|                                                             |
\------------------------------------------------------------*/

void resetbddblock( BddSystem )

  bddsystem *BddSystem;
{
  bddblock  *BddBlock;
  bddnode  **PrevNode;
  bddnode   *ScanNode;
  long       Counter;

  setbddlocalsystem( BddSystem );

  BddBlock =  BddLocalSystem->BLOCK;
  PrevNode = &BddLocalSystem->NODE_FREE;

  for ( BddBlock  = BddLocalSystem->BLOCK;
        BddBlock != (bddblock *)0;
        BddBlock  = BddBlock->NEXT )
  {
    ScanNode = BddBlock->NODE;

    memset( (void *)ScanNode, 0,
            (size_t)BDD_NODE_BLOCK_SIZE * sizeof( bddnode ) );

    for ( Counter = 0; Counter < BDD_NODE_BLOCK_SIZE; Counter++ )
    {
     *PrevNode =  ScanNode;
      PrevNode = &ScanNode->HIGH;
      ScanNode =  ScanNode + 1;
    }

    BddBlock->NODE_FREE = 0;
    BddLocalSystem->NUMBER_FREE += BDD_NODE_BLOCK_SIZE;
  }

  *PrevNode = (bddnode *)0;

  BddLocalSystem->NUMBER_NODE = 0;
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Destroy Block                     |
|                                                             |
\------------------------------------------------------------*/

void destroybddblock( BddSystem )

  bddsystem *BddSystem;
{
  bddblock *BddBlock;
  bddblock *DelBddBlock;

  setbddlocalsystem( BddSystem );

  BddBlock = BddLocalSystem->BLOCK;

  while ( BddBlock != (bddblock *)0 )
  {
    DelBddBlock = BddBlock;
    BddBlock    = BddBlock->NEXT;

    freebddnodeblock( DelBddBlock->NODE );
    freebddblock( DelBddBlock );
  }

  BddLocalSystem->BLOCK       = (bddblock *)0;
  BddLocalSystem->NODE_FREE   = (bddnode  *)0;
  BddLocalSystem->NUMBER_FREE = 0;
  BddLocalSystem->NUMBER_NODE = 0;
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd View Block                        |
|                                                             |
\------------------------------------------------------------*/

void viewbddblock( BddSystem, FuncView )

  bddsystem *BddSystem;
  void       (*FuncView)();
{
  bddblock  *BddBlock;
  bddnode   *ScanNode;
  long       Counter;
  long       NumberBlock;
  long       NumberNode;

  if ( BddSystem == (bddsystem *)0 )
  {
    BddSystem = BddLocalSystem;
  }

  fprintf( stdout, "--> BlockNodeTable\n" );

  fprintf( stdout, "  NUMBER_FREE : %ld\n", BddSystem->NUMBER_FREE );

  NumberBlock = 0;

  for ( BddBlock  = BddLocalSystem->BLOCK;
        BddBlock != (bddblock *)0;
        BddBlock  = BddBlock->NEXT )
  {
    fprintf( stdout, "--> BLOCK_NODE[ %ld ]\n", NumberBlock );

    ScanNode   = BddBlock->NODE;
    NumberNode = 0;

    for ( Counter = 0; Counter < BDD_NODE_BLOCK_SIZE; Counter++ )
    {
      if ( ScanNode[ Counter ].LOW != (bddnode *)0 )
      {
        NumberNode = NumberNode + 1;

        if ( FuncView ) (*FuncView)( ScanNode[ Counter ] );
      }
    }

    fprintf( stdout, "  NUMBER_NODE : %ld\n", NumberNode );
    fprintf( stdout, "<-- BLOCK_NODE[ %ld ]\n", NumberBlock );

    NumberBlock = NumberBlock + 1;
  }

  fprintf( stdout, "  NUMBER_BLOCK : %ld\n", NumberBlock );
  fprintf( stdout, "<-- BlockNodeTable\n" );
}
