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
| File    :                  boom_procrast.c                  |
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
# include "boom_optim.h"

# include "boom_window.h"
# include "boom_procrast.h"

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
|                      BoomBehOptimizeProcrast                |
|                                                             |
\------------------------------------------------------------*/

void BoomBehOptimizeProcrast( BehFigure )

  befig_list *BehFigure;
{
  beaux_list    *BehAux;
  boombefiginfo *FigInfo;
  bddvar         NumberChild;
  bddvar         ChildTo;
  bddvar         ChildFrom;
  long           Cost;
  long           NewCost;
  long           NumberNode;
  long           NewNumberNode;
  long           LastNewNumberNode;
  long           Iteration;
  long           MaxIteration;
  int            Level;       
  long           MaxNode;

  FigInfo = GetBoomBehFigInfo( BehFigure );
  MaxIteration = FigInfo->ITERATION;
  Level        = FigInfo->OPTIM_LEVEL;

  if ( MaxIteration <= 0 ) MaxIteration = 2;

  MaxIteration = MaxIteration * (Level + 1);

  NumberChild = BddLocalSystem->NUMBER_VAR;

  BehAux = BoomBehComputeAux( BehFigure, BehFigure->BEAUX );
  Cost   = BoomBehGiveCost( BehFigure );

  NumberNode = BoomBehGetNumberBigBddNode( BehFigure );

  MaxNode = ( BddLocalSystem->NUMBER_NODE * 120 ) / 100;

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Initial cost %ld %ld\n", Cost, NumberNode );
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

        /*
        NewNumberNode = BoomBehGetNumberBigBddNode( BehFigure );

        BoomPrintf( stdout, "NewNumberNode: %ld\n", NewNumberNode );
        */

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

      NewNumberNode = BoomBehGetNumberBigBddNode( BehFigure );

      if ( IsBoomDebugLevel0() )
      {
        BoomPrintf( stdout, "NewNumberNode: %ld\n", NewNumberNode );
      }
  
      /*
      if ( NewNumberNode <= NumberNode )
      */
      if ( ( NewNumberNode != NumberNode                 ) &&
           ( NewNumberNode != LastNewNumberNode          ) &&
           ( NewNumberNode <= ( NumberNode * 105 ) / 100 ) )
      {
        if ( NewNumberNode < NumberNode ) NumberNode = NewNumberNode;

        BehAux     = BoomBehComputeAux( BehFigure, BehAux );
        NewCost    = BoomBehGiveCost( BehFigure );
  
        if ( IsBoomDebugLevel0() )
        {
          BoomPrintf( stdout, "NewCost: %ld %ld\n", NewCost, NumberNode );
        }
      }
      else
      {
        NewCost = Cost + 1;
      }

      LastNewNumberNode = NewNumberNode;

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

  BehAux = BoomBehComputeAux( BehFigure, BehAux );
  Cost   = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Final cost %ld\n", Cost );
  }
}

