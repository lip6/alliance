/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU General Public |
| License as  published by the Free Software Foundation;      |
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
| Tool    :                     BOOM                          |
|                                                             |
| File    :                  boom_burgun.c                    |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   08.08.00                        |
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
# include ABE_H
# include ABV_H
# include ABT_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "boom_error.h"
# include "boom_debug.h"
# include "boom_shared.h"
# include "boom_aux.h"
# include "boom_abl.h"
# include "boom_bdd.h"
# include "boom_cost.h"
# include "boom_optim.h"

# include "boom_burgun.h"

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

  static bddnode *BoomBddTopVarNode = (bddnode *)0;
  static long     BoomCurrentVar    = (long     )0;
  static long     BoomBurgunVar       = (long     )0;
  static long     BoomCurrentCost   = 0;

  static int      BoomExplosionFlag = 0;

  static char    *BoomIndexArray     = (char *)0;
  static long     BoomIndexArraySize = 0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       BoomBurgunTopBurgunVar                |
|                                                             |
\------------------------------------------------------------*/

static bddnode *BoomBurgunTopBurgunVar( BddNode )

  bddnode *BddNode;
{
  bddnode *NewBddNode;

  NewBddNode = composebddnode( BddLocalSystem, BddNode,
                               BoomBurgunVar   , BoomBddTopVarNode );

  return( NewBddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                       BoomBurgunTopBddNode                  |
|                                                             |
\------------------------------------------------------------*/

static void BoomBurgunTopBddNode( BddNode )

  bddnode *BddNode;
{
  bddnode *NewBddNode;
  long     Number;

  if ( ! BoomExplosionFlag )
  {
    Number = getbddnodenum( (bddsystem *)0, BddNode );
    
    if ( Number < BOOM_BDD_NODE_BIG_SIZE ) return;

    NewBddNode = composebddnode( BddLocalSystem, BddNode,
                                 BoomCurrentVar, BoomBddTopVarNode );

    if ( IsBddSystemExplosion( BddLocalSystem ) )
    {
      BoomPrintf( stdout, "Explosion !!! %ld\n", BddLocalSystem->NUMBER_NODE );

      garbagebddsystem( BddLocalSystem );
      explosionbddsystemrestart( BddLocalSystem );
      
      BoomExplosionFlag = 1;
    }
    else
    {
      BoomCurrentCost += getbddnodenum( (bddsystem *)0, NewBddNode );
      /*
      Number = getbddnodenum( (bddsystem *)0, NewBddNode );

      if ( Number >= BOOM_BDD_NODE_BIG_SIZE ) BoomCurrentCost += Number;
      */

      decbddrefext( NewBddNode );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomBehBurgunBddOrder                  |
|                                                             |
\------------------------------------------------------------*/

chain_list *BoomBehBurgunBddOrder( BehFigure )

  befig_list *BehFigure;
{
  boombefiginfo *FigInfo;
  chain_list    *OrderList;
  long           NumberIndex;
  long           NumberVar;
  long           VarIndex;
  long           TopVar;
  long           FirstVar;
  long           LastVar;
  long           ExplosionLimit;
  long           MaxNode;
  long           BurgunCost;
  char          *Name;
  char           Buffer[ 32 ];

  FigInfo = GetBoomBehFigInfo( BehFigure );

  NumberIndex = BddLocalSystem->NUMBER_INDEX;
  NumberVar   = BddLocalSystem->NUMBER_VAR;

  if ( BoomIndexArray == (char *)0 )
  {
    BoomIndexArray     = (char *)autallocblock( sizeof( char ) * NumberIndex );
    BoomIndexArraySize = NumberIndex;
  }
  else
  if ( NumberIndex > BoomIndexArraySize )
  {
    BoomIndexArray = (char *)autresizeblock( (char *)BoomIndexArray,
                                               sizeof( long ) * BoomIndexArraySize,
                                               sizeof( long ) * NumberIndex );
    BoomIndexArraySize = NumberIndex;
  }

  memset( BoomIndexArray, 0, BoomIndexArraySize );

  OrderList = (chain_list *)0;

  for ( TopVar = 0; TopVar < NumberVar; TopVar++ )
  {
    VarIndex          = getbddvarindex( BddLocalSystem, TopVar );
    sprintf( Buffer, "_top_%ld", TopVar );
    BoomBddTopVarNode = addbddcircuitin( BddLocalCircuit, Buffer,
                                         VarIndex, BDD_IN_MODE_BEFORE );

    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "TopVar %ld %s\n", TopVar, Buffer );
    }

    MaxNode  = BddLocalSystem->NUMBER_NODE * 2;
    FirstVar = TopVar + 1;
    LastVar  = TopVar + NumberVar;

    BoomBurgunVar = -1;
    BurgunCost    = 0;
  
    for ( BoomCurrentVar = FirstVar; BoomCurrentVar <= LastVar; BoomCurrentVar++ )
    {
      VarIndex = getbddvarindex( (bddsystem *)0, BoomCurrentVar );

      if ( BoomIndexArray[ VarIndex ] != 0 ) continue;

      if ( BddLocalSystem->NUMBER_NODE >= MaxNode )
      {
        if ( IsBoomDebugLevel0() )
        {
          BoomPrintf( stdout, "Sweep !\n" );
        }
  
        sweepbddvar( BddLocalSystem, TopVar, 1 );
  
        if ( BddLocalSystem->NUMBER_NODE >= MaxNode )
        {
          if ( IsBoomDebugLevel0() )
          {
            BoomPrintf( stdout, "Garbage !\n" );
          }
  
          garbagebddsystem( BddLocalSystem );
          MaxNode = BddLocalSystem->NUMBER_NODE * 4;
        }
      }
  
      ExplosionLimit = BddLocalSystem->NUMBER_NODE * 2;
  
      if ( ExplosionLimit < BOOM_BURGUN_EXPLOSION_LIMIT )
      {
        ExplosionLimit = BOOM_BURGUN_EXPLOSION_LIMIT;
      }

      BoomExplosionFlag = 0;
  
      explosionbddsystemdynamic( (bddsystem *)0, NULL, ExplosionLimit );
  
      BoomCurrentCost = 0;
  
      BoomBehScanBdd( BehFigure, BoomBurgunTopBddNode );

      if ( ! BoomExplosionFlag )
      {
        if ( ( BoomBurgunVar == -1          ) ||
             ( BoomCurrentCost < BurgunCost ) )
        {
          BoomBurgunVar = BoomCurrentVar;
          BurgunCost    = BoomCurrentCost;
        }
      }
      else
      {
        BoomExplosionFlag = 0;
      }
  
      if ( IsBoomDebugLevel1() )
      {
        Name = getbddcircuitinname( (bddcircuit *)0, VarIndex );
        BoomPrintf( stdout, "%ld %ld %s\n", BoomCurrentVar, BoomCurrentCost, Name );
      }
    }

    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "BurgunCost %ld BoomBurgunVar %ld\n", BurgunCost, BoomBurgunVar );
    }

    explosionbddsystemdynamic( (bddsystem *)0, NULL, 0 );

    VarIndex = getbddvarindex( (bddsystem *)0, BoomBurgunVar );
    Name     = getbddcircuitinname( (bddcircuit *)0, VarIndex );

    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "Var %s\n", Name );
    }

    OrderList = addchain( OrderList, (void *)Name );

    BoomIndexArray[ VarIndex ] = 1;

    BoomBehMapBdd( BehFigure, BoomBurgunTopBurgunVar );
  }

  OrderList = reverse( OrderList );

  BoomBehRebuildBddNode( BehFigure, OrderList );

  return( OrderList );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomBehOptimizeBurgun                  |
