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
| File    :                  boom_gradient.c                  |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   06.06.00                        |
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

# include "boom_gradient.h"

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
  static long     BoomCurrentCost   = 0;

  static int      BoomExplosionFlag = 0;


  static long    *BoomVarCostArray = (long *)0;
  static long    *BoomVarSortArray = (long *)0;
  static long     BoomVarArraySize = 0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       BoomGradientTopBddNode                |
|                                                             |
\------------------------------------------------------------*/

static void BoomGradientTopBddNode( BddNode )

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
|                      BoomGradientCompareCost                |
|                                                             |
\------------------------------------------------------------*/

static long BoomGradientCompareCost( CostArray, Index1, Index2 )

  long *CostArray;
  long  Index1;
  long  Index2;
{
  return( CostArray[ Index2 ] - CostArray[ Index1 ] );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomBehGradientBddOrder                |
|                                                             |
\------------------------------------------------------------*/

chain_list *BoomBehGradientBddOrder( BehFigure )

  befig_list *BehFigure;
{
  boombefiginfo *FigInfo;
  chain_list    *OrderList;
  long           NumberVar;
  long           VarIndex;
  long           Index;
  long           ExplosionLimit;
  long           MaxNode;
  char          *Name;

  FigInfo = GetBoomBehFigInfo( BehFigure );

  VarIndex          = getbddvarindex( BddLocalSystem, 0 );
  BoomBddTopVarNode = addbddcircuitin( BddLocalCircuit, "_top_", VarIndex, 
                                       BDD_IN_MODE_BEFORE );

  NumberVar    = BddLocalSystem->NUMBER_VAR;

  if ( BoomVarCostArray == (long *)0 )
  {
    BoomVarCostArray = (long *)autallocblock( sizeof( long ) * NumberVar );
    BoomVarSortArray = (long *)autallocblock( sizeof( long ) * NumberVar );

    BoomVarArraySize = NumberVar;
  }
  else
  if ( NumberVar > BoomVarArraySize )
  {
    BoomVarCostArray = (long *)autresizeblock( (char *)BoomVarCostArray,
                                               sizeof( long ) * BoomVarArraySize,
                                               sizeof( long ) * NumberVar );
    BoomVarSortArray = (long *)autresizeblock( (char *)BoomVarSortArray,
                                               sizeof( long ) * BoomVarArraySize,
                                               sizeof( long ) * NumberVar );
    BoomVarArraySize = NumberVar;
  }

  if ( IsBoomDebugLevel1() )
  {
    BoomPrintf( stdout, "Compute Cost for each variables\n" );
  }

  MaxNode = BddLocalSystem->NUMBER_NODE * 2;

  for ( BoomCurrentVar = 1; BoomCurrentVar < NumberVar; BoomCurrentVar++ )
  {
    if ( BddLocalSystem->NUMBER_NODE >= MaxNode )
    {
      if ( IsBoomDebugLevel0() )
      {
        BoomPrintf( stdout, "Sweep !\n" );
      }

      sweepbddvar( BddLocalSystem, 0, 1 );

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

    if ( ExplosionLimit < BOOM_GRADIENT_EXPLOSION_LIMIT )
    {
      ExplosionLimit = BOOM_GRADIENT_EXPLOSION_LIMIT;
    }

    explosionbddsystemdynamic( (bddsystem *)0, NULL, ExplosionLimit );

    BoomCurrentCost = 0;

    BoomBehScanBdd( BehFigure, BoomGradientTopBddNode );

    if ( BoomExplosionFlag )
    {
      BoomExplosionFlag = 0;
      BoomVarCostArray[ BoomCurrentVar - 1 ] = - BoomCurrentVar;
    }
    else
    {
      BoomVarCostArray[ BoomCurrentVar - 1 ] = BoomCurrentCost;
    }

    if ( IsBoomDebugLevel1() )
    {
      VarIndex = getbddvarindex( (bddsystem *)0, BoomCurrentVar );
      Name = getbddcircuitinname( (bddcircuit *)0, VarIndex );

      BoomPrintf( stdout, "%ld %ld %s\n", BoomCurrentVar, BoomCurrentCost, Name );
    }
  }

  explosionbddsystemdynamic( (bddsystem *)0, NULL, 0 );

  sortautarray( BoomVarCostArray, BoomVarSortArray, NumberVar - 1, BoomGradientCompareCost );

  OrderList = (chain_list *)0;

  if ( IsBoomDebugLevel1() )
  {
    BoomPrintf( stdout, "New variable order (reverse)\n" );
  }
  
  for ( Index = NumberVar - 2; Index >= 0; Index-- )
  {
    VarIndex = getbddvarindex( (bddsystem *)0, BoomVarSortArray[ Index ] + 1 );
    Name = getbddcircuitinname( (bddcircuit *)0, VarIndex );

    if ( IsBoomDebugLevel0() )
    {
      BoomPrintf( stdout, "%ld %ld %s\n", BoomVarSortArray[ Index ] + 1, VarIndex, Name );
    }

    OrderList = addchain( OrderList, (void *)Name );
  }

  BoomBehRebuildBddNode( BehFigure, OrderList );

  return( OrderList );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomBehOptimizeGradient                |
|                                                             |
\------------------------------------------------------------*/

void BoomBehOptimizeGradient( BehFigure )

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
  chain_list    *BestOrder;
  chain_list    *NewOrder;
  int            Level;       

  FigInfo = GetBoomBehFigInfo( BehFigure );
  MaxIteration = FigInfo->ITERATION;
  MaxAmplitude = FigInfo->AMPLITUDE;
  Level        = FigInfo->OPTIM_LEVEL;

  if ( MaxIteration <= 0 ) MaxIteration = 2;
  if ( MaxAmplitude <= 0 ) MaxAmplitude = 5;

  BehAux     = BoomBehComputeAux( BehFigure, BehFigure->BEAUX );
  Cost       = BoomBehGiveCost( BehFigure );
  BestOrder  = BoomBehGetBddOrder( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Initial cost %ld\n", Cost );
  }

  MaxIteration = MaxIteration * (Level + 1);

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
  
    Amplitude = MaxAmplitude;
    
    do
    {
      if ( IsBoomDebugLevel0() )
      {
        BoomPrintf( stdout, "Amplitude %ld\n", Amplitude );
      }

      BoomBehRandomBddOrder( BehFigure, Amplitude );

      NewOrder = BoomBehGradientBddOrder( BehFigure );
  
      BehAux  = BoomBehComputeAux( BehFigure, BehAux );
      NewCost = BoomBehGiveCost( BehFigure );
  
      if ( IsBoomDebugLevel0() )
      {
        BoomPrintf( stdout, "NewCost: %ld\n", NewCost );
      }

      if ( NewCost < Cost )
      {
        Cost = NewCost;

        freechain( BestOrder );
        BestOrder = NewOrder;
      }
      else
      {
        freechain( NewOrder );
      }
      
      Amplitude--;
    }
    while ( Amplitude > 0 );

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
    BoomPrintf( stdout, "Best cost : %ld\n", Cost );
  }

  BoomBehRebuildBddNode( BehFigure, BestOrder );

  BehAux = BoomBehComputeAux( BehFigure, BehAux );
  Cost   = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Final cost %ld\n", Cost );
  }
}
