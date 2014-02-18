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
| File    :                  bddsystem.c                      |
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
# include "bddsystem.h"
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

  bddsystem *BddLocalSystem = (bddsystem *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Bdd Create System                     |
|                                                             |
\------------------------------------------------------------*/

bddsystem *createbddsystem( ModelVar, ModelOper, MaxVar, MaxNode  )

  long   ModelVar;
  long   ModelOper;
  long   MaxVar;
  long   MaxNode;
{
  bddsystem    *BddSystem;
  bddnode      *BddOne;
  bddnode      *BddZero;
  long          MaxIndex;

  MaxIndex = BDD_INDEX_MIN + MaxVar;

  if ( ModelVar <= 0 )
  {
    bdderror( BDD_MODEL_VAR_ERROR, ModelVar );
  }

  if ( MaxVar >= BDD_MAX_VAR )
  {
    bdderror( BDD_BAD_VAR_ERROR, MaxVar );
  }

  if ( ModelOper <= 0 )
  {
    bdderror( BDD_MODEL_OPER_ERROR, ModelOper );
  }

  BddSystem = allocbddsystem();

  BddSystem->HASH_OPER  = createbddhopertable( ModelOper );
  BddSystem->OPER_MODEL = ModelOper;

  BddSystem->MAX_VAR    = MaxVar;
  BddSystem->MAX_INDEX  = MaxIndex;
  BddSystem->MAX_NODE   = MaxNode;
  BddSystem->VAR_NODE   = allocbddvarnode( MaxIndex );
  BddSystem->VAR_MODEL  = ModelVar;

  BddSystem->INDEX_NODE = allocbddindexnode( MaxIndex );
  BddSystem->INDEX_NODE[ BDD_INDEX_ZERO ] = createbddhnodetable( ModelVar );
  BddSystem->INDEX_NODE[ BDD_INDEX_ONE  ] = BddSystem->INDEX_NODE[ BDD_INDEX_ZERO ];

  BddSystem->VAR_TO_INDEX = allocbddindex( MaxVar );
  BddSystem->INDEX_TO_VAR = allocbddvar( MaxIndex );
  BddSystem->INDEX_TO_VAR[ BDD_INDEX_ZERO ] = BDD_MAX_VAR;
  BddSystem->INDEX_TO_VAR[ BDD_INDEX_ONE  ] = BDD_MAX_VAR;

  BddSystem->NUMBER_VAR   = 0;
  BddSystem->NUMBER_INDEX = BDD_INDEX_MIN;

  setbddlocalsystem( BddSystem );

  createbddblock( (bddsystem *)0 );

  BddOne  = addbddnode( (bddsystem *)0, BDD_INDEX_ONE , (bddnode *)0, (bddnode *)0 );
  BddZero = addbddnode( (bddsystem *)0, BDD_INDEX_ZERO, (bddnode *)0, (bddnode *)0 );

  BddOne->REF_EXT  = BDD_MAX_REF;
  BddOne->REF_INT  = BDD_MAX_REF;
  BddZero->REF_EXT = BDD_MAX_REF;
  BddZero->REF_INT = BDD_MAX_REF;

  BddSystem->ONE  = BddOne;
  BddSystem->ZERO = BddZero;

  BddSystem->VAR_NODE[ BDD_INDEX_ZERO ] = BddZero;
  BddSystem->VAR_NODE[ BDD_INDEX_ONE  ] = BddOne;

  return( BddSystem );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Reset System                      |
|                                                             |
\------------------------------------------------------------*/

void resetbddsystem( BddSystem )

  bddsystem *BddSystem;
{
  bddindexnode *IndexNode;
  bddnode      *BddOne;
  bddnode      *BddZero;
  bddindex      NumberIndex;
  bddindex      Index;
  
  setbddlocalsystem( BddSystem );

  resetbddblock( (bddsystem *)0 );
  destroybddassoc( (bddsystem *)0 );

  resetbddhopertable( BddLocalSystem->HASH_OPER );

  NumberIndex = BddLocalSystem->NUMBER_INDEX;
  IndexNode   = BddLocalSystem->INDEX_NODE;

  resetbddhnodetable( IndexNode[ BDD_INDEX_ZERO ] );

  for ( Index = BDD_INDEX_MIN; Index < NumberIndex; Index++ )
  {
    resetbddhnodetable( IndexNode[ Index ] );
  }

  BddLocalSystem->NUMBER_VAR   = 0;
  BddLocalSystem->NUMBER_INDEX = BDD_INDEX_MIN;
  BddLocalSystem->FLAGS        = 0;
  BddLocalSystem->MARK         = 0;

  BddOne  = addbddnode( (bddsystem *)0, BDD_INDEX_ONE , (bddnode *)0, (bddnode *)0 );
  BddZero = addbddnode( (bddsystem *)0, BDD_INDEX_ZERO, (bddnode *)0, (bddnode *)0 );

  BddOne->REF_EXT  = BDD_MAX_REF;
  BddOne->REF_INT  = BDD_MAX_REF;
  BddZero->REF_EXT = BDD_MAX_REF;
  BddZero->REF_INT = BDD_MAX_REF;

  BddLocalSystem->ONE  = BddOne;
  BddLocalSystem->ZERO = BddZero;

  BddLocalSystem->VAR_NODE[ BDD_INDEX_ZERO ] = BddZero;
  BddLocalSystem->VAR_NODE[ BDD_INDEX_ONE  ] = BddOne;

  BddLocalSystem->REORDER_LIMIT = BddLocalSystem->REORDER_LOW;
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Destroy System                    |
|                                                             |
\------------------------------------------------------------*/

void destroybddsystem( BddSystem )

  bddsystem *BddSystem;
{
  bddindexnode *IndexNode;
  bddindex      NumberIndex;
  bddindex      Index;
  
  setbddlocalsystem( BddSystem );

  destroybddblock( (bddsystem *)0 );
  destroybddassoc( (bddsystem *)0 );
  destroybdduserfunc( (bddsystem *)0 );

  destroybddhopertable( BddLocalSystem->HASH_OPER );

  NumberIndex = BddLocalSystem->NUMBER_INDEX;
  IndexNode   = BddLocalSystem->INDEX_NODE;

  destroybddhnodetable( IndexNode[ BDD_INDEX_ZERO ] );

  for ( Index = BDD_INDEX_MIN; Index < NumberIndex; Index++ )
  {
    destroybddhnodetable( IndexNode[ Index ] );
  }

  freebddvar( BddLocalSystem->INDEX_TO_VAR );
  freebddindex( BddLocalSystem->VAR_TO_INDEX );

  freebddindexnode( BddLocalSystem->INDEX_NODE );
  freebddvarnode( BddLocalSystem->VAR_NODE );

  freebddsystem( BddLocalSystem );

  BddLocalSystem = (bddsystem *)0;
}

/*------------------------------------------------------------\
|                                                             |
|                        View Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Bdd View System                        |
|                                                             |
\------------------------------------------------------------*/

void viewbddsystem( BddSystem, ViewIndex )

  bddsystem *BddSystem;
  char       ViewIndex;
{
  bddvar   *IndexToVar;
  bddindex *VarToIndex;
  bddvar    Variable;
  bddindex  Index;

  if ( BddSystem == (bddsystem *)0 )
  {
    BddSystem = BddLocalSystem;
  }

  fprintf( stdout, "--> BddSystem\n" );

  fprintf( stdout, "  NUMBER_NODE     : %ld\n", BddSystem->NUMBER_NODE     );
  fprintf( stdout, "  NUMBER_FREE     : %ld\n", BddSystem->NUMBER_FREE     );
  fprintf( stdout, "  MAX_NODE        : %ld\n", BddSystem->MAX_NODE        );
  fprintf( stdout, "  TOP_NODE        : %ld\n", BddSystem->TOP_NODE        );
  fprintf( stdout, "  NUMBER_VAR      : %ld\n", BddSystem->NUMBER_VAR      );
  fprintf( stdout, "  MAX_VAR         : %ld\n", BddSystem->MAX_VAR         );
  fprintf( stdout, "  NUMBER_INDEX    : %ld\n", BddSystem->NUMBER_INDEX    );
  fprintf( stdout, "  MAX_INDEX       : %ld\n", BddSystem->MAX_INDEX       );
  fprintf( stdout, "  ASSOC_RESET     : %d\n" , BddSystem->ASSOC_RESET     );
  fprintf( stdout, "  FLAGS           : %lx\n", BddSystem->FLAGS           );
  fprintf( stdout, "  REORDER_LOW     : %ld\n", BddSystem->REORDER_LOW     );
  fprintf( stdout, "  REORDER_LIMIT   : %ld\n", BddSystem->REORDER_LIMIT   );
  fprintf( stdout, "  REORDER_RATIO   : %ld\n", BddSystem->REORDER_RATIO   );
  fprintf( stdout, "  EXPLOSION_LIMIT : %ld\n", BddSystem->EXPLOSION_LIMIT );
  fprintf( stdout, "  VAR_MODEL       : %ld\n", BddSystem->VAR_MODEL       );
  fprintf( stdout, "  OPER_MODEL      : %ld\n", BddSystem->OPER_MODEL      );

  if ( ViewIndex )
  {
    IndexToVar = BddSystem->INDEX_TO_VAR;
    VarToIndex = BddSystem->VAR_TO_INDEX;

    for ( Index = 0; Index < BddSystem->NUMBER_INDEX; Index++ )
    {
      fprintf( stdout, "  INDEX_TO_VAR[ %u ] = %d\n",
               Index, IndexToVar[ Index ] );
    }

    for ( Variable = 0; Variable < BddSystem->NUMBER_VAR; Variable++ )
    {
      fprintf( stdout, "  VAR_TO_INDEX[ %u ] = %d\n",
               Variable, VarToIndex[ Variable ] );
    }
  }

  fprintf( stdout, "<-- BddSystem\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                      Bdd View System Info                   |
|                                                             |
\------------------------------------------------------------*/

void viewbddsysteminfo( BddSystem )

  bddsystem *BddSystem;
{
  bddhopertable *HashOper;
  bddindexnode  *IndexNode;
  bddhnodetable *HashTable;
  long           NumberIndex;
  long           Index;
  long           Sum;
  long           Slot;
  long           Fill;
  long           Hit;
  long           Miss;
  long           Stretch;
  long           Resize;
  long           Scan;

  if ( BddSystem == (bddsystem *)0 )
  {
    BddSystem = BddLocalSystem;
  }

  HashOper  = BddSystem->HASH_OPER;
  IndexNode = BddSystem->INDEX_NODE;

  fprintf( stdout, "--> BddSystem Info\n" );

  Slot = HashOper->TABLE_SIZE;
  Fill = ( HashOper->NUMBER_OPER * 100 ) / Slot;
  Sum  = HashOper->NUMBER_HIT + HashOper->NUMBER_MISS;

  if ( Sum != 0 )
  {
    Hit  = ( 100 * HashOper->NUMBER_HIT  ) / Sum;
    Miss = ( 100 * HashOper->NUMBER_MISS ) / Sum;
  }
  else
  {
    Hit = Miss = -1;
  }

  fprintf( stdout, "  HASH_OPER SLOTS     : %ld   \n", Slot );
  fprintf( stdout, "  HASH_OPER FILL RATE : %ld %%\n", Fill );
  fprintf( stdout, "  HASH_OPER HIT RATE  : %ld %%\n", Hit  );
  fprintf( stdout, "  HASH_OPER MISS RATE : %ld %%\n", Miss );

  Slot        = 0;
  Fill        = 0;
  Scan        = 0;
  Stretch     = 0;
  Resize      = 0;

  NumberIndex = BddLocalSystem->NUMBER_INDEX;

  for ( Index = BDD_INDEX_MIN; Index < NumberIndex; Index++ )
  {
    HashTable = IndexNode[ Index ];

    Slot    += HashTable->TABLE_SIZE;
    Fill    += ( HashTable->NUMBER_NODE * 100 ) / HashTable->TABLE_SIZE;
    Scan    += HashTable->NUMBER_SCAN;
    Stretch += HashTable->NUMBER_STRETCH;
    Resize  += HashTable->NUMBER_RESIZE;
  }

  NumberIndex -= BDD_INDEX_MIN;

  Slot    /= NumberIndex;
  Fill    /= NumberIndex;
  Scan    /= NumberIndex;
  Stretch /= NumberIndex;
  Resize  /= NumberIndex;

  fprintf( stdout, "  HASH_NODE SLOTS AVERAGE   : %ld   \n", Slot    );
  fprintf( stdout, "  HASH_NODE FILL RATE       : %ld %%\n", Fill    );
  fprintf( stdout, "  HASH_NODE SCAN AVERAGE    : %ld   \n", Scan    );
  fprintf( stdout, "  HASH_OPER STRETCH AVERAGE : %ld   \n", Stretch );
  fprintf( stdout, "  HASH_OPER RESIZE AVERAGE  : %ld   \n", Resize  );

  fprintf( stdout, "<-- BddSystem Info\n" );
}
