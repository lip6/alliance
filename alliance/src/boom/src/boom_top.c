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
| File    :                   boom_top.c                      |
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
# include "boom_top.h"

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
|                      BoomBehOptimizeTop                     |
|                                                             |
\------------------------------------------------------------*/

void BoomBehOptimizeTop( BehFigure )

  befig_list *BehFigure;
{
  boombefiginfo *FigInfo;
  beaux_list    *BehAux;
  char          *ReorderLevel;
  bddvar         NumberChild;
  bddvar         BestChild;
  bddvar         ChildCount;
  bddvar         ChildTo;
  bddvar         ChildFrom;
  char           SwapReorder;
  long           Cost;
  long           NewCost;

  FigInfo = GetBoomBehFigInfo( BehFigure ); 

  NumberChild  = BddLocalSystem->NUMBER_VAR;
  ReorderLevel = (char *)autallocblock( sizeof( char ) * NumberChild );

  for ( ChildFrom = 0; ChildFrom < ( NumberChild - 1 ); ChildFrom++ )
  {
    ReorderLevel[ ChildFrom ] = 1;
  }

  BehAux = BoomBehComputeAux( BehFigure, BehFigure->BEAUX );
  Cost   = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Initial cost %ld\n", Cost );
  }

  ChildCount = ( NumberChild - 1 );
  ChildFrom  = 0;

  if ( ( FigInfo->VERBOSE ) &&
       ( ! BoomDebugFlag  ) )
  {
    fprintf( stdout, "\t--> Optimization %% 0\b" );
    fflush( stdout );
  }

  while ( ChildCount > 0 )
  {
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

      BehAux  = BoomBehComputeAux( BehFigure, BehAux );
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

    if ( ( FigInfo->VERBOSE ) &&
         ( ! BoomDebugFlag  ) )
    {
      fprintf( stdout, "%-4d\b\b\b\b", 
               ( ( NumberChild - ChildCount ) * 100 ) / ( NumberChild - 1 ) );
      fflush( stdout );
    }

    while ( ! ReorderLevel[ ChildFrom ] )
    {
      ChildFrom = ChildFrom + 1;
    }

    ChildCount = ChildCount - 1;
  }

  if ( FigInfo->VERBOSE )
  {
    fprintf( stdout, "\n\n" );
  }

  autfreeblock( ReorderLevel );

  BehAux = BoomBehComputeAux( BehFigure, BehAux );
  Cost   = BoomBehGiveCost( BehFigure );

  if ( IsBoomDebugLevel0() )
  {
    BoomPrintf( stdout, "Final cost %ld\n", Cost );
  }
}

