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
| File    :                  boom_logic.c                     |
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
# include "boom_logic.h"

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
|                      BoomLogicComputeAux                    |
|                                                             |
\------------------------------------------------------------*/

beaux_list *BoomLogicComputeAux( BehFigure, BehAux )

  befig_list *BehFigure;
  beaux_list *BehAux;
{
  beaux_list  *ScanAux;
  beaux_list **PrevAux;

  PrevAux = &BehFigure->BEAUX;

  for ( ScanAux  = BehFigure->BEAUX;
        ScanAux != (beaux_list *)0;
        ScanAux  = ScanAux->NEXT )
  {
    if ( ScanAux == BehAux )
    {
      *PrevAux = (beaux_list *)0; break;
    }

    PrevAux = &ScanAux->NEXT;
  }

  BoomBehFreeBehAux( BehFigure->BEAUX );

  BehFigure->BEAUX = ScanAux;
  BehAux = ScanAux;

  BoomBehBddNode2Abl( BehFigure );

  if ( IsBoomDebugLevel2() )
  {
    BoomDebugSaveBehFigure( BehFigure, NULL );
  }

  BoomBehCreateAux( BehFigure );

  if ( IsBoomDebugLevel2() )
  {
    BoomDebugSaveBehFigure( BehFigure, NULL );
  }

  return( BehAux );
}

/*------------------------------------------------------------\
|                                                             |
|                 Boom Logic Reorder Window 2                 |
|                                                             |
\------------------------------------------------------------*/

int BoomLogicReorderWindow2( BehFigure, BehAux, BddVar, PCost )

  befig_list *BehFigure;
  beaux_list *BehAux;
  bddvar      BddVar;
  long       *PCost;
{
  long NewCost;

  swapbddvar( (bddsystem *)0, BddVar );

  BehAux  = BoomLogicComputeAux( BehFigure, BehAux );
  NewCost = BoomBehGiveCost( BehFigure );

  if ( NewCost < *PCost )
  {
    *PCost = NewCost;

    return( 1 );
  }
  else
  if ( NewCost == *PCost )
  {
    return( 0 );
  }

  swapbddvar( BddLocalSystem, BddVar );

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                 Boom Logic Reorder Window 3                 |
|                                                             |
\------------------------------------------------------------*/

int BoomLogicReorderWindow3( BehFigure, BehAux, BddVar, PCost )

  befig_list *BehFigure;
  beaux_list *BehAux;
  bddvar      BddVar;
  long       *PCost;
{
  long Cost;
  long NewCost;
  int  BestOrder;

  BestOrder = BOOM_LOGIC_REORDER_123;
  Cost      = *PCost;

  if ( IsBoomDebugLevel1() )
  {
     BoomPrintf( stdout, "Window3: Initial cost %ld\n", Cost );
  }

  swapbddvar( (bddsystem *)0, BddVar );

  BehAux  = BoomLogicComputeAux( BehFigure, BehAux );
  NewCost = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "NewCost: %ld\n", NewCost );
  }

  if ( NewCost < Cost )
  {
    Cost      = NewCost;
    BestOrder = BOOM_LOGIC_REORDER_213;

    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "Window3: 213 new cost %ld\n", Cost );
    }
  }

  swapbddvar( (bddsystem *)0, BddVar );

  BehAux  = BoomLogicComputeAux( BehFigure, BehAux );
  NewCost = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "NewCost: %ld\n", NewCost );
  }

  if ( NewCost < Cost )
  {
    Cost      = NewCost;
    BestOrder = BOOM_LOGIC_REORDER_231;

    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "Window3: 231 new cost %ld\n", Cost );
    }
  }

  swapbddvar( (bddsystem *)0, BddVar );

  BehAux  = BoomLogicComputeAux( BehFigure, BehAux );
  NewCost = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "NewCost: %ld\n", NewCost );
  }

  if ( NewCost < Cost )
  {
    Cost      = NewCost;
    BestOrder = BOOM_LOGIC_REORDER_321;

    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "Window3: 321 new cost %ld\n", Cost );
    }
  }

  swapbddvar( (bddsystem *)0, BddVar );

  BehAux  = BoomLogicComputeAux( BehFigure, BehAux );
  NewCost = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "NewCost: %ld\n", NewCost );
  }

  if ( NewCost < Cost )
  {
    Cost      = NewCost;
    BestOrder = BOOM_LOGIC_REORDER_312;

    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "Window3: 312 new cost %ld\n", Cost );
    }
  }

  swapbddvar( (bddsystem *)0, BddVar );

  BehAux  = BoomLogicComputeAux( BehFigure, BehAux );
  NewCost = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "NewCost: %ld\n", NewCost );
  }

  if ( NewCost < Cost )
  {
    Cost      = NewCost;
    BestOrder = BOOM_LOGIC_REORDER_132;

    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "Window3: 132 new cost %ld\n", Cost );
    }
  }

  switch ( BestOrder )
  {
    case BOOM_LOGIC_REORDER_123 : swapbddvar( (bddsystem *)0, BddVar );
    break;

    case BOOM_LOGIC_REORDER_213 : swapbddvar( (bddsystem *)0, BddVar );
                                  swapbddvar( (bddsystem *)0, BddVar );
    break;

    case BOOM_LOGIC_REORDER_231 : swapbddvar( (bddsystem *)0, BddVar );
                                  swapbddvar( (bddsystem *)0, BddVar );
                                  swapbddvar( (bddsystem *)0, BddVar );
    break;

    case BOOM_LOGIC_REORDER_321 : swapbddvar( (bddsystem *)0, BddVar );
                                  swapbddvar( (bddsystem *)0, BddVar );
    break;

    case BOOM_LOGIC_REORDER_312 : swapbddvar( (bddsystem *)0, BddVar );
    break;
  }

  *PCost = Cost;

  return( BestOrder != BOOM_LOGIC_REORDER_123 );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomLogicOptimizeWindow                |
