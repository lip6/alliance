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
| File    :                  boom_anneal.c                    |
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
# include "boom_anneal.h"


#ifndef AUTO_HAS_DRAND48
/* The  drand48() function return non-negative double-precision 
 * floating-point values uniformly distributed between [0.0, 1.0).
 */
#ifndef RAND_MAX 
# define RAND_MAX   (unsigned long)( (1L<<31) - 1)
# endif
#define drand48()       ((double) rand() / (double) (RAND_MAX + 1.0))
#endif

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
|                      BoomDupBddOrder                        |
|                                                             |
\------------------------------------------------------------*/

static chain_list *BoomDupBddOrder( BddOrder )

  chain_list *BddOrder;
{
  chain_list  *DupBddOrder;
  chain_list  *ScanChain;
  chain_list  *DupChain;
  chain_list **PrevBddOrder;

  DupBddOrder = (chain_list *)0;
  PrevBddOrder = &DupBddOrder;

  for ( ScanChain  = BddOrder;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    DupChain = addchain( (chain_list *)0, ScanChain->DATA );
    *PrevBddOrder = DupChain;
    PrevBddOrder  = &DupChain->NEXT;
  }

  return( DupBddOrder );
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomBehOptimizeAnneal                  |
|                                                             |
\------------------------------------------------------------*/

void BoomBehOptimizeAnneal( BehFigure )

  befig_list *BehFigure;
{
  beaux_list    *BehAux;
  boombefiginfo *FigInfo;
  long           Cost;
  double         Proba;
  double         MaxProba;
  long           NewCost;
  long           BestCost;
  long           Iteration;
  long           Amplitude;
  long           MaxIteration;
  long           MaxAmplitude;
  chain_list    *AnnealOrder;
  chain_list    *NewOrder;
  chain_list    *BestOrder;
  int            Level;       

  FigInfo = GetBoomBehFigInfo( BehFigure );
  MaxIteration = FigInfo->ITERATION;
  MaxAmplitude = FigInfo->AMPLITUDE;
  Level        = FigInfo->OPTIM_LEVEL;

  if ( MaxIteration <= 0 ) MaxIteration = 100;
  /*
  if ( MaxAmplitude <= 0 ) MaxAmplitude = 1 + (BddLocalSystem->NUMBER_VAR / 8);
  */
  /*
  if ( MaxAmplitude <= 0 ) MaxAmplitude = 1 + (BddLocalSystem->NUMBER_VAR / 2);
  */
  if ( MaxAmplitude <= 0 ) MaxAmplitude = 5;

  MaxIteration = MaxIteration * (Level + 1);

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Compute initial cost !\n" );
  }

  BehAux     = BoomBehComputeAux( BehFigure, BehFigure->BEAUX );
  Cost       = BoomBehGiveCost( BehFigure );
  /*
  AnnealOrder  = BoomBehGetBddOrder( BehFigure );
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
    AnnealOrder = BoomBehGradientBddOrder( BehFigure );
  }
  else
  {
    AnnealOrder = BoomBehGetBddOrder( BehFigure );
  }

  BestCost   = Cost;
  BestOrder  = (chain_list *)0;

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
    Amplitude = MaxAmplitude; 
    /*
    Amplitude = 1 + (long)( (double)MaxAmplitude * 
                           exp( - ( (double)Iteration * 8.0 ) / (double)MaxIteration ) );
    */

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

    MaxProba = exp( - (double)Iteration / 5.0 );
    Proba    = drand48();

    if ( IsBoomDebugLevel0() )
    {
      BoomPrintf( stdout, "Proba : %g %g\n", Proba, MaxProba );
    }

    if ( NewCost < BestCost )
    {
      BestCost  = NewCost;

      freechain( BestOrder );
      BestOrder = BoomDupBddOrder( AnnealOrder );
    }

    if ( ( Proba   <  MaxProba ) ||
         ( NewCost <  Cost     ) )
    {
      if ( IsBoomDebugLevel0() )
      {
        BoomPrintf( stdout, "Accept new solution %ld (gain %ld)\n", NewCost, Cost - NewCost );
      }

      Cost = NewCost;

      freechain( AnnealOrder );
      AnnealOrder = NewOrder;
    }
    else
    {
      BoomBehRebuildBddNode( BehFigure, AnnealOrder );
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
    BoomPrintf( stdout, "Anneal cost : %ld\n", Cost );
  }

  if ( ( BestCost < Cost              ) &&
       ( BestOrder != (chain_list *)0 ) )
  {
    freechain( AnnealOrder );
    AnnealOrder = BestOrder;
    BestOrder   = (chain_list *)0;
  }
  else
  {
    freechain( BestOrder );
    BestOrder = (chain_list *)0;
  }

  BoomBehRebuildBddNode( BehFigure, AnnealOrder );

  BehAux = BoomBehComputeAux( BehFigure, BehAux );
  Cost   = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Final cost %ld\n", Cost );
  }
}
