/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     BOOM                          |
|                                                             |
| File    :                  boom_window.c                    |
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

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "abe.h"
# include "abv.h"
# include "abt.h"

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
# include "boom_window.h"

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
|                     Boom Beh Reorder Window 2               |
|                                                             |
\------------------------------------------------------------*/

int BoomBehReorderWindow2( BehFigure, BehAux, BddVar, PCost )

  befig_list *BehFigure;
  beaux_list *BehAux;
  bddvar      BddVar;
  long       *PCost;
{
  long NewCost;

  swapbddvar( (bddsystem *)0, BddVar );

//BehAux  = BoomBehComputeAux( BehFigure, BehAux );
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
|                 Boom Beh Reorder  Window 3                  |
|                                                             |
\------------------------------------------------------------*/

int BoomBehReorderWindow3( BehFigure, BehAux, BddVar, PCost )

  befig_list *BehFigure;
  beaux_list *BehAux;
  bddvar      BddVar;
  long       *PCost;
{
  long Cost;
  long NewCost;
  int  BestOrder;

  BestOrder = BOOM_WINDOW_REORDER_123;
  Cost      = *PCost;
  NewCost   = Cost;

  if ( IsBoomDebugLevel1() )
  {
     BoomPrintf( stdout, "Window3: Initial cost %ld\n", Cost );
  }

  swapbddvar( (bddsystem *)0, BddVar );

//BehAux  = BoomBehComputeAux( BehFigure, BehAux );
  NewCost = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "NewCost: %ld\n", NewCost );
  }

  if ( NewCost < Cost )
  {
    Cost      = NewCost;
    BestOrder = BOOM_WINDOW_REORDER_213;

    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "Window3: 213 new cost %ld\n", Cost );
    }
  }

  swapbddvar( (bddsystem *)0, BddVar );

//BehAux  = BoomBehComputeAux( BehFigure, BehAux );
  NewCost = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "NewCost: %ld\n", NewCost );
  }

  if ( NewCost < Cost )
  {
    Cost      = NewCost;
    BestOrder = BOOM_WINDOW_REORDER_231;

    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "Window3: 231 new cost %ld\n", Cost );
    }
  }

  swapbddvar( (bddsystem *)0, BddVar );

//BehAux  = BoomBehComputeAux( BehFigure, BehAux );
  NewCost = BoomBehGiveCost( BehFigure );
  
  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "NewCost: %ld\n", NewCost );
  }
  
  if ( NewCost < Cost )
  {
    Cost      = NewCost;
    BestOrder = BOOM_WINDOW_REORDER_321;
  
    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "Window3: 321 new cost %ld\n", Cost );
    }
  }

  swapbddvar( (bddsystem *)0, BddVar );

//BehAux  = BoomBehComputeAux( BehFigure, BehAux );
  NewCost = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "NewCost: %ld\n", NewCost );
  }

  if ( NewCost < Cost )
  {
    Cost      = NewCost;
    BestOrder = BOOM_WINDOW_REORDER_312;

    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "Window3: 312 new cost %ld\n", Cost );
    }
  }

  swapbddvar( (bddsystem *)0, BddVar );

//BehAux  = BoomBehComputeAux( BehFigure, BehAux );
  NewCost = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "NewCost: %ld\n", NewCost );
  }

  if ( NewCost < Cost )
  {
    Cost      = NewCost;
    BestOrder = BOOM_WINDOW_REORDER_132;

    if ( IsBoomDebugLevel1() )
    {
      BoomPrintf( stdout, "Window3: 132 new cost %ld\n", Cost );
    }
  }

  switch ( BestOrder )
  {
    case BOOM_WINDOW_REORDER_123 : swapbddvar( (bddsystem *)0, BddVar );
    break;

    case BOOM_WINDOW_REORDER_213 : swapbddvar( (bddsystem *)0, BddVar );
                                   swapbddvar( (bddsystem *)0, BddVar );
    break;

    case BOOM_WINDOW_REORDER_231 : swapbddvar( (bddsystem *)0, BddVar );
                                   swapbddvar( (bddsystem *)0, BddVar );
                                   swapbddvar( (bddsystem *)0, BddVar );
    break;

    case BOOM_WINDOW_REORDER_321 : swapbddvar( (bddsystem *)0, BddVar );
                                   swapbddvar( (bddsystem *)0, BddVar );
    break;

    case BOOM_WINDOW_REORDER_312 : swapbddvar( (bddsystem *)0, BddVar );
    break;
  }

  *PCost = Cost;

  return( BestOrder != BOOM_WINDOW_REORDER_123 );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomBehOptimizeWindow                  |
|                                                             |
\------------------------------------------------------------*/

void BoomBehOptimizeWindow( BehFigure )

  befig_list *BehFigure;
{
  boombefiginfo *FigInfo;
  beaux_list    *BehAux;
  short         *ReorderLevel;
  bddvar         NumberChild;
  bddvar         ChildCount;
  bddvar         ChildFrom;
  int            AnySwap;
  int            Moved;
  long           Cost;

  FigInfo = GetBoomBehFigInfo( BehFigure ); 

  NumberChild  = BddLocalSystem->NUMBER_VAR;
  ReorderLevel = (short *)autallocblock( sizeof( short ) * NumberChild );

  for ( ChildFrom = 0; ChildFrom < ( NumberChild - 1 ); ChildFrom++ )
  {
    ReorderLevel[ ChildFrom ] = 1;
  }

  BehAux = BoomBehComputeAux( BehFigure, BehFigure->BEAUX );
  Cost   = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel1() )
  {
    BoomPrintf( stdout, "Initial cost %ld\n", Cost );
  }

  if ( ( FigInfo->VERBOSE ) &&
       ( ! BoomDebugFlag  ) )
  {
    fprintf( stdout, "\t--> Optimization %% 0\b" );
    fflush( stdout );
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
          Moved = BoomBehReorderWindow3( BehFigure, BehAux, ChildCount, &Cost );
        }
        else
        {
          Moved = BoomBehReorderWindow2( BehFigure, BehAux, ChildCount, &Cost );
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

      if ( ( FigInfo->VERBOSE ) &&
         ( ! BoomDebugFlag  ) )
      {
        fprintf( stdout, "%-3d\b\b\b", ( ( ChildCount + 1 ) * 100 ) / ( NumberChild - 1 ) );
        fflush( stdout );
      }
    }
  }
  while ( AnySwap );

  if ( FigInfo->VERBOSE )
  {
    fprintf( stdout, "\n\n" );
  }

  autfreeblock( ReorderLevel );

  BehAux = BoomBehComputeAux( BehFigure, BehAux );
  Cost   = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel1() )
  {
    BoomPrintf( stdout, "Final cost %ld\n", Cost );
  }
}