|                                                             |
\------------------------------------------------------------*/

void BoomLogicOptimizeWindow( BehFigure )

  befig_list *BehFigure;
{
  beaux_list *BehAux;
  short      *ReorderLevel;
  bddvar      NumberChild;
  bddvar      BestChild;
  bddvar      ChildCount;
  bddvar      ChildTo;
  bddvar      ChildFrom;
  int         SwapReorder;
  int         AnySwap;
  int         Moved;
  long        Cost;
  long        NewCost;

  NumberChild  = BddLocalSystem->NUMBER_VAR;
  ReorderLevel = (short *)autallocblock( sizeof( short ) * NumberChild );

  for ( ChildFrom = 0; ChildFrom < ( NumberChild - 1 ); ChildFrom++ )
  {
    ReorderLevel[ ChildFrom ] = 1;
  }

  BehAux = BoomLogicComputeAux( BehFigure, BehFigure->BEAUX );
  Cost   = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel1() )
  {
    BoomPrintf( stdout, "Initial cost %ld\n", Cost );
  }

  do
  {
    AnySwap = 0;

    for ( ChildCount = 0; ChildCount < NumberChild - 1; ChildCount++ )
    {
      if ( ReorderLevel[ ChildCount ] )
      {
        if ( ChildCount < NumberChild - 2 )
        {
          Moved = BoomLogicReorderWindow3( BehFigure, BehAux, ChildCount, &Cost );
        }
        else
        {
          Moved = BoomLogicReorderWindow2( BehFigure, BehAux, ChildCount, &Cost );
        }

        if ( Moved )
        {
          AnySwap = 1;

          if ( ChildCount > 0 ) ReorderLevel[ ChildCount - 1 ] = 1;
          if ( ChildCount > 1 ) ReorderLevel[ ChildCount - 2 ] = 1;

          ReorderLevel[ ChildCount + 1 ] = 1;

          if ( ChildCount < NumberChild - 2 ) ReorderLevel[ ChildCount + 2 ] = 1;
          if ( ChildCount < NumberChild - 3 ) ReorderLevel[ ChildCount + 3 ] = 1;
          if ( ChildCount < NumberChild - 4 ) ReorderLevel[ ChildCount + 4 ] = 1;
        }
        else
        {
          ReorderLevel[ ChildCount ] = 0;
        }
      }
    }
  }
  while ( AnySwap );

  autfreeblock( ReorderLevel );

  BehAux = BoomLogicComputeAux( BehFigure, BehAux );
  Cost   = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel1() )
  {
    BoomPrintf( stdout, "Final cost %ld\n", Cost );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomLogicOptimizeTop                   |
|                                                             |
\------------------------------------------------------------*/

void BoomLogicOptimizeTop( BehFigure )

  befig_list *BehFigure;
{
  beaux_list *BehAux;
  char       *ReorderLevel;
  bddvar      NumberChild;
  bddvar      BestChild;
  bddvar      ChildCount;
  bddvar      ChildTo;
  bddvar      ChildFrom;
  char        SwapReorder;
  long        Cost;
  long        NewCost;

  NumberChild  = BddLocalSystem->NUMBER_VAR;
  ReorderLevel = (char *)autallocblock( sizeof( char ) * NumberChild );

  for ( ChildFrom = 0; ChildFrom < ( NumberChild - 1 ); ChildFrom++ )
  {
    ReorderLevel[ ChildFrom ] = 1;
  }

  BehAux = BoomLogicComputeAux( BehFigure, BehFigure->BEAUX );
  Cost   = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Initial cost %ld\n", Cost );
  }

  ChildCount = ( NumberChild - 1 );
  ChildFrom  = 0;

  while ( ChildCount > 0 )
  {
    /*
    fprintf( stdout, "%-4d\b\b\b\b", ChildCount );
    fflush( stdout );
    */

    if ( IsBoomDebugLevel2() )
    {
      BoomPrintf( stdout, "-> ChildFrom %d\n", ChildFrom );
    }

    ReorderLevel[ ChildFrom ] = 0;

    BestChild  = ChildFrom;

    for ( ChildTo = ChildFrom; ChildTo < ( NumberChild - 1 ); ChildTo++ )
    {
      if ( IsBoomDebugLevel2() )
      {
        BoomPrintf( stdout, "-> Swap %d and %d\n", ChildTo, ChildTo + 1 );
      }

      swapbddvar( (bddsystem *)0, ChildTo );

      SwapReorder                 = ReorderLevel[ ChildTo     ];
      ReorderLevel[ ChildTo     ] = ReorderLevel[ ChildTo + 1 ];
      ReorderLevel[ ChildTo + 1 ] = SwapReorder;

      BehAux  = BoomLogicComputeAux( BehFigure, BehAux );
      NewCost = BoomBehGiveCost( BehFigure );

      if ( IsBoomDebugLevel0() )
      {
        BoomPrintf( stdout, "NewCost: %ld\n", NewCost );
      }

      if ( NewCost < Cost )
      {
        BestChild = ChildTo + 1; Cost = NewCost;
      }
    }

    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "--> Best child %d\n", BestChild );
      BoomPrintf( stdout, "--> Best cost %ld\n", Cost      );
    }

    for ( ChildTo = ( NumberChild - 1 ); ChildTo > BestChild; ChildTo-- )
    {
# ifdef BDD_DEBUG
  if ( IsBoomDebugLevel1() )
  {
fprintf( stdout, "-> UnSwap %d and %d\n", ChildTo - 1, ChildTo );
  }
# endif

      SwapReorder                 = ReorderLevel[ ChildTo - 1 ];
      ReorderLevel[ ChildTo - 1 ] = ReorderLevel[ ChildTo     ];
      ReorderLevel[ ChildTo     ] = SwapReorder;

      swapbddvar( (bddsystem *)0, ChildTo - 1 );
    }

    while ( ! ReorderLevel[ ChildFrom ] )
    {
      ChildFrom = ChildFrom + 1;
    }

    ChildCount = ChildCount - 1;
  }

  autfreeblock( ReorderLevel );

  BehAux = BoomLogicComputeAux( BehFigure, BehAux );
  Cost   = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Final cost %ld\n", Cost );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomLogicOptimizeProcrast              |
