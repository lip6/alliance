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
| File    :                  boom_random.c                    |
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

# include "boom_window.h"
# include "boom_random.h"

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
|                      BoomBehOptimizeRandom                  |
|                                                             |
\------------------------------------------------------------*/

void BoomBehOptimizeRandom( BehFigure )

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

  BehAux = BoomBehComputeAux( BehFigure, BehFigure->BEAUX );
  Cost   = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Initial cost %ld, NumberVar %d\n", Cost, NumberVar );
  }

  if ( MaxIteration <= 0 ) MaxIteration = NumberVar;

  if ( ( FigInfo->VERBOSE ) &&
       ( ! BoomDebugFlag  ) )
  {
    fprintf( stdout, "\t--> Optimization %% 0\b" );
    fflush( stdout );
  }

  for ( Iteration = 0; Iteration < MaxIteration; Iteration++ )
  {
    BddVar = (rand() >> 8) % (NumberVar - 1);

    if ( IsBoomDebugLevel0() )
    {
      BoomPrintf( stdout, "Swap %d\n", BddVar );
    }

    BoomBehReorderWindow2( BehFigure, BehAux, BddVar, &Cost );

    if ( IsBoomDebugLevel0() )
    {
      BoomPrintf( stdout, "New cost %ld\n", Cost );
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

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Final cost %ld\n", Cost );
  }
}

# if 0
void BoomBehOptimizeRandom( BehFigure )

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
  BehAux = BoomBehComputeAux( BehFigure, BehFigure->BEAUX );
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

    BoomOptimizeProcrast( BehFigure );
  }

  Cost = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Final cost %ld\n", Cost );
  }
}
# endif

