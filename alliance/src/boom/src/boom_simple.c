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
| File    :                  boom_simple.c                    |
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
# include "boom_simple.h"

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
|                      BoomBehOptimizeSimple                  |
|                                                             |
\------------------------------------------------------------*/

void BoomBehOptimizeSimple( BehFigure )

  befig_list *BehFigure;
{
  boombefiginfo *FigInfo;
  beaux_list    *BehAux;
  bddvar         NumberVar;
  bddvar         BddVar;
  long           Cost;

  FigInfo = GetBoomBehFigInfo( BehFigure ); 

  NumberVar = BddLocalSystem->NUMBER_VAR;

  BehAux = BoomBehComputeAux( BehFigure, BehFigure->BEAUX );
  Cost   = BoomBehGiveCost( BehFigure );

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

  for ( BddVar = 0; BddVar < NumberVar - 1; BddVar++ )
  {
    if ( BddVar < NumberVar - 2 )
    {
      BoomBehReorderWindow3( BehFigure, BehAux, BddVar, &Cost );
    }
    else
    {
      BoomBehReorderWindow2( BehFigure, BehAux, BddVar, &Cost );
    }

    if ( IsBoomDebugLevel0() )
    {
      BoomPrintf( stdout, "New cost %ld\n", Cost );
    }

    if ( ( FigInfo->VERBOSE ) &&
         ( ! BoomDebugFlag  ) )
    {
      fprintf( stdout, "%-3d\b\b\b", ( ( BddVar + 1 ) * 100 ) / ( NumberVar - 1 ) );
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