|                                                             |
\------------------------------------------------------------*/

void BoomLogicOptimizeProcrast( BehFigure )

  befig_list *BehFigure;
{
  beaux_list    *BehAux;
  boombefiginfo *FigInfo;
  bddvar         NumberChild;
  bddvar         ChildTo;
  bddvar         ChildFrom;
  char           SwapReorder;
  long           Cost;
  long           NumberNode;
  long           NewCost;
  long           Iteration;
  int            MaxIteration;
  long           MaxNode;

  FigInfo = GetBoomBehFigInfo( BehFigure );
  MaxIteration = FigInfo->ITERATION;

  if ( MaxIteration <= 0 ) MaxIteration = 2;

  NumberChild = BddLocalSystem->NUMBER_VAR;

  BehAux = BoomLogicComputeAux( BehFigure, BehFigure->BEAUX );
  Cost   = BoomBehGiveCost( BehFigure );

  MaxNode = ( BddLocalSystem->NUMBER_NODE * 120 ) / 100;

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Initial cost %ld\n", Cost );
  }

  for ( Iteration = 0; Iteration < MaxIteration; Iteration++ )
  {
    ChildFrom = 0;
  
    for ( ChildFrom = 0; ChildFrom < (NumberChild - 1); ChildFrom++ )
    {
      if ( IsBoomDebugLevel1() )
      {
        BoomPrintf( stdout, "-> ChildFrom %d\n", ChildFrom );
      }
  
      for ( ChildTo = ChildFrom; ChildTo < ( NumberChild - 1 ); ChildTo++ )
      {
        if ( IsBoomDebugLevel1() )
        {
          BoomPrintf( stdout, "-> Swap %d and %d\n", ChildTo, ChildTo + 1 );
        }
  
        swapbddvar( (bddsystem *)0, ChildTo );

        if ( BddLocalSystem->NUMBER_NODE >= MaxNode )
        {
          garbagebddsystem( BddLocalSystem );
        }

        if ( BddLocalSystem->NUMBER_NODE >= MaxNode )
        {
          BoomPrintf( stdout, "@@@@ Max nodes %ld reached %ld\n",
                      MaxNode, BddLocalSystem->NUMBER_NODE );
          break;
        }
      }

      NumberNode = BoomBehGetNumberBddNode( BehFigure );
  
      BehAux  = BoomLogicComputeAux( BehFigure, BehAux );
      NewCost = BoomBehGiveCost( BehFigure );
  
      if ( IsBoomDebugLevel0() )
      {
        BoomPrintf( stdout, "NewCost: %ld %ld\n", NewCost, NumberNode );
      }

      if ( NewCost > Cost )
      {
        while ( ChildTo > ChildFrom )
        {
          if ( IsBoomDebugLevel1() )
          {
            BoomPrintf( stdout, "-> UnSwap %d and %d\n", ChildTo, ChildTo - 1 );
          }

          swapbddvar( (bddsystem *)0, ChildTo - 1 );

          ChildTo = ChildTo - 1;
        }
      }
      else
      {
        Cost = NewCost;
      }
    }
  }

  BehAux = BoomLogicComputeAux( BehFigure, BehAux );
  Cost   = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Final cost %ld\n", Cost );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomLogicOptimizeBest                  |
