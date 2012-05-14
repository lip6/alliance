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
| File    :                  boom_simul.c                     |
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
# include <math.h>

# include "boom_error.h"
# include "boom_debug.h"
# include "boom_shared.h"
# include "boom_aux.h"
# include "boom_abl.h"
# include "boom_bdd.h"
# include "boom_cost.h"
# include "boom_optim.h"

# include "boom_gradient.h"
# include "boom_simul.h"

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
|                      BoomBehOptimizeSimul                    |
|                                                             |
\------------------------------------------------------------*/

void BoomBehOptimizeSimul( BehFigure )

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
  chain_list    *SimulOrder;
  chain_list    *NewOrder;
  int            Level;       

  FigInfo = GetBoomBehFigInfo( BehFigure );
  MaxIteration = FigInfo->ITERATION;
  MaxAmplitude = FigInfo->AMPLITUDE;
  Level        = FigInfo->OPTIM_LEVEL;

  if ( MaxIteration <= 0 ) MaxIteration = 50;
  if ( MaxAmplitude <= 0 ) MaxAmplitude = 1 + (BddLocalSystem->NUMBER_VAR / 4);

  MaxIteration = MaxIteration * (Level + 1);

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Compute initial cost !\n" );
  }

  BehAux     = BoomBehComputeAux( BehFigure, BehFigure->BEAUX );
  Cost       = BoomBehGiveCost( BehFigure );
  /*
  SimulOrder  = BoomBehGetBddOrder( BehFigure );
  */

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Initial cost %ld\n", Cost );
  }

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Compute initial order\n" );
  }

  if ( Level >= 1 )
  {
    SimulOrder = BoomBehGradientBddOrder( BehFigure );
  }
  else
  {
    SimulOrder = BoomBehGetBddOrder( BehFigure );
  }

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "done\n" );
  }

  if ( ( FigInfo->VERBOSE ) &&
       ( ! BoomDebugFlag  ) )
  {
    fprintf( stdout, "\t--> Optimization %% 0\b" );
    fflush( stdout );
  }

  for ( Iteration = 0; Iteration < MaxIteration; Iteration++ )
  {
    Amplitude = 1 + (long)( (double)MaxAmplitude * exp( - (double)Iteration / 5.0 ) );

    if ( IsBoomDebugLevel0() )
    {
      BoomPrintf( stdout, "Iteration %ld, Amplitude %ld\n", Iteration, Amplitude );
    }

    BoomBehRandomBddOrder( BehFigure, Amplitude );

    NewOrder = BoomBehGetBddOrder( BehFigure );
  
    BehAux  = BoomBehComputeAux( BehFigure, BehAux );
    NewCost = BoomBehGiveCost( BehFigure );
  
    if ( IsBoomDebugLevel0() )
    {
      BoomPrintf( stdout, "NewCost: %ld\n", NewCost );
    }

    if ( NewCost < Cost )
    {
      Cost = NewCost;

      freechain( SimulOrder );
      SimulOrder = NewOrder;
    }
    else
    {
      BoomBehRebuildBddNode( BehFigure, SimulOrder );
      freechain( NewOrder );
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
    BoomPrintf( stdout, "Simul cost : %ld\n", Cost );
  }

  BoomBehRebuildBddNode( BehFigure, SimulOrder );

  BehAux = BoomBehComputeAux( BehFigure, BehAux );
  Cost   = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Final cost %ld\n", Cost );
  }
}