|                                                             |
\------------------------------------------------------------*/

void BoomBehOptimizeBurgun( BehFigure )

  befig_list *BehFigure;
{
  beaux_list    *BehAux;
  boombefiginfo *FigInfo;
  long           Cost;
  long           NewCost;
  long           Iteration;
  long           Amplitude;
  long           MaxIteration;
  long           MaxAmplitude;
  chain_list    *BurgunOrder;
  chain_list    *NewOrder;
  int            Level;       

  FigInfo = GetBoomBehFigInfo( BehFigure );
  MaxIteration = FigInfo->ITERATION;
  MaxAmplitude = FigInfo->AMPLITUDE;
  Level        = FigInfo->OPTIM_LEVEL;

  if ( MaxIteration <= 0 ) MaxIteration = 2;
  if ( MaxAmplitude <= 0 ) MaxAmplitude = 5;

  MaxIteration = MaxIteration * (Level + 1);
  MaxAmplitude = MaxAmplitude + Level;

  BehAux      = BoomBehComputeAux( BehFigure, BehFigure->BEAUX );
  Cost        = BoomBehGiveCost( BehFigure );
  BurgunOrder = BoomBehGetBddOrder( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Initial cost %ld\n", Cost );
  }

  if ( ( FigInfo->VERBOSE ) &&
       ( ! BoomDebugFlag  ) )
  {
    fprintf( stdout, "\t--> Optimization %% 0\b" );
    fflush( stdout );
  }

  for ( Iteration = 0; Iteration < MaxIteration; Iteration++ )
  {
    if ( IsBoomDebugLevel0() )
    {
      BoomPrintf( stdout, "Iteration %ld\n", Iteration );
    }
  
    Amplitude = MaxAmplitude + 1;
    
    do
    {
      if ( IsBoomDebugLevel0() )
      {
        BoomPrintf( stdout, "Amplitude %ld\n", Amplitude );
      }

      if ( Amplitude <= MaxAmplitude )
      {
        BoomBehRandomBddOrder( BehFigure, Amplitude );
      }

      NewOrder = BoomBehBurgunBddOrder( BehFigure );
  
      BehAux  = BoomBehComputeAux( BehFigure, BehAux );
      NewCost = BoomBehGiveCost( BehFigure );
  
      if ( IsBoomDebugLevel0() )
      {
        BoomPrintf( stdout, "NewCost: %ld\n", NewCost );
      }

      if ( NewCost < Cost )
      {
        Cost = NewCost;

        freechain( BurgunOrder );
        BurgunOrder = NewOrder;
      }
      else
      {
        freechain( NewOrder );
      }

      Amplitude--;
    }
    while ( Amplitude >= 0 );

    if ( ( FigInfo->VERBOSE ) &&
         ( ! BoomDebugFlag  ) )
    {
      fprintf( stdout, "%-3ld\b\b\b", ( ( Iteration + 1 ) * 100 ) / MaxIteration );
      fflush( stdout );
    }
  }

  if ( FigInfo->VERBOSE )
  {
    fprintf( stdout, "\n\n" );
  }

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Burgun cost : %ld\n", Cost );
  }

  BoomBehRebuildBddNode( BehFigure, BurgunOrder );

  BehAux = BoomBehComputeAux( BehFigure, BehAux );
  Cost   = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Final cost %ld\n", Cost );
  }
}
