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
| File    :                  boom_optim.c                     |
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
# include "boom_dc.h"
# include "boom_auxinit.h"

# include "boom_window.h"
# include "boom_top.h"
# include "boom_simple.h"
# include "boom_one.h"
# include "boom_random.h"
# include "boom_procrast.h"
# include "boom_burgun.h"
# include "boom_gradient.h"
# include "boom_simul.h"
# include "boom_anneal.h"

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
|                      BoomBehOptimize                        |
|                                                             |
\------------------------------------------------------------*/

void BoomBehOptimize( BehFigure )

  befig_list *BehFigure;
{
  boombefiginfo *FigInfo;
  bddsystem     *BddSystem;
  bddcircuit    *BddCircuit;
  beaux_list    *BehAux;

  FigInfo = GetBoomBehFigInfo( BehFigure ); 

  BoomBehCreateAuxInit( BehFigure );

  if ( FigInfo->OPTIM_ALGORITHM == BOOM_OPTIM_ALGORITHM_NOTHING )
  {
    return;
  }

  BddSystem = createbddsystem( 1000, 20000, 100, 10000000 );

  if ( ! BoomDebugFlag )
  {
    SetBddSystemNoWarning( BddSystem );
  }

  reorderbddsystemdynamic( BddSystem, reorderbddsystemwindow, 100000, 50 );
  /*
  explosionbddsystemdynamic( BddSystem, NULL, 50 );
  */

  if ( FigInfo->VERBOSE )
  {
    fprintf( stdout, "\t--> Translate Abl to Bdd\n" );
  }

  BoomBehAbl2BddNode( BehFigure, FigInfo->INIT_BDD_ORDER, FigInfo->DONT_CARE );
  reorderbddsystemdynamic( BddSystem, garbagebddsystem, 100000, 50 );

  if ( FigInfo->VERBOSE )
  {
    fprintf( stdout, "\t    Total Bdd nodes %ld\n\n", BddSystem->NUMBER_NODE );
  }

  if ( FigInfo->DONT_CARE )
  {
    if ( FigInfo->VERBOSE )
    {
      fprintf( stdout, "\t    Optimize don't care\n" );
    }

    BoomOptimDontCare( BehFigure );

    if ( FigInfo->VERBOSE )
    {
      fprintf( stdout, "\t    Total Bdd nodes %ld\n\n", BddSystem->NUMBER_NODE );
    }
  }

  BehAux = BehFigure->BEAUX;

/*\
  while ( testbddcircuit( (bddcircuit *)0 ) )
  {
    BehAux = BoomOptimComputeAux( BehFigure, BehAux );

    Cost = BoomBehGiveCost( BehFigure );
    fprintf( stdout, "New cost %d\n", Cost );
  }
\*/

  switch ( FigInfo->OPTIM_ALGORITHM )
  {
    case BOOM_OPTIM_ALGORITHM_SIMPLE   : BoomBehOptimizeSimple( BehFigure );
    break;

    case BOOM_OPTIM_ALGORITHM_WINDOW   : BoomBehOptimizeWindow( BehFigure );
    break;

    case BOOM_OPTIM_ALGORITHM_TOP      : BoomBehOptimizeTop( BehFigure );
    break;

    case BOOM_OPTIM_ALGORITHM_ONE      : BoomBehOptimizeOne( BehFigure );
    break;

    case BOOM_OPTIM_ALGORITHM_PROCRAST : BoomBehOptimizeProcrast( BehFigure );
    break;

    case BOOM_OPTIM_ALGORITHM_BURGUN   : BoomBehOptimizeBurgun( BehFigure );
    break;

    case BOOM_OPTIM_ALGORITHM_RANDOM   : BoomBehOptimizeRandom( BehFigure );
    break;

    case BOOM_OPTIM_ALGORITHM_GRADIENT : BoomBehOptimizeGradient( BehFigure );
    break;

    case BOOM_OPTIM_ALGORITHM_SIMUL    : BoomBehOptimizeSimul( BehFigure );
    break;

    case BOOM_OPTIM_ALGORITHM_ANNEAL   : BoomBehOptimizeAnneal( BehFigure );
    break;
  }

  BoomBehResetBddNode( BehFigure );

  BddCircuit = BehFigure->CIRCUI;
  BehFigure->CIRCUI = (bddcircuit *)0;

  destroybddcircuit( BddCircuit );
  destroybddsystem( BddSystem );

  for ( BehAux  = BehFigure->BEAUX;
        BehAux != (beaux_list *)0;
        BehAux  = BehAux->NEXT )
  {
    ClearBoomBehAuxKeep( BehAux );
    ClearBoomBehAuxDontTouch( BehAux );
  }

  BoomBehDeleteUselessAux( BehFigure, (beaux_list *)0, 1 );

  BoomBehMapAbl( BehFigure, simpablexpr, 1 );
}