|                                                             |
\------------------------------------------------------------*/

void BoomLogicOptimizeBest( BehFigure )

  befig_list *BehFigure;
{
  beaux_list    *BehAux;
  boombefiginfo *FigInfo;
  bddvar         NumberChild;
  bddvar         ChildTo;
  bddvar         ChildFrom;
  char           SwapReorder;
  long           Cost;
  long           NewCost;
  long           Iteration;
  int            MaxIteration;
  long           MaxNode;

  FigInfo = GetBoomBehFigInfo( BehFigure );
  MaxIteration = FigInfo->ITERATION;

  if ( MaxIteration <= 0 ) MaxIteration = 2;

  NumberChild = BddLocalSystem->NUMBER_VAR;

  BehAux = BoomLogicComputeAux( BehFigure, BehFigure->BEAUX );
  Cost   = BoomBehGiveCost( BehFigure );

  MaxNode = BddLocalSystem->NUMBER_NODE << 1;

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Initial cost %ld\n", Cost );
  }

  for ( Iteration = 0; Iteration < MaxIteration; Iteration++ )
  {
    ChildFrom = 0;
  
    for ( ChildFrom = 0; ChildFrom < (NumberChild - 1); ChildFrom++ )
    {
      if ( IsBoomDebugLevel1() )
      {
        BoomPrintf( stdout, "-> ChildFrom %d\n", ChildFrom );
      }
  
      for ( ChildTo = ChildFrom; ChildTo < ( NumberChild - 1 ); ChildTo++ )
      {
        if ( IsBoomDebugLevel1() )
        {
          BoomPrintf( stdout, "-> Swap %d and %d\n", ChildTo, ChildTo + 1 );
        }
  
        swapbddvar( (bddsystem *)0, ChildTo );

        if ( BddLocalSystem->NUMBER_NODE >= MaxNode )
        {
          BoomPrintf( stdout, "@@@@ Max nodes %ld reached %ld\n",
                      MaxNode, BddLocalSystem->NUMBER_NODE );
          break;
        }
      }
  
      BehAux  = BoomLogicComputeAux( BehFigure, BehAux );
      NewCost = BoomBehGiveCost( BehFigure );
  
      if ( IsBoomDebugLevel0() )
      {
        BoomPrintf( stdout, "NewCost: %ld\n", NewCost );
      }
  
      if ( NewCost > Cost )
      {
        while ( ChildTo > ChildFrom )
        {
          if ( IsBoomDebugLevel1() )
          {
            BoomPrintf( stdout, "-> UnSwap %d and %d\n", ChildTo, ChildTo - 1 );
          }

          swapbddvar( (bddsystem *)0, ChildTo - 1 );

          ChildTo = ChildTo - 1;
        }
      }
      else
      {
        Cost = NewCost;
      }
    }
  }

  BehAux = BoomLogicComputeAux( BehFigure, BehAux );
  Cost   = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Final cost %ld\n", Cost );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomLogicOptimizeSimple                |
|                                                             |
\------------------------------------------------------------*/

void BoomLogicOptimizeSimple( BehFigure )

  befig_list *BehFigure;
{
  beaux_list *BehAux;
  bddvar      NumberVar;
  bddvar      BddVar;
  long        Cost;

  NumberVar = BddLocalSystem->NUMBER_VAR;

  BehAux = BoomLogicComputeAux( BehFigure, BehFigure->BEAUX );
  Cost   = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Initial cost %ld\n", Cost );
  }

  for ( BddVar = 0; BddVar < NumberVar - 1; BddVar++ )
  {
    /*
    fprintf( stdout, "%-4d\b\b\b\b", NumberVar - BddVar );
    fflush( stdout );
    */

    if ( BddVar < NumberVar - 2 )
    {
      BoomLogicReorderWindow3( BehFigure, BehAux, BddVar, &Cost );
    }
    else
    {
      BoomLogicReorderWindow2( BehFigure, BehAux, BddVar, &Cost );
    }

    if ( IsBoomDebugLevel0() )
    {
      BoomPrintf( stdout, "New cost %ld\n", Cost );
    }
  }

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Final cost %ld\n", Cost );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomLogicOptimizeRandom                |
|                                                             |
\------------------------------------------------------------*/

void BoomLogicOptimizeRandom( BehFigure )

  befig_list *BehFigure;
{
  boombefiginfo *FigInfo;
  beaux_list    *BehAux;
  long           MaxIteration;
  bddvar         NumberVar;
  bddvar         BddVar;
  int            Iteration;
  long           Cost;

  static unsigned char InitOptimizeRandom = 1;

  FigInfo = GetBoomBehFigInfo( BehFigure );
  MaxIteration = FigInfo->ITERATION;

  if ( InitOptimizeRandom )
  {
    srand( getpid() );
    InitOptimizeRandom = 0;
  }

  NumberVar = BddLocalSystem->NUMBER_VAR;

  BehAux = BoomLogicComputeAux( BehFigure, BehFigure->BEAUX );
  Cost   = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Initial cost %ld, NumberVar %d\n", Cost, NumberVar );
  }

  if ( MaxIteration <= 0 ) MaxIteration = NumberVar;

  for ( Iteration = 0; Iteration < MaxIteration; Iteration++ )
  {
    BddVar = (rand() >> 8) % (NumberVar - 1);

    if ( IsBoomDebugLevel0() )
    {
      BoomPrintf( stdout, "Swap %d\n", BddVar );
    }

    BoomLogicReorderWindow2( BehFigure, BehAux, BddVar, &Cost );

    if ( IsBoomDebugLevel0() )
    {
      BoomPrintf( stdout, "New cost %ld\n", Cost );
    }
  }

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Final cost %ld\n", Cost );
  }
}

# if 0
void BoomLogicOptimizeRandom( BehFigure )

  befig_list *BehFigure;
{
  boombefiginfo *FigInfo;
  beaux_list    *BehAux;
  long           MaxIteration;
  bddvar         NumberVar;
  bddvar         BddVar;
  int            Iteration;
  int            Amplitude;
  int            Swap;
  long           Cost;

  static unsigned char InitOptimizeRandom = 1;

  FigInfo = GetBoomBehFigInfo( BehFigure );
  MaxIteration = FigInfo->ITERATION;

  if ( InitOptimizeRandom )
  {
    srand( getpid() );
    InitOptimizeRandom = 0;
  }

  NumberVar = BddLocalSystem->NUMBER_VAR;

  /*
  BehAux = BoomLogicComputeAux( BehFigure, BehFigure->BEAUX );
  */
  Cost = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Initial cost %ld, NumberVar %d\n", Cost, NumberVar );
  }

  /*
  if ( MaxIteration <= 0 ) MaxIteration = NumberVar;
  */
  MaxIteration = 1 + ( ( NumberVar + 2 ) / 2 );

  for ( Iteration = 0; Iteration < MaxIteration; Iteration++ )
  {
    if ( Iteration != 0 )
    {
      BoomBehMakeBddNode( BehFigure, 0 );
    }

    for ( BddVar = 0; BddVar < ( NumberVar - 1 ); BddVar++ )
    {
      Amplitude = (rand() >> 8) % 8;

      for ( Swap = 0; Swap < Amplitude; Swap++ )
      {
        if ( ( BddVar + Swap ) >= ( NumberVar - 1 ) ) break;

        if ( IsBoomDebugLevel1() )
        {
          BoomPrintf( stdout, "-> Swap %d and %d\n", BddVar + Swap, BddVar + Swap + 1 );
        }
  
        swapbddvar( (bddsystem *)0, BddVar + Swap );
      }
    }

    BoomLogicOptimizeProcrast( BehFigure );
  }

  Cost = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Final cost %ld\n", Cost );
  }
}
# endif

/*------------------------------------------------------------\
|                                                             |
|                      BoomLogicOptimizeOne                   |
|                                                             |
\------------------------------------------------------------*/

void BoomLogicOptimizeOne( BehFigure )

  befig_list *BehFigure;
{
  beaux_list *BehAux;
  bddvar      NumberVar;
  bddvar      BddVar;
  long        Cost;

  reorderbddsystemwindow( BddLocalSystem );

  NumberVar = BddLocalSystem->NUMBER_VAR;

  BehAux = BoomLogicComputeAux( BehFigure, BehFigure->BEAUX );
  Cost   = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Final cost %ld\n", Cost );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomLogicOptimization                  |
|                                                             |
\------------------------------------------------------------*/

void BoomLogicOptimization( BehFigure )

  befig_list *BehFigure;
{
  boombefiginfo *FigInfo;
  bddsystem     *BddSystem;
  bddcircuit    *BddCircuit;
  beaux_list    *BehAux;

  FigInfo = GetBoomBehFigInfo( BehFigure ); 

  BoomBehCreateAuxInit( BehFigure );

  if ( FigInfo->LOGIC_METHOD == BOOM_LOGIC_METHOD_NOTHING )
  {
    return;
  }

  BddSystem = createbddsystem( 1000, 200000, 100, 3000000 );
  reorderbddsystemdynamic( BddSystem, reorderbddsystemwindow, 200000, 50 );
  /*
  explosionbddsystemdynamic( BddSystem, NULL, 50 );
  */

  BoomBehAbl2BddNode( BehFigure, FigInfo->KEEP_AUX, FigInfo->INIT_BDD_ORDER );
  reorderbddsystemdynamic( BddSystem, garbagebddsystem, 200000, 50 );

  BehAux = BehFigure->BEAUX;

  if ( IsBoomDebugLevel1() )
  {
    testbddcircuit( (bddcircuit *)0 );
  }
/*\
  while ( testbddcircuit( (bddcircuit *)0 ) )
  {
    BehAux = BoomLogicComputeAux( BehFigure, BehAux );

    Cost = BoomBehGiveCost( BehFigure );
    fprintf( stdout, "New cost %d\n", Cost );
  }
\*/

  switch ( FigInfo->LOGIC_METHOD )
  {
    case BOOM_LOGIC_METHOD_SIMPLE   : BoomLogicOptimizeSimple( BehFigure );
    break;

    case BOOM_LOGIC_METHOD_WINDOW   : BoomLogicOptimizeWindow( BehFigure );
    break;

    case BOOM_LOGIC_METHOD_TOP      : BoomLogicOptimizeTop( BehFigure );
    break;

    case BOOM_LOGIC_METHOD_ONE      : BoomLogicOptimizeOne( BehFigure );
    break;

    case BOOM_LOGIC_METHOD_PROCRAST : BoomLogicOptimizeProcrast( BehFigure );
    break;

    case BOOM_LOGIC_METHOD_BEST     : BoomLogicOptimizeBest( BehFigure );
    break;

    case BOOM_LOGIC_METHOD_RANDOM   : BoomLogicOptimizeRandom( BehFigure );
    break;
  }
/*\
  BoomBehResetBddNode( BehFigure );
\*/

  BddCircuit = BehFigure->CIRCUI;
  BehFigure->CIRCUI = (bddcircuit *)0;

  destroybddcircuit( BddCircuit );
  destroybddsystem( BddSystem );

  /*
  BoomBehDeleteUselessAux( BehFigure, (beaux_list *)0 );
  */
}
